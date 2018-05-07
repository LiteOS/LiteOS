 .syntax unified
 .arch armv6-m
 .thumb

.equ    OS_NVIC_INT_CTRL,            0xE000ED04
.equ    OS_NVIC_SYSPRI2,             0xE000ED20
.equ    OS_NVIC_PENDSV_PRI,          0xF0F00000
.equ    OS_NVIC_PENDSVSET,           0x10000000
.equ    OS_TASK_STATUS_RUNNING,      0x0010

    .section .text
    .thumb

    .type LOS_StartToRun, %function
    .global LOS_StartToRun

LOS_StartToRun:
    .fnstart
    .cantunwind

    ldr     r4, =OS_NVIC_SYSPRI2
    ldr     r5, =OS_NVIC_PENDSV_PRI
    str     r5, [r4]

    ldr     r0, =g_bTaskScheduled
    movs    r1, #1
    str     r1, [r0]

    movs    r0, #2
    msr     CONTROL, r0

    ldr     r0, =g_stLosTask
    ldr     r2, [r0, #4]
    ldr     r0, =g_stLosTask
    str     r2, [r0]

    ldr     r3, =g_stLosTask
    ldr     r0, [r3]
    ldrh    r7, [r0, #4]
    movs    r6, #OS_TASK_STATUS_RUNNING
    orrs    r7, r7, r6
    strh    r7, [r0, #4]

    ldr     r3, [r0]
    adds    r3, r3, #36

    ldmfd   r3!, {r0-r2}
    adds    r3, r3, #4
    ldmfd   r3!, {r4-r7}
    msr     PSP, r3
    subs    r3, r3, #20
    ldr     r3, [r3]

    mov     lr, r5

    cpsie   i
    bx      r6
    .fnend


    .type LOS_IntLock, %function
    .global LOS_IntLock
LOS_IntLock:
    .fnstart
    .cantunwind

    mrs r0, PRIMASK
    cpsid i
    bx lr
    .fnend


    .type LOS_IntUnLock, %function
    .global LOS_IntUnLock
LOS_IntUnLock:
    .fnstart
    .cantunwind

    mrs r0, PRIMASK
    cpsie i
    bx lr
    .fnend


    .type LOS_IntRestore, %function
    .global LOS_IntRestore
LOS_IntRestore:
    .fnstart
    .cantunwind

    msr PRIMASK, r0
    bx lr
    .fnend


    .type osTaskSchedule, %function
    .global osTaskSchedule
osTaskSchedule:
    .fnstart
    .cantunwind

    ldr     r0, =OS_NVIC_INT_CTRL
    ldr     r1, =OS_NVIC_PENDSVSET
    str     r1, [r0]
    bx      lr
   .fnend


    .type osPendSV, %function
    .global osPendSV
osPendSV:
    .fnstart
    .cantunwind

    mrs     r12, PRIMASK
    cpsid   i

    ldr     r2, =g_pfnTskSwitchHook
    ldr     r2, [r2]
    cmp     r2, #0
    beq     TaskSwitch
    mov     r1, lr
    push    {r0, r1}
    blx     r2
    pop     {r0, r1}
    mov     lr, r1

TaskSwitch:
    mrs     r0, PSP

    subs    r0, #36
    stmia   r0!, {r4-r7}
    mov     r3, r8
    mov     r4, r9
    mov     r5, r10
    mov     r6, r11
    mov     r7, r12
    stmia   r0!, {r3-r7}

    subs    r0, #36

    ldr     r5, =g_stLosTask
    ldr     r6, [r5]
    str     r0, [r6]

    ldrh    r7, [r6, #4]
    movs    r3, #OS_TASK_STATUS_RUNNING
    bics    r7, r7, r3
    strh    r7, [r6, #4]

    ldr     r0, =g_stLosTask
    ldr     r0, [r0, #4]
    str     r0, [r5]

    ldrh    r7, [r0, #4]
    movs    r3, #OS_TASK_STATUS_RUNNING
    orrs    r7, r7, r3
    strh    r7, [r0 , #4]

    ldr     r1, [r0]
    adds    r1, #16
    ldmfd   r1!, {r3-r7}
    mov     r8, r3
    mov     r9, r4
    mov     r10, r5
    mov     r11, r6
    mov     r12, r7
    msr     PSP, r1
    subs    r1, #36
    ldmfd   r1!, {r4-r7}

    msr     PRIMASK, r12
    bx      lr
    .fnend
