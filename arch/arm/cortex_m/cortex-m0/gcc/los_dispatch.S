.syntax unified
.arch armv6-m
.thumb

.section .text
.equ OS_NVIC_INT_CTRL, 0xE000ED04
.equ OS_NVIC_SYSPRI2, 0xE000ED20
.equ OS_NVIC_PENDSV_PRI, 0x00F00000
.equ OS_NVIC_PENDSVSET, 0x10000000
.equ OS_TASK_STATUS_RUNNING, 0x0010

    .type OsStartToRun, %function
    .global OsStartToRun
OsStartToRun:
    .fnstart
    .cantunwind
    LDR     R4, =OS_NVIC_SYSPRI2
    LDR     R5, =OS_NVIC_PENDSV_PRI
    STR     R5, [R4]

    LDR     R1, =g_oldTask
    STR     R0, [R1]

    MOVS    R1, #2
    MSR     CONTROL, R1


    LDRH    R7, [R0 , #4]
    MOVS    R6,  #OS_TASK_STATUS_RUNNING
    ORRS    R7,  R7, R6
    STRH    R7,  [R0 , #4]

    LDR     R3, [R0]
    ADDS    R3, R3, #36

    LDMFD   R3!, {R0-R2}
    ADDS    R3, R3, #4
    LDMFD   R3!, {R4-R7}
    MSR     PSP, R3
    SUBS    R3, R3, #20
    LDR     R3,  [R3]

    MOV     LR, R5
    CPSIE   I
    BX      R6
    .fnend

    .type OsIntLock, %function
    .global OsIntLock
OsIntLock:
    .fnstart
    .cantunwind
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR
    .fnend

    .type OsIntUnLock, %function
    .global OsIntUnLock
OsIntUnLock:
    .fnstart
    .cantunwind
    MRS    R0, PRIMASK
    CPSIE  I
    BX     LR
    .fnend

    .type OsIntRestore, %function
    .global OsIntRestore
OsIntRestore:
    .fnstart
    .cantunwind
    MSR   PRIMASK, R0
    BX    LR
    .fnend

    .type OsTaskSchedule, %function
    .global OsTaskSchedule
OsTaskSchedule:
    .fnstart
    .cantunwind
    LDR     R2, =OS_NVIC_INT_CTRL
    LDR     R3, =OS_NVIC_PENDSVSET
    STR     R3, [R2]
    BX      LR
    .fnend

    .type PendSV_Handler, %function
    .global PendSV_Handler
PendSV_Handler:
    .fnstart
    .cantunwind
    MRS     R12, PRIMASK
    CPSID   I



TaskSwitch:
    MRS     R0, PSP

    SUBS    R0, #36
    STMIA   R0!, {R4-R7}
    MOV     R3, R8
    MOV     R4, R9
    MOV     R5, R10
    MOV     R6, R11
    MOV     R7, R12
    STMIA   R0!, {R3 - R7}

    SUBS    R0, #36

    LDR     R5, =g_oldTask
    LDR     R1, [R5]
    STR     R0, [R1]


    LDR     R0, =g_runTask
    LDR     R0, [R0]
    /* g_oldTask = g_runTask */
    STR     R0, [R5]
    LDR     R1,   [R0]
    ADDS    R1,   #16
    LDMFD   R1!, {R3-R7}
    MOV     R8, R3
    MOV     R9, R4
    MOV     R10, R5
    MOV     R11, R6
    MOV     R12, R7
    SUBS    R1,  #36
    LDMFD   R1!, {R4-R7}

    ADDS    R1,   #20
    MSR     PSP,  R1

    MSR     PRIMASK, R12
    BX      LR
    .fnend
