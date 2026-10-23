/* CAN-Bibliothek: startet im internen Loopback, ohne CAN-Verkabelung.
   Für echten Bus LOOPBACK auf 0 setzen. PB8 RX / PB9 TX, AF9,
   externer CAN-Transceiver, Terminierung und zweiter ACK-Knoten nötig. */
#include "include/stm32g431xx.h" // SysTick: controller-specific example
#include "mcu_can.h"
#include "mcu_gpio.h"
#include "mcu_clock.h"
volatile bool clock_ready = false;
#define LOOPBACK 1
volatile bool can_ready = false;
volatile uint32_t sent_frames = 0, received_frames = 0, receive_overflows = 0;
volatile CAN::Result last_result = CAN::Ok;
CAN::Message received;
extern "C" int main() {
  GPIO can_disable(GPIO::Port::C, 13);
  can_disable.enableClock();
  can_disable.write(false); // PC13_CAN_disable: low enables the transceiver.
  can_disable.mode(GPIO::PushPull);
  can_disable.pull(GPIO::NoPull);
  can_disable.speed(GPIO::LowSpeed);
  clock_ready = Clock::select(Clock::Source::External);
  if (!clock_ready)
    for (;;) __asm volatile("nop");
  GPIO led(GPIO::Port::B, 1);
  CAN bus(CAN::Instance::Port1);
  led.enableClock();
  led.write(false);
  led.mode(GPIO::PushPull);
#if !LOOPBACK
  GPIO rx(GPIO::Port::B, 8), tx(GPIO::Port::B, 9);
  rx.alternate(9);
  tx.alternate(9);
#endif
  can_ready = bus.begin(Clock::frequencyHz(), 500000, LOOPBACK ? CAN::InternalLoopback : CAN::Normal);
  if (!can_ready)
    for (;;)
      __asm volatile("nop");
  SysTick->LOAD = Clock::frequencyHz() / 1000u - 1u; /* 1 ms, Polling statt Interrupt */
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
  uint32_t milliseconds = 0;
  CAN::Message message = {};
  message.id = 0x123;
  message.length = 2;
  message.data[1] = 0x42;
  for (;;) {
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
      ++milliseconds;
    if (milliseconds >= 500) {
      milliseconds = 0;
      message.data[0] = (uint8_t)sent_frames;
      last_result = bus.send(message);
      if (last_result == CAN::Ok)
        ++sent_frames;
    }
    CAN::Result result = bus.receive(received);
    if (result == CAN::Ok) {
      ++received_frames;
      led.toggle();
    } else if (result != CAN::WouldBlock)
      last_result = result;
    if (bus.takeOverflow())
      ++receive_overflows;
  }
}
