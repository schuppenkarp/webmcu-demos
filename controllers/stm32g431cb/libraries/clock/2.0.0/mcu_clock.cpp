// webMCU Clock 2.0.0 | MIT | STM32G431CB adapter
// RM0440 + DS12589: PLL input 2.66..16 MHz, VCO 96..344 MHz, CPU <=170 MHz.
// No GPIO access: board oscillator enable is the project's responsibility.
#include "mcu_clock.h"
#include "include/stm32g431xx.h"

volatile Clock::Settings Clock::settings = {};
static uint32_t externalHz = 0;
static void settle() {
  // More than 1 us even at 170 MHz; no timer dependency during clock changes.
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


static uint32_t fail() {
  Clock::settings.valid = false;
  Clock::settings.actualHz = Clock::frequencyHz();
  return 0;
}
static uint32_t finish() {
  Clock::settings.actualHz = Clock::frequencyHz();
  Clock::settings.valid = Clock::settings.actualHz == Clock::settings.requestedHz;
  return Clock::settings.valid ? Clock::settings.actualHz : 0;
}
uint32_t Clock::configure(Source source, uint32_t sourceHz, uint32_t cpuHz, uint32_t budget) {
  settings.source = source;
  settings.sourceHz = sourceHz;
  settings.requestedHz = cpuHz;
  settings.valid = false;
  settings.pllM = 0; settings.pllN = 0; settings.pllR = 0;
  settings.pllEnabled = false;
  if ((source != Source::Internal && source != Source::ExternalCrystal &&
       source != Source::ExternalOscillator) || !cpuHz || cpuHz > 170000000u)
    return fail();
  if ((source == Source::Internal && sourceHz != 16000000u) ||
      (source == Source::ExternalCrystal && (sourceHz < 4000000u || sourceHz > 48000000u)) ||
      (source == Source::ExternalOscillator && (!sourceHz || sourceHz > 50000000u)))
    return fail();
  const bool usePll = sourceHz != cpuHz;
  uint32_t m = 0, n = 0, r = 0;
  if (usePll) {
    // Integer arithmetic avoids rounded solutions. Prefer the smallest M and R.
    for (uint32_t div = 1; div <= 16 && !m; ++div) {
      if (sourceHz < 2660000u * div || sourceHz > 16000000u * div) continue;
      for (uint32_t out = 2; out <= 8; out += 2) {
        const uint32_t vco = cpuHz * out; // <= 1.36 GHz, fits uint32_t.
        if (vco < 96000000u || vco > 344000000u) continue;
        for (uint32_t mul = 8; mul <= 127; ++mul) {
          if ((uint64_t)sourceHz * mul == (uint64_t)vco * div) {
            m = div; n = mul; r = out; break;
          }
        }
        if (m) break;
      }
    }
    if (!m) return fail();
  }
  const uint32_t current = RCC->CFGR & RCC_CFGR_SWS;
  // Do not disrupt PLL peripheral consumers or CSS-managed clocks.
  if ((RCC->CR & RCC_CR_CSSON) ||
      (RCC->PLLCFGR & (RCC_PLLCFGR_PLLPEN | RCC_PLLCFGR_PLLQEN)) ||
      (RCC->CFGR & (RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2)) ||
      (current != RCC_CFGR_SWS_HSI && current != RCC_CFGR_SWS_HSE && current != RCC_CFGR_SWS_PLL))
    return fail();
  const uint32_t pll = usePll ?
    (source == Source::Internal ? RCC_PLLCFGR_PLLSRC_HSI : RCC_PLLCFGR_PLLSRC_HSE) |
    ((m - 1u) << RCC_PLLCFGR_PLLM_Pos) | (n << RCC_PLLCFGR_PLLN_Pos) |
    ((r / 2u - 1u) << RCC_PLLCFGR_PLLR_Pos) | RCC_PLLCFGR_PLLREN : 0u;
  settings.pllM = m; settings.pllN = n; settings.pllR = r;
  settings.pllEnabled = usePll;
  RCC->CR |= RCC_CR_HSION;
  if (!waitBits(RCC->CR, RCC_CR_HSIRDY, RCC_CR_HSIRDY, budget)) return fail();
  if (current == RCC_CFGR_SWS_PLL) {
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV2;
    settle();
  }
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;
  if (!waitBits(RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_HSI, budget)) return fail();
  settle();
  externalHz = source == Source::Internal ? 0u : sourceHz;
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CR &= ~RCC_CR_PLLON;
  if (!waitBits(RCC->CR, RCC_CR_PLLRDY, 0, budget)) return fail();
  // HSEBYP may only change while HSE is off. Switch away before stopping it.
  RCC->CR &= ~RCC_CR_HSEON;
  if (!waitBits(RCC->CR, RCC_CR_HSERDY, 0, budget)) return fail();

  if (source == Source::Internal) {
    RCC->CR &= ~RCC_CR_HSEBYP;
  } else {
    if (source == Source::ExternalOscillator) RCC->CR |= RCC_CR_HSEBYP;
    else RCC->CR &= ~RCC_CR_HSEBYP;
    RCC->CR |= RCC_CR_HSEON;
    if (!waitBits(RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY, budget)) {
      // SYSCLK is still HSI16. Stop the failed external source.
      RCC->CR &= ~RCC_CR_HSEON;
      return fail();
    }
  }
  // Range 1 boost and 4 wait states safely cover all supported frequencies.
  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
  (void)RCC->APB1ENR1;
  PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | PWR_CR1_VOS_0;
  if (!waitBits(PWR->SR2, PWR_SR2_VOSF, 0, budget)) return fail();
  PWR->CR5 &= ~PWR_CR5_R1MODE;
  settle();
  FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_4WS;
  if (!waitBits(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS, budget)) return fail();
  if (usePll) {
    RCC->PLLCFGR = pll;
    RCC->CR |= RCC_CR_PLLON;
    if (!waitBits(RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY, budget)) return fail();
  }
  // Intermediate AHB /2 limits the voltage undershoot when crossing 80 MHz.
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV2;
  const uint32_t sw = usePll ? RCC_CFGR_SW_PLL :
      (source == Source::Internal ? RCC_CFGR_SW_HSI : RCC_CFGR_SW_HSE);
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | sw;
  // On timeout keep every source alive; the requested switch can finish later.
  const uint32_t sws = usePll ? RCC_CFGR_SWS_PLL :
      (source == Source::Internal ? RCC_CFGR_SWS_HSI : RCC_CFGR_SWS_HSE);
  if (!waitBits(RCC->CFGR, RCC_CFGR_SWS, sws, budget)) return fail();
  settle();
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  return finish();
}


uint32_t Clock::frequencyHz() {
  const uint32_t cfgr = RCC->CFGR, pll = RCC->PLLCFGR;
  uint64_t hz = 0;
  switch (cfgr & RCC_CFGR_SWS) {
  case RCC_CFGR_SWS_HSI: hz = 16000000u; break;
  case RCC_CFGR_SWS_HSE: hz = externalHz; break;
  case RCC_CFGR_SWS_PLL: {
    const uint32_t src = pll & RCC_PLLCFGR_PLLSRC;
    const uint32_t input = src == RCC_PLLCFGR_PLLSRC_HSI ? 16000000u :
                           src == RCC_PLLCFGR_PLLSRC_HSE ? externalHz : 0u;
    const uint32_t m = ((pll & RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1u;
    const uint32_t n = (pll & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
    const uint32_t r = (((pll & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1u) * 2u;
    if (!(pll & RCC_PLLCFGR_PLLREN)) return 0;
    const uint32_t divisor = m * r;
    hz = (uint64_t)(input / divisor) * n + ((input % divisor) * n) / divisor;
    break;
  }
  default: return 0;
  }
  const uint32_t hpre = (cfgr & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
  static const uint32_t divisors[8] = {2,4,8,16,64,128,256,512};
  if (hz > 170000000u) return 0;
  return (uint32_t)hz / (hpre < 8 ? 1u : divisors[hpre - 8]);
}
