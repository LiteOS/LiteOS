/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __FSL_DSPI_FREERTOS_H__
#define __FSL_DSPI_FREERTOS_H__

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "portable.h"
#include "semphr.h"

#include "fsl_dspi.h"

/*!
 * @addtogroup dspi_freertos_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
* @cond RTOS_PRIVATE
* @brief DSPI FreeRTOS handle
*/
typedef struct _dspi_rtos_handle
{
    SPI_Type *base;                  /*!< DSPI base address */
    dspi_master_handle_t drv_handle; /*!< Handle of the underlying driver, treated as opaque by the RTOS layer */
    status_t async_status;           /*!< Transactional state of the underlying driver */
    SemaphoreHandle_t mutex;         /*!< Mutex to lock the handle during a transfer */
    SemaphoreHandle_t event;         /*!< Semaphore to notify and unblock a task when a transfer ends */
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    StaticSemaphore_t mutexBuffer;     /*!< Statically allocated memory for mutex */
    StaticSemaphore_t semaphoreBuffer; /*!< Statically allocated memory for event */
#endif
} dspi_rtos_handle_t;
/*! \endcond */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name DSPI RTOS Operation
 * @{
 */

/*!
 * @brief Initializes the DSPI.
 *
 * This function initializes the DSPI module and the related RTOS context.
 *
 * @param handle The RTOS DSPI handle, the pointer to an allocated space for RTOS context.
 * @param base The pointer base address of the DSPI instance to initialize.
 * @param masterConfig A configuration structure to set-up the DSPI in master mode.
 * @param srcClock_Hz A frequency of the input clock of the DSPI module.
 * @return status of the operation.
 */
status_t DSPI_RTOS_Init(dspi_rtos_handle_t *handle,
                        SPI_Type *base,
                        const dspi_master_config_t *masterConfig,
                        uint32_t srcClock_Hz);

/*!
 * @brief Deinitializes the DSPI.
 *
 * This function deinitializes the DSPI module and the related RTOS context.
 *
 * @param handle The RTOS DSPI handle.
 */
status_t DSPI_RTOS_Deinit(dspi_rtos_handle_t *handle);

/*!
 * @brief Performs the SPI transfer.
 *
 * This function performs the SPI transfer according to the data given in the transfer structure.
 *
 * @param handle The RTOS DSPI handle.
 * @param transfer A structure specifying the transfer parameters.
 * @return status of the operation.
 */
status_t DSPI_RTOS_Transfer(dspi_rtos_handle_t *handle, dspi_transfer_t *transfer);

/*!
 * @}
 */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* __FSL_DSPI_FREERTOS_H__ */
