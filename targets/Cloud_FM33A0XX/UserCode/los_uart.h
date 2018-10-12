#ifndef _LOS_DEV_ST_UART_H
#define _LOS_DEV_ST_UART_H


//LiteOS header files
/*
#include "los_base.ph"
#include "los_hwi.h"
#include "los_sem.h"
*/
//BSP header files
//#include "stm32l4xx_hal.h"
#include <stdint.h>


#define CN_COM_NUM 8


// init uart puarts[uartidx], puarts please see los_dev_st_uart.c
int los_dev_uart_init(int uartidx, int baudrate, char *buf, int size);
// read data from puarts[uartidx] , puarts please see los_dev_st_uart.c
int los_dev_uart_read(int uartidx, char *outbuf, int rsize, int mstimeout);
// write data to puarts[uartidx] , puarts please see los_dev_st_uart.c
int los_dev_uart_write(int uartidx, char *inbuf, int wsize, int mstimeout);
// flush all the  receive data in the uart 
int los_dev_uart_rflush(int uartidx);

#endif
