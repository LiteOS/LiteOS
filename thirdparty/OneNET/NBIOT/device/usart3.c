#include "cmsis_os.h"    
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "M5310.h"
#include "usart3.h"
 
#include "os-api.h"
#include "los_bsp_led.h"
 

static char us3_rxBuffer[US3_RX_BUFSIZE];
static volatile int  us3_rxCnt = 0;
static char us3_cmdBuffer[US3_RX_BUFSIZE];
static sem_t *us3_cmdSem;


osPoolDef(us3_pool_name, US3_POOL_NUM, us3_pool_t);  /* This must be global */
osMessageQDef(us3_queue_name, US3_POOL_NUM, 0);
osMessageQDef(us3_txQueue_name, 512, 0);


osPoolId us3_pool;
osMessageQId us3_rxQueue;
osMessageQId us3_txQueue;

void us3_hard_init(int ulWantedBaud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* If the queue/semaphore was created correctly then setup the serial port
	hardware. */
	/* Enable USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Configure USART3 Rx (PB11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	/* Configure USART3 Tx (PB10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	USART_InitStructure.USART_BaudRate = ulWantedBaud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init( USART3, &USART_InitStructure );

	USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );
	USART_ITConfig( USART3, USART_IT_IDLE, ENABLE );

#if 0
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
#else
void USART3_IRQHandler( void );
	LOS_HwiCreate(USART3_IRQn, 0,0,USART3_IRQHandler,NULL); //suzhen
#endif

	USART_Cmd( USART3, ENABLE );
}

void USART3_Init(void)
{	
	us3_pool = osPoolCreate(osPool(us3_pool_name));
	OS_ASSERT(us3_pool);
	
	us3_rxQueue = osMessageCreate(osMessageQ(us3_queue_name), 0);
	OS_ASSERT(us3_rxQueue);
	
	us3_rxCnt = 0;
	memset(us3_rxBuffer, 0, sizeof(us3_rxBuffer));

	us3_txQueue = osMessageCreate(osMessageQ(us3_txQueue_name), 0);
	OS_ASSERT(us3_txQueue);

	us3_cmdSem = NULL;
	
	us3_hard_init(9600);
}

void USART3_IRQHandler( void )
{
	char  cChar;

  if( USART_GetITStatus( USART3, USART_IT_TXE ) != RESET )
  {
		osEvent e;
		e = osMessageGet(us3_txQueue, 0);
		
    if( e.status == osEventMessage ) {
			cChar = (char) (e.value.v);
      /* A character was retrieved from the queue so can be sent to the
      THR now. */
      USART_SendData( USART3, cChar );
    } else {
			
      USART_ITConfig( USART3, USART_IT_TXE, DISABLE );
			
			if (osEventTimeout != e.status ) {
				PRINT_ERR("%s - %d osMessageGet failed!\n", __func__, __LINE__);
			}

    }
  }

  if( USART_GetITStatus( USART3, USART_IT_RXNE ) != RESET )
  {
    cChar = USART_ReceiveData( USART3 );
		
		if (us3_rxCnt < sizeof(us3_rxBuffer))
			us3_rxBuffer[us3_rxCnt++] = cChar;
  }
	
	/* got one data frame */
	if( USART_GetITStatus( USART3, USART_IT_IDLE ) != RESET )
  {
		char tmp;
		tmp = USART3->SR;
		tmp = USART3->DR;
		(void)tmp;
		if (strstr(us3_rxBuffer, "5683") != NULL) {
			us3_pool_t *pPool = (us3_pool_t *) osPoolAlloc(us3_pool);
			if (pPool) {
				memcpy(pPool->buf, us3_rxBuffer, us3_rxCnt);
				if (osMessagePut(us3_rxQueue, (uint32_t)pPool, 0) != LOS_OK) {
					PRINT_ERR("%s - %d osMessagePut failed!\n", __func__, __LINE__);
				}
			} else {
				PRINT_ERR("%s - %d osPoolAlloc failed!\n", __func__, __LINE__);
			}
			
		} else if (strstr(us3_rxBuffer, "+CEREG:0") != NULL) {
			;
		} else if (strstr(us3_rxBuffer, "+CEREG:1") != NULL) {
			;
		} else if (strstr(us3_rxBuffer, "+CSCON") != NULL) {
			;
		} else {
			memcpy(us3_cmdBuffer, us3_rxBuffer, us3_rxCnt);
			if (us3_cmdSem)
				sem_post(us3_cmdSem);
		}
		
		us3_rxCnt = 0;
		memset(us3_rxBuffer, 0, sizeof(us3_rxBuffer));
	}
}

int us3_write(const char *buffer, int size)
{
	char ch;
  int sendLen = 0;
	
  while(1) {
		int rc;
		ch = *(buffer + sendLen);
		if (ch == 0) {
			PRINT_ERR("%s - %d us3_write can't be 0 !\n", __func__, __LINE__);
		}
		
		rc = osMessagePut(us3_txQueue, ch, OSA_WAIT_FOREVER);
		if (LOS_OK != rc) {
			PRINT_ERR("%s - %d osMessagePut failed!\n", __func__, __LINE__);
		}
		
		if ( ++sendLen >= size )
			break;
  }

  USART_ITConfig( USART3, USART_IT_TXE, ENABLE );

  return sendLen;
}

int at_send_cmd(char *cmd, int len, char *want, int delay, int retry)
{
	int err = 0;
	int ret = 0;
	
	sem_t sem;
	sem_init(&sem, 0, 0);
	us3_cmdSem = &sem;
	
	
	while (1) {
		
		memset(us3_cmdBuffer, 0, sizeof(us3_cmdBuffer));
		us3_write(cmd, len);
		PRINT_INFO("AT SEND: %s\n", cmd);

		if (sem_timedwait(&sem, 1000) == 0) {
			PRINT_INFO("AT GOT : %s\n", us3_cmdBuffer);
			if (strstr(us3_cmdBuffer, want) != NULL) {
				ret = 0;
				break;
			}
		}
		
		if (err++ > retry) {
			ret = -1;
			break;
		}
		
		osDelay(delay);
	}
	
	us3_cmdSem = NULL;
  sem_destroy(&sem);

	return ret;
}


void SendCmd(char* cmd, char *result, uint16_t timeout, uint16_t retry)
{
	at_send_cmd(cmd, strlen(cmd), result, timeout, retry);
}

void SendData(char* cmd, char *result, uint16_t len, uint16_t timeout, uint16_t retry)
{
	int ret = at_send_cmd(cmd, len, result, timeout, retry);
	if (ret < 0) {
		PRINT_ERR("SendData Failed: %s", cmd);
	}
}


void netif_rx(uint8_t *buf, uint16_t *read)
{
	osEvent e;
	uint8_t *pdata;
	char len[4], i=0;
	int length = 0;
	
	*read = 0;
	
	e = osMessageGet(us3_rxQueue, 0);  /* non-block */
	
	if( e.status == osEventMessage )
	{
		us3_pool_t *msg = (us3_pool_t *) e.value.v;
		if ((pdata = (uint8_t*)strstr((const char *)msg->buf,  "5683")) != NULL)
		{
			pdata += 5;
			while (*pdata != ',') {
				len[i] = *pdata;
				pdata++;
				i++;
			}
			len[i] = '\0';
			length = utils_atoi(len, strlen(len));
			hexstring_to_hex(buf, pdata+1, length);
			*read = length;	
		} else {
			PRINT_ERR("%s - %d netif_rx invalid packet!\n", __func__, __LINE__);
		}
		
		osPoolFree(us3_pool, msg);
		
	} else if (osEventTimeout != e.status ) {
		PRINT_ERR("%s - %d osMessageGet failed!\n", __func__, __LINE__);
	}
}

