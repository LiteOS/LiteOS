/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
#ifndef __CDN_VENDORS_H__
#define __CDN_VENDORS_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include <ti/net/ota/otauser.h>

/* OTA server info */
/* --------------- */
#if OTA_SERVER_TYPE == OTA_SERVER_GITHUB

#include <ti/net/ota/source/CdnVendors/CdnGithub.h>

#define CdnVendor_SendReqDir            CdnGithub_SendReqDir
#define CdnVendor_ParseRespDir          CdnGithub_ParseRespDir
#define CdnVendor_SendReqFileUrl        CdnGithub_SendReqFileUrl
#define CdnVendor_ParseRespFileUrl      CdnGithub_ParseRespFileUrl
#define CdnVendor_SendReqFileContent    CdnGithub_SendReqFileContent

#elif OTA_SERVER_TYPE == OTA_SERVER_DROPBOX_V2

#include <ti/net/ota/source/CdnVendors/CdnDropboxV2.h>

#define CdnVendor_SendReqDir            CdnDropboxV2_SendReqDir
#define CdnVendor_ParseRespDir          CdnDropboxV2_ParseRespDir
#define CdnVendor_SendReqFileUrl        CdnDropboxV2_SendReqFileUrl
#define CdnVendor_ParseRespFileUrl      CdnDropboxV2_ParseRespFileUrl
#define CdnVendor_SendReqFileContent    CdnDropboxV2_SendReqFileContent

#endif


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __CDN_VENDORS_H__ */
