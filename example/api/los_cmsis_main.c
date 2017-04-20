#include "cmsis_os.h"

#define TASK_CMSIS_STACK_SIZE_TEST 0x200

void Cmsis_Task_Test(void const *argument)
{
    uint32_t count=0;;
    while(1)
    {
        count++;
        osDelay(200);
    }
}

osThreadDef (Cmsis_Task_Test, osPriorityNormal, 1, TASK_CMSIS_STACK_SIZE_TEST);
int32_t Create_Cmsis_Task(void)
{
    osThreadId id;
    id = osThreadCreate (osThread (Cmsis_Task_Test), NULL);
    if (NULL == id)
    {
        return -1;
    }
    return 0;
}


#if 0
int main()
{
    int32_t uwRet = 0;
    
    /* Init LiteOS kernel */
    osKernelInitialize();
    
    /* create a test task */
    uwRet = Create_Cmsis_Task();
    if (uwRet)
    {
        return -1;
    }
    
    /* start to run LiteOS kernel */
    osKernelStart();
    
    return 0;
}
#endif
