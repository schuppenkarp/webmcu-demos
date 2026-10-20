# Eigene Interrupt-Handler

`startup.s` enthält die vollständige STM32G431-Vektortabelle mit den ST-Namen.
Reservierte Einträge sind `0`. Jeder nicht selbst implementierte IRQ zeigt
über einen **weak**-Alias auf `Default_Handler` (Endlosschleife, im Debugger sichtbar).

Den gewünschten Handler in `main.cpp` oder einer eigenen `.cpp` definieren:

```cpp
volatile unsigned ticks = 0;

extern "C" void SysTick_Handler() {
    ++ticks;
}
```

Kein `weak` und kein `static` verwenden. `extern "C"` verhindert die
C++-Namensänderung; die starke Definition ersetzt beim Linken den Dummy.
Der Name muss exakt dem Eintrag in `startup.s` entsprechen.
Eine bloße Deklaration ersetzt den Dummy noch nicht.

Die Peripherie und ihre Interruptfreigabe separat konfigurieren und
gegebenenfalls das Interruptflag quittieren. Die TIM6-Demo zeigt dies komplett.
Für Klassen ruft der freie Handler eine Methode deines Objekts auf.
Die Vektortabelle selbst muss dafür nicht geändert werden.
