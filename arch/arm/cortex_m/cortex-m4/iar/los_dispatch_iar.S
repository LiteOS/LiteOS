
PRESERVE8
REQUIRE8
THUMB

OS_NVIC_INT_CTRL              EQU    0xE000ED04
OS_NVIC_SYSPRI2               EQU    0xE000ED20
OS_NVIC_PENDSV_PRI            EQU    0xF0F00000
OS_NVIC_PENDSVSET             EQU    0x10000000
OS_TASK_STATUS_RUNNING        EQU    0x0010

    SECTION    .text:CODE(2)
    THUMB


    EXPORT OsStartToRun
OsStartToRun
    .fnstart
    .cantunwind
    ldr     r4, =OS_NVIC_SYSPRI2
    ldr     r5, =OS_NVIC_PENDSV_PRI
    str     r5, [r4]

    ldr     r1, =g_oldTask
    str     r0, [r1]

    ldr     r1, =g_runTask
    str     r0, [r1]

    mov     r1, #2
    msr     CONTROL, r1


    ldrh    r7, [r0 , #4]
    mov     r8,  #OS_TASK_STATUS_RUNNING
    orr     r7,  r7,  r8
    strh    r7,  [r0 , #4]

    ldr     r12, [r0]
    ADD     r12, r12, #100

    ldmfd   r12!, {R0-R7}
    add     r12, r12, #72
    msr     psp, r12
    vpush   {s0};
    vpop    {s0};

    mov     lr, r5
   ;msr     xPSR, R7

    cpsie   I
    bx      r6
    .fnend


    EXPORT OsIntLock
OsIntLock
    .fnstart
    .cantunwind
    mrs     r0, PRIMASK
    cpsid   I
    bx      lr
    .fnend


    EXPORT OsIntUnLock
OsIntUnLock
    .fnstart
    .cantunwind
    mrs     r0, PRIMASK
    cpsie   I
    bx      lr
    .fnend


    EXPORT OsIntRestore
OsIntRestore
    .fnstart
    .cantunwind
    msr     PRIMASK, r0
    bx      lr
    .fnend


    EXPORT OsTaskSchedule
OsTaskSchedule
    .fnstart
    .cantunwind
    ldr     r2, =OS_NVIC_INT_CTRL
    ldr     r3, =OS_NVIC_PENDSVSET
    str     r3, [r2]
    bx      lr
    .fnend


    EXPORT osPendSV
osPendSV
    .fnstart
    .cantunwind
    mrs     r12, PRIMASK
    cpsid   I


TaskSwitch
    mrs     r0, psp

    stmfd   r0!, {r4-r12}
    vstmdb  r0!, {d8-d15}

    ldr     r5, =g_oldTask
    ldr     r1, [r5]
    str     r0, [r1]

    ldr     r0, =g_runTask
    ldr     r0, [r0]
    ;/* g_oldTask = g_runTask */
    str     r0, [r5]
    ldr     r1, [r0]

    vldmia  r1!, {d8-d15}
    ldmfd   r1!, {r4-r12}
    msr     psp,  r1

    msr     PRIMASK, r12
    bx      lr

    .fnend

    END
