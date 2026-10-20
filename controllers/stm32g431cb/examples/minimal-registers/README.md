# 05 · Mini-Blink · ST-Registernamen

PB1 → Vorwiderstand → LED → GND. STM32G431CB nach Reset, HSI16.

Derselbe PB1-Blinker mit RCC/ GPIOB und originalen ST-Registerdefinitionen.
Alle Zugriffe stehen direkt in main.cpp; die Pause ist absichtlich ungenau.
Taktfreigabe und Ausgangsmodus sind die beiden unverzichtbaren Einstellungen.

Startup: nur Stackpointer, Reset-Vektor und Sprung zu main.
Keine IRQ-Tabelle, keine RAM-Initialisierung, keine globalen/statischen Daten
und keine C++-Konstruktorinitialisierung. Dafür bei größeren Projekten die
vollständige Vorlage der Bibliotheksdemos verwenden.
