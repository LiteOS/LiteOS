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
#ifndef __OTA_USER_H__
#define __OTA_USER_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/ota/ota.h>

//#define SL_ENABLE_OTA_DEBUG_TRACES /* uncomment to enable internal OTA debug info */
#ifndef SL_ENABLE_OTA_DEBUG_TRACES
#define _SlOtaLibTrace(pargs) 
#else

#ifdef _WIN32

#define _SlOtaLibTrace(pargs)  printf pargs

#elif __MSP432P401R__

extern void LogMessage(const char *pcFormat, ...);
#define _SlOtaLibTrace(pargs)  LogMessage pargs

#else /*CC32xx*/

extern int Report(const char *format, ...);
#define _SlOtaLibTrace(pargs)  Report pargs

#endif

#endif

#define OTA_SERVER_GITHUB      1
#define OTA_SERVER_DROPBOX_V2  2
#define OTA_SERVER_CUSTOM      99

/* USER SHOULD DEFINE HERE WHICH CLOUD TO USE */
/* -------------------------------------------*/
//#define OTA_SERVER_TYPE    OTA_SERVER_GITHUB
#define OTA_SERVER_TYPE    OTA_SERVER_DROPBOX_V2

/* OTA server info */
/* --------------- */

#ifdef _WIN32
#define OTA_VENDOR_DIR  "OTA_CC3120"
#elif __MSP432P401R__
#define OTA_VENDOR_DIR  "OTA_CC3120"
#else
#define OTA_VENDOR_DIR  "OTA_CC3220SF"
#endif

#if OTA_SERVER_TYPE == OTA_SERVER_GITHUB

/* Github server info */
#define OTA_SERVER_NAME                 "api.github.com"
#define OTA_SERVER_IP_ADDRESS           0x00000000
#define OTA_SERVER_SECURED              1

/* Github vendor info */
#define OTA_VENDOR_ROOT_DIR             "/repos/<user account>/<user directory>"
//#define OTA_VENDOR_TOKEN              "<User defined Github token>"
#ifndef OTA_VENDOR_TOKEN
#error "Please define your personal cloud account token in OTA_VENDOR_TOKEN above"
#endif

#define OTA_SERVER_ROOT_CA_CERT         "DigCert_High_Assurance_CA.der"
#define OTA_SERVER_AUTH_IGNORE_DATA_TIME_ERROR
#define OTA_SERVER_AUTH_DISABLE_CERT_STORE

#elif OTA_SERVER_TYPE == OTA_SERVER_DROPBOX_V2

/* Dropbox V2 server info */
#define OTA_SERVER_NAME                 "api.dropboxapi.com"
#define OTA_SERVER_IP_ADDRESS           0x00000000
#define OTA_SERVER_SECURED              1

/* Dropbox V2 vendor info */
//#define OTA_VENDOR_TOKEN              "<User defined Dropbox token>"
#ifndef OTA_VENDOR_TOKEN
#error "Please define your personal cloud account token in OTA_VENDOR_TOKEN above"
#endif

#define OTA_SERVER_ROOT_CA_CERT         "DigCert_High_Assurance_CA.der"
#define OTA_SERVER_AUTH_IGNORE_DATA_TIME_ERROR
#define OTA_SERVER_AUTH_DISABLE_CERT_STORE

#elif OTA_SERVER_TYPE == OTA_SERVER_CUSTOM

/* add your customer server header here */
#include ""

/* Github server info */
#define OTA_SERVER_NAME                 ""
#define OTA_SERVER_IP_ADDRESS           0x00000000
#define OTA_SERVER_SECURED              1

/* Vendor info */
#define OTA_VENDOR_TOKEN                "User defined token"

#define CdnVendor_SendReqDir           
#define CdnVendor_ParseRespDir          
#define CdnVendor_SendReqFileUrl        
#define CdnVendor_ParseRespFileUrl      
#define CdnVendor_SendReqFileContent    

#endif


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __OTA_USER_H__ */
