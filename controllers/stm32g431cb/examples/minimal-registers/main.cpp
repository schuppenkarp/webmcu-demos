// Derselbe Ablauf, diesmal mit den originalen ST-Registernamen.
#include "include/stm32g431xx.h"

extern "C" int main() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
  (void)RCC->AHB2ENR;
  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE1) | GPIO_MODER_MODE1_0;

  while (true) {
    GPIOB->ODR ^= GPIO_ODR_OD1;
    for (volatile uint32_t pause = 0; pause < 400000; ++pause) {
    }
  }
}
