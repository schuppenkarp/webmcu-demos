# 02 · UART-Bibliothek · USART3 Echo (PB10/PB11)

USART3 Echo: PB10 = TX, PB11 = RX, AF7. 115200 Baud, 8N1; PCLK1 = 16 MHz im Reset-Zustand. USB-UART mit 3,3-V-Pegeln: PB10 an RX, PB11 an TX und GND verbinden.

C++-Klasse UART. USART1/2/3, UART4 und LPUART1. 8N1 mit PCLK, einfache Textausgabe und nicht blockierende Byte-API. Pins separat konfigurieren. Pollbudget ist kein Millisekunden-Timeout. Gemeinsame API; STM32G431CB-Adapter.

Gemeinsame C++-API 3.0.0 plus STM32G431CB-Adapter unter lib/.
Die verwendeten Registerheader sind von ST. Keine HAL und kein Arduino.
Software geprüft; physische Abnahme am Board steht noch aus.
