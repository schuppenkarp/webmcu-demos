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
// LG webMCU GPIO 3.0.0 | portable interface | C++11 or later
#pragma once
#include <stddef.h>
#include <stdint.h>

// Do not share a peripheral between drivers or concurrent interrupt handlers.
class GPIO {
public:
  enum class Port : uint8_t { A, B, C, D, E, F, G };
  enum Mode : uint8_t { Input, PushPull, OpenDrain, Analog };
  enum Pull : uint8_t { NoPull, PullUp, PullDown };
  enum Speed : uint8_t { LowSpeed, MediumSpeed, HighSpeed, VeryHighSpeed };
  // Only stores port and pin. Call enableClock() before accessing registers.
  // Pin is 0..15, not a bit mask. No heap or hardware access in constructor.
  constexpr GPIO(Port target, uint32_t pin) : target(target), pin(pin) {}
  bool enableClock() const { return enableClock(target); }
  // Enables only the peripheral bus clock, never changes the system clock.
  static bool enableClock(Port target);
  bool mode(Mode value);
  bool pull(Pull value);
  bool speed(Speed value);
  // AF0..15, alternate-function mode with push-pull output.
  bool alternate(uint32_t function);
  bool write(bool high);
  bool read() const;
  // Not atomic against concurrent accesses to the same pin.
  bool toggle();

private:
  const Port target;
  const uint32_t pin;
};
