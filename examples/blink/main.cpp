#include "include/stm32g431xx.h"
#include "config.hpp"

// 1. GPIO-Takt einschalten, Ausgangswert vorbereiten, PB1 als Ausgang setzen.
static void led_init() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    (void)RCC->AHB2ENR; // Taktfreigabe vor dem ersten GPIO-Zugriff abschliessen.
    GPIOB->BSRR = GPIO_BSRR_BR1; // LED zunaechst aus.
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT1; // Push-Pull.
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD1; // Keine internen Pull-Widerstaende.
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1; // Langsame Flanken reichen.
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE1) | GPIO_MODER_MODE1_0;
}

// 2. Einfache, blockierende Pause. __NOP verhindert das Entfernen der Schleife.
static void wait() {
    for (unsigned n = 0; n < WAIT_LOOPS; ++n) {
        __NOP();
    }
}

// 3. Hauptprogramm: PB1 HIGH -> warten -> PB1 LOW -> warten.
extern "C" int main() {
    led_init();
    while (true) {
        GPIOB->BSRR = GPIO_BSRR_BS1; // LED an (PB1 -> Widerstand -> LED -> GND).
        wait();
        GPIOB->BSRR = GPIO_BSRR_BR1; // LED aus. BSRR veraendert nur diesen Pin.
        wait();
    }
}
