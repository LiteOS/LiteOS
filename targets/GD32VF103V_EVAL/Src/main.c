#include <stdio.h>
#include "gd32vf103.h"
#include "gd32v103v_eval.h"
#include "gd32v103v_lcd_eval.h"
#include "touch_panel.h"
#include "picture.h"
#include "gd25qxx.h"
#include "systick.h"

#include "los_base.h"
#include "los_typedef.h"
#include "los_tick.h"

#include "mem.h"

#include "osal.h"

#if defined (__CC_ARM)
extern char __heap_start__ [];
#elif defined (__GNUC__)
extern char __los_heap_addr_start__ [];
extern char __los_heap_addr_end__ [];
#else
#error "fix me"
#endif

extern VOID LOS_MemInfo (bool bShowDetail);
extern int link_main(void *args);


const struct phys_mem system_phys_mem [] =
    {
#if defined (__CC_ARM)
        { __heap_start__, (char *) 0x2002FC00, },
#elif defined (__GNUC__)
        {(unsigned long)__los_heap_addr_start__, (unsigned long)__los_heap_addr_end__, },
#else
#error "fix me"
#endif
        { 0, 0 }
    };

extern int lcd_demo();
static UINT32 g_TaskID1, g_TaskID2, g_TaskID3;


LITE_OS_SEC_TEXT VOID TEST1(VOID)
{
    while (1)
    {
        printf("test1\n");
        LOS_TaskDelay(10);
    }
}

LITE_OS_SEC_TEXT VOID TEST2(VOID)
{
    while (1)
    {
        printf("test2\n");
        LOS_TaskDelay(20);
    }
}

LITE_OS_SEC_TEXT VOID TEST3(VOID)
{
    while (1)
    {
        printf("test3\n");
        LOS_TaskDelay(30);
    }
}

void LOS_BoadExampleEntry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TEST1;
    stTaskInitParam.uwStackSize = 0x130;
    stTaskInitParam.pcName = "Test1";
    stTaskInitParam.usTaskPrio = 2;
    uwRet = LOS_TaskCreate(&g_TaskID1, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TEST2;
    stTaskInitParam.uwStackSize = 0x130;
    stTaskInitParam.pcName = "Test2";
    stTaskInitParam.usTaskPrio = 2;
    uwRet = LOS_TaskCreate(&g_TaskID2, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TEST3;
    stTaskInitParam.uwStackSize = 0x130;
    stTaskInitParam.pcName = "Test3";
    stTaskInitParam.usTaskPrio = 2;
    uwRet = LOS_TaskCreate(&g_TaskID3, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return;
    }

    return;
}


static int link_test()
{
    int ret = -1;
    UINT32 uwRet = LOS_OK;
    UINT32  handle;
    TSK_INIT_PARAM_S task_init_param;

    memset (&task_init_param, 0, sizeof (TSK_INIT_PARAM_S));
    task_init_param.uwArg = (unsigned int)NULL;
    task_init_param.usTaskPrio = 2;
    task_init_param.pcName =(char *) "link_main";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)link_main;
    task_init_param.uwStackSize = 0x1000;
    uwRet = LOS_TaskCreate(&handle, &task_init_param);
    if(LOS_OK == uwRet){
        ret = 0;
    }
    return ret;
}


int main() {
    /* configure EVAL_COM0 */
    gd_eval_com_init(EVAL_COM0);
    /* output a message on hyperterminal using printf function */
    printf("0x%x---0x%x\r\n", __los_heap_addr_start__, __los_heap_addr_end__ );

    UINT32 uwRet = LOS_OK;


    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }
    //LOS_MemInfo(1);
    //LOS_BoadExampleEntry();
    link_test();
    lcd_demo();

    (void)LOS_Start();
    return 0;

}
