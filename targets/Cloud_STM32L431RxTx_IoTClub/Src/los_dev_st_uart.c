#include "los_dev_st_uart.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOS_DEV_UART_NUM 4
los_dev_uart_t comx[LOS_DEV_UART_NUM];

/*
    STM32L431RBT6 uart2
*/
USART_TypeDef *puarts[] = {
    USART1,USART2,USART3
	  
    
};

static HAL_StatusTypeDef los_dev_uart_start(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    uint32_t tmp_state = 0;
  
    tmp_state = huart->gState;
    if((tmp_state == HAL_UART_STATE_READY) || (tmp_state == HAL_UART_STATE_BUSY_TX))
    {
        if((pData == NULL ) || (Size == 0))
        {
            return HAL_ERROR;
        }

        /* Process Locked */
        __HAL_LOCK(huart);

        huart->pRxBuffPtr = pData;
        huart->RxXferSize = Size;
        huart->RxXferCount = Size;

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        /* Check if a transmit process is ongoing or not */
        if(huart->gState == HAL_UART_STATE_BUSY_TX)
        {
            huart->gState = HAL_UART_STATE_BUSY_TX_RX;
        }
        else
        {
            huart->gState = HAL_UART_STATE_BUSY_RX;
        }

        /* Process Unlocked */
        __HAL_UNLOCK(huart);

        /* Enable the UART Parity Error Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_PE);

        /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

        /* Enable the UART Data Register not empty Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
    
        /* Enable the UART Data Register IDEL Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

static void los_dev_uart_gpio_init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

}

int los_dev_uart_init(int uartidx, int baudrate, char *buf, int size)
{
    if (uartidx > LOS_DEV_UART_NUM || uartidx <= 0)
    {
        return -1;
    }
    if (NULL == buf || size <= 0)
    {
        return -1;
    }
    if (baudrate != 9600 && baudrate != 115200)
    {
        return -1;
    }
    
    los_dev_uart_gpio_init();
    
    /* init read write mutex and lock it */
    LOS_SemCreate(0,&comx[uartidx -1].rsem);
    LOS_SemCreate(1,&comx[uartidx -1].wsem);
//    LOS_SemPend(comx[uartidx -1].rsem, LOS_WAIT_FOREVER);
//    LOS_SemPend(comx[uartidx -1].wsem, LOS_WAIT_FOREVER);
    
    comx[uartidx -1].dev.Instance = puarts[uartidx -1];//USART2;
    comx[uartidx -1].dev.Init.BaudRate = 9600;
    comx[uartidx -1].dev.Init.WordLength = UART_WORDLENGTH_8B;
    comx[uartidx -1].dev.Init.StopBits = UART_STOPBITS_1;
    comx[uartidx -1].dev.Init.Parity = UART_PARITY_NONE;
    comx[uartidx -1].dev.Init.Mode = UART_MODE_TX_RX;
    comx[uartidx -1].dev.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    comx[uartidx -1].dev.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&(comx[uartidx -1].dev)) != HAL_OK)
    {
        while(1);
    }
    comx[uartidx -1].pbuf = buf;
    comx[uartidx -1].dualbuf = buf + size / 2;
    comx[uartidx -1].interrbuf = NULL;
    comx[uartidx -1].size = size;
    comx[uartidx -1].readcnt = 0;
    // use buf as a dual buf, so need size/2
    if(HAL_BUSY == los_dev_uart_start(&(comx[uartidx -1].dev), (uint8_t *)buf, size / 2))
    {
        return -2;
    }
    
    return 0;
}

static HAL_StatusTypeDef los_receive_it( int uartidx)
{
    uint16_t* tmp;
    uint32_t tmp_state = 0;
    UART_HandleTypeDef *huart;
    
    huart = &(comx[uartidx -1].dev);
    
    tmp_state = huart->gState; 
    if((tmp_state == HAL_UART_STATE_BUSY_RX) || (tmp_state == HAL_UART_STATE_BUSY_TX_RX))
    {
        if(huart->Init.WordLength == UART_WORDLENGTH_9B)
        {
            tmp = (uint16_t*) huart->pRxBuffPtr;
            if(huart->Init.Parity == UART_PARITY_NONE)
            {
                *tmp = (uint16_t)(huart->Instance->RDR & (uint16_t)0x01FF);
                huart->pRxBuffPtr += 2;
            }
            else
            {
                *tmp = (uint16_t)(huart->Instance->RDR & (uint16_t)0x00FF);
                huart->pRxBuffPtr += 1;
            }
        }
        else
        {
            if(huart->Init.Parity == UART_PARITY_NONE)
            {
                *huart->pRxBuffPtr++ = (uint8_t)(huart->Instance->RDR & (uint8_t)0x00FF);
            }
            else
            {
                *huart->pRxBuffPtr++ = (uint8_t)(huart->Instance->RDR & (uint8_t)0x007F);
            }
        }

        if(--huart->RxXferCount == 0)
        {
            if (huart->pRxBuffPtr > (uint8_t *)comx[uartidx -1].dualbuf)
            {
                huart->pRxBuffPtr = (uint8_t *)comx[uartidx -1].pbuf;
                comx[uartidx -1].interrbuf = comx[uartidx -1].dualbuf;
            }
            else
            {
                huart->pRxBuffPtr = (uint8_t *)comx[uartidx -1].dualbuf;
                comx[uartidx -1].interrbuf = comx[uartidx -1].pbuf;
            }
            huart->RxXferCount = huart->RxXferSize;
            
            comx[uartidx -1].readcnt = huart->RxXferSize;
            LOS_SemPost(comx[uartidx -1].rsem);
            
            return HAL_OK;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY; 
    }
}

extern HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart);
extern HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart);
void los_dev_uart_irqhandler(int uartidx)
{
    UART_HandleTypeDef *huart;
    uint32_t tmp_flag = 0, tmp_it_source = 0;
    
    huart = &(comx[uartidx -1].dev);
    
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);  
    /* UART IDEL interrupt occurred ------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    { 
        // recive data end, means no data from remote 
        //todo post a signal to wake up uart read process
        if (huart->pRxBuffPtr > (uint8_t *)comx[uartidx -1].dualbuf)
        {
            huart->pRxBuffPtr = (uint8_t *)comx[uartidx -1].pbuf;
            comx[uartidx -1].interrbuf = comx[uartidx -1].dualbuf;
        }
        else
        {
            huart->pRxBuffPtr = (uint8_t *)comx[uartidx -1].dualbuf;
            comx[uartidx -1].interrbuf = comx[uartidx -1].pbuf;
        }
        comx[uartidx -1].readcnt = huart->RxXferSize - huart->RxXferCount;
        huart->RxXferCount = huart->RxXferSize;
        LOS_SemPost(comx[uartidx -1].rsem);
        __HAL_UART_CLEAR_IDLEFLAG(huart);
    }
    
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE);  
    /* UART parity error interrupt occurred ------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    { 
        huart->ErrorCode |= HAL_UART_ERROR_PE;
    }
  
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
    /* UART frame error interrupt occurred -------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    { 
        huart->ErrorCode |= HAL_UART_ERROR_FE;
    }
  
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
    /* UART noise error interrupt occurred -------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    { 
        huart->ErrorCode |= HAL_UART_ERROR_NE;
    }
  
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
    /* UART Over-Run interrupt occurred ----------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    { 
        huart->ErrorCode |= HAL_UART_ERROR_ORE;
    }
  
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
    /* UART in mode Receiver ---------------------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    { 
        los_receive_it(uartidx);
    }
  
    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);
    /* UART in mode Transmitter ------------------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        UART_Transmit_IT(huart);
    }

    tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_TC);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC);
    /* UART in mode Transmitter end --------------------------------------------*/
    if((tmp_flag != RESET) && (tmp_it_source != RESET))
    {
        UART_EndTransmit_IT(huart);
    }  

    if(huart->ErrorCode != HAL_UART_ERROR_NONE) 
    {
        #if 0
        /* some error happened, we discard all data, but let uart still work */
        if (huart->pRxBuffPtr > (uint8_t *)comx[uartidx -1].dualbuf)
        {
            huart->pRxBuffPtr = (uint8_t *)comx[uartidx -1].pbuf;
            comx[uartidx -1].interrbuf = comx[uartidx -1].dualbuf;
        }
        else
        {
            huart->pRxBuffPtr = (uint8_t *)comx[uartidx -1].dualbuf;
            comx[uartidx -1].interrbuf = comx[uartidx -1].pbuf;
        }
        huart->RxXferCount = huart->RxXferSize;
        comx[uartidx -1].readcnt = 0;
        #else
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        __HAL_UART_CLEAR_OREFLAG(huart);
        #endif
        //los_receive_it(uartidx);
    }  
}


int los_dev_uart_read(int uartidx, char *outbuf, int rsize, int mstimout)
{
    unsigned long tick_start;
    unsigned long tick_cur;
    UINT32 uwRet;
    int tmpread = rsize;
    int tmptimout = mstimout;
    char *pouttmp = outbuf;
    
    if (uartidx > LOS_DEV_UART_NUM || uartidx <= 0)
    {
        return -1;
    }
    if (NULL == outbuf || rsize <= 0)
    {
        return -1;
    }
    
    // no nonblock read uart buf
    if (0 == mstimout)
    {
        if (comx[uartidx -1].readcnt == 0)
        {
            return 0;
        }
        else
        {
            if (rsize > comx[uartidx -1].readcnt)
            {
                memcpy((void *)outbuf, (const void *)comx[uartidx -1].interrbuf, (unsigned int)comx[uartidx -1].readcnt);
                comx[uartidx -1].readcnt = 0;
                return comx[uartidx -1].readcnt;
            }
            else
            {
                memcpy(outbuf, comx[uartidx -1].interrbuf, rsize);
                comx[uartidx -1].interrbuf = comx[uartidx -1].interrbuf + rsize;
                comx[uartidx -1].readcnt = comx[uartidx -1].readcnt - rsize;
                return rsize;
            }
        }
    }
    
    tick_start = (UINT32)LOS_TickCountGet();
    
    while(1)
    {
        if (comx[uartidx -1].readcnt == 0)
        {
            // no data buffered, need wait for data
            uwRet = LOS_SemPend(comx[uartidx -1].rsem, tmptimout);
            if(uwRet == LOS_OK)
            {
                if (tmpread <= comx[uartidx -1].readcnt)
                {
                    // read enough, directly return
                    memcpy(pouttmp, comx[uartidx -1].interrbuf, tmpread);
                    memset(comx[uartidx -1].interrbuf, 0, tmpread);
                    comx[uartidx -1].interrbuf = comx[uartidx -1].interrbuf + tmpread;
                    comx[uartidx -1].readcnt = comx[uartidx -1].readcnt - tmpread;
                    return rsize;
                }
                else
                {
                    // not enough data , need continue read
                    memcpy(pouttmp, comx[uartidx -1].interrbuf, comx[uartidx -1].readcnt);
                    memset(comx[uartidx -1].interrbuf, 0, comx[uartidx -1].readcnt);
                    tmpread = tmpread - comx[uartidx -1].readcnt;
                    pouttmp = pouttmp + comx[uartidx -1].readcnt;
                    comx[uartidx -1].readcnt = 0;
                }
                
                tick_cur = (UINT32)LOS_TickCountGet();
                if (tick_cur - tick_start < tmptimout)
                {
                    tmptimout = tmptimout - (tick_cur - tick_start);
                    tick_start = (UINT32)LOS_TickCountGet();
                    continue;
                }
                else
                {
                    break ;
                }
            }
            else if(uwRet == LOS_ERRNO_SEM_TIMEOUT )
            {
                // no data come, nee return ;
                return rsize - tmpread;
            }
        }
        else
        {
            // there are some data in the buffer, can directly read out
            if (tmpread > comx[uartidx -1].readcnt)
            {
                memcpy(pouttmp, comx[uartidx -1].interrbuf, comx[uartidx -1].readcnt);
                memset(comx[uartidx -1].interrbuf, 0, comx[uartidx -1].readcnt);
                tmpread = tmpread - comx[uartidx -1].readcnt;
                pouttmp = pouttmp + comx[uartidx -1].readcnt;
                comx[uartidx -1].readcnt = 0;
                // need wait data come
                continue;
            }
            else
            {
                // there are enough data ,so directly read out
                memcpy(pouttmp, comx[uartidx -1].interrbuf, tmpread);
                memset(comx[uartidx -1].interrbuf, 0, tmpread);
                comx[uartidx -1].interrbuf = comx[uartidx -1].interrbuf + tmpread;
                comx[uartidx -1].readcnt = comx[uartidx -1].readcnt - tmpread;
                return rsize;
            }
        }  
    }
    return 0;
}

int los_dev_uart_write(int uartidx, char *inbuf, int wsize, int mstimout)
{
    UINT32 uwRet;
    if (uartidx > LOS_DEV_UART_NUM || uartidx <= 0)
    {
        return -1;
    }
    if (NULL == inbuf || wsize <= 0)
    {
        return -1;
    }
    uwRet = LOS_SemPend(comx[uartidx -1].wsem, mstimout);
    if(uwRet == LOS_OK)
    {
        HAL_UART_Transmit_IT(&comx[uartidx -1].dev, (uint8_t*)inbuf, wsize);
        //HAL_UART_Transmit(&comx[uartidx -1].dev, (uint8_t*)inbuf, wsize, 1000);
        LOS_SemPost(comx[uartidx -1].wsem);
        return wsize;
    }
    else if(uwRet == LOS_ERRNO_SEM_TIMEOUT )
    {
        return 0;
    }
    return 0;
}

