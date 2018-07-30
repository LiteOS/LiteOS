/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
 
/* Includes -----------------------------------------------------------------*/
#include "adapter_layer.h"
#include "unistd.h"
#include <semaphore.h>

/* Defines ------------------------------------------------------------------*/
#ifdef OS_SYS_MEM_SIZE
#undef OS_SYS_MEM_SIZE
#endif
#define OS_SYS_MEM_SIZE     0x0024000          // size 200k

//for lwip defined
#define F_GETFL 3

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
UINT32  g_semCrt = -1;
UINT32  g_vuwIntCount = 0;
UINT64  g_ullTickCount = 0;
UINT8   sysMem0[OS_SYS_MEM_SIZE];
UINT8   *m_aucSysMem0 = sysMem0; 
void    *g_pstAllSem;

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/

/*###################################   LiteOS    #######################################*/

/************************************* interrupt ***********************************/
UINTPTR LOS_IntLock(VOID)
{
    return 0;
}
VOID LOS_IntRestore(UINTPTR uvIntSave)
{
    return;
}

/************************************* task ***********************************/
UINT32 LOS_TaskCreate(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam)
{
    return 0;
}

UINT32 LOS_TaskDelay(UINT32 uwTick)
{
    return 0;
}

UINT32 LOS_TaskDelete(UINT32 uwTaskID)
{
    return 0;
}

/************************************* semaphore ***********************************/
UINT32 LOS_BinarySemCreate (UINT16 usCount, UINT32 *puwSemHandle)
{
    return g_semCrt;
}

UINT32 LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle)
{   
    return 0;
}

UINT32 LOS_SemDelete(UINT32 uwSemHandle)
{
    return 0;
}
UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout)
{
    usleep(1000);
    return 0;
}
UINT32 LOS_SemPost(UINT32 uwSemHandle)
{
    return 0;
}

/************************************* memory ***********************************/
void *LOS_MemAlloc (VOID *pPool, UINT32  uwSize)
{
    return malloc(uwSize);
}
UINT32 LOS_MemFree(VOID *pPool, VOID *pMem)
{
    free(pMem);
    return 0;
}

/************************************* mutex ***********************************/
UINT32 LOS_MuxPend(UINT32 uwMuxHandle, UINT32 uwTimeout)
{
    return 0;
}

UINT32 LOS_MuxPost(UINT32 uwMuxHandle)
{
    return 0;
}

UINT32  LOS_MuxCreate (UINT32 *puwMuxHandle)
{
    return 0;
}

UINT32 LOS_MuxDelete(UINT32 uwMuxHandle)
{
    return 0;
}

/************************************* mutex ***********************************/
UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                            UINT32 uwFlags,UINT16 usMaxMsgSize )
{
    return 0;
}

UINT32 LOS_QueueReadCopy(UINT32  uwQueueID,VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut)
{
    return 0;
}

UINT32 LOS_QueueDelete(UINT32 uwQueueID)
{
    return 0;
}

UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID,VOID * pBufferAddr,
                             UINT32 uwBufferSize, UINT32 uwTimeOut )
{
    return 0;
}

/************************************* tick ***********************************/
UINT64 LOS_TickCountGet (VOID)
{
    return 0;
}

/*###################################   Agent Tiny   #######################################*/

void atiny_log(const char* fmt, ...)
{
//    printf("%s",fmt);
    return;
}

void atiny_usleep(unsigned long usec)
{
    usleep(usec);
}

int atiny_random(void* output, size_t len)
{
    return 1;
}

void atiny_reboot(void)
{

}

/*###################################       AT      #######################################*/

void at_transmit(uint8_t * cmd, int32_t len,int flag)
{
}

int32_t at_usart_init(void)
{
    return 0;
}

void at_usart_deinit(void)
{

}

int read_resp(uint8_t * buf)
{    
    return 0;
}

/*###################################     FOTA    #######################################*/

void hal_spi_flash_config(void)
{
    return 0;
}
int hal_spi_flash_read(void* buf, int32_t len, uint32_t location)
{
    return 0;
}
int hal_spi_flash_erase_write(const void* buf, int32_t len, uint32_t location)
{
    return 0;
}

/*###################################     DTLS    #######################################*/

mbedtls_ssl_context *dtls_ssl_new_with_psk(char *psk, unsigned psk_len, char *psk_identity)
{
    return NULL;
}
int dtls_shakehand(mbedtls_ssl_context *ssl, const char *host, const char *port)
{
    return 0;
}
void dtls_ssl_destroy(mbedtls_ssl_context *ssl)
{
    return;
}
int dtls_read(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len, uint32_t timeout)
{
    return 0;
}
int dtls_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len)
{
    return 0;
}

/*###################################     LWIP    #######################################*/

int lwip_fcntl(int s, int cmd, int val)
{
    /*
     * Never return 'WOULD BLOCK' on a non-blocking socket
     */
    int ret = -1;
    switch (cmd)
    {
    case F_GETFL:
        {
            ret = 0;
        }
        break;
    default:
        break;
    }
    return ret;
}

int  lwip_shutdown(int s, int how)
{
    return -1;
}

//int lwip_close(int s)
//{
//    return -1;
//}

uint32_t sys_now(void)
{
    static int i = 0;
    return i++;
}


/*###################################     OTHERS    #######################################*/

void delayus(uint32_t usec)
{
    usleep(usec);
}


/* Private functions --------------------------------------------------------*/

