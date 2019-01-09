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
#include "los_sem.h"
#include "uds/uds.h"
#include "uds/uds_usart.h"
#include "dal_usart.h"


/* Defines ------------------------------------------------------------------*/

#define DAL_USART_DEBUG
#ifdef DAL_USART_DEBUG
#define USART_LOG(fmt, arg...) printf("[%s:%d]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define USART_LOG(fmt, arg...)
#endif

#define MAX_USART_NUM 3
#define MAX_USART_RECV_BUF_LEN 2048


/* Typedefs -----------------------------------------------------------------*/

typedef struct
{
    UART_HandleTypeDef usart;
    uint32_t irqn;
    uint8_t *recv_buf;
    uint32_t rp;
    uint32_t wp;
    uds_usart_recv_callback cb;
    int32_t break_type;
    int32_t break_cond;
} dal_usart_handle;

typedef enum
{
    DAL_USART_RX,
    DAL_USART_EXIT
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
//static uint32_t g_usart_qid = LOS_ERRNO_QUEUE_INVALID;
static uint32_t g_sem_handle = LOS_ERRNO_SEM_INVALID;
static uint32_t g_active_usart_cnt = 0;
static uint32_t g_recv_task_exit = 0;


/* Public functions ---------------------------------------------------------*/

static void usart_port_adapter(uint32_t port)
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

static uint32_t usart_copy_recv_data(uint8_t *buf, usart_recv_data *data)
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

    hdl->rp = data->end;

    return len;
}

static void usart_report_data(dal_usart_handle *hdl, uint8_t *buf)
{
    uint32_t rp = hdl->rp;
    uint32_t wp = hdl->wp;
    uint32_t rp_cur = 0;
    uint32_t tlen = (wp + MAX_USART_RECV_BUF_LEN - rp) % MAX_USART_RECV_BUF_LEN;
    usart_recv_data data = {DAL_USART_RX};

    data.port = hdl - g_usart_handle + 1;

    if (hdl->break_type == 1)
    {
        while (tlen >= hdl->break_cond)
        {
            rp_cur = (rp + hdl->break_cond) % MAX_USART_RECV_BUF_LEN;

            data.ori = rp;
            data.end = rp_cur;
            hdl->rp = rp_cur;
            rp = rp_cur;
            tlen -= hdl->break_cond;

            uint32_t len = usart_copy_recv_data(buf, &data);
            hdl->cb(data.port, buf, len);
        }
    }
    else if (hdl->break_type == 2)
    {
        rp_cur = rp;

        while ((int32_t)tlen > 0 && rp_cur != wp)
        {
            uint8_t c = hdl->recv_buf[rp_cur++];
            rp_cur %= MAX_USART_RECV_BUF_LEN;

            if (c == (uint8_t)hdl->break_cond)
            {
                data.ori = rp;
                data.end = rp_cur;
                hdl->rp = rp_cur;
                rp = rp_cur;

                uint32_t len = usart_copy_recv_data(buf, &data);
                hdl->cb(data.port, buf, len);
                tlen -= len;
            }
        }
    }
}

/*
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
        uint32_t len = (hdl->wp + MAX_USART_RECV_BUF_LEN - hdl->rp) % MAX_USART_RECV_BUF_LEN;
        if (len >= hdl->break_cond)
        {
            flag = 1;
        }
    }
    else if (hdl->break_type == 1)
    {
        if (c == (uint8_t)hdl->break_cond)
        {
            flag = 1;
        }
    }

    if (flag)
    {
        usart_recv_data data;
        data.type = DAL_USART_RX;
        data.port = hdl - g_usart_handle + 1;
        data.ori = hdl->rp;
        data.end = hdl->wp;
        hdl->rp = hdl->wp;

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
            uint32_t len = (hdl->wp + MAX_USART_RECV_BUF_LEN - hdl->rp) % MAX_USART_RECV_BUF_LEN;
            if (len >= hdl->break_cond)
            {
                flag = 1;
            }
        }
        else if (hdl->break_type == 1)
        {
            if (c == (uint8_t)hdl->break_cond)
            {
                flag = 1;
            }
        }

        if (flag)
        {
            usart_recv_data data;
            data.type = DAL_USART_RX;
            data.port = hdl - g_usart_handle;
            data.ori = hdl->rp;
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
*/

int32_t dal_usart_init(uds_usart_cfg *cfg)
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

    usart_port_adapter(cfg->port);

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
    if (g_active_usart_cnt == 0)
    {
        g_recv_task_exit = 1;

#if 0
        if (g_usart_qid != LOS_ERRNO_QUEUE_INVALID)
        {
            usart_recv_data data = {DAL_USART_EXIT, 0};
            data.port = port;

            if(LOS_QueueWriteCopy(g_usart_qid, &data, sizeof(data), 0) != LOS_OK)
            {
                USART_LOG("LOS_QueueWriteCopy failed!");
            }
        }
#endif
    }

    if (hdl->recv_buf)
    {
        free(hdl->recv_buf);
        hdl->recv_buf = NULL;
    }

    return 0;
}

int32_t dal_usart_send(uint32_t port, uint8_t *buf, uint32_t len, uint32_t timeout)
{
    if (port - 1 >= MAX_USART_NUM || NULL == buf || 0 == len)
    {
        return -1;
    }

    UART_HandleTypeDef *usart = &g_usart_handle[port-1].usart;
    (void)HAL_UART_Transmit(usart, buf, len, timeout);
    return len - usart->TxXferCount;
}

int32_t dal_usart_recv(uint32_t port, uint8_t *buf, uint32_t len, uint32_t timeout)
{
    if (port - 1 >= MAX_USART_NUM || NULL == buf || 0 == len)
    {
        return -1;
    }

    UART_HandleTypeDef *usart = &g_usart_handle[port-1].usart;
    (void)HAL_UART_Receive(usart, buf, len, timeout);
    return len - usart->RxXferCount;
}

static void usart_recv_task()
{
    uint8_t *buf = (uint8_t *)malloc(MAX_USART_RECV_BUF_LEN);
    if (NULL == buf)
    {
        USART_LOG("malloc tmp buf failed!");
        return;
    }

    while(1)
    {
        //usart_recv_data data = {DAL_USART_RX, 0};
        //UINT32 dlen = sizeof(data);
#if 1
        //LOS_SemPend(g_sem_handle, LOS_WAIT_FOREVER);
        LOS_SemPend(g_sem_handle, 100);

        if (g_recv_task_exit)
        {
            USART_LOG("usart recv task exit");
            break;
        }

        for (int i = 0; i < MAX_USART_NUM; i++)
        {
            dal_usart_handle *hdl = &g_usart_handle[i];
            if (hdl->cb && hdl->recv_buf)
            {
                usart_report_data(hdl, buf);
            }
        }

#else
        UINT32 ret = LOS_QueueReadCopy(g_usart_qid, &data, &dlen, LOS_WAIT_FOREVER);
        if(ret != LOS_OK)
        {
            continue;
        }
#endif

#if 0
        if (data.type == DAL_USART_EXIT)
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
        uint32_t len = usart_copy_recv_data(buf, &data);

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

    if (LOS_ERRNO_SEM_INVALID != g_sem_handle)
    {
        (void)LOS_SemDelete(g_sem_handle);
        g_sem_handle = LOS_ERRNO_SEM_INVALID;
    }

#if 0
    if (LOS_ERRNO_QUEUE_INVALID != g_usart_qid)
    {
        (void)LOS_QueueDelete(g_usart_qid);
        g_usart_qid = LOS_ERRNO_QUEUE_INVALID;
    }
#endif
}

static uint32_t usart_create_recv_task(void)
{
    uint32_t ret = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "usart_recv_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)usart_recv_task;
    task_init_param.uwStackSize = 0x2000;

    ret = LOS_TaskCreate((UINT32 *)&g_usart_task_id, &task_init_param);
    if(LOS_OK != ret)
    {
        return ret;
    }

    return ret;
}

static int32_t check_break_condition(int32_t break_type, int32_t break_cond)
{
    switch (break_type)
    {
    case 1:
        if (break_cond <= 0)
        {
            return -1;
        }
        break;
    case 2:
        if (break_cond <= 0 || break_cond > 0xff)
        {
            return -1;
        }
        break;
    default:
        return -1;
    }

    return 0;
}

int32_t dal_set_usart_recv_callback(uint32_t port, uds_usart_recv_callback cb,
                                                int32_t break_type, int32_t break_cond)
{
    uint32_t ret = LOS_OK;

    if (port - 1 >= MAX_USART_NUM || NULL == cb
        || check_break_condition(break_type, break_cond))
    {
        return -1;
    }

    dal_usart_handle *hdl = &g_usart_handle[port-1];
    hdl->cb = cb;
    hdl->break_type = break_type;
    hdl->break_cond = break_cond;

    if (LOS_ERRNO_SEM_INVALID == g_sem_handle)
    {
        ret = LOS_SemCreate(0, (UINT32 *)&g_sem_handle);
        if (ret != LOS_OK)
        {
            USART_LOG("init usart recv semaphore failed: %d", ret);
            return -1;
        }
    }

#if 0
    if (LOS_ERRNO_QUEUE_INVALID == g_usart_qid)
    {
        ret = LOS_QueueCreate("UsartRecvQueue", 16, (UINT32 *)&g_usart_qid, 0, sizeof(usart_recv_data));
        if (ret != LOS_OK)
        {
            USART_LOG("init usart recv queue failed: %d", ret);
            return -1;
        }
    }
#endif

    if (LOS_ERRNO_TSK_ID_INVALID == g_usart_task_id)
    {
        if(LOS_OK != usart_create_recv_task())
        {
            if (LOS_ERRNO_SEM_INVALID != g_sem_handle)
            {
                (void)LOS_SemDelete(g_sem_handle);
                g_sem_handle = LOS_ERRNO_SEM_INVALID;
            }

#if 0
            if (LOS_ERRNO_QUEUE_INVALID != g_usart_qid)
            {
                (void)LOS_QueueDelete(g_usart_qid);
                g_usart_qid = LOS_ERRNO_QUEUE_INVALID;
            }
#endif

            USART_LOG("create usart_create_recv_task failed!");
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

    dal_usart_handle *hdl = (dal_usart_handle *)huart;

    if (hdl >= g_usart_handle && hdl <= &g_usart_handle[MAX_USART_NUM-1])
    {
        hdl->wp++;
        if (hdl->wp >= MAX_USART_RECV_BUF_LEN)
        {
            hdl->wp = 0;
        }

        (void)HAL_UART_Receive_IT(huart, &hdl->recv_buf[hdl->wp], 1);
        //(void)LOS_SemPost(g_sem_handle);
    }

#if 0
    dal_usart_handle *hdl = (dal_usart_handle *)((char *)huart - LOS_OFF_SET_OF(dal_usart_handle, usart));
    if (hdl >= g_usart_handle && hdl <= &g_usart_handle[MAX_USART_NUM-1])
    {
        dal_usart_irq_handler(hdl);
    }
#endif
}

uint32_t HAL_GetTick(void)
{
    return (uint32_t)LOS_TickCountGet();
}

#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    (void)HAL_UART_Transmit(&g_usart_handle[0].usart, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
#endif

static s32_t uds_usart_open(void *pri, s32_t flag)
{
    return 0;
}

static s32_t uds_usart_read(void *pri, u32_t offset, u8_t *buf, s32_t len, u32_t timeout)
{
    dal_usart_handle *hdl = (dal_usart_handle *)pri;
    uint32_t port = hdl - g_usart_handle + 1;

    return dal_usart_recv(port, buf, len, timeout);
}

static s32_t uds_usart_write(void *pri, u32_t offset, u8_t *buf, s32_t len, u32_t timeout)
{
    dal_usart_handle *hdl = (dal_usart_handle *)pri;
    uint32_t port = hdl - g_usart_handle + 1;

    return dal_usart_send(port, buf, len, timeout);
}

static void uds_usart_close(void *pri)
{
}

static s32_t uds_usart_ioctl(void *pri, u32_t cmd, void *para, s32_t len)
{
    dal_usart_handle *hdl = (dal_usart_handle *)pri;
    uint32_t port = hdl - g_usart_handle + 1;
    uds_usart_cb_cfg *cb_cfg = NULL;
    int32_t ret = 0;

    switch (cmd)
    {
    case USART_SET_RECV_CALLBACK:
        if (NULL == para || len != sizeof(uds_usart_cb_cfg))
        {
            return -1;
        }

        cb_cfg = (uds_usart_cb_cfg *)para;
        ret = dal_set_usart_recv_callback(port, cb_cfg->cb, cb_cfg->break_type, cb_cfg->break_cond);
        break;

    case USART_CLR_RECV_CALLBACK:
        hdl->cb = NULL;
        hdl->break_type = 0;
        hdl->break_cond = 0;
        break;

    default:
        return -1;
    }

    return ret;
}

static s32_t uds_usart_init(void *pri)
{
    return 0;
}

static void uds_usart_deinit(void *pri)
{
#if 0
    dal_usart_handle *hdl = (dal_usart_handle *)pri;
    uint32_t port = hdl - g_usart_handle + 1;

    (void)dal_usart_deinit(port);
#endif
}


static uds_driv_op_t uds_opt =
{
    .open     = uds_usart_open,
    .read     = uds_usart_read,
    .write    = uds_usart_write,
    .close    = uds_usart_close,
    .ioctl    = uds_usart_ioctl,
    .init     = uds_usart_init,
    .deinit   = uds_usart_deinit,
};

s32_t uds_usart_dev_install(const char *name, void *para)
{
    uds_usart_cfg *cfg = (uds_usart_cfg *)para;
    int32_t ret = 0;

    if (NULL == name || NULL == para)
    {
        return -1;
    }

    ret = dal_usart_init(cfg);
    if (ret)
    {
        return ret;
    }

    if(NULL == uds_driv_register(name, &uds_opt, &g_usart_handle[cfg->port-1], O_EXCL))
    {
        return -1;
    }

    return 0;
}

static void demo_recv_callback(uint32_t port, uint8_t *buf, uint32_t len)
{
    dal_usart_send(port, buf, len, 0xffff);
    printf("\n");
}

void demo_usart(void)
{
    int ret = 0;
    uds_usart_cfg cfg =
    {
        .port = 1,
        .baudrate = 115200,
        .word_length = UART_WORDLENGTH_8B,
        .stop_bits = UART_STOPBITS_1,
        .parity = UART_PARITY_NONE,
        .mode = UART_MODE_TX_RX,
        .flow_ctrl = UART_HWCONTROL_NONE,
        .over_sampling = UART_OVERSAMPLING_16,
    };

    uds_driv_t dev;
    uds_driv_init();
    uds_usart_dev_install("USART1", (void *)&cfg);

    dev = uds_dev_open("USART1", O_RDWR);
    if(NULL == dev)
    {
        printf("uds_dev_open fail\n");
    }

    unsigned char buf[] = "hello world";
    ret = uds_dev_write(dev, 0, buf, sizeof(buf)-1, 0xffff);
    printf("\nret = %d, wait for input\n", ret);
    //ret = uds_dev_read(dev, 0, buf, 5, 5000);
    //printf("ret=%d, buf=%s\n", ret, buf);
    uds_usart_cb_cfg cb_cfg =
    {
        .cb = demo_recv_callback,
        .break_type = 1,//2,
        .break_cond = 5,//'0',
    };
    uds_dev_ioctl(dev, USART_SET_RECV_CALLBACK, (void *)&cb_cfg, sizeof(cb_cfg));
}

