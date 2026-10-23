// webMCU Clock 1.0.0 | MIT | generic C++ interface
#pragma once
#include <stdint.h>

class Clock {
public:
  enum class Source : uint8_t { Internal, External };
  // Call during startup, before configuring peripherals, timers or interrupts.
  // Adapter defines frequencies and external oscillator enable wiring.
  // Budget is a polling iteration limit per wait, not milliseconds.
  static bool select(Source source, uint32_t budget = 1000000);
  // SYSCLK, HCLK, PCLK1 and PCLK2 are equal in supported configurations.
  // Returns zero for unsupported clock configurations. No frequency measurement.
  static uint32_t frequencyHz();
};
