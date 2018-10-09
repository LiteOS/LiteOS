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

#if defined(WITH_AT_FRAMEWORK)

#include "at_hal.h"
#include "usart.h"
#include "stm32f4xx_hal.h"

extern at_task at;
extern at_config at_user_conf;

UART_HandleTypeDef *g_pt_atusart = NULL;
static uint32_t s_uwIRQn = USART2_IRQn;

//uint32_t list_mux;
uint8_t buff_full = 0;

uint32_t wi = 0;
uint32_t wi_bak = 0;
uint32_t ri = 0;





void at_irq_handler(void)
{
    if(__HAL_UART_GET_FLAG(g_pt_atusart, UART_FLAG_RXNE) != RESET)
    {
        at.recv_buf[wi++] = (uint8_t)(g_pt_atusart->Instance->DR & 0x00FF);
        if(wi == ri)buff_full = 1;
        if (wi >= at_user_conf.user_buf_len)wi = 0;
    }
    else if (__HAL_UART_GET_FLAG(g_pt_atusart, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(g_pt_atusart);

        wi_bak = wi;

        (void)LOS_SemPost(at.recv_sem);
    }
}

int32_t at_usart_init(void)
{
    
    if(2 == at_user_conf.usart_port)
    {
        g_pt_atusart = &huart2;
        MX_USART2_UART_Init();
    }
    else if(3 == at_user_conf.usart_port)
    {
        g_pt_atusart = &huart3;
        s_uwIRQn = USART3_IRQn;
        MX_USART3_UART_Init();
    }
    else
    {
        return AT_FAILED;
    }

    __HAL_UART_CLEAR_FLAG(g_pt_atusart, UART_FLAG_TC);
    __HAL_UART_ENABLE_IT(g_pt_atusart, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(g_pt_atusart, UART_IT_RXNE);
    printf("exit at_usart_init,port(%d)\n",at_user_conf.usart_port);
		
    return AT_OK;
}

void at_usart_deinit(void)
{
    HAL_UART_DeInit(g_pt_atusart);
}

void at_transmit(uint8_t *cmd, int32_t len, int flag)
{
    char *line_end = at_user_conf.line_end;
    (void)HAL_UART_Transmit(g_pt_atusart, (uint8_t *)cmd, len, 0xffff);
    if(flag == 1)
    {
        (void)HAL_UART_Transmit(g_pt_atusart, (uint8_t *)line_end, strlen(at_user_conf.line_end), 0xffff);
    }
}

int read_resp(uint8_t *buf, recv_buff* recv_buf)
{
    uint32_t len = 0;

    uint32_t tmp_len = 0;

    if (NULL == buf)
    {
        return -1;
    }
    if(1 == buff_full)
    {
        AT_LOG("buf maybe full,buff_full is %d",buff_full);
    }
    NVIC_DisableIRQ((IRQn_Type)s_uwIRQn);

    //wi = recv_buf->end;//wi_bak
    if (recv_buf->end == recv_buf->ori)
    {
        len = 0;
        goto END;
    }

    if (recv_buf->end > recv_buf->ori)
    {
        len = recv_buf->end - recv_buf->ori;
        memcpy(buf, &at.recv_buf[recv_buf->ori], len);
    }
    else
    {
        tmp_len = at_user_conf.user_buf_len - recv_buf->ori;
        memcpy(buf, &at.recv_buf[recv_buf->ori], tmp_len);
        memcpy(buf + tmp_len, at.recv_buf, recv_buf->end);
        len = recv_buf->end + tmp_len;
    }

    ri = recv_buf->end;
END:
    NVIC_EnableIRQ((IRQn_Type)s_uwIRQn);
    return len;
    }

void write_at_task_msg(at_msg_type_e type)
{
    recv_buff recv_buf;
    int ret;

    memset(&recv_buf,  0, sizeof(recv_buf));
    recv_buf.msg_type = type;

    ret = LOS_QueueWriteCopy(at.rid, &recv_buf, sizeof(recv_buff), 0);
    if(ret != LOS_OK)
    {
        printf("LOS_QueueWriteCopy failed now,ret is %d!!!",ret);
    }
}

#endif
