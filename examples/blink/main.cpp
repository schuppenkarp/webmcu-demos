// STM32G431CB: LED blinkt an PB1, 500 ms ein / 500 ms aus.
// Blink-LED: PB1 -> Vorwiderstand -> LED -> GND (aktiv HIGH).
#include "board.hpp"

extern "C" int main() {
    board::init();
    while (true) {
        board::led(true);
        board::delay_ms(500);
        board::led(false);
        board::delay_ms(500);
    }
}
