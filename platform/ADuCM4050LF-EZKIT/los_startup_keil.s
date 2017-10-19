        PRESERVE8

Stack_Size  EQU     0x00000400
Heap_Size   EQU     0x00000200

    AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem SPACE   Stack_Size
__initial_sp

    AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem    SPACE   Heap_Size
__heap_limit

    AREA    RESET, CODE, READONLY
    THUMB

    ;IMPORT  ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||
    IMPORT  PendSV_Handler
    IMPORT  SysTick_Handler;SysTick_Handler

    EXPORT  _BootVectors
    EXPORT  Reset_Handler

_BootVectors
    DCD     __initial_sp
    DCD     Reset_Handler
    DCD     0               ; NMI Handler
    DCD     0               ; Hard Fault Handler
    DCD     0               ; MPU Fault Handler
    DCD     0               ; Bus Fault Handler
    DCD     0               ; Usage Fault Handler
    DCD     0               ; Reserved
    DCD     0               ; Reserved
    DCD     0               ; Reserved
    DCD     0               ; Reserved
    DCD     0               ; SVCall Handler
    DCD     0               ; Debug Monitor Handler
    DCD     0               ; Reserved
    DCD     PendSV_Handler  ; PendSV Handler
    DCD     SysTick_Handler ; SysTick_Handler

Reset_Handler
    LDR.W   R0, =0xE000ED88
    LDR     R1, [R0]
    ORR     R1, R1, #(0xF << 20)
    STR     R1, [R0]
    CPSID   I
    
    IMPORT  __main
    LDR     R0, =__main
    BX      R0


    ALIGN

    IF      :DEF:__MICROLIB
                
    EXPORT  __initial_sp
    EXPORT  __heap_base
    EXPORT  __heap_limit
                
    ELSE
                
    IMPORT  __use_two_region_memory
    EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

    LDR     R0, =  Heap_Mem
    LDR     R1, =(Stack_Mem + Stack_Size)
    LDR     R2, = (Heap_Mem +  Heap_Size)
    LDR     R3, = Stack_Mem
    BX      LR

    ALIGN
    ENDIF


    END

