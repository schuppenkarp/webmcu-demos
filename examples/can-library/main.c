/* CAN-Bibliothek: startet im internen Loopback, ohne CAN-Verkabelung.
   Für echten Bus LOOPBACK auf 0 setzen. PB8 RX / PB9 TX, AF9,
   externer CAN-Transceiver, Terminierung und zweiter ACK-Knoten nötig. */
#include "mcu_gpio.h"
#include "mcu_can.h"
#define LOOPBACK 1
volatile bool can_ready = false;
volatile uint32_t sent_frames = 0, received_frames = 0, receive_overflows = 0;
volatile CAN_Result last_result = CAN_Ok;
CAN_Message received;
int main(void) {
  PeripheralClockEnable(GPIOB);
  GPIO_Write(GPIOB, 1, false);
  GPIO_Mode(GPIOB, 1, PushPull);
#if !LOOPBACK
  GPIO_Alternate(GPIOB, 8, 9);
  GPIO_Alternate(GPIOB, 9, 9);
#endif
  can_ready = CAN_Init(FDCAN1, 16000000, 500000,
                      LOOPBACK ? CAN_InternalLoopback : CAN_Normal);
  if (!can_ready) for (;;) __NOP();
  SysTick->LOAD = 16000 - 1; /* 1 ms, Polling statt Interrupt */
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
  uint32_t milliseconds = 0;
  CAN_Message message = {0};
  message.id = 0x123; message.length = 2; message.data[1] = 0x42;
  for (;;) {
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) ++milliseconds;
    if (milliseconds >= 500) {
      milliseconds = 0;
      message.data[0] = (uint8_t)sent_frames;
      last_result = CAN_Send(FDCAN1, &message);
      if (last_result == CAN_Ok) ++sent_frames;
    }
    CAN_Result result = CAN_Receive(FDCAN1, &received);
    if (result == CAN_Ok) { ++received_frames; GPIO_Toggle(GPIOB, 1); }
    else if (result != CAN_WouldBlock) last_result = result;
    if (CAN_TakeOverflow(FDCAN1)) ++receive_overflows;
  }
}
