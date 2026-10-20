# 03 · CAN-Bibliothek · Loopback / Bus

C++-Klasse CAN. Klassisches CAN auf FDCAN1: 11-/29-Bit-IDs, 0–8 Bytes, alle IDs angenommen. Drei TX-/RX-Plätze, Polling, Bus-Off-/Überlaufmeldung. Internes Loopback für den Einstieg. Kein CAN FD. Gemeinsame API; STM32G431CB-Adapter.

Gemeinsame C++-API 3.0.0 plus STM32G431CB-Adapter unter lib/.
Die verwendeten Registerheader sind von ST. Keine HAL und kein Arduino.
Software geprüft; physische Abnahme am Board steht noch aus.
