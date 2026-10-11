# WebMCU Demos

Offizielle Demo-Sammlung für WebMCU Studio. Referenz: STM32G431CB, ST-Link V3. Registerprogrammierung mit originalen ST- und CMSIS-Headern, ohne HAL/LL.

## Verwenden

In WebMCU Studio unter Sammlungen auf **Demos laden** klicken und ein Projekt öffnen. Alternativ **schuppenkarp/webmcu-demos** als Repository verbinden. Die geprüften Projekt-ZIPs können offline installiert und unabhängig heruntergeladen werden. Weitere eigene Repositories lassen sich zusätzlich verbinden.

- **[01 · PB1 einfach blinken](examples/blink/README.md)**: GPIO-Initialisierung und einfache Warteschleife. `config.hpp` enthält die Warteeinstellung.
- **[02 · PB1 mit TIM6-Interrupt](examples/timer/README.md)**: 500-ms-Umschalten mit Timer, NVIC und passendem Startup-Vektor. Die CPU wartet mit WFI; Live-Watches zeigen Zähler und LED-Zustand.
- **[03 · USART1 senden und Echo](examples/usart1/README.md)**: PA9/PA10, 115200 Baud, 8N1; Begrüßung, Byte-Echo und Empfangsfehlerzähler. Anleitung für den WebSerial-Monitor enthalten.
- **live**: SWD-Watches mit Klasse, Struktur, Enum und Array; Debug-Profil verwenden.
- **empty**: Registerprojekt ohne Pin-Initialisierung.

Lesbare Quellen liegen unter examples/. Das Manifest webmcu-collection.json verweist auf eigenständige Projekt-ZIPs mit SHA-256. Nach Änderungen Quellen und ZIPs gemeinsam aktualisieren.

Die drei Einstiegsdemos gliedern Einstellungen, Initialisierung und Hauptprogramm. Jede Anleitung enthält Verdrahtung, Lesereihenfolge und Versuche. Die Beispiele werden in Debug und Release mit dem WASM-Compiler geprüft; beim Timer wird zusätzlich der tatsächlich gelinkte Interrupt-Vektor kontrolliert. Nach einer Aktualisierung in der IDE **Sammlungen → Demos laden** oder **Aktualisieren** wählen. Bereits geöffnete Projekte werden nicht überschrieben.

## Entwicklungsstand und Rechte

Die Projekte werden mit dem Browser-WASM-Compiler gebaut. Eine physische Hardwareabnahme ist noch offen. ST- und Arm-Dateien behalten ihre enthaltenen Lizenztexte unter include/ und ihre Copyright-Hinweise. Für eigenen Beispielcode ist derzeit keine zusätzliche Open-Source-Lizenz vergeben.

## Controllerprofile

Unter controllers/ liegen versionierte JSON-Profile. WebMCU lädt sie über die GitHub-API und bietet sie im Controller-Dropdown bei der Projektanlage an. Eigene Profile werden im freigegebenen Arbeitsordner unter .webmcu/controllers/ gespeichert und haben Vorrang vor gleichnamigen öffentlichen Profilen. Ein Profil beschreibt CPU, Header, Startup-Vorlage und mehrere Speicherregionen. Ein neues Profil ersetzt kein passendes Startup-/Header-Paket oder die Hardwareabnahme.
