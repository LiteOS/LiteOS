
        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
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
		DCD     WWDT_IRQHandler           ; 0:  WWDT 
		DCD     SVD_IRQHandler            ; 1:  SVD 	
		DCD     RTC_IRQHandler            ; 2:  RTC 	
		DCD     FLASH_IRQHandler          ; 3:  NVMIF	
		DCD     FDET_IRQHandler           ; 4:  FDET	
		DCD     ADC_IRQHandler            ; 5:  ADC	
		DCD     HSPI_IRQHandler           ; 6:  HSPI	
		DCD     SPI1_IRQHandler           ; 7:  SPI1 	
		DCD     SPI2_IRQHandler           ; 8:  SPI2	
		DCD     UART0_IRQHandler          ; 9:  UART0	
		DCD     UART1_IRQHandler          ; 10:  UART1	
		DCD     UART2_IRQHandler          ; 11:  UART2	
		DCD     UART3_IRQHandler          ; 12:  UART3	
		DCD     UART4_IRQHandler          ; 13:  UART4	
		DCD     UART5_IRQHandler          ; 14:  UART5	
		DCD     U78160_IRQHandler         ; 15:  U78160	
		DCD     U78161_IRQHandler         ; 16:  U78161	
		DCD     I2C_IRQHandler            ; 17:  I2C	
		DCD     LCD_IRQHandler            ; 18:  LCD DISP	
		DCD     AES_IRQHandler         	  ; 19:  AES	
		DCD     LPTIM_IRQHandler          ; 20:  LPTIM	
		DCD     DMA_IRQHandler            ; 21:  DMA	
		DCD     TRNG_IRQHandler           ; 22:  TRNG	
		DCD     COMP_IRQHandler           ; 23:  COMP	
		DCD     BTIM1_IRQHandler          ; 24:  BTIMER1 BT1
		DCD     BTIM2_IRQHandler          ; 25:  BTIMER2 BT2
		DCD     ETIM1_IRQHandler          ; 26:  ETIMER1	
		DCD     ETIM2_IRQHandler          ; 27:  ETIMER2	
		DCD     ETIM3_IRQHandler          ; 28:  ETIMER3	
		DCD     ETIM4_IRQHandler          ; 39:  ETIMER4	
        DCD     GPIO_IRQHandler           ; 30:  GPIO 
        DCD     0            		      ; 31: Reserved
                
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        IMPORT  SystemInit
        LDR     R0, =SystemInit
        BLX     R0		
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler
        
        
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler
       
        
        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler
        
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler  
        B PendSV_Handler
        
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler
        
        
        PUBWEAK WWDT_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WWDT_IRQHandler
        B WWDT_IRQHandler
        
                
        PUBWEAK SVD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVD_IRQHandler
        B SVD_IRQHandler
        
                
        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler
        
                
        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler
        
                
        PUBWEAK FDET_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FDET_IRQHandler
        B FDET_IRQHandler
        
                
        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler
        
                
        PUBWEAK HSPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HSPI_IRQHandler
        B HSPI_IRQHandler
        

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler
        
                                
        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler
        
                
        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART5_IRQHandler
        B UART5_IRQHandler
        
                
        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART4_IRQHandler
        B UART4_IRQHandler
        
                
        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART3_IRQHandler
        B UART3_IRQHandler
        
                
        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART2_IRQHandler
        B UART2_IRQHandler
        
                
        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler
        
                 
        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART0_IRQHandler
        B UART0_IRQHandler
        
                 
        PUBWEAK U78160_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
U78160_IRQHandler
        B U78160_IRQHandler
        
                
        PUBWEAK U78161_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
U78161_IRQHandler
        B U78161_IRQHandler
        
                
        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_IRQHandler
        B I2C_IRQHandler
                  
                  
        PUBWEAK LCD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LCD_IRQHandler
        B LCD_IRQHandler
                
                
        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AES_IRQHandler
        B AES_IRQHandler
        
        
        PUBWEAK LPTIM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM_IRQHandler
        B LPTIM_IRQHandler


        PUBWEAK DMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA_IRQHandler
        B DMA_IRQHandler


        PUBWEAK CRC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
CRC_IRQHandler
        B CRC_IRQHandler     


        PUBWEAK TRNG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TRNG_IRQHandler
        B TRNG_IRQHandler
        
                
        PUBWEAK COMP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
COMP_IRQHandler
        B COMP_IRQHandler
        
        
        PUBWEAK BTIM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
BTIM1_IRQHandler
        B BTIM1_IRQHandler
        
        
        PUBWEAK BTIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
BTIM2_IRQHandler
        B BTIM2_IRQHandler
         
         
        PUBWEAK ETIM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETIM1_IRQHandler
        B ETIM1_IRQHandler
        
        
         PUBWEAK ETIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETIM2_IRQHandler
        B ETIM2_IRQHandler
        
        
         PUBWEAK ETIM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETIM3_IRQHandler
        B ETIM3_IRQHandler
        
        
         PUBWEAK ETIM4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETIM4_IRQHandler
        B ETIM4_IRQHandler
        
        
         PUBWEAK GPIO_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPIO_IRQHandler
        B GPIO_IRQHandler
     
         
         END
