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

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __ADAPTER_LAYER_H__
#define __ADAPTER_LAYER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include <stdlib.h>
#include "los_typedef.h"
#include "los_task.h"
#include "dtls_interface.h"
#include "fota_package_storage_device.h"

/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/

/*###################################   LiteOS    #######################################*/
/* interrupt */
extern UINTPTR LOS_IntLock(VOID);
extern VOID LOS_IntRestore(UINTPTR uvIntSave);
/* task */
extern UINT32 LOS_TaskCreate(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam);
extern UINT32 LOS_TaskDelay(UINT32 uwTick);
extern UINT32 LOS_TaskDelete(UINT32 uwTaskID);
/* semaphore */
extern UINT32 LOS_BinarySemCreate (UINT16 usCount, UINT32 *puwSemHandle);
extern UINT32 LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle);
extern UINT32 LOS_SemDelete(UINT32 uwSemHandle);
extern UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout);
extern UINT32 LOS_SemPost(UINT32 uwSemHandle);
/* memory */
extern void  *LOS_MemAlloc (VOID *pPool, UINT32  uwSize);
extern UINT32 LOS_MemFree(VOID *pPool, VOID *pMem);
/* mutex */
extern UINT32 LOS_MuxCreate (UINT32 *puwMuxHandle);
extern UINT32 LOS_MuxDelete(UINT32 uwMuxHandle);
extern UINT32 LOS_MuxPend(UINT32 uwMuxHandle, UINT32 uwTimeout);
extern UINT32 LOS_MuxPost(UINT32 uwMuxHandle);
/* queue */
extern UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                                    UINT32 uwFlags,UINT16 usMaxMsgSize );
extern UINT32 LOS_QueueDelete(UINT32 uwQueueID);
extern UINT32 LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                                    UINT32 * puwBufferSize, UINT32  uwTimeOut);
extern UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID, VOID * pBufferAddr,
                                    UINT32 uwBufferSize, UINT32 uwTimeOut );
/* tick */
extern UINT64 LOS_TickCountGet(VOID);

/*###################################   Agent Tiny   #######################################*/
extern void atiny_log(const char* fmt, ...);
extern int  atiny_random(void* output, size_t len);
extern void atiny_reboot(void);
extern void atiny_usleep(unsigned long usec);

/*###################################       AT      #######################################*/
extern void at_transmit(uint8_t * cmd, int32_t len,int flag);
extern void at_usart_deinit(void);
extern int32_t at_usart_init(void);
extern int read_resp(uint8_t * buf);

/*###################################     FOTA    #######################################*/
extern void hal_spi_flash_config(void);
extern int hal_spi_flash_read(void* buf, int32_t len, uint32_t location);
extern int hal_spi_flash_erase_write(const void* buf, int32_t len, uint32_t location);

/*###################################     DTLS    #######################################*/
extern int dtls_read(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len, uint32_t timeout);
extern int dtls_shakehand(mbedtls_ssl_context *ssl, const char *host, const char *port);
extern void dtls_ssl_destroy(mbedtls_ssl_context *ssl);
extern mbedtls_ssl_context *dtls_ssl_new_with_psk(char *psk, unsigned psk_len, char *psk_identity);
extern int dtls_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len);

/*###################################     LWIP    #######################################*/
extern int lwip_fcntl(int s, int cmd, int val);
extern int lwip_shutdown(int s, int how);
extern uint32_t sys_now(void);


/*###################################     OTHERS    #######################################*/
extern void delayus(uint32_t usec);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __ADAPTER_LAYER_H__ */

