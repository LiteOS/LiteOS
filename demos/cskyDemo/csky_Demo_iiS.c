
/********************************************************************
 * I2S(Inter—ICSound)总线, 又称集成电路内置音频总线
 * 是飞利浦公司为数字音频设备之间的音频数据传输而制定的一种总线标准
 * 该总线专门用于音频设备 之间的数据传输，广泛应用于各种多媒体系统。
 * 标准的 I2S 总线电缆是由 3 根串 行导线组成的：
 * 								1 根是 时分多路复用（简称 TDM）数据线；
 * 								1 根是 字选择线；
 * 								1 根是 时钟线
 * ****************************************************************/
 
#include "stdio.h"
#include "csky_All_Demo.h"


#ifdef CONFIG_Audio_Codec
#include "ck_i2s.h"
#include "drv_i2s.h"
#include <string.h>
#include <csi_config.h>
#include "soc.h"
#include "pin.h"

  
 static volatile uint8_t i2s_event_flag;
 static i2s_handle_t i2s_handle=NULL;
 #define WAV_DATA_BASEADDR 		0x10010000 
 #define CODEC_PLAYBACK_TYPE 	0x01 
 #define CODEC_CAPTRUE_TYPE 	0x02 
 #define MUSIC_SIZE_BUFFER		1024*4* 17
 
//导入外设（音频编码器）初始化函数
extern int codec_init(uint8_t type); //包括设备和引脚的初始化（但此工程中未定义该函数）
extern int read_wav_init(uint32_t address,int32_t* SR,int32_t* ch,int32_t* sw,int32_t*ws);
extern int read_wav_data(uint32_t wav_data_addr,uint8_t* data_send,uint32_t cnt); 


 //i2s的回调函数
static void i2s_event_cb_fun(int32_t idx,i2s_event_e event) 
{ 
	if(event==I2S_EVENT_RECEIVE_COMPLETE||event==I2S_EVENT_SEND_COMPLETE)
	{ 
		i2s_event_flag=1; 
	}
} 

static void sound_i2s_stream_playback(i2s_handle_t handle) 
{ 
	/*read wav file from storage device,the storage device base address is WAV_DATA_BASEADDR */ 
	uint32_t samplerate;
	uint32_t channle; 
	uint32_t samplewidth;
	uint32_t wav_size;
	

	//初始化音频编码器  
	int32_t ret=codec_init(CODEC_PLAYBACK_TYPE); 
	if(ret<0)
	{ 
		printf("[error]codec_init error(缺少音频编码器设备)\n"); 
		return; 
	}
	ret=read_wav_init(WAV_DATA_BASEADDR,(int32_t*)&samplerate,(int32_t*)&channle, (int32_t*)&samplewidth,(int32_t*)&wav_size);
 
	ret=csi_i2s_config_protocol(handle,I2S_PROTOCOL_I2S); 
	if(ret<0){ printf("[error]config protocol error\n"); return; }

	ret=csi_i2s_config_sample_width(handle,samplewidth); 
	if(ret<0){ printf("[error]config sample width error\n"); return; }

	ret=csi_i2s_config_work_mode(handle,I2S_MODE_TX_MASTER); 
	if(ret<0){ printf("[error]config work mode error\n"); return; }

	ret=csi_i2s_config_sample_rate(handle,samplerate); 
	if(ret<0){ printf("[error]config sample rate error\n"); return; }

	uint8_t*data_send=malloc(MUSIC_SIZE_BUFFER); 
	if(data_send==NULL){ printf("[error]malloc error\n"); return; }
 
	uint32_t wav_data_addr=0; 
	uint32_t cnt=0; 
	csi_i2s_power_control(i2s_handle,DRV_POWER_FULL);
 
	while(wav_size)
	{ 
		if(wav_size<MUSIC_SIZE_BUFFER)
		{ 
			cnt=wav_size; 
		}else{ 
			cnt=MUSIC_SIZE_BUFFER; 
		} 
		read_wav_data(wav_data_addr,data_send,cnt); 
		i2s_event_flag=0; 
		csi_i2s_send(i2s_handle,data_send,cnt); 
		while(!i2s_event_flag){}
		wav_data_addr+=cnt; 
		wav_size-=cnt; 
	}
	csi_i2s_power_control(i2s_handle,DRV_POWER_OFF); 
	free(data_send); //这里一定要记住释放空间
	printf("[info]sound_i2s_stream_playback done\n");
}

int task_IIS(void) 
{ 
	
	//1、得到I2S的句柄
	i2s_handle=csi_i2s_initialize(0,i2s_event_cb_fun);
	if(i2s_handle==NULL)
	{ 
		printf("[error]csi_i2s_initialize error\n"); 
		return 0;
	}
	printf("[success]csi_i2s_initialize success\n");
	
	//2、开始 音频i2s流 播放
	sound_i2s_stream_playback(i2s_handle);
	return 0;
}

#else //未定义音频驱动文件宏
int task_IIS(void) 
{ 
	
	HopeRun_printf("开始IIS测试............\n");
	printf("[info]I2S(Inter—ICSound)总线, 又称集成电路内置音频总线。\n（1）iis是飞利浦公司为数字音频设备之间的音频数据传输而制定的一种总线标准。\n（2）该总线专门用于音频设备之间的数据传输，广泛应用于各种多媒体系统。\n");
	printf("[info]本工程不存在Audio Codec音频编码器的驱动文件和驱动设备。因而选择禁用IIS测试用例。\n");
	return 0;
}

#endif