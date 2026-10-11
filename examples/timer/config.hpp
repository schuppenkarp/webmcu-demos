#pragma once

// Einstellungen fuer TIM6. clock_init() stellt HSI16 und APB1-Teiler 1 ein.
constexpr unsigned TIMER_CLOCK_HZ = 16000000;
constexpr unsigned TIMER_TICK_HZ = 10000; // Ein Zaehlschritt dauert 0,1 ms.
constexpr unsigned LED_INTERVAL_MS = 500; // Umschaltabstand, NICHT volle Periode.

constexpr unsigned TIMER_PRESCALER = TIMER_CLOCK_HZ / TIMER_TICK_HZ - 1;
constexpr unsigned TIMER_COUNTS = TIMER_TICK_HZ / 1000 * LED_INTERVAL_MS;
static_assert(TIMER_CLOCK_HZ == 16000000, "clock_init verwendet HSI16");
static_assert(TIMER_TICK_HZ >= 1000 && TIMER_TICK_HZ <= TIMER_CLOCK_HZ
              && TIMER_TICK_HZ % 1000 == 0 && TIMER_CLOCK_HZ % TIMER_TICK_HZ == 0,
              "Ganzzahligen Timertakt mit ganzen Schritten pro ms verwenden");
static_assert(TIMER_PRESCALER <= 65535, "TIM6-Prescaler hat 16 Bit");
static_assert(LED_INTERVAL_MS > 0 && LED_INTERVAL_MS <= 65536 / (TIMER_TICK_HZ / 1000),
              "Intervall passt nicht in den 16-Bit-Timer");
