# 01 · Einfachstes Blinklicht an PB1

Ziel: GPIO-Takt, Ausgangsmodus und atomisches Setzen/Rücksetzen mit BSRR verstehen. STM32G431CB; ausschließlich ST-Registerheader und CMSIS, kein HAL/LL.

## Aufbau und Start

PB1 → 330 Ω Vorwiderstand → LED-Anode; LED-Kathode → GND. Die LED ist bei HIGH an. Eine vorhandene Board-LED muss tatsächlich an PB1 angeschlossen sein; die Boardbelegung ist nicht vorausgesetzt.

In der IDE `main.cpp` öffnen, Debug bauen, Firmware flashen und starten. `config.hpp` enthält `WAIT_LOOPS`. Das Projekt startet nach Reset mit internem HSI16 (16 MHz); es benötigt keinen Quarz. Compiler-/Linker-Einstellungen stehen in den Projekteinstellungen, Flash/RAM im mitgelieferten Linkerskript.

## Lesereihenfolge

1. `config.hpp`: Anzahl Warteschleifendurchläufe.
2. `led_init()`: Porttakt → Ausgangswert → Ausgangseigenschaften → Ausgangsmodus.
3. `main()`: BSRR setzt PB1; die obere Hälfte desselben Registers setzt PB1 zurück.

Die Wartezeit ist **nicht kalibriert**. Debug und Release können deutlich unterschiedlich schnell blinken. Die CPU arbeitet während der gesamten Pause. Für einen definierten Zeitabstand folgt Demo 02 mit TIM6.

## Ausprobieren

- `WAIT_LOOPS` halbieren und verdoppeln.
- Einen Haltepunkt auf die beiden BSRR-Zuweisungen setzen und die LED schrittweise umschalten.
- Debug und Release vergleichen: Weshalb ist eine Schleife keine verlässliche Uhr?

Originalunterlagen: [Datenblatt STM32G431](https://www.st.com/resource/en/datasheet/stm32g431cb.pdf), [Referenzhandbuch RM0440](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).

Validierung: WASM-Compiler/Linker; die physische Hardwareabnahme ist noch offen. ST-/CMSIS-Lizenzen liegen unter `include/`.
