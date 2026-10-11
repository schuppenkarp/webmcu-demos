#include "include/stm32g431xx.h"
#include "config.hpp"

volatile unsigned received_bytes = 0;
volatile unsigned receive_errors = 0;
volatile unsigned last_received = 0;

// 1. Takte und Pins: PA9 sendet, PA10 empfaengt. Beide verwenden AF7.
static void pins_and_clock_init() {
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)) {}
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    (void)RCC->APB2ENR;
    // PA9/PA10 haben auch UCPD-Dead-Battery-Funktionen. Diese hier deaktivieren.
    PWR->CR3 |= PWR_CR3_UCPD_DBDIS;
    RCC->CCIPR = (RCC->CCIPR & ~RCC_CCIPR_USART1SEL) | RCC_CCIPR_USART1SEL_1;
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10);
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED9 | GPIO_OSPEEDR_OSPEED10);
    GPIOA->PUPDR = (GPIOA->PUPDR & ~(GPIO_PUPDR_PUPD9 | GPIO_PUPDR_PUPD10))
                  | GPIO_PUPDR_PUPD10_0; // RX im Ruhezustand HIGH halten.
    GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(GPIO_AFRH_AFSEL9 | GPIO_AFRH_AFSEL10))
                   | (7U << GPIO_AFRH_AFSEL9_Pos) | (7U << GPIO_AFRH_AFSEL10_Pos);
    GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE9 | GPIO_MODER_MODE10))
                  | GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1;
}

// 2. UART-Format: 8 Datenbits, keine Paritaet, 1 Stopbit, kein Handshake.
static void usart_init() {
    USART1->CR1 = 0; // UE=0: erst konfigurieren, dann einschalten.
    USART1->CR2 = 0;
    USART1->CR3 = 0;
    USART1->PRESC = 0; // Kernel-Takt /1.
    USART1->BRR = UART_BRR; // Oversampling 16: gerundeter Takt/Baud-Teiler (139).
    USART1->ICR = USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_PECF;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    while ((USART1->ISR & (USART_ISR_TEACK | USART_ISR_REACK))
           != (USART_ISR_TEACK | USART_ISR_REACK)) {}
}

// 3. Polling-Senden: auf Platz im Senderegister warten, dann ein Byte schreiben.
static void send_char(char c) {
    while (!(USART1->ISR & USART_ISR_TXE_TXFNF)) {}
    USART1->TDR = static_cast<unsigned char>(c);
}
static void send_text(const char* text) {
    while (*text) send_char(*text++);
}

extern "C" int main() {
    pins_and_clock_init();
    usart_init();
    send_text("\r\nSTM32G431CB - USART1 bereit. Taste senden fuer Echo.\r\n");
    while (true) {
        const unsigned status = USART1->ISR;
        const unsigned errors = USART_ISR_ORE | USART_ISR_FE | USART_ISR_NE | USART_ISR_PE;
        if (status & errors) {
            // Ein Fehler darf den Empfang nicht dauerhaft blockieren.
            if (status & USART_ISR_RXNE_RXFNE) (void)USART1->RDR;
            USART1->ICR = USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_PECF;
            ++receive_errors;
        } else if (status & USART_ISR_RXNE_RXFNE) {
            const unsigned byte = USART1->RDR & 0xffU; // Lesen quittiert RXNE.
            last_received = byte;
            ++received_bytes;
            if (ECHO_ENABLED) send_char(static_cast<char>(byte));
        }
    }
}
