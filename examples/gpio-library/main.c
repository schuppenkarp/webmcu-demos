/* GPIO-Bibliothek: PB1 -> Vorwiderstand -> LED -> GND.
   Reset-Takt HSI16. Die Pause ist absichtlich einfach und ungenau. */
#include "mcu_gpio.h"
volatile uint32_t toggles = 0;
int main(void) {
  PeripheralClockEnable(GPIOB);
  GPIO_Write(GPIOB, 1, false);
  GPIO_Mode(GPIOB, 1, PushPull);
  GPIO_Pull(GPIOB, 1, NoPull);
  GPIO_Speed(GPIOB, 1, LowSpeed);
  for (;;) {
    GPIO_Toggle(GPIOB, 1);
    ++toggles;
    for (volatile uint32_t delay = 0; delay < 400000; ++delay) __NOP();
  }
}
