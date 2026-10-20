/* CAN-Bibliothek: startet im internen Loopback, ohne CAN-Verkabelung.
   Für echten Bus LOOPBACK auf 0 setzen. PB8 RX / PB9 TX, AF9,
   externer CAN-Transceiver, Terminierung und zweiter ACK-Knoten nötig. */
#include "mcu_gpio.h"
#include "mcu_can.h"
#define LOOPBACK 1
volatile bool can_ready = false;
volatile uint32_t sent_frames = 0, received_frames = 0, receive_overflows = 0;
volatile CAN::Result last_result = CAN::Ok;
CAN::Message received;
extern "C" int main() {
  GPIO led(GPIOB, 1);
  CAN bus(FDCAN1);
  led.enableClock();
  led.write(false);
  led.mode(GPIO::PushPull);
#if !LOOPBACK
  GPIO rx(GPIOB, 8), tx(GPIOB, 9);
  rx.alternate(9);
  tx.alternate(9);
#endif
  can_ready = bus.begin(16000000, 500000,
                      LOOPBACK ? CAN::InternalLoopback : CAN::Normal);
  if (!can_ready) for (;;) __NOP();
  SysTick->LOAD = 16000 - 1; /* 1 ms, Polling statt Interrupt */
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
  uint32_t milliseconds = 0;
  CAN::Message message = {};
  message.id = 0x123; message.length = 2; message.data[1] = 0x42;
  for (;;) {
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) ++milliseconds;
    if (milliseconds >= 500) {
      milliseconds = 0;
      message.data[0] = (uint8_t)sent_frames;
      last_result = bus.send(message);
      if (last_result == CAN::Ok) ++sent_frames;
    }
    CAN::Result result = bus.receive(received);
    if (result == CAN::Ok) { ++received_frames; led.toggle(); }
    else if (result != CAN::WouldBlock) last_result = result;
    if (bus.takeOverflow()) ++receive_overflows;
  }
}
