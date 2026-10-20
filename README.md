# WebMCU Demos

Fünf Beispiele für STM32G431CB. In WebMCU: **Neues Projekt → Vorlage**.

- **01 · GPIO-Bibliothek:** PB1 mit GPIO-Klasse, C++.
- **02 · UART-Bibliothek:** USART1-Echo; Klasse auch für USART2/3, UART4 und LPUART1.
- **03 · CAN-Bibliothek:** klassisches CAN, internes Loopback oder echter Bus.
- **04 · Mini-Blink · direkte Adressen:** PB1 ohne Includes, nur direkte Speicherzugriffe in main.cpp.
- **05 · Mini-Blink · ST-Registernamen:** derselbe Blinker mit RCC/ GPIOB aus dem ST-Header.

Die Mini-Beispiele haben nur Stackpointer, Reset-Vektor und Sprung zu main.
Keine RAM-Initialisierung, keine C++-Konstruktorinitialisierung und keine IRQ-Tabelle.
Sie verwenden keine globalen/statischen Daten und keine Interrupts.
Die nötige GPIO-Taktfreigabe und der Ausgangsmodus stehen direkt in main.
Die einfache Warteschleife ist absichtlich ungenau. LED: PB1 → Vorwiderstand → LED → GND.

Die drei Bibliotheksbeispiele behalten den vollständigen Startup mit überschreibbaren weak IRQ-Handlern; siehe INTERRUPTS.md im jeweiligen Projekt.

[Versionierte C++-Bibliotheken](libraries/README.md): je .cpp + .h, Revision 2.0.0.
Quellen unter examples/, vollständige Projekt-ZIPs mit SHA-256 in webmcu-collection.json.
Eigene Sammlungen können zusätzlich verbunden werden. Frühere Beispiele bleiben in Git verfügbar.

## Eigene Quellen

In WebMCU unter **Bibliotheken → Quellen verwalten** weitere GitHub-Repositories
oder lokale Ordner hinzufügen. Dieses Standardrepository bleibt daneben verfügbar.
Ein lokaler Clone dieses Repositories kann ebenfalls als Ordnerquelle dienen.

Eigene Quellen enthalten im Hauptordner `webmcu-libraries.json` und/oder
`webmcu-collection.json` mit den darin referenzierten Dateien und SHA-256-Werten.
Die hier enthaltenen Manifeste sind die Formatvorlage. Beim Laden werden beide
Katalogarten geprüft und offline verfügbar gemacht. Beispielprojekte erscheinen
anschließend auch unter **Neues Projekt**.

Die gepflegten Library-Originale liegen unter `libraries/<id>/<revision>/`.
Die Kopien unter `examples/` gehören zu eigenständig baubaren Beispielprojekten
und werden daraus erzeugt. Neue Library-Inhalte erhalten eine neue Revision;
bestehende Revisionen werden nicht überschrieben.

Herstellerheader unter der jeweiligen ST-/CMSIS-Lizenz; siehe include/.
Bibliotheken unter MIT, siehe libraries/LICENSE. Hardwareabnahme weiterhin offen.
