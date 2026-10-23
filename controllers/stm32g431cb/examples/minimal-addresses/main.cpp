// STM32G431CB nach Reset: HSI16, PB1 -> Vorwiderstand -> LED -> GND.
// Nur feste Ganzzahltypen; keine Peripheriebibliothek oder Init-Funktionen.
#include <stdint.h>
extern "C" int main() {
  // RCC_AHB2ENR: GPIOB-Takt einschalten.
  *(volatile uint32_t *)0x4002104C |= 1u << 1;
  // Auslesen lt. Datenblatt verpflichtend
  (void)*(volatile uint32_t *)0x4002104C;

  // GPIOB_MODER: PB1 als Ausgang (Bits 3:2 = 01).
  *(volatile uint32_t *)0x48000400 = (*(volatile uint32_t *)0x48000400 & ~(3u << 2)) | (1u << 2);

  while (true) {
    // GPIOB_ODR: PB1 Einschalten.
    *(volatile uint32_t *)0x48000414 |= 1u << 1;
    for (volatile uint32_t pause = 0; pause < 400000; ++pause);
    // GPIOB_ODR: PB1 Ausschalten.
    *(volatile uint32_t *)0x48000414 &= ~(1u << 1);
    for (volatile uint32_t pause = 0; pause < 400000; ++pause);
  }
}
