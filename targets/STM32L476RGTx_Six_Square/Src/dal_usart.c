/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2018>, <Huawei Technologies Co., Ltd>
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

/* Includes -----------------------------------------------------------------*/

#include <stdlib.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_usart.h"
#include "los_task.h"
#include "los_queue.h"
#include "dal_usart.h"


/* Defines ------------------------------------------------------------------*/

#define DAL_USART_DEBUG
#ifdef DAL_USART_DEBUG
#define USART_LOG(fmt, arg...) printf("[%s:%d]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define USART_LOG(fmt, arg...)
#endif

#define MAX_USART_NUM 3
#define MAX_USART_RECV_BUF_LEN 1024


/* Typedefs -----------------------------------------------------------------*/

typedef struct
{
    UART_HandleTypeDef usart;
    uint32_t irqn;
    uint8_t *recv_buf;
    uint32_t rp_cb;
    uint32_t rp_bk;
    uint32_t wp;
    dal_usart_recv_callback cb;
    int32_t break_type;
    int32_t break_condition;
} dal_usart_handle;

typedef enum
{
    DAL_USART_RX,
    DAL_USART_QUIT
} usart_data_type;

typedef struct
{
    usart_data_type type;
    uint32_t port;
    uint32_t ori;
    uint32_t end;
} usart_recv_data;


/* Local variables ----------------------------------------------------------*/

static dal_usart_handle g_usart_handle[MAX_USART_NUM];
static uint32_t g_usart_task_id = LOS_ERRNO_TSK_ID_INVALID;
static uint32_t g_usart_qid = LOS_ERRNO_QUEUE_INVALID;
static uint32_t g_active_usart_cnt = 0;
static uint32_t g_sem_handle = 0;


/* Public functions ---------------------------------------------------------*/

static void dal_usart_adapter(uint32_t port)
{
    dal_usart_handle *hdl = &g_usart_handle[port-1];

    switch (port)
    {
    case 1 :
        hdl->usart.Instance = USART1;
        hdl->irqn = USART1_IRQn;
        break;
    case 2 :
        hdl->usart.Instance = USART2;
        hdl->irqn = USART2_IRQn;
        break;
    case 3 :
        hdl->usart.Instance = USART3;
        hdl->irqn = USART3_IRQn;
        break;
    default:
        break;
    }
}

int g_dbg_cnt = 0;
static void dal_usart_irq_handler(dal_usart_handle *arg)
{
    dal_usart_handle *hdl = arg;
    int flag = 0;
    uint8_t c = hdl->recv_buf[hdl->wp];

    g_dbg_cnt++;

    hdl->wp++;
    if (hdl->wp >= MAX_USART_RECV_BUF_LEN)
    {
        hdl->wp = 0;
    }

    if (hdl->break_type == 1)
    {
        uint32_t len = (hdl->wp + MAX_USART_RECV_BUF_LEN - hdl->rp_cb) % MAX_USART_RECV_BUF_LEN;
        if (len >= hdl->break_condition)
        {
            flag = 1;
        }
    }
    else if (hdl->break_type == 1)
    {
        if (c == (uint8_t)hdl->break_condition)
        {
            flag = 1;
        }
    }

    if (flag)
    {
        usart_recv_data data;
        data.type = DAL_USART_RX;
        data.port = hdl - g_usart_handle + 1;
        data.ori = hdl->rp_cb;
        data.end = hdl->wp;
        hdl->rp_cb = hdl->wp;

#if 0
        (void)LOS_SemPost(g_sem_handle);
#else
        if(LOS_QueueWriteCopy(g_usart_qid, &data, sizeof(data), 0) != LOS_OK)
        {
            USART_LOG("LOS_QueueWriteCopy failed!");
        }
#endif
    }

    (void)HAL_UART_Receive_IT(&hdl->usart, &hdl->recv_buf[hdl->wp], 1);

#if 0
    if(__HAL_UART_GET_FLAG(&hdl->usart, UART_FLAG_RXNE) != RESET)
    {
        int flag = 0;
        uint8_t c = (uint8_t)(hdl->usart.Instance->RDR & 0x00FF);
        hdl->recv_buf[hdl->wp++] = c;
        if (hdl->wp >= MAX_USART_RECV_BUF_LEN)
        {
            hdl->wp = 0;
        }

        if (hdl->break_type == 1)
        {
            uint32_t len = (hdl->wp + MAX_USART_RECV_BUF_LEN - hdl->rp_cb) % MAX_USART_RECV_BUF_LEN;
            if (len >= hdl->break_condition)
            {
                flag = 1;
            }
        }
        else if (hdl->break_type == 1)
        {
            if (c == (uint8_t)hdl->break_condition)
            {
                flag = 1;
            }
        }

        if (flag)
        {
            usart_recv_data data;
            data.type = DAL_USART_RX;
            data.port = hdl - g_usart_handle;
            data.ori = hdl->rp_cb;
            data.end = hdl->wp;

            if(LOS_QueueWriteCopy(g_usart_qid, &data, sizeof(data), 0) != LOS_OK)
            {
                USART_LOG("LOS_QueueWriteCopy failed!");
            }
        }
    }
    else if (__HAL_UART_GET_FLAG(&hdl->usart, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&hdl->usart);
    }
#endif
}

int32_t dal_usart_init(dal_usart_config *cfg)
{
    if (NULL == cfg || cfg->port - 1 >= MAX_USART_NUM)
    {
        return -1;
    }

    dal_usart_handle *hdl = &g_usart_handle[cfg->port-1];
    UART_HandleTypeDef *usart = &hdl->usart;

    hdl->recv_buf = (uint8_t *)malloc(MAX_USART_RECV_BUF_LEN);
    if (NULL == hdl->recv_buf)
    {
        USART_LOG("malloc recv_buf failed!");
        return -1;
    }

    dal_usart_adapter(cfg->port);

    usart->Init.BaudRate = cfg->baudrate;
    usart->Init.WordLength = cfg->word_length;
    usart->Init.StopBits = cfg->stop_bits;
    usart->Init.Parity = cfg->parity;
    usart->Init.Mode = cfg->mode;
    usart->Init.HwFlowCtl = cfg->flow_ctrl;
    usart->Init.OverSampling = cfg->over_sampling;
    usart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    usart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if(HAL_UART_Init(usart) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    //LOS_HwiCreate(hdl->irqn, 0, 0, dal_usart_irq_handler, (HWI_ARG_T)hdl);
    //__HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);
    //__HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);
    //HAL_NVIC_SetPriority(hdl->irqn, 0, 0);
    //HAL_NVIC_EnableIRQ(USART1_IRQn);
    g_active_usart_cnt++;

    return 0;
}

int32_t dal_usart_deinit(uint32_t port)
{
    if (port - 1 >= MAX_USART_NUM)
    {
        return -1;
    }

    dal_usart_handle *hdl = &g_usart_handle[port-1];
    UART_HandleTypeDef *usart = &hdl->usart;
    __HAL_UART_DISABLE(usart);
    __HAL_UART_DISABLE_IT(usart, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(usart, UART_IT_RXNE);

    g_active_usart_cnt--;
    if (g_active_usart_cnt == 0 && g_usart_qid != LOS_ERRNO_QUEUE_INVALID)
    {
        usart_recv_data data = {DAL_USART_QUIT, 0};
        data.port = port;

        if(LOS_QueueWriteCopy(g_usart_qid, &data, sizeof(data), 0) != LOS_OK)
        {
            USART_LOG("LOS_QueueWriteCopy failed!");
        }
    }

    if (hdl->recv_buf)
    {
        free(hdl->recv_buf);
        hdl->recv_buf = NULL;
    }

    return 0;
}

int32_t dal_usart_send(uint32_t port, uint8_t *buf, uint32_t len)
{
    if (port - 1 >= MAX_USART_NUM)
    {
        return -1;
    }


    UART_HandleTypeDef *usart = &g_usart_handle[port-1].usart;
    HAL_StatusTypeDef res = HAL_UART_Transmit(usart, buf, len, 0xffff);
    if (res == HAL_OK)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int32_t dal_usart_recv(uint32_t port, uint8_t *buf, uint32_t len, uint32_t timeout)
{
    if (port - 1 >= MAX_USART_NUM)
    {
        return -1;
    }

    dal_usart_handle *hdl = &g_usart_handle[port-1];
    UART_HandleTypeDef *usart = &hdl->usart;
    HAL_StatusTypeDef res = HAL_UART_Receive(usart, buf, len, timeout);
    if (res == HAL_OK)
    {
        hdl->rp_bk += len;
        return 0;
    }
    else
    {
        return -1;
    }
}

static uint32_t copy_usart_recv_data(uint8_t *buf, usart_recv_data *data)
{
    uint32_t len = 0;
    dal_usart_handle *hdl = &g_usart_handle[data->port-1];

    if (data->end == data->ori)
    {
        len = 0;
    }
    else if (data->end > data->ori)
    {
        len = data->end - data->ori;
        memcpy(buf, &hdl->recv_buf[data->ori], len);
    }
    else
    {
        uint32_t tmp_len = MAX_USART_RECV_BUF_LEN - data->ori;
        memcpy(buf, &hdl->recv_buf[data->ori], tmp_len);
        memcpy(buf + tmp_len, hdl->recv_buf, data->end);
        len = data->end + tmp_len;
    }

    hdl->rp_cb = data->end;

    return len;
}

static void dal_usart_recv_task()
{
    uint8_t *buf = (uint8_t *)malloc(MAX_USART_RECV_BUF_LEN);
    if (NULL == buf)
    {
        USART_LOG("malloc tmp buf failed!");
        return;
    }

    while(1)
    {
        usart_recv_data data = {DAL_USART_RX, 0};
        UINT32 dlen = sizeof(data);
#if 0
        LOS_SemPend(g_sem_handle, LOS_WAIT_FOREVER);
#else
        UINT32 ret = LOS_QueueReadCopy(g_usart_qid, &data, &dlen, LOS_WAIT_FOREVER);
        if(ret != LOS_OK)
        {
            continue;
        }
#endif

#if 1
        if (data.type == DAL_USART_QUIT)
        {
            USART_LOG("usart recv task quit");
            break;
        }

        if (data.type != DAL_USART_RX)
        {
            USART_LOG("usart recv data with unknown type");
            continue;
        }

        memset(buf, 0, MAX_USART_RECV_BUF_LEN);
        uint32_t len = copy_usart_recv_data(buf, &data);

        if (len <= 0)
        {
            USART_LOG("usart recv err, len = %d", len);
            continue;
        }

        //USART_LOG("usart recv len = %u, buf = %s", len, buf);

        dal_usart_handle *hdl = &g_usart_handle[data.port-1];
        if (hdl->cb)
        {
            hdl->cb(data.port, buf, len);
        }
#endif
    }

    if (buf)
    {
        free(buf);
        buf = NULL;
    }

    if (LOS_ERRNO_QUEUE_INVALID != g_usart_qid)
    {
        (void)LOS_QueueDelete(g_usart_qid);
        g_usart_qid = LOS_ERRNO_QUEUE_INVALID;
    }
}

static uint32_t dal_usart_create_recv_task(void)
{
    uint32_t ret = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "usart_recv_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)dal_usart_recv_task;
    task_init_param.uwStackSize = 0x2000;

    ret = LOS_TaskCreate((UINT32 *)&g_usart_task_id, &task_init_param);
    if(LOS_OK != ret)
    {
        return ret;
    }

    return ret;
}

static int32_t check_break_condition(int32_t break_type, int32_t break_condition)
{
    switch (break_type)
    {
    case 1:
        if (break_condition <= 0)
        {
            return -1;
        }
        break;
    case 2:
        if (break_condition <= 0 || break_condition > 0xff)
        {
            return -1;
        }
        break;
    default:
        return -1;
    }

    return 0;
}

int32_t dal_set_usart_recv_callback(uint32_t port, dal_usart_recv_callback cb,
                                                int32_t break_type, int32_t break_condition)
{
    uint32_t ret = LOS_OK;

    if (port - 1 >= MAX_USART_NUM || NULL == cb
        || check_break_condition(break_type, break_condition))
    {
        return -1;
    }

    dal_usart_handle *hdl = &g_usart_handle[port-1];
    hdl->cb = cb;
    hdl->break_type = break_type;
    hdl->break_condition = break_condition;

    if (LOS_ERRNO_QUEUE_INVALID == g_usart_qid)
    {
        ret = LOS_QueueCreate("UsartRecvQueue", 16, (UINT32 *)&g_usart_qid, 0, sizeof(usart_recv_data));
        if (ret != LOS_OK)
        {
            USART_LOG("init usart recv queue failed: %d", ret);
            return -1;
        }
    }

    LOS_SemCreate(0, (UINT32 *)&g_sem_handle);

    if (LOS_ERRNO_TSK_ID_INVALID == g_usart_task_id)
    {
        if(LOS_OK != dal_usart_create_recv_task())
        {
            if (LOS_ERRNO_QUEUE_INVALID != g_usart_qid)
            {
                (void)LOS_QueueDelete(g_usart_qid);
                g_usart_qid = LOS_ERRNO_QUEUE_INVALID;
            }

            USART_LOG("create dal_usart_create_recv_task failed!");
            return -1;
        }
    }

    (void)HAL_UART_Receive_IT(&hdl->usart, &hdl->recv_buf[hdl->wp], 1);

    return 0;
}

/**
* @brief This function handles USART1 global interrupt.
*/
int g_dbg_irq = 0;
void USART1_IRQHandler(void)
{
    g_dbg_irq++;
    /* USER CODE BEGIN USART1_IRQn 0 */

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&g_usart_handle[0].usart);
    /* USER CODE BEGIN USART1_IRQn 1 */

    /* USER CODE END USART1_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&g_usart_handle[1].usart);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles USART3 global interrupt.
*/
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&g_usart_handle[2].usart);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief Rx Transfer completed callback.
  * @param huart: UART handle.
  * @retval None
  */
int g_dbg_cb =0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    g_dbg_cb++;
    dal_usart_handle *hdl = (dal_usart_handle *)((char *)huart - LOS_OFF_SET_OF(dal_usart_handle, usart));
    if (hdl >= g_usart_handle && hdl <= &g_usart_handle[MAX_USART_NUM-1])
    {
        dal_usart_irq_handler(hdl);
    }
}

#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    (void)HAL_UART_Transmit(&g_usart_handle[0].usart, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
#endif

uint32_t HAL_GetTick(void)
{
    return (uint32_t)LOS_TickCountGet();
}
