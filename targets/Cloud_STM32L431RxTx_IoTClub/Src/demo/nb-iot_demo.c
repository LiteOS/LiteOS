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
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)

#include "nb-iot_demo.h"


#ifdef	MODULE_DHT11
DHT11_Data_TypeDef  DHT11_Data;
msg_for_DHT11       DHT11_send;
#endif

#ifdef	MODULE_SMOKE
msg_for_SMOKE       SMOKE_send;
#endif

#ifdef	MODULE_GPS
gps_msg             gpsmsg;
msg_for_GPS         GPS_send;
float Longitude;
float Latitude;
static unsigned char gps_uart[1000];
#endif

#ifdef	MODULE_BH1750
msg_for_BH1750      BH1750_send;
#endif


void data_collection_task(void)
{
	UINT32 uwRet = LOS_OK;
#ifdef	MODULE_DHT11
    DHT11_Init();
#endif
#ifdef	MODULE_SMOKE		
	short int Value;
	short int MaxValue=1000;
	SMOKE_send.index=1;
    MX_ADC1_Init();
#endif
	
#ifdef	MODULE_GPS		
    MX_USART3_UART_Init();
		LOS_HwiCreate(USART3_IRQn, 0,0,USART3_IRQHandler,NULL);
#endif

#ifdef	MODULE_BH1750
	short int Lux;
	Init_BH1750();
#endif
	while (1)
  {
		
#ifdef	MODULE_DHT11
		/****************temperature and humidity*****************/
		if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
		{
			printf("Read DHT11 success!-->Hum is %.1f ％RH ，Temp is %.1f℃ \n",DHT11_Data.humidity,DHT11_Data.temperature);
		}
		else
		{
			printf("Read DHT11 fail!\n");
			DHT11_Init();      
		}
			sprintf(DHT11_send.temp, "%.1f", DHT11_Data.temperature);
			sprintf(DHT11_send.hum, "%.1f", DHT11_Data.humidity);
		// printf("This collection_task\r\n");
#endif

#ifdef	MODULE_SMOKE		
		/****************smoke******************/
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 50);
		Value = HAL_ADC_GetValue(&hadc1);
		printf("\r\n******************************MQ2 Value is  %d\r\n",Value);
		sprintf(SMOKE_send.Value, "%4d", Value);
		sprintf(SMOKE_send.MaxValue, "%4d", MaxValue);
#endif

#ifdef	MODULE_GPS		
		/****************GPS******************/
	  HAL_UART_Receive_IT(&huart3,gps_uart,1000);
		printf("GPS_recive:%s\r\n",gps_uart);
		NMEA_BDS_GPRMC_Analysis(&gpsmsg,(uint8_t*)gps_uart);	//分析字符串
		Longitude=(float)((float)gpsmsg.longitude_bd/100000);	
		printf("Longitude:%.5f %lc     \r\n",Longitude,gpsmsg.ewhemi_bd);
		Latitude=(float)((float)gpsmsg.latitude_bd/100000);
		printf("Latitude:%.5f %1c   \r\n",Latitude,gpsmsg.nshemi_bd);	
  
#endif

#ifdef	MODULE_BH1750
		/****************BH1750******************/
		Lux=(int)Convert_BH1750();
	   	printf("\r\n******************************BH1750 Value is  %d\r\n",Lux);
    	sprintf(BH1750_send.Lux, "%5d", Lux);	
#endif

		uwRet=LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
		return;
	
  }
}



void nb_data_report(void)
{
	UINT32 uwRet = LOS_OK;
     while (1)
    {
    #ifdef	MODULE_DHT11
/*******************temperature and humidity*****************/
			
		club_nbiot_send((char*)(&DHT11_send),sizeof(DHT11_send));		//发送数据到平台	
		
/*******************************END**********************************************/
#endif
			
#ifdef	MODULE_SMOKE	
/***************************smoke*********************************/
			
		club_nbiot_send((char*)(&SMOKE_send),sizeof(SMOKE_send));	//发送数据到平台	

/*******************************END**********************************************/
#endif
			
#ifdef	MODULE_GPS
/***********************************GPS*************************************/
			
		  if(Latitude!=0&&Longitude!=0)
		{
			memset(GPS_send.Latitude, 0, 8);
			memset(GPS_send.Longitude, 0, 9);
			sprintf(GPS_send.Latitude, "%.5f", Latitude);
			sprintf(GPS_send.Longitude, "%.5f", Longitude);		
			club_nbiot_send((char*)(&GPS_send),sizeof(GPS_send));	//发送数据到平台	
		
		}
/*******************************END**********************************************/
#endif
			
#ifdef	MODULE_BH1750
/**********************************BH1750*************************************/
		
			club_nbiot_send(( char*)(&BH1750_send),sizeof(BH1750_send));		//发送数据到平台	
	
/*******************************END**********************************************/
#endif
		
  	uwRet=LOS_TaskDelay(3000);
		if(uwRet != LOS_OK)
		return;
    }
}


void nb_iot_entry(void)
{

	printf("\r\n=====================================================");
	printf("\r\nSTEP1: Init NB Module( NB Init )");
	printf("\r\n=====================================================\r\n");

	club_nbiot_init("218.4.33.71","5683");

	printf("\r\n=====================================================");
	printf("\r\nSTEP3: Report Data to Server( NB Report )");
	printf("\r\n=====================================================\r\n");
	
}
#endif

