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

    EXPORT  __Vectors
    EXPORT  Reset_Handler

__Vectors
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

    ; External Interrupts
    DCD     WDT_BOD_IRQHandler  ; Windowed watchdog timer, Brownout detect
    DCD     DMA0_IRQHandler  ; DMA controller
    DCD     GINT0_IRQHandler  ; GPIO group 0
    DCD     GINT1_IRQHandler  ; GPIO group 1
    DCD     PIN_INT0_IRQHandler  ; Pin interrupt 0 or pattern match engine slice 0
    DCD     PIN_INT1_IRQHandler  ; Pin interrupt 1or pattern match engine slice 1
    DCD     PIN_INT2_IRQHandler  ; Pin interrupt 2 or pattern match engine slice 2
    DCD     PIN_INT3_IRQHandler  ; Pin interrupt 3 or pattern match engine slice 3
    DCD     UTICK0_IRQHandler  ; Micro-tick Timer
    DCD     MRT0_IRQHandler  ; Multi-rate timer
    DCD     CTIMER0_IRQHandler  ; Standard counter/timer CTIMER0
    DCD     CTIMER1_IRQHandler  ; Standard counter/timer CTIMER1
    DCD     SCT0_IRQHandler  ; SCTimer/PWM
    DCD     CTIMER3_IRQHandler  ; Standard counter/timer CTIMER3
    DCD     FLEXCOMM0_IRQHandler  ; Flexcomm Interface 0 (USART, SPI, I2C)
    DCD     FLEXCOMM1_IRQHandler  ; Flexcomm Interface 1 (USART, SPI, I2C)
    DCD     FLEXCOMM2_IRQHandler  ; Flexcomm Interface 2 (USART, SPI, I2C)
    DCD     FLEXCOMM3_IRQHandler  ; Flexcomm Interface 3 (USART, SPI, I2C)
    DCD     FLEXCOMM4_IRQHandler  ; Flexcomm Interface 4 (USART, SPI, I2C)
    DCD     FLEXCOMM5_IRQHandler  ; Flexcomm Interface 5 (USART, SPI, I2C)
    DCD     FLEXCOMM6_IRQHandler  ; Flexcomm Interface 6 (USART, SPI, I2C, I2S)
    DCD     FLEXCOMM7_IRQHandler  ; Flexcomm Interface 7 (USART, SPI, I2C, I2S)
    DCD     ADC0_SEQA_IRQHandler  ; ADC0 sequence A completion.
    DCD     ADC0_SEQB_IRQHandler  ; ADC0 sequence B completion.
    DCD     ADC0_THCMP_IRQHandler  ; ADC0 threshold compare and error.
    DCD     DMIC0_IRQHandler  ; Digital microphone and DMIC subsystem
    DCD     HWVAD0_IRQHandler  ; Hardware Voice Activity Detector
    DCD     USB0_NEEDCLK_IRQHandler  ; USB Activity Wake-up Interrupt
    DCD     USB0_IRQHandler  ; USB device
    DCD     RTC_IRQHandler  ; RTC alarm and wake-up interrupts
    DCD     IOH_IRQHandler  ; IOH
    DCD     MAILBOX_IRQHandler  ; Mailbox interrupt (present on selected devices)
    DCD     PIN_INT4_IRQHandler  ; Pin interrupt 4 or pattern match engine slice 4 int
    DCD     PIN_INT5_IRQHandler  ; Pin interrupt 5 or pattern match engine slice 5 int
    DCD     PIN_INT6_IRQHandler  ; Pin interrupt 6 or pattern match engine slice 6 int
    DCD     PIN_INT7_IRQHandler  ; Pin interrupt 7 or pattern match engine slice 7 int
    DCD     CTIMER2_IRQHandler  ; Standard counter/timer CTIMER2
    DCD     CTIMER4_IRQHandler  ; Standard counter/timer CTIMER4
    DCD     Reserved54_IRQHandler  ; Reserved interrupt
    DCD     SPIFI0_IRQHandler  ; SPI flash interface
    
    
; Dummy Exception Handlers (infinite loops which can be modified)
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP

HardFault_Handler \
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP

MemManage_Handler     PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP

BusFault_Handler PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP

UsageFault_Handler PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP

DebugMon_Handler PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP

;PendSV_Handler  PROC
;                EXPORT  PendSV_Handler            [WEAK]
;                B       .
;                ENDP

;SysTick_Handler PROC
;                EXPORT  SysTick_Handler           [WEAK]
;                B       .
;                ENDP

WDT_BOD_IRQHandler\
                PROC
                EXPORT     WDT_BOD_IRQHandler        [WEAK]
                LDR        R0, =WDT_BOD_DriverIRQHandler
                BX         R0
                ENDP

DMA0_IRQHandler\
                PROC
                EXPORT     DMA0_IRQHandler        [WEAK]
                LDR        R0, =DMA0_DriverIRQHandler
                BX         R0
                ENDP

GINT0_IRQHandler\
                PROC
                EXPORT     GINT0_IRQHandler        [WEAK]
                LDR        R0, =GINT0_DriverIRQHandler
                BX         R0
                ENDP

GINT1_IRQHandler\
                PROC
                EXPORT     GINT1_IRQHandler        [WEAK]
                LDR        R0, =GINT1_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT0_IRQHandler\
                PROC
                EXPORT     PIN_INT0_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT0_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT1_IRQHandler\
                PROC
                EXPORT     PIN_INT1_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT1_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT2_IRQHandler\
                PROC
                EXPORT     PIN_INT2_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT2_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT3_IRQHandler\
                PROC
                EXPORT     PIN_INT3_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT3_DriverIRQHandler
                BX         R0
                ENDP

UTICK0_IRQHandler\
                PROC
                EXPORT     UTICK0_IRQHandler        [WEAK]
                LDR        R0, =UTICK0_DriverIRQHandler
                BX         R0
                ENDP

MRT0_IRQHandler\
                PROC
                EXPORT     MRT0_IRQHandler        [WEAK]
                LDR        R0, =MRT0_DriverIRQHandler
                BX         R0
                ENDP

CTIMER0_IRQHandler\
                PROC
                EXPORT     CTIMER0_IRQHandler        [WEAK]
                LDR        R0, =CTIMER0_DriverIRQHandler
                BX         R0
                ENDP

CTIMER1_IRQHandler\
                PROC
                EXPORT     CTIMER1_IRQHandler        [WEAK]
                LDR        R0, =CTIMER1_DriverIRQHandler
                BX         R0
                ENDP

SCT0_IRQHandler\
                PROC
                EXPORT     SCT0_IRQHandler        [WEAK]
                LDR        R0, =SCT0_DriverIRQHandler
                BX         R0
                ENDP

CTIMER3_IRQHandler\
                PROC
                EXPORT     CTIMER3_IRQHandler        [WEAK]
                LDR        R0, =CTIMER3_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM0_IRQHandler\
                PROC
                EXPORT     FLEXCOMM0_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM0_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM1_IRQHandler\
                PROC
                EXPORT     FLEXCOMM1_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM1_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM2_IRQHandler\
                PROC
                EXPORT     FLEXCOMM2_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM2_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM3_IRQHandler\
                PROC
                EXPORT     FLEXCOMM3_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM3_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM4_IRQHandler\
                PROC
                EXPORT     FLEXCOMM4_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM4_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM5_IRQHandler\
                PROC
                EXPORT     FLEXCOMM5_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM5_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM6_IRQHandler\
                PROC
                EXPORT     FLEXCOMM6_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM6_DriverIRQHandler
                BX         R0
                ENDP

FLEXCOMM7_IRQHandler\
                PROC
                EXPORT     FLEXCOMM7_IRQHandler        [WEAK]
                LDR        R0, =FLEXCOMM7_DriverIRQHandler
                BX         R0
                ENDP

ADC0_SEQA_IRQHandler\
                PROC
                EXPORT     ADC0_SEQA_IRQHandler        [WEAK]
                LDR        R0, =ADC0_SEQA_DriverIRQHandler
                BX         R0
                ENDP

ADC0_SEQB_IRQHandler\
                PROC
                EXPORT     ADC0_SEQB_IRQHandler        [WEAK]
                LDR        R0, =ADC0_SEQB_DriverIRQHandler
                BX         R0
                ENDP

ADC0_THCMP_IRQHandler\
                PROC
                EXPORT     ADC0_THCMP_IRQHandler        [WEAK]
                LDR        R0, =ADC0_THCMP_DriverIRQHandler
                BX         R0
                ENDP

DMIC0_IRQHandler\
                PROC
                EXPORT     DMIC0_IRQHandler        [WEAK]
                LDR        R0, =DMIC0_DriverIRQHandler
                BX         R0
                ENDP

HWVAD0_IRQHandler\
                PROC
                EXPORT     HWVAD0_IRQHandler        [WEAK]
                LDR        R0, =HWVAD0_DriverIRQHandler
                BX         R0
                ENDP

USB0_NEEDCLK_IRQHandler\
                PROC
                EXPORT     USB0_NEEDCLK_IRQHandler        [WEAK]
                LDR        R0, =USB0_NEEDCLK_DriverIRQHandler
                BX         R0
                ENDP

USB0_IRQHandler\
                PROC
                EXPORT     USB0_IRQHandler        [WEAK]
                LDR        R0, =USB0_DriverIRQHandler
                BX         R0
                ENDP

RTC_IRQHandler\
                PROC
                EXPORT     RTC_IRQHandler        [WEAK]
                LDR        R0, =RTC_DriverIRQHandler
                BX         R0
                ENDP

IOH_IRQHandler\
                PROC
                EXPORT     IOH_IRQHandler        [WEAK]
                LDR        R0, =IOH_DriverIRQHandler
                BX         R0
                ENDP

MAILBOX_IRQHandler\
                PROC
                EXPORT     MAILBOX_IRQHandler        [WEAK]
                LDR        R0, =MAILBOX_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT4_IRQHandler\
                PROC
                EXPORT     PIN_INT4_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT4_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT5_IRQHandler\
                PROC
                EXPORT     PIN_INT5_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT5_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT6_IRQHandler\
                PROC
                EXPORT     PIN_INT6_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT6_DriverIRQHandler
                BX         R0
                ENDP

PIN_INT7_IRQHandler\
                PROC
                EXPORT     PIN_INT7_IRQHandler        [WEAK]
                LDR        R0, =PIN_INT7_DriverIRQHandler
                BX         R0
                ENDP

CTIMER2_IRQHandler\
                PROC
                EXPORT     CTIMER2_IRQHandler        [WEAK]
                LDR        R0, =CTIMER2_DriverIRQHandler
                BX         R0
                ENDP

CTIMER4_IRQHandler\
                PROC
                EXPORT     CTIMER4_IRQHandler        [WEAK]
                LDR        R0, =CTIMER4_DriverIRQHandler
                BX         R0
                ENDP

Reserved54_IRQHandler\
                PROC
                EXPORT     Reserved54_IRQHandler        [WEAK]
                LDR        R0, =Reserved54_DriverIRQHandler
                BX         R0
                ENDP

SPIFI0_IRQHandler\
                PROC
                EXPORT     SPIFI0_IRQHandler        [WEAK]
                LDR        R0, =SPIFI0_DriverIRQHandler
                BX         R0
                ENDP

Default_Handler PROC
                EXPORT     WDT_BOD_DriverIRQHandler        [WEAK]
                EXPORT     DMA0_DriverIRQHandler        [WEAK]
                EXPORT     GINT0_DriverIRQHandler        [WEAK]
                EXPORT     GINT1_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT0_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT1_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT2_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT3_DriverIRQHandler        [WEAK]
                EXPORT     UTICK0_DriverIRQHandler        [WEAK]
                EXPORT     MRT0_DriverIRQHandler        [WEAK]
                EXPORT     CTIMER0_DriverIRQHandler        [WEAK]
                EXPORT     CTIMER1_DriverIRQHandler        [WEAK]
                EXPORT     SCT0_DriverIRQHandler        [WEAK]
                EXPORT     CTIMER3_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM0_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM1_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM2_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM3_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM4_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM5_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM6_DriverIRQHandler        [WEAK]
                EXPORT     FLEXCOMM7_DriverIRQHandler        [WEAK]
                EXPORT     ADC0_SEQA_DriverIRQHandler        [WEAK]
                EXPORT     ADC0_SEQB_DriverIRQHandler        [WEAK]
                EXPORT     ADC0_THCMP_DriverIRQHandler        [WEAK]
                EXPORT     DMIC0_DriverIRQHandler        [WEAK]
                EXPORT     HWVAD0_DriverIRQHandler        [WEAK]
                EXPORT     USB0_NEEDCLK_DriverIRQHandler        [WEAK]
                EXPORT     USB0_DriverIRQHandler        [WEAK]
                EXPORT     RTC_DriverIRQHandler        [WEAK]
                EXPORT     IOH_DriverIRQHandler        [WEAK]
                EXPORT     MAILBOX_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT4_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT5_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT6_DriverIRQHandler        [WEAK]
                EXPORT     PIN_INT7_DriverIRQHandler        [WEAK]
                EXPORT     CTIMER2_DriverIRQHandler        [WEAK]
                EXPORT     CTIMER4_DriverIRQHandler        [WEAK]
                EXPORT     Reserved54_DriverIRQHandler        [WEAK]
                EXPORT     SPIFI0_DriverIRQHandler        [WEAK]

WDT_BOD_DriverIRQHandler
DMA0_DriverIRQHandler
GINT0_DriverIRQHandler
GINT1_DriverIRQHandler
PIN_INT0_DriverIRQHandler
PIN_INT1_DriverIRQHandler
PIN_INT2_DriverIRQHandler
PIN_INT3_DriverIRQHandler
UTICK0_DriverIRQHandler
MRT0_DriverIRQHandler
CTIMER0_DriverIRQHandler
CTIMER1_DriverIRQHandler
SCT0_DriverIRQHandler
CTIMER3_DriverIRQHandler
FLEXCOMM0_DriverIRQHandler
FLEXCOMM1_DriverIRQHandler
FLEXCOMM2_DriverIRQHandler
FLEXCOMM3_DriverIRQHandler
FLEXCOMM4_DriverIRQHandler
FLEXCOMM5_DriverIRQHandler
FLEXCOMM6_DriverIRQHandler
FLEXCOMM7_DriverIRQHandler
ADC0_SEQA_DriverIRQHandler
ADC0_SEQB_DriverIRQHandler
ADC0_THCMP_DriverIRQHandler
DMIC0_DriverIRQHandler
HWVAD0_DriverIRQHandler
USB0_NEEDCLK_DriverIRQHandler
USB0_DriverIRQHandler
RTC_DriverIRQHandler
IOH_DriverIRQHandler
MAILBOX_DriverIRQHandler
PIN_INT4_DriverIRQHandler
PIN_INT5_DriverIRQHandler
PIN_INT6_DriverIRQHandler
PIN_INT7_DriverIRQHandler
CTIMER2_DriverIRQHandler
CTIMER4_DriverIRQHandler
Reserved54_DriverIRQHandler
SPIFI0_DriverIRQHandler
    
                B       .

                ENDP

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

