.syntax unified
.cpu cortex-m4
.thumb
.section .isr_vector,"a",%progbits
.balign 512
.global g_pfnVectors
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .rept 116
  .word Default_Handler
  .endr

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
