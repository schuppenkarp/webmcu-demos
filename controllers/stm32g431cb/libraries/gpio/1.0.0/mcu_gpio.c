/* WebMCU GPIO 1.0.0 | MIT. Register definitions only, no HAL. */
#include "mcu_gpio.h"

static bool valid(GPIO_TypeDef *port, unsigned pin) {
  return pin < 16 && (port == GPIOA || port == GPIOB || port == GPIOC ||
                     port == GPIOD || port == GPIOE || port == GPIOF || port == GPIOG);
}
bool PeripheralClockEnable(const void *p) {
  volatile uint32_t *reg;
  uint32_t mask;
  if (p == GPIOA) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIOAEN; }
  else if (p == GPIOB) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIOBEN; }
  else if (p == GPIOC) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIOCEN; }
  else if (p == GPIOD) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIODEN; }
  else if (p == GPIOE) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIOEEN; }
  else if (p == GPIOF) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIOFEN; }
  else if (p == GPIOG) { reg = &RCC->AHB2ENR; mask = RCC_AHB2ENR_GPIOGEN; }
  else if (p == USART1) { reg = &RCC->APB2ENR; mask = RCC_APB2ENR_USART1EN; }
  else if (p == USART2) { reg = &RCC->APB1ENR1; mask = RCC_APB1ENR1_USART2EN; }
  else if (p == USART3) { reg = &RCC->APB1ENR1; mask = RCC_APB1ENR1_USART3EN; }
  else if (p == UART4) { reg = &RCC->APB1ENR1; mask = RCC_APB1ENR1_UART4EN; }
  else if (p == LPUART1) { reg = &RCC->APB1ENR2; mask = RCC_APB1ENR2_LPUART1EN; }
  else if (p == FDCAN1) { reg = &RCC->APB1ENR1; mask = RCC_APB1ENR1_FDCANEN; }
  else return false;
  *reg |= mask; (void)*reg;
  return true;
}
bool GPIO_Mode(GPIO_TypeDef *port, unsigned pin, GPIO_ModeType mode) {
  if (!valid(port, pin) || mode < Input || mode > Analog) return false;
  uint32_t shift = pin * 2;
  uint32_t value = mode == Input ? 0 : mode == Analog ? 3 : 1;
  port->MODER = (port->MODER & ~(3u << shift)) | (value << shift);
  if (mode == OpenDrain) port->OTYPER |= 1u << pin;
  else port->OTYPER &= ~(1u << pin);
  return true;
}
bool GPIO_Pull(GPIO_TypeDef *port, unsigned pin, GPIO_PullType pull) {
  if (!valid(port,pin) || pull < NoPull || pull > PullDown) return false;
  port->PUPDR = (port->PUPDR & ~(3u << (pin*2))) | ((uint32_t)pull << (pin*2));
  return true;
}
bool GPIO_Speed(GPIO_TypeDef *port, unsigned pin, GPIO_SpeedType speed) {
  if (!valid(port,pin) || speed < LowSpeed || speed > VeryHighSpeed) return false;
  port->OSPEEDR = (port->OSPEEDR & ~(3u << (pin*2))) | ((uint32_t)speed << (pin*2));
  return true;
}
bool GPIO_Alternate(GPIO_TypeDef *port, unsigned pin, unsigned alternate) {
  if (!valid(port,pin) || alternate > 15) return false;
  unsigned index=pin/8, shift=(pin%8)*4;
  port->AFR[index]=(port->AFR[index] & ~(15u<<shift)) | (alternate<<shift);
  port->OTYPER &= ~(1u<<pin);
  port->MODER=(port->MODER & ~(3u<<(pin*2))) | (2u<<(pin*2));
  return true;
}
bool GPIO_Write(GPIO_TypeDef *port, unsigned pin, bool high) {
  if (!valid(port,pin)) return false;
  port->BSRR = 1u << (pin + (high ? 0 : 16));
  return true;
}
bool GPIO_Read(GPIO_TypeDef *port, unsigned pin) {
  return valid(port,pin) && (port->IDR & (1u<<pin)) != 0;
}
bool GPIO_Toggle(GPIO_TypeDef *port, unsigned pin) {
  if (!valid(port,pin)) return false;
  return GPIO_Write(port,pin,(port->ODR & (1u<<pin)) == 0);
}
