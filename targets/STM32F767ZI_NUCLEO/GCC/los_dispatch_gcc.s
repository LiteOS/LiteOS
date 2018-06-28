 .syntax unified
 .arch armv7e-m
 .thumb
;.arch_extension sec



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
    mov     r1, #1
    str     r1, [r0]

    mov     r0, #2
    msr     CONTROL, r0


    ldr     r0, =g_stLosTask
    ldr     r2, [r0, #4]
    ldr     r0, =g_stLosTask
    str     r2, [r0]

    ldr     r3, =g_stLosTask
    ldr     r0, [r3]
    ldrh    r7, [r0 , #4]
    mov     r8,  #OS_TASK_STATUS_RUNNING
    orr     r7,  r7,  r8
    strh    r7,  [r0 , #4]

    ldr     r12, [r0]
    add     r12, r12, #100

    ldmfd   r12!, {r0-r7}
    add     r12, r12, #72
    msr     psp, r12
    vpush    {S0}
    vpop     {S0}
    mov     lr, r5
    //MSR     xPSR, R7

    cpsie   I
    bx      r6

    .fnend


    .type LOS_IntLock, %function
    .global LOS_IntLock
LOS_IntLock:
    .fnstart
    .cantunwind

    MRS R0, PRIMASK
    CPSID I
    BX LR
    .fnend

    .type LOS_IntUnLock, %function
    .global LOS_IntUnLock
LOS_IntUnLock:
    .fnstart
    .cantunwind

    MRS R0, PRIMASK
    CPSIE I
    BX LR
    .fnend

    .type LOS_IntRestore, %function
    .global LOS_IntRestore
LOS_IntRestore:
    .fnstart
    .cantunwind

    MSR PRIMASK, R0
    BX LR
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
    cpsid   I

    ldr     r2, =g_pfnTskSwitchHook
    ldr     r2, [r2]
    cbz     r2, TaskSwitch
    push    {r12, lr}
    blx     r2
    pop     {r12, lr}

TaskSwitch:

    mrs     r0, psp

    stmfd   r0!, {r4-r12}

    vstmdb   r0!, {d8-d15}
    ldr     r5, =g_stLosTask
    ldr     r6, [r5]
    str     r0, [r6]


    ldrh    r7, [r6 , #4]
    mov     r8,#OS_TASK_STATUS_RUNNING
    bic     r7, r7, r8
    strh    r7, [r6 , #4]


    ldr     r0, =g_stLosTask
    ldr     r0, [r0, #4]
    str     r0, [r5]


    ldrh    r7, [r0 , #4]
    mov     r8,  #OS_TASK_STATUS_RUNNING
    orr     r7, r7, r8
    strh    r7,  [r0 , #4]

    ldr     r1,   [r0]

    vldmia   r1!, {d8-d15}
    ldmfd   r1!, {r4-r12}
    msr     psp,  r1

    msr     PRIMASK, r12


    bx      lr
    .fnend

