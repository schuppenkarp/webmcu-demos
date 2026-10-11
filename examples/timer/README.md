# 02 · PB1 blinken mit TIM6-Interrupt

Ziel: Prescaler, Auto-Reload, Update-Flag, NVIC und Interrupt-Vektortabelle verstehen. Keine Warteschleife für das Blinken, kein HAL/LL.

## Anschließen und einstellen

PB1 → 330 Ω → LED-Anode; Kathode → GND. `config.hpp` enthält die Einstellungen:

| Einstellung | Vorgabe | Bedeutung |
| --- | --- | --- |
| TIMER_CLOCK_HZ | 16000000 | HSI16, AHB und APB1 ohne Teilung |
| TIMER_TICK_HZ | 10000 | 0,1 ms je Zählschritt |
| LED_INTERVAL_MS | 500 | LED alle 500 ms umschalten |

Die volle Blinkperiode beträgt 1000 ms, also 1 Hz. `PSC = 1599` und `ARR = 4999`, weil beide Register den Wert **Anzahl minus eins** enthalten. Compilezeit-Prüfungen verhindern unpassende Teiler und Intervalle. Wird später der APB-Teiler geändert, muss auch die Timer-Taktberechnung angepasst werden; bei APB-Teilung kann der Timertakt doppelt so hoch wie der APB-Takt sein.

## Ablauf nachvollziehen

1. `clock_init()` stellt HSI16 und die Bus-Teiler ausdrücklich ein.
2. `led_init()` bereitet PB1 vor.
3. `timer_init()` setzt PSC/ARR, übernimmt sie mit UG und löscht das dadurch gesetzte UIF. Erst danach werden Timer-Interrupt, NVIC und Zähler freigegeben.
4. TIM6 setzt UIF und fordert IRQ 54 an. `startup.s` enthält an Vektorindex `16 + 54` den Eintrag `TIM6_DAC_IRQHandler`.
5. Der Handler quittiert UIF, schaltet PB1 um und zählt `timer_interrupts` hoch. `main()` wartet währenddessen mit WFI.

Der projektspezifische Vektoreintrag ist erforderlich: Nur eine C++-Funktion mit passendem Namen würde beim ursprünglichen Minimal-Startup nicht aufgerufen. TIM6 und DAC teilen sich die IRQ-Leitung; diese Demo aktiviert keinen DAC.

## Debug und Versuche

Debug bauen, flashen und starten. `timer_interrupts` und `led_on` sind als Watches vorbereitet. Bei einem Haltepunkt friert DBGMCU den Timer ein; im normalen Lauf zählt er weiter. Der interne RC-Oszillator bestimmt die Zeitgenauigkeit.

- Intervall auf 100 ms ändern; Blinkperiode und Interruptanzahl vorhersagen.
- Haltepunkt in den Handler setzen und UIF, PSC, ARR sowie CNT betrachten.
- Erklären, weshalb UG vor dem Freigeben des Interrupts quittiert werden muss.

Originalunterlagen: [STM32G431-Datenblatt](https://www.st.com/resource/en/datasheet/stm32g431cb.pdf), [RM0440](https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf), Kapitel RCC, TIM6/TIM7 und DBGMCU.

Validierung: Debug-/Release-WASM-Builds und Vektoreintrag geprüft. Physische Hardwareabnahme offen. Herstellerlizenzen unter `include/`.
