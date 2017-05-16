#include "los_bsp_uart.h"
#include <stdarg.h>

#ifdef EFM32PG1B200F256GM48
#include "retargetserial.h"

void Error_Handler(void)
{
  /* Turn LED4 on */
  //BSP_LED_On(LED4);
  while(1)
  {
  }
}
#endif

void LOS_EvbUartInit(void)
{
#ifdef EFM32PG1B200F256GM48
  /* Initialize LEUART/USART and map LF to CRLF */
  RETARGET_SerialInit();
  RETARGET_SerialCrLf(1);
#endif
	
	return ;
}


/*************************************************************************************************
 *  功能：向串口1发送一个字符                                                                    *
 *  参数：(1) 需要被发送的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
#ifdef EFM32PG1B200F256GM48
	USART_RETARGET_WriteChar(c);
#endif
	return ;
}

/*************************************************************************************************
 *  功能：向串口1发送一个字符串                                                                  *
 *  参数：(1) 需要被发送的字符串                                                                 *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef EFM32PG1B200F256GM48
    while (*str)
    {
			USART_RETARGET_WriteChar(*str);
			str++;
    }
#endif
	return ;
}


/*************************************************************************************************
 *  功能：从串口1接收一个字符                                                                    *
 *  参数：(1) 存储接收到的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
#ifdef EFM32PG1B200F256GM48
	*c = USART_RETARGET_ReadChar();
#endif
	return ;
}

#ifdef EFM32PG1B200F256GM48
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef EFM32PG1B200F256GM48
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
	return ;
}
