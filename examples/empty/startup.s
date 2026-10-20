/* STM32G431xx vector names/order from STMicroelectronics CMSIS device G4
 * commit 626ee412334a5ed2e5b320af5a8d77d69f03a558, GCC startup_stm32g431xx.s.
 * Copyright (c) 2019 STMicroelectronics. License: include/ST-LICENSE.md.
 * WebMCU reset path initializes data, BSS and C++ constructors without libc.
 * In C++ define e.g. extern "C" void USART1_IRQHandler() { ... }
 * without weak/static. Unimplemented interrupts stop in Default_Handler.
 */
.syntax unified
.cpu cortex-m4
.thumb
.section .isr_vector,"a",%progbits
.balign 512
.global g_pfnVectors
.type g_pfnVectors,%object
g_pfnVectors:
  .word _estack // Core vector 0
  .word Reset_Handler // Core vector 1
  .word NMI_Handler // Core vector 2
  .word HardFault_Handler // Core vector 3
  .word MemManage_Handler // Core vector 4
  .word BusFault_Handler // Core vector 5
  .word UsageFault_Handler // Core vector 6
  .word 0 // Core vector 7 (reserved)
  .word 0 // Core vector 8 (reserved)
  .word 0 // Core vector 9 (reserved)
  .word 0 // Core vector 10 (reserved)
  .word SVC_Handler // Core vector 11
  .word DebugMon_Handler // Core vector 12
  .word 0 // Core vector 13 (reserved)
  .word PendSV_Handler // Core vector 14
  .word SysTick_Handler // Core vector 15
  .word WWDG_IRQHandler // IRQ 0
  .word PVD_PVM_IRQHandler // IRQ 1
  .word RTC_TAMP_LSECSS_IRQHandler // IRQ 2
  .word RTC_WKUP_IRQHandler // IRQ 3
  .word FLASH_IRQHandler // IRQ 4
  .word RCC_IRQHandler // IRQ 5
  .word EXTI0_IRQHandler // IRQ 6
  .word EXTI1_IRQHandler // IRQ 7
  .word EXTI2_IRQHandler // IRQ 8
  .word EXTI3_IRQHandler // IRQ 9
  .word EXTI4_IRQHandler // IRQ 10
  .word DMA1_Channel1_IRQHandler // IRQ 11
  .word DMA1_Channel2_IRQHandler // IRQ 12
  .word DMA1_Channel3_IRQHandler // IRQ 13
  .word DMA1_Channel4_IRQHandler // IRQ 14
  .word DMA1_Channel5_IRQHandler // IRQ 15
  .word DMA1_Channel6_IRQHandler // IRQ 16
  .word 0 // IRQ 17 (reserved)
  .word ADC1_2_IRQHandler // IRQ 18
  .word USB_HP_IRQHandler // IRQ 19
  .word USB_LP_IRQHandler // IRQ 20
  .word FDCAN1_IT0_IRQHandler // IRQ 21
  .word FDCAN1_IT1_IRQHandler // IRQ 22
  .word EXTI9_5_IRQHandler // IRQ 23
  .word TIM1_BRK_TIM15_IRQHandler // IRQ 24
  .word TIM1_UP_TIM16_IRQHandler // IRQ 25
  .word TIM1_TRG_COM_TIM17_IRQHandler // IRQ 26
  .word TIM1_CC_IRQHandler // IRQ 27
  .word TIM2_IRQHandler // IRQ 28
  .word TIM3_IRQHandler // IRQ 29
  .word TIM4_IRQHandler // IRQ 30
  .word I2C1_EV_IRQHandler // IRQ 31
  .word I2C1_ER_IRQHandler // IRQ 32
  .word I2C2_EV_IRQHandler // IRQ 33
  .word I2C2_ER_IRQHandler // IRQ 34
  .word SPI1_IRQHandler // IRQ 35
  .word SPI2_IRQHandler // IRQ 36
  .word USART1_IRQHandler // IRQ 37
  .word USART2_IRQHandler // IRQ 38
  .word USART3_IRQHandler // IRQ 39
  .word EXTI15_10_IRQHandler // IRQ 40
  .word RTC_Alarm_IRQHandler // IRQ 41
  .word USBWakeUp_IRQHandler // IRQ 42
  .word TIM8_BRK_IRQHandler // IRQ 43
  .word TIM8_UP_IRQHandler // IRQ 44
  .word TIM8_TRG_COM_IRQHandler // IRQ 45
  .word TIM8_CC_IRQHandler // IRQ 46
  .word 0 // IRQ 47 (reserved)
  .word 0 // IRQ 48 (reserved)
  .word LPTIM1_IRQHandler // IRQ 49
  .word 0 // IRQ 50 (reserved)
  .word SPI3_IRQHandler // IRQ 51
  .word UART4_IRQHandler // IRQ 52
  .word 0 // IRQ 53 (reserved)
  .word TIM6_DAC_IRQHandler // IRQ 54
  .word TIM7_IRQHandler // IRQ 55
  .word DMA2_Channel1_IRQHandler // IRQ 56
  .word DMA2_Channel2_IRQHandler // IRQ 57
  .word DMA2_Channel3_IRQHandler // IRQ 58
  .word DMA2_Channel4_IRQHandler // IRQ 59
  .word DMA2_Channel5_IRQHandler // IRQ 60
  .word 0 // IRQ 61 (reserved)
  .word 0 // IRQ 62 (reserved)
  .word UCPD1_IRQHandler // IRQ 63
  .word COMP1_2_3_IRQHandler // IRQ 64
  .word COMP4_IRQHandler // IRQ 65
  .word 0 // IRQ 66 (reserved)
  .word 0 // IRQ 67 (reserved)
  .word 0 // IRQ 68 (reserved)
  .word 0 // IRQ 69 (reserved)
  .word 0 // IRQ 70 (reserved)
  .word 0 // IRQ 71 (reserved)
  .word 0 // IRQ 72 (reserved)
  .word 0 // IRQ 73 (reserved)
  .word 0 // IRQ 74 (reserved)
  .word CRS_IRQHandler // IRQ 75
  .word SAI1_IRQHandler // IRQ 76
  .word 0 // IRQ 77 (reserved)
  .word 0 // IRQ 78 (reserved)
  .word 0 // IRQ 79 (reserved)
  .word 0 // IRQ 80 (reserved)
  .word FPU_IRQHandler // IRQ 81
  .word 0 // IRQ 82 (reserved)
  .word 0 // IRQ 83 (reserved)
  .word 0 // IRQ 84 (reserved)
  .word 0 // IRQ 85 (reserved)
  .word 0 // IRQ 86 (reserved)
  .word 0 // IRQ 87 (reserved)
  .word 0 // IRQ 88 (reserved)
  .word 0 // IRQ 89 (reserved)
  .word RNG_IRQHandler // IRQ 90
  .word LPUART1_IRQHandler // IRQ 91
  .word I2C3_EV_IRQHandler // IRQ 92
  .word I2C3_ER_IRQHandler // IRQ 93
  .word DMAMUX_OVR_IRQHandler // IRQ 94
  .word 0 // IRQ 95 (reserved)
  .word 0 // IRQ 96 (reserved)
  .word DMA2_Channel6_IRQHandler // IRQ 97
  .word 0 // IRQ 98 (reserved)
  .word 0 // IRQ 99 (reserved)
  .word CORDIC_IRQHandler // IRQ 100
  .word FMAC_IRQHandler // IRQ 101
.size g_pfnVectors, .-g_pfnVectors

.section .text.Reset_Handler,"ax",%progbits
.global Reset_Handler
.type Reset_Handler,%function
.thumb_func
Reset_Handler:
  ldr r0, =0xe000ed08
  ldr r1, =g_pfnVectors
  str r1, [r0]
  dsb
  isb
  ldr r0, =_sidata
  ldr r1, =_sdata
  ldr r2, =_edata
1:
  cmp r1, r2
  bcs 2f
  ldr r3, [r0], #4
  str r3, [r1], #4
  b 1b
2:
  ldr r1, =_sbss
  ldr r2, =_ebss
  movs r3, #0
3:
  cmp r1, r2
  bcs 4f
  str r3, [r1], #4
  b 3b
4:
  // Run C++ static constructors (no libc needed).
  ldr r4, =__init_array_start
  ldr r5, =__init_array_end
5:
  cmp r4, r5
  bcs 6f
  ldr r3, [r4], #4
  blx r3
  b 5b
6:
  bl main
  b Default_Handler
.size Reset_Handler, .-Reset_Handler

.section .text.Default_Handler,"ax",%progbits
.global Default_Handler
.type Default_Handler,%function
.thumb_func
Default_Handler:
  b Default_Handler
.size Default_Handler, .-Default_Handler

// Weak Thumb aliases: any strong C-linkage handler overrides its default.
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.weak MemManage_Handler
.thumb_set MemManage_Handler, Default_Handler

.weak BusFault_Handler
.thumb_set BusFault_Handler, Default_Handler

.weak UsageFault_Handler
.thumb_set UsageFault_Handler, Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler

.weak DebugMon_Handler
.thumb_set DebugMon_Handler, Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler

.weak WWDG_IRQHandler
.thumb_set WWDG_IRQHandler, Default_Handler

.weak PVD_PVM_IRQHandler
.thumb_set PVD_PVM_IRQHandler, Default_Handler

.weak RTC_TAMP_LSECSS_IRQHandler
.thumb_set RTC_TAMP_LSECSS_IRQHandler, Default_Handler

.weak RTC_WKUP_IRQHandler
.thumb_set RTC_WKUP_IRQHandler, Default_Handler

.weak FLASH_IRQHandler
.thumb_set FLASH_IRQHandler, Default_Handler

.weak RCC_IRQHandler
.thumb_set RCC_IRQHandler, Default_Handler

.weak EXTI0_IRQHandler
.thumb_set EXTI0_IRQHandler, Default_Handler

.weak EXTI1_IRQHandler
.thumb_set EXTI1_IRQHandler, Default_Handler

.weak EXTI2_IRQHandler
.thumb_set EXTI2_IRQHandler, Default_Handler

.weak EXTI3_IRQHandler
.thumb_set EXTI3_IRQHandler, Default_Handler

.weak EXTI4_IRQHandler
.thumb_set EXTI4_IRQHandler, Default_Handler

.weak DMA1_Channel1_IRQHandler
.thumb_set DMA1_Channel1_IRQHandler, Default_Handler

.weak DMA1_Channel2_IRQHandler
.thumb_set DMA1_Channel2_IRQHandler, Default_Handler

.weak DMA1_Channel3_IRQHandler
.thumb_set DMA1_Channel3_IRQHandler, Default_Handler

.weak DMA1_Channel4_IRQHandler
.thumb_set DMA1_Channel4_IRQHandler, Default_Handler

.weak DMA1_Channel5_IRQHandler
.thumb_set DMA1_Channel5_IRQHandler, Default_Handler

.weak DMA1_Channel6_IRQHandler
.thumb_set DMA1_Channel6_IRQHandler, Default_Handler

.weak ADC1_2_IRQHandler
.thumb_set ADC1_2_IRQHandler, Default_Handler

.weak USB_HP_IRQHandler
.thumb_set USB_HP_IRQHandler, Default_Handler

.weak USB_LP_IRQHandler
.thumb_set USB_LP_IRQHandler, Default_Handler

.weak FDCAN1_IT0_IRQHandler
.thumb_set FDCAN1_IT0_IRQHandler, Default_Handler

.weak FDCAN1_IT1_IRQHandler
.thumb_set FDCAN1_IT1_IRQHandler, Default_Handler

.weak EXTI9_5_IRQHandler
.thumb_set EXTI9_5_IRQHandler, Default_Handler

.weak TIM1_BRK_TIM15_IRQHandler
.thumb_set TIM1_BRK_TIM15_IRQHandler, Default_Handler

.weak TIM1_UP_TIM16_IRQHandler
.thumb_set TIM1_UP_TIM16_IRQHandler, Default_Handler

.weak TIM1_TRG_COM_TIM17_IRQHandler
.thumb_set TIM1_TRG_COM_TIM17_IRQHandler, Default_Handler

.weak TIM1_CC_IRQHandler
.thumb_set TIM1_CC_IRQHandler, Default_Handler

.weak TIM2_IRQHandler
.thumb_set TIM2_IRQHandler, Default_Handler

.weak TIM3_IRQHandler
.thumb_set TIM3_IRQHandler, Default_Handler

.weak TIM4_IRQHandler
.thumb_set TIM4_IRQHandler, Default_Handler

.weak I2C1_EV_IRQHandler
.thumb_set I2C1_EV_IRQHandler, Default_Handler

.weak I2C1_ER_IRQHandler
.thumb_set I2C1_ER_IRQHandler, Default_Handler

.weak I2C2_EV_IRQHandler
.thumb_set I2C2_EV_IRQHandler, Default_Handler

.weak I2C2_ER_IRQHandler
.thumb_set I2C2_ER_IRQHandler, Default_Handler

.weak SPI1_IRQHandler
.thumb_set SPI1_IRQHandler, Default_Handler

.weak SPI2_IRQHandler
.thumb_set SPI2_IRQHandler, Default_Handler

.weak USART1_IRQHandler
.thumb_set USART1_IRQHandler, Default_Handler

.weak USART2_IRQHandler
.thumb_set USART2_IRQHandler, Default_Handler

.weak USART3_IRQHandler
.thumb_set USART3_IRQHandler, Default_Handler

.weak EXTI15_10_IRQHandler
.thumb_set EXTI15_10_IRQHandler, Default_Handler

.weak RTC_Alarm_IRQHandler
.thumb_set RTC_Alarm_IRQHandler, Default_Handler

.weak USBWakeUp_IRQHandler
.thumb_set USBWakeUp_IRQHandler, Default_Handler

.weak TIM8_BRK_IRQHandler
.thumb_set TIM8_BRK_IRQHandler, Default_Handler

.weak TIM8_UP_IRQHandler
.thumb_set TIM8_UP_IRQHandler, Default_Handler

.weak TIM8_TRG_COM_IRQHandler
.thumb_set TIM8_TRG_COM_IRQHandler, Default_Handler

.weak TIM8_CC_IRQHandler
.thumb_set TIM8_CC_IRQHandler, Default_Handler

.weak LPTIM1_IRQHandler
.thumb_set LPTIM1_IRQHandler, Default_Handler

.weak SPI3_IRQHandler
.thumb_set SPI3_IRQHandler, Default_Handler

.weak UART4_IRQHandler
.thumb_set UART4_IRQHandler, Default_Handler

.weak TIM6_DAC_IRQHandler
.thumb_set TIM6_DAC_IRQHandler, Default_Handler

.weak TIM7_IRQHandler
.thumb_set TIM7_IRQHandler, Default_Handler

.weak DMA2_Channel1_IRQHandler
.thumb_set DMA2_Channel1_IRQHandler, Default_Handler

.weak DMA2_Channel2_IRQHandler
.thumb_set DMA2_Channel2_IRQHandler, Default_Handler

.weak DMA2_Channel3_IRQHandler
.thumb_set DMA2_Channel3_IRQHandler, Default_Handler

.weak DMA2_Channel4_IRQHandler
.thumb_set DMA2_Channel4_IRQHandler, Default_Handler

.weak DMA2_Channel5_IRQHandler
.thumb_set DMA2_Channel5_IRQHandler, Default_Handler

.weak UCPD1_IRQHandler
.thumb_set UCPD1_IRQHandler, Default_Handler

.weak COMP1_2_3_IRQHandler
.thumb_set COMP1_2_3_IRQHandler, Default_Handler

.weak COMP4_IRQHandler
.thumb_set COMP4_IRQHandler, Default_Handler

.weak CRS_IRQHandler
.thumb_set CRS_IRQHandler, Default_Handler

.weak SAI1_IRQHandler
.thumb_set SAI1_IRQHandler, Default_Handler

.weak FPU_IRQHandler
.thumb_set FPU_IRQHandler, Default_Handler

.weak RNG_IRQHandler
.thumb_set RNG_IRQHandler, Default_Handler

.weak LPUART1_IRQHandler
.thumb_set LPUART1_IRQHandler, Default_Handler

.weak I2C3_EV_IRQHandler
.thumb_set I2C3_EV_IRQHandler, Default_Handler

.weak I2C3_ER_IRQHandler
.thumb_set I2C3_ER_IRQHandler, Default_Handler

.weak DMAMUX_OVR_IRQHandler
.thumb_set DMAMUX_OVR_IRQHandler, Default_Handler

.weak DMA2_Channel6_IRQHandler
.thumb_set DMA2_Channel6_IRQHandler, Default_Handler

.weak CORDIC_IRQHandler
.thumb_set CORDIC_IRQHandler, Default_Handler

.weak FMAC_IRQHandler
.thumb_set FMAC_IRQHandler, Default_Handler

