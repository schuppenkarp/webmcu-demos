/*
 * MIT License
 * 
 * Copyright (c) 2026 WebMCU Studio contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* WebMCU UART 1.0.0 | MIT | STM32G431 | C and C++ */
#ifndef WEBMCU_UART_H
#define WEBMCU_UART_H
#include <stdbool.h>
#include <stddef.h>
#include "include/stm32g431xx.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { UART_Ok, UART_WouldBlock, UART_Timeout, UART_Error, UART_Invalid } UART_Result;
/* USART1/2/3, UART4, LPUART1. 8 data bits, no parity, 1 stop bit.
   Chooses PCLK; pass actual PCLK2 for USART1, PCLK1 for all others.
   Pins/alternate functions are configured by the caller. No interrupts/heap. */
bool UART_Init(USART_TypeDef *uart, uint32_t peripheralClockHz, uint32_t baud);
UART_Result UART_TryWrite(USART_TypeDef *uart, uint8_t byte);
UART_Result UART_TryRead(USART_TypeDef *uart, uint8_t *byte);
/* Budget is polling iterations, NOT milliseconds. Partial output is possible. */
UART_Result UART_Write(USART_TypeDef *uart, const void *data, size_t size, uint32_t budget);
UART_Result UART_Print(USART_TypeDef *uart, const char *text, uint32_t budget);
UART_Result UART_Flush(USART_TypeDef *uart, uint32_t budget);
#ifdef __cplusplus
}
#endif
#endif
