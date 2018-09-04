#include "define_all.h"
#include "los_uart.h"
#include "neul_bc95.h"
#include "dht12.h"
#include <string.h> 

#define CN_RCV_BUFLEN  128
char gRcvBuf[CN_RCV_BUFLEN];
#define COM_UART   3

VOID* main_printf(UINT32 uwArg)
{
	char *str = "hello newworld\r\n";
	while(1)
	{		
	  LOS_TaskDelay(1000);
	  printf("HELLO NEWWORLD\n\r");
	} 
	return NULL;
}

VOID* mainsendtask(UINT32 uwArg)
{

	char *str = "hello newworld\r\n";
	while(1)
	{		
	  LOS_TaskDelay(1000);
	  los_dev_uart_write(3,str,strlen(str),100);
	} 
	return NULL;
}

VOID* mainreceivetask(UINT32 uwArg)
{
	int ret;
	char buf[64];
	while(1)
	{		
		//printf("hello\n\r");
	  ret = los_dev_uart_read(3,buf,64,100);

		if(ret > 0)
		{
					printf("read %d bytes\n\r",ret);
		}
		else
		{
			printf("receive timeout");
		}
	} 
	return NULL;
}

//flash the led
#define LED1_GPIO		GPIOA
#define LED1_PIN		GPIO_Pin_5
/*操作宏定义*/
#define LED1_ON			GPIO_ResetBits(LED1_GPIO, LED1_PIN)
#define LED1_OFF		GPIO_SetBits(LED1_GPIO, LED1_PIN)
#define LED1_TOG		GPIO_ToggleBits(LED1_GPIO, LED1_PIN)
VOID* main_dwg(UINT32 uwArg)
{
	while(1)
	{
		
		IWDT_Clr();
		LOS_TaskDelay(10); 
		LED1_ON;
		LOS_TaskDelay(500); //wait five seconds
		LED1_OFF;
		LOS_TaskDelay(500); //wait five seconds
	}
	
	return 	NULL;
}


static INT8 temp_dht12 = 0;
static UINT8 humidity_dht12 = 0; 
INT8 tempTemp;  //temporary temperature
UINT8 humidityTemp;  //temporary humidity

//nb95 report/receive data from cloud
VOID* main_nb95(UINT32 uwArg)
{
	int  ret;
	char buf[10];
  UINT32 uwRet = LOS_OK;
	ret = neul_bc95_reboot();  	//初始化模块
	while(1)
	{
		LOS_TaskDelay(1000); //wait five seconds
		ret = neul_bc95_set_cdpserver("218.4.33.72");
		if(ret <0)
		{
			continue; //链接华为云服务器不OK
		}
		else
		{
			printf("NB MODULE SET SERVER OK\n\r");
		}

		ret = neul_bc95_get_csq();
		if(ret <0)
		{
			continue; //信号强度
		}
		else
		{
			printf("NB MODULE CSQ:%d\n\r",ret);
		}
		LOS_TaskDelay(1000); //wait five seconds
		ret = neul_bc95_get_netstat() ;
		if(ret<0)
		{
			continue; //模组网络状态不OK
		}
		else
		{
			printf("NB MODULE NET CHECK OK\n\r");
		}
		break; //all the check is ok, then do the costumer things
	}

	//now do the application  works:upload ourstat
    while(1)
    {
		LOS_TaskDelay(5000); //wait five seconds
        printf("app task test\r\n");
        /*******************temperature and humidity*****************/
				//char *str = "  123";
        char strdht12[5] = {0};
				if (temp_dht12 != 0x00)
				{
				  sprintf(strdht12, "   %d", temp_dht12);					
				}
				else
				{
					sprintf(strdht12, "   %d%d", temp_dht12, temp_dht12);
				}
        if(neul_bc95_send_coap_paylaod((const char*)(strdht12),5)>=0)       //发送数据到平台
        {
            printf("ocean_send_data OK!\n");
        }//发送成功
        else                                                                                    //发送失败
        {
            printf("ocean_send_data Fail!\n");
        }
        uart_data_flush();
		LOS_TaskDelay(5000);
        //check if any data receive
        memset(buf,0,sizeof(buf));
        ret = neul_bc95_read_coap_msg(buf,5);
        if(ret >0)
        {
			printf("RECEIVE DATA:%d :%s\n\r",ret,buf);
			if(0 == memcmp("buf","ON",2))
			{
				LED0_ON;
			}
			else if(0 == memcmp("buf","OFF",3))
			{
				LED0_OFF;
			}
			else
			{
				LED0_TOG;
			}
        }
    }
}

// init dht12
// return:  0 init ok , !0 not ok
int temp_sensor_init()
{                
    uint8_t ret = DHT12_Init();
    return ret;
}

// collect temperature and humidity
VOID* main_dht12(UINT32 uwArg)
{
	/* Init Temperature and humidity sensor */
	uint8_t ret;
	(VOID)LOS_TaskDelay(1000);
	IWDT_Clr(); 
	(VOID)LOS_TaskDelay(1000);
	IWDT_Clr(); 
	(VOID)LOS_TaskDelay(1000);
	IWDT_Clr(); 
		UINT32 uwIntSave;
		uwIntSave = LOS_IntLock();
    temp_sensor_init();
		LOS_IntRestore(uwIntSave);
	while(1){
		UINT32 uwIntSave;
		uwIntSave = LOS_IntLock();
		ret = DHT12_Read_Data(&tempTemp, &humidityTemp);
		LOS_IntRestore(uwIntSave);
		if(!ret)
		{
			temp_dht12 = tempTemp;
			humidity_dht12 = humidityTemp;      	      
//			printf("running, Temp = %d, Humidity = %d \r\n", temp_dht12, humidity_dht12); 
		}
		IWDT_Clr(); 
		LOS_TaskDelay(1000); 
//		TicksDelayMs(100,NULL);
	}
}


UINT32 creat_maintask(const char *taskname,TSK_ENTRY_FUNC fnTask,UINT32 stackisize,UINT32 uwArg,UINT16 prior)
{
    UINT32 uwRet = LOS_OK;
		UINT32  handle;
    TSK_INIT_PARAM_S task_init_param;
	
		task_init_param.uwArg = uwArg;
    task_init_param.usTaskPrio = prior;
    task_init_param.pcName =(char *) taskname;
    task_init_param.pfnTaskEntry = fnTask;
    task_init_param.uwStackSize = stackisize;
    uwRet = LOS_TaskCreate(&handle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}
int main (void)
{	
  UINT32 uwRet = LOS_OK;
	Init_System();				//系统初始化
	Init_PrintUartx();   //suzhen
	
  uwRet = LOS_KernelInit();
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }
	//initialize the uart for the comunication
	los_dev_uart_init(COM_UART,9600,gRcvBuf,CN_RCV_BUFLEN);

	printf("hello IOT\n\r");
  uwRet = creat_maintask("main_nb95",main_nb95,0x400,0,12);
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }

  uwRet = creat_maintask("main_dwg",main_dwg,0x400,0,14);
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }

	uwRet = creat_maintask("main_dht12",main_dht12,0x400,0,16);
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }

  LOS_Start();
	while(1);
}




