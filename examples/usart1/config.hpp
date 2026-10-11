#pragma once

// Einstellungen: feste Pinbelegung PA9=TX / PA10=RX, Alternate Function 7.
// USART1 erhaelt HSI16 direkt, unabhaengig von den APB-Teilern.
constexpr unsigned USART_CLOCK_HZ = 16000000;
constexpr unsigned UART_BAUD = 115200;
constexpr bool ECHO_ENABLED = true;

constexpr unsigned UART_BRR = (USART_CLOCK_HZ + UART_BAUD / 2) / UART_BAUD;
static_assert(USART_CLOCK_HZ == 16000000, "USART1 verwendet HSI16");
static_assert(UART_BAUD > 0 && UART_BRR >= 16 && UART_BRR <= 65535,
              "Baudrate passt nicht zu Oversampling 16 und 16-Bit-BRR");
