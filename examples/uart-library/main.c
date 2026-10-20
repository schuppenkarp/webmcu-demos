/* UART-Bibliothek: USART1, PA9 TX / PA10 RX, 115200 Baud, 8N1.
   USB-UART mit 3,3-V-Pegeln: TX/RX kreuzen und GND verbinden. */
#include "mcu_gpio.h"
#include "mcu_uart.h"
volatile uint32_t received_bytes = 0, receive_errors = 0, transmit_errors = 0;
volatile uint8_t last_byte = 0;
volatile bool uart_ready = false;
int main(void) {
  PeripheralClockEnable(GPIOA);
  GPIO_Alternate(GPIOA, 9, 7);
  GPIO_Alternate(GPIOA, 10, 7);
  GPIO_Pull(GPIOA, 10, PullUp);
  uart_ready = UART_Init(USART1, 16000000, 115200);
  if (!uart_ready) for (;;) __NOP();
  if (UART_Print(USART1, "WebMCU UART 1.0.0: Echo bereit\r\n", 100000) != UART_Ok) ++transmit_errors;
  for (;;) {
    uint8_t byte;
    UART_Result result = UART_TryRead(USART1, &byte);
    if (result == UART_Ok) {
      last_byte = byte; ++received_bytes;
      if (UART_Write(USART1, &byte, 1, 100000) != UART_Ok) ++transmit_errors;
    } else if (result == UART_Error) ++receive_errors;
  }
}
