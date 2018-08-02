/**
******************************************************************************
* @file   		ntp.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		ntp 演示函数  
******************************************************************************
**/
#include "w5500.h"
#include "w5500_conf.h"
#include "socket.h"
#include "types.h"
#include "ntp.h"

#include <string.h>
#include <stdio.h>

NPTformat ntpformat;
DateTime nowdate;
uint8 ntp_message[48];
tstamp total_seconds=0;
uint8 time_zone = 39;
uint8 ntp_server_ip[4]={202,112,29,82}; 					/*NTP服务器地址*/
uint8 ntp_port = 123; 																											/*NTP服务器端口号*/
uint8 bufpub[2048];
uint8 npt_debuge=0;

/**
*@brief		从NTP服务器获取时间
*@param		buf：存放缓存
*@param		idx：定义存放数据起始位置
*@return	无
*/
void get_seconds_from_ntp_server(uint8* buf,uint16 idx)
{
	tstamp seconds = 0;
	uint8 i=0;
	for (i = 0; i < 4; i++)
	{
		seconds = (seconds << 8) | buf[idx + i];
	}
	switch (time_zone)
	{
		case 0:
			seconds -=  12*3600;
		break;
		case 1:
			seconds -=  11*3600;
		break;
		case 2:
			seconds -=  10*3600;
		break;
		case 3:
			seconds -=  (9*3600+30*60);
		break;
		case 4:
			seconds -=  9*3600;
		break;
		case 5:
		case 6:
			seconds -=  8*3600;
		break;
		case 7:
		case 8:
			seconds -=  7*3600;
		break;
		case 9:
		case 10:
		seconds -=  6*3600;
		break;
		case 11:
		case 12:
		case 13:
			seconds -= 5*3600;
		break;
		case 14:
			seconds -=  (4*3600+30*60);
		break;
		case 15:
		case 16:
			seconds -=  4*3600;
		break;
		case 17:
			seconds -=  (3*3600+30*60);
		break;
		case 18:
			seconds -=  3*3600;
		break;
		case 19:
			seconds -=  2*3600;
		break;
		case 20:
			seconds -=  1*3600;
		break;
		case 21:                           
		case 22:
		break;
		case 23:
		case 24:
		case 25:
			seconds +=  1*3600;
		break;
		case 26:
		case 27:
			seconds +=  2*3600;
		break;
		case 28:
		case 29:
			seconds +=  3*3600;
		break;
		case 30:
			seconds +=  (3*3600+30*60);
		break;
		case 31:
			seconds +=  4*3600;
		break;
		case 32:
			seconds +=  (4*3600+30*60);
		break;
		case 33:
			seconds +=  5*3600;
		break;
		case 34:
			seconds +=  (5*3600+30*60);
		break;
		case 35:
			seconds +=  (5*3600+45*60);
		break;
		case 36:
			seconds +=  6*3600;
		break;
		case 37:
			seconds +=  (6*3600+30*60);
		break;
		case 38:
			seconds +=  7*3600;
		break;
		case 39:
			seconds +=  8*3600;
		break;
		case 40:
			seconds +=  9*3600;
		break;
		case 41:
			seconds +=  (9*3600+30*60);
		break;
		case 42:
			seconds +=  10*3600;
		break;
		case 43:
			seconds +=  (10*3600+30*60);
		break;
		case 44:
			seconds +=  11*3600;
		break;
		case 45:
			seconds +=  (11*3600+30*60);
		break;
		case 46:
			seconds +=  12*3600;
		break;
		case 47:
			seconds +=  (12*3600+45*60);
		break;
		case 48:
			seconds +=  13*3600;
		break;
		case 49:
			seconds +=  14*3600;
		break;
	}
	
	total_seconds=seconds;
	calc_date_time(seconds); 																									/*由UTC时间计算日期*/
}

/**
*@brief		初始化NTP Client信息
*@param		无
*@return	无
*/
void ntp_client_init(void)
{
	uint8 flag;
	ntp_timer_init();
	ntpformat.dstaddr[0] = ntp_server_ip[0];
	ntpformat.dstaddr[1] = ntp_server_ip[1];
	ntpformat.dstaddr[2] = ntp_server_ip[2];
	ntpformat.dstaddr[3] = ntp_server_ip[3];
		
	ntpformat.leap = 0;           /* leap indicator */ 
	ntpformat.version = 4;        /* version number */
	ntpformat.mode = 3;           /* mode */
	ntpformat.stratum = 0;        /* stratum */
	ntpformat.poll = 0;           /* poll interval */
	ntpformat.precision = 0;      /* precision */
	ntpformat.rootdelay = 0;      /* root delay */
	ntpformat.rootdisp = 0;       /* root dispersion */
	ntpformat.refid = 0;          /* reference ID */
	ntpformat.reftime = 0;        /* reference time */
	ntpformat.org = 0;            /* origin timestamp */
	ntpformat.rec = 0;            /* receive timestamp */
	ntpformat.xmt = 1;            /* transmit timestamp */
	
	flag = (ntpformat.leap<<6)+(ntpformat.version<<3)+ntpformat.mode; //one byte Flag
	memcpy(ntp_message,(void const*)(&flag),1);
}

/**
*@brief			执行NTP Client主函数
*@param			无
*@return		无
*/
uint8 ntp_try_times=0; 												/*请求次数*/
void do_ntp_client(void)
{
	if(total_seconds>0)return; 										/*已获取时间则不再执行NPT程序*/
	else
	{
		uint16 len;
		uint8 * data_buf = bufpub;
		uint32 destip = 0;
		uint16 destport;
		uint16 startindex = 40; 									/*回复包中时间数据首地址*/
		switch(getSn_SR(SOCK_NTP))
		{
			case SOCK_UDP:																										/*UDP模式开启*/
				if(total_seconds>0) return;							/*已获取时间则不再执行NPT程序*/
				if(ntp_try_times<100)
				{  
					if(ntptimer>0) 																								/*1秒请求一次*/
					{
						/*发送请求到NPT服务器*/
						sendto(SOCK_NTP,ntp_message,sizeof(ntp_message),ntp_server_ip, ntp_port);
						ntp_try_times++;
						if(npt_debuge)
							printf("ntp_try_times:%d \r\n",ntp_try_times);	/*打印请求次数*/
							
						ntptimer=0;									 																/*在定时器2中断中加1*/
					}
				}
				else 
				{
					ntp_try_times=0;
					if(npt_debuge)
					printf("ntp retry failed!\r\n");						/*NTP请求失败*/
				}
				if ((len = getSn_RX_RSR(SOCK_NTP)) > 0) 		
				{
					if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE;	
					recvfrom(SOCK_NTP, data_buf, len, (uint8*)&destip, &destport);  /*接收NTP服务器回复数据*/
					get_seconds_from_ntp_server(data_buf,startindex);				/*从NTP服务器获取时间*/
					printf("北京时间：%d-%02d-%02d %02d:%02d:%02d\r\n",  
					(nowdate.year[0]<<8)+nowdate.year[1],							
					nowdate.month,
					nowdate.day,
					nowdate.hour,
					nowdate.minute,
					nowdate.second);
					ntp_try_times=0;
				}

			break;
			case SOCK_CLOSED:  																							
				socket(SOCK_NTP,Sn_MR_UDP,ntp_port,0);
			break;
		}
	} 
}

/**
*@brief			计算日期时间
*@param			secondsUTC 世界标准时间 
*@return		无
*/
void calc_date_time(tstamp seconds)
{
	uint8 yf=0;
	uint32 p_year_total_sec;
	uint32 r_year_total_sec;
	tstamp n=0,d=0,total_d=0,rz=0;
	uint16 y=0,r=0,yr=0;
	signed long long yd=0;
	
	n = seconds;
	total_d = seconds/(SECS_PERDAY);
	d=0;
	p_year_total_sec=SECS_PERDAY*365;
	r_year_total_sec=SECS_PERDAY*366;
	while(n>=p_year_total_sec) 
	{
		if((EPOCH+r)%400==0 || ((EPOCH+r)%100!=0 && (EPOCH+r)%4==0))
		{
			n = n -(r_year_total_sec);
			d = d + 366;
		}
		else
		{
			n = n - (p_year_total_sec);
			d = d + 365;
		}
		r+=1;
		y+=1;

	}

	y += EPOCH;

	nowdate.year[0] = (uint8)((y & 0xff00)>>8);
	nowdate.year[1] = (uint8)(y & 0xff);

	yd=0;
	yd = total_d - d;

	yf=1;
	while(yd>=28) 
	{
	
		if(yf==1 || yf==3 || yf==5 || yf==7 || yf==8 || yf==10 || yf==12)
		{
			yd -= 31;
			if(yd<0)break;
			rz += 31;
		}
		
		if (yf==2)
		{
			if (y%400==0 || (y%100!=0 && y%4==0)) 
			{
				yd -= 29;
				if(yd<0)break;
				rz += 29;
			}
			else 
			{
				yd -= 28;
				if(yd<0)break;
				rz += 28;
			}
		} 
		if(yf==4 || yf==6 || yf==9 || yf==11 )
		{
			yd -= 30;
			if(yd<0)break;
			rz += 30;
		}
		yf += 1;

	}
	nowdate.month=yf;
	yr = total_d-d-rz;
	
	yr += 1;
	
	nowdate.day=yr;
	
	//calculation for time
	seconds = seconds%SECS_PERDAY;
	nowdate.hour = seconds/3600;
	nowdate.minute = (seconds%3600)/60;
	nowdate.second = (seconds%3600)%60;

}

/**
*@brief			改编日期时间为秒
*@param		  seconds： 
*@return		无
*/
tstamp change_datetime_to_seconds(void) 
{
	tstamp seconds=0;
	uint32 total_day=0;
	uint16 i=0,run_year_cnt=0,l=0;
	
	l = nowdate.year[0];//high
	
	l = (l<<8);
	
	l = l + nowdate.year[1];//low
	
	
	for(i=EPOCH;i<l;i++)
	{
		if((i%400==0) || ((i%100!=0) && (i%4==0))) 
		{
			run_year_cnt += 1;
		}
	}
	
	total_day=(l-EPOCH-run_year_cnt)*365+run_year_cnt*366;
	
	for(i=1;i<=nowdate.month;i++)
	{
		if(i==5 || i==7 || i==10 || i==12)
		{
			total_day += 30;
		}
		if (i==3)
		{
			if (l%400==0 && l%100!=0 && l%4==0) 
			{
				total_day += 29;
			}
			else 
			{
				total_day += 28;
			}
		} 
		if(i==2 || i==4 || i==6 || i==8 || i==9 || i==11)
		{
			total_day += 31;
		}
	}
	
	seconds = (total_day+nowdate.day-1)*24*3600;  
	seconds += nowdate.second;
	seconds += nowdate.minute*60;
	seconds += nowdate.hour*3600;
	
	return seconds;
}
