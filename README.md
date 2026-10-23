# webMCU · Controllerpakete und Beispiele

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
Das Manifest verweist direkt auf diese Quellen und die gemeinsamen Controller-
und Librarydateien. Im Repository liegen keine Demo-ZIPs oder zusätzlichen
Kopien der Header und Libraries in den Beispielordnern.

In der IDE: Bibliotheken → Quellen verwalten. Weitere GitHub-Repositories oder
lokale Ordner mit webmcu-libraries.json und/oder webmcu-collection.json können
zusätzlich verbunden werden. Die Manifeste hier sind die Formatvorlage.

Libraries unter MIT; Herstellerheader unter ihren enthaltenen ST-/CMSIS-Lizenzen.
Builds und Registersimulationen sind geprüft; physische UART-/CAN-/DFU-Abnahme offen.

## Demos ohne Archive

webmcu-collection.json verweist auf normale Dateien unter controllers/ und
libraries/. Die IDE lädt und prüft sie direkt aus diesem Git und speichert
sie lokal für Offline-Nutzung. Projektmetadaten und erzeugte Lockdateien
liegen separat; gemeinsame Header und Libraryadapter werden nicht kopiert.
Neue Git-Stände aktualisieren den Vorlagenkatalog, niemals bestehende Projekte.
