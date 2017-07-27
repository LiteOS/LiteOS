	.syntax unified
	.cpu cortex-m4
	.thumb
;.arch_extension sec

        .global  LOS_IntLock
        .global  LOS_IntUnLock
        .global  LOS_IntRestore
        .global  LOS_StartToRun
        .global  osTaskSchedule
        .global  SVC_Handler
        .global  PendSV_Handler
	.global  LOS_IntNumGet
	.global  osDisableIRQ
        
        .extern  g_stLosTask
        .extern  g_pfnTskSwitchHook
        .extern  g_bTaskScheduled

.equ	OS_NVIC_INT_CTRL,            0xE000ED04
.equ	OS_NVIC_SYSPRI2,             0xE000ED20
.equ	OS_NVIC_PENDSV_PRI,          0xF0F00000
.equ	OS_NVIC_PENDSVSET,           0x10000000
.equ	OS_TASK_STATUS_RUNNING,      0x0010

	.section .text 
    .thumb

LOS_StartToRun:
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
    push    {r0}
    mov     r0, #3
    msr     CONTROL, r0
    pop     {r0}
    vpush    {s0}
    vpop     {s0}

    mov     lr, r5
    ;MSR     xPSR, R7

    cpsie   I
    bx      r6

LOS_IntNumGet:
    mrs     r0, ipsr
    bx      lr

osDisableIRQ:
    cpsid   I
    bx      lr

LOS_IntLock:
    mrs     r0, PRIMASK
    cpsid   I
    bx      lr

LOS_IntUnLock:
    mrs     r0, PRIMASK
    cpsie   I
    bx      lr

LOS_IntRestore:
    msr     PRIMASK, r0
    bx      lr

osTaskSchedule:
    svc     #0
    ldr     r0, =OS_NVIC_INT_CTRL
    ldr     r1, =OS_NVIC_PENDSVSET
    str     r1, [r0]
    bx      lr


    .type SVC_Handler, %function
SVC_Handler:
    tst     lr, #(1 << 2)
    bne     1f

    bx      lr
1:  mrs     r0, CONTROL
    bics    r0, r0, #1
    msr     CONTROL, r0
    bx     lr




  
    .type PendSV_Handler, %function
PendSV_Handler:
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
    vstmdb  r0!, {d8-d15}

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
    vldmia  r1!, {d8-d15}
    ldmfd   r1!, {r4-r12}
    msr     psp,  r1

    msr     PRIMASK, r12

    mrs     r0, CONTROL
    orr     r0, r0, #1
    msr     CONTROL, r0
    
    bx      lr


