#pragma once

// Einstellungen: mehr Durchlaeufe = langsameres Blinken.
// Absichtlich KEINE Millisekunden: die Dauer haengt von Optimierung und Takt ab.
constexpr unsigned WAIT_LOOPS = 800000;
static_assert(WAIT_LOOPS > 0, "WAIT_LOOPS muss groesser als null sein");
