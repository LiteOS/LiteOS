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
#include <driver.h>
#include "sys/fcntl.h"
#include <iot_link_config.h>


UART_HandleTypeDef uart_at;
static USART_TypeDef*     s_pUSART = LPUART1;
static uint32_t           s_uwIRQn = LPUART1_IRQn;

#ifndef CONFIG_UARTAT_RCVMAX
#define CONFIG_UARTAT_RCVMAX     1024  //cache a frame
#endif

#ifndef CONFIG_UARTAT_BAUDRATE
#define CONFIG_UARTAT_BAUDRATE    115200
#endif


#ifndef CONFIG_UARTAT_DEVNAME
#define CONFIG_UARTAT_DEVNAME    "atdev"
#endif

#define CN_RCVMEM_LEN  CONFIG_UARTAT_RCVMAX


struct atio_cb
{
    unsigned short        w_next;    //the next position to be write
    osal_semp_t           rcvsync;   //if a frame has been written to the ring, then active it
    tag_ring_buffer_t     rcvring;
    unsigned char         rcvbuf[CONFIG_UARTAT_RCVMAX];
    unsigned char         rcvringmem[CN_RCVMEM_LEN];
    //for the debug here
    unsigned int          rframeover; //how many times the frame has been over the max length
    unsigned int          rframedrop; //how many frame has been droped for memmory
    unsigned int          sndlen;     //how many bytes has been sent
    unsigned int          rcvlen;     //how many bytes has been received
    unsigned int          sndframe;   //how many frame has been sent
    unsigned int          rcvframe;   //how many frame has been received
    unsigned int          rcvringrst; //how many times the receive ring has been reset
};
static struct atio_cb   g_atio_cb;


/*******************************************************************************
function     :use this function to deal the uart interrupt
parameters   :
instruction  :we cached the data in the temp buffer,when the idle interrupt reached,
              then we write the data and the length to the ring if the ring has enough
              space
*******************************************************************************/
static void atio_irq(void)
{
    unsigned char  value;
    unsigned short ringspace;
    if (__HAL_UART_GET_FLAG(&uart_at, UART_FLAG_RXNE) != RESET) {
       value = (uint8_t)(uart_at.Instance->RDR & 0x00FF);
       g_atio_cb.rcvlen++;
       if (g_atio_cb.w_next < CONFIG_UARTAT_RCVMAX) {
           g_atio_cb.rcvbuf[g_atio_cb.w_next] = value;
           g_atio_cb.w_next++;
       } else {
            g_atio_cb.rframeover++;
       }
    } else if (__HAL_UART_GET_FLAG(&uart_at,UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&uart_at);
        ringspace = CN_RCVMEM_LEN - ring_buffer_datalen(&g_atio_cb.rcvring);
        if (ringspace < g_atio_cb.w_next) { //not enough mem
            g_atio_cb.rframedrop++;
        } else {
            //write data to the ring buffer:len+data format
            ringspace = g_atio_cb.w_next;
            ring_buffer_write(&g_atio_cb.rcvring,(unsigned char *)&ringspace,sizeof(ringspace));
            ring_buffer_write(&g_atio_cb.rcvring,g_atio_cb.rcvbuf,ringspace);
            (void) osal_semp_post(g_atio_cb.rcvsync);
            g_atio_cb.rcvframe++;
        }
        g_atio_cb.w_next=0; //write from the head
    } else { ///< clear the flags
        __HAL_UART_CLEAR_PEFLAG(&uart_at);
        __HAL_UART_CLEAR_FEFLAG(&uart_at);
        __HAL_UART_CLEAR_NEFLAG(&uart_at);
        __HAL_UART_CLEAR_OREFLAG(&uart_at);
    }
}

/*******************************************************************************
function     :use this function to initialize the uart
parameters   :
instruction  :
*******************************************************************************/
bool_t uart_at_init(void *pri)
{
    //initialize the at controller
    (void) memset(&g_atio_cb,0,sizeof(g_atio_cb));
    if (false == osal_semp_create(&g_atio_cb.rcvsync,CN_RCVMEM_LEN,0)) {
        printf("%s:semp create error\n\r",__FUNCTION__);
        goto EXIT_SEMP;
    }
    ring_buffer_init(&g_atio_cb.rcvring,g_atio_cb.rcvringmem,CN_RCVMEM_LEN,0,0);

    uart_at.Instance = s_pUSART;
    uart_at.Init.BaudRate = CONFIG_UARTAT_BAUDRATE;
    uart_at.Init.WordLength = UART_WORDLENGTH_8B;
    uart_at.Init.StopBits = UART_STOPBITS_1;
    uart_at.Init.Parity = UART_PARITY_NONE;
    uart_at.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart_at.Init.Mode = UART_MODE_TX_RX;
    uart_at.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&uart_at) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_UART_CLEAR_FLAG(&uart_at,UART_FLAG_TC);
    LOS_HwiCreate(s_uwIRQn, 3, 0, atio_irq, 0);
    __HAL_UART_ENABLE_IT(&uart_at, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&uart_at, UART_IT_RXNE);
    return true;

EXIT_SEMP:
    return false;
}

void uart_at_deinit(void *pri)
{
    __HAL_UART_DISABLE(&uart_at);
    __HAL_UART_DISABLE_IT(&uart_at, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(&uart_at, UART_IT_RXNE);
}

/*******************************************************************************
function     :use this function to send a frame to the uart
parameters   :
instruction  :
*******************************************************************************/
static ssize_t uart_at_send(const char  *buf, size_t len,uint32_t timeout)
{
    HAL_UART_Transmit(&uart_at,(unsigned char *)buf,len,timeout);
    g_atio_cb.sndlen += len;
    g_atio_cb.sndframe ++;

    return len;
}

/*******************************************************************************
function     :use this function to read a frame from the uart
parameters   :
instruction  :
*******************************************************************************/
static ssize_t uart_at_receive(void *buf,size_t len,uint32_t timeout)
{
    unsigned short cpylen;
    unsigned short framelen;
    unsigned short readlen;
    int32_t ret = 0;
    unsigned int lock;
    if (osal_semp_pend(g_atio_cb.rcvsync,timeout)) {
        lock = LOS_IntLock();
        readlen = sizeof(framelen);
        cpylen = ring_buffer_read(&g_atio_cb.rcvring,(unsigned char *)&framelen,readlen);
        if (cpylen != readlen) {
            ring_buffer_reset(&g_atio_cb.rcvring);  //bad ring format here
            g_atio_cb.rcvringrst++;
        } else {
            if (framelen > len) {
                ring_buffer_reset(&g_atio_cb.rcvring);  //bad ring format here
                g_atio_cb.rcvringrst++;
            } else {
                readlen = framelen;
                cpylen = ring_buffer_read(&g_atio_cb.rcvring,(unsigned char *)buf,readlen);
                if (cpylen != framelen) {
                    ring_buffer_reset(&g_atio_cb.rcvring);  //bad ring format here
                    g_atio_cb.rcvringrst++;
                } else {
                    ret = cpylen;
                }
            }
        }
        LOS_IntRestore(lock);
    }
    return ret;
}

//make it as the at device here
static ssize_t  __at_read  (void *pri,size_t offset,void *buf,size_t len, uint32_t timeout)
{
    return uart_at_receive(buf,len, timeout);

}
static ssize_t  __at_write (void *pri, size_t offset,const void *buf,size_t len,uint32_t timeout)
{
    return uart_at_send(buf, len, timeout);

}

static const los_driv_op_t s_at_op = {

        .init = uart_at_init,
        .deinit = uart_at_deinit,
        .read = __at_read,
        .write = __at_write,
};

OSDRIV_EXPORT(uart_at_driv,CONFIG_UARTAT_DEVNAME,(los_driv_op_t *)&s_at_op,NULL,O_RDWR);