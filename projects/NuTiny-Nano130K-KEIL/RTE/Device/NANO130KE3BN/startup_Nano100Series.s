;/******************************************************************************
; * @file     startup_Nano100Series.s
; * @version  V1.00
; * $Revision: 4 $
; * $Date: 15/06/08 5:12p $ 
; * @brief    CMSIS ARM Cortex-M0 Core Device Startup File
; *
; * @note
; * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
;*****************************************************************************/  


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
    IF :LNOT: :DEF: Stack_Size
Stack_Size      EQU     0x00000500
    ENDIF
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
    IF :LNOT: :DEF: Heap_Size
Heap_Size       EQU     0x00000500
    ENDIF
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     BOD_IRQHandler            ; Brownout low voltage detected interrupt  
                DCD     WDT_IRQHandler            ; Watch Dog Timer interrupt  
                DCD     EINT0_IRQHandler          ; External signal interrupt from PB.14 pin
                DCD     EINT1_IRQHandler          ; External signal interrupt from PB.15 pin
                DCD     GPABC_IRQHandler          ; External interrupt from PA[15:0]/PB[15:0]/PC[15:0]  
                DCD     GPDEF_IRQHandler          ; External interrupt from PD[15:0]/PE[15:0]/PF[7:0]
                DCD     PWM0_IRQHandler           ; PWM 0 interrupt 
                DCD     PWM1_IRQHandler           ; PWM 1 interrupt  
                DCD     TMR0_IRQHandler           ; Timer 0 interrupt
                DCD     TMR1_IRQHandler           ; Timer 1 interrupt  
                DCD     TMR2_IRQHandler           ; Timer 2 interrupt 
                DCD     TMR3_IRQHandler           ; Timer 3 interrupt 
                DCD     UART0_IRQHandler          ; UART0 interrupt
                DCD     UART1_IRQHandler          ; UART1 interrupt
                DCD     SPI0_IRQHandler           ; SPI0 interrupt 
                DCD     SPI1_IRQHandler           ; SPI1 interrupt 
                DCD     SPI2_IRQHandler           ; SPI2 interrupt 
                DCD     HIRC_IRQHandler           ; HIRC interrupt 
                DCD     I2C0_IRQHandler           ; I2C0 interrupt 
                DCD     I2C1_IRQHandler           ; I2C1 interrupt 
                DCD     SC2_IRQHandler            ; SC2 interrupt
                DCD     SC0_IRQHandler            ; SC0 interrupt
                DCD     SC1_IRQHandler            ; SC1 interrupt
                DCD     USBD_IRQHandler           ; USB FS Device interrupt 
                DCD     0                         ; Reserved  
                DCD     LCD_IRQHandler            ; LCD interrupt 
                DCD     PDMA_IRQHandler           ; PDMA interrupt
                DCD     I2S_IRQHandler            ; I2S interrupt 
                DCD     PDWU_IRQHandler           ; Power Down Wake up interrupt
                DCD     ADC_IRQHandler            ; ADC interrupt
                DCD     DAC_IRQHandler            ; DAC interrupt
                DCD     RTC_IRQHandler            ; Real time clock interrupt
                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]                
                IMPORT  __main
                ; Unlock Register
                LDR     R0, =0x50000100
                LDR     R1, =0x59
                STR     R1, [R0]
                LDR     R1, =0x16
                STR     R1, [R0]
                LDR     R1, =0x88
                STR     R1, [R0]

                ; Init POR
                LDR     R2, =0x50000060
                LDR     R1, =0x00005AA5
                STR     R1, [R2]

                ; Lock register
                MOVS    R1, #0
                STR     R1, [R0]

                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)                

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  GPABC_IRQHandler          [WEAK]
                EXPORT  GPDEF_IRQHandler          [WEAK]
                EXPORT  PWM0_IRQHandler           [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  TMR0_IRQHandler           [WEAK]
                EXPORT  TMR1_IRQHandler           [WEAK]
                EXPORT  TMR2_IRQHandler           [WEAK]
                EXPORT  TMR3_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  SPI0_IRQHandler           [WEAK]
                EXPORT  SPI1_IRQHandler           [WEAK]
                EXPORT  SPI2_IRQHandler           [WEAK]
                EXPORT  HIRC_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  SC2_IRQHandler            [WEAK]
                EXPORT  SC0_IRQHandler            [WEAK]
                EXPORT  SC1_IRQHandler            [WEAK]
                EXPORT  USBD_IRQHandler           [WEAK]
                EXPORT  LCD_IRQHandler            [WEAK]
                EXPORT  PDMA_IRQHandler           [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  PDWU_IRQHandler           [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  DAC_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                
BOD_IRQHandler
WDT_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
GPABC_IRQHandler
GPDEF_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
TMR0_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
HIRC_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SC2_IRQHandler
SC0_IRQHandler    
SC1_IRQHandler
USBD_IRQHandler
LCD_IRQHandler
PDMA_IRQHandler
I2S_IRQHandler
PDWU_IRQHandler
ADC_IRQHandler
DAC_IRQHandler
RTC_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

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
