        PRESERVE8

    AREA    RESET, CODE, READONLY
    THUMB

    IMPORT  ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||
    IMPORT  osPendSV

    EXPORT  _BootVectors
    EXPORT  Reset_Handler

_BootVectors
    DCD     ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||
    DCD     Reset_Handler


Reset_Handler
	LDR.W	R0, =0xE000ED88
	LDR		R1, [R0]
	ORR		R1, R1, #(0xF << 20)
	STR		R1, [R0]
    CPSID   I
	
    IMPORT  __main
    LDR     R0, =__main
    BX      R0


    ALIGN
    END

