;/*****************************************************************************
; * @file     startup_SAMG55.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File for
; *           Atmel SAMG55 Device Series
; * @version  V1.00
; * @date     13. October 2014
; *
; * @note
; * Copyright (C) 2014 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ;    Top of Stack
                DCD     Reset_Handler             ;    Reset Handler
                DCD     NMI_Handler               ;    NMI Handler
                DCD     HardFault_Handler         ;    Hard Fault Handler
                DCD     MemManage_Handler         ;    MPU Fault Handler
                DCD     BusFault_Handler          ;    Bus Fault Handler
                DCD     UsageFault_Handler        ;    Usage Fault Handler
                DCD     0                         ;    Reserved
                DCD     0                         ;    Reserved
                DCD     0                         ;    Reserved
                DCD     0                         ;    Reserved
                DCD     SVC_Handler               ;    SVCall Handler
                DCD     DebugMon_Handler          ;    Debug Monitor Handler
                DCD     0                         ;    Reserved
                DCD     PendSV_Handler            ;    PendSV Handler
                DCD     SysTick_Handler           ;    SysTick Handler

                ; External Interrupts
                DCD     SUPC_Handler              ;  0  Supply Controller
                DCD     RSTC_Handler              ;  1  Reset Controller
                DCD     RTC_Handler               ;  2  Real Time Clock
                DCD     RTT_Handler               ;  3  Real Time Timer
                DCD     WDT_Handler               ;  4  Watchdog Timer
                DCD     PMC_Handler               ;  5  Power Management Controller
                DCD     EFC_Handler               ;  6  Enhanced Flash Controller
                DCD     FLEXCOM7_Handler          ;  7  FLEXCOM 7
                DCD     FLEXCOM0_Handler          ;  8  FLEXCOM 0
                DCD     FLEXCOM1_Handler          ;  9  FLEXCOM 1
                DCD     0                         ; 10 Reserved
                DCD     PIOA_Handler              ; 11 Parallel I/O Controller A
                DCD     PIOB_Handler              ; 12 Parallel I/O Controller B
                DCD     PDMIC0_Handler            ; 13 PDM 0
                DCD     FLEXCOM2_Handler          ; 14 FLEXCOM2
                DCD     MEM2MEM_Handler           ; 15 MEM2MEM
                DCD     I2SC0_Handler             ; 16 I2SC0
                DCD     I2SC1_Handler             ; 17 I2SC1
                DCD     PDMIC1_Handler            ; 18 PDM 1
                DCD     FLEXCOM3_Handler          ; 19 FLEXCOM3
                DCD     FLEXCOM4_Handler          ; 20 FLEXCOM4
                DCD     FLEXCOM5_Handler          ; 21 FLEXCOM5
                DCD     FLEXCOM6_Handler          ; 22 FLEXCOM6
                DCD     TC0_Handler               ; 23 Timer/Counter 0
                DCD     TC1_Handler               ; 24 Timer/Counter 1
                DCD     TC2_Handler               ; 25 Timer/Counter 2
                DCD     TC3_Handler               ; 26 Timer/Counter 3
                DCD     TC4_Handler               ; 27 Timer/Counter 4
                DCD     TC5_Handler               ; 28 Timer/Counter 5
                DCD     ADC_Handler               ; 29 Analog To Digital Converter
                DCD     ARM_Handler               ; 30 FPU
                DCD     0                         ; 31 Reserved
                DCD     0                         ; 32 Reserved
                DCD     0                         ; 33 Reserved
                DCD     0                         ; 34 Reserved
                DCD     0                         ; 35 Reserved
                DCD     0                         ; 36 Reserved
                DCD     0                         ; 37 Reserved
                DCD     0                         ; 38 Reserved
                DCD     0                         ; 39 Reserved
                DCD     0                         ; 40 Reserved
                DCD     0                         ; 41 Reserved
                DCD     0                         ; 42 Reserved
                DCD     0                         ; 43 Reserved
                DCD     0                         ; 44 Reserved
                DCD     0                         ; 45 Reserved
                DCD     0                         ; 46 Reserved
                DCD     UHP_Handler               ; 47 USB OHCI
                DCD     UDP_Handler               ; 48 USB Device FS
                DCD     CRCCU_Handler             ; 49 CRCCU
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
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
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
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

                EXPORT  SUPC_Handler              [WEAK]
                EXPORT  RSTC_Handler              [WEAK]
                EXPORT  RTC_Handler               [WEAK]
                EXPORT  RTT_Handler               [WEAK]
                EXPORT  WDT_Handler               [WEAK]
                EXPORT  PMC_Handler               [WEAK]
                EXPORT  EFC_Handler               [WEAK]
                EXPORT  FLEXCOM7_Handler          [WEAK]
                EXPORT  FLEXCOM0_Handler          [WEAK]
                EXPORT  FLEXCOM1_Handler          [WEAK]
                EXPORT  PIOA_Handler              [WEAK]
                EXPORT  PIOB_Handler              [WEAK]
                EXPORT  PDMIC0_Handler            [WEAK]
                EXPORT  FLEXCOM2_Handler          [WEAK]
                EXPORT  MEM2MEM_Handler           [WEAK]
                EXPORT  I2SC0_Handler             [WEAK]
                EXPORT  I2SC1_Handler             [WEAK]
                EXPORT  PDMIC1_Handler            [WEAK]
                EXPORT  FLEXCOM3_Handler          [WEAK]
                EXPORT  FLEXCOM4_Handler          [WEAK]
                EXPORT  FLEXCOM5_Handler          [WEAK]
                EXPORT  FLEXCOM6_Handler          [WEAK]
                EXPORT  TC0_Handler               [WEAK]
                EXPORT  TC1_Handler               [WEAK]
                EXPORT  TC2_Handler               [WEAK]
                EXPORT  TC3_Handler               [WEAK]
                EXPORT  TC4_Handler               [WEAK]
                EXPORT  TC5_Handler               [WEAK]
                EXPORT  ADC_Handler               [WEAK]
                EXPORT  ARM_Handler               [WEAK]
                EXPORT  UHP_Handler               [WEAK]
                EXPORT  UDP_Handler               [WEAK]
                EXPORT  CRCCU_Handler             [WEAK]

SUPC_Handler
RSTC_Handler
RTC_Handler
RTT_Handler
WDT_Handler
PMC_Handler
EFC_Handler
FLEXCOM7_Handler
FLEXCOM0_Handler
FLEXCOM1_Handler
PIOA_Handler
PIOB_Handler
PDMIC0_Handler
FLEXCOM2_Handler
MEM2MEM_Handler
I2SC0_Handler
I2SC1_Handler
PDMIC1_Handler
FLEXCOM3_Handler
FLEXCOM4_Handler
FLEXCOM5_Handler
FLEXCOM6_Handler
TC0_Handler
TC1_Handler
TC2_Handler
TC3_Handler
TC4_Handler
TC5_Handler
ADC_Handler
ARM_Handler
UHP_Handler
UDP_Handler
CRCCU_Handler
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
