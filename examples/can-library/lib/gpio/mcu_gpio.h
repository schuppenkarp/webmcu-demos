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
/* WebMCU GPIO 1.0.0 | MIT | STM32G431 | C and C++ */
#ifndef WEBMCU_GPIO_H
#define WEBMCU_GPIO_H
#include <stdbool.h>
#include "include/stm32g431xx.h"
#ifdef __cplusplus
extern "C" {
#endif

/* pin is a number 0..15, not a bit mask. Configure pins before enabling IRQs. */
typedef enum { Input, PushPull, OpenDrain, Analog } GPIO_ModeType;
typedef enum { NoPull, PullUp, PullDown } GPIO_PullType;
typedef enum { LowSpeed, MediumSpeed, HighSpeed, VeryHighSpeed } GPIO_SpeedType;

/* Supports available GPIO, USART/UART/LPUART and FDCAN1 peripherals.
   Enables only the bus clock; does not change the system clock. */
bool PeripheralClockEnable(const void *peripheral);
bool GPIO_Mode(GPIO_TypeDef *port, unsigned pin, GPIO_ModeType mode);
bool GPIO_Pull(GPIO_TypeDef *port, unsigned pin, GPIO_PullType pull);
bool GPIO_Speed(GPIO_TypeDef *port, unsigned pin, GPIO_SpeedType speed);
/* Select AF0..15; sets alternate-function mode and push-pull output. */
bool GPIO_Alternate(GPIO_TypeDef *port, unsigned pin, unsigned alternate);
bool GPIO_Write(GPIO_TypeDef *port, unsigned pin, bool high);
bool GPIO_Read(GPIO_TypeDef *port, unsigned pin);
/* Read/modify/write: do not toggle the same pin from concurrent IRQs. */
bool GPIO_Toggle(GPIO_TypeDef *port, unsigned pin);

#ifdef __cplusplus
}
#endif
#endif
