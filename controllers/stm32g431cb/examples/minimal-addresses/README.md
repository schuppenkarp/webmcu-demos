# 04 · Mini-Blink · direkte Adressen

PB1 → Vorwiderstand → LED → GND. STM32G431CB nach Reset, HSI16.

PB1 direkt über Speicheradressen. Nur stdint.h für feste Datentypen; keine Peripheriebibliothek oder Init-Funktionen.
Alle Zugriffe stehen direkt in main.cpp; die Pause ist absichtlich ungenau.
Taktfreigabe und Ausgangsmodus sind die beiden unverzichtbaren Einstellungen.

Startup: nur Stackpointer, Reset-Vektor und Sprung zu main.
Keine IRQ-Tabelle, keine RAM-Initialisierung, keine globalen/statischen Daten
und keine C++-Konstruktorinitialisierung. Dafür bei größeren Projekten die
vollständige Vorlage der Bibliotheksdemos verwenden.
