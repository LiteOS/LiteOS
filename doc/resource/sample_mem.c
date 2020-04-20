#include "los_config.h"
#include "los_memory.h"
#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define TEST_POOL_SIZE (20*1024*1024)
UINT8 m_aucTstPool[TEST_POOL_SIZE];

void * pool_ = &m_aucTstPool[0];
#define POOL_BASE_ADDR &m_aucTstPool[0]
#define RANDOM(x) (rand()%x)

#define USE_NEW_MEM
#ifdef USE_NEW_MEM
#define LOS_INIT_MEM LOS_DLnkInitMem
#define LOS_ALLOC_MEM LOS_DLnkAllocMem
#define LOS_REALLOC_MEM LOS_DLnkReAllocMem
#define LOS_FREE_MEM LOS_DLnkFreeMem

#define LOS_GET_MEM_USED_SIZE LOS_DLnkGetTotalMemUsed
#define LOS_GET_MEM_USED_BLKS LOS_DLnkGetMemUsedBlks
#define LOS_GET_MEM_FREE_BLKS  LOS_DLnkGetMemFreeBlks
#else
#define LOS_INIT_MEM LOS_SLnkInitMem
#define LOS_ALLOC_MEM LOS_SLnkAllocMem
#define LOS_REALLOC_MEM LOS_SLnkReAllocMem
#define LOS_FREE_MEM LOS_SLnkFreeMem

#define LOS_GET_MEM_USED_SIZE osGetUsedMem
#define LOS_GET_MEM_USED_BLKS osGetUsedBlks
#define LOS_GET_MEM_FREE_BLKS  osGetFreeBlks
#endif
UINT8 *m_aucSysMem_Tmp;
VOID los_memory_test() {
    UINT32 *p_num = NULL;
    UINT32 uwRet;
    uwRet = LOS_MemInit(m_aucSysMem_Tmp, 32);
    if (LOS_OK  == uwRet) {
        dprintf("内存池初始化成功!\n");
    }
    else {
        dprintf("内存池初始化失败!\n");
        return;
    }
    /*分配内存*/
    p_num = (int*)LOS_MemAlloc(m_aucSysMem_Tmp, 4);
    if (NULL == p_num) {
        dprintf("内存分配失败!\n");
        return;
    }
    dprintf("内存分配成功\n");
    /*赋值*/
    *p_num = 828;
    dprintf("*p_num = %d\n", *p_num);
    /*释放内存*/
    uwRet = LOS_MemFree(m_aucSysMem_Tmp, p_num);
    if (LOS_OK == uwRet) {
        dprintf("内存释放成功!\n");
    }
    else {
        dprintf("内存释放失败!\n");
    }
	return;
}

void It_mem_performance(void)
{
    log_memory_test();
	return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

