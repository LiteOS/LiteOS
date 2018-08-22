/*
 * Copyright (c) 2017 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== PTLS.c ========
 *  POSIX Task Local Storage
 */

#include <FreeRTOS.h>  /* defines PTLS_TLS_INDEX via FreeRTOSConfig.h */
#include <task.h>

#include "PTLS.h"


/*
 *  ======== PTLS_getBuf ========
 */
void *PTLS_getBuf(void)
{
    TaskHandle_t task;
    void *buf;

    task = xTaskGetCurrentTaskHandle();
    buf = pvTaskGetThreadLocalStoragePointer(task, PTLS_TLS_INDEX);

    return (buf);
}

/*
 *  ======== PTLS_setBuf ========
 */
void PTLS_setBuf(void *buf)
{
    TaskHandle_t task;

    task = xTaskGetCurrentTaskHandle();
    vTaskSetThreadLocalStoragePointer(task, PTLS_TLS_INDEX, buf);
}

/*
 *  ======== PTLS_taskDeleteHook ========
 *  Called from FreeRTOS kernel via traceTASK_DELETE hook
 */
void PTLS_taskDeleteHook(void *task)
{
    void *buf;

    buf = pvTaskGetThreadLocalStoragePointer(task, PTLS_TLS_INDEX);

    if (buf != NULL) {
        vPortFree(buf);
    }
}
