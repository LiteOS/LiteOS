/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Fabien Fleutot - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Baijie & Longrong, China Mobile - Please refer to git log
 *    
 *******************************************************************************/
/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>

*/
#ifndef _CIS_INTERNALS_H_
#define _CIS_INTERNALS_H_


#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "er-coap-13/er-coap-13.h"
#include "cis_def.h"
#include "cis_list.h"
#include "cis_api.h"
#include "cis_if_net.h"
#include "cis_if_sys.h"
#include "cis_log.h"

#define DEFAULT_LIFETIME  86400
#define INFINITE_TIMEOUT   0xFFFFFFFF

#ifdef CIS_ENABLE_JSON
#define REG_LWM2M_RESOURCE_TYPE     ">;rt=\"oma.lwm2m\";ct=1543,"   // Temporary value
#define REG_LWM2M_RESOURCE_TYPE_LEN 25
#else
#define REG_LWM2M_RESOURCE_TYPE     ">;rt=\"oma.lwm2m\","
#define REG_LWM2M_RESOURCE_TYPE_LEN 17
#endif
#define REG_START           "<"
#define REG_DEFAULT_PATH    "/"

#define URI_HEADER_HOST_LEN       7   // "coap://"
#define URI_TAILER_HOST_LEN       5   // ":5683"

#define REG_OBJECT_MIN_LEN  5   // "</n>,"
#define REG_PATH_END        ">,"
#define REG_PATH_SEPARATOR  "/"

#define REG_OBJECT_PATH             "<%s/%hu>,"
#define REG_OBJECT_INSTANCE_PATH    "<%s/%hu/%hu>,"

#define URI_REGISTRATION_SEGMENT        "rd"
#define URI_REGISTRATION_SEGMENT_LEN    2
#define URI_BOOTSTRAP_SEGMENT           "bs"
#define URI_BOOTSTRAP_SEGMENT_LEN       2

#define QUERY_TEMPLATE      "ep="
#define QUERY_LENGTH        3       // strlen("ep=")
#define QUERY_SMS           "sms="
#define QUERY_SMS_LEN       4
#define QUERY_LIFETIME      "lt="
#define QUERY_LIFETIME_LEN  3
#define QUERY_VERSION       "lwm2m="
#define QUERY_VERSION_LEN   6
#define QUERY_BINDING       "b="
#define QUERY_BINDING_LEN   2
#define QUERY_DELIMITER     "&"

#define QUERY_VERSION_FULL      "lwm2m=1.0"
#define QUERY_VERSION_FULL_LEN  9

#define REG_URI_START       '<'
#define REG_URI_END         '>'
#define REG_DELIMITER       ','
#define REG_ATTR_SEPARATOR  ';'
#define REG_ATTR_EQUALS     '='
#define REG_ATTR_TYPE_KEY           "rt"
#define REG_ATTR_TYPE_KEY_LEN       2
#define REG_ATTR_TYPE_VALUE         "\"oma.lwm2m\""
#define REG_ATTR_TYPE_VALUE_LEN     11
#define REG_ATTR_CONTENT_KEY        "ct"
#define REG_ATTR_CONTENT_KEY_LEN    2
#define REG_ATTR_CONTENT_JSON       "1543"   // Temporary value
#define REG_ATTR_CONTENT_JSON_LEN   4

#define ATTR_SERVER_ID_STR       "ep="
#define ATTR_SERVER_ID_LEN       3
#define ATTR_MIN_PERIOD_STR      "pmin="
#define ATTR_MIN_PERIOD_LEN      5
#define ATTR_MAX_PERIOD_STR      "pmax="
#define ATTR_MAX_PERIOD_LEN      5
#define ATTR_GREATER_THAN_STR    "gt="
#define ATTR_GREATER_THAN_LEN    3
#define ATTR_LESS_THAN_STR       "lt="
#define ATTR_LESS_THAN_LEN       3
#define ATTR_STEP_STR            "st="
#define ATTR_STEP_LEN            3
#define ATTR_DIMENSION_STR       "dim="
#define ATTR_DIMENSION_LEN       4

//Need add define in spec
#define URI_MAX_STRING_LEN    18      // /65535/65535/65535
#define _PRV_64BIT_BUFFER_SIZE 8

#define LINK_ITEM_START             "<"
#define LINK_ITEM_START_SIZE        1
#define LINK_ITEM_END               ">,"
#define LINK_ITEM_END_SIZE          2
#define LINK_ITEM_DIM_START         ">;dim="
#define LINK_ITEM_DIM_START_SIZE    6
#define LINK_ITEM_ATTR_END          ","
#define LINK_ITEM_ATTR_END_SIZE     1
#define LINK_URI_SEPARATOR          "/"
#define LINK_URI_SEPARATOR_SIZE     1
#define LINK_ATTR_SEPARATOR         ";"
#define LINK_ATTR_SEPARATOR_SIZE    1

#define ATTR_FLAG_NUMERIC (uint8_t)(ATTR_FLAG_LESS_THAN | ATTR_FLAG_GREATER_THAN | ATTR_FLAG_STEP)



#define STR_STATUS(S)                                               \
    ((S) == STATE_UNCREATED ? "STATE_INITED" :                         \
    ((S) == STATE_CREATED ? "STATE_CREATED" :                       \
    ((S) == STATE_CREATE_FAILED ? "STATE_CREATE_FAILED" :           \
    ((S) == STATE_CONNECT_PENDING ? "STATE_CONNECT_PENDING" :       \
    ((S) == STATE_CONNECTED ? "STATE_CONNECTED" :                   \
    ((S) == STATE_REG_PENDING ? "STATE_REG_PENDING" :               \
    ((S) == STATE_REGISTERED ? "STATE_REGISTERED" :                 \
    ((S) == STATE_REG_FAILED ? "STATE_REG_FAILED" :                 \
    ((S) == STATE_REG_UPDATE_PENDING ? "STATE_REG_UPDATE_PENDING" : \
    ((S) == STATE_REG_UPDATE_NEEDED ? "STATE_REG_UPDATE_NEEDED" :   \
    ((S) == STATE_REG_UPDATE_NEEDED_WITHOBJECTS ? "STATE_REG_UPDATE_NEEDED_WITHOBJECTS" :   \
    ((S) == STATE_DEREG_PENDING ? "STATE_DEREG_PENDING" :           \
    ((S) == STATE_BS_HOLD_OFF ? "STATE_BS_HOLD_OFF" :               \
    ((S) == STATE_BS_INITIATED ? "STATE_BS_INITIATED" :             \
    ((S) == STATE_BS_PENDING ? "STATE_BS_PENDING" :                 \
    ((S) == STATE_BS_FINISHED ? "STATE_BS_FINISHED" :               \
    ((S) == STATE_BS_FAILED ? "STATE_BS_FAILED" :                   \
    "Unknown")))))))))))))))))

#define STR_MEDIA_TYPE(M)                                \
    ((M) == LWM2M_CONTENT_TEXT ? "LWM2M_CONTENT_TEXT" :      \
    ((M) == LWM2M_CONTENT_LINK ? "LWM2M_CONTENT_LINK" :      \
    ((M) == LWM2M_CONTENT_OPAQUE ? "LWM2M_CONTENT_OPAQUE" :  \
    ((M) == LWM2M_CONTENT_TLV ? "LWM2M_CONTENT_TLV" :        \
    ((M) == LWM2M_CONTENT_TLV_NEW ? "LWM2M_CONTENT_TLV_NEW" : \
    ((M) == LWM2M_CONTENT_JSON ? "LWM2M_CONTENT_JSON" :      \
    "Unknown"))))))

#define STR_STATE(S)                                \
    ((S) == PUMP_STATE_HALT ? "PUMP_STATE_HALT" :      \
    ((S) == PUMP_STATE_INITIAL ? "PUMP_STATE_INITIAL" :      \
    ((S) == PUMP_STATE_BOOTSTRAPPING ? "PUMP_STATE_BOOTSTRAPPING" :      \
    ((S) == PUMP_STATE_CONNECTING ? "PUMP_STATE_CONNECTING" :      \
    ((S) == PUMP_STATE_REGISTER_REQUIRED ? "PUMP_STATE_REGISTER_REQUIRED" :      \
    ((S) == PUMP_STATE_REGISTERING ? "PUMP_STATE_REGISTERING" :      \
    ((S) == PUMP_STATE_READY ? "PUMP_STATE_READY" :      \
    ((S) == PUMP_STATE_DISCONNECTED ? "PUMP_STATE_DISCONNECTED" :      \
    ((S) == PUMP_STATE_UNREGISTER ? "PUMP_STATE_UNREGISTER" :      \
    "Unknown")))))))))


#define STR_EVENT_CODE(M)                                \
    ((M) == CIS_EVENT_BOOTSTRAP_START ? "EVENT_BOOTSTRAP_START" :      \
    ((M) == CIS_EVENT_BOOTSTRAP_SUCCESS ? "EVENT_BOOTSTRAP_SUCCESS" :      \
    ((M) == CIS_EVENT_BOOTSTRAP_FAILED ? "EVENT_BOOTSTRAP_FAILED" :  \
    ((M) == CIS_EVENT_CONNECT_SUCCESS ? "EVENT_CONNECT_SUCCESS" :        \
    ((M) == CIS_EVENT_CONNECT_FAILED ? "EVENT_CONNECT_FAILED" : \
    ((M) == CIS_EVENT_REG_SUCCESS ? "EVENT_REG_SUCCESS" :      \
    ((M) == CIS_EVENT_REG_FAILED ? "EVENT_REG_FAILED" :      \
    ((M) == CIS_EVENT_REG_TIMEOUT ? "EVENT_REG_TIMEOUT" :      \
    ((M) == CIS_EVENT_LIFETIME_TIMEOUT ? "EVENT_LIFETIME_TIMEOUT" :      \
    ((M) == CIS_EVENT_STATUS_HALT ? "EVENT_STATUS_HALT" :      \
    ((M) == CIS_EVENT_UPDATE_SUCCESS ? "EVENT_UPDATE_SUCCESS" :      \
    ((M) == CIS_EVENT_UPDATE_FAILED ? "EVENT_UPDATE_FAILED" :      \
    ((M) == CIS_EVENT_UPDATE_TIMEOUT ? "EVENT_UPDATE_TIMEOUT" :      \
    ((M) == CIS_EVENT_UPDATE_NEED ? "EVENT_UPDATE_NEED" :      \
    ((M) == CIS_EVENT_UNREG_DONE ? "EVENT_UNREG_DONE" :      \
    ((M) == CIS_EVENT_RESPONSE_FAILED ? "EVENT_RESPONSE_FAILED" :      \
    ((M) == CIS_EVENT_RESPONSE_SUCCESS ? "EVENT_RESPONSE_SUCCESS" :      \
    ((M) == CIS_EVENT_NOTIFY_FAILED ? "EVENT_NOTIFY_FAILED" :      \
    ((M) == CIS_EVENT_NOTIFY_SUCCESS ? "EVENT_NOTIFY_SUCCESS" :      \
    "Unknown")))))))))))))))))))

#define LOG_URI(NAME,URI)                                                           \
{                                                                                   \
    if ((URI) != NULL)																\
    {                                                                               \
    LOG_PRINT("URI(%s):(%d",NAME,(URI)->objectId);                                  \
    if (CIS_URI_IS_SET_INSTANCE(URI)) LOG_PRINT("/%d", (URI)->instanceId);      \
    if (CIS_URI_IS_SET_RESOURCE(URI)) LOG_PRINT("/%d", (URI)->resourceId);      \
    LOG_PRINT(")\n");                                                         \
    }                                                                               \
}																					 



typedef enum
{
    STATE_UNCREATED,          // not connect or boostrap not started
    STATE_CREATED,            //
    STATE_CREATE_FAILED,      //
    STATE_CONNECT_PENDING,    // connect resuested but not connect successful;
    STATE_CONNECTED,          // not registered,but connected
    STATE_CONNECT_FAILED,     //
    STATE_REG_PENDING,        // registration pending
    STATE_REG_FAILED,         // last registration failed
    STATE_REGISTERED,         // successfully registered
    STATE_REG_UPDATE_PENDING, // registration update pending
    STATE_REG_UPDATE_NEEDED,  // registration update required
    STATE_REG_UPDATE_NEEDED_WITHOBJECTS,
    STATE_DEREG_PENDING,      // deregistration pending
    STATE_BS_HOLD_OFF,
    STATE_BS_INITIATED,
    STATE_BS_PENDING,
    STATE_BS_FINISHED,
    STATE_BS_FAILED,
} et_status_t;

typedef enum
{
    BINDING_UNKNOWN = 0,
    BINDING_U,   // UDP
    BINDING_UQ,  // UDP queue mode
    BINDING_S,   // SMS
    BINDING_SQ,  // SMS queue mode
    BINDING_US,  // UDP plus SMS
    BINDING_UQS  // UDP queue mode plus SMS
} et_binding_t;

typedef enum
{
    PUMP_STATE_HALT = -1,
    PUMP_STATE_INITIAL,
	PUMP_STATE_BOOTSTRAPPING,
    PUMP_STATE_CONNECTING,
    PUMP_STATE_REGISTER_REQUIRED,
    PUMP_STATE_REGISTERING,

    PUMP_STATE_READY,
    PUMP_STATE_DISCONNECTED,
    PUMP_STATE_UNREGISTER,
} et_client_state_t;



typedef enum
{
	DATA_TYPE_UNDEFINE = cis_data_type_undefine,
	DATA_TYPE_STRING = cis_data_type_string,
	DATA_TYPE_OPAQUE = cis_data_type_opaque,
	DATA_TYPE_INTEGER = cis_data_type_integer,
	DATA_TYPE_FLOAT = cis_data_type_float,
	DATA_TYPE_BOOL = cis_data_type_bool,
	DATA_TYPE_LINK,

	DATA_TYPE_OBJECT,
	DATA_TYPE_OBJECT_INSTANCE,
	DATA_TYPE_MULTIPLE_RESOURCE,
}et_data_type_t;


typedef enum
{
    LWM2M_CONTENT_TEXT      = 0,        // Also used as undefined
    LWM2M_CONTENT_LINK      = 40,
    LWM2M_CONTENT_OPAQUE    = 42,
    LWM2M_CONTENT_TLV       = 1542,     // Temporary value
    LWM2M_CONTENT_TLV_NEW   = 11542,    // Temporary value
    LWM2M_CONTENT_JSON      = 1543      // Temporary value
} et_media_type_t;

typedef enum
{
    CALLBACK_TYPE_READ = 0,
    CALLBACK_TYPE_WRITE,
    CALLBACK_TYPE_EXECUTE,
    CALLBACK_TYPE_DISCOVER,
    CALLBACK_TYPE_OBSERVE,
    CALLBACK_TYPE_OBSERVE_CANCEL,
    CALLBACK_TYPE_OBSERVE_PARAMS,
}et_callback_type_t;


#if CIS_ENABLE_BLOCK
/*
 * LWM2M block1 data
 *
 * Temporary data needed to handle block1 request.
 * Currently support only one block1 request by server.
 */
typedef struct cis_block1_data cis_block1_data_t;

struct cis_block1_data
{
    uint8_t *             block1buffer;     // data buffer
    uint32_t              block1bufferSize; // buffer size
    uint16_t              lastmid;          // mid of the last message received
};

#endif//CIS_ENABLE_BLOCK


/////
typedef struct st_cis_observe_attr  st_observe_attr_t;
typedef struct st_cis_callback      st_callback_t;
typedef struct st_cisnet_context    st_cisnet_t;
typedef struct st_cis_context       st_context_t;
typedef struct st_object            st_object_t;
typedef struct st_request           st_request_t;
typedef struct st_notify            st_notify_t;
typedef struct st_server            st_server_t;
typedef struct st_observed          st_observed_t;
typedef struct st_uri               st_uri_t;
typedef struct st_transaction       st_transaction_t;
typedef struct st_firmware_serialize st_serialize_t;
typedef struct st_data				st_data_t;


typedef void (*cis_result_callback_t) (uint16_t clientID, st_uri_t * uriP, int status, et_media_type_t format, uint8_t * data, int dataLength, void * userData);
typedef void (*cis_transaction_callback_t) (st_transaction_t * transacP, void * message);

struct st_object
{
    struct st_object *  next;               // for internal use only.
    cis_listid_t        objID;
    cis_instcount_t     instBitmapBytes;
    cis_instcount_t     instBitmapCount;
    uint8_t*            instBitmapPtr;
    cis_instcount_t     instValidCount;

    cis_attrcount_t     attributeCount;
    cis_actcount_t      actionCount;
    void *              userData;
};

struct st_data
{
	uint16_t             id;
	et_data_type_t       type;

	struct
	{
		uint32_t    length;
		uint8_t*    buffer;
	}asBuffer;

	union
	{
		bool            asBoolean;
		int64_t         asInteger;
		double          asFloat;

		struct
		{
			uint32_t        count;
			struct st_data* array;
		}asChildren;

		struct
		{
			cis_oid_t     objectId;
			cis_iid_t     instId;
		}asObjLink;
	} value;
} ;


struct st_request
{
	struct st_request * next;               // for internal use only.
	cis_listid_t        mid;
    et_callback_type_t  type;
    uint8_t             tokenLen;
    uint8_t             token[COAP_TOKEN_LEN];
    st_uri_t            uri;
	cis_time_t			time;
	et_media_type_t     format;

    st_data_t          *respData;
    uint16_t            respDataCount;
};

struct st_notify
{
    struct st_notify *  next;               // for internal use only.
    cis_listid_t        id;
    cis_listid_t        mid;
    cis_coapret_t         result;
    cis_data_t*         value;
    cis_uri_t           uri;
    bool                isResponse;
};



struct st_server
{
    struct st_server * next;   // matches lwm2m_list_t::next
    cis_listid_t      secObjInstID; // matches lwm2m_list_t::id
    uint16_t          shortID;      // servers short ID, may be 0 for bootstrap server
    char*             host;         // server ip address, by strdup
    cis_time_t        registration; // date of the last registration in sec or end of client hold off time for bootstrap servers
    et_binding_t      binding;      // client connection mode with this server
    void *            sessionH;     // as cisnet_t
    
    et_status_t       status;
    char *            location;
    bool              dirty;

#if CIS_ENABLE_BLOCK
    cis_block1_data_t *   block1Data;
#endif//CIS_ENABLE_BLOCK
} ;


#define STRING_ID_MAX_LEN 6
/////
#define URI_FLAG_DM           (uint8_t)0x00
#define URI_FLAG_DELETE_ALL   (uint8_t)0x10
#define URI_FLAG_REGISTRATION (uint8_t)0x20
#define URI_FLAG_BOOTSTRAP    (uint8_t)0x40

#define URI_MASK_TYPE (uint8_t)0x70
#define URI_MASK_ID   (uint8_t)0x07


#define URI_IS_SET_OBJECT(uri) (((uri)->flag & URI_FLAG_OBJECT_ID) != 0)
// defined in observe.c
/*
 * LWM2M result callback
 *
 * When used with an observe, if 'data' is not nil, 'status' holds the observe counter.
 */

typedef struct
{
    st_uri_t uri;
    cis_result_callback_t callback;
    void * userData;
} dm_data_t;

typedef enum
{
    URI_DEPTH_OBJECT,
    URI_DEPTH_OBJECT_INSTANCE,
    URI_DEPTH_RESOURCE,
    URI_DEPTH_RESOURCE_INSTANCE
} uri_depth_t;

struct st_observed
{
	struct st_observed * next;
	cis_listid_t        msgid;
	st_uri_t            uri;
    bool                actived;
	st_observe_attr_t * parameters;
	uint8_t             token[8];
	size_t              tokenLen;
	cis_time_t          lastTime;
	uint32_t            counter;
	uint16_t            lastMid;
    et_media_type_t     format;

    st_data_t          *reportData;
    uint16_t            reportDataCount;
};

struct st_firmware_serialize
{
    uint16_t size;
    uint16_t reserved1;
    uint8_t  host[16];
    uint8_t  time[4];
    uint8_t  reserved2[8];
};

struct st_transaction
{
    st_transaction_t *  next;  // matches lwm2m_list_t::next
    cis_listid_t        mID;   // matches lwm2m_list_t::id
	st_server_t*        server;
    uint8_t             ackReceived; // indicates, that the ACK was received
    cis_time_t          responseTimeout; // timeout to wait for response, if token is used. When 0, use calculated acknowledge timeout.
    uint8_t             retransCounter;
    cis_time_t          retransTime;
    /*TODO:update to uri format type*/
    char                objStringID[STRING_ID_MAX_LEN];
    char                instanceStringID[STRING_ID_MAX_LEN];
    char                resourceStringID[STRING_ID_MAX_LEN];
    void *              message;
    uint32_t            buffer_len;
    uint8_t *           buffer;
    cis_transaction_callback_t callback;
    void *              userData;
};


struct st_cis_context
{
    char*               endpointName;
    char *              altPath;
    uint16_t            nextMid;
    uint16_t            nextObserveNum;
    cis_listid_t        nextNotifyId;
    bool                registerEnabled;
    et_client_state_t   stateStep;
    st_server_t*        server;
	st_request_t *      requestList;
    st_notify_t*        notifyList;
    uint16_t            objectNum;
    st_object_t *       objectList;

    st_observed_t *     observedList;
    st_transaction_t *  transactionList;

    st_callback_t       callback;
	st_server_t *       bootstrapServer;

    cis_time_t          lasttime;
    cis_time_t          lifetime;
    cis_time_t          lifetimeWarnningTime;

    void*               lockNotify;
    void*               lockRequest;
};

/*
 * LWM2M Link Attributes
 * Used for observation parameters.
 */
#define ATTR_FLAG_MIN_PERIOD      (uint8_t)0x01
#define ATTR_FLAG_MAX_PERIOD      (uint8_t)0x02
#define ATTR_FLAG_GREATER_THAN    (uint8_t)0x04
#define ATTR_FLAG_LESS_THAN       (uint8_t)0x08
#define ATTR_FLAG_STEP            (uint8_t)0x10

#define TLV_HEADER_MAX_LENGTH       6

#define CIS_MIN(a,b) ((a) < (b) ? (a) : (b))
#define CIS_MAX(a,b) ((a) < (b) ? (b) : (a))
#define CIS_CLAMP(i,v,x) (CIS_MAX(CIS_MIN(v,x), i))

// defined in uri.c
// Parse an URI in LWM2M format and fill the uri.
// Return the number of characters read from buffer or 0 in case of error.
// Valid URIs: /1, /1/, /1/2, /1/2/, /1/2/3
// Invalid URIs: /, //, //2, /1//, /1//3, /1/2/3/, /1/2/3/4
int         uri_stringToUri(const char * buffer, size_t buffer_len, st_uri_t * uriP);
st_uri_t *  uri_decode(char * altPath, multi_option_t *uriPath);
int         uri_getNumber(uint8_t * uriString, size_t uriLength);
int         uri_toString(st_uri_t * uriP, uint8_t * buffer, size_t bufferLen, uri_depth_t * depthP);
int         uri_make(cis_oid_t objectId,cis_iid_t instanceId,cis_rid_t resourceId, st_uri_t * uriP);
bool        uri_exist(const st_uri_t * uri1,const st_uri_t * uri2);

st_transaction_t * transaction_new(coap_message_type_t type, coap_method_t method, char * altPath, st_uri_t * uriP, uint16_t mID, uint8_t token_len, uint8_t* token);
bool        transaction_send(st_context_t * contextP, st_transaction_t * transacP);
void        transaction_free(st_transaction_t * transacP);
void        transaction_remove(st_context_t * contextP, st_transaction_t * transacP);
void        transaction_removeAll(st_context_t* contextP);

bool        transaction_handleResponse(st_context_t * contextP,coap_packet_t * message, coap_packet_t * response);
void        transaction_step(st_context_t * contextP, cis_time_t currentTime);
int         transaction_count(st_context_t * contextP);

coap_status_t   object_read(st_context_t * contextP, st_uri_t * uriP, coap_packet_t* message);
coap_status_t   object_readSerialize(st_context_t * contextP, st_uri_t * uriP, et_media_type_t * formatP, uint8_t ** bufferP, size_t * lengthP,coap_packet_t* message);
coap_status_t   object_write(st_context_t * contextP, st_uri_t * uriP, et_media_type_t format, uint8_t * buffer, size_t length,coap_packet_t* message);
coap_status_t   object_execute(st_context_t * contextP, st_uri_t * uriP, uint8_t * buffer, size_t length,coap_packet_t* message);
coap_status_t   object_discover( st_context_t * contextP,st_uri_t * uriP,coap_packet_t* message);
coap_status_t   object_writeInstance(st_context_t * contextP,st_uri_t * uriP,st_data_t * dataP);

bool            object_isInstanceNew(st_context_t * contextP, cis_oid_t  objectId, cis_iid_t  instanceId);
int             object_getRegisterPayload(st_context_t * contextP, uint8_t * buffer, size_t length);
coap_status_t   object_asynAckReadData(st_context_t * context,st_request_t* request,cis_coapret_t result);
coap_status_t   object_asynAckNodata(st_context_t * context,st_request_t* request,cis_coapret_t result);
void            object_removeAll(st_context_t* context);
bool            object_checkInstExisted(uint8_t* inst_bitmap, cis_iid_t iid);

// defined in core.c
void            core_callbackEvent(st_context_t* context,cis_evt_t id,void* param);
void            core_updatePumpState(st_context_t* context,et_client_state_t state);


// defined in management.c
st_server_t *   management_makeServerList(st_context_t * contextP,bool bootstrap);
void*           management_createNetwork(st_context_t * context,st_server_t* serverP);
bool            management_connectServer(st_context_t * context,st_server_t* serverP);
void            management_disconnectServer(st_context_t * context,st_server_t* serverP);
coap_status_t   management_handleRequest(st_context_t * contextP,st_uri_t * uriP,coap_packet_t * message,coap_packet_t * response);
void            management_destoryServer(st_context_t * context,st_server_t * serverP);
void            management_refreshDirtyServer(st_context_t * contextP);

//Observe
coap_status_t   observe_asynAckNodata(st_context_t * context,st_request_t* request,cis_coapret_t result);
coap_status_t   observe_asynReport(st_context_t* contextP,const st_observed_t* observe,cis_coapret_t result);
void            observe_removeAll(st_context_t* contextP);

coap_status_t   observe_handleRequest(st_context_t * contextP, st_uri_t * uriP,coap_packet_t * message, coap_packet_t * response);
void            observe_cancel(st_context_t * contextP, uint16_t mid, st_uri_t* uriP);
coap_status_t   observe_setParameters(st_context_t * contextP, st_uri_t * uriP,st_observe_attr_t * attrP,coap_packet_t* message);
bool            observe_handleNotify(st_context_t * contextP, coap_packet_t * message, coap_packet_t * response);
st_observed_t * observe_findByUri(st_context_t * contextP, st_uri_t * uriP);
st_observed_t * observe_findByMsgid(st_context_t * contextP, cis_mid_t mid);


/*
* defined in cis_discover.c
*/
int discover_serialize( st_context_t *contextP,st_uri_t *uriP,int size,st_data_t *dataP,uint8_t **bufferP );

// defined in registration.c

// send a registration update to the server
// If withObjects is true, the registration update contains the object list.
int             registration_update_registration(st_context_t * contextP, bool withObjects);
coap_status_t   registration_handleRequest(st_context_t * contextP, st_uri_t * uriP, coap_packet_t * message, coap_packet_t * response);
void            registration_deregister(st_context_t * contextP);
coap_status_t   registration_start(st_context_t * contextP);
void            registration_step(st_context_t * contextP, cis_time_t currentTime);
et_status_t     registration_getStatus(st_context_t * contextP);


// defined in tlv.c
int             tlv_parse(uint8_t * buffer, size_t bufferLen, st_data_t ** dataP);
size_t          tlv_serialize(bool isInstance, int size, st_data_t * dataP, uint8_t ** bufferP);

//new coming in tlv.c
/*
 * Utility function to parse TLV buffers directly
 *
 * Returned value: number of bytes parsed
 * buffer: buffer to parse
 * buffer_len: length in bytes of buffer
 * oType: (OUT) type of the parsed TLV record. can be:
 *          - LWM2M_TYPE_OBJECT
 *          - LWM2M_TYPE_OBJECT_INSTANCE
 *          - LWM2M_TYPE_MULTIPLE_RESOURCE
 *          - LWM2M_TYPE_OPAQUE
 * oID: (OUT) ID of the parsed TLV record
 * oDataIndex: (OUT) index of the data of the parsed TLV record in the buffer
 * oDataLen: (OUT) length of the data of the parsed TLV record
 */


int         tlv_decode_TLV(const uint8_t * buffer, size_t buffer_len, et_data_type_t * oType, uint16_t * oID, size_t * oDataIndex, size_t * oDataLen);

// defined in json.c
#ifdef CIS_ENABLE_JSON
int         json_parse(st_uri_t * uriP, uint8_t * buffer, size_t bufferLen, st_data_t ** dataP);
size_t      json_serialize(st_uri_t * uriP, int size, st_data_t * tlvP, uint8_t ** bufferP);
#endif


#if CIS_ENABLE_BLOCK
// defined in block1.c
coap_status_t   coap_block1_handler(cis_block1_data_t ** block1Data, uint16_t mid, uint8_t * buffer, size_t length, uint16_t blockSize, uint32_t blockNum, bool blockMore, uint8_t ** outputBuffer, uint32_t * outputLength);
void            free_block1_buffer(cis_block1_data_t * block1Data);
#endif//CIS_ENABLE_BLOCK

// defined in utils.c

int             utils_checkBigendian();

#if CIS_ENABLE_JSON
et_data_type_t  utils_depthToDatatype(uri_depth_t depth);
#endif//CIS_ENABLE_JSON

et_binding_t    utils_stringToBinding(uint8_t *buffer, size_t length);
et_media_type_t utils_convertMediaType(coap_content_type_t type);
int             utils_isAltPathValid(const char * altPath);
size_t          utils_intToText(int64_t data, uint8_t * string, size_t length);
size_t          utils_floatToText(double data, uint8_t * string, size_t length);
int             utils_plainTextToInt64(uint8_t * buffer, int length, int64_t * dataP);
int             utils_plainTextToFloat64(uint8_t * buffer, int length, double * dataP);
size_t          utils_int64ToPlainText(int64_t data, uint8_t ** bufferP);
size_t          utils_float64ToPlainText(double data, uint8_t ** bufferP);
size_t          utils_boolToPlainText(bool data, uint8_t ** bufferP);

int             utils_opaqueToInt(const uint8_t * buffer, size_t buffer_len, int64_t * dataP);
int             utils_opaqueToFloat(const uint8_t * buffer, size_t buffer_len, double * dataP);
size_t          utils_encodeInt(int64_t data, uint8_t data_buffer[_PRV_64BIT_BUFFER_SIZE]);
size_t          utils_encodeFloat(double data, uint8_t data_buffer[_PRV_64BIT_BUFFER_SIZE]);
size_t          utils_base64ToOpaque(uint8_t * dataP, size_t dataLen, uint8_t ** bufferP);
size_t          utils_opaqueToBase64(uint8_t * dataP, size_t dataLen, uint8_t ** bufferP);
size_t          utils_base64Encode(uint8_t * dataP, size_t dataLen, uint8_t * bufferP, size_t bufferLen);

int             utils_stringCopy(char * buffer, size_t length, const char * str);
int             utils_intCopy(char * buffer, size_t length, int32_t value);
void            utils_copyValue(void * dst, const void * src, size_t len);
char *          utils_strdup(const char * str);
uint8_t*        utils_bufferdup(const uint8_t * src,size_t len);

// Compare at most the n first bytes of s1 and s2, return 0 if they match
int             utils_strncmp(const char * s1, const char * s2, size_t n);
int             utils_strncasecmp(const char * s1, const char * s2, size_t n);
int             utils_strlen(const char *str);
int             utils_strtoi(const char *str, char **endptr);
#if CIS_ENABLE_LOG
int             utils_vsnprintf(char *buf, size_t buf_size, const char *fmt, va_list args);
int             utils_snprintf(char* buffer, size_t size, const char* format, ...);
#endif//CIS_ENABLE_LOG

st_server_t *   utils_findBootstrapServer(st_context_t * contextP,void * fromSessionH);

int utils_atoi( const char *str,int size );

int utils_itoa( int val,char *str,int size );

// defined in bootstrap.c
void            bootstrap_step(st_context_t * context, cis_time_t currentTime);
coap_status_t   bootstrap_handleCommand(st_context_t * context, st_uri_t * uriP, st_server_t * serverP, coap_packet_t * message, coap_packet_t * response);
coap_status_t   bootstrap_handleDeleteAll(st_context_t * context, void * fromSessionH);
coap_status_t   bootstrap_handleFinish(st_context_t * context, void * fromSessionH);
void            bootstrap_init(st_context_t * context);
void            bootstrap_create(st_context_t * context);
void            bootstrap_connect(st_context_t * context);
void            bootstrap_destory(st_context_t * context);
et_status_t     bootstrap_getStatus(st_context_t * context);

// This function must return the number of seconds elapsed since origin.
// The origin (Epoch, system boot, etc...) does not matter as this
// function is used only to determine the elapsed time since the last call to it.
// In case of error, this must return a negative value.
// Per POSIX specifications, time_t is a signed integer.
int32_t utils_gettime_s(void);



//end of new implementation

//packet.c
// dispatch received data to liblwm2m
bool packet_asynPushRequest(st_context_t * context,const coap_packet_t* packet,et_callback_type_t type,cis_mid_t mid);
void packet_asynRemoveRequest(st_context_t * context,cis_mid_t mid,st_request_t** node);
bool packet_asynFindObserveRequest(st_context_t * context,cis_coapid_t coapmid,cis_mid_t *observemid);

void packet_asynRemoveRequestAll(st_context_t * context);
void packet_asynRemoveNotifyAll(st_context_t * context);


void packet_handle_packet(st_context_t * contextP, void* session, uint8_t * buffer, int length);
void packet_step(st_context_t *context, cis_time_t currentTime);
bool packet_read(st_context_t *context);
coap_status_t   packet_send(st_context_t * contextP, coap_packet_t * message);


/////memory related
#if CIS_ENABLE_MEMORYTRACE
// same functions as above with caller location for debugging purposes
void *  cis_trace_malloc(size_t size, const char * file, const char * function, int lineno);
void    cis_trace_free(void * mem, const char * file, const char * function, int lineno);

#define cis_malloc(S) cis_trace_malloc(S, __FILE__, __FUNCTION__, __LINE__)
#define cis_free(M)   cis_trace_free(M, __FILE__, __FUNCTION__, __LINE__)

void trace_print(int loops, int level);
void trace_status(int* blocks, size_t* size);

#else

// Allocate a block of size bytes of memory, returning a pointer to the beginning of the block.
void *  cis_malloc(size_t s);
// Deallocate a block of memory previously allocated by lwm2m_malloc() or lwm2m_strdup()
void    cis_free(void * p);
// Allocate a memory block, duplicate the string str in it and return a pointer to this new block.

#endif

//others about memory manipulation
#define  cis_memset(a1,a2,a3) cissys_memset(a1,a2,a3)
#define  cis_memcpy(a1,a2,a3) cissys_memcpy(a1,a2,a3)
#define  cis_memmove(a1,a2,a3) cissys_memmove(a1,a2,a3)
#define  cis_memcmp(a1,a2,a3) cissys_memcmp(a1,a2,a3)


//data.c
st_data_t  *data_new(int size);
int         data_parse(st_uri_t * uriP, uint8_t * buffer, size_t bufferLen, et_media_type_t format, st_data_t ** dataP);
int         data_serialize(st_uri_t * uriP, int size, st_data_t * dataP, et_media_type_t * formatP, uint8_t ** bufferP);
bool        data_copy(st_data_t* dst,const st_data_t* src);
void        data_free(int size, st_data_t * dataP);
void        data_encode_string(const char * string, st_data_t * dataP);
void        data_encode_nstring(const char * string, size_t length, st_data_t * dataP);
void        data_encode_opaque(uint8_t * buffer, size_t length, st_data_t * dataP);

void        data_encode_int(int64_t value, st_data_t * dataP);
int         data_decode_int(const st_data_t * dataP, int64_t * valueP);
void        data_encode_float(double value, st_data_t * dataP);
int         data_decode_float(const st_data_t * dataP, double * valueP);
void        data_encode_bool(bool value, st_data_t * dataP);
int         data_decode_bool(const st_data_t * dataP, bool * valueP);
void        data_encode_objlink(cis_oid_t objectId, cis_iid_t objectInstanceId, st_data_t * dataP);
void        data_encode_instances(st_data_t * subDataP, size_t count, st_data_t * dataP);
void        data_include(st_data_t * subDataP, size_t count, st_data_t * dataP);

#endif//_CIS_INTERNALS_H_
