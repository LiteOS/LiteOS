#ifndef _LOS_DEV_ST_UART_H
#define _LOS_DEV_ST_UART_H


//LiteOS header files

#include "los_base.ph"
#include "los_hwi.h"
#include "los_sem.h"

//BSP header files
#include "stm32l4xx_hal.h"
#define LOS_STM32L431_UART1 1
#define LOS_STM32L431_UART2 2
#define LOS_STM32L431_UART3 3

typedef struct _los_dev_uart_t
{
    UART_HandleTypeDef dev;
    UINT32 rsem;// liteos mutex for uart read
    UINT32 wsem;// liteos mutex for uart write
    char *pbuf;//   uart receive data buf
    char *dualbuf;//   uart receive data buf
    char *interrbuf;//   uart receive data buf
    int size;  //   uart receive data buf size
    int readcnt;//  received data bytes
}los_dev_uart_t;

// init uart puarts[uartidx], puarts please see los_dev_st_uart.c
int los_dev_uart_init(int uartidx, int baudrate, char *buf, int size);
// read data from puarts[uartidx] , puarts please see los_dev_st_uart.c
int los_dev_uart_read(int uartidx, char *outbuf, int rsize, int mstimout);
// write data to puarts[uartidx] , puarts please see los_dev_st_uart.c
int los_dev_uart_write(int uartidx, char *inbuf, int wsize, int mstimout);

/* uart irq handler */
void los_dev_uart_irqhandler(int uartidx);

#endif
