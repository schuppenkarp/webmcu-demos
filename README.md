# LG webMCU · Controllerpakete und Beispiele

Referenz: STM32G431CB, ST-Link V3. Keine HAL, kein Arduino.

- [Controller, Registerheader und Startup](controllers/stm32g431cb/)
- [Gemeinsame GPIO-/UART-/CAN-APIs 3.0](libraries/README.md)
- [STM32G431CB-Adapter](controllers/stm32g431cb/libraries/)
- [Fünf Beispiele](controllers/stm32g431cb/examples/): GPIO, UART, CAN, zwei minimale PB1-Blinker.

Gemeinsame Header liegen unter libraries/<id>/<revision>/, hardwareabhängige
Implementierungen unter controllers/<controller>/libraries/<id>/<revision>/.
Die IDE installiert den gemeinsamen Header und den passenden Adapter zusammen.
Alte Revisionen 1.0/2.0 bleiben inhaltlich unverändert beim Controller erhalten.

Controllerheader liegen in includes/, Startup in src/, Linkervorlagen in linker/.
Im erzeugten Projekt heißt der Headerpfad include/stm32g431xx.h. Herstellerdateien
und Lizenzen sind unverändert; headers.lock.json dokumentiert die Herkunft.

Die Beispiele im Quellbaum enthalten main.cpp, README.md und example.json.
Vollständige, eigenständige Projekt-ZIPs stehen unter packages/collections/stm32/.
Die ZIPs werden aus den gemeinsamen Quellen erzeugt; Header und Libraries werden
nicht zusätzlich in den Beispielordnern gepflegt.

In der IDE: Bibliotheken → Quellen verwalten. Weitere GitHub-Repositories oder
lokale Ordner mit webmcu-libraries.json und/oder webmcu-collection.json können
zusätzlich verbunden werden. Die Manifeste hier sind die Formatvorlage.

Libraries unter MIT; Herstellerheader unter ihren enthaltenen ST-/CMSIS-Lizenzen.
Builds und Registersimulationen sind geprüft; physische UART-/CAN-/DFU-Abnahme offen.
