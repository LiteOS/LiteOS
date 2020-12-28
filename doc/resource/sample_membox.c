#include "los_config.h"
#include "los_membox.h"
#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID Example_StaticMem(VOID)
{
    UINT32 *mem = NULL;
    UINT32 blkSize = 10;
    UINT32 boxSize = 100;
    UINT32 boxMem[1000];
    UINT32 ret;

    ret = LOS_MemboxInit(&boxMem[0], boxSize, blkSize);
    if(ret != LOS_OK) {
        printf("内存池初始化失败!\n");
        return;
    } else {
        printf("内存池初始化成功!\n");
    }
    
    /*申请内存块*/
    mem = (UINT32 *)LOS_MemboxAlloc(boxMem);
    if (NULL == mem) {
        printf("内存分配失败!\n");
        return;
    }
    printf("内存分配成功\n");

    /*赋值*/
    *mem = 828;
    printf("*mem = %d\n", *mem);

     /*清除内存内容*/
     LOS_MemboxClr(boxMem, mem);
     printf("清除内存内容成功\n *mem = %d\n", *mem);

    /*释放内存*/
    ret = LOS_MemboxFree(boxMem, mem);
    if (LOS_OK == ret) {
        printf("内存释放成功!\n");
    } else {
        printf("内存释放失败!\n");
    }

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
