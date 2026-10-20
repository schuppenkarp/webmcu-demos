# STM32G431CB-Adapter

3.0.0 implementiert die [gemeinsamen APIs](../../../libraries/README.md) mit ST-Registerzugriffen.
GPIO A–G; UART Port1=USART1, Port2=USART2, Port3=USART3, Port4=UART4, LowPower1=LPUART1.
CAN Port1=FDCAN1. GPIO-Alternate-Function-Nummern gelten für diesen Controller.
UART erwartet den tatsächlichen PCLK (USART1 PCLK2, sonst PCLK1); CAN PCLK1.

1.0.0/2.0.0 sind unveränderte ältere APIs mit direkten STM32-Registerzeigern.
Bestehende Projekte werden nicht automatisch migriert.
