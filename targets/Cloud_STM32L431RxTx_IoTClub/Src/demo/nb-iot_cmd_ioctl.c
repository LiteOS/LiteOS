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

#include "nb-iot_cmd_ioctl.h"
#include "nb-iot_demo.h"
#include "bc95.h"
#include "main.h"


msg_sys_type bc95_net_data;
/* ============================================================
func name   :  neul_bc95_hex_to_str
discription :  convert char to hex string 
               
param       :  bufin @ input param , the data that need to convert
               len @ input param, data length
               bufout @ output param, hex string data 
return      :  0 convert ok , !0 not ok
Revision    : 
=============================================================== */
static int neul_bc95_hex_to_str(const char *bufin, int len, char *bufout)
{
    int i = 0;
    unsigned char tmp2 = 0x0;
    unsigned int tmp = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i = i+2)
    {
        #if 1
        tmp2 =  bufin[i];
        tmp2 =  tmp2 <= '9'?tmp2-0x30:tmp2-0x37;
        tmp =  bufin[i+1];
        tmp =  tmp <= '9'?tmp-0x30:tmp-0x37;
        bufout[i/2] =(tmp2<<4)|(tmp&0x0F);
        #else
        sscanf(bufin+i, "%02x", &tmp);
        bufout[i/2] = tmp;
        #endif
    }
    return 0; 
}

int nb_cmd_data_ioctl(void* arg, char  * buf, int32_t len)
{
		int readlen = 0;
		char tmpbuf[1064] = {0};
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
				sscanf(buf,"\r\n+NNMI:%d,%s\r\n",&readlen,tmpbuf);
				memset(bc95_net_data.net_nmgr, 0, 30);
    if (readlen > 0)
    {  
				neul_bc95_hex_to_str(tmpbuf, readlen*2, bc95_net_data.net_nmgr);
    }
				AT_LOG("cmd is:%s\n",bc95_net_data.net_nmgr);
		
/***************************board LED control*********************************/			
	#ifdef	MODULE_DHT11
		if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开灯
			{	
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);    // 输出高电平
			}
		if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关灯
			{	
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);  // 输出低电平
			}
	#endif
/*******************************END**********************************************/
			
/***************************smoke beep control*********************************/			
	#ifdef	MODULE_SMOKE
		if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开蜂鸣器
			{	
					HAL_GPIO_WritePin(Beep_GPIO_Port,Beep_Pin,GPIO_PIN_SET);    // 输出高电平
			}
		if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关蜂鸣器
			{	
					HAL_GPIO_WritePin(Beep_GPIO_Port,Beep_Pin,GPIO_PIN_RESET);  // 输出低电平
			}
	#endif
/*******************************END**********************************************/
			
/***************************GPS beep control*********************************/			
	#ifdef	MODULE_GPS
		if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开灯
			{	
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);    // 输出高电平
			}
		if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关灯
			{	
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);  // 输出低电平
			}
	#endif
/*******************************END**********************************************/
			
/***************************Light LED control*********************************/			
	#ifdef	MODULE_BH1750
		if(strcmp(bc95_net_data.net_nmgr,"ON")==0) //开灯
			{	
					HAL_GPIO_WritePin(Light_GPIO_Port,Light_Pin,GPIO_PIN_RESET);    // 输出低电平
			}
		if(strcmp(bc95_net_data.net_nmgr,"OFF")==0) //关灯
			{	
					HAL_GPIO_WritePin(Light_GPIO_Port,Light_Pin,GPIO_PIN_SET);  // 输出高电平
			}
	#endif
/*******************************END**********************************************/

	return 0;
}




