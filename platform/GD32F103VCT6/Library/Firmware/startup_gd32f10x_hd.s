;/**
;  ******************************************************************************
;  * @file    startup_gd32f10x_hd.s
;  * @author  MCU SD
;  * @version V1.0
;  * @date    26-Dec-2014
;  * @brief   GD32F10X startup code.
;  ******************************************************************************
;  */

;/* <<< Use Configuration Wizard in Context Menu >>>                                                        */

; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs
;// <h> Stack Configuration
;//   <o> Stack Size (in Bytes) <0-8192:8>
;// </h>
Stack_Size          EQU     0x400

                    AREA    STACK, NOINIT, READWRITE, ALIGN = 3
Stack_Mem           SPACE   Stack_Size
__initial_sp

;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0-4096:8>
;// </h>
Heap_Size           EQU     0x200

                    AREA    HEAP, NOINIT, READWRITE, ALIGN = 3
__heap_base
Heap_Mem            SPACE   Heap_Size
__heap_limit


                    PRESERVE8
                    THUMB

; Vector table entries with the exceptions ISR address
                    AREA    RESET, DATA, READONLY
                    EXPORT  __Vectors
                    EXPORT  __Vectors_End
                    EXPORT  __Vectors_Size

__Vectors           DCD     __initial_sp                      ; Top of Stack
                    DCD     Reset_Handler                     ; Vector Number 1,Reset Handler
                    DCD     NMI_Handler                       ; Vector Number 2,NMI Handler
                    DCD     HardFault_Handler                 ; Vector Number 3,Hard Fault Handler
                    DCD     MemManage_Handler                 ; Vector Number 4,MPU Fault Handler
                    DCD     BusFault_Handler                  ; Vector Number 5,Bus Fault Handler
                    DCD     UsageFault_Handler                ; Vector Number 6,Usage Fault Handler
                    DCD     0                                 ; Reserved
                    DCD     0                                 ; Reserved
                    DCD     0                                 ; Reserved
                    DCD     0                                 ; Reserved
                    DCD     SVC_Handler                       ; Vector Number 11,SVCall Handler
                    DCD     DebugMon_Handler                  ; Vector Number 12,Debug Monitor Handler
                    DCD     0                                 ; Reserved
                    DCD     PendSV_Handler                    ; Vector Number 14,PendSV Handler
                    DCD     SysTick_Handler                   ; Vector Number 15,SysTick Handler

                    ; External Interrupts
                    DCD     WWDG_IRQHandler                   ; Vector Number 16,Window Watchdog
                    DCD     LVD_IRQHandler                    ; Vector Number 17,LVD through EXTI Line detect
                    DCD     TAMPER_IRQHandler                 ; Vector Number 18,Tamper Interrupt   
                    DCD     RTC_IRQHandler                    ; Vector Number 19,RTC through EXTI Line
                    DCD     FMC_IRQHandler                    ; Vector Number 20,FMC
                    DCD     RCC_IRQHandler                    ; Vector Number 21,RCC
                    DCD     EXTI0_IRQHandler                  ; Vector Number 22,EXTI Line 0
                    DCD     EXTI1_IRQHandler                  ; Vector Number 23,EXTI Line 1
                    DCD     EXTI2_IRQHandler                  ; Vector Number 24,EXTI Line 2
                    DCD     EXTI3_IRQHandler                  ; Vector Number 25,EXTI Line 3
                    DCD     EXTI4_IRQHandler                  ; Vector Number 26,EXTI Line 4
                    DCD     DMA1_Channel1_IRQHandler          ; Vector Number 27,DMA1 Channel 1
                    DCD     DMA1_Channel2_IRQHandler          ; Vector Number 28,DMA1 Channel 2
                    DCD     DMA1_Channel3_IRQHandler          ; Vector Number 29,DMA1 Channel 3
                    DCD     DMA1_Channel4_IRQHandler          ; Vector Number 30,DMA1 Channel 4
                    DCD     DMA1_Channel5_IRQHandler          ; Vector Number 31,DMA1 Channel 5
                    DCD     DMA1_Channel6_IRQHandler          ; Vector Number 32,DMA1 Channel 6 
                    DCD     DMA1_Channel7_IRQHandler          ; Vector Number 33,DMA1 Channel 7
                    DCD     ADC1_2_IRQHandler                 ; Vector Number 34,ADC1 and ADC2
                    DCD     USB_HP_CAN1_TX_IRQHandler         ; Vector Number 35,USB Device High Priority or CAN1 TX
                    DCD     USB_LP_CAN1_RX0_IRQHandler        ; Vector Number 36,USB Device Low Priority or CAN1 RX0
                    DCD     CAN1_RX1_IRQHandler               ; Vector Number 37,CAN1 RX1
                    DCD     CAN1_SCE_IRQHandler               ; Vector Number 38,CAN1 SCE
                    DCD     EXTI9_5_IRQHandler                ; Vector Number 39,EXTI Line 9..5
                    DCD     TIMER1_BRK_IRQHandler             ; Vector Number 40,TIMER1 Break
                    DCD     TIMER1_UP_IRQHandler              ; Vector Number 41,TIMER1 Update
                    DCD     TIMER1_TRG_COM_IRQHandler         ; Vector Number 42,TIMER1 Break, Update, Trigger and Commutation
                    DCD     TIMER1_CC_IRQHandler              ; Vector Number 43,TIMER1 Capture Compare
                    DCD     TIMER2_IRQHandler                 ; Vector Number 44,TIMER2
                    DCD     TIMER3_IRQHandler                 ; Vector Number 45,TIMER3
                    DCD     TIMER4_IRQHandler                 ; Vector Number 46,TIMER4
                    DCD     I2C1_EV_IRQHandler                ; Vector Number 47,I2C1 Event
                    DCD     I2C1_ER_IRQHandler                ; Vector Number 48,I2C1 Error
                    DCD     I2C2_EV_IRQHandler                ; Vector Number 49,I2C2 Event
                    DCD     I2C2_ER_IRQHandler                ; Vector Number 50,I2C1 Error
                    DCD     SPI1_IRQHandler                   ; Vector Number 51,SPI1
                    DCD     SPI2_IRQHandler                   ; Vector Number 52,SPI2
                    DCD     USART1_IRQHandler                 ; Vector Number 53,USART1
                    DCD     USART2_IRQHandler                 ; Vector Number 54,USART2
                    DCD     USART3_IRQHandler                 ; Vector Number 55,USART3
                    DCD     EXTI15_10_IRQHandler              ; Vector Number 56,External Line[15:10]
	                DCD     RTCAlarm_IRQHandler               ; Vector Number 57,RTC Alarm through EXTI Line
                    DCD     USBWakeUp_IRQHandler              ; Vector Number 58,USB Device WakeUp from suspend through EXTI Line
                    DCD     TIMER8_BRK_IRQHandler             ; Vector Number 59,TIMER8 Break
                    DCD     TIMER8_UP_IRQHandler              ; Vector Number 60,TIMER8 Update
                    DCD     TIMER8_TRG_COM_IRQHandler         ; Vector Number 61,TIMER8 Trigger and Commutation
                    DCD     TIMER8_CC_IRQHandler              ; Vector Number 62,TIMER8 Capture Compare
                    DCD     ADC3_IRQHandler                   ; Vector Number 63,ADC3
                    DCD	    EXMC_IRQHandler                   ; Vector Number 64,EXMC 
                    DCD     SDIO_IRQHandler                   ; Vector Number 65,SDIO
                    DCD     TIMER5_IRQHandler                 ; Vector Number 66,TIMER5
                    DCD     SPI3_IRQHandler                   ; Vector Number 67,SPI3
                    DCD     UART4_IRQHandler                  ; Vector Number 68,UART4
                    DCD     UART5_IRQHandler                  ; Vector Number 69,UART5
                    DCD     TIMER6_IRQHandler                 ; Vector Number 70,TIMER6
                    DCD     TIMER7_IRQHandler                 ; Vector Number 71,TIMER7
                    DCD     DMA2_Channel1_IRQHandler          ; Vector Number 72,DMA2 Channel1
                    DCD     DMA2_Channel2_IRQHandler          ; Vector Number 73,DMA2 Channel2
                    DCD     DMA2_Channel3_IRQHandler          ; Vector Number 74,DMA2 Channel3
                    DCD     DMA2_Channel4_5_IRQHandler        ; Vector Number 75,DMA2 Channel4 and Channel5



__Vectors_End

__Vectors_Size      EQU  __Vectors_End - __Vectors

                    AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler       PROC
                    EXPORT  Reset_Handler                     [WEAK]
                    IMPORT  __main
                    IMPORT  SystemInit  
                    LDR     R0, =SystemInit
                    BLX     R0
                    LDR     R0, =__main
                    BX      R0
                    ENDP

; Dummy Exception Handlers
NMI_Handler         PROC
                    EXPORT  NMI_Handler                         [WEAK]
                    B       .
                    ENDP

HardFault_Handler   PROC
                    EXPORT  HardFault_Handler                   [WEAK]
                    B       .
                    ENDP

MemManage_Handler   PROC
                    EXPORT  MemManage_Handler                   [WEAK]
                    B       .
                    ENDP

BusFault_Handler    PROC
                    EXPORT  BusFault_Handler                    [WEAK]
                    B       .
                    ENDP

UsageFault_Handler  PROC
                    EXPORT  UsageFault_Handler                  [WEAK]
                    B       .
                    ENDP

SVC_Handler         PROC
                    EXPORT  SVC_Handler                         [WEAK]
                    B       .
                    ENDP

DebugMon_Handler    PROC
                    EXPORT  DebugMon_Handler                    [WEAK]
                    B       .
                    ENDP

PendSV_Handler      PROC
                    EXPORT  PendSV_Handler                      [WEAK]
                    B       .
                    ENDP

SysTick_Handler     PROC
                    EXPORT  SysTick_Handler                     [WEAK]
                    B       .
                    ENDP

Default_Handler     PROC
                    EXPORT  WWDG_IRQHandler                     [WEAK]
                    EXPORT  LVD_IRQHandler                      [WEAK]
                    EXPORT  TAMPER_IRQHandler                   [WEAK]
                    EXPORT  RTC_IRQHandler                      [WEAK]
                    EXPORT  FMC_IRQHandler                      [WEAK]
                    EXPORT  RCC_IRQHandler                      [WEAK]
                    EXPORT  EXTI0_IRQHandler                    [WEAK]
                    EXPORT  EXTI1_IRQHandler                    [WEAK]
                    EXPORT  EXTI2_IRQHandler                    [WEAK]
                    EXPORT  EXTI3_IRQHandler                    [WEAK]
                    EXPORT  EXTI4_IRQHandler                    [WEAK]
                    EXPORT  DMA1_Channel1_IRQHandler            [WEAK]
                    EXPORT  DMA1_Channel2_IRQHandler            [WEAK]
                    EXPORT  DMA1_Channel3_IRQHandler            [WEAK]
                    EXPORT  DMA1_Channel4_IRQHandler            [WEAK]
                    EXPORT  DMA1_Channel5_IRQHandler            [WEAK]
                    EXPORT  DMA1_Channel6_IRQHandler            [WEAK]
                    EXPORT  DMA1_Channel7_IRQHandler            [WEAK]
                    EXPORT  ADC1_2_IRQHandler                   [WEAK]
                    EXPORT  USB_HP_CAN1_TX_IRQHandler           [WEAK]
                    EXPORT  USB_LP_CAN1_RX0_IRQHandler          [WEAK]
                    EXPORT  CAN1_RX1_IRQHandler                 [WEAK]
                    EXPORT  CAN1_SCE_IRQHandler                 [WEAK]
                    EXPORT  EXTI9_5_IRQHandler                  [WEAK]
                    EXPORT  TIMER1_BRK_IRQHandler               [WEAK]
                    EXPORT  TIMER1_UP_IRQHandler                [WEAK]
                    EXPORT  TIMER1_TRG_COM_IRQHandler           [WEAK]
                    EXPORT  TIMER1_CC_IRQHandler                [WEAK]
                    EXPORT  TIMER2_IRQHandler                   [WEAK]
                    EXPORT  TIMER3_IRQHandler                   [WEAK]
                    EXPORT  TIMER4_IRQHandler                   [WEAK]
                    EXPORT  I2C1_EV_IRQHandler                  [WEAK]
                    EXPORT  I2C1_ER_IRQHandler                  [WEAK]
                    EXPORT  I2C2_EV_IRQHandler                  [WEAK]
                    EXPORT  I2C2_ER_IRQHandler                  [WEAK]
                    EXPORT  SPI1_IRQHandler                     [WEAK]
                    EXPORT  SPI2_IRQHandler                     [WEAK]
                    EXPORT  USART1_IRQHandler                   [WEAK]
                    EXPORT  USART2_IRQHandler                   [WEAK]
                    EXPORT  USART3_IRQHandler                   [WEAK]
                    EXPORT  EXTI15_10_IRQHandler                [WEAK]
                    EXPORT  RTCAlarm_IRQHandler                 [WEAK]
                    EXPORT  USBWakeUp_IRQHandler                [WEAK]
                    EXPORT  TIMER8_BRK_IRQHandler               [WEAK]
                    EXPORT  TIMER8_UP_IRQHandler                [WEAK]
                    EXPORT  TIMER8_TRG_COM_IRQHandler           [WEAK]
                    EXPORT  TIMER8_CC_IRQHandler                [WEAK]
                    EXPORT  ADC3_IRQHandler                     [WEAK]
                    EXPORT  EXMC_IRQHandler                     [WEAK]
                    EXPORT  SDIO_IRQHandler                     [WEAK]
                    EXPORT  TIMER5_IRQHandler                   [WEAK]
                    EXPORT  SPI3_IRQHandler                     [WEAK]
                    EXPORT  UART4_IRQHandler                    [WEAK]
                    EXPORT  UART5_IRQHandler                    [WEAK]
                    EXPORT  TIMER6_IRQHandler                   [WEAK]
                    EXPORT  TIMER7_IRQHandler                   [WEAK]
                    EXPORT  DMA2_Channel1_IRQHandler            [WEAK]
                    EXPORT  DMA2_Channel2_IRQHandler            [WEAK]
                    EXPORT  DMA2_Channel3_IRQHandler            [WEAK]
                    EXPORT  DMA2_Channel4_5_IRQHandler          [WEAK]


WWDG_IRQHandler
LVD_IRQHandler
TAMPER_IRQHandler
RTC_IRQHandler
FMC_IRQHandler 
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
ADC1_2_IRQHandler
USB_HP_CAN1_TX_IRQHandler
USB_LP_CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIMER1_BRK_IRQHandler
TIMER1_UP_IRQHandler
TIMER1_TRG_COM_IRQHandler
TIMER1_CC_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
TIMER4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTCAlarm_IRQHandler
USBWakeUp_IRQHandler
TIMER8_BRK_IRQHandler
TIMER8_UP_IRQHandler
TIMER8_TRG_COM_IRQHandler
TIMER8_CC_IRQHandler
ADC3_IRQHandler
EXMC_IRQHandler
SDIO_IRQHandler
TIMER5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIMER6_IRQHandler
TIMER7_IRQHandler
DMA2_Channel1_IRQHandler
DMA2_Channel2_IRQHandler
DMA2_Channel3_IRQHandler
DMA2_Channel4_5_IRQHandler
                B       .
                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
