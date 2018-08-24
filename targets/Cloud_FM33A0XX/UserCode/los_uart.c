
#include "define_all.h"  
#include "los_uart.h"
#include "los_mux.h"
#include <string.h> 
#include "ring.h"

#define CN_UART_SYNCTIMEOUT 1000
#define CN_AF30XX_COM_NUM  6
void *UARTHandle[CN_AF30XX_COM_NUM]={UART0,UART1,UART2,UART3,UART4,UART5};
typedef struct
{
	void *UARTHandle;
	tagRingBuf  tx;
	tagRingBuf  rx;
	UINT32 rmutex;   //used for the uart read sync
	UINT32 wmutex;   //used for the uart write sync
}tagUARTCB;
tagUARTCB   gUARTCB[CN_AF30XX_COM_NUM];

//GENERAL DEAL HERE
void UART_ISR(int id)
{
	unsigned char tmp08;
	tagUARTCB *cb;
	UARTx_Type *uart;
	if(id >=CN_AF30XX_COM_NUM)
	{
		return;
	}
	cb = &gUARTCB[id];
	uart = cb->UARTHandle;
	if(NULL == uart)
	{
		return;  //NOT YET REGISTERED
	}
	//接收中断
	if((ENABLE == UART_UARTIE_RxTxIE_GetableEx(uart, RxInt))
		&&(SET == UART_UARTIF_RxTxIF_ChkEx(uart, RxInt)))
	{
		tmp08 = UARTx_RXREG_Read(uart);//读寄存器就会清中断
		RingWrite(&cb->rx,(char *)&tmp08,1); //将数据写进缓冲区
	}
	//发送中断
	if((ENABLE == UART_UARTIE_RxTxIE_GetableEx(uart, TxInt))
		&&(SET == UART_UARTIF_RxTxIF_ChkEx(uart, TxInt)))
	{
		if(RingRead(&cb->tx,(char *)&tmp08,1) > 0)
		{
			UARTx_TXREG_Write(uart, tmp08); //取缓冲区数据进行发送
		}
		UART_UARTIF_RxTxIF_ClrEx(uart);	//清中断
	}
}


void UART0_IRQHandler(void)
{
	UART_ISR(0);

}
	
void UART1_IRQHandler(void)
{
	UART_ISR(1);
}

void UART2_IRQHandler(void)
{
	UART_ISR(2);
}

void UART3_IRQHandler(void)
{
	UART_ISR(3);
}

void UART4_IRQHandler(void)
{
	UART_ISR(4);
}

void UART5_IRQHandler(void)
{
	UART_ISR(5);
}


void Uartx_Init(UARTx_Type* UARTx)
{
	UART_SInitTypeDef UART_para;
	RCC_ClocksType RCC_Clocks;
	volatile uint08 tmp08;
	
	RCC_PERCLK_SetableEx(UARTCOMCLK, ENABLE);	//UART0~5?????????
	
	tmp08 = ((uint32_t)UARTx - UART0_BASE)>>5;	//??uart	
	switch(tmp08)
	{
		case 0:
			RCC_PERCLK_SetableEx(UART0CLK, ENABLE);	//UARTx????
			//	UART0 IO ??
			AltFunIO(GPIOF, GPIO_Pin_3, 0);		//PF3 UART0 RX
			AltFunIO(GPIOF, GPIO_Pin_4, 0);		//PF4 UART0 TX
		
			/*NVIC????*/
			NVIC_DisableIRQ(UART0_IRQn);
			NVIC_SetPriority(UART0_IRQn,2);//???????
			NVIC_EnableIRQ(UART0_IRQn);	
			break;
		
		case 1:
			RCC_PERCLK_SetableEx(UART1CLK, ENABLE);	//UARTx????	
			//	UART1 IO ??
			AltFunIO(GPIOB, GPIO_Pin_0, 0);		//PB0 UART1 RX
			AltFunIO(GPIOB, GPIO_Pin_1, 0);		//PB1 UART1 TX
			
			//AltFunIO(GPIOE, GPIO_Pin_3, 0);		//PE3 UART1 RX
			//AltFunIO(GPIOE, GPIO_Pin_4, 0);		//PE4 UART1 TX
		
			/*NVIC????*/
			NVIC_DisableIRQ(UART1_IRQn);
			NVIC_SetPriority(UART1_IRQn,2);//???????
			NVIC_EnableIRQ(UART1_IRQn);	
			break;
		
		case 2:
			RCC_PERCLK_SetableEx(UART2CLK, ENABLE);	//UARTx????	
			//	UART2 IO ??
			AltFunIO(GPIOB, GPIO_Pin_2, 0);		//PB2 UART2 RX
			AltFunIO(GPIOB, GPIO_Pin_3, 0);		//PB3 UART2 TX	
		
			/*NVIC????*/
			NVIC_DisableIRQ(UART2_IRQn);
			NVIC_SetPriority(UART2_IRQn,2);//???????
			NVIC_EnableIRQ(UART2_IRQn);	
			break;
		
		case 3:
			RCC_PERCLK_SetableEx(UART3CLK, ENABLE);	//UARTx????	
			//	UART3 IO ??
			AltFunIO(GPIOC, GPIO_Pin_10, 0);	//PC10 UART3 RX
			AltFunIO(GPIOC, GPIO_Pin_11, 0);	//PC11 UART3 TX
		
			/*NVIC????*/
			NVIC_DisableIRQ(UART3_IRQn);
			NVIC_SetPriority(UART3_IRQn,2);//???????
			NVIC_EnableIRQ(UART3_IRQn);	
			break;
		
		case 4:
			RCC_PERCLK_SetableEx(UART4CLK, ENABLE);	//UARTx????	
			//	UART4 IO ??
			AltFunIO(GPIOD, GPIO_Pin_0, 0);		//PD0 UART4 RX
			AltFunIO(GPIOD, GPIO_Pin_1, 0);		//PD1 UART4 TX
			//?100?
			//AltFunIO(GPIOD, GPIO_Pin_9, 0);		//PD9 UART4 RX
			//AltFunIO(GPIOD, GPIO_Pin_10, 0);	//PD10 UART4 TX
		
			/*NVIC????*/
			NVIC_DisableIRQ(UART4_IRQn);
			NVIC_SetPriority(UART4_IRQn,2);//???????
			NVIC_EnableIRQ(UART4_IRQn);	
			break;
		
		case 5:
			RCC_PERCLK_SetableEx(UART5CLK, ENABLE);	//UARTx????	
			//	UART5 IO ??
			AltFunIO(GPIOC, GPIO_Pin_4, 0);		//PC4 UART5 RX
			AltFunIO(GPIOC, GPIO_Pin_5, 0);		//PC5 UART5 TX	
		
			/*NVIC????*/
			NVIC_DisableIRQ(UART5_IRQn);
			NVIC_SetPriority(UART5_IRQn,2);//???????
			NVIC_EnableIRQ(UART5_IRQn);	
			break;
		
		default:
			break;
	}

	//UART?????
	UART_para.BaudRate = 9600;			//???
	UART_para.DataBit = Eight8Bit;		//????
	UART_para.ParityBit = NONE;			//????
	UART_para.StopBit = OneBit;			//???
	
	RCC_GetClocksFreq(&RCC_Clocks);//??APB????
	UART_SInit(UARTx, &UART_para, RCC_Clocks.APBCLK_Frequency);	//???uart
}


int los_dev_uart_init(int uartidx, int baudrate, char *buf, int size)
{
    int ret = -1;
	UARTx_Type* UARTx;
	UARTx = UARTHandle[uartidx];
	if((uartidx < 0)||(uartidx >= CN_AF30XX_COM_NUM)||(NULL ==buf)||(size == 0))
	{
		return ret;
	}
	Uartx_Init(UARTx);//???uart??
	UARTx_RXSTA_RXEN_Setable(UARTx, ENABLE);		//??????
	UARTx_TXSTA_TXEN_Setable(UARTx, ENABLE);		//??????
	UART_UARTIE_RxTxIE_SetableEx(UARTx, RxInt, ENABLE);//??????

	RingInit(&gUARTCB[uartidx].rx,buf,size,0,0);
	RingInit(&gUARTCB[uartidx].tx,NULL,0,0,0);
	LOS_MuxCreate(&gUARTCB[uartidx].rmutex);
	LOS_MuxCreate(&gUARTCB[uartidx].wmutex);
	gUARTCB[uartidx].UARTHandle = UARTx;
    return ret;
}

int los_dev_uart_read(int uartidx, char *outbuf, int rsize, int mstimeout)
{
  int ret = 0;
	if((uartidx < 0)||(uartidx >= CN_AF30XX_COM_NUM)||(NULL ==outbuf)||(rsize == 0))
	{
		return ret;
	}
	if(LOS_MuxPend(gUARTCB[uartidx].rmutex,CN_UART_SYNCTIMEOUT)==LOS_OK)
	{
		do{
			if(mstimeout > 0)
			{
				LOS_TaskDelay(1);
				mstimeout--;
			}
		}while((mstimeout > 0)&&(RingDataLen(&gUARTCB[uartidx].rx) < rsize));
		//copy the data here
		ret = RingRead(&gUARTCB[uartidx].rx,outbuf,rsize);
		LOS_MuxPost(gUARTCB[uartidx].rmutex);
	}
	if(ret > 0)
	{
		outbuf[rsize] = 0;
		printf("%s",outbuf);
	}
    return ret;
}	

int los_dev_uart_write(int uartidx, char *inbuf, int wsize, int mstimeout)
{
  int ret = 0;
  UARTx_Type* UARTx;
	UARTx = UARTHandle[uartidx];
	if((uartidx < 0)||(uartidx >= CN_AF30XX_COM_NUM)||(NULL ==inbuf)||(wsize == 0))
	{
		return ret;
	}

	if(LOS_MuxPend(gUARTCB[uartidx].wmutex,CN_UART_SYNCTIMEOUT)==LOS_OK)
	{
		RingInit(&gUARTCB[uartidx].tx,inbuf,wsize,1,wsize-1);

		UART_UARTIF_RxTxIF_ClrEx(UARTx);
		UART_UARTIE_RxTxIE_SetableEx(UARTx, TxInt, ENABLE);
		UARTx_TXREG_Write(UARTx,inbuf[0]);
		LOS_TaskDelay(10);
		LOS_MuxPost(gUARTCB[uartidx].wmutex);
	}
    return ret;
}

int los_dev_uart_rflush(int uartidx)
{
	int ret = 0;
	if((uartidx < 0)||(uartidx >= CN_AF30XX_COM_NUM))
	{
		return ret;
	}
	if(LOS_MuxPend(gUARTCB[uartidx].rmutex,CN_UART_SYNCTIMEOUT)==LOS_OK)
	{
		RingReset(&gUARTCB[uartidx].tx);
		LOS_MuxPost(gUARTCB[uartidx].rmutex);
	}

    return ret;
}

int los_dev_uart_wflush(int uartidx)
{
	int ret = 0;
	if((uartidx < 0)||(uartidx >= CN_AF30XX_COM_NUM))
	{
		return ret;
	}
	if(LOS_MuxPend(gUARTCB[uartidx].wmutex,CN_UART_SYNCTIMEOUT)==LOS_OK)
	{
		RingReset(&gUARTCB[uartidx].tx);
		LOS_MuxPost(gUARTCB[uartidx].wmutex);
	}
    return ret;
}



