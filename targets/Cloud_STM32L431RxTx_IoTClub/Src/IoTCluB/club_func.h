/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 1.0 
 *Title: Club func.h
**/

#ifndef __CLUB_FUNC__
#define __CLUB_FUNC__

#include "sys_init.h"
#include "agent_tiny_demo.h"
#include "nb-iot_demo.h"
#include "club_include.h"
#include "club_oled.h"
#include "club_key.h"
#include "club_nbiot.h"

#define CLUB_DEBUG	printf

/*   创建任务  */
UINT32 creat_all_task();

//extern void main_task(void);
extern void proc_main_task(UINT32 uwArg);

extern UINT32 g_TskHandle;



#endif 

