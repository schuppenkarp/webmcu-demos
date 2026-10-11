# 03 · USART1: Text senden und Echo empfangen

Ziel: Alternate Functions, Baudratenteiler, UART-Format und Polling von TXE/RXNE verstehen. Registerprogrammierung ohne HAL/LL, Interrupts oder DMA.

## Verdrahtung

| STM32G431CB | USB-UART-Adapter / passend herausgeführter ST-Link-V3-VCP |
| --- | --- |
| PA9 · USART1_TX · AF7 | RX |
| PA10 · USART1_RX · AF7 | TX |
| GND | GND |

3,3-V-UART-Pegel verwenden, keine RS-232-Pegel. TX und RX werden gekreuzt. SWD allein stellt keine UART-Verbindung her: Auch beim ST-Link-VCP müssen dessen UART-Pins verdrahtet sein. Pinverfügbarkeit und Adapteranschlüsse mit der eigenen Platine vergleichen.

## Einstellungen und Ablauf

`config.hpp`: `UART_BAUD = 115200`, `ECHO_ENABLED = true`. Pins und AF7 sind für diese Demo fest. `USART_CLOCK_HZ = 16000000` beschreibt die ausdrücklich gewählte HSI16-Taktquelle.

1. `pins_and_clock_init()`: HSI16 und Port-/Peripherietakte aktivieren, HSI16 als USART1-Kernel-Takt wählen, PA9/PA10 auf AF7 setzen. UCPD-Dead-Battery-Funktion wird deaktiviert.
2. `usart_init()`: Bei ausgeschaltetem USART 8N1, Oversampling 16 und BRR einstellen, danach Sender/Empfänger aktivieren.
3. Bei 16 MHz und 115200 Baud ergibt sich gerundet `BRR = 139`, tatsächlich etwa 115108 Baud (−0,08 % Teilungsfehler, zusätzlich HSI-Toleranz).
4. `send_text()` gibt einmal die Begrüßung aus. Anschließend wird jedes empfangene Byte unverändert zurückgesendet. Empfangsfehler werden quittiert und gezählt.

## In WebMCU ausprobieren

Debug bauen, flashen und starten. Im **Serialmonitor** den UART-Port öffnen: **115200 Baud, 8 Datenbits, keine Parität, 1 Stopbit, kein RTS/CTS**. Falls die Begrüßung bereits vor dem Öffnen gesendet wurde, Firmware nach dem Öffnen zurücksetzen. `Hallo` senden und das Echo prüfen; CR/LF werden unverändert zurückgesendet, keine zusätzliche Zeile wird erzeugt.

Watches: `received_bytes`, `last_received`, `receive_errors`. Ein Debug-Halt kann Zeichen verlieren und Overrun erzeugen. Diese einfache Polling-Demo besitzt keinen Empfangspuffer und ist für interaktive Texte, nicht für verlustfreie Dauerdaten gedacht. Während der Begrüßung wird noch nicht empfangen.

## Versuche

- Baudrate in Firmware und Serialmonitor gemeinsam auf 9600 setzen.
- Echo ausschalten und die Empfangszähler beobachten.
- Im Hex-Modus Bytes wie `41 42 0D 0A` senden und ihre Werte nachvollziehen.

Originalunterlagen: [STM32G431-Datenblatt, AF-Tabelle](https://www.st.com/resource/en/datasheet/stm32g431cb.pdf), [RM0440, RCC/PWR/USART](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).

Validierung: Debug-/Release-WASM-Builds. UART und Verdrahtung sind noch nicht physisch abgenommen. ST-/CMSIS-Lizenzen unter `include/`.
