// STM32G431CB nach Reset: HSI16, PB1 -> Vorwiderstand -> LED -> GND.
// Keine Includes, keine Bibliothek, keine Init-Funktionen.
extern "C" int main() {
    // RCC_AHB2ENR: GPIOB-Takt einschalten.
    *reinterpret_cast<volatile unsigned*>(0x4002104C) |= 1u << 1;
    (void)*reinterpret_cast<volatile unsigned*>(0x4002104C);

    // GPIOB_MODER: PB1 als Ausgang (Bits 3:2 = 01).
    *reinterpret_cast<volatile unsigned*>(0x48000400) =
        (*reinterpret_cast<volatile unsigned*>(0x48000400) & ~(3u << 2)) | (1u << 2);

    while (true) {
        // GPIOB_ODR: PB1 umschalten.
        *reinterpret_cast<volatile unsigned*>(0x48000414) ^= 1u << 1;
        for (volatile unsigned pause = 0; pause < 400000; ++pause) {}
    }
}
