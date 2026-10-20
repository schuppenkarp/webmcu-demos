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
// WebMCU UART 2.0.0 | STM32G431 | C++11 or later
#pragma once
#include <stddef.h>
#include "include/stm32g431xx.h"

// Do not share a peripheral between drivers or concurrent interrupt handlers.
class UART {
public:
  enum Result { Ok, WouldBlock, Timeout, Error, Invalid };
  // USART1/2/3, UART4, LPUART1. Constructor does not touch hardware.
  constexpr explicit UART(USART_TypeDef* peripheral) : u(peripheral) {}
  // 8N1; selects PCLK. Actual PCLK2 for USART1, PCLK1 for other instances.
  // Configure pins and alternate functions separately before begin().
  bool begin(uint32_t peripheralClockHz, uint32_t baud);
  Result tryWrite(uint8_t byte);
  Result tryRead(uint8_t& byte);
  // Budget counts polling iterations, NOT milliseconds. Partial output possible.
  Result write(const void* data, size_t size, uint32_t budget);
  Result print(const char* text, uint32_t budget);
  Result flush(uint32_t budget);
private:
  USART_TypeDef* const u;
};
