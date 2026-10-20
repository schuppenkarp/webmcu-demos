/* UART-Bibliothek: USART1, PA9 TX / PA10 RX, 115200 Baud, 8N1.
   USB-UART mit 3,3-V-Pegeln: TX/RX kreuzen und GND verbinden. */
#include "mcu_gpio.h"
#include "mcu_uart.h"
volatile uint32_t received_bytes = 0, receive_errors = 0, transmit_errors = 0;
volatile uint8_t last_byte = 0;
volatile bool uart_ready = false;
extern "C" int main() {
  GPIO tx(GPIOA, 9), rx(GPIOA, 10);
  UART serial(USART1);
  tx.enableClock();
  tx.alternate(7);
  rx.alternate(7);
  rx.pull(GPIO::PullUp);
  uart_ready = serial.begin(16000000, 115200);
  if (!uart_ready) for (;;) __NOP();
  if (serial.print("WebMCU UART 2.0.0: Echo bereit\r\n", 100000) != UART::Ok) ++transmit_errors;
  for (;;) {
    uint8_t byte;
    UART::Result result = serial.tryRead(byte);
    if (result == UART::Ok) {
      last_byte = byte; ++received_bytes;
      if (serial.write(&byte, 1, 100000) != UART::Ok) ++transmit_errors;
    } else if (result == UART::Error) ++receive_errors;
  }
}
