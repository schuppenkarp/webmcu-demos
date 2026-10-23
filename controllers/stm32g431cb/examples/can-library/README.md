# 03 · CAN-Bibliothek · Loopback / Bus

Externer 8-MHz-Oszillator an PF0, Enable PF1 = high; HSE-Bypass + PLL auf 168 MHz (PLL automatisch). Clock::configure vor der Peripherieinitialisierung, clock_ready zeigt den Erfolg.

PC13_CAN_disable wird bei der Initialisierung auf low gesetzt. CAN: 500 kbit/s, PB8 RX / PB9 TX (AF9). SysTick: 1 ms bei 168 MHz.

C++-Klasse CAN. Klassisches CAN auf FDCAN1: 11-/29-Bit-IDs, 0–8 Bytes, alle IDs angenommen. Drei TX-/RX-Plätze, Polling, Bus-Off-/Überlaufmeldung. Internes Loopback für den Einstieg. Kein CAN FD. Gemeinsame API; STM32G431CB-Adapter.

GPIO/UART/CAN API 3.0.0, Clock API 2.0.0 plus STM32G431CB-Adapter unter lib/.
Die verwendeten Registerheader sind von ST. Keine HAL und kein Arduino.
Software geprüft; physische Abnahme am Board steht noch aus.
