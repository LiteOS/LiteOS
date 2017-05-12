/******************************************************
 *             ATSAMD21 串口通信模块
 ******************************************************/
#ifndef _SERCOM_UART_H_
#define _SERCOM_UART_H_

extern int sercom_uart_init(void);
extern void USART_putc(char c);
extern int  USART_getc(void);
extern int usart_write_wait(const unsigned short tx_data);
extern int usart_read_wait(unsigned short *const rx_data);
extern int usart_write_buffer_wait(
        const unsigned char *tx_data,
        unsigned short length);
        
#endif
