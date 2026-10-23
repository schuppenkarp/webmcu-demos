/* UART-Bibliothek: UART::Instance::Port3, PB10 TX / PB11 RX, 115200 Baud, 8N1.
   USB-UART mit 3,3-V-Pegeln: TX/RX kreuzen und GND verbinden. */
#include "mcu_gpio.h"
#include "mcu_clock.h"
#include "mcu_uart.h"
volatile bool clock_ready = false;
volatile uint32_t received_bytes = 0, receive_errors = 0, transmit_errors = 0;
volatile uint8_t last_byte = 0;
volatile bool uart_ready = false;
extern "C" int main() {
  // Board wiring: enable the external 8 MHz oscillator before clock setup.
  GPIO oscillator_enable(GPIO::Port::F, 1);
  oscillator_enable.enableClock();
  oscillator_enable.write(true);
  oscillator_enable.mode(GPIO::PushPull);
  clock_ready = Clock::configure(Clock::Source::ExternalOscillator,
                                  8000000u, 168000000u) != 0;
  if (!clock_ready)
    for (;;) __asm volatile("nop");
  GPIO tx(GPIO::Port::B, 10), rx(GPIO::Port::B, 11);
  UART serial(UART::Instance::Port3);
  tx.enableClock();
  tx.alternate(7);
  rx.alternate(7);
  rx.pull(GPIO::PullUp);
  uart_ready = serial.begin(Clock::frequencyHz(), 115200);
  if (!uart_ready)
    for (;;)
      __asm volatile("nop");
  if (serial.print("webMCU USART3: Echo bereit\r\n", 100000) != UART::Ok)
    ++transmit_errors;
  for (;;) {
    uint8_t byte;
    UART::Result result = serial.tryRead(byte);
    if (result == UART::Ok) {
      last_byte = byte;
      ++received_bytes;
      if (serial.write(&byte, 1, 100000) != UART::Ok)
        ++transmit_errors;
    } else if (result == UART::Error)
      ++receive_errors;
  }
}
