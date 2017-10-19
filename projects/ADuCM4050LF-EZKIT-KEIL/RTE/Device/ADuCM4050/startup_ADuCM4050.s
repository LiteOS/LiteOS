;/****************************************************************************
; * @file     startup_ADuCM4050.s
; * @brief    CMSIS Core Device Startup File for
; *           ADuCM4050
; * @version  V1.08
; * @date     23. November 2012
; *
; * @note     Modified 28. November 2016 Analog Devices
; *
; ******************************************************************************/
;/* Copyright (c) 2011 - 2012 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;
;   Portions Copyright (c) 2016 Analog Devices, Inc.
;   ---------------------------------------------------------------------------*/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

#include <rtos_map/adi_rtos_map.h>

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000C00

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

__Vectors       DCD     __initial_sp                ; Top of Stack
                DCD     Reset_Handler               ; Reset Handler
                DCD     NMI_Handler                 ; NMI Handler
                DCD     HardFault_Handler           ; Hard Fault Handler
                DCD     MemManage_Handler           ; MPU Fault Handler
                DCD     BusFault_Handler            ; Bus Fault Handler
                DCD     UsageFault_Handler          ; Usage Fault Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     SVC_HANDLER                 ; SVCall Handler
                DCD     DebugMon_Handler            ; Debug Monitor Handler
                DCD     0                           ; Reserved
                DCD     PENDSV_HANDLER              ; PendSV Handler
                DCD     SYSTICK_HANDLER             ; SysTick Handler

                ; External Interrupts
                DCD     RTC1_Int_Handler            ; Real Time Clock 1         /* 0  */
                DCD     Ext_Int0_Handler            ; External Interrupt 0      /* 1  */
                DCD     Ext_Int1_Handler            ; External Interrupt 1      /* 2  */
                DCD     Ext_Int2_Handler            ; External Interrupt 2      /* 3  */
                DCD     Ext_Int3_Handler            ; External Interrupt 3      /* 4  */
                DCD     WDog_Tmr_Int_Handler        ; Watchdog Timer            /* 5  */
                DCD     Vreg_over_Int_Handler       ; VREG Over                 /* 6  */
                DCD     Battery_Voltage_Int_Handler ; Battery Voltage Range     /* 7  */
                DCD     RTC0_Int_Handler            ; Real Time Clock 0         /* 8  */
                DCD     GPIO_A_Int_Handler          ; GPIO Int A                /* 9  */
                DCD     GPIO_B_Int_Handler          ; GPIO Int B                /* 10 */
                DCD     GP_Tmr0_Int_Handler         ; GP Timer 0                /* 11 */
                DCD     GP_Tmr1_Int_Handler         ; GP Timer 1                /* 12 */
                DCD     Flash0_Int_Handler          ; Flash Controller          /* 13 */
                DCD     UART0_Int_Handler           ; UART0                     /* 14 */
                DCD     SPI0_Int_Handler            ; SPI0                      /* 15 */
                DCD     SPI2_Int_Handler            ; SPI-H                     /* 16 */
                DCD     I2C0_Slave_Int_Handler      ; I2C0 Slave                /* 17 */
                DCD     I2C0_Master_Int_Handler     ; I2C0 Master               /* 18 */
                DCD     DMA_Err_Int_Handler         ; DMA Error                 /* 19 */
                DCD     DMA_SPIH_TX_Int_Handler     ; DMA Channel 0 Done        /* 20 */
                DCD     DMA_SPIH_RX_Int_Handler     ; DMA Channel 1 Done        /* 21 */
                DCD     DMA_SPORT0A_Int_Handler     ; DMA Channel 2 Done        /* 22 */
                DCD     DMA_SPORT0B_Int_Handler     ; DMA Channel 3 Done        /* 23 */
                DCD     DMA_SPI0_TX_Int_Handler     ; DMA Channel 4 Done        /* 24 */
                DCD     DMA_SPI0_RX_Int_Handler     ; DMA Channel 5 Done        /* 25 */
                DCD     DMA_SPI1_TX_Int_Handler     ; DMA Channel 6 Done        /* 26 */
                DCD     DMA_SPI1_RX_Int_Handler     ; DMA Channel 7 Done        /* 27 */
                DCD     DMA_UART0_TX_Int_Handler    ; DMA Channel 8 Done        /* 28 */
                DCD     DMA_UART0_RX_Int_Handler    ; DMA Channel 9 Done        /* 29 */
                DCD     DMA_I2C0_STX_Int_Handler    ; DMA Channel 10 Done       /* 30 */
                DCD     DMA_I2C0_SRX_Int_Handler    ; DMA Channel 11 Done       /* 31 */
                DCD     DMA_I2C0_MX_Int_Handler     ; DMA Channel 12 Done       /* 32 */
                DCD     DMA_AES0_IN_Int_Handler     ; DMA Channel 13 Done       /* 33 */
                DCD     DMA_AES0_OUT_Int_Handler    ; DMA Channel 14 Done       /* 34 */
                DCD     DMA_FLASH0_Int_Handler      ; DMA Channel 15 Done       /* 35 */
                DCD     SPORT0A_Int_Handler         ; SPORT0A                   /* 36 */
                DCD     SPORT0B_Int_Handler         ; SPORT0B                   /* 37 */
                DCD     Crypto_Int_Handler          ; Crypto                    /* 38 */
                DCD     DMA_ADC0_Int_Handler        ; DMA Channel 24 Done       /* 39 */
                DCD     GP_Tmr2_Int_Handler         ; GP Timer 2                /* 40 */
                DCD     Crystal_osc_Int_Handler     ; Crystal Oscillator        /* 41 */
                DCD     SPI1_Int_Handler            ; SPI1                      /* 42 */
                DCD     PLL_Int_Handler             ; PLL                       /* 43 */
                DCD     RNG_Int_Handler             ; Nandom Number Generator   /* 44 */
                DCD     Beep_Int_Handler            ; Beeper                    /* 45 */
                DCD     ADC0_Int_Handler            ; ADC0                      /* 46 */
                DCD     0                           ; Reserved                  /* 47 */
                DCD     0                           ; Reserved                  /* 48 */
                DCD     0                           ; Reserved                  /* 49 */
                DCD     0                           ; Reserved                  /* 50 */
                DCD     0                           ; Reserved                  /* 51 */
                DCD     0                           ; Reserved                  /* 52 */
                DCD     0                           ; Reserved                  /* 53 */
                DCD     0                           ; Reserved                  /* 54 */
                DCD     0                           ; Reserved                  /* 55 */
                DCD     DMA_SIP0_Int_Handler        ; DMA Channel 16 Done (SIP) /* 56 */
                DCD     DMA_SIP1_Int_Handler        ; DMA Channel 17 Done (SIP) /* 57 */
                DCD     DMA_SIP2_Int_Handler        ; DMA Channel 18 Done (SIP) /* 58 */
                DCD     DMA_SIP3_Int_Handler        ; DMA Channel 19 Done (SIP) /* 59 */
                DCD     DMA_SIP4_Int_Handler        ; DMA Channel 20 Done (SIP) /* 60 */
                DCD     DMA_SIP5_Int_Handler        ; DMA Channel 21 Done (SIP) /* 61 */
                DCD     DMA_SIP6_Int_Handler        ; DMA Channel 22 Done (SIP) /* 62 */
                DCD     DMA_SIP7_Int_Handler        ; DMA Channel 23 Done (SIP) /* 63 */
                DCD     0                           ; Reserved                  /* 64 */
                DCD     0                           ; Reserved                  /* 65 */
                DCD     UART1_Int_Handler           ; UART1                     /* 66 */
                DCD     DMA_UART1_TX_Int_Handler    ; DMA Channel 25 Done       /* 67 */
                DCD     DMA_UART1_RX_Int_Handler    ; DMA Channel 26 Done       /* 68 */
                DCD     RGB_Tmr_Int_Handler         ; Timer RGB                 /* 69 */
                DCD     0                           ; Reserved                  /* 70 */
                DCD     Root_Clk_Err_Handler        ; ROOT Clock Failure        /* 71 */

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
SVC_HANDLER     PROC
                EXPORT  SVC_HANDLER               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PENDSV_HANDLER  PROC
                EXPORT  PENDSV_HANDLER            [WEAK]
                B       .
                ENDP
SYSTICK_HANDLER PROC
                EXPORT  SYSTICK_HANDLER           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT RTC1_Int_Handler            [WEAK]
                EXPORT Ext_Int0_Handler            [WEAK]
                EXPORT Ext_Int1_Handler            [WEAK]
                EXPORT Ext_Int2_Handler            [WEAK]
                EXPORT Ext_Int3_Handler            [WEAK]
                EXPORT WDog_Tmr_Int_Handler        [WEAK]
                EXPORT Vreg_over_Int_Handler       [WEAK]
                EXPORT Battery_Voltage_Int_Handler [WEAK]
                EXPORT RTC0_Int_Handler            [WEAK]
                EXPORT GPIO_A_Int_Handler          [WEAK]
                EXPORT GPIO_B_Int_Handler          [WEAK]
                EXPORT GP_Tmr0_Int_Handler         [WEAK]
                EXPORT GP_Tmr1_Int_Handler         [WEAK]
                EXPORT Flash0_Int_Handler          [WEAK]
                EXPORT UART0_Int_Handler           [WEAK]
                EXPORT SPI0_Int_Handler            [WEAK]
                EXPORT SPI2_Int_Handler            [WEAK]
                EXPORT I2C0_Slave_Int_Handler      [WEAK]
                EXPORT I2C0_Master_Int_Handler     [WEAK]
                EXPORT DMA_Err_Int_Handler         [WEAK]
                EXPORT DMA_SPIH_TX_Int_Handler     [WEAK]
                EXPORT DMA_SPIH_RX_Int_Handler     [WEAK]
                EXPORT DMA_SPORT0A_Int_Handler     [WEAK]
                EXPORT DMA_SPORT0B_Int_Handler     [WEAK]
                EXPORT DMA_SPI0_TX_Int_Handler     [WEAK]
                EXPORT DMA_SPI0_RX_Int_Handler     [WEAK]
                EXPORT DMA_SPI1_TX_Int_Handler     [WEAK]
                EXPORT DMA_SPI1_RX_Int_Handler     [WEAK]
                EXPORT DMA_UART0_TX_Int_Handler    [WEAK]
                EXPORT DMA_UART0_RX_Int_Handler    [WEAK]
                EXPORT DMA_I2C0_STX_Int_Handler    [WEAK]
                EXPORT DMA_I2C0_SRX_Int_Handler    [WEAK]
                EXPORT DMA_I2C0_MX_Int_Handler     [WEAK]
                EXPORT DMA_AES0_IN_Int_Handler     [WEAK]
                EXPORT DMA_AES0_OUT_Int_Handler    [WEAK]
                EXPORT DMA_FLASH0_Int_Handler      [WEAK]
                EXPORT SPORT0A_Int_Handler         [WEAK]
                EXPORT SPORT0B_Int_Handler         [WEAK]
                EXPORT Crypto_Int_Handler          [WEAK]
                EXPORT DMA_ADC0_Int_Handler        [WEAK]
                EXPORT GP_Tmr2_Int_Handler         [WEAK]
                EXPORT Crystal_osc_Int_Handler     [WEAK]
                EXPORT SPI1_Int_Handler            [WEAK]
                EXPORT PLL_Int_Handler             [WEAK]
                EXPORT RNG_Int_Handler             [WEAK]
                EXPORT Beep_Int_Handler            [WEAK]
                EXPORT ADC0_Int_Handler            [WEAK]
                EXPORT DMA_SIP0_Int_Handler        [WEAK]
                EXPORT DMA_SIP1_Int_Handler        [WEAK]
                EXPORT DMA_SIP2_Int_Handler        [WEAK]
                EXPORT DMA_SIP3_Int_Handler        [WEAK]
                EXPORT DMA_SIP4_Int_Handler        [WEAK]
                EXPORT DMA_SIP5_Int_Handler        [WEAK]
                EXPORT DMA_SIP6_Int_Handler        [WEAK]
                EXPORT DMA_SIP7_Int_Handler        [WEAK]
                EXPORT UART1_Int_Handler           [WEAK]
                EXPORT DMA_UART1_TX_Int_Handler    [WEAK]
                EXPORT DMA_UART1_RX_Int_Handler    [WEAK]
                EXPORT RGB_Tmr_Int_Handler         [WEAK]
                EXPORT Root_Clk_Err_Handler        [WEAK]

RTC1_Int_Handler
Ext_Int0_Handler
Ext_Int1_Handler
Ext_Int2_Handler
Ext_Int3_Handler
WDog_Tmr_Int_Handler
Vreg_over_Int_Handler
Battery_Voltage_Int_Handler
RTC0_Int_Handler
GPIO_A_Int_Handler
GPIO_B_Int_Handler
GP_Tmr0_Int_Handler
GP_Tmr1_Int_Handler
Flash0_Int_Handler
UART0_Int_Handler
SPI0_Int_Handler
SPI2_Int_Handler
I2C0_Slave_Int_Handler
I2C0_Master_Int_Handler
DMA_Err_Int_Handler
DMA_SPIH_TX_Int_Handler
DMA_SPIH_RX_Int_Handler
DMA_SPORT0A_Int_Handler
DMA_SPORT0B_Int_Handler
DMA_SPI0_TX_Int_Handler
DMA_SPI0_RX_Int_Handler
DMA_SPI1_TX_Int_Handler
DMA_SPI1_RX_Int_Handler
DMA_UART0_TX_Int_Handler
DMA_UART0_RX_Int_Handler
DMA_I2C0_STX_Int_Handler
DMA_I2C0_SRX_Int_Handler
DMA_I2C0_MX_Int_Handler
DMA_AES0_IN_Int_Handler
DMA_AES0_OUT_Int_Handler
DMA_FLASH0_Int_Handler
SPORT0A_Int_Handler
SPORT0B_Int_Handler
Crypto_Int_Handler
DMA_ADC0_Int_Handler
GP_Tmr2_Int_Handler
Crystal_osc_Int_Handler
SPI1_Int_Handler
PLL_Int_Handler
RNG_Int_Handler
Beep_Int_Handler
ADC0_Int_Handler
DMA_SIP0_Int_Handler
DMA_SIP1_Int_Handler
DMA_SIP2_Int_Handler
DMA_SIP3_Int_Handler
DMA_SIP4_Int_Handler
DMA_SIP5_Int_Handler
DMA_SIP6_Int_Handler
DMA_SIP7_Int_Handler
UART1_Int_Handler
DMA_UART1_TX_Int_Handler
DMA_UART1_RX_Int_Handler
RGB_Tmr_Int_Handler
Root_Clk_Err_Handler
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

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
