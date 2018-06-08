/*
 Copyright (c) 2018 Chinamobile

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "cmsis_os.h"
#include "M5310.h"
#include "usart3.h"
#include "cis_internals.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "win_timers.h"

#include "los_bsp_led.h"

void hex_to_hexstring(char *src,char *dst,int len)
{
	int i = 0,j = 0;
	for(i=0,j=0;j<len*2;i++,j++)
	{
		if((src[i]<=9)&&(src[i]>=0))
		{
			dst[j] =  '0';
			dst[++j] = src[i] + '0';	
		}
		else  if(src[i]>=0x0A&&src[i]<=0x0F)
		{
			dst[j] =  '0';
			dst[++j] = src[i] + 55;	
		}
		else if (src[i]>=0x0F)
		{
			dst[j] =  src[i]/16 +'0';
			switch (dst[j])
			{
				case '0': dst[j] = '0';break;
				case '1': dst[j] = '1';break;
				case '2': dst[j] = '2';break;
				case '3': dst[j] = '3';break;
				case '4': dst[j] = '4';break;
				case '5': dst[j] = '5';break;
				case '6': dst[j] = '6';break;
				case '7': dst[j] = '7';break;
				case '8': dst[j] = '8';break;
				case '9': dst[j] = '9';break;
				case ':': dst[j] = 'A';break;
				case ';': dst[j] = 'B';break;
				case '<': dst[j] = 'C';break;
				case '=': dst[j] = 'D';break;
				case '>': dst[j] = 'E';break;
				case '?': dst[j] = 'F';break;
				default : break;
			}
			dst[++j] =  src[i]%16 +'0';
			switch (dst[j])
			{
				case '0': dst[j] = '0';break;
				case '1': dst[j] = '1';break;
				case '2': dst[j] = '2';break;
				case '3': dst[j] = '3';break;
				case '4': dst[j] = '4';break;
				case '5': dst[j] = '5';break;
				case '6': dst[j] = '6';break;
				case '7': dst[j] = '7';break;
				case '8': dst[j] = '8';break;
				case '9': dst[j] = '9';break;
				case ':': dst[j] = 'A';break;
				case ';': dst[j] = 'B';break;
				case '<': dst[j] = 'C';break;
				case '=': dst[j] = 'D';break;
				case '>': dst[j] = 'E';break;
				case '?': dst[j] = 'F';break;
				default : break;
			}
		}

	}	 
	strcat(dst,"\r\n");
// nbiot_printf(dst);
	return;
}

char hexstring_to_hex(unsigned char *outstr,unsigned char *instr,int len)
{
	int i = 0;
	int j = 0;
	int temp[2];
	unsigned char *Port2Recevie = instr;
	for(j=0;j<len;j++)
	{
		temp[0] = Port2Recevie[i];
		i++;
		temp[1] = Port2Recevie[i];
		i++;
		switch(temp[0])
		{
			case '0' : temp[0] = 0;break;
			case '1' : temp[0] = 1;break;
			case '2' : temp[0] = 2;break;
			case '3' : temp[0] = 3;break;
			case '4' : temp[0] = 4;break;
			case '5' : temp[0] = 5;break;
			case '6' : temp[0] = 6;break;
			case '7' : temp[0] = 7;break;
			case '8' : temp[0] = 8;break;
			case '9' : temp[0] = 9;break;
			case 'A' : temp[0] = 10;break;
			case 'B' : temp[0] = 11;break;
			case 'C' : temp[0] = 12;break;
			case 'D' : temp[0] = 13;break;
			case 'E' : temp[0] = 14;break;
			case 'F' : temp[0] = 15;break;
			default: break;
		}
		switch(temp[1])
		{
			case '0' : temp[1] = 0;break;
			case '1' : temp[1] = 1;break;
			case '2' : temp[1] = 2;break;
			case '3' : temp[1] = 3;break;
			case '4' : temp[1] = 4;break;
			case '5' : temp[1] = 5;break;
			case '6' : temp[1] = 6;break;
			case '7' : temp[1] = 7;break;
			case '8' : temp[1] = 8;break;
			case '9' : temp[1] = 9;break;
			case 'A' : temp[1] = 10;break;
			case 'B' : temp[1] = 11;break;
			case 'C' : temp[1] = 12;break;
			case 'D' : temp[1] = 13;break;
			case 'E' : temp[1] = 14;break;
			case 'F' : temp[1] = 15;break;
			default: break;
		}
		outstr[j] = temp[0]*16 + temp[1];
	}
	return 0;
}

void M5310_Power_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
}

void netdev_init(void)
{
	M5310_Power_Init();
	osDelay(5000);
	USART3_Init();
	SendCmd("AT+CIMI\r\n", "OK", 2000, 5);
	SendCmd("AT+CMVER\r\n", "OK", 3000, 2);
	SendCmd("AT+CMEE=1\r\n","OK", 2000, 2);
	SendCmd("AT+CSCON=1\r\n","OK", 2000, 2);
	SendCmd("AT+CEREG=2\r\n","OK", 2000, 2);
	SendCmd("AT+CEDRXS=0,5\r\n","OK",3000, 2);
	SendCmd("AT+CPSMS=0\r\n","OK", 3000, 2);
	SendCmd("AT+CEREG?\r\n","CEREG:2,1", 3000, 0xFFFF);
	SendCmd("AT+csq\r\n", "OK", 2000, 3);
	SendCmd("AT+NSOCR=DGRAM,17,78,2\r\n", "OK", 2000, 0xFFFF);  /* TODO: update the M5310 firmware */
	printf("connect NB-IoT sucess\r\n");
}

uint32_t ip_SendData(char * buf, char *ptr, uint32_t len)
{
	hex_to_hexstring(buf, ptr+strlen(ptr), len);
	SendData(ptr, "OK", strlen((const char *)ptr), 100, 2);
	return len;
}

