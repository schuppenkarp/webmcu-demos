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
// WebMCU CAN 3.0.0 | STM32G431 | C++11 or later
#pragma once
#include <stddef.h>
#include <stdint.h>

// Do not share a peripheral between drivers or concurrent interrupt handlers.
class CAN {
public:
  enum class Instance : uint8_t { Port1 };
  enum Mode : uint8_t { Normal, InternalLoopback };
  enum Result : uint8_t { Ok, WouldBlock, BusOff, Invalid, Unsupported };
  struct Message {
    uint32_t id = 0;
    uint8_t length = 0;    // 0..8
    bool extended = false; // false: 11-bit ID, true: 29-bit ID
    bool remote = false;
    uint8_t data[8] = {};
  };
  constexpr explicit CAN(Instance instance = Instance::Port1) : instance(instance) {}
  // Classic CAN, accepts all IDs. Clock source is defined by the adapter.
  // Reinitializes the peripheral and FIFOs; configure board pins separately.
  bool begin(uint32_t pclk1Hz, uint32_t bitrate, Mode mode = Normal);
  // Nonblocking: Ok means queued, not acknowledged. Three TX/RX slots.
  Result send(const Message &message);
  Result receive(Message &message);
  bool isBusOff() const;
  // Returns and clears RX FIFO0 message-lost indication.
  bool takeOverflow();

private:
  const Instance instance;
};
