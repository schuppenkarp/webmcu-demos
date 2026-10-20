/* WebMCU UART 1.0.0 | MIT. Standalone: does not depend on mcu_gpio. */
#include "mcu_uart.h"
static bool valid(USART_TypeDef *u) {
  return u==USART1 || u==USART2 || u==USART3 || u==UART4 || u==LPUART1;
}
bool UART_Init(USART_TypeDef *u, uint32_t hz, uint32_t baud) {
  if (!valid(u) || !hz || !baud || baud>hz || hz > 170000000u) return false;
  uint64_t scaled=(uint64_t)hz * (u==LPUART1 ? 256u : 1u);
  uint32_t brr=hz/baud, remainder=hz%baud;
  if(u==LPUART1) {
    if(brr>4095)return false;
    /* Fractional division without a 64-bit division runtime dependency. */
    for(unsigned bit=0;bit<8;bit++){
      brr*=2;remainder*=2;
      if(remainder>=baud){remainder-=baud;++brr;}
    }
  }
  if(remainder >= (baud+1u)/2u)++brr;
  if (u==LPUART1 ? (brr<0x300 || brr>0xfffff) : (brr<16 || brr>0xffff)) return false;
  /* Reject baud rounding errors over 2%. */
  uint64_t expected=(uint64_t)baud*brr;
  uint64_t error=scaled>expected ? scaled-expected : expected-scaled;
  if(error*100u > expected*2u) return false;
  uint32_t selection;
  if(u==USART1){RCC->APB2ENR|=RCC_APB2ENR_USART1EN;(void)RCC->APB2ENR;selection=RCC_CCIPR_USART1SEL;}
  else if(u==USART2){RCC->APB1ENR1|=RCC_APB1ENR1_USART2EN;(void)RCC->APB1ENR1;selection=RCC_CCIPR_USART2SEL;}
  else if(u==USART3){RCC->APB1ENR1|=RCC_APB1ENR1_USART3EN;(void)RCC->APB1ENR1;selection=RCC_CCIPR_USART3SEL;}
  else if(u==UART4){RCC->APB1ENR1|=RCC_APB1ENR1_UART4EN;(void)RCC->APB1ENR1;selection=RCC_CCIPR_UART4SEL;}
  else {RCC->APB1ENR2|=RCC_APB1ENR2_LPUART1EN;(void)RCC->APB1ENR2;selection=RCC_CCIPR_LPUART1SEL;}
  u->CR1=0; RCC->CCIPR &= ~selection;
  u->CR2=0;u->CR3=0;u->PRESC=0;u->BRR=brr;
  u->ICR=USART_ICR_ORECF|USART_ICR_NECF|USART_ICR_FECF|USART_ICR_PECF|USART_ICR_TCCF;
  u->CR1=USART_CR1_UE|USART_CR1_TE|USART_CR1_RE;
  uint32_t budget=100000;
  while((u->ISR & (USART_ISR_TEACK|USART_ISR_REACK)) != (USART_ISR_TEACK|USART_ISR_REACK)) {
    if(!budget--){u->CR1=0;return false;}
  }
  return true;
}
UART_Result UART_TryWrite(USART_TypeDef *u, uint8_t byte) {
  if(!valid(u) || !(u->CR1 & USART_CR1_UE)) return UART_Invalid;
  if(!(u->ISR & USART_ISR_TXE_TXFNF)) return UART_WouldBlock;
  u->TDR=byte;return UART_Ok;
}
UART_Result UART_TryRead(USART_TypeDef *u, uint8_t *byte) {
  if(!valid(u) || !byte || !(u->CR1 & USART_CR1_UE)) return UART_Invalid;
  uint32_t status=u->ISR;
  if(status & (USART_ISR_ORE|USART_ISR_NE|USART_ISR_FE|USART_ISR_PE)) {
    if(status & USART_ISR_RXNE_RXFNE)(void)u->RDR;
    u->ICR=USART_ICR_ORECF|USART_ICR_NECF|USART_ICR_FECF|USART_ICR_PECF;
    return UART_Error;
  }
  if(!(status & USART_ISR_RXNE_RXFNE))return UART_WouldBlock;
  *byte=(uint8_t)u->RDR;return UART_Ok;
}
UART_Result UART_Write(USART_TypeDef *u, const void *data, size_t size, uint32_t budget) {
  if(!valid(u) || (!data && size)) return UART_Invalid;
  const uint8_t *bytes=(const uint8_t *)data;
  for(size_t i=0;i<size;) {
    UART_Result r=UART_TryWrite(u,bytes[i]);
    if(r==UART_Ok)++i;
    else if(r!=UART_WouldBlock)return r;
    else if(!budget--)return UART_Timeout;
  }
  return UART_Ok;
}
UART_Result UART_Print(USART_TypeDef *u, const char *text, uint32_t budget) {
  if(!text)return UART_Invalid;
  size_t size=0;while(text[size])++size;
  return UART_Write(u,text,size,budget);
}
UART_Result UART_Flush(USART_TypeDef *u, uint32_t budget) {
  if(!valid(u) || !(u->CR1 & USART_CR1_UE))return UART_Invalid;
  while(!(u->ISR & USART_ISR_TC))if(!budget--)return UART_Timeout;
  return UART_Ok;
}
