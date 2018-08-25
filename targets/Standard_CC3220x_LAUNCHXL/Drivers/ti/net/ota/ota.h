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
#ifndef __OTA_H__
#define __OTA_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define OTA_LIB_VERSION    "OTA_LIB_2.0.0.7"

/* Configurable numbers */
#define MAX_CONSECUTIVE_OTA_ERRORS           5


/* !Not Configurable numbers */
#define MAX_URL_SIZE            512
#define NET_BUF_SIZE	        1440

/* check in sl_OtaInit if this buffer is enough for OTA lib */
#define OTA_BLOCK_SIZE 7800

typedef struct
{
	uint8_t Buff[OTA_BLOCK_SIZE];
}OTA_memBlock;

typedef enum
{
    OTA_RUN_NON_BLOCKING/* not supported , OTA_RUN_BLOCKING */
} OTA_runningMode;

typedef void (*OTA_eventHandler) (void *);

#define MAX_SERVER_NAME         64
#define MAX_VENDOR_TOKEN_SIZE   96
#define VERSION_STR_SIZE        14      /* sizeof "YYYYMMDDHHMMSS"    */

/******************************************************************************
 * Server info Structure
******************************************************************************/
typedef struct  
{
    uint32_t  IpAddress;                            /* 0x0 – no, ip address, use server name */
    uint32_t  SecuredConnection;                    /* 1 - secured, use port 443  0 - not secured, use port 80 */
    uint8_t   ServerName[MAX_SERVER_NAME];
    uint8_t   VendorToken[MAX_VENDOR_TOKEN_SIZE];
} Ota_optServerInfo;

typedef struct  
{
	uint8_t   CurrentVersion[VERSION_STR_SIZE+1];
	uint8_t   NewVersion[VERSION_STR_SIZE+1];
} Ota_optVersionsInfo;

/******************************************************************************
 * Enumerations
******************************************************************************/
typedef enum
{
    EXTLIB_OTA_SET_OPT_SERVER_INFO = 0,  /* see Ota_optServerInfo   */
    EXTLIB_OTA_SET_OPT_VENDOR_ID,                    
    EXTLIB_OTA_SET_OPT_ACCEPT_UPDATE,
    EXTLIB_OTA_SET_OPT_DECLINE_UPDATE,
    EXTLIB_OTA_SET_OPT_IMAGE_COMMIT,
    EXTLIB_OTA_SET_OPT_IMAGE_ROLLBACK,
    EXTLIB_OTA_GET_OPT_IS_ACTIVE,  
    EXTLIB_OTA_GET_OPT_VERSIONS,  
    EXTLIB_OTA_GET_OPT_IS_PENDING_COMMIT
} OTA_option;

/******************************************************************************
 * Macros
******************************************************************************/
/* sl_OtaRun error codes */
#define OTA_RUN_STATUS_DOWNLOAD_DONE                                (104L)
#define OTA_RUN_STATUS_CHECK_OLDER_VERSION                          (103L)
#define OTA_RUN_STATUS_CHECK_NEWER_VERSION                          (102L)
#define OTA_RUN_STATUS_NO_UPDATES                                   (101L)
#define OTA_RUN_STATUS_OK                                           (0L)

#define OTA_RUN_STATUS_CONTINUE                                     (+20001L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_CONNECT_OTA_SERVER   (+20002L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_RECV_APPEND          (+20003L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_REQ_OTA_DIR          (+20004L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_REQ_FILE_URL         (+20005L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_CONNECT_FILE_SERVER  (+20006L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_REQ_FILE_CONTENT     (+20007L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_FILE_HDR             (+20008L)
#define OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_DOWNLOAD_AND_SAVE    (+20009L)

#define OTA_RUN_ERROR_NO_SERVER_NO_VENDOR                           (-21001L)
#define OTA_RUN_ERROR_UNEXPECTED_STATE                              (-21002L)
#define OTA_RUN_ERROR_CONSECUTIVE_OTA_ERRORS                        (-21003L)
#define OTA_RUN_ERROR_SECURITY_ALERT                                (-21004L)

/* sl_OtaGet error codes */
#define OTA_STATUS_OK                                        (0L)
#define OTA_OPT_ERROR_WRONG_STATE                       (-20601L)
#define OTA_OPT_ERROR_COMMIT                            (-20602L)
#define OTA_OPT_ERROR_ROLLBACK                          (-20603L)
#define OTA_OPT_ERROR_OPTION_CODE                       (-20604L)
#define OTA_OPT_ERROR_VENDOR_DIR_SIZE                   (-20605L)
#define OTA_INIT_ERROR                                  (-20701L)

/*****************************************************************************
 *   API Prototypes
 *****************************************************************************/

/*!

    \addtogroup OTA
    @{

*/

/*!
    \brief Initialize the OTA application

    \param[in] runningMode		Supported only OTA_RUN_NON_BLOCKING
    \param[in] pMemBlock		Is the OTA library control block allocated by the application
    \param[in] eventHandler		Callback to the app, currently not supported, need to be NULL

	This function initializes the OTA application and modules. The size of the control block parameter
	\e should be enough for OTA init (currently 7800 bytes) otherwize the init will failed

    \return Return zero ot -ve otherwise.

    -\b OTA_INIT_ERROR    - when the size of the OTA control block OtaCB_t is not enough for the OTA lib \n

    \sa                       
    \warning
    \par                 Example:  
    \code                
    For example: To initialize OTA from host invoke
    
    OTA_memBlock otaMemBlock;
    int16_t Status;
    Status = OTA_init(OTA_RUN_NON_BLOCKING, &otaMemBlock, NULL);
    
    \endcode
*/
int16_t OTA_init(OTA_runningMode runningMode, OTA_memBlock* pMemBlock, OTA_eventHandler eventHandler);

/*!
    \brief Run the OTA App state machine

    \param[in] pOtaLib     Pointer to OTA application pointer
    

    Run one step from the OTA application state machine. Host should repeat
	calling this function and examine the return value in order to check if OTA
	completed or got error or just need to be continued. This pattern is useful
	in host with NON-OS. In host with OS, host should start OTA task and
	continuously calling this function till OTA completed.

    \return Return zero or +ve bitmap number on success, -ve otherwise.

    -\b OTA_RUN_STATUS_CONTINUE    - Host should continue calling sl_OtaRun \n
    -\b OTA_RUN_STATUS_CONTINUE_WARNING_...  - group of OTA WARNINGS, actually those are errors but 
    the OTA will retry the process 5 times so user should continue run the OTA \n
    -\b OTA_RUN_STATUS_NO_UPDATES  - No updates for now, host can retry after another period of time\n
    -\b OTA_RUN_STATUS_CHECK_NEWER_VERSION  - OTA found newer update version, user can accept the new version 
    by calling  sl_OtaSet with option _ ACCEPT_UPDATE , or just continue running, or do another check before continue \n
    -\b OTA_RUN_STATUS_CHECK_OLDER_VERSION  - OTA found older update version, user can stop the OTA by calling 
    sl_OtaSet with option _DECLINE_UPDATE , or continue running, or do another check before continue \n
    -\b OTA_RUN_STATUS_DOWNLOAD_DONE - Current OTA update completed,
	host should move the image to BUNDLE_STATE_TESTING by calling sl_Stop and then ro reset the MCU \n
    -\b OTA_RUN_ERROR_... - group of OTA ERRORS, user should stop the OTA process
    -\b OTA_RUN_ERROR_CONSECUTIVE_OTA_ERRORS - OTA process failed 5 consecutive times
    -\b OTA_RUN_ERROR_NO_SERVER_NO_VENDOR - user didn't call sl_OtaSet with EXTLIB_OTA_OPT_VENDOR_ID
    and EXTLIB_OTA_OPT_SERVER_INFO  before running the OTA
    -\b OTA_RUN_ERROR_SECURITY_ALERT - security alert from file system, user must stop trying downloading current OTA update
    -\b OTA_RUN_ERROR_UNEXPECTED_STATE - OTA state error, must init the OTA before continue

    \sa                       
    \note                     
    \warning
    \par                 Example:  
    \code                
    For example: Run OTA from host   
    
    Status = Ota_run();
    
    \endcode
*/
int16_t OTA_run();

/*!
    \brief Set OTA command/parameter

    \param[in] pOtaLib     OTA control block pointer
    \param[in] Option       Select the option
    \param[in] OptionLen    Option structure length
    \param[in] pOptionVal   pointer to the option structure

	This function sets OTA command/parameter. The parameter \e Option can be
	one of the following:

	\b EXTLIB_OTA_SET_OPT_SERVER_INFO - Set the Server information \n
	\b EXTLIB_OTA_SET_OPT_VENDOR_ID   - Set the Vendor DIR string \n
	\b EXTLIB_OTA_SET_OPT_ACCEPT_UPDATE   - Accept the new image \n
	\b EXTLIB_OTA_SET_OPT_DECLINE_UPDATE   - Decline the new image \n
	\b EXTLIB_OTA_SET_OPT_IMAGE_COMMIT   - Commit the currnet image in testing mode \n
	\b EXTLIB_OTA_SET_OPT_IMAGE_ROLLBACK   - decline the currnet image in testing mode and rollback to the previous image \n

    \return         On success, zero is returned. On error, -1 is returned

    \sa                       
    \note                     
    \warning
    \par                 Example:  
    \code                
    For example: Set OTA server info from host   
    
    Ota_optServerInfo g_otaOptServerInfo;

    g_otaOptServerInfo.IpAddress         = OTA_SERVER_IP_ADDRESS;
    g_otaOptServerInfo.SecuredConnection = OTA_SERVER_SECURED;
    strcpy((char *)g_otaOptServerInfo.ServerName,  OTA_SERVER_NAME);
    strcpy((char *)g_otaOptServerInfo.VendorToken, OTA_VENDOR_TOKEN);
    Status = Ota_set(EXTLIB_OTA_SET_OPT_SERVER_INFO, sizeof(g_otaOptServerInfo), (uint8_t *)&g_otaOptServerInfo);

    Ota_set(EXTLIB_OTA_SET_OPT_SERVER_INFO, sizeof(g_otaOptServerInfo), (uint8_t *)&g_otaOptServerInfo, 0);
    
    \endcode
*/
int16_t OTA_set(OTA_option option, int32_t optionLen, uint8_t *pOptionVal, int32_t flags);

/*!
    \brief Get the current OTA status

    
    \param[in] pOtaLib     OTA control block pointer
    \param[in] Option       Selects the option
    \param[in] OptionLen    option structure length
    \param[in] pOptionVal   pointer to the option structure

	This function gets the current OTA of active or Idle.
	The parameter \e option, could be one of the following:

    \b EXTLIB_OTA_GET_OPT_IS_ACTIVE - Check if OTA process is active or idle \n
    \b EXTLIB_OTA_GET_OPT_VERSIONS - return the current OTA version and the new update version for the user to compare \n
    \b EXTLIB_OTA_GET_OPT_IS_PENDING_COMMIT - Check if the new image is in the state BUNDLE_STATE_WAITING_FOR_COMMIT \n

	Value at \e pOptionVal will be set to 1 if OTA process is active, 0 if Idle


    \return         Return 0 on success, -1 otherwise.

    \sa                       
    \note                     
    \warning
    \par                 Example:  
    \code                
    For example: check if pending commit   
    
    int32_t isPendingCommit;
    int32_t isPendingCommit_len;
    int32_t Status;

    Status = Ota_get(EXTLIB_OTA_GET_OPT_IS_PENDING_COMMIT, &isPendingCommit_len, (uint8_t *)&isPendingCommit);
    
    \endcode
*/
int16_t OTA_get(OTA_option option, int32_t *pOptionLen, uint8_t *pOptionVal);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __OTA_H__ */
