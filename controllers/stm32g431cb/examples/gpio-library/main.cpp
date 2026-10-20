/* GPIO-Bibliothek: PB1 -> Vorwiderstand -> LED -> GND.
   Reset-Takt HSI16. Die Pause ist absichtlich einfach und ungenau. */
#include "mcu_gpio.h"
volatile uint32_t toggles = 0;
extern "C" int main() {
  GPIO led(GPIO::Port::B, 1);
  led.enableClock();
  led.write(false);
  led.mode(GPIO::PushPull);
  led.pull(GPIO::NoPull);
  led.speed(GPIO::LowSpeed);
  for (;;) {
    led.toggle();
    ++toggles;
    for (volatile uint32_t delay = 0; delay < 400000; ++delay)
      __asm volatile("nop");
  }
}
