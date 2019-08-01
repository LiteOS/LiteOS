/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "usart.h"

#include "stm32l4xx.h"
#include <stdint.h>
#include <stddef.h>

#include <osal.h>
#include <link_misc.h>


#define CN_RCV_RING_BUFLEN  128

static USART_TypeDef*     s_pUSART = USART1;
static uint32_t           s_uwIRQn = USART1_IRQn;

static bool_t s_uart_init = false;

/* USART1 init function */
UART_HandleTypeDef uart_debug;
static tag_ring_buffer_t  s_uartdebug_rcv_ring;
static unsigned char      s_uartdebug_rcv_ringmem[CN_RCV_RING_BUFLEN];
static osal_semp_t        s_uartdebug_rcv_sync;
static void uart_debug_irq(void)
{
    unsigned char value;
    if(__HAL_UART_GET_FLAG(&uart_debug, UART_FLAG_RXNE) != RESET)
    {
        value = (uint8_t)(uart_debug.Instance->RDR & 0x00FF);
        ring_write(&s_uartdebug_rcv_ring,&value,1);
        osal_semp_post(s_uartdebug_rcv_sync);
    }
    else if (__HAL_UART_GET_FLAG(&uart_debug,UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&uart_debug);
    }
}
void shell_uart_init(int baud)
{
    uart_debug.Instance = s_pUSART;
    uart_debug.Init.BaudRate = baud;
    uart_debug.Init.WordLength = UART_WORDLENGTH_8B;
    uart_debug.Init.StopBits = UART_STOPBITS_1;
    uart_debug.Init.Parity = UART_PARITY_NONE;
    uart_debug.Init.Mode = UART_MODE_TX_RX;
    uart_debug.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart_debug.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&uart_debug) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    LOS_HwiCreate(s_uwIRQn, 3, 0, uart_debug_irq, 0);
    //__HAL_UART_ENABLE_IT(&uart_debug, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&uart_debug, UART_IT_RXNE);
    
    //create the receive buffer and receive sync
    osal_semp_create(&s_uartdebug_rcv_sync,CN_RCV_RING_BUFLEN,0);
    ring_init(&s_uartdebug_rcv_ring,s_uartdebug_rcv_ringmem,CN_RCV_RING_BUFLEN,0,0);
    s_uart_init = true;

}


/* define fputc */
#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&uart_debug, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
int fgetc(FILE *f){
    int ret = 0;
    unsigned char  value;
    do
    {
        if(osal_semp_pend(s_uartdebug_rcv_sync,LOS_WAIT_FOREVER))
        {
            ret = ring_read(&s_uartdebug_rcv_ring,&value,1);
        }
    }while(ret <=0);
    ret = value;
    return ret;
}
#elif defined ( __GNUC__ )  /* GCC: printf will call _write to print */


__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    if(s_uart_init)
    {
        HAL_UART_Transmit(&uart_debug, (uint8_t *)ptr, len, 0xFFFF);
    }
    return len;
}
__attribute__((used)) int _read(int fd, char *ptr, int len)
{
    int ret = 0;
    unsigned char  value;
    do
    {
        if(osal_semp_pend(s_uartdebug_rcv_sync,cn_osal_timeout_forever))
        {
            ret = ring_read(&s_uartdebug_rcv_ring,&value,1);
        }
    }while(ret <=0);
    *(unsigned char *)ptr = value;
    return 1;
}

#endif
