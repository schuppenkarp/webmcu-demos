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
/* WebMCU CAN 1.0.0 | MIT | STM32G431 FDCAN1 | C and C++ */
#ifndef WEBMCU_CAN_H
#define WEBMCU_CAN_H
#include <stdbool.h>
#include "include/stm32g431xx.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { CAN_Normal, CAN_InternalLoopback } CAN_Mode;
typedef enum { CAN_Ok, CAN_WouldBlock, CAN_BusOff, CAN_Invalid, CAN_Unsupported } CAN_Result;
typedef struct {
  uint32_t id;
  uint8_t length;       /* 0..8 */
  bool extended;       /* false: 11-bit ID; true: 29-bit ID */
  bool remote;         /* remote request; no payload */
  uint8_t data[8];
} CAN_Message;
/* Classic CAN only. Accepts all standard/extended and remote frames into FIFO0.
   Selects PCLK1, divider 1. Pass actual PCLK1 Hz. Pins/AF9 are caller-owned.
   Resets FDCAN1 and its FIFOs. Blocking init has a bounded polling budget.
   Do not share this peripheral with another driver or call from concurrent IRQs. */
bool CAN_Init(FDCAN_GlobalTypeDef *can, uint32_t pclk1Hz, uint32_t bitrate, CAN_Mode mode);
/* Nonblocking. CAN_Ok means queued, not acknowledged on the bus.
   Three TX slots; automatic retransmission. A missing ACK can fill the queue. */
CAN_Result CAN_Send(FDCAN_GlobalTypeDef *can, const CAN_Message *message);
CAN_Result CAN_Receive(FDCAN_GlobalTypeDef *can, CAN_Message *message);
bool CAN_IsBusOff(FDCAN_GlobalTypeDef *can);
/* Returns and clears the RX FIFO0 message-lost indication. Drain FIFO promptly. */
bool CAN_TakeOverflow(FDCAN_GlobalTypeDef *can);
#ifdef __cplusplus
}
#endif
#endif
