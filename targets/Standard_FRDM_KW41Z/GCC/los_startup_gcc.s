/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MKW41Z4.s                                                           */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                                   */
/*            MKW41Z4                                                                     */
/*  @version: 1.0                                                                         */
/*  @date:    2015-9-23                                                                   */
/*  @build:   b170113                                                                     */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright (c) 1997 - 2016, Freescale Semiconductor, Inc.                               */
/* Copyright 2016 - 2017 NXP                                                              */
/* Redistribution and use in source and binary forms, with or without modification,       */
/* are permitted provided that the following conditions are met:                          */
/*                                                                                        */
/* o Redistributions of source code must retain the above copyright notice, this list     */
/*   of conditions and the following disclaimer.                                          */
/*                                                                                        */
/* o Redistributions in binary form must reproduce the above copyright notice, this       */
/*   list of conditions and the following disclaimer in the documentation and/or          */
/*   other materials provided with the distribution.                                      */
/*                                                                                        */
/* o Neither the name of the copyright holder nor the names of its                        */
/*   contributors may be used to endorse or promote products derived from this            */
/*   software without specific prior written permission.                                  */
/*                                                                                        */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND        */
/* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED          */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                 */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES         */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;           */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON         */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS          */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/
    .syntax unified
    .arch armv6-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   DMA0_IRQHandler                                 /* DMA channel 0 transfer complete*/
    .long   DMA1_IRQHandler                                 /* DMA channel 1 transfer complete*/
    .long   DMA2_IRQHandler                                 /* DMA channel 2 transfer complete*/
    .long   DMA3_IRQHandler                                 /* DMA channel 3 transfer complete*/
    .long   Reserved20_IRQHandler                           /* Reserved interrupt*/
    .long   FTFA_IRQHandler                                 /* Command complete and read collision*/
    .long   LVD_LVW_DCDC_IRQHandler                         /* Low-voltage detect, low-voltage warning, DCDC*/
    .long   LLWU_IRQHandler                                 /* Low leakage wakeup Unit*/
    .long   I2C0_IRQHandler                                 /* I2C0 interrupt*/
    .long   I2C1_IRQHandler                                 /* I2C1 interrupt*/
    .long   SPI0_IRQHandler                                 /* SPI0 single interrupt vector for all sources*/
    .long   TSI0_IRQHandler                                 /* TSI0 single interrupt vector for all sources*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status and error*/
    .long   TRNG0_IRQHandler                                /* TRNG0 interrupt*/
    .long   CMT_IRQHandler                                  /* CMT interrupt*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   TPM0_IRQHandler                                 /* TPM0 single interrupt vector for all sources*/
    .long   TPM1_IRQHandler                                 /* TPM1 single interrupt vector for all sources*/
    .long   TPM2_IRQHandler                                 /* TPM2 single interrupt vector for all sources*/
    .long   RTC_IRQHandler                                  /* RTC alarm*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds*/
    .long   PIT_IRQHandler                                  /* PIT interrupt*/
    .long   LTC0_IRQHandler                                 /* LTC0 interrupt*/
    .long   Radio_0_IRQHandler                              /* BTLE, ZIGBEE, ANT, GENFSK interrupt 0*/
    .long   DAC0_IRQHandler                                 /* DAC0 interrupt*/
    .long   Radio_1_IRQHandler                              /* BTLE, ZIGBEE, ANT, GENFSK interrupt 1*/
    .long   MCG_IRQHandler                                  /* MCG interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTMR0 interrupt*/
    .long   SPI1_IRQHandler                                 /* SPI1 single interrupt vector for all sources*/
    .long   PORTA_IRQHandler                                /* PORTA Pin detect*/
    .long   PORTB_PORTC_IRQHandler                          /* PORTB and PORTC Pin detect*/

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFE

/* define LiteOS heap info in rom */
    .section  .text
    .global __LOS_HEAP_ADDR_START__
    .global __LOS_HEAP_ADDR_END__
__LOS_HEAP_ADDR_START__:
    .word  __los_heap_addr_start__
__LOS_HEAP_ADDR_END__:
    .word  __los_heap_addr_end__

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/* PART-1: init VTOR and MSP */
    cpsid   i               /* Mask interrupts */
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__isr_vector
    str     r1, [r0]
    ldr     r2, [r1]
    msr     msp, r2

/* PART-2: Loop to copy interrupt from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __interrupt_rom_start: start of interrupts_ram section, i.e., begin of interrupts_ram sections to copy from.
 *      __interrupts_ram_start__/__interrupts_ram_end__: RAM address range that interrupts_ram should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */
    ldr    r1, =__interrupt_rom_start
    ldr    r2, =__interrupts_ram_start__
    ldr    r3, =__interrupts_ram_end__

    subs    r3, r2
    ble     .LC5

.LC4:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .LC4
.LC5:

/* PART-3: Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __data_rom_start: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */
    ldr    r1, =__data_rom_start
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble     .LC0

.LC1:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .LC1
.LC0:

/* PART-4: Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    subs    r2, r1
    ble .LC3

    movs    r0, 0
.LC2:
    str r0, [r1, r2]
    subs    r2, 4
    bge .LC2
.LC3:

    cpsie   i               /* Unmask interrupts */

/* PART-5: call SystemInit
 *         call __libc_init_array
 *         jump to main
 *
 */
    ldr   r0,=SystemInit
    blx   r0
    ldr   r0,=__libc_init_array
    blx   r0
    ldr   r0,=main
    bx    r0

    .pool
    .size Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    ldr r0, =DefaultISR
    bx r0
    .size DefaultISR, . - DefaultISR

    .align 1
    .thumb_func
    .weak NMI_Handler
    .type NMI_Handler, %function
NMI_Handler:
    ldr   r0,=NMI_Handler
    bx    r0
    .size NMI_Handler, . - NMI_Handler

    .align 1
    .thumb_func
    .weak HardFault_Handler
    .type HardFault_Handler, %function
HardFault_Handler:
    ldr   r0,=HardFault_Handler
    bx    r0
    .size HardFault_Handler, . - HardFault_Handler

    .align 1
    .thumb_func
    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    ldr   r0,=SVC_Handler
    bx    r0
    .size SVC_Handler, . - SVC_Handler

    .align 1
    .thumb_func
    .weak PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    ldr   r0,=PendSV_Handler
    bx    r0
    .size PendSV_Handler, . - PendSV_Handler

    .align 1
    .thumb_func
    .weak SysTick_Handler
    .type SysTick_Handler, %function
SysTick_Handler:
    ldr   r0,=SysTick_Handler
    bx    r0
    .size SysTick_Handler, . - SysTick_Handler

    .align 1
    .thumb_func
    .weak DMA0_IRQHandler
    .type DMA0_IRQHandler, %function
DMA0_IRQHandler:
    ldr   r0,=DMA0_DriverIRQHandler
    bx    r0
    .size DMA0_IRQHandler, . - DMA0_IRQHandler

    .align 1
    .thumb_func
    .weak DMA1_IRQHandler
    .type DMA1_IRQHandler, %function
DMA1_IRQHandler:
    ldr   r0,=DMA1_DriverIRQHandler
    bx    r0
    .size DMA1_IRQHandler, . - DMA1_IRQHandler

    .align 1
    .thumb_func
    .weak DMA2_IRQHandler
    .type DMA2_IRQHandler, %function
DMA2_IRQHandler:
    ldr   r0,=DMA2_DriverIRQHandler
    bx    r0
    .size DMA2_IRQHandler, . - DMA2_IRQHandler

    .align 1
    .thumb_func
    .weak DMA3_IRQHandler
    .type DMA3_IRQHandler, %function
DMA3_IRQHandler:
    ldr   r0,=DMA3_DriverIRQHandler
    bx    r0
    .size DMA3_IRQHandler, . - DMA3_IRQHandler

    .align 1
    .thumb_func
    .weak I2C0_IRQHandler
    .type I2C0_IRQHandler, %function
I2C0_IRQHandler:
    ldr   r0,=I2C0_DriverIRQHandler
    bx    r0
    .size I2C0_IRQHandler, . - I2C0_IRQHandler

    .align 1
    .thumb_func
    .weak I2C1_IRQHandler
    .type I2C1_IRQHandler, %function
I2C1_IRQHandler:
    ldr   r0,=I2C1_DriverIRQHandler
    bx    r0
    .size I2C1_IRQHandler, . - I2C1_IRQHandler

    .align 1
    .thumb_func
    .weak SPI0_IRQHandler
    .type SPI0_IRQHandler, %function
SPI0_IRQHandler:
    ldr   r0,=SPI0_DriverIRQHandler
    bx    r0
    .size SPI0_IRQHandler, . - SPI0_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART0_IRQHandler
    .type LPUART0_IRQHandler, %function
LPUART0_IRQHandler:
    ldr   r0,=LPUART0_DriverIRQHandler
    bx    r0
    .size LPUART0_IRQHandler, . - LPUART0_IRQHandler

    .align 1
    .thumb_func
    .weak SPI1_IRQHandler
    .type SPI1_IRQHandler, %function
SPI1_IRQHandler:
    ldr   r0,=SPI1_DriverIRQHandler
    bx    r0
    .size SPI1_IRQHandler, . - SPI1_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    DMA0_DriverIRQHandler
    def_irq_handler    DMA1_DriverIRQHandler
    def_irq_handler    DMA2_DriverIRQHandler
    def_irq_handler    DMA3_DriverIRQHandler
    def_irq_handler    Reserved20_IRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    LVD_LVW_DCDC_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    I2C1_DriverIRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    TRNG0_IRQHandler
    def_irq_handler    CMT_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    PIT_IRQHandler
    def_irq_handler    LTC0_IRQHandler
    def_irq_handler    Radio_0_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    Radio_1_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    SPI1_DriverIRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_PORTC_IRQHandler

    .end
