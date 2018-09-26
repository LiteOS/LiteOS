/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
bool initialized = false;
void Debug_USART1_UART_Init(void)
{
	RETARGET_SerialInit();
}

void RETARGET_SerialInit(void)
{
	#define RETARGET_RX_LOCATION	USART_ROUTELOC0_RXLOC_LOC0
	#define RETARGET_TX_LOCATION	USART_ROUTELOC0_TXLOC_LOC0
  // Enable peripheral clocks 
  CMU_ClockEnable(cmuClock_HFPER, true);
  //Configure GPIO pins
  CMU_ClockEnable(cmuClock_GPIO, true);
  //To avoid false start, configure output as high
  GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInput, 0);

#if defined(RETARGET_USART)
  USART_TypeDef           *usart = RETARGET_UART;
  USART_InitAsync_TypeDef init   = USART_INITASYNC_DEFAULT;

  //Enable DK RS232/UART switch
  RETARGET_PERIPHERAL_ENABLE();

  CMU_ClockEnable(RETARGET_CLK, true);

  //Configure USART for basic async operation
  init.enable = usartDisable;
  USART_InitAsync(usart, &init);

  //Enable pins at correct UART/USART location.
  #if defined( USART_ROUTEPEN_RXPEN )
  usart->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  usart->ROUTELOC0 = ( usart->ROUTELOC0 &
                       ~( _USART_ROUTELOC0_TXLOC_MASK
                          | _USART_ROUTELOC0_RXLOC_MASK ) )
                     | ( RETARGET_TX_LOCATION << _USART_ROUTELOC0_TXLOC_SHIFT )
                     | ( RETARGET_RX_LOCATION << _USART_ROUTELOC0_RXLOC_SHIFT );
  #else
  usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | RETARGET_LOCATION;
  #endif

  //Clear previous RX interrupts
  USART_IntClear(RETARGET_UART, USART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(RETARGET_IRQn);

  //Enable RX interrupts 
  USART_IntEnable(RETARGET_UART, USART_IF_RXDATAV);
  NVIC_EnableIRQ(RETARGET_IRQn);

  //Finally enable it
  USART_Enable(usart, usartEnable);

#else
  LEUART_TypeDef      *leuart = RETARGET_UART;
  LEUART_Init_TypeDef init    = LEUART_INIT_DEFAULT;

  // Enable DK LEUART/RS232 switch
  RETARGET_PERIPHERAL_ENABLE();

  //Enable CORE LE clock in order to access LE modules
  CMU_ClockEnable(cmuClock_CORELE, true);

#if defined(RETARGET_VCOM)
  // Select HFXO/2 for LEUARTs (and wait for it to stabilize)
#if defined(_CMU_LFCLKSEL_LFB_HFCORECLKLEDIV2)
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
#else
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_HFCLKLE);
#endif
#else
  //Select LFXO for LEUARTs (and wait for it to stabilize)
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
#endif

  CMU_ClockEnable(RETARGET_CLK, true);

  //Do not prescale clock
  CMU_ClockDivSet(RETARGET_CLK, cmuClkDiv_1);

  //Configure LEUART
  init.enable = leuartDisable;
#if defined(RETARGET_VCOM)
  init.baudrate = 115200;
#endif
  LEUART_Init(leuart, &init);
  //Enable pins at default location 
  #if defined( LEUART_ROUTEPEN_RXPEN )
  leuart->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  leuart->ROUTELOC0 = ( leuart->ROUTELOC0 &
                       ~( _LEUART_ROUTELOC0_TXLOC_MASK
                          | _LEUART_ROUTELOC0_RXLOC_MASK ) )
                     | ( RETARGET_TX_LOCATION << _LEUART_ROUTELOC0_TXLOC_SHIFT )
                     | ( RETARGET_RX_LOCATION << _LEUART_ROUTELOC0_RXLOC_SHIFT );
  #else
  leuart->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | RETARGET_LOCATION;
  #endif

  //Clear previous RX interrupts
  LEUART_IntClear(RETARGET_UART, LEUART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(RETARGET_IRQn);

  //Enable RX interrupts
  LEUART_IntEnable(RETARGET_UART, LEUART_IF_RXDATAV);
  NVIC_EnableIRQ(RETARGET_IRQn);

  //Finally enable it
  LEUART_Enable(leuart, leuartEnable);
#endif

#if !defined(__CROSSWORKS_ARM) && defined(__GNUC__)
  setvbuf(stdout, NULL, _IONBF, 0);   //Set unbuffered mode for stdout (newlib)
#endif

  initialized = true;
}






/* define fputc */
//#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  //KEIL and IAR: printf will call fputc to print 
//int fputc(int ch, FILE *f)
///{
    //(void)HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
//    return ch;
//}

#if USE_PPPOS

#include "osport.h"
#include "los_hwi.h"

int uart3_send(unsigned char *buf,int len);
int uart3_recv(unsigned char *buf,int len,int timeout);
tagRingBuf  gRcvRing;
#define CN_RING_lEN 1500
unsigned char gRcvBuf[CN_RING_lEN];
UART_HandleTypeDef huart3;


void uart3_irq()
{
    unsigned char data;
    unsigned int flags;
    
    flags = huart3.Instance->SR;
    if(flags & USART_SR_RXNE)
    {
        data = (uint8_t)(huart3.Instance->DR & (uint8_t)0x00FF);
        ring_write(&gRcvRing,&data,1);
    }   
}
void uart3_init(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    ring_init(&gRcvRing,gRcvBuf,CN_RING_lEN,0,0);
    
    
    LOS_HwiCreate(39,4,0,uart3_irq,0);
        /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(huart3.Instance->CR3, USART_CR3_EIE);

    /* Enable the UART Parity Error and Data Register not empty Interrupts */
    SET_BIT(huart3.Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);
}



int uart3_send(unsigned char *buf,int len)
{
    int ret = 0;
    
    if(HAL_OK == HAL_UART_Transmit(&huart3, buf, len, 0xFFFF))
    {
        ret= len;
    }
    return ret;
}
int uart3_recv(unsigned char *buf,int len,int timeout)
{
    int datalen = 0;
    int ret = 0;
    unsigned char value;
    do{
        datalen = ring_datalen(&gRcvRing);
        task_sleepms(1);
    }while((timeout-- >0)&&(datalen < len));
    if(datalen > 0)
    {
        ret = ring_read(&gRcvRing,buf,len);
    }
    return ret;
}

void uart_init(void)
{
    uart3_init();
}
int uart_write(char *buf,int len,int timeout)
{
    return uart3_send((unsigned char *)buf,len);
}
int uart_read(char *buf,int len,int timeout)
{
    return uart3_recv((unsigned char *)buf,len,timeout);
}

#endif


//#elif defined ( __GNUC__ )  /* GCC: printf will call _write to print */
//__attribute__((used)) int _write(int fd, char *ptr, int len)
//{
    //(void)HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
 //   return len;
//}
//#endif

/* USER CODE BEGIN 1 */



