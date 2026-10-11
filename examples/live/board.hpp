#pragma once
#include "include/stm32g431xx.h"

// Example wiring: PB1 -> resistor -> LED -> GND (active high).
// HSI16 reset clock. Only symbolic ST/CMSIS register definitions.
namespace board {
inline void init() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    (void)RCC->AHB2ENR;
    GPIOB->BSRR = GPIO_BSRR_BR1;
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT1;
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1_Msk;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD1_Msk;
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE1_Msk) | GPIO_MODER_MODE1_0;
    SysTick->LOAD = 16000U - 1U;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
inline void led(bool on) { GPIOB->BSRR = on ? GPIO_BSRR_BS1 : GPIO_BSRR_BR1; }
inline void delay_ms(unsigned milliseconds) {
    (void)SysTick->CTRL;
    while (milliseconds--) while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}
}
}
