# Gemeinsame C++-Schnittstellen · 3.0.0

Die Header enthalten ausschließlich portable Datentypen und Klassenverträge.
Die Implementierung kommt aus dem zum Projekt passenden Controlleradapter.
Keine virtuellen Aufrufe, kein Heap und keine Hardwareinitialisierung im Konstruktor.

```cpp
#include "mcu_gpio.h"
GPIO led(GPIO::Port::B, 1);
led.enableClock();
led.write(false);
led.mode(GPIO::PushPull);
led.toggle();

#include "mcu_uart.h"
UART serial(UART::Instance::Port1);
serial.begin(16000000, 115200);
serial.print("Hallo!\r\n", 100000);

#include "mcu_can.h"
CAN bus(CAN::Instance::Port1);
bus.begin(16000000, 500000, CAN::InternalLoopback);
```

GPIO-Portnamen und UART/CAN-Instanzen sind logische Kennungen.
Der STM32G431CB-Adapter bildet UART Port1/2/3/4 auf USART1/2/3/UART4 ab;
LowPower1 ist LPUART1. CAN Port1 ist FDCAN1.
Pinmux, Pins und tatsächliche Peripherietakte bleiben controller-/boardspezifisch.
Nicht unterstützte IDs müssen fehlschlagen; keine stille Umleitung.

Pollingbudgets sind Schleifendurchläufe, keine Millisekunden. UART ist 8N1.
CAN verwendet klassische Frames mit 0–8 Bytes, nimmt alle IDs an und stellt
keine Filter-API bereit. Senden meldet eingereiht, nicht am Bus bestätigt.

Neue Controller implementieren dieselben Klassen in eigenen .cpp-Adaptern.
Der Katalog nennt pro Controller genau Header plus Adapter und deren SHA-256.
Alle API- oder Adapteränderungen erhalten neue Revisionen.



## Clock 2.0.0

Generische API: libraries/clock/2.0.0/mcu_clock.h; STM32G431CB-Implementierung:
controllers/stm32g431cb/libraries/clock/2.0.0/mcu_clock.cpp.

```cpp
uint32_t actual = Clock::configure(
    Clock::Source::ExternalOscillator, 8000000u, 168000000u);
if (!actual) { /* ungültige Frequenz oder Hardware-Timeout */ }
uint32_t active = Clock::frequencyHz();
```

Quellen: Internal (G431 HSI16), ExternalCrystal, ExternalOscillator (HSE-Bypass).
Der Adapter sucht PLL M/N/R automatisch innerhalb der Hardwaregrenzen.
Nur exakt erreichbare Frequenzen werden akzeptiert, keine stille Rundung.
CPU maximal 170 MHz; Busse /1; Range 1 Boost und vier Flash-Wartezyklen.
Aufruf bei Programmstart vor Peripherie/Interrupts. Unverträgliche
PLL-Peripherienutzung, CSS oder Busprescaler werden abgewiesen.
Clock::settings ist ein RAM-Objekt für Debugger und Live-Watches:
source, sourceHz, requestedHz, actualHz, pllM/N/R, pllEnabled, valid.
Bei Fehler ist valid=false; der aktive Takt lässt sich separat abfragen.
frequencyHz() dekodiert RCC mit der angegebenen externen Frequenz,
misst aber keinen physischen Oszillator. Das Board muss elektrisch stimmen.

Enable-GPIOs gehören zum Projekt. Unsere Demos setzen PF1 HIGH, bevor
Clock auf den externen 8-MHz-Oszillator und 168 MHz umschaltet.
PC13_CAN_disable wird im CAN-Projekt LOW gesetzt. Beide Minimal-Blinker
bleiben bei HSI16. UART-Baudrate, CAN und SysTick verwenden den aktiven Takt.

Referenzen: STM32G431 DS12589 und RM0440.
WASM-Builds und ARM-Registersimulation ersetzen keine Hardwareabnahme.
