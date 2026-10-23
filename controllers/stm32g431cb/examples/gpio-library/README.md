# 01 · GPIO-Bibliothek · PB1

Externer 8-MHz-Oszillator an PF0, Enable PF1 = high; HSE-Bypass + PLL auf 168 MHz (PLL automatisch). Clock::configure vor der Peripherieinitialisierung, clock_ready zeigt den Erfolg.

C++-Klasse GPIO. Porttakt, Push-Pull/Open-Drain, Ein-/Ausgang, Pull-Widerstände, Geschwindigkeit und Alternate Function. Pinnummern 0–15; keine Bitmasken. Gemeinsame API; STM32G431CB-Adapter.

GPIO/UART/CAN API 3.0.0, Clock API 2.0.0 plus STM32G431CB-Adapter unter lib/.
Die verwendeten Registerheader sind von ST. Keine HAL und kein Arduino.
Software geprüft; physische Abnahme am Board steht noch aus.
