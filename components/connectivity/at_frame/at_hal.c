#if defined(WITH_AT_FRAMEWORK)

#include "atadapter.h"

extern at_task at;
extern at_config at_user_conf;

UART_HandleTypeDef at_usart;
#ifdef USE_USARTRX_DMA

DMA_HandleTypeDef  at_hdma_usart3_rx;
#endif

//uint32_t list_mux;

uint32_t wi = 0;
uint32_t wi_bak= 0;
uint32_t ri = 0;

#ifdef USE_USARTRX_DMA
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
        at.recv_buf[wi++] = (uint8_t)(at_usart.Instance->DR & 0x00FF);
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
        if(empty > at_user_conf.user_buf_len)
        {
        //error
        }
        else
        {
            wi = (at_user_conf.user_buf_len - empty);
        }
#endif   
        wi_bak = wi;
        LOS_SemPost(at.recv_sem);
    }
}

void at_usart_config(void)
{
	UART_HandleTypeDef * usart = &at_usart;
    usart->Instance = at_user_conf.usart;
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
    LOS_HwiCreate(at_user_conf.irqn, 0, 0, at_irq_handler, 0);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);

#ifdef USE_USARTRX_DMA
    at_usart3rx_dma_config(&at_hdma_usart3_rx);
#else
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);
#endif
}



void at_transmit(uint8_t * cmd, int32_t len,int flag)
{
    char * line_end = at_user_conf.line_end;
    (void)HAL_UART_Transmit(&at_usart, (uint8_t*)cmd, len, 0xffff);
	if(flag == 1)
    	(void)HAL_UART_Transmit(&at_usart, (uint8_t*)line_end, strlen(at_user_conf.line_end), 0xffff);

}

int read_resp(uint8_t * buf)
{
    uint32_t len = 0;
    uint32_t wi = wi_bak;
    uint32_t tmp_len = 0;
    if (NULL == buf){
        return -1;
    }

    if (wi == ri){
        return 0;
    }

    if (wi > ri){
        len = wi - ri;
        memcpy(buf, &at.recv_buf[ri], len);
    } 
    else 
    {
#ifndef USE_USARTRX_DMA
        tmp_len = at_user_conf.user_buf_len - ri;
        memcpy(buf, &at.recv_buf[ri], tmp_len);
        memcpy(buf + tmp_len, at.recv_buf, wi);
        len = wi + tmp_len;
#endif
    }
  
#ifndef USE_USARTRX_DMA
    ri = wi;
#else
    ri = 0;
    HAL_UART_Receive_DMA(&at_usart,&at.recv_buf[0],at_user_conf.user_buf_len-1);
#endif
    return len;
}
#endif
