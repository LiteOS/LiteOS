#include "los_base.h"

#include "uart.h"
#include "los_printf_entry.h"


int main(void)
{
    MX_USART2_USART_Init();
    UINT32 uwRet = LOS_OK;


    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    LOS_Printf_Entry();

    LOS_Start();
}
