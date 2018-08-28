/**************************************************************************//**
 * @file     retarget.c
 * @version  V1.00
 * $Revision: 12 $
 * $Date: 15/10/30 3:33p $
 * @brief    NUC472/NUC442 Debug Port and Semihost Setting Source File
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NUC472_442.h"

#if defined ( __CC_ARM   )
#if (__ARMCC_VERSION < 400000)
#else
/* Insist on keeping widthprec, to avoid X propagation by benign code in C-lib    */
#pragma import _printf_widthprec
#endif
#endif

/* Un-comment this line to disable all printf and getchar. getchar() will always return 0x00*/
//#define DISABLE_UART

#if defined(DEBUG_ENABLE_SEMIHOST)
    #ifndef DISABLE_UART
        #define DISABLE_UART
    #endif
#endif

/*---------------------------------------------------------------------------------------------------------   */
/* Global variables                                                                                           */
/*---------------------------------------------------------------------------------------------------------   */
#if !(defined(__ICCARM__) && (__VER__ >= 6010000))
struct __FILE {
    int handle; /* Add whatever you need here */
};
#endif
FILE __stdout;
FILE __stdin;

enum { r0, r1, r2, r3, r12, lr, pc, psr};

/**
 * @brief       Helper function to dump register while hard fault occurred
 * @param[in]   stack pointer points to the dumped registers in SRAM
 * @return      None
 */
static void stackDump(uint32_t stack[])
{   
    printf("r0  = 0x%x\n", stack[r0]);
    printf("r1  = 0x%x\n", stack[r1]);
    printf("r2  = 0x%x\n", stack[r2]);
    printf("r3  = 0x%x\n", stack[r3]);
    printf("r12 = 0x%x\n", stack[r12]);
    printf("lr  = 0x%x\n", stack[lr]);
    printf("pc  = 0x%x\n", stack[pc]);
    printf("psr = 0x%x\n", stack[psr]);
}

/**
 * @brief       Hard fault handler
 * @param[in]   stack pointer points to the dumped registers in SRAM
 * @return      None
 * @note        Replace while(1) at the end of this function with chip reset if WDT is not enabled for end product
 */
void Hard_Fault_Handler(uint32_t stack[])
{
    printf("In Hard Fault Handler\n");

    stackDump(stack);
    // Replace while(1) with chip reset if WDT is not enabled for end product
    while(1);
    //SYS->IPRSTC1 = SYS_IPRSTC1_CHIP_RST_Msk;
}

#if defined(DEBUG_ENABLE_SEMIHOST)
/* The static buffer is used to speed up the semihost    */
static char g_buf[16];
static char g_buf_len = 0;

# if defined(__ICCARM__)

void SH_End(void)
{
    asm("MOVS   R0,#1 \n"        //; Set return value to 1
        "BX     lr    \n"            //; Return
       );
}

void SH_ICE(void)
{
  asm("CMP   R2,#0   \n"
      "BEQ   SH_End  \n"
      "STR   R0,[R2] \n"       //; Save the return value to *pn32Out_R0
     );
}

/**
 *
 * @brief      The function to process semihosted command
 * @param[in]  n32In_R0  : semihost register 0
 * @param[in]  n32In_R1  : semihost register 1
 * @param[out] pn32Out_R0: semihost register 0
 * @retval     0: No ICE debug
 * @retval     1: ICE debug
 *
 */
int32_t SH_DoCommand(int32_t n32In_R0, int32_t n32In_R1, int32_t *pn32Out_R0)
{
    asm("BKPT   0xAB   \n"       //; This instruction will cause ICE trap or system HardFault
        "B      SH_ICE \n"
        "SH_HardFault: \n"       //; Captured by HardFault
        "MOVS   R0,#0  \n"       //; Set return value to 0
        "BX     lr     \n"       //; Return
        );

    return 1;                    //; Return 1 when it is trap by ICE
}

void Get_LR_and_Branch(void)
{
    asm("MOV     R1, LR               \n" //; LR current value
        "B       Hard_Fault_Handler   \n"
       );
}

void Stack_Use_MSP(void)
{
    asm("MRS     R0, MSP           \n" //; stack use MSP
        "B       Get_LR_and_Branch \n"
       );
}

void HardFault_Handler_Ret(void)
{
    asm("MOVS    r0, #4                        \n"
        "MOV     r1, LR                        \n"
        "TST     r0, r1                        \n"
        "BEQ     Stack_Use_MSP                 \n"
        "MRS     R0, PSP                       \n" //; stack use PSP
        "B       Get_LR_and_Branch             \n"
       );
}

void SP_Read_Ready(void)
{
    asm("LDR     R1, [R0, #24] \n"        //; Get previous PC
        "LDRH    R3, [R1]      \n"        //; Get instruction
        "LDR     R2, [pc, #8]  \n"        //; The special BKPT instruction
        "CMP     R3, R2        \n"        //; Test if the instruction at previous PC is BKPT
        "BNE     HardFault_Handler_Ret \n" //; Not BKPT
        "ADDS    R1, #4        \n"        //; Skip BKPT and next line
        "STR     R1, [R0, #24] \n"        //; Save previous PC
        "BX      lr            \n"        //; Return
        "DCD     0xBEAB        \n"        //; BKPT instruction code
        "B       HardFault_Handler_Ret \n"
       );
}


void SP_is_PSP(void)
{
    asm(
        "MRS     R0, PSP       \n"
       );
}

/**
 * @brief    This HardFault handler is implemented to support semihost
 *
 * @param    None
 *
 * @returns  None
 *
 * @details  This function is implement to support semihost message print.
 *
 */
void HardFault_Handler (void)
{
    asm("MOV     R0, lr        \n"
        "LSLS    R0, #29       \n"        //; Check bit 2
        "BMI     SP_is_PSP     \n"        //; previous stack is PSP
        "MRS     R0, MSP       \n"        //; previous stack is MSP, read MSP
        "B       SP_Read_Ready \n"
       );

    while(1);
}

# else

/**
 * @brief    This HardFault handler is implemented to support semihost
 *
 * @param    None
 *
 * @returns  None
 *
 * @details  This function is implement to support semihost message print.
 *
 */
__asm int32_t HardFault_Handler(void)
{
    MOV     R0, LR
    LSLS    R0, #29               //; Check bit 2
    BMI     SP_is_PSP             //; previous stack is PSP
    MRS     R0, MSP               //; previous stack is MSP, read MSP
    B       SP_Read_Ready
SP_is_PSP
    MRS     R0, PSP               //; Read PSP

SP_Read_Ready
    LDR     R1, [R0, #24]         //; Get previous PC
    LDRH    R3, [R1]              //; Get instruction
    LDR     R2, =0xBEAB           //; The special BKPT instruction
                 CMP     R3, R2                //; Test if the instruction at previous PC is BKPT
                 BNE     HardFault_Handler_Ret //; Not BKPT

                 ADDS    R1, #4                //; Skip BKPT and next line
                 STR     R1, [R0, #24]         //; Save previous PC

                 BX      LR                    //; Return
HardFault_Handler_Ret

    /* TODO: Implement your own hard fault handler here. */
    MOVS    r0, #4
    MOV     r1, LR
    TST     r0, r1
    BEQ     Stack_Use_MSP
    MRS     R0, PSP ;stack use PSP
    B       Get_LR_and_Branch
Stack_Use_MSP
    MRS     R0, MSP ; stack use MSP
Get_LR_and_Branch
    MOV     R1, LR ; LR current value
    LDR     R2,=__cpp(Hard_Fault_Handler)
    BX      R2

    B       .

                 ALIGN
}

/**
 *
 * @brief      The function to process semihosted command
 * @param[in]  n32In_R0  : semihost register 0
 * @param[in]  n32In_R1  : semihost register 1
 * @param[out] pn32Out_R0: semihost register 0
 * @retval     0: No ICE debug
 * @retval     1: ICE debug
 *
 */
__asm int32_t SH_DoCommand(int32_t n32In_R0, int32_t n32In_R1, int32_t *pn32Out_R0)
{
    BKPT   0xAB          //; Wait ICE or HardFault
    //; ICE will step over BKPT directly
    //; HardFault will step BKPT and the next line
    B      SH_ICE

SH_HardFault             //; Captured by HardFault
    MOVS   R0, #0        //; Set return value to 0
    BX     lr            //; Return

SH_ICE                   //; Captured by ICE
    //; Save return value
    CMP    R2, #0
    BEQ    SH_End
    STR    R0, [R2]      //; Save the return value to *pn32Out_R0

SH_End
    MOVS   R0, #1        //; Set return value to 1
    BX     lr            //; Return
}
#endif

#else

# if defined(__ICCARM__)

void Get_LR_and_Branch(void)
{
    asm("MOV     R1, LR                  \n" //; LR current value
        "B       Hard_Fault_Handler      \n"
       );
}

void Stack_Use_MSP(void)
{
    asm("MRS     R0, MSP           \n" //; stack use MSP
        "B       Get_LR_and_Branch \n"
       );
}

/**
 * @brief    This HardFault handler is implemented to show r0, r1, r2, r3, r12, lr, pc, psr
 *
 * @param    None
 *
 * @returns  None
 *
 * @details  This function is implement to print r0, r1, r2, r3, r12, lr, pc, psr.
 *
 */
void HardFault_Handler(void)
{
    asm("MOVS    r0, #4                        \n"
        "MOV     r1, LR                        \n"
        "TST     r0, r1                        \n"
        "BEQ     Stack_Use_MSP                 \n"
        "MRS     R0, PSP                       \n" //; stack use PSP
        "B       Get_LR_and_Branch             \n"
       );

    while(1);
}

# else

/**
 * @brief    This HardFault handler is implemented to show r0, r1, r2, r3, r12, lr, pc, psr
 *
 * @param    None
 *
 * @returns  None
 *
 * @details  This function is implement to print r0, r1, r2, r3, r12, lr, pc, psr
 *
 */
__asm int32_t HardFault_Handler(void)
{
    MOVS    r0, #4
    MOV     r1, LR
    TST     r0, r1
    BEQ     Stack_Use_MSP
    MRS     R0, PSP ;stack use PSP
    B       Get_LR_and_Branch
Stack_Use_MSP
    MRS     R0, MSP ; stack use MSP
Get_LR_and_Branch
    MOV     R1, LR ; LR current value
    LDR     R2,=__cpp(Hard_Fault_Handler)
    BX      R2
}

#endif

#endif

#define DEBUG_PORT   UART0

/**
  * @brief  Write a char to UART.
  * @param  ch The character sent to UART.
  * @return None
  */
void SendChar_ToUART(int ch)
{
#ifndef DISABLE_UART
    while((DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk)); //waits for TXFULL bit is clear
    DEBUG_PORT->DAT = ch;
    if(ch == '\n') {
        while((DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk)); //waits for TXFULL bit is clear
        DEBUG_PORT->DAT = '\r';
    }
#endif
}

/**
  * @brief  Write a char to debug console.
  * @param  ch The character sent to debug console
  * @return None
  */
void SendChar(int ch)
{
#if defined(DEBUG_ENABLE_SEMIHOST)
    g_buf[g_buf_len++] = ch;
    g_buf[g_buf_len] = '\0';
    if(g_buf_len + 1 >= sizeof(g_buf) || ch == '\n' || ch == '\0') {

        /* Send the char */
        if(SH_DoCommand(0x04, (int)g_buf, NULL) != 0) {
            g_buf_len = 0;
            return;
        } else {
            int i;

            for(i=0; i<g_buf_len; i++)
                SendChar_ToUART(g_buf[i]);
            g_buf_len = 0;
        }
    }
#else
    SendChar_ToUART(ch);
#endif
}

/**
  * @brief  Read a char from debug console.
  * @param  None
  * @return Received character from debug console
  * @note   This API waits until UART debug port or semihost input a character
  */
char GetChar(void)
{
#if defined(DEBUG_ENABLE_SEMIHOST)
# if defined ( __CC_ARM   )
    int nRet;
    while(SH_DoCommand(0x101, 0, &nRet) != 0) {
        if(nRet != 0) {
            SH_DoCommand(0x07, 0, &nRet);
            return (char)nRet;
        }
    }
# else
    int nRet;
    while(SH_DoCommand(0x7, 0, &nRet) != 0) {
        if(nRet != 0)
            return (char)nRet;
    }
# endif
#endif

#ifndef DISABLE_UART
    while (1) {
        if((DEBUG_PORT->FIFOSTS & UART_FIFOSTS_RXEMPTY_Msk) == 0 ) {
            return (DEBUG_PORT->DAT);

        }
    }
#else
    return(0);
#endif
}


/**
  * @brief  Check whether UART receive FIFO is empty or not.
  * @param  None
  * @return UART Rx FIFO empty status
  * @retval 1 Indicates at least one character is available in UART Rx FIFO
  * @retval 0 UART Rx FIFO is empty
  */
int kbhit(void)
{
#ifndef DISABLE_UART
    return !(DEBUG_PORT->FIFOSTS & UART_FIFOSTS_RXFULL_Msk);
#else
    return(0);
#endif
}

/**
  * @brief  Check whether UART transmit FIFO is empty or not.
  * @param  None
  * @return UART Tx FIFO empty status
  * @retval 1 UART Tx FIFO is empty
  * @retval 0 UART Tx FIFO is not empty
  */
int IsDebugFifoEmpty(void)
{
#ifndef DISABLE_UART
    return (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXEMPTYF_Msk);
#else
    return(1);
#endif
}

/**
  * @brief    C library retargetting
  * @param    ch Character to send out
  * @return  None
  * @details  Check if message finished (FIFO empty of debug port)
  */

void _ttywrch(int ch)
{
    SendChar(ch);
    return;
}

/**
  * @brief      Write character to stream
  * @param[in]  ch       Character to be written. The character is passed as its int promotion.
  * @param[in]  stream   Pointer to a FILE object that identifies the stream where the character is to be written.
  * @return    If there are no errors, the same character that has been written is returned.
  *             If an error occurs, EOF is returned and the error indicator is set (see ferror).
  * @details    Writes a character to the stream and advances the position indicator.\n
  *             The character is written at the current position of the stream as indicated \n
  *             by the internal position indicator, which is then advanced one character.
  * @note       The above descriptions are copied from http://www.cplusplus.com/reference/clibrary/cstdio/fputc/.
  *
  */
int fputc(int ch, FILE *stream)
{
    SendChar(ch);
    return ch;
}

/**
  * @brief      Get character from UART debug port or semihosting input
  * @param[in]  stream   Pointer to a FILE object that identifies the stream on which the operation is to be performed.
  * @return    The character read from UART debug port or semihosting
  * @details    For get message from debug port or semihosting.
  *
  */
int fgetc(FILE *stream)
{
    return (GetChar());
}

/**
  * @brief      Check error indicator
  * @param[in]  stream   Pointer to a FILE object that identifies the stream.
  * @return    If the error indicator associated with the stream was set, the function returns a nonzero value.
  *            Otherwise, it returns a zero value.
  * @details    Checks if the error indicator associated with stream is set, returning a value different
  *             from zero if it is. This indicator is generally set by a previous operation on the stream that failed.
  * @note       The above descriptions are copied from http://www.cplusplus.com/reference/clibrary/cstdio/ferror/.
  */
int ferror(FILE *stream)
{
    return EOF;
}

#ifdef DEBUG_ENABLE_SEMIHOST
# ifdef __ICCARM__
void __exit(int return_code)
{

    /* Check if link with ICE    */
    if(SH_DoCommand(0x18, 0x20026, NULL) == 0) {
        /* Make sure all message is print out    */
        while(IsDebugFifoEmpty() == 0);
    }
label:
    goto label;  /* endless loop    */
}
# else
void _sys_exit(int return_code)
{

    /* Check if link with ICE    */
    if(SH_DoCommand(0x18, 0x20026, NULL) == 0) {
        /* Make sure all message is print out    */
        while(IsDebugFifoEmpty() == 0);
    }
label:
    goto label;  /* endless loop    */
}
# endif
#endif

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
