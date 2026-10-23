// webMCU Clock 1.0.0 | MIT | STM32G431CB board adapter
// External 8 MHz active oscillator: PF0 input, PF1 enable (high = on).
// HSE bypass -> PLL (M=2, N=84, R=2) -> 168 MHz. Buses /1. HSI16 stays on.
#include "mcu_clock.h"
#include "include/stm32g431xx.h"

static constexpr uint32_t pll168 = RCC_PLLCFGR_PLLSRC_HSE |
    (1u << RCC_PLLCFGR_PLLM_Pos) | (84u << RCC_PLLCFGR_PLLN_Pos) | RCC_PLLCFGR_PLLREN;
static void settle() {
  // More than 1 us even at 168 MHz; no timer dependency during clock changes.
  for (uint32_t i = 0; i < 256; ++i) __asm volatile("nop");
}

static bool waitBits(volatile uint32_t &reg, uint32_t mask,
                     uint32_t expected, uint32_t budget) {
  while ((reg & mask) != expected) {
    if (budget == 0) return false;
    --budget;
  }
  return true;
}

bool Clock::select(Source source, uint32_t budget) {
  if (source != Source::Internal && source != Source::External) return false;
  // Refuse unknown PLL configurations or CSS; this driver owns only its PLL.
  if (RCC->CR & RCC_CR_CSSON) return false;
  if ((RCC->CR & RCC_CR_PLLON) && RCC->PLLCFGR != pll168) return false;
  if (RCC->CFGR & (RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2)) return false;
  const uint32_t current = RCC->CFGR & RCC_CFGR_SWS;
  if (current != RCC_CFGR_SWS_HSI && current != RCC_CFGR_SWS_HSE && current != RCC_CFGR_SWS_PLL) return false;

  RCC->CR |= RCC_CR_HSION;
  if (!waitBits(RCC->CR, RCC_CR_HSIRDY, RCC_CR_HSIRDY, budget)) return false;
  if (current == RCC_CFGR_SWS_PLL) {
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV2;
    settle();
  }
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;
  if (!waitBits(RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_HSI, budget)) return false;
  settle();
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CR &= ~RCC_CR_PLLON;
  if (!waitBits(RCC->CR, RCC_CR_PLLRDY, 0, budget)) return false;
  // HSEBYP may only change while HSE is off. Switch away before stopping it.
  RCC->CR &= ~RCC_CR_HSEON;
  if (!waitBits(RCC->CR, RCC_CR_HSERDY, 0, budget)) return false;

  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
  (void)RCC->AHB2ENR;
  RCC->CR |= RCC_CR_HSEBYP;
  GPIOF->BSRR = source == Source::External ? (1u << 1) : (1u << 17);
  GPIOF->OTYPER &= ~(1u << 1);
  GPIOF->OSPEEDR &= ~(3u << 2);
  GPIOF->PUPDR &= ~15u; // PF0/PF1: no pull resistors.
  GPIOF->MODER = (GPIOF->MODER & ~15u) | 3u | (1u << 2);
  if (source == Source::Internal) return true;

  RCC->CR |= RCC_CR_HSEON;
  if (!waitBits(RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY, budget)) {
    // SYSCLK is still HSI16. Stop the failed external source.
    RCC->CR &= ~RCC_CR_HSEON;
    GPIOF->BSRR = 1u << 17;
    return false;
  }
  // 168 MHz requires voltage range 1 boost and 4 Flash wait states.
  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
  (void)RCC->APB1ENR1;
  PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | PWR_CR1_VOS_0;
  if (!waitBits(PWR->SR2, PWR_SR2_VOSF, 0, budget)) return false;
  PWR->CR5 &= ~PWR_CR5_R1MODE;
  settle();
  FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_4WS;
  if (!waitBits(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS, budget)) return false;
  RCC->PLLCFGR = pll168;
  RCC->CR |= RCC_CR_PLLON;
  if (!waitBits(RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY, budget)) return false;
  // Intermediate AHB /2 limits the voltage undershoot when crossing 80 MHz.
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV2;
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
  // On timeout keep every source alive; the requested switch can finish later.
  if (!waitBits(RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL, budget)) return false;
  settle();
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  return true;
}

uint32_t Clock::frequencyHz() {
  if (RCC->CFGR & (RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2)) return 0;
  switch (RCC->CFGR & RCC_CFGR_SWS) {
  case RCC_CFGR_SWS_HSI: return 16000000u;
  case RCC_CFGR_SWS_PLL:
    return RCC->PLLCFGR == pll168 && (RCC->CR & RCC_CR_HSEBYP) ? 168000000u : 0u;
  case RCC_CFGR_SWS_HSE:
    return (RCC->CR & RCC_CR_HSEBYP) ? 8000000u : 0u;
  default: return 0;
  }
}
