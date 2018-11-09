#include "los_base.h"
#include "los_typedef.h"
#include "los_task.h"
#include "osport.h"
#include "shell.h"
#include <stdio.h>
#include <unistd.h>

UINT32 mytaskA(void *arg)
{
    char a[]="A";
    while (1) {
        LOS_TaskDelay(10000);
        write(STDIN_FILENO, a, 1);
    };
    return 0;
}

UINT32 mytaskB(void *arg)
{
    char b[]="B";
    LOS_TaskDelay(5000);
    while (1) {
       LOS_TaskDelay(10000);
       write(STDIN_FILENO, b, 1);
    };
    return 0;
}

int main()
{
    UINT32 uwRet = LOS_OK;
    //HardWare_init();
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
        return LOS_NOK;

    shell_install(20);

    task_create("mytaskA", mytaskA, 0, NULL, NULL, 10);
    task_create("mytaskB", mytaskB, 0, NULL, NULL, 10);
    
    LOS_Start();
    return 0;
}
