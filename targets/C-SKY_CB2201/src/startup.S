#include <csi_config.h>
//#include "target_config.h"//将该文件路径添加到汇编头文件路径中，发现该文件中包含了los_typedef.h,此文件中包含类型定义等C语言代码，无法在此路径下展开。20181128
        .import irq_stack

#define AHB_BASE        0x40000000
#undef  EFLASH_CONTROL_BASE
#define EFLASH_CONTROL_BASE  0x4003f000

#undef  PMU_LP_CONTROL
#define PMU_LP_CONTROL 0x40002000

#undef  PMU_MCLK_SEL
#define PMU_MCLK_SEL   0x40002004

#undef  PMU_CLK_STABLE
#define PMU_CLK_STABLE 0x40002018

#undef  PMU_PLL_CTRL
#define PMU_PLL_CTRL   0x4000202c

        .section .vectors
        .align  10
        .globl  __Vectors
        .type   __Vectors, @object
__Vectors:
        .long   Reset_Handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
        .long   vec_handler
		
#ifndef CONFIG_STARTUP_IRQ		//如果没有定义 （使用底层非接管中断），则使用接管中断
		.long   irq_handler             /*  0:  UART         */
        .long   irq_handler             /*  1:  CoreTIM      */
        .long   irq_handler        		/*  2:  Timer0       */
        .long   irq_handler             /*  3:  Timer1       */
        .long   irq_handler             /*  4:  Timer2       */
        .long   irq_handler             /*  5:  Timer3       */
        .long   irq_handler            
        .long   irq_handler             /*  7:  GPIO0        */
        .long   irq_handler       		/*  8:  USART2        */
        .long   irq_handler             /*  9:  GPIO2        */
        .long   irq_handler             /*  10: GPIO3        */
        .long   irq_handler             /*  11: GPIO4        */
        .long   irq_handler             /*  12: GPIO5        */
        .long   irq_handler             /*  13: GPIO6        */
        .long   irq_handler             /*  14: GPIO7        */
        .long   irq_handler            
        .long   irq_handler             /*  16: STimer0      */
        .long   irq_handler        		/*  17: STimer1      */
        .long   irq_handler             /*  18: STimer2      */
        .long   irq_handler             /*  19: STimer3      */
        .long   irq_handler             /*  20: pad          */
		.long   irq_handler             /*  21: pad          */
		.long   irq_handler             /*  22: pad          */
		.long   irq_handler             /*  23: pad          */
		.long   irq_handler             /*  24: pad          */
		.long   irq_handler             /*  25: pad          */
		.long   irq_handler          	/*  26: pad          */
		.long   irq_handler             /*  27: pad          */
		.long   irq_handler             /*  28: pad          */
		.long   irq_handler             /*  29: pad          */
#else								//如果定义了 （使用底层非接管中断），则使用非接管中断
        /* External interrupts */
        .long   GPIO0_IRQHandler        /*  0:  UART         */
        .long   irq_handler             /*  1:  CoreTIM      *///如果定义了这个，会导致shell的回车显示两行（暂时不替换）20181128
        .long   TIMA0_IRQHandler        /*  2:  Timer0       */
        .long   TIMA1_IRQHandler        /*  3:  Timer1       */
        .long   I2S_IRQHandler          /*  4:  Timer2       */
        .long   WDT_IRQHandler          /*  5:  Timer3       */
        .long   USART0_IRQHandler            
        .long   USART1_IRQHandler       /*  7:  GPIO0        */
        .long   USART2_IRQHandler       /*  8:  USART2        */
        .long   I2C0_IRQHandler         /*  9:  GPIO2        */
        .long   I2C1_IRQHandler         /*  10: GPIO3        */
        .long   SPI1_IRQHandler         /*  11: GPIO4        */
        .long   SPI0_IRQHandler         /*  12: GPIO5        */
        .long   RTC_IRQHandler          /*  13: GPIO6        */
        .long   irq_handler             /*  14: GPIO7        */
        .long   ADC_IRQHandler            
        .long   irq_handler             /*  16: STimer0      */
        .long   DMAC0_IRQHandler        /*  17: STimer1      */
        .long   irq_handler             /*  18: STimer2      */
        .long   irq_handler             /*  19: STimer3      */
        .long   irq_handler             /*  20: pad          */
		.long   irq_handler             /*  21: pad          */
		.long   DMAC1_IRQHandler        /*  22: pad          */
		.long   TIMB0_IRQHandler        /*  23: pad          */
		.long   TIMB1_IRQHandler        /*  24: pad          */
		.long   RTC1_IRQHandler         /*  25: pad          */
		.long   AES_IRQHandler          /*  26: pad          */
		.long   GPIO1_IRQHandler        /*  27: pad          */
		.long   RSA_IRQHandler          /*  28: pad          */
		.long   SHA_IRQHandler          /*  29: pad          */
#endif
        .size    __Vectors, . - __Vectors

        .text
        .long   _start
        .align  1
_start:
        .text
        .align  1
        .globl  Reset_Handler
        .type   Reset_Handler, %function
Reset_Handler:
    /* set the priority cpu ahb */
    lrw     r0, AHB_BASE
    movi    r1, 0x1
    movi    r2, 0x2
    movi    r3, 0x3
    movi    r4, 0x4
    stw     r1, (r0, 0x0)
    stw     r2, (r0, 0xc)
    stw     r3, (r0, 0x4)
    stw     r4, (r0, 0x8)

    /* restore the eflash state when system reboot from deep sleep */
    lrw     r0, PMU_LP_CONTROL
    ldw     r1, (r0, 0)
    btsti   r1, 5
    bf      .LSetClk
    lrw     r0, EFLASH_CONTROL_BASE
    movi    r1, 0x35
    stw     r1, (r0, 0x24)
    movi    r1, 0x16
    stw     r1, (r0, 0x28)
    movi    r1, 0x35
    stw     r1, (r0, 0x2c)
    movi    r1, 0x1b9
    stw     r1, (r0, 0x30)
    movi    r1, 0x8b10
    stw     r1, (r0, 0x34)

.LSetClk:
    /* set clk 24MHz 设置CPU主频为24MHz*/
    lrw     r0, PMU_PLL_CTRL
    lrw     r1, 0xc0206
    stw     r1, (r0)
    lrw     r0, PMU_CLK_STABLE

    lrw     r2, 10000
.LWaitClkStabe:
	/* 等待时钟稳定 */
    subi    r2, 1
    cmpnei  r2, 0
    bf      .Ltimeout
    ldw     r1, (r0)
    btsti   r1, 4
    bf      .LWaitClkStabe
.Ltimeout:
    lrw     r0, PMU_MCLK_SEL
    movi    r1, 0x8
    stw     r1, (r0)

    /* Enable instruction cache 启用指令缓存*/
    lrw     r0, 0xe000f000   /* Cache register base address */
    lrw     r1, 0x10000063   /* CRCR0 value means: Cached data from 0x100000000, Size=256KB, Enable*/
    stw     r1, (r0, 0x8)    /* store 0x10000063 to CRCR0(0xe000f008) */
    lrw     r1, 0x1          /* store 0x1 to r1 */
    stw     r1, (r0, 0x4)    /* set CIR(0xe000f004) to 0x1, means invalid all caches */
    lrw     r1, 0x3          /* store 0x3 to r1 */
    stw     r1, (r0, 0x0)    /* set CER(0xe000f000) to 0x3, means enable icache */

    /* under normal circumstances,  it should not be opened */

#ifndef CONFIG_SYSTEM_SECURE
    lrw     r0, 0x80000000
    mtcr    r0, psr
#else
    lrw     r0, 0xe0000000
    mtcr    r0, psr
#endif

/* Initialize the normal stack pointer from the linker definition. （从链接器定义）初始化正常堆栈指针*/
    lrw     a1, irq_stack
    mov     sp, a1

/*
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    lrw     r1, __erodata
    lrw     r2, __data_start__
    lrw     r3, __data_end__

    subu    r3, r2
    cmpnei  r3, 0
    bf      .L_loop0_done

.L_loop0:
    ldw     r0, (r1, 0)
    stw     r0, (r2, 0)
    addi    r1, 4
    addi    r2, 4
    subi    r3, 4
    cmpnei  r3, 0
    bt      .L_loop0

.L_loop0_done:

/*
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
    lrw     r1, __bss_start__
    lrw     r2, __bss_end__

    movi    r0, 0

    subu    r2, r1
    cmpnei  r2, 0
    bf      .L_loop1_done

.L_loop1:
    stw     r0, (r1, 0)
    addi    r1, 4
    subi    r2, 4
    cmpnei  r2, 0
    bt      .L_loop1
.L_loop1_done:

#ifndef __NO_SYSTEM_INIT
    jbsr    SystemInit
#endif

#ifndef __NO_BOARD_INIT
    jbsr    board_init
#endif

    jbsr    main

    .size   Reset_Handler, . - Reset_Handler
__exit:
        /* fall thru to infinite loop */

vec_handler:
        psrset  ie
        psrset  ee
        br      vec_handler
        
        .end
