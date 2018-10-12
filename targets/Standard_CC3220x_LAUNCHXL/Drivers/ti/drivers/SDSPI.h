/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
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

#ifndef ti_drivers_SDSPI__include
#define ti_drivers_SDSPI__include

#ifdef __cplusplus
extern "C" {
#endif

#warning "ti/drivers/SDSPI is deprecated & not recommended for new designs. \
The SDSPILegacy module & macros below are only for compatibility with \
existing projects. A new SD driver implementation based on the SPI driver \
(ti/drivers/sd/SDSPI) is provided as a means to communicate with SD cards via \
a SPI peripheral.  The SDFatFs driver is the means of interfacing with a \
FatFs file system via a SD driver instance.  Projects should move to use the \
new drivers at their earliest convenience.  Refer to ti/drivers/SDFatFS.h & \
ti/drivers/SD.h for details."

/* Macros provided for backwards compatibility */
#define Board_initSDSPI      SDSPILegacy_init
#define SDSPI_close          SDSPILegacy_close
#define SDSPI_control        SDSPILegacy_control
#define SDSPI_init           SDSPILegacy_init
#define SDSPI_open           SDSPILegacy_open
#define SDSPI_Params_init    SDSPILegacy_Params_init

#include <ti/drivers/SDSPILegacy.h>

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_SDSPI__include */
