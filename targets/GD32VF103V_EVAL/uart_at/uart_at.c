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

//#include "usart.h"
#include "gd32vf103.h"
#include "gd32v103v_eval.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <osal.h>
#include <link_misc.h>
#include <driver.h>
#include "sys/fcntl.h"
#include "los_hwi.h"

uint32_t uart_at;
static uint32_t           s_pUSART = EVAL_COM1;
static uint32_t           s_uwIRQn = USART1_IRQn;

#define CN_RCVBUF_LEN  256  //cache a frame
#define CN_RCVMEM_LEN  512  //use to cache more frames

struct atio_cb
{
    unsigned short        w_next;    //the next position to be write
    osal_semp_t           rcvsync;   //if a frame has been written to the ring, then active it
    tag_ring_buffer_t     rcvring;
    unsigned char         rcvbuf[CN_RCVBUF_LEN];
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
    if(usart_flag_get(uart_at, USART_FLAG_RBNE) != RESET)
    {
       value = (uint8_t)(usart_data_receive(uart_at) & 0x00FF);
       g_atio_cb.rcvlen++;
       if(g_atio_cb.w_next < CN_RCVBUF_LEN)
       {
           g_atio_cb.rcvbuf[g_atio_cb.w_next] = value;
           g_atio_cb.w_next++;
       }
       else
       {
            g_atio_cb.rframeover++;
       }
    }
    else if (usart_flag_get(uart_at,USART_FLAG_IDLEF) != RESET)
    {
        //__HAL_UART_CLEAR_IDLEFLAG(&uart_at);
        //TODO clear IDLE flag
        usart_flag_get(uart_at,USART_FLAG_IDLEF);
        usart_data_receive(uart_at);
        ringspace = CN_RCVMEM_LEN - ring_datalen(&g_atio_cb.rcvring);
        if(ringspace < g_atio_cb.w_next)  //not enough mem
        {
            g_atio_cb.rframedrop++;
        }
        else
        {
            //write data to the ring buffer:len+data format
            ringspace = g_atio_cb.w_next;
            ring_write(&g_atio_cb.rcvring,(unsigned char *)&ringspace,sizeof(ringspace));
            ring_write(&g_atio_cb.rcvring,g_atio_cb.rcvbuf,ringspace);
            osal_semp_post(g_atio_cb.rcvsync);
            g_atio_cb.rcvframe++;
        }
        g_atio_cb.w_next=0; //write from the head
    }
}

void USART1_IRQHandler(void)
{
    atio_irq();
}

/*******************************************************************************
function     :use this function to initialize the uart
parameters   :
instruction  :
*******************************************************************************/
bool_t uart_at_init(int baud)
{
    //initialize the at controller
    memset(&g_atio_cb,0,sizeof(g_atio_cb));
    if(false == osal_semp_create(&g_atio_cb.rcvsync,CN_RCVMEM_LEN,0))
    {
        printf("%s:semp create error\n\r",__FUNCTION__);
        goto EXIT_SEMP;
    }
    ring_init(&g_atio_cb.rcvring,g_atio_cb.rcvringmem,CN_RCVMEM_LEN,0,0);

    uart_at = s_pUSART;
    gd_eval_com_init(uart_at);
    {
        //_Error_Handler(__FILE__, __LINE__);
    }
    usart_flag_clear(uart_at, USART_FLAG_TC);
    //LOS_HwiCreate(s_uwIRQn, 3, 0, atio_irq, 0);
    usart_interrupt_enable(uart_at,USART_INT_IDLE);
    usart_interrupt_enable(uart_at,USART_INT_RBNE);
    return true;

EXIT_SEMP:
    return false;
}

void uartat_deinit(void)
{
    usart_deinit(uart_at);
}

/*******************************************************************************
function     :use this function to send a frame to the uart
parameters   :
instruction  :
*******************************************************************************/
static ssize_t uart_at_send(const char *buf, size_t len,uint32_t timeout)
{
    int i = 0;

    while(i < len)
    {
        printf("0x%x ", buf[i]);
        usart_data_transmit(uart_at,buf[i]);
        i++;

        while (usart_flag_get(uart_at, USART_FLAG_TBE)== RESET){}

        /* wait for completion of USART transmission */
        while(RESET == usart_flag_get(uart_at, USART_FLAG_TC)){}
    }

    g_atio_cb.sndlen += len;
    g_atio_cb.sndframe ++;

    return len;
}
/*******************************************************************************
function     :use this function to read a frame from the uart
parameters   :
instruction  :
*******************************************************************************/
static ssize_t uart_at_receive(void *buf,size_t len, uint32_t timeout)
{
    unsigned short cpylen;
    unsigned short framelen;
    unsigned short readlen;
    int32_t ret = 0;
    unsigned int lock;
    if(osal_semp_pend(g_atio_cb.rcvsync,timeout))
    {
        lock = LOS_IntLock();
        readlen = sizeof(framelen);
        cpylen = ring_read(&g_atio_cb.rcvring,(unsigned char *)&framelen,readlen);
        if(cpylen != readlen)
        {
            ring_reset(&g_atio_cb.rcvring);  //bad ring format here
            g_atio_cb.rcvringrst++;
        }
        else
        {
            if(framelen > len)
            {
                ring_reset(&g_atio_cb.rcvring);  //bad ring format here
                g_atio_cb.rcvringrst++;
            }
            else
            {
                readlen = framelen;
                cpylen = ring_read(&g_atio_cb.rcvring,(unsigned char *)buf,readlen);
                if(cpylen != framelen)
                {
                    ring_reset(&g_atio_cb.rcvring);  //bad ring format here
                    g_atio_cb.rcvringrst++;
                }
                else
                {
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

    .read = __at_read,
    .write = __at_write,
};

OSDRIV_EXPORT(uart_at_driv,"atdev_EC20CEFAG",(los_driv_op_t *)&s_at_op,NULL,O_RDWR);
