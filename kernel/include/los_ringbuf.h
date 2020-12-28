/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: Ring Buffer Public HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-10-10
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
 * --------------------------------------------------------------------------- */

#ifndef _LOS_RINGBUF_H
#define _LOS_RINGBUF_H

#include "los_typedef.h"
#include "los_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_ringbuf
 * Ringbuf Status.
 */
typedef enum {
    RBUF_UNINIT,          /**< Ringbuf is not inited. */
    RBUF_INITED           /**< Ringbuf is inited. */
} RingbufStatus;

/**
 * @ingroup los_ringbuf
 * Ringbuf information structure.
 *
 */
typedef struct {
    UINT32 startIdx;      /**< Ringbuf read index */
    UINT32 endIdx;        /**< Ringbuf write index */
    UINT32 size;          /**< Ringbuf total size */
    UINT32 remain;        /**< Ringbuf free size */
    SPIN_LOCK_S lock;     /**< Lock for read and write */
    RingbufStatus status; /**< Ringbuf status */
    CHAR *fifo;           /**< Buf to store data */
} Ringbuf;

/**
 * @ingroup los_ringbuf
 * @brief Init a ringbuf.
 *
 * @par Description:
 * This API is used to init a ringbuf.
 * @attention
 * The size must not be bigger than the fifo's actual size.
 *
 * @param  ringbuf        [OUT] Ringbuf control block.
 * @param  fifo           [IN] Data buf address.
 * @param  size           [IN] Data buf size.
 *
 * @retval #LOS_NOK       Init failed, check the legality of function parameters.
 * @retval #LOS_OK        Init success.
 *
 * @par Dependency:
 * <ul><li>los_ringbuf.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_RingbufInit
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_RingbufInit(Ringbuf *ringbuf, CHAR *fifo, UINT32 size);

/**
 * @ingroup los_ringbuf
 * @brief Reset a ringbuf.
 *
 * @par Description:
 * This API is used to reset a ringbuf to the init status.
 * @attention
 * The specific ringbuf must be inited first.
 *
 * @param  ringbuf        [IN] Ringbuf created by LOS_RingbufInit.
 *
 * @retval #None.
 *
 * @par Dependency:
 * <ul><li>los_ringbuf.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_RingbufReset
 * @since Huawei LiteOS V200R005C00
 */
extern VOID LOS_RingbufReset(Ringbuf *ringbuf);

/**
 * @ingroup los_ringbuf
 * @brief Write data to ringbuf.
 *
 * @par Description:
 * This API is used to write data to ringbuf.
 * @attention
 * The specific ringbuf must be inited first.
 *
 * @param  ringbuf        [IN] The ringbuf write data to.
 * @param  buf            [IN] The source buf address.
 * @param  size           [IN] The source buf size.
 *
 * @retval #UINT32        The actual written size.
 *
 * @par Dependency:
 * <ul><li>los_ringbuf.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_RingbufWrite
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_RingbufWrite(Ringbuf *ringbuf, const CHAR *buf, UINT32 size);

/**
 * @ingroup los_ringbuf
 * @brief Read data from ringbuf.
 *
 * @par Description:
 * This API is used to get data from ringbuf.
 * @attention
 * The specific ringbuf must be inited first.
 *
 * @param  ringbuf        [IN] The ringbuf read data from.
 * @param  buf            [OUT] The dest buf address.
 * @param  size           [IN] The dest buf size.
 *
 * @retval #UINT32        The actual read size.
 *
 * @par Dependency:
 * <ul><li>los_ring.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_RingbufRead
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_RingbufRead(Ringbuf *ringbuf, CHAR *buf, UINT32 size);

/**
 * @ingroup los_ringbuf
 * @brief Get a ringbuf's used size.
 *
 * @par Description:
 * This API is used to get a ringbuf's used size.
 * @attention
 * The specific ringbuf must be inited first.
 *
 * @param  ringbuf        [IN] The ringbuf address
 *
 * @retval #UINT32        The used size of ringbuf.
 *
 * @par Dependency:
 * <ul><li>los_ringbuf.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_RingbufUsedSize
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_RingbufUsedSize(Ringbuf *ringbuf);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_RINGBUF_H */
