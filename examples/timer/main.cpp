#include "include/stm32g431xx.h"
#include "config.hpp"

// Gut beobachtbare Variablen fuer die SWD-Liveansicht.
volatile unsigned timer_interrupts = 0;
volatile bool led_on = false;

// 1. Explizite Taktbasis: HSI16 -> SYSCLK -> AHB /1 -> APB1 /1.
static void clock_init() {
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)) {}
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {}
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
}

// 2. LED an PB1, aktiv HIGH.
static void led_init() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    (void)RCC->AHB2ENR;
    GPIOB->BSRR = GPIO_BSRR_BR1;
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT1;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD1;
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1;
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE1) | GPIO_MODER_MODE1_0;
}

// 3. Zeitgeber: f_update = 16 MHz / (PSC + 1) / (ARR + 1).
static void timer_init() {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
    (void)RCC->APB1ENR1;
    TIM6->CR1 = 0; // Noch nicht zaehlen.
    TIM6->DIER = 0;
    TIM6->PSC = TIMER_PRESCALER; // 1599 -> 10 kHz.
    TIM6->ARR = TIMER_COUNTS - 1; // 4999 -> 500 ms.
    TIM6->EGR = TIM_EGR_UG; // Gepufferten Prescaler jetzt uebernehmen.
    TIM6->SR = 0; // Das durch UG erzeugte Update-Flag vor Freigabe loeschen.
    NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
    NVIC_SetPriority(TIM6_DAC_IRQn, 2);
    TIM6->DIER = TIM_DIER_UIE; // Update-Ereignis darf Interrupt anfordern.
    NVIC_EnableIRQ(TIM6_DAC_IRQn); // Interrupt auch im Core freigeben.
    TIM6->CR1 = TIM_CR1_CEN; // Erst jetzt starten.
}

// 4. Starke Definition ersetzt den weak-Dummy aus startup.s automatisch.
// extern "C" sorgt auch in C++ fuer den exakten Namen in der Vektortabelle.
// Diese Demo aktiviert keinen DAC, der sich dieselbe IRQ-Leitung teilen wuerde.
extern "C" void TIM6_DAC_IRQHandler() {
    if (TIM6->SR & TIM_SR_UIF) {
        TIM6->SR = 0; // TIM6-Update quittieren, keine anderen Quellen benutzt.
        led_on = !led_on;
        GPIOB->BSRR = led_on ? GPIO_BSRR_BS1 : GPIO_BSRR_BR1;
        ++timer_interrupts;
    }
}

extern "C" int main() {
    clock_init();
    led_init();
    // Bei einem Debug-Halt auch den Timer anhalten.
    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM6_STOP;
    timer_init();
    while (true) {
        __WFI(); // CPU schlaeft bis zum Interrupt; Timer zaehlt weiter.
    }
}
