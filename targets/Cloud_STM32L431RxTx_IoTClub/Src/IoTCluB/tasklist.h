#ifndef __TASKLIST__
#define __TASKLIST__


/*            TASK           TaskPrio		     Arg  	StatckSize												pcName 					Resved     */
TASK_ITEM(proc_main_task,       0,   					0, 			0x1000,									 			"main_task", 			0)
TASK_ITEM(proc_oled_task,       1,   					0, 			LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE, 			"oled_task", 			0)

#if defined USE_NB_NEUL95
TASK_ITEM(proc_nbiot_task,      1,   					0, 			0x1000,												"nbiot_task", 		0)
TASK_ITEM(data_collection_task, 1,   		      0, 			0x1000,								      	"data_collection_task", 		0)
TASK_ITEM(nb_data_report,      1,   					0, 			0x1000,												"nb_data_report", 		0)
#endif

#endif