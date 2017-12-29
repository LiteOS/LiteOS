#ifndef __X_MODEM_H__
#define __X_MODEM_H__

#include "los_bsp_uart.h"

#define LOS_xModemHalPortInByte(a) LOS_EvbUartReadByte(a)
#define LOS_xModemHalPortOutByte(a) LOS_EvbUartWriteByte(a)

int LOS_xModemTransmit(unsigned char *src, int srcsz);
unsigned short LOS_xModemCRC16(const unsigned char *buf, int len);




#endif
