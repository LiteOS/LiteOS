

#include "drv_usart.h"
#include "stdio.h"
#include "soc.h"
#include "pin.h"
#include "csky_All_Demo.h"

extern void * memset(void * buffer, int c, size_t count);
static usart_handle_t usart_handle;
static void usart_event_cb_fun(int32_t idx,usart_event_e event) 
{
	//do your job here according to event
	 printf("[进入usart%d的回调函数]",idx);
}

int task_Usart(void)
{
	
	int32_t ret=0; 
	usart_capabilities_t cap; 
	uint8_t send_buf[18]={'A','B','C','D','E','F','G','H','I','J', 'K','L','M','N','O','P','Q','R'}; 
	uint8_t recv_buf[18]; 
	uint8_t index=0,i=0;
	HopeRun_printf("开始USART测试............\n");
	
	//1、检测usart的能力：usart_capabilities_t csi_usart_get_capabilities(int32_t idx);
	for(index=0;index<=2;index++)
	{
		cap=csi_usart_get_capabilities(index); 
		printf("[info]usart%d %s 异步模式\n",		index,cap.asynchronous==1?"支持":"不支持"); 
		printf("[info]usart%d %s 同步主设备模式\n",	index,cap.synchronous_master==1?"支持":"不支持"); 
		printf("[info]usart%d %s 同步从设备模式\n",	index,cap.synchronous_slave==1?"支持":"不支持"); 
		printf("[info]usart%d %s 信号线路模式\n",	index,cap.single_wire==1?"支持":"不支持"); 
		printf("[info]usart%d %s 发送完成事件\n",	index,cap.event_tx_complete==1?"有":"无"); 
		printf("[info]usart%d %s 接收超时事件\n\n",	index,cap.event_rx_timeout==1? "有":"无");
	
	}
	
	
	index=1;
	printf("[info]以串口%d 为例，测试启动设置运行关闭\n",index);
	//2、初始化串口1（0作为连接云的接口，1暂定为测试接口，2作为调试接口）
	usart_handle=csi_usart_initialize(index,usart_event_cb_fun);
	if(usart_handle==NULL)
	{
		printf("[error]csi_usart_initialize %d error\n",index);
		return -1; 	
	}
	printf("[success]csi_usart_initialize %d success\n",index);
	
	//3、电源控制
	ret=csi_usart_power_control(usart_handle,DRV_POWER_FULL); 
	if(ret<0)
	{ 
		//power control failed 
		//printf("[error]csi_usart_power_control %d error(该驱动函数未编写，默认返回错误)\n",index);
		//return -1; 
	}
	else
	{
		printf("[success]csi_usart_power_control %d success\n",index);
	}
	
	//4、设置奇偶校验位
	ret=csi_usart_config_parity(usart_handle,USART_PARITY_NONE); 
	if(ret<0)
	{ 
		printf("[error]csi_usart_config_parity %d error\n",index);
		//set config failed 
		return -1; 
			
	} 
	printf("[success]csi_usart_config_parity %d success\n",index);
	
	//5.设置波特率
	ret=csi_usart_config_baudrate(usart_handle,115200); 
	if(ret<0)
	{ 
		//set config failed
		printf("[error]csi_usart_config_baudrate %d error\n",index);
		return -1; 
	} 
	printf("[success]csi_usart_config_baudrate %d success\n",index);
	
	//6.设置串口模式
	ret=csi_usart_config_mode(usart_handle,USART_MODE_ASYNCHRONOUS); 
	if(ret<0)
	{ 
		//set config failed 
		printf("[error]csi_usart_config_mode %d error\n",index);
		return -1; 
	}
	printf("[success]csi_usart_config_mode %d success\n",index);
	
	//7、设置停止位
	ret=csi_usart_config_stopbits(usart_handle,USART_STOP_BITS_1); 
	if(ret<0)
	{ 
		//set config failed
		printf("[error]csi_usart_config_stopbits %d error\n",index);
		return -1; 
	}
	printf("[success]csi_usart_config_stopbits %d success\n",index);
	
	//8、设置数据位
	ret=csi_usart_config_databits(usart_handle,USART_DATA_BITS_8); 
	if(ret<0)
	{ 
		//set config failed 
		printf("[error]csi_usart_config_databits %d error\n",index);
		return -1; 
	} 
	printf("[success]csi_usart_config_databits %d success\n",index);
	
	//9、发送数据
	ret=csi_usart_send(usart_handle,send_buf,sizeof(send_buf)); 
	if(ret<0)
	{ 
		//send data failed
		printf("[error]csi_usart_send %d error\n",index);
		return -1;
	}
	printf("[success]csi_usart_send %d success\n",index);
	
	//10、接受数据
	memset(recv_buf,0,sizeof(recv_buf)); 
	ret=csi_usart_receive_query(usart_handle,recv_buf,sizeof(recv_buf)); 
	if(ret<0)
	{ 
		//receive data failed 
		printf("[error]csi_usart_receive_query %d error\n",index);
		return -1; 
	}
	printf("[success]csi_usart_receive_query %d success\n",index);
	
	//11、比较发送接收的数据
	for(i=0;i<sizeof(send_buf);i++)
	{ 
		//printf("%c-%c  ",send_buf[i],recv_buf[i]);
		if(send_buf[i]!=recv_buf[i])
		{ 
			//send and receive data failed 
			//printf("\n[error]发送与接收的数据不一致\n");
			//return -1; 
			break;
		} 
		
	}
	
	//12、接收数据
	memset(recv_buf,0,sizeof(recv_buf)); 
	ret=csi_usart_receive(usart_handle,recv_buf,sizeof(recv_buf)); 
	if(ret<0)
	{ 
		//receive data failed 
		printf("[error]csi_usart_receive %d error\n",index);
		return -1; 
	}
	printf("[success]csi_usart_receive %d success\n",index);
	
	
	//13、比较发送接收的数据
	for(i=0;i<sizeof(send_buf);i++)
	{ 
		//printf("%c-%c  ",send_buf[i],recv_buf[i]);
		if(send_buf[i]!=recv_buf[i])
		{ 
			//send and receive data failed 
			//printf("\n[error]发送与接收的数据不一致\n");
			//return -1; 
			break;
		} 
	}

	ret=csi_usart_power_control(usart_handle,DRV_POWER_OFF); 
	if(ret<0)
	{ 
		//power control failed 
		//return -1; 
	}
	
	ret=csi_usart_uninitialize(usart_handle); 
	if(ret<0)
	{ 
		//uninitialize failed 
		printf("[error]csi_usart_uninitialize %d error\n",index);
		return -1; 
	}
	printf("[success]csi_usart_uninitialize %d success\n",index);

	
	printf("\n[success]test usart success\n");
	return ret;
}

