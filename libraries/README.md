# Kleine Registerbibliotheken · STM32G431CB

WebMCU: **Projektmenü ⋯ → Bibliotheken → Revision wählen → Ins Projekt laden**.
Jede Bibliothek besteht nur aus einer `.cpp` und einer `.h` (ab Revision 2.0.0). Beide landen unter
`lib/<name>/`; `libraries.lock.json` hält Revision, Herkunft und SHA-256 fest.
Die Dateien gehören danach zum Projekt und dürfen bearbeitet werden.
Updates erfolgen bewusst; lokal veränderte Dateien werden nicht überschrieben.

| Paket | Kurzbeispiel | Umfang |
|---|---|---|
| GPIO 2.0.0 | `GPIO led(GPIOB, 1); led.mode(GPIO::PushPull);` | Pinmodus, Pull, AF, Geschwindigkeit, Lesen/Schreiben |
| UART 2.0.0 | `UART serial(USART1); serial.begin(16000000, 115200);` | USART1/2/3, UART4, LPUART1, 8N1, Bytes/Text |
| CAN 2.0.0 | `CAN bus(FDCAN1); bus.begin(16000000, 500000);` | Klassisches CAN, alle IDs, 0–8 Bytes |

```cpp
#include "mcu_gpio.h"
GPIO led(GPIOB, 1);
led.enableClock();
led.write(false);
led.mode(GPIO::PushPull);
led.toggle();
```

Die Argumente enthalten Pinnummern 0–15, keine Bitmasken. GPIO-Konfiguration
vor Interruptbetrieb ausführen. `write` nutzt BSRR; `toggle` ist
nicht atomar gegenüber einem gleichzeitigen Zugriff auf denselben Pin.
Die STM32-Registerheader werden unter `include/stm32g431xx.h` erwartet.

UART stellt den Taktmultiplexer auf PCLK. Tatsächlichen PCLK2 für USART1,
PCLK1 für die übrigen Instanzen übergeben. Die Pins sind frei wählbar,
müssen aber die richtige Alternate Function der gewählten Instanz besitzen.
Die Demo verwendet PA9/PA10, AF7, 115200 Baud. `tryRead`/`tryWrite` blockieren
nicht. Bei `write`, `print` und `flush` zählt das Budget Polling-Schritte,
keine Millisekunden. Fehler- und Timeout-Rückgaben auswerten.

CAN nutzt FDCAN1 mit PCLK1 und internem Teiler 1. Die Bibliothek akzeptiert
Standard-, Extended- und Remote-Frames ohne Filtertabellen. `bus.send(frame) == CAN::Ok`
bedeutet eingereiht, nicht vom Bus bestätigt. Je drei TX-/RX-Plätze;
Empfang regelmäßig abholen und `takeOverflow` prüfen. Bus-Off wird
gemeldet, nicht verborgen automatisch zurückgesetzt. Nach Behebung des
Busfehlers kann `begin` bewusst erneut aufgerufen werden.

Die CAN-Demo startet intern im Loopback. Für einen echten Bus `LOOPBACK=0`:
PB8 RX / PB9 TX, AF9, geeigneter CAN-Transceiver, gemeinsames GND und
120 Ω an beiden Busenden. Ein zweiter aktiver Knoten muss ACK geben.
PB8 ist auch BOOT0: Boardkonfiguration beachten. Bei realen CAN-Netzen
Taktgenauigkeit und Bit-Timing prüfen; die HSI16-Demo ersetzt keine
physische Busabnahme. Kein CAN FD, keine DMA-/Interruptverwaltung.

Neue Projekte bieten passende GPIO-, UART- und CAN-Demos.
Geprüft: echte ARM-Builds im Browsercompiler und ARM-Ausführung gegen
simulierte Register. Physische GPIO/UART/CAN-Abnahme steht noch aus.

Quellen: [G431-Datenblatt](https://www.st.com/resource/en/datasheet/stm32g431cb.pdf),
[RM0440](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).
ST-HAL-Quellen wurden zur Gegenprüfung des festen Message-RAM-Layouts gelesen;
die Bibliotheken enthalten keine HAL und benötigen keine HAL.

Veröffentlichungen sind unveränderlich: Änderungen erhalten eine neue
Versionsnummer und einen neuen Versionsordner. `webmcu-libraries.json`
referenziert konkrete Dateien mit SHA-256. Der IDE-Download liest alle Dateien
aus einem festen Git-Commit. Mitgelieferte Offlinepakete sind durch Version
und Dateihashes gebunden, auch ohne Repository-Verbindung.

Revision 2.0.0 verwendet echte C++-Klassen (C++11 oder neuer), ohne Heap, virtuelle Methoden, Exceptions oder versteckte Initialisierung. Konstruktoren speichern nur die Instanz bzw. Port/Pinnummer. Initialisierung erfolgt explizit über `enableClock()` und `begin()`. Die Demos verwenden `main.cpp`; `extern "C"` am Einstieg verbindet lediglich den Assembler-Startup mit C++.

Migration von 1.0.0: neue Revision bewusst im Bibliotheksdialog auswählen und eigene Aufrufer auf die Klassen-API umstellen. Alte unveränderte `.c` wird entfernt, `.cpp` und `.h` werden installiert. Geänderte Dateien blockieren den Wechsel. Bestehende Projekte werden nicht automatisch verändert; 1.0.0 bleibt als Altversion verfügbar.
