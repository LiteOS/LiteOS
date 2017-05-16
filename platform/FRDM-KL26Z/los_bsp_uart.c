#include "los_bsp_uart.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef FRDM_KL25Z
#include "MKL25Z4.h"
#endif

#ifdef FRDM_KL26Z
#include "MKL26Z4.h"
#endif

#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
void BSP_UART_Init (uint32_t baud)
{
    register uint16_t sbr, osr;
    uint8_t temp;
    uint32_t sysclk,tmp_baud;
    uint16_t tmp_diff=~0,tmp_sbr,diff_abs_baud;

    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK; //使能 UART0 时钟

    PORTA_PCR1  &=  ~(0x07<<8);
    PORTA_PCR1  |=   0x02<<8; //mode

    PORTA_PCR2  &=  ~(0x07<<8);
    PORTA_PCR2  |=   0x02<<8; //mode

    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM_SOPT2 |= (0
                  | SIM_SOPT2_UART0SRC(1)
                  | SIM_SOPT2_PLLFLLSEL_MASK
                 );

    // 设置的时候，禁止发送接受
    UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
    UART0_C1 = 0;       //默认工作模式
    sysclk = 48000000/2;

    //UART 波特率 = UART 模块时钟/((OSR+1) × SBR)
    //4 <= OSR <= 32
    //SBR = (1 ,8191)
    for(temp = 4;temp<=32;temp++)
    {
        tmp_sbr = sysclk/(baud*(temp+1));
        if(tmp_sbr > 0x1FFF)
        {
            continue;
        }
        tmp_baud = sysclk/(tmp_sbr*(temp+1));
        diff_abs_baud =  (int32_t)(tmp_baud - baud);
        if(diff_abs_baud < 0)
        {
            diff_abs_baud = -diff_abs_baud;
        }
        
        if(diff_abs_baud == 0)
        {
            sbr = tmp_sbr;
            osr = temp;
            break;
        }
        if(tmp_diff > diff_abs_baud )
        {
            tmp_diff = diff_abs_baud;
            sbr = tmp_sbr;
            osr = temp;
        }

        //由于不一定是整除，因此还需要 加1
        tmp_sbr++;
        if(tmp_sbr > 0x1FFF)
        {
            continue;
        }
        tmp_baud = sysclk/(tmp_sbr*(temp+1));
        diff_abs_baud =  (int32_t)(tmp_baud - baud);
        if(diff_abs_baud < 0)
        {
            diff_abs_baud = -diff_abs_baud;
        }
            
        if(diff_abs_baud == 0)
        {
            sbr = tmp_sbr;
            osr = temp;
            break;
        }
        if(tmp_diff > diff_abs_baud )
        {

            sbr = tmp_sbr;
            osr = temp;
        }
    }

    //写 SBR
    temp = UART0_BDH & (~UART0_BDH_SBR_MASK);           //缓存 清空 SBR 的 UARTx_BDH的值
    UART0_BDH = temp |  UART0_BDH_SBR(sbr >> 8);        //先写入SBR高位
    UART0_BDL = UART0_BDL_SBR(sbr);                     //再写入SBR低位

    //写 OSR
    temp = UART0_C4 & (~UART0_C4_OSR_MASK) ;            //缓存 清空 BRFA 的 UARTx_C4 的值
    UART0_C4 = temp |  UART0_C4_OSR(osr);               //写入BRFA
        
    UART0_C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);  // 使能发送接受
}

uint8_t byte;
void UART0_IRQHandler(void)
{
    if (UART0_S1&UART_S1_RDRF_MASK)
    {
        byte = UART0_D;
    }
}
#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU
int fputc(int ch,FILE *f)
{
    while(!(UART0_S1&UART_S1_TDRE_MASK) && !(UART0_S1&UART_S1_TC_MASK));
    UART0_D  = ch;
    
    return(ch);
}
#endif
#endif

void LOS_EvbUartInit(void)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    BSP_UART_Init(115200);
#endif
    
    return;
}


/*************************************************************************************************
 *  功能：向串口1发送一个字符                                                                    *
 *  参数：(1) 需要被发送的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    while(!(UART0_S1&UART_S1_TDRE_MASK) && !(UART0_S1&UART_S1_TC_MASK));
    UART0_D  = c;
#endif
    return;
}

/*************************************************************************************************
 *  功能：向串口1发送一个字符串                                                                  *
 *  参数：(1) 需要被发送的字符串                                                                 *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    while (*str)
    {
        while(!(UART0_S1&UART_S1_TDRE_MASK) && !(UART0_S1&UART_S1_TC_MASK));
        UART0_D  = *str;
        str++;
    }
#endif
    return;
}


/*************************************************************************************************
 *  功能：从串口1接收一个字符                                                                    *
 *  参数：(1) 存储接收到的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    /* Wait until character has been received */
    while (!(UART0_S1 & UART0_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    *c = UART0_D;
#endif
    return;
}

#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#if (defined FRDM_KL25Z) || (defined FRDM_KL26Z)
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
#endif
    return;
}
