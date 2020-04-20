#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "osTest.h"
#include "math.h"
#include "semaphore.h"
#include "pthread.h"
#include "time.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "codes.h"
#include "hal_platform_ints.h"
#include "los_task.h"

extern USER_ERR_FUNC_S      g_stUserErrFunc;
void *err_handler(CHAR *pcFileName,
                                     UINT32 uwLineNo,
                                     UINT32 uwErrorNo,
                                     UINT32 uwParaLen,
                                     VOID  *pPara)
{
    printf("err handel ok\n");
}
UINT32 Example_ErrCaseEntry(VOID)
{
    LOS_ErrHandle(NULL, 0,0,0, NULL);
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
