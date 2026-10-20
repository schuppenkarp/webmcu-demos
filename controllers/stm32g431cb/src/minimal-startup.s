// Nur Stackpointer und Reset-Einstieg. Keine IRQs, RAM- oder C++-Initialisierung.
.syntax unified
.cpu cortex-m4
.thumb

.section .isr_vector,"a",%progbits
.global g_pfnVectors
g_pfnVectors:
  .word _estack
  .word Reset_Handler

.section .text.Reset_Handler,"ax",%progbits
.global Reset_Handler
.type Reset_Handler,%function
.thumb_func
Reset_Handler:
  b main
.size Reset_Handler, .-Reset_Handler
