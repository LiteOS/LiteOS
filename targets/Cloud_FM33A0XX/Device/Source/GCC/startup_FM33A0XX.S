;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WWDT_IRQHandler           ;  0:  Watchdog
                DCD     SVD_IRQHandler            ;  1:  SVD
                DCD     RTC_IRQHandler            ;  2:  RTC
                DCD     FLASH_IRQHandler          ;  3:  FLASH
                DCD     FDET_IRQHandler           ;  4:  FDET
                DCD     ADC_IRQHandler            ;  5:  ADC
                DCD     SPI0_IRQHandler           ;  6:  SPI0
                DCD     SPI1_IRQHandler           ;  7:  SPI1
                DCD     SPI2_IRQHandler           ;  8:  SPI2
                DCD     UART_IRQHandler           ;  9:  uart0-5
                DCD     USART0_IRQHandler         ; 10: USART0
                DCD     USART1_IRQHandler         ; 11: USART1
                DCD     I2C_IRQHandler            ; 12: I2C
                DCD     LCD_IRQHandler            ; 13: LCD
                DCD     AES_IRQHandler            ; 14: AES
                DCD     LPTIM_IRQHandler          ; 15: LPTIM
                DCD     DMA_IRQHandler            ; 16: DMA
                DCD     CRC_IRQHandler            ; 17: CRC
                DCD     TRNG_IRQHandler           ; 18: TRNG
                DCD     COPM0_IRQHandler          ; 19: COMP0
                DCD     COPM1_IRQHandler          ; 20: COMP1
                DCD     0                         ; 21: Reserved
                DCD     TIMER12_IRQHandler        ; 22: Timer1 / Timer2
                DCD     TIMER34_IRQHandler        ; 23: Timer3 / Timer4
                DCD     0                         ; 24: Reserved
                DCD     0                         ; 25: Reserved
                DCD     0                         ; 26: Reserved
                DCD     0                         ; 27: Reserved
                DCD     0       				  ; 28: Reserved 
                DCD     0                         ; 29: Reserved 
                DCD     0          				  ; 30: Reserved 
                DCD     0            		      ; 31: Reserved 
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDT_IRQHandler           [WEAK]
                EXPORT  SVD_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler           	[WEAK]
                EXPORT  FLASH_IRQHandler          [WEAK]
                EXPORT  FDET_IRQHandler           [WEAK]
                EXPORT  ADC_IRQHandler           	[WEAK]
                EXPORT  SPI0_IRQHandler          	[WEAK]
                EXPORT  SPI1_IRQHandler         	[WEAK]
                EXPORT  SPI2_IRQHandler          	[WEAK]
                EXPORT  UART_IRQHandler          	[WEAK]
                EXPORT  USART0_IRQHandler         [WEAK]
                EXPORT  USART1_IRQHandler         [WEAK]
                EXPORT  I2C_IRQHandler           	[WEAK]
                EXPORT  LCD_IRQHandler           	[WEAK]
                EXPORT  AES_IRQHandler          	[WEAK]
                EXPORT  LPTIM_IRQHandler          [WEAK]
                EXPORT  DMA_IRQHandler          	[WEAK]
                EXPORT  CRC_IRQHandler        		[WEAK]
                EXPORT  TRNG_IRQHandler           [WEAK]
                EXPORT  COPM0_IRQHandler          [WEAK]
                EXPORT  COPM1_IRQHandler          [WEAK]
                EXPORT  TIMER12_IRQHandler       	[WEAK]
                EXPORT  TIMER34_IRQHandler        [WEAK]

WWDT_IRQHandler
SVD_IRQHandler
RTC_IRQHandler
FLASH_IRQHandler
FDET_IRQHandler
ADC_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
UART_IRQHandler
USART0_IRQHandler
USART1_IRQHandler
I2C_IRQHandler
LCD_IRQHandler
AES_IRQHandler
LPTIM_IRQHandler
DMA_IRQHandler
CRC_IRQHandler
TRNG_IRQHandler
COPM0_IRQHandler
COPM1_IRQHandler
TIMER12_IRQHandler
TIMER34_IRQHandler

                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

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
