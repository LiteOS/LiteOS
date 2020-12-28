/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: At Hal Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "at_hal.h"
#include "usart.h"
#include "main.h"

UART_HandleTypeDef at_usart;

static USART_TypeDef *g_atUSART = USART2;
static uint32_t g_atIRQn = USART2_IRQn;

uint8_t buff_full = 0;
static uint32_t g_disscard_cnt = 0;

uint32_t wi = 0;
uint32_t pre_ri = 0; /* only save cur msg start */
uint32_t ri = 0;

static void at_usart_adapter(uint32_t port)
{
#ifdef LOSCFG_PLATFORM_STM32L431_BearPi
    g_atUSART = LPUART1;
    g_atIRQn = LPUART1_IRQn;
    return;
#endif
    switch (port) {
        case 1: // 1: usart1
            g_atUSART = USART1;
            g_atIRQn = USART1_IRQn;
            break;
        case 2: // 2: usart2
            g_atUSART = USART2;
            g_atIRQn = USART2_IRQn;
            break;
        case 3: // 3: usart3
            g_atUSART = USART3;
#ifdef STM32F072xB
        case 4:
            g_atIRQn = USART3_4_IRQn;
#else
            g_atIRQn = USART3_IRQn;
#endif
            break;
        default:
            break;
    }
}

void at_irq_handler(void)
{
    uint8_t ch;
    recv_buff recv_buf;
    at_config *at_user_conf = at_get_config();

    if (__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_RXNE) != RESET) {
        HAL_UART_Receive(&at_usart, &ch, 1, 0);
        at.recv_buf[wi++] = ch;
        if (wi == ri) {
            buff_full = 1;
        }
        if (wi >= at_user_conf->user_buf_len) {
            wi = 0;
        }
    } else if (__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&at_usart);
        /*
        Ring Buffer ri------------------------>wi

         __________________________________________________
         |      msg0           |  msg1        |   msg2    |
         ri(pre_ri0)        pre_ri1         pre_ri2     wi(pre_ri3)
         __________________________________________________

         read_resp ---->ri= pre_ri1----------->---------->ri=wi=pre_ri3(end)
        */
        recv_buf.ori = pre_ri;
        recv_buf.end = wi;

        pre_ri = recv_buf.end;
        recv_buf.msg_type = AT_USART_RX;

        if (LOS_QueueWriteCopy(at.rid, &recv_buf, sizeof(recv_buff), 0) != LOS_OK) {
            g_disscard_cnt++;
        }
    }
}

int32_t at_usart_init(void)
{
    UART_HandleTypeDef *usart = &at_usart;
    at_config *at_user_conf = at_get_config();

    at_usart_adapter(at_user_conf->usart_port);

    usart->Instance = g_atUSART;
    usart->Init.BaudRate = at_user_conf->buardrate;

    usart->Init.WordLength = UART_WORDLENGTH_8B;
    usart->Init.StopBits = UART_STOPBITS_1;
    usart->Init.Parity = UART_PARITY_NONE;
    usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart->Init.Mode = UART_MODE_RX | UART_MODE_TX;
    if (HAL_UART_Init(usart) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_NVIC_EnableIRQ(g_atIRQn);
    __HAL_UART_CLEAR_FLAG(usart, UART_FLAG_TC);
    LOS_HwiCreate(g_atIRQn + 16, 0, 0, at_irq_handler, 0);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);

    return AT_OK;
}

void at_usart_deinit(void)
{
    UART_HandleTypeDef *husart = &at_usart;
    __HAL_UART_DISABLE(husart);
    __HAL_UART_DISABLE_IT(husart, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(husart, UART_IT_RXNE);
}

void at_transmit(uint8_t *cmd, int32_t len, int flag)
{
    at_config *at_user_conf = at_get_config();

    char *line_end = at_user_conf->line_end;
    (void)HAL_UART_Transmit(&at_usart, (uint8_t *)cmd, len, 0xffff);
    if (flag == 1) {
        (void)HAL_UART_Transmit(&at_usart, (uint8_t *)line_end, strlen(at_user_conf->line_end), 0xffff);
    }
}

int read_resp(uint8_t *buf, recv_buff *recv_buf)
{
    uint32_t len = 0;
    uint32_t tmp_len;

    at_config *at_user_conf = at_get_config();

    if (buf == NULL) {
        return -1;
    }

    if (buff_full == 1) {
        AT_LOG("buf maybe full,buff_full is %d", buff_full);
    }

    if (recv_buf->end == recv_buf->ori) {
        len = 0;
        goto END;
    }

    if (recv_buf->end > recv_buf->ori) {
        len = recv_buf->end - recv_buf->ori;
        (void)memcpy_s(buf, len, &at.recv_buf[recv_buf->ori], len);
    } else {
        tmp_len = at_user_conf->user_buf_len - recv_buf->ori;
        (void)memcpy_s(buf, tmp_len, &at.recv_buf[recv_buf->ori], tmp_len);
        (void)memcpy_s(buf + tmp_len, recv_buf->end, at.recv_buf, recv_buf->end);
        len = recv_buf->end + tmp_len;
    }

    ri = recv_buf->end;

END:
    return len;
}

void write_at_task_msg(at_msg_type_e type)
{
    recv_buff recv_buf;
    int ret;

    (void)memset_s(&recv_buf, sizeof(recv_buf), 0, sizeof(recv_buf));
    recv_buf.msg_type = type;

    ret = LOS_QueueWriteCopy(at.rid, &recv_buf, sizeof(recv_buff), 0);
    if (ret != LOS_OK) {
        g_disscard_cnt++;
    }
}