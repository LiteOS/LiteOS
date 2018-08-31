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
#include "stm32l4xx_hal.h"

extern at_task at;
extern at_config at_user_conf;

UART_HandleTypeDef at_usart;

static USART_TypeDef* s_pUSART ;
static uint32_t s_uwIRQn ;

//uint32_t list_mux;

#ifndef USE_USARTRX_DMA
uint32_t wi = 0;
uint32_t wi_bak= 0;
uint32_t ri = 0;
#else
/*DMA操作*/
DMA_HandleTypeDef  at_hdma_usart3_rx;
uint8_t dma_wbi = 0;
uint8_t dma_rbi = 0;
uint16_t *dma_wi = NULL;
uint8_t dma_wi_coun;
#endif

static void at_usart_adapter(uint32_t port)
{
    switch ( port )
    {
    case 1 :
        s_pUSART = USART1;
        s_uwIRQn = USART1_IRQn;
        break;
    case 2 :
        s_pUSART = USART2;
        s_uwIRQn = USART2_IRQn;
        break;
    case 3 :
        s_pUSART = USART3;
        s_uwIRQn = USART3_IRQn;
        break;
    default:
        break;
    }
}

#ifdef USE_USARTRX_DMA
int32_t at_dmawi_init(void)
{
    dma_wi_coun = at_user_conf.recv_buf_len/at_user_conf.user_buf_len;
    dma_wi = at_malloc(dma_wi_coun * sizeof(*dma_wi));
    if (NULL == dma_wi)
    {
        AT_LOG("malloc dma_wi failed!");
        return AT_FAILED;
    }

    return AT_OK;
}

void at_usart3rx_dma_irqhandler(void)
{
    HAL_DMA_IRQHandler(&at_hdma_usart3_rx);
}

void at_usart3rx_dma_config(DMA_HandleTypeDef *hdma)
{
    printf("Now Init DMA1\n");
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma->Instance = DMA1_Stream1;
    hdma->Init.Channel = DMA_CHANNEL_4;
    hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma->Init.MemInc = DMA_MINC_ENABLE;
    hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;

    hdma->Init.Mode = DMA_NORMAL;
    hdma->Init.Priority = DMA_PRIORITY_HIGH;//DMA_PRIORITY_LOW;
    hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma->Init.MemBurst = DMA_MBURST_SINGLE;
    hdma->Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_Init(hdma);

    __HAL_LINKDMA(&at_usart,hdmarx,*hdma);
        
    LOS_HwiCreate(DMA1_Stream1_IRQn, 0, 0,at_usart3rx_dma_irqhandler, NULL);
}
#endif


void at_irq_handler(void)
{
#ifndef USE_USARTRX_DMA
    if(__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_RXNE) != RESET)
    {
        at.recv_buf[wi++] = (uint8_t)(at_usart.Instance->RDR & 0x00FF);
        if (wi >= at_user_conf.user_buf_len)wi = 0;
    }
    else 
#endif
    if (__HAL_UART_GET_FLAG(&at_usart,UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&at_usart);
#ifdef USE_USARTRX_DMA
        HAL_UART_DMAStop(&at_usart);  
        uint32_t empty = __HAL_DMA_GET_COUNTER(&at_hdma_usart3_rx);
        dma_wi[dma_wbi++] = at_user_conf.user_buf_len - empty;
        if(dma_wbi >= dma_wi_coun)
            dma_wbi = 0;       	   
#else
        wi_bak = wi;
#endif   
        LOS_SemPost(at.recv_sem);
        
#ifdef USE_USARTRX_DMA		
        HAL_UART_Receive_DMA(&at_usart,&at.recv_buf[at_user_conf.user_buf_len*dma_wbi],at_user_conf.user_buf_len);
#endif   
    }
}

int32_t at_usart_init(void)
{
	UART_HandleTypeDef * usart = &at_usart;

    at_usart_adapter(at_user_conf.usart_port);
    
    usart->Instance = s_pUSART;
    usart->Init.BaudRate = at_user_conf.buardrate;

    usart->Init.WordLength = UART_WORDLENGTH_8B;
    usart->Init.StopBits = UART_STOPBITS_1;
    usart->Init.Parity = UART_PARITY_NONE;
    usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart->Init.Mode = UART_MODE_RX | UART_MODE_TX;
    if(HAL_UART_Init(usart) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    __HAL_UART_CLEAR_FLAG(usart,UART_FLAG_TC);
    LOS_HwiCreate(s_uwIRQn, 0, 0, at_irq_handler, 0);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);

#ifdef USE_USARTRX_DMA
    if( AT_OK != at_dmawi_init)
    {
        return AT_FAILED;
    }
    HAL_UART_Receive_DMA(&at_usart,&at.recv_buf[at_user_conf.user_buf_len*0],at_user_conf.user_buf_len);
    at_usart3rx_dma_config(&at_hdma_usart3_rx);
#else
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);
#endif
    return AT_OK;
}

void at_usart_deinit(void)
{
    UART_HandleTypeDef * husart = &at_usart;
    __HAL_UART_DISABLE(husart);
    __HAL_UART_DISABLE_IT(husart, UART_IT_IDLE);
    
#ifdef USE_USARTRX_DMA
    __HAL_RCC_DMA1_CLK_DISABLE();
    if(NULL != dma_wi)
    {
        at_free(dma_wi);
        dma_wi = NULL;
    }
#else
    __HAL_UART_DISABLE_IT(husart, UART_IT_RXNE);
#endif
}

void at_transmit(uint8_t * cmd, int32_t len, int flag)
{
    char * line_end = at_user_conf.line_end;
    (void)HAL_UART_Transmit(&at_usart, (uint8_t*)cmd, len, 0xffff);
	if(flag == 1){
    	(void)HAL_UART_Transmit(&at_usart, (uint8_t*)line_end, strlen(at_user_conf.line_end), 0xffff);
	}
}

int read_resp(uint8_t * buf)
{
    uint32_t len = 0;
#ifndef USE_USARTRX_DMA
    uint32_t wi = wi_bak;
    uint32_t tmp_len = 0;
#endif
    if (NULL == buf){
        return -1;
    }
#ifndef USE_USARTRX_DMA

    if (wi == ri){
        return 0;
    }

    if (wi > ri){
        len = wi - ri;
        memcpy(buf, &at.recv_buf[ri], len);
    } 
    else 
    {
        tmp_len = at_user_conf.user_buf_len - ri;
        memcpy(buf, &at.recv_buf[ri], tmp_len);
        memcpy(buf + tmp_len, at.recv_buf, wi);
        len = wi + tmp_len;
    }
#else
    if (dma_wbi == dma_rbi){
        return 0;
	}
    memcpy(buf, &at.recv_buf[dma_rbi*at_user_conf.user_buf_len], dma_wi[dma_rbi]);     
    len = dma_wi[dma_rbi];
    dma_rbi++;
    if(dma_rbi >= dma_wi_coun){
        dma_rbi = 0;
    }
#endif  
#ifndef USE_USARTRX_DMA
    ri = wi;
#endif
    return len;
}
#endif
