#include "arch/regs.h"
#include "arch/exception.h"

.arch armv8-a

.extern __bss_start
.extern __bss_end
.global __irq_stack_top
.global __irq_stack
.global __stack_startup_top
.global __stack_startup
.global psci_call
.extern test_uart_putc

.align 5
.text
.global arm64_elX_2_el1
.type   arm64_elX_2_el1,function
.global arm64_el3_to_el1
.type   arm64_el3_to_el1,function
.global reset_vector
.type   reset_vector,function
.extern arm64_exception_handlers

/* param0 is stack top, param1 is stack size, param2 is magic num */
.macro STACK_MAGIC_SET param0, param1, param2
    ldr     x0, =\param0
    mov     x2, \param1
    ldr     x1, =\param2
    bl      excstack_magic_set
.endm

/* Startup code which will get the machine into supervisor mode */
.section ".vectors","ax"
.global reset_vector
.type   reset_vector,function
reset_vector:
    mrs     x11, mpidr_el1
    ubfx    x11, x11, #0, #24
    cbnz    x11, excstatck_loop_done
/* clear out the interrupt and exception stack and set magic num to check the overflow */
do_excstatck:
    ldr     x1, =OS_STACK_INIT
    ldr     x2, =__stack_startup
    ldr     x3, =__irq_stack_top
/* Main loop sets 32 bytes at a time. */
excstatck_loop:
    str     x1, [x2], #8
    str     x1, [x2], #8
    str     x1, [x2], #8
    str     x1, [x2], #8
    cmp     x2,  x3
    blt     excstatck_loop

    STACK_MAGIC_SET __irq_stack, #OS_EXC_IRQ_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __stack_startup, #OS_EXC_START_STACK_SIZE, OS_STACK_MAGIC_WORD

excstatck_loop_done:

    bl      arm64_elX_to_el1

    mrs     x9, sctlr_el1
    orr     x9, x9, #(1<<12) /* Enable icache */
    orr     x9, x9, #(1<<2)  /* Enable dcache/ucache */
    orr     x9, x9, #(1<<3)  /* Enable Stack Alignment Check EL1 */
    orr     x9, x9, #(1<<4)  /* Enable Stack Alignment Check EL0 */
    bic     x9, x9, #(1<<1)  /* Disable Alignment Checking for EL1 EL0 */
    msr     sctlr_el1, x9

    /* enable el1 fp/neon */
    mov     x9, #(0x3 << 20)
    msr     cpacr_el1, x9
    isb

    /* set exception base to system register */
    ldr     x9, =arm64_exception_handlers
    msr     vbar_el1, x9

    cbnz    x11, cpu_start
    ldr     x10, =__stack_startup_top
    mov     sp, x10

    /* clear bss */
.L__do_bss:
    /* clear out the bss excluding the stack and kernel translation table  */
    /* NOTE: relies on __post_prebss_bss_start and __bss_end being 8 byte aligned */
    ldr     x9, =__bss_start
    ldr     x10, =__bss_end
    sub     x10, x10, x9
    cbz     x10, .L__bss_loop_done
.L__bss_loop:
    sub     x10, x10, #8
    str     xzr, [x9], #8
    cbnz    x10, .L__bss_loop
.L__bss_loop_done:

    bl      main
    b       .

cpu_start:
    /* Set up the stack */
    ldr     x10, =__stack_startup_top
    mov     x9, #OS_EXC_START_STACK_SIZE
    mul     x9, x9, x11
    sub     sp, x10, x9
    bl      secondary_cpu_start
    b       .

arm64_el3_to_el1:
    /* set EL2 to 64bit */
    mrs     x0, scr_el3
    orr     x0, x0, #(1<<10)
    msr     scr_el3, x0

    /* set EL1 to 64bit */
    mov     x0, #(1<<31)
    msr     hcr_el2, x0

    /* disable EL2 coprocessor traps */
    mov     x0, #0x33ff
    msr     cptr_el2, x0

    /* disable EL1 FPU traps */
    mov     x0, #(0b11<<20)
    msr     cpacr_el1, x0

    /* set up the EL1 bounce interrupt */
    mov     x0, sp
    msr     sp_el1, x0

    adr     x0, .Ltarget
    msr     elr_el3, x0

    mov     x0, #((0b1111 << 6) | (0b0101)) /* EL1h runlevel */
    msr     spsr_el3, x0
    isb

arm64_elX_to_el1:
    mrs     x4, CurrentEL

    cmp     x4, #(0b01 << 2)
    bne     .notEL1
    /* Already in EL1 */
    ret

.notEL1:
    cmp     x4, #(0b10 << 2)
    beq     .inEL2

    /* set EL2 to 64bit */
    mrs     x4, scr_el3
    orr     x4, x4, #(1<<10)
    msr     scr_el3, x4


    adr     x4, .Ltarget
    msr     elr_el3, x4

    mov     x4, #((0b1111 << 6) | (0b0101)) /* EL1h runlevel */
    msr     spsr_el3, x4
    b       .confEL1

.inEL2:
    adr     x4, .Ltarget
    msr     elr_el2, x4
    mov     x4, #((0b1111 << 6) | (0b0101)) /* EL1h runlevel */
    msr     spsr_el2, x4

.confEL1:
    /* disable EL2 coprocessor traps */
    mov     x0, #0x33ff
    msr     cptr_el2, x0

    /* set EL1 to 64bit */
    mov     x0, #(1<<31)
    msr     hcr_el2, x0

    /* disable EL1 FPU traps */
    mov     x0, #(0b11<<20)
    msr     cpacr_el1, x0

    /* set up the EL1 bounce interrupt */
    mov     x0, sp
    msr     sp_el1, x0

    isb
    eret

.Ltarget:
    ret

psci_call:
    hvc     #0
    ret

/*
 * set magic num to stack top for all cpu
 * x0 is stack top, x2 is stack size, x1 is magic num
 */
excstack_magic_set:
    mov     x3, #0
excstack_magic_loop:
    str     x1, [x0]
    add     x0, x0, x2
    add     x3, x3, #1
    cmp     x3, #CORE_NUM
    blt     excstack_magic_loop
    ret

.section .bss.prebss.stack
    .align 4
__stack_startup:
    .skip OS_EXC_START_STACK_SIZE * CORE_NUM
__stack_startup_top:
__irq_stack:
    .skip OS_EXC_IRQ_STACK_SIZE * CORE_NUM
__irq_stack_top:
