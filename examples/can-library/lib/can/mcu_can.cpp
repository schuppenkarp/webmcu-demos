/* WebMCU CAN 2.0.0 | MIT. RM0440: G431 fixed FDCAN message RAM layout.
   No HAL, heap, IRQ handler or user-managed filter tables. */
#include "mcu_can.h"
enum {
  StandardWords=28, ExtendedWords=8*2, ElementWords=18,
  Rx0Word=StandardWords+ExtendedWords,
  TxWord=Rx0Word+3*ElementWords+3*ElementWords+3*2,
  RamWords=TxWord+3*ElementWords
};
#define MESSAGE_RAM ((volatile uint32_t *)SRAMCAN_BASE)
#define ELEMENT_EXTENDED (1u<<30)
#define ELEMENT_REMOTE (1u<<29)
#define ELEMENT_FD (1u<<21)
static bool ready(FDCAN_GlobalTypeDef *can) {
  return can==FDCAN1 && !(can->CCCR & FDCAN_CCCR_INIT);
}
static bool waitBit(volatile uint32_t *reg, uint32_t mask, bool set) {
  uint32_t budget=100000;
  while(((*reg & mask)!=0)!=set)if(!budget--)return false;
  return true;
}
static bool timing(uint32_t hz, uint32_t rate, uint32_t *nbtp) {
  if(!hz || hz>170000000 || !rate || rate>1000000 || hz%rate) return false;
  uint32_t clocks=hz/rate;
  /* Prefer many time quanta, approximately 80% sample point. Exact bitrate only. */
  for(uint32_t tq=40;tq>=8;--tq) {
    if(clocks%tq)continue;
    uint32_t prescaler=clocks/tq, seg2=tq/5, seg1=tq-1-seg2;
    if(!prescaler || prescaler>512)continue;
    uint32_t sjw=seg2<4 ? seg2 : 4;
    *nbtp=((prescaler-1)<<FDCAN_NBTP_NBRP_Pos) |
          ((seg1-1)<<FDCAN_NBTP_NTSEG1_Pos) |
          ((seg2-1)<<FDCAN_NBTP_NTSEG2_Pos) |
          ((sjw-1)<<FDCAN_NBTP_NSJW_Pos);
    return true;
  }
  return false;
}
bool CAN::begin(uint32_t hz, uint32_t bitrate, Mode mode) {
  uint32_t nbtp;
  if(can!=FDCAN1 || (mode!=Normal && mode!=InternalLoopback) || !timing(hz,bitrate,&nbtp))return false;
  RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN; (void)RCC->APB1ENR1;
  RCC->CCIPR=(RCC->CCIPR & ~RCC_CCIPR_FDCANSEL) | RCC_CCIPR_FDCANSEL_1;
  RCC->APB1RSTR1 |= RCC_APB1RSTR1_FDCANRST;
  RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_FDCANRST;
  can->CCCR &= ~FDCAN_CCCR_CSR;
  if(!waitBit(&can->CCCR,FDCAN_CCCR_CSA,false))return false;
  can->CCCR |= FDCAN_CCCR_INIT;
  if(!waitBit(&can->CCCR,FDCAN_CCCR_INIT,true))return false;
  can->CCCR |= FDCAN_CCCR_CCE;
  FDCAN_CONFIG->CKDIV=0;
  can->CCCR=FDCAN_CCCR_INIT|FDCAN_CCCR_CCE|
    (mode==InternalLoopback ? FDCAN_CCCR_TEST|FDCAN_CCCR_MON : 0);
  if(mode==InternalLoopback)can->TEST=FDCAN_TEST_LBCK;
  can->NBTP=nbtp;
  can->RXGFC=0; /* zero filters, all nonmatching frames to FIFO0, blocking FIFO */
  can->XIDAM=0x1fffffffu;
  can->TXBC=0; /* FIFO mode */
  can->IE=0;can->ILE=0;can->TXBTIE=0;can->TXBCIE=0;
  can->IR=0x3fffffffu;
  for(unsigned i=0;i<RamWords;i++)MESSAGE_RAM[i]=0;
  __DSB();
  can->CCCR &= ~FDCAN_CCCR_INIT;
  if(!waitBit(&can->CCCR,FDCAN_CCCR_INIT,false)){
    can->CCCR |= FDCAN_CCCR_INIT;return false;
  }
  return true;
}
bool CAN::isBusOff() const {
  return can==FDCAN1 && (can->PSR & FDCAN_PSR_BO)!=0;
}
bool CAN::takeOverflow() {
  if(can!=FDCAN1)return false;
  bool lost=(can->IR & FDCAN_IR_RF0L)!=0;
  if(lost)can->IR=FDCAN_IR_RF0L;
  return lost;
}
CAN::Result CAN::send(const Message &message) {
  const Message *m = &message;
  if(can!=FDCAN1 || m->length>8 || m->id>(m->extended?0x1fffffffu:0x7ffu))return Invalid;
  if(isBusOff())return BusOff;
  if(!ready(can))return Invalid;
  uint32_t state=can->TXFQS;
  if(state & FDCAN_TXFQS_TFQF)return WouldBlock;
  unsigned index=(state & FDCAN_TXFQS_TFQPI)>>FDCAN_TXFQS_TFQPI_Pos;
  if(index>=3)return Invalid;
  volatile uint32_t *slot=MESSAGE_RAM+TxWord+index*ElementWords;
  slot[0]=(m->extended ? m->id|ELEMENT_EXTENDED : m->id<<18) | (m->remote?ELEMENT_REMOTE:0);
  slot[1]=(uint32_t)m->length<<16;
  for(unsigned word=0;word<2;word++){
    uint32_t value=0;
    for(unsigned b=0;b<4;b++){
      unsigned i=word*4+b;
      if(!m->remote && i<m->length)value|=(uint32_t)m->data[i]<<(b*8);
    }
    slot[2+word]=value;
  }
  __DMB();can->TXBAR=1u<<index;
  return Ok;
}
CAN::Result CAN::receive(Message &message) {
  Message *m = &message;
  if(can!=FDCAN1)return Invalid;
  if(isBusOff())return BusOff;
  if(!ready(can))return Invalid;
  uint32_t state=can->RXF0S;
  if(!(state & FDCAN_RXF0S_F0FL))return WouldBlock;
  unsigned index=(state & FDCAN_RXF0S_F0GI)>>FDCAN_RXF0S_F0GI_Pos;
  if(index>=3)return Invalid;
  volatile uint32_t *slot=MESSAGE_RAM+Rx0Word+index*ElementWords;
  uint32_t r0=slot[0],r1=slot[1];
  if(r1 & ELEMENT_FD){can->RXF0A=index;return Unsupported;}
  m->extended=(r0 & ELEMENT_EXTENDED)!=0;m->remote=(r0 & ELEMENT_REMOTE)!=0;
  m->id=m->extended ? r0 & 0x1fffffffu : (r0>>18)&0x7ffu;
  unsigned dlc=(r1>>16)&15u;m->length=(uint8_t)(dlc>8 ? 8 : dlc);
  for(unsigned word=0;word<2;word++){
    uint32_t value=slot[2+word];
    for(unsigned b=0;b<4;b++){
      unsigned i=word*4+b;
      m->data[i]=(!m->remote && i<m->length) ? (uint8_t)(value>>(b*8)) : 0;
    }
  }
  __DMB();can->RXF0A=index;
  return Ok;
}
