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
#include "usart.h"
#include "at_hal.h"
#include "stm32l4xx_hal.h"

extern at_task at;

at_config          *at_hal_conf;


//uint32_t list_mux;
uint8_t buff_full = 0;
static uint32_t g_disscard_cnt = 0;

uint32_t wi = 0;
uint32_t ri = 0;


static void at_usart_adapter(uint32_t port)
{
}

void at_frame_notify_task(void)
{
    recv_buff recv_buf;
    recv_buf.ori = ri;
    recv_buf.end = wi;
    
    recv_buf.msg_type = AT_USART_RX;

    if(LOS_QueueWriteCopy(at.rid, &recv_buf, sizeof(recv_buff), 0) != LOS_OK)
    {
        g_disscard_cnt++;
    }
}

void at_receive_one_byte(UART_HandleTypeDef *huart)
{
    uint16_t uhdata = (uint16_t) READ_REG(huart->Instance->RDR);
    
    at.recv_buf[wi] = (uint8_t)(uhdata & (uint8_t)0x00FFU);
    wi++; 
    if(wi == ri)buff_full = 1;
    if (wi >= at_hal_conf->user_buf_len)wi = 0;
}
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
        
    if(LPUART1 == huart->Instance)
    {
       printf("%c\n",at.recv_buf[wi]);
       wi++; 
       if(wi == ri)buff_full = 1;
       if (wi >= at_hal_conf->user_buf_len)wi = 0;
       
       HAL_UART_Receive_IT(&hlpuart1,&at.recv_buf[wi],1);
    }
}
#endif

int32_t at_usart_init(void)
{

    at_hal_conf = at_get_config();

    at_usart_adapter(at_hal_conf->usart_port);
    __HAL_UART_CLEAR_FLAG(&hlpuart1, UART_FLAG_TC);

    __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_IDLE);
    HAL_UART_Receive_IT(&hlpuart1,&at.recv_buf[wi],1);
    printf("after call at_usart_init \n");
    return AT_OK;
}

void at_usart_deinit(void)
{
    

}

void at_transmit(uint8_t *cmd, int32_t len, int flag)
{
    HAL_StatusTypeDef ret;
    
    char *line_end = at_hal_conf->line_end;
    ret = HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmd, len, 0xffff);
    printf("cmd is %s, ret is %d\n",cmd,ret);
    if(flag == 1)
    {
        ret = HAL_UART_Transmit(&hlpuart1, (uint8_t *)line_end, strlen(at_hal_conf->line_end), 0xffff);
        printf("line_end is %s, ret is %d\n",line_end,ret);
        
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
    

    
    if (recv_buf->end == recv_buf->ori)
    {
        len = 0;
        printf("receive null char for idle int\n");
        return len;
    }

    if (recv_buf->end > recv_buf->ori)
    {
        len = recv_buf->end - recv_buf->ori;
        memcpy(buf, &at.recv_buf[recv_buf->ori], len);
    }
    else
    {
        tmp_len = at_hal_conf->user_buf_len - recv_buf->ori;
        memcpy(buf, &at.recv_buf[recv_buf->ori], tmp_len);
        memcpy(buf + tmp_len, at.recv_buf, recv_buf->end);
        len = recv_buf->end + tmp_len;
    }

    ri = recv_buf->end;

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
        g_disscard_cnt++;
    }
}

#endif
