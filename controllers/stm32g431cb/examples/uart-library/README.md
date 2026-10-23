# 02 · UART-Bibliothek · USART3 Echo (PB10/PB11)

Externer 8-MHz-Oszillator an PF0, Enable PF1 = high; HSE-Bypass + PLL auf 168 MHz (PLL automatisch). Clock::configure vor der Peripherieinitialisierung, clock_ready zeigt den Erfolg.

USART3 Echo: PB10 = TX, PB11 = RX, AF7. 115200 Baud, 8N1; PCLK1 = 168 MHz, PLL aus externen 8 MHz. USB-UART mit 3,3-V-Pegeln: PB10 an RX, PB11 an TX und GND verbinden.

C++-Klasse UART. USART1/2/3, UART4 und LPUART1. 8N1 mit PCLK, einfache Textausgabe und nicht blockierende Byte-API. Pins separat konfigurieren. Pollbudget ist kein Millisekunden-Timeout. Gemeinsame API; STM32G431CB-Adapter.

GPIO/UART/CAN API 3.0.0, Clock API 2.0.0 plus STM32G431CB-Adapter unter lib/.
Die verwendeten Registerheader sind von ST. Keine HAL und kein Arduino.
Software geprüft; physische Abnahme am Board steht noch aus.
