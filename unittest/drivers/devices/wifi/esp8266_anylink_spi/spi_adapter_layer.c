#include "spi_adapter_layer.h"
#include "unistd.h"
#include <semaphore.h>
#include "los_task.h"


int LOS_SemPost_Control = 0;
int HAL_SPI_Init_Control = 0;
int M8266HostIf_SPI_Select_Control = 0;
int M8266WIFI_SPI_Interface_Communication_OK_Control = 0; 
int M8266WIFI_SPI_Interface_Communication_Stress_Test_Control = 0;
int M8266WIFI_SPI_Set_Opmode_Control = 0;
int M8266WIFI_SPI_Get_Opmode_Control = 0;
int M8266WIFI_SPI_STA_Connect_Ap_Control = 0;
int M8266WIFI_SPI_Get_STA_Connection_Status_Control = 0;
int M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI_Control = 0;
int M8266WIFI_SPI_Get_STA_IP_Addr_Control = 0;
int M8266WIFI_SPI_Setup_Connection_Control = 0;
int M8266WIFI_SPI_Query_Connection_Control = 0;
int M8266WIFI_SPI_Send_Data_Control = 0;
int M8266WIFI_SPI_RecvData_Control = 0;
int M8266WIFI_SPI_Has_DataReceived_Control = 0;
int M8266WIFI_SPI_Delete_Connection_Control = 0;
int LOS_SemCreate_Control = 0;
int LOS_QueueCreate_Control = 0;
int LOS_QueueWriteCopy_Control = 0;
int LOS_TaskCreate_Control = 0;
int LOS_QueueReadCopy_Control = 0;
int LOS_QueueDelete_Control = 0;
int m8266wifi_softstart_config_control = 0;

UINT32  g_vuwIntCount = 0;
UINT64  g_ullTickCount = 1;




void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
	;
}

UINT32 LOS_HwiCreate( HWI_HANDLE_T uwHwiNum, HWI_PRIOR_T usHwiPrio, HWI_MODE_T usMode, HWI_PROC_FUNC pfnHandler, HWI_ARG_T uwArg )
{
	return 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	;
}

UINT32 LOS_SemPost(UINT32 uwSemHandle)
{
	if(LOS_SemPost_Control == 0)
		return 0;
	else
		return 1;
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	;
}


HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
	(void)hspi;
	if(HAL_SPI_Init_Control == 0)
		return (HAL_StatusTypeDef)0;
	else
		return (HAL_StatusTypeDef)1; 
	
}

uint32_t HAL_RCC_GetPCLK2Freq(void)
{
	return 10000000;
}


u8 M8266HostIf_SPI_Select(uint32_t spi_base_addr, uint32_t spi_clock, u16* status)
{
	if(M8266HostIf_SPI_Select_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_Interface_Communication_OK(u8* byte)
{
	if(M8266WIFI_SPI_Interface_Communication_OK_Control == 0)
		return 0;
	else
		return 1;

}


u32 M8266WIFI_SPI_Interface_Communication_Stress_Test(u32 max_times)
{
	if(M8266WIFI_SPI_Interface_Communication_Stress_Test_Control == 0)
		return 0;
	else 
		return 100000;
}

u8 M8266WIFI_SPI_Set_Opmode(u8 op_mode, u8 saved, u16* status)
{
	if(M8266WIFI_SPI_Set_Opmode_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_Get_Opmode(u8* op_mode, u16* status)
{
	if(M8266WIFI_SPI_Get_Opmode_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_STA_Connect_Ap(u8 ssid[32], u8 password[64], u8 saved, u8 timeout_in_s, u16* status)
{
	if(M8266WIFI_SPI_STA_Connect_Ap_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_Get_STA_Connection_Status(u8* connection_status, u16* status)
{
	if(M8266WIFI_SPI_Get_STA_Connection_Status_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI(char* ssid, s8* rssi, u16* status)
{
	if(M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI_Control == 0)
		return 0;
	else
		return 1;
}

u8 M8266WIFI_SPI_Get_STA_IP_Addr(char* sta_ip , u16* status)
{
	if(M8266WIFI_SPI_Get_STA_IP_Addr_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, char* remote_addr, u16 remote_port, u8 link_no, u8 timeout_in_s, u16* status)
{
	if(M8266WIFI_SPI_Setup_Connection_Control == 0)
		return 0;
	else 
		return 1;
}


u8 M8266WIFI_SPI_Query_Connection(u8 link_no, u8* connection_type, u8* connection_state, u16* local_port, u8* remote_ip, u16* remote_port, u16* status)
{
	if(M8266WIFI_SPI_Query_Connection_Control == 0)
		return 0;
	else
		return 1;
}

u16 M8266WIFI_SPI_Send_Data(u8 Data[], u16 Data_len, u8 link_no, u16* status)
{
	if(M8266WIFI_SPI_Send_Data_Control == 0)
		return 0;
	else
		return 10;
}

u16 M8266WIFI_SPI_RecvData(u8 Data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status)
{
	if(M8266WIFI_SPI_RecvData_Control == 0)
		return 0;
	else
		return 10;
}


u8 M8266WIFI_SPI_Has_DataReceived(void)
{
	if(M8266WIFI_SPI_Has_DataReceived_Control == 0)
		return 0;
	else
		return 1;
}


u8 M8266WIFI_SPI_Delete_Connection(u8 link_no, u16* status)
{
	if(M8266WIFI_SPI_Delete_Connection_Control == 0)
		return 0;
	else
		return 1;
		
}


uint64_t osMs2Tick(uint64_t ticks)
{
	return ticks;
}

osStatus_t osDelay (uint32_t ticks)
{
	return osOK;
}


UINT32 LOS_SemCreate(UINT16 usCount, UINT32 *puwSemHandle)
{
	if(LOS_SemCreate_Control == 0)
		return 0;
	else
		return 1;
}


UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID, UINT32 uwFlags,UINT16 usMaxMsgSize)
{
	if(LOS_QueueCreate_Control == 0)
		return 0;
	else
		return 1;
}

UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout)
{
	return 0;
}


UINT32 LOS_QueueWriteCopy(UINT32 uwQueueID, VOID *pBufferAddr, UINT32 uwBufferSize, UINT32 uwTimeOut)
{
	if(LOS_QueueWriteCopy_Control == 0)
		return 0;
	else
		return 1;
}


UINT32 LOS_TaskCreate(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam)
{
	if(LOS_TaskCreate_Control == 0)
		return 0;
	else
		return 1;
}

UINT32 LOS_QueueReadCopy(UINT32 uwQueueID, VOID *pBufferAddr, UINT32 *puwBufferSize, UINT32 uwTimeOut)
{
	if(LOS_QueueReadCopy_Control == 0)
	{
		uint8_t * pointer;
		uint8_t len = strlen((char*)"Hello Hello Hello");
		printf("len is %d\n" , len);
		pointer = (uint8_t *)malloc(len + 1);
		if(pointer != NULL)
		{
			printf("malloc in LOS_QueueReadCopy OK\n");
			((queue_element*)pBufferAddr)->addr = pointer;
			((queue_element*)pBufferAddr)->len  = len + 1;
			strcpy((char*)pointer , (char*)"Hello Hello Hello");
			printf("copy in LOS_QueueReadCopy OK\n");
		}
		return 0;
	}
	else
		return 1;
}

UINT32 LOS_QueueDelete(UINT32 uwQueueID)
{
	if(LOS_QueueDelete_Control == 0)
		return 0;
	else
		return 1;
}


UINTPTR LOS_IntLock(VOID)
{
	return 1;
}


VOID LOS_IntRestore(UINTPTR uvIntSave)
{
	return;
}


int hal_rng_generate_buffer(void* buf, size_t len)
{
	return 0;
}

void atiny_usleep(unsigned long usec)
{
	return;
}


void* atiny_malloc(size_t size)
{
	return (void*)malloc(size);
}


void atiny_free(void* ptr)
{
	if(ptr != NULL)
	{
		free(ptr);
		printf("free OK\n");
	}
}

void    m8266wifi_hardware_config(void)
{
	return;
}

uint8_t m8266wifi_softstart_config()
{
	if(m8266wifi_softstart_config_control == 0)
		return 0;
	else
		return 1;
}

void    m8266wifi_gpio_exti_config(void)
{
	return;
}


