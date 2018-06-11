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

#include "sys_init.h"

/****** chose a sensor for your application*****/

#define MODULE_DHT11
//#define MODULE_SMOKE
//#define MODULE_GPS
//#define MODULE_BH1750

UINT32 g_TskHandle;

msg_sys_type bc95_net_data;
static char nbiot_uart[500];

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


VOID HardWare_Init(VOID)
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    DelayInit();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    printf("Welcome to IoT-Club, This is EVB-M1 Board.\r\n");
}

void user_hw_init(void)
{
    char *str = "IoT-Club good";
    OLED_Init();
    DHT11_Init();
    OLED_Clear();
    OLED_ShowCHinese(0,0,0);
    OLED_ShowCHinese(18,0,1);
    OLED_ShowCHinese(36,0,2);
    OLED_ShowCHinese(54,0,3);
    OLED_ShowCHinese(72,0,4);
    OLED_ShowCHinese(90,0,5);
    OLED_ShowString(0,2,(uint8_t*)str,16);
    LOS_HwiCreate(EXTI0_IRQn, 2,0,EXTI0_IRQHandler,NULL);
    LOS_HwiCreate(EXTI1_IRQn, 3,0,EXTI1_IRQHandler,NULL);
    LOS_HwiCreate(EXTI2_IRQn, 4,0,EXTI2_IRQHandler,NULL);
    LOS_HwiCreate(EXTI3_IRQn, 5,0,EXTI3_IRQHandler,NULL);
    //LOS_HwiCreate(USART1_IRQn, 6,0,USART1_IRQHandler,NULL);
    LOS_HwiCreate(USART2_IRQn, 6,0,USART2_IRQHandler,NULL);
    LOS_HwiCreate(USART3_IRQn, 7,0,USART3_IRQHandler,NULL);
    los_dev_uart_init(LOS_STM32L431_UART2, 9600, nbiot_uart, 500);
}

VOID data_collection_task(VOID)
{
    UINT32 uwRet = LOS_OK;
#ifdef	MODULE_SMOKE
    short int Value;
    short int MaxValue=1000;
    SMOKE_send.index=1;
#endif

#ifdef	MODULE_GPS

#endif

#ifdef	MODULE_BH1750
    short int Lux;
    Init_BH1750();
#endif
    user_hw_init();
    while (1)
    {
#ifdef	MODULE_DHT11
          /****************temperature and humidity*****************/
        if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
        {
            printf("读取DHT11成功!-->湿度为%.1f ％RH ，温度为 %.1f℃ \n",DHT11_Data.humidity,DHT11_Data.temperature);
        }
        else
        {
            printf("读取DHT11信息失败\n");
            DHT11_Init();
        }
        sprintf(DHT11_send.temp, "%.1f", DHT11_Data.temperature);
        sprintf(DHT11_send.hum, "%.1f", DHT11_Data.humidity);
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

        uwRet=LOS_TaskDelay(1500);
        if(uwRet !=LOS_OK)
        return;

  }
}
UINT32 creat_data_collection_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "data_collection_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)data_collection_task;
    task_init_param.uwStackSize = 0x800;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

VOID data_report_task(VOID)
{
    UINT32 uwRet = LOS_OK;
    neul_bc95_reboot();                                             //初始化模块
    while(neul_bc95_get_netstat()<0);                               //等待连接上网络
    //neul_bc95_set_cdpserver("180.101.147.115");                   //连接电信平台
    neul_bc95_set_cdpserver("218.4.33.71");                         //连接华为平台
    while(1)
    {
        printf("app task test\r\n");
#ifdef	MODULE_DHT11
        /*******************temperature and humidity*****************/
        if(neul_bc95_send_coap_paylaod((const char*)(&DHT11_send),sizeof(DHT11_send))>=0)       //发送数据到平台
            printf("ocean_send_data OK!\n");                                                    //发送成功
        else                                                                                    //发送失败
        {
            printf("ocean_send_data Fail!\n");
        }
        uart_data_flush();
        memset(bc95_net_data.net_nmgr, 0, 5);
        neul_bc95_read_coap_msg(bc95_net_data.net_nmgr,5);
        printf("%s\n",bc95_net_data.net_nmgr);
        if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开灯
        {
        	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);    // 输出高电平
        }
        if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关灯
        {
        	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);  // 输出低电平
        }
#endif

#ifdef	MODULE_SMOKE
        /***************************smoke*********************************/
        SMOKE_send.CSQ=neul_bc95_get_csq();
        if(neul_bc95_send_coap_paylaod((const char*)(&SMOKE_send),sizeof(SMOKE_send))>=0)   //发送数据到平台
            printf("ocean_send_data OK!\n");                                                //发送成功
        else                                                                                //发送失败
        {
            printf("ocean_send_data Fail!\n");
        }
        uart_data_flush();
        memset(bc95_net_data.net_nmgr, 0, 5);
        neul_bc95_read_coap_msg(bc95_net_data.net_nmgr,5);
        printf("%s\n",bc95_net_data.net_nmgr);
        if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开蜂鸣器
        {
            HAL_GPIO_WritePin(Beep_GPIO_Port,Beep_Pin,GPIO_PIN_SET);    // 输出高电平
        }
        if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关蜂鸣器
        {
            HAL_GPIO_WritePin(Beep_GPIO_Port,Beep_Pin,GPIO_PIN_RESET);  // 输出低电平
        }
#endif

#ifdef	MODULE_GPS
        /***********************************GPS*************************************/
        if(Latitude!=0&&Longitude!=0)
        {
            memset(GPS_send.Latitude, 0, 8);
            memset(GPS_send.Longitude, 0, 9);
            sprintf(GPS_send.Latitude, "%.5f", Latitude);
            sprintf(GPS_send.Longitude, "%.5f", Longitude);
            if(neul_bc95_send_coap_paylaod((const char*)(&GPS_send),sizeof(GPS_send))>=0)   //发送数据到平台
                printf("ocean_send_data OK!\n");                                            //发送成功
            else                                                                            //发送失败
            {
                printf("ocean_send_data Fail!\n");
            }
            uart_data_flush();
            memset(bc95_net_data.net_nmgr, 0, 5);
            neul_bc95_read_coap_msg(bc95_net_data.net_nmgr,5);
            printf("%s\n",bc95_net_data.net_nmgr);
            if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开灯
            {
                HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);    // 输出高电平
            }
            if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关灯
            {
                HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);  // 输出低电平
            }
        }

#endif

#ifdef	MODULE_BH1750
        /**********************************BH1750*************************************/
        if(neul_bc95_send_coap_paylaod((const char*)(&BH1750_send),sizeof(BH1750_send))>=0)         //发送数据到平台
            printf("ocean_send_data OK!\n");                                                        //发送成功
        else                                                                                        //发送失败
        {
            printf("ocean_send_data Fail!\n");
        }
        uart_data_flush();
        memset(bc95_net_data.net_nmgr, 0, 5);
        neul_bc95_read_coap_msg(bc95_net_data.net_nmgr,5);
        printf("%s\n",bc95_net_data.net_nmgr);
        if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开灯
        {
            HAL_GPIO_WritePin(Light_GPIO_Port,Light_Pin,GPIO_PIN_RESET);    // 输出低电平
        }
        if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关灯
        {
            HAL_GPIO_WritePin(Light_GPIO_Port,Light_Pin,GPIO_PIN_SET);  // 输出高电平
        }
#endif
	  uwRet=LOS_TaskDelay(500);
		if(uwRet !=LOS_OK)
		return;
	}
}
UINT32 creat_data_report_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "data_report_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)data_report_task;
    task_init_param.uwStackSize = 0x400;
    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

int main(void)
{
    UINT32 uwRet = LOS_OK;

    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    uwRet = creat_data_collection_task();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    uwRet = creat_data_report_task();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    LOS_Start();
}
