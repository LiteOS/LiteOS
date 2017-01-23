#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"
#include "los_compiler.h"


/*****************************************************************************
 Function    : main
 Description : Main function entry
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT
int main(void)
{
    UINT32 uwRet;
    uwRet = osMain();
    if (uwRet != LOS_OK) {
        return LOS_NOK;
    }
		/* config system clock and start system tick irq */
    uwRet = osTickStart();
    if (uwRet != LOS_OK)
    {
        PRINT_ERR("osTickStart error\n");
        return uwRet;
    }
		
		/* Kernel start to run */
    LOS_Start();
    for (;;);
    /* Replace the dots (...) with your own code.  */
}
