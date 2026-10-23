// webMCU Clock 2.0.0 | MIT | generic C++ interface
#pragma once
#include <stdint.h>

class Clock {
public:
  enum class Source : uint8_t { Internal, ExternalCrystal, ExternalOscillator };
  struct Settings {
    Source source;
    uint32_t sourceHz;
    uint32_t requestedHz;
    uint32_t actualHz;
    uint32_t pllM, pllN, pllR;
    bool pllEnabled;
    bool valid;
  };
  // Retained in RAM for debugger watches. Read-only to application code by convention.
  static volatile Settings settings;
  // Startup only, before peripherals/interrupts. Exact frequencies only.
  // Returns the configured CPU frequency; zero means invalid or hardware timeout.
  // Enable pins and board wiring belong to the caller. Budget is polls per wait.
  static uint32_t configure(Source source, uint32_t sourceHz,
                            uint32_t cpuHz, uint32_t budget = 1000000);
  // Decode the active hardware clock and bus dividers. Zero means unknown.
  // External frequency is the caller's input, not a physical measurement.
  static uint32_t frequencyHz();
};
