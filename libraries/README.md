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

## Clock 1.0.0: 168 MHz aus externen 8 MHz

Generischer Header unter libraries/clock/1.0.0, Adapter unter
controllers/stm32g431cb/libraries/clock/1.0.0 (je .h + .cpp).
Clock::select(Clock::Source::External): PF1 high, HSE-Bypass an PF0,
PLL M=2/N=84/R=2, 168 MHz für SYSCLK/HCLK/PCLK1/PCLK2. Range 1 Boost,
Flash 4 Wait States, AHB /2-Zwischenschritt beim Taktwechsel.
Internal wählt HSI16 und deaktiviert den externen Oszillator.
Vor Peripherie/Interrupts aufrufen; unbekannte PLL/CSS/Busprescaler ablehnen.
Alle Wartezeiten sind begrenzt, false nie ignorieren. Bei Umschalt-Timeout
bleiben Quellen an; frequencyHz() ist eine Konfigurationsauskunft, keine Messung.

GPIO-, USART3- und CAN-Demo nutzen 168 MHz. UART: 115200/8N1;
CAN: 500 kbit/s und SysTick 1 ms. PC13_CAN_disable wird bei Init low gesetzt.
Die beiden Minimal-Blinker bleiben bei HSI16 und ohne Clock-Library.
Referenz: RM0440 (HSE bypass, PLL, Voltage scaling, Flash latency).
Softwareprüfungen ersetzen keine physische Oszillator-/CAN-Abnahme.
