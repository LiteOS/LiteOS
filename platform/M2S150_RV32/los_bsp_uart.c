#include "los_bsp_uart.h"
#include <stdarg.h>
#include <stdio.h>

/******************************************************************************
 * CoreUARTapb instance data.
 *****************************************************************************/
#ifdef LOS_M2S150_RV32

#include "core_uart_apb.h"
#include "hw_platform.h"
UART_instance_t g_uart;

#endif /*LOS_M2S150_RV32*/

void LOS_EvbUartInit(void)
{
#ifdef LOS_M2S150_RV32

	UART_init(&g_uart,
    		  COREUARTAPB0_BASE_ADDR,
			  BAUD_VALUE_115200,
              (DATA_8_BITS | NO_PARITY) );

#endif /*LOS_M2S150_RV32*/

}


/*************************************************************************************************
 *  ���ܣ��򴮿�1����һ���ַ�                                                                    *
 *  ������(1) ��Ҫ�����͵��ַ�                                                                   *
 *  ���أ�                                                                                       *
 *  ˵����                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
#ifdef LOS_M2S150_RV32

	UART_send(&g_uart,(const uint8_t *)&c,1);

#endif
}

/*************************************************************************************************
 *  ���ܣ��򴮿�1����һ���ַ���                                                                  *
 *  ������(1) ��Ҫ�����͵��ַ���                                                                 *
 *  ���أ�                                                                                       *
 *  ˵����                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef LOS_M2S150_RV32

	UART_polled_tx_string(&g_uart,(const uint8_t *)str);

#endif
}


/*************************************************************************************************
 *  ���ܣ��Ӵ���1����һ���ַ�                                                                    *
 *  ������(1) �洢���յ����ַ�                                                                   *
 *  ���أ�                                                                                       *
 *  ˵����                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
#ifdef LOS_M2S150_RV32

	UART_get_rx( &g_uart, (uint8_t *)c, 1 );

#endif

}

#ifdef LOS_M2S150_RV32

static char _buffer[128];

#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_M2S150_RV32

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

}
