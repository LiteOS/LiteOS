#include <string.h>
#include "los_sys.h"
#include "los_task.ph"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"

#ifdef LOS_FLASH

#include "los_fs.h"
#include "los_api_fs.h"



/* Private value-----------------------------------------------*/
LOS_FRESULT res_flash; 
unsigned char buffer[1024]={0}; /* file copy buffer*/
unsigned char textFileBuffer[] = "LiteOS FatFS文件系统测试";

static UINT32 g_uwboadTaskID;

/* Private function prototypes -----------------------------------------------*/
static void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


void Example_FsTsk(void)
{
    int fnew;
    unsigned int br, bw; /* File R/W count */
    
    /************************* MCU flash FS *********************************************/
    //mount the file system
    res_flash = LOS_fmount("1:",1);
    LOS_EvbUartPrintf("\r\n f_mount res_flash = [%d] \r\n",res_flash);
    
    //If there is no file system,format to create the file system.
    if(res_flash == LOS_FS_NO_FILESYSTEM)
    {
        res_flash = LOS_fmkfs("1:",0,0);                    //format
        LOS_EvbUartPrintf("\r\n f_mkfs res_flash=%d",res_flash);
        res_flash = LOS_fmount("1:",0);                     //After formatting,cancel the mount first.
        res_flash = LOS_fmount("1:",1);                     //mount
        LOS_EvbUartPrintf("\r\n remount res_flash = [%d] \r\n",res_flash);
    }
    
    //File system test: written test. Open the file, if there is no file, create it.
    fnew = LOS_fopen("1:LiteOS.txt", LOS_FS_CREATE_ALWAYS | LOS_FS_WRITE );
    if(0 != fnew)
    {
        LOS_EvbUartPrintf("\r\nWrite = [%s] ", textFileBuffer);
        bw = LOS_fwrite(textFileBuffer,1, sizeof(textFileBuffer),fnew);
        if(0 == bw)
        {
            LOS_EvbUartPrintf("\r\f_write failed.");
            LOS_EvbLedControl(LOS_LED2, LED_ON);
            
            LOS_fclose(fnew);
            return;
        }   
        LOS_EvbUartPrintf("\r\nbw = [%d] ", bw);
        LOS_fclose(fnew);
    }
    else
    {
        LOS_EvbUartPrintf("\r\ncreate file failed.");
        LOS_EvbLedControl(LOS_LED2, LED_ON);
        return;
    }

    //File system test: read test.
    fnew = LOS_fopen("1:LiteOS.txt", LOS_FS_OPEN_EXISTING | LOS_FS_READ);
    if(0 == fnew)
    {
        LOS_EvbUartPrintf("\r\f_open failed.");
        LOS_EvbLedControl(LOS_LED2, LED_ON);
        return;
    }
    
    br = LOS_fread( buffer, 1 ,sizeof(buffer),fnew);
    
    if(0 != strncmp((char *)textFileBuffer,(char *)buffer,sizeof(textFileBuffer)))
    {
        LOS_EvbUartPrintf("\r\f_read failed.");
        LOS_EvbLedControl(LOS_LED2, LED_ON);
    }
    else
    {
        LOS_EvbLedControl(LOS_LED1, LED_ON);
        LOS_EvbUartPrintf("\r\nRead sucess buffer = [%s],br = [%d]", buffer,br);
    }
    
    /* Close open files */
    res_flash = LOS_fclose(fnew);
    
    //No longer use the file system, cancel the mounted file system.
    res_flash = LOS_fmount("1:",0);
    
    while(1)
    {
        LOS_EvbLedControl(LOS_LED3, LED_ON);
        Delay(0xffffff);
        LOS_EvbLedControl(LOS_LED3, LED_OFF);
        Delay(0xffffff);
    }
}


UINT32 Example_FsEntry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_FsTsk;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "FsDemo";
    stTaskInitParam.usTaskPrio = 30;
    uwRet = LOS_TaskCreate(&g_uwboadTaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }
    return LOS_OK;
}

#endif

