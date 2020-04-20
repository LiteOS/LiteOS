/*************************************************************************
	> File Name: it_Timer_example.c
	> Author: 
	> Mail: 
	> Created Time: Sat 25 Jul 2015 03:42:36 PM CST
 ************************************************************************/

#include<stdio.h>
#include"osTest.h"
#include "los_swtmr.h"
#include "time.h"
#include "los_sys.h"


void Timer1_Callback  (uint32_t arg);  // callback fuction 
                  
void Timer2_Callback	(uint32_t arg);//»Øµ÷º¯Êý


UINT32 g_timercount1 = 0;  
UINT32 g_timercount2 = 0; 


void Timer1_Callback(uint32_t arg)
{
  
  unsigned long tick_last1;   
  g_timercount1 ++;
  tick_last1=(UINT32)LOS_TickCountGet();
  dprintf("g_timercount1=%d\n",g_timercount1);
  dprintf("tick_last1=%d\n",tick_last1);
  
}

void Timer2_Callback(uint32_t arg)
{
  
  unsigned long tick_last2;  
  tick_last2=(UINT32)LOS_TickCountGet();
  g_timercount2 ++;
  dprintf("g_timercount2=%d\n",g_timercount2);
  dprintf("tick_last2=%d\n",tick_last2);
}




void Timer_example (void)  {                                                         
  UINT16 id1;  
  UINT16 id2;// timer id
  UINT32 uwTick;
 // uint32_t  timerDelay;   // timer value
  
  LOS_SwTmrCreate (1000, LOS_SWTMR_MODE_ONCE,Timer1_Callback,&id1,1);
  LOS_SwTmrCreate(100,LOS_SWTMR_MODE_PERIOD,Timer2_Callback,&id2,1);
  dprintf("create Timer1 success\n");
 // timerDelay = 100;  
  LOS_SwTmrStart (id1); 
  dprintf("start Timer1 sucess\n");
  LOS_TaskDelay(200);
  LOS_SwTmrTimeGet(id1,&uwTick);
  dprintf("uwTick =%d\n",uwTick);
  LOS_SwTmrStop(id1);
  dprintf("stop Timer1 sucess\n");
  LOS_SwTmrStart(id1);
  LOS_TaskDelay(1000);
  LOS_SwTmrDelete(id1);
  dprintf("delete Timer1 sucess\n");
  LOS_SwTmrStart(id2);
  dprintf("start Timer2\n");
  LOS_TaskDelay(1000);
  LOS_SwTmrStop(id2);
  LOS_SwTmrDelete(id2); 
  

  // Create periodic timer
  
  //......
}


