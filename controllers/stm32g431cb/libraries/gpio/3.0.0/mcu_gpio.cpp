/* WebMCU GPIO 3.0.0 | MIT. Register definitions only, no HAL. */
#include "mcu_gpio.h"
#include "include/stm32g431xx.h"

static GPIO_TypeDef *resolvePort(GPIO::Port port) {
  switch (port) {
  case GPIO::Port::A:
    return GPIOA;
  case GPIO::Port::B:
    return GPIOB;
  case GPIO::Port::C:
    return GPIOC;
  case GPIO::Port::D:
    return GPIOD;
  case GPIO::Port::E:
    return GPIOE;
  case GPIO::Port::F:
    return GPIOF;
  case GPIO::Port::G:
    return GPIOG;
  }
  return nullptr;
}

static bool valid(GPIO_TypeDef *port, uint32_t pin) {
  return pin < 16 && (port == GPIOA || port == GPIOB || port == GPIOC || port == GPIOD ||
                      port == GPIOE || port == GPIOF || port == GPIOG);
}
bool GPIO::enableClock(Port target) {
  const void *p = resolvePort(target);
  volatile uint32_t *reg;
  uint32_t mask;
  if (p == GPIOA) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIOAEN;
  } else if (p == GPIOB) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIOBEN;
  } else if (p == GPIOC) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIOCEN;
  } else if (p == GPIOD) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIODEN;
  } else if (p == GPIOE) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIOEEN;
  } else if (p == GPIOF) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIOFEN;
  } else if (p == GPIOG) {
    reg = &RCC->AHB2ENR;
    mask = RCC_AHB2ENR_GPIOGEN;
  } else
    return false;
  *reg |= mask;
  (void)*reg;
  return true;
}
bool GPIO::mode(Mode mode) {
  auto *const port = resolvePort(target);
  if (!valid(port, pin) || mode < Input || mode > Analog)
    return false;
  uint32_t shift = pin * 2;
  uint32_t value = mode == Input ? 0 : mode == Analog ? 3 : 1;
  port->MODER = (port->MODER & ~(3u << shift)) | (value << shift);
  if (mode == OpenDrain)
    port->OTYPER |= 1u << pin;
  else
    port->OTYPER &= ~(1u << pin);
  return true;
}
bool GPIO::pull(Pull pull) {
  auto *const port = resolvePort(target);
  if (!valid(port, pin) || pull < NoPull || pull > PullDown)
    return false;
  port->PUPDR = (port->PUPDR & ~(3u << (pin * 2))) | ((uint32_t)pull << (pin * 2));
  return true;
}
bool GPIO::speed(Speed speed) {
  auto *const port = resolvePort(target);
  if (!valid(port, pin) || speed < LowSpeed || speed > VeryHighSpeed)
    return false;
  port->OSPEEDR = (port->OSPEEDR & ~(3u << (pin * 2))) | ((uint32_t)speed << (pin * 2));
  return true;
}
bool GPIO::alternate(uint32_t alternate) {
  auto *const port = resolvePort(target);
  if (!valid(port, pin) || alternate > 15)
    return false;
  uint32_t index = pin / 8, shift = (pin % 8) * 4;
  port->AFR[index] = (port->AFR[index] & ~(15u << shift)) | (alternate << shift);
  port->OTYPER &= ~(1u << pin);
  port->MODER = (port->MODER & ~(3u << (pin * 2))) | (2u << (pin * 2));
  return true;
}
bool GPIO::write(bool high) {
  auto *const port = resolvePort(target);
  if (!valid(port, pin))
    return false;
  port->BSRR = 1u << (pin + (high ? 0 : 16));
  return true;
}
bool GPIO::read() const {
  auto *const port = resolvePort(target);
  return valid(port, pin) && (port->IDR & (1u << pin)) != 0;
}
bool GPIO::toggle() {
  auto *const port = resolvePort(target);
  if (!valid(port, pin))
    return false;
  return write((port->ODR & (1u << pin)) == 0);
}
