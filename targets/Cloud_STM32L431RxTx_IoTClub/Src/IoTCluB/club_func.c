/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club func.h
 *Addr: www.iot-club.cn
**/

#include "club_func.h"

UINT32 creat_all_task()
{
    UINT32 uwRet = LOS_OK;
	UINT8 taskCount = 0, tempCount = 0;
	TSK_INIT_PARAM_S task_init_param[] = {
		#include "tasklist.h"
	};
	// printf("Begin create task: %d\r\n", (sizeof(task_init_param)/sizeof(task_init_param[0])));

	for(tempCount = 0; tempCount < (sizeof(task_init_param)/sizeof(task_init_param[0])); tempCount ++){
		printf("Create Task \r\n");
		LOS_TaskCreate(&g_TskHandle, &task_init_param[tempCount]);
	}
		
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}