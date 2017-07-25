#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

void LOS_EvbUartInit(void);
void LOS_EvbUartWriteByte(char c);
void LOS_EvbUartWriteStr(const char* str);
void LOS_EvbUartReadByte(char* c);

#endif /*_LOS_BSP_UART_H*/
