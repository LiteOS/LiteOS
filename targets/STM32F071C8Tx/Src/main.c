#include "los_inspect_entry.h"


int main(void)
{
    UINT32 uwRet = LOS_OK;


    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    LOS_Inspect_Entry();

    LOS_Start();
}
