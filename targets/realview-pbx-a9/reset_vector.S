#include "arch/asm.h"
#include "arch/regs.h"
#include "arch/exception.h"

    .global __exc_stack_top
    .global __irq_stack_top
    .global __fiq_stack_top
    .global __svc_stack_top
    .global __abt_stack_top
    .global __undef_stack_top
    .global __exc_stack
    .global __irq_stack
    .global __fiq_stack
    .global __svc_stack
    .global __abt_stack
    .global __undef_stack


    .extern _osExceptFiqHdl
    .extern _osExceptDataAbortHdl
    .extern _osExceptPrefetchAbortHdl
    .extern _osExceptSwiHdl
    .extern _osExceptUndefInstrHdl
    .extern board_config
    .global enable_scu

/* param0 is stack top, param1 is stack size, param2 is magic num */
.macro STACK_MAGIC_SET param0, param1, param2
    ldr     r0, =\param0
    mov     r1, \param1
    ldr     r2, =\param2
    bl      excstack_magic
.endm

    .code   32
    .section ".vectors","ax"

    .global __exception_handlers
__exception_handlers:
    /*
    *Assumption:  ROM code has these vectors at the hardware reset address.
    *A simple jump removes any address-space dependencies [i.e. safer]
    */
    b   reset_vector
    b   _osExceptUndefInstrHdl
    b   _osExceptSwiHdl
    b   _osExceptPrefetchAbortHdl
    b   _osExceptDataAbortHdl
    b   _osExceptAddrAbortHdl
    b   OsIrqHandler
    b   _osExceptFiqHdl

    .text
    /* Startup code which will get the machine into supervisor mode */
    .global reset_vector
    .type   reset_vector,function
reset_vector:
    /* get cpuid and keep it in r11 */
    mrc     p15, 0, r11, c0, c0, 5
    and     r11, r11, #MPIDR_CPUID_MASK
    cmp     r11, #0
    bne     excstatck_loop_done
excstatck_loop:
    /* clear out the interrupt and exception stack and set magic num to check the overflow */
    ldr     r0, =__undef_stack
    ldr     r1, =__exc_stack_top
    bl      stack_init

    STACK_MAGIC_SET __undef_stack, #OS_EXC_UNDEF_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __abt_stack, #OS_EXC_ABT_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __irq_stack, #OS_EXC_IRQ_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __fiq_stack, #OS_EXC_FIQ_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __svc_stack, #OS_EXC_SVC_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __exc_stack, #OS_EXC_STACK_SIZE, OS_STACK_MAGIC_WORD

excstatck_loop_done:
    /* set svc stack, every cpu has OS_EXC_SVC_STACK_SIZE stack */
    ldr     r0, = __svc_stack_top
    mov     r1, #OS_EXC_SVC_STACK_SIZE
    mul     r1, r1, r11
    sub     r0, r0, r1
    mov     sp, r0

    /* enable fpu+neon */
    LDR     r0, =(0xF << 20)
    MCR     p15, 0, r0, c1, c0, 2

    MOV     r3, #0x40000000
    VMSR    FPEXC, r3

    LDR     r0, =__exception_handlers
    MCR     p15, 0, r0, c12, c0, 0

    cmp     r11, #0
    bne     cpu_start
    /* only cpu0 will call following functions */
    bl      enable_scu
    bl      board_config
    bl      main
    b       .

cpu_start:
    bl      secondary_cpu_start
    b       .

enable_scu:
    /* Read peripheral base address */
    mrc     p15, 4, r0, c15, c0, 0
    ldr     r1, [r0]
    orr     r1, r1, #0x1
    str     r1, [r0]
    bx      lr

/*
 * init stack to initial value
 * r0 is stack mem start, r1 is stack mem end
 */
stack_init:
    ldr     r2, =OS_STACK_INIT
    ldr     r3, =OS_STACK_INIT
    /* Main loop sets 32 bytes at a time. */
stack_init_loop:
    .irp    offset, #0, #8, #16, #24
    strd    r2, r3, [r0, \offset]
    .endr
    add     r0, #32
    cmp     r0, r1
    blt     stack_init_loop
    bx      lr

/*
 * set magic num to stack top for all cpu
 * r0 is stack top, r1 is stack size, r2 is magic num
 */
excstack_magic:
    mov     r3, #0
excstack_magic_loop:
    str     r2, [r0]
    add     r0, r0, r1
    add     r3, r3, #1
    cmp     r3, #CORE_NUM
    blt     excstack_magic_loop
    bx      lr

    /*
    * Temporary interrupt stack
    */
    .section ".int_stack", "wa", %nobits
    .align  3

__undef_stack:
    .space OS_EXC_UNDEF_STACK_SIZE * CORE_NUM
__undef_stack_top:

__abt_stack:
    .space OS_EXC_ABT_STACK_SIZE * CORE_NUM
__abt_stack_top:

__irq_stack:
    .space OS_EXC_IRQ_STACK_SIZE * CORE_NUM
__irq_stack_top:

__fiq_stack:
    .space OS_EXC_FIQ_STACK_SIZE * CORE_NUM
__fiq_stack_top:

__svc_stack:
    .space OS_EXC_SVC_STACK_SIZE * CORE_NUM
__svc_stack_top:

__exc_stack:
    .space OS_EXC_STACK_SIZE * CORE_NUM
__exc_stack_top:
