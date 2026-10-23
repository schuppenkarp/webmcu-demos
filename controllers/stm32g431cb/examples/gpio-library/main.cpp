/* GPIO-Bibliothek: PB1 -> Vorwiderstand -> LED -> GND.
   8 MHz an PF0 -> PLL -> 168 MHz, Enable PF1 = high. Die Pause ist absichtlich einfach und ungenau. */
#include "mcu_gpio.h"
#include "mcu_clock.h"
volatile bool clock_ready = false;
volatile uint32_t toggles = 0;
extern "C" int main() {
  // Board wiring: enable the external 8 MHz oscillator before clock setup.
  GPIO oscillator_enable(GPIO::Port::F, 1);
  oscillator_enable.enableClock();
  oscillator_enable.write(true);
  oscillator_enable.mode(GPIO::PushPull);
  clock_ready = Clock::configure(Clock::Source::ExternalOscillator,
                                  8000000u, 168000000u) != 0;
  if (!clock_ready)
    for (;;) __asm volatile("nop");
  GPIO led(GPIO::Port::B, 1);
  led.enableClock();
  led.write(false);
  led.mode(GPIO::PushPull);
  led.pull(GPIO::NoPull);
  led.speed(GPIO::LowSpeed);
  for (;;) {
    led.toggle();
    ++toggles;
    for (volatile uint32_t delay = 0; delay < 4200000; ++delay)
      __asm volatile("nop");
  }
}
