/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		UDP演示函数
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "utility.h"
#include "udp_demo.h"


static uint8 buff[2048];                                                /*定义一个2KB的缓存*/ 

/**
*@brief		UDP测试程序
*@param		无
*@return	无
*/
void do_udp(void)
{                                                              
    uint16 len=0;
    switch(getSn_SR(SOCK_UDPS))                                        /*获取socket的状态*/
    {
    case SOCK_CLOSED:                                                  /*socket处于关闭状态*/
        socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                      /*初始化socket*/
        break;

    case SOCK_UDP:                                                     /*socket初始化完成*/
        wizDelayMs(10);
        if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
        {
            setSn_IR(SOCK_UDPS, Sn_IR_RECV);                           /*清接收中断*/
        }
        if((len=getSn_RX_RSR(SOCK_UDPS))>0)                            /*接收到数据*/
        {
            recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);     /*W5500接收计算机发送来的数据*/
            buff[len-8] = 0x00;                                        /*添加字符串结束符*/
            printf("%s\r\n",buff);                                     /*打印接收缓存*/ 
            sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);      /*W5500把接收到的数据发送给Remote*/
        }
        break;
    }

}

