#include "main.h"
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



VOID data_collection_task(VOID)
{
    UINT32 uwRet = LOS_OK;
    while (1)
    {
#ifdef	MODULE_DHT11
          /****************temperature and humidity*****************/
        if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
        {
            printf("??DHT11??!-->???%.1f %RH ,??? %.1f? \n",DHT11_Data.humidity,DHT11_Data.temperature);
        }
        else
        {
            printf("??DHT11????\n");
            DHT11_Init();
        }
        sprintf(DHT11_send.temp, "%.1f", DHT11_Data.temperature);
        sprintf(DHT11_send.hum, "%.1f", DHT11_Data.humidity);
#endif
        uwRet=LOS_TaskDelay(1500);
        if(uwRet !=LOS_OK)
        return;

  }
}
VOID data_report_task(VOID)
{
    UINT32 uwRet = LOS_OK;
    neul_bc95_reboot();                                             //?????
    while(neul_bc95_get_netstat()<0);                               //???????
    //neul_bc95_set_cdpserver("180.101.147.115");                   //??????
    neul_bc95_set_cdpserver("218.4.33.71");                         //??????
    while(1)
    {
        printf("app task test\r\n");
        /*******************temperature and humidity*****************/
        if(neul_bc95_send_coap_paylaod((const char*)(&DHT11_send),sizeof(DHT11_send))>=0)       //???????
            printf("ocean_send_data OK!\n");                                                    //????
        else                                                                                    //????
        {
            printf("ocean_send_data Fail!\n");
        }
        uart_data_flush();
        memset(bc95_net_data.net_nmgr, 0, 5);
        neul_bc95_read_coap_msg(bc95_net_data.net_nmgr,5);
        printf("%s\n",bc95_net_data.net_nmgr);
        if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //??
        {
        	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);    // ?????
        }
        if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //??
        {
        	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);  // ?????
        }
	    uwRet=LOS_TaskDelay(500);
		if(uwRet !=LOS_OK)
		return;
	}
}
