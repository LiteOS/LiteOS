/*
 Copyright (c) 2017 Chinamobile

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

*/


#include <stdio.h>
#include <stdlib.h>
#include <cis_def.h>
#include <cis_api.h>
#include <cis_if_sys.h>
#include <cis_log.h>
#include <cis_list.h>
#include <win_timers.h>
#include <cis_api.h>
#include "cis_sample_defs1.h"
#include <string.h>
#include <cmsis_os.h>
#include "usart3.h"
#include "M5310.h"
#include "cis_internals.h"


static void     prv_observeNotify     (void* context,cis_uri_t* uri,cis_mid_t mid);
static void     prv_observeNotify    (void* context,cis_uri_t* uri,cis_mid_t mid);

static void     prv_readResponse     (void* context,cis_uri_t* uri,cis_mid_t mid);
static void     prv_discoverResponse (void* context,cis_uri_t* uri,cis_mid_t mid);
static void     prv_writeResponse    (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t count,cis_mid_t mid);
static void     prv_execResponse     (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid);
static void     prv_paramsResponse   (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid);
static cis_data_t* prv_dataDup(const cis_data_t* value,cis_attrcount_t attrcount);


static cis_coapret_t cis_onRead        (void* context,cis_uri_t* uri,cis_mid_t mid);
static cis_coapret_t cis_onWrite       (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t attrcount,cis_mid_t mid);
static cis_coapret_t cis_onExec        (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid);
static cis_coapret_t cis_onObserve     (void* context,cis_uri_t* uri,bool flag,cis_mid_t mid);
static cis_coapret_t cis_onParams      (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid);
static cis_coapret_t cis_onDiscover    (void* context,cis_uri_t* uri,cis_mid_t mid);
static void        cis_onEvent       (void* context,cis_evt_t eid,void* param);



static struct st_callback_info* g_callbackList = NULL;
static struct st_observe_info* g_observeList = NULL;

static void*      g_context = NULL;
static bool       g_shutdown = false;
bool       g_doUnregister = false;
bool       g_doRegister = false;


static cis_time_t g_lifetimeLast = 0;
static cis_time_t g_lifetime = 0;
static cis_time_t g_notifyLast = 0;

static st_sample_object     g_objectList[SAMPLE_OBJECT_MAX];
static st_instance_a        g_instList_a[SAMPLE_A_INSTANCE_COUNT];
static st_instance_b        g_instList_b[SAMPLE_B_INSTANCE_COUNT];

typedef enum{
	SAMPLE_CALLBACK_UNKNOWN = 0,
	SAMPLE_CALLBACK_READ,
	SAMPLE_CALLBACK_WRITE,
	SAMPLE_CALLBACK_EXECUTE,
	SAMPLE_CALLBACK_OBSERVE,
	SAMPLE_CALLBACK_SETPARAMS,
	SAMPLE_CALLBACK_DISCOVER,
}et_callback_type_t1;

struct st_observe_info
{
    struct st_observe_info* next;
    cis_listid_t    mid;
    cis_uri_t       uri;
    cis_observe_attr_t params;
};

struct st_callback_info
{
    struct st_callback_info* next;
    cis_listid_t       mid;
    et_callback_type_t1 flag; 
    cis_uri_t uri;

    union
    {
        struct{
            cis_data_t* value;
            cis_attrcount_t count;
        }asWrite;

        struct{
            uint8_t* buffer;
            uint32_t length;
        }asExec;
        
        struct  
        {
            bool flag;
        }asObserve;
        
        struct  
        {
            cis_observe_attr_t params;
        }asObserveParam;
    }param;

};



static void prv_make_sample_data()
{
    int i = 0;
    cis_instcount_t instIndex;
    cis_instcount_t instCount;
    for (i= 0;i<SAMPLE_OBJECT_MAX ; i++)
    {
        st_sample_object* obj = &g_objectList[i];
        switch(i){
            case 0:
            {
                obj->oid = SAMPLE_OID_A;
                obj->instBitmap = SAMPLE_A_INSTANCE_BITMAP;
                instCount = SAMPLE_A_INSTANCE_COUNT;
                for (instIndex = 0;instIndex < instCount;instIndex++)
                {
                    if(obj->instBitmap[instIndex] != '1'){
                        g_instList_a[instIndex].instId = instIndex;
                        g_instList_a[instIndex].enabled = false;
                    }else
                    {
                        g_instList_a[instIndex].instId = instIndex;
                        g_instList_a[instIndex].enabled = true;

                        g_instList_a[instIndex].instance.boolValue = true;
                        strcpy(g_instList_a[instIndex].instance.strValue, "kylin led1");
                    }
                }
                obj->attrCount = sizeof(const_AttrIds_a) / sizeof(cis_rid_t);
                obj->attrListPtr = const_AttrIds_a;

            }
            break;
            case 1:
            {
                obj->oid = SAMPLE_OID_B;
                obj->instBitmap = SAMPLE_B_INSTANCE_BITMAP;
                instCount = SAMPLE_B_INSTANCE_COUNT;
                for (instIndex = 0;instIndex < instCount;instIndex++)
                {
                    if(obj->instBitmap[instIndex] != '1'){
                        g_instList_b[instIndex].instId = instIndex;
                        g_instList_b[instIndex].enabled = false;
                    }else
                    {
                        g_instList_b[instIndex].instId = instIndex;
                        g_instList_b[instIndex].enabled = true;

                        g_instList_b[instIndex].instance.floatValue = 0;
                        strcpy(g_instList_b[instIndex].instance.strValue,"temp sensor");
                    }
                }

                obj->attrCount = sizeof(const_AttrIds_b) / sizeof(cis_rid_t);
                obj->attrListPtr = const_AttrIds_b;

                obj->actCount = sizeof(const_ActIds_b) / sizeof(cis_rid_t);
                obj->actListPtr = const_ActIds_b;
            }
            break;
        }
    }
}


int taskThread(char *argv[])
{
    while(!g_shutdown)
    {
        struct st_callback_info* node;
        if(g_callbackList == NULL){
            cissys_sleepms(100);
            continue;
        }
        node = g_callbackList;
        g_callbackList = g_callbackList->next;
        
        switch (node->flag)
        {
            case 0:
            break;
            case SAMPLE_CALLBACK_READ:
            {
                cis_uri_t uriLocal;
                uriLocal = node->uri;
                prv_readResponse(g_context,&uriLocal,node->mid);
            }
            break;
            case SAMPLE_CALLBACK_DISCOVER:
            {
                cis_uri_t uriLocal;
                uriLocal = node->uri;
                prv_discoverResponse(g_context,&uriLocal,node->mid);
            }
            break;
            case SAMPLE_CALLBACK_WRITE:
            {
                //write
                prv_writeResponse(g_context,&node->uri,node->param.asWrite.value,node->param.asWrite.count,node->mid);
                cis_data_t* data = node->param.asWrite.value;
                cis_attrcount_t count = node->param.asWrite.count;

                for (int i=0;i<count;i++)
                {
                    if(data[i].type == cis_data_type_string || data[i].type == cis_data_type_opaque)
                    {
                        if(data[i].asBuffer.buffer != NULL)
                            cis_free(data[i].asBuffer.buffer);
                    }
                }
                cis_free(data);
            }
            break;
            case SAMPLE_CALLBACK_EXECUTE:
            {
                //exec and notify
                prv_execResponse(g_context,&node->uri,node->param.asExec.buffer,node->param.asExec.length,node->mid);
                cis_free(node->param.asExec.buffer);
            }
            break;
            case SAMPLE_CALLBACK_SETPARAMS:
            {
                //set parameters and notify
                prv_paramsResponse(g_context,&node->uri,node->param.asObserveParam.params,node->mid);
            }
            break;
            case SAMPLE_CALLBACK_OBSERVE:
            {
                if(node->param.asObserve.flag){
                    uint16_t count = 0;
                    struct st_observe_info* observe_new = (struct st_observe_info*)cis_malloc(sizeof(struct st_observe_info));
                    cissys_assert(observe_new!=NULL);
                    observe_new->mid = node->mid;
                    observe_new->uri = node->uri;
                    observe_new->next = NULL;

                    g_observeList = (struct st_observe_info*)cis_list_add((cis_list_t*)g_observeList,(cis_list_t*)observe_new);

                    cis_response(g_context,NULL,NULL,node->mid,CIS_RESPONSE_OBSERVE);
                }else{
                    struct st_observe_info* delnode = NULL;

                    g_observeList = (struct st_observe_info *)cis_list_remove((cis_list_t *)g_observeList,node->mid,(cis_list_t **)&delnode);
                    if(delnode == NULL)return CIS_RESPONSE_NOT_FOUND;

                    cis_free(delnode);
                    cis_response(g_context,NULL,NULL,node->mid,CIS_RESPONSE_OBSERVE);
                }
            }
            default:
            break;
        }

        cis_free(node);
    }

    return 1;
}

#define CIS_TASK_STACK_SIZE  2048
#define CIS_TASK_PRIO        osPriorityNormal

int cis_sample_entry(void* config_bin,uint32_t config_size)
{
    cis_version_t ver;
    int index = 0;

    cis_callback_t callback;
    callback.onRead = cis_onRead;
    callback.onWrite = cis_onWrite;
    callback.onExec = cis_onExec;
    callback.onObserve = cis_onObserve;
    callback.onSetParams = cis_onParams;
    callback.onEvent = cis_onEvent;
    callback.onDiscover = cis_onDiscover;

    g_lifetime = 30;
    /*init sample data*/
    prv_make_sample_data();
    netdev_init();

    if(cis_init(&g_context, config_bin, config_size) != CIS_RET_OK){
        printf("cis entry init failed.\n");
        return -1;
    }

    cis_version(&ver);
    printf("CIS SDK Version:%u.%u\n",ver.major,ver.minor);

    for (index= 0;index<SAMPLE_OBJECT_MAX ; index++)
    {
        cis_inst_bitmap_t bitmap;
        cis_res_count_t  rescount;
        cis_instcount_t instCount,instBytes;
        const char* instAsciiPtr;
        uint8_t * instPtr;
        cis_oid_t oid;
        int16_t i;
        st_sample_object* obj = &g_objectList[index];

        oid = obj->oid;
        instCount = utils_strlen(obj->instBitmap);//实例的个数包括不使能的实例
        instBytes = (instCount - 1) / 8 + 1;//要用多少个字节来表示实例，每个实例占用一位
        instAsciiPtr = obj->instBitmap;
        instPtr = (uint8_t*)cis_malloc(instBytes);
        cissys_assert(instPtr!=NULL);
        memset(instPtr,0,instBytes);

        for (i = 0;i < instCount;i++)//这一段代码的意思是把类似"1101"的二进制字符串转换为二进制数据1101
        {
            cis_instcount_t instBytePos = i / 8;
            cis_instcount_t instByteOffset = 7 - (i % 8);
            if(instAsciiPtr[i] == '1'){
                instPtr[instBytePos] += 0x01 << instByteOffset;
            }
        }

        bitmap.instanceCount = instCount;
        bitmap.instanceBitmap = instPtr;
        bitmap.instanceBytes = instBytes;

        rescount.attrCount = obj->attrCount;
        rescount.actCount = obj->actCount;

        cis_addobject(g_context,oid,&bitmap,&rescount);
        cis_free(instPtr);
    }

    g_shutdown = false;
    g_doUnregister = false;
    
    //register enabled
    g_doRegister = true;

    os_thread_new("callbackTask", taskThread, NULL, CIS_TASK_STACK_SIZE, CIS_TASK_PRIO);
    
    while(!g_shutdown)
    {
        uint32_t pumpRet;
        
        if(g_doRegister)
        {
            g_doRegister = false;
            cis_register(g_context,g_lifetime,&callback);//将回调函数注册进SDK内
        }

        if(g_doUnregister){
            g_doUnregister = false;
            cis_unregister(g_context);
            struct st_observe_info* delnode;
            while(g_observeList != NULL){
                g_observeList =(struct st_observe_info *)CIS_LIST_RM((cis_list_t *)g_observeList,g_observeList->mid,(cis_list_t **)&delnode);
                cis_free(delnode);
            }
            cissys_sleepms(1000);
            g_doRegister = 1;
        }

        /*pump function*/
        pumpRet = cis_pump(g_context);
        if(pumpRet == PUMP_RET_CUSTOM)
        {
            cissys_sleepms(1000);
        }

        uint32_t nowtime;
        /*data observe data report*/
        nowtime = cissys_gettime();
        struct st_observe_info* node = g_observeList;
        if(nowtime - g_notifyLast > 10*1000){
            g_notifyLast = nowtime;
            while(node != NULL){
                if(node->mid == 0)
                    continue;
                if(node->uri.flag == 0)
                    continue;
                cis_uri_t uriLocal;
                uriLocal = node->uri;
                prv_observeNotify(g_context,&uriLocal,node->mid);//订阅资源发送接口
                node = node->next;
            }
        }
    }
    
    
    cis_deinit(&g_context);
    
    struct st_observe_info* delnode;
    while(g_observeList != NULL){
        g_observeList =(struct st_observe_info *)CIS_LIST_RM((cis_list_t *)g_observeList,g_observeList->mid,(cis_list_t **)&delnode);
        cis_free(delnode);
    }

    cissys_sleepms(2000);

    return 0;
}


//////////////////////////////////////////////////////////////////////////
//private funcation;
void prv_observeNotify(void* context,cis_uri_t* uri,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;
    cis_data_t value;
    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectList[index].oid ==  uri->objectId){
            object = &g_objectList[index];
        }
    }

    if(object == NULL){
        return;
    }


    if(!CIS_URI_IS_SET_INSTANCE(uri) && !CIS_URI_IS_SET_RESOURCE(uri)) // one object
    {
        switch(uri->objectId)
        {
            case SAMPLE_OID_A:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_a *inst = &g_instList_a[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[2];

                        tmpdata[0].type = cis_data_type_bool;
                        tmpdata[0].value.asBoolean = inst->instance.boolValue;
                        uri->resourceId = attributeA_boolValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[1].type = cis_data_type_string;
                        tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeA_stringValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTENT,false);
                    }
                }
            }
            break;
            case SAMPLE_OID_B:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_b *inst = &g_instList_b[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[2];

                        tmpdata[0].type = cis_data_type_float;
                        tmpdata[0].value.asFloat = inst->instance.floatValue;
                        uri->resourceId = attributeB_floatValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                        tmpdata[1].type = cis_data_type_string;
                        tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[1].asBuffer.buffer =(uint8_t*)(inst->instance.strValue); //(uint8_t*)utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeB_stringValue;
                        cis_uri_update(uri);
                        cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTENT,false);
                    }
                }
            }
            break;
        }
    } else if (CIS_URI_IS_SET_INSTANCE(uri)) {
        switch(object->oid)
        {
            case SAMPLE_OID_A:
            {
                if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                    return;
                }
                st_instance_a *inst = &g_instList_a[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeA_boolValue)
                    {
                        value.type = cis_data_type_bool;
                        value.value.asBoolean = inst->instance.boolValue;
                    }
                    else if(uri->resourceId == attributeA_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_notify(context,uri,&value,mid,CIS_NOTIFY_CONTENT,false);

                }else{
                    cis_data_t tmpdata[2];

                    tmpdata[0].type = cis_data_type_bool;
                    tmpdata[0].value.asBoolean = inst->instance.boolValue;
                    uri->resourceId = attributeA_boolValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                    tmpdata[1].type = cis_data_type_string;
                    tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeA_stringValue;
                    cis_uri_update(uri);//更新uri信息
                    cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTENT,false);
                }
            }
            break;
            case SAMPLE_OID_B:
            {
                if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                    return;
                }
                st_instance_b *inst = &g_instList_b[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeB_floatValue)
                    {
                        value.type = cis_data_type_float;
                        value.value.asFloat = inst->instance.floatValue;
                    }
                    else if(uri->resourceId == attributeB_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_notify(context,uri,&value,mid,CIS_NOTIFY_CONTENT,false);

                }else{
                    cis_data_t tmpdata[2];

                    tmpdata[0].type = cis_data_type_float;
                    tmpdata[0].value.asFloat = inst->instance.floatValue;
                    uri->resourceId = attributeB_floatValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[0],mid,CIS_NOTIFY_CONTINUE,false);

                    tmpdata[1].type = cis_data_type_string;
                    tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeB_stringValue;
                    cis_uri_update(uri);
                    cis_notify(context,uri,&tmpdata[1],mid,CIS_NOTIFY_CONTENT,false);
                }
            }
            break;
        }
    }
}



void prv_readResponse(void* context,cis_uri_t* uri,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;
    cis_data_t value;
    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectList[index].oid ==  uri->objectId){
            object = &g_objectList[index];
        }
    }

    if(object == NULL){
        return;
    }


    if(!CIS_URI_IS_SET_INSTANCE(uri) && !CIS_URI_IS_SET_RESOURCE(uri)) // one object
    {
        switch(uri->objectId)
        {
            case SAMPLE_OID_A:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_a *inst = &g_instList_a[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[2];

                        tmpdata[0].type = cis_data_type_bool;
                        tmpdata[0].value.asBoolean = inst->instance.boolValue;
                        uri->resourceId = attributeA_boolValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[1].type = cis_data_type_string;
                        tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeA_stringValue;
                        uri->instanceId = inst->instId;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_CONTINUE);
                    }
                }
            }
            break;
            case SAMPLE_OID_B:
            {
                for(index=0;index<SAMPLE_A_INSTANCE_COUNT;index++)
                {                   
                    st_instance_b *inst = &g_instList_b[index];
                    if(inst != NULL &&  inst->enabled == true)
                    {
                        cis_data_t tmpdata[2];

                        tmpdata[0].type = cis_data_type_float;
                        tmpdata[0].value.asFloat = inst->instance.floatValue;
                        uri->resourceId = attributeB_floatValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                        tmpdata[1].type = cis_data_type_string;
                        tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                        tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                        uri->resourceId = attributeB_stringValue;
                        cis_uri_update(uri);
                        cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_CONTINUE);
                    }
                }
            }
            break;
        }
        cis_response(context,NULL,NULL,mid,CIS_RESPONSE_READ);

    }else
    {
        switch(object->oid)
        {
            case SAMPLE_OID_A:
            {
                if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                    return;
                }
                st_instance_a *inst = &g_instList_a[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeA_boolValue)
                    {
                        value.type = cis_data_type_bool;
                        value.value.asBoolean = inst->instance.boolValue;
                    }
                    else if(uri->resourceId == attributeA_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_response(context,uri,&value,mid,CIS_RESPONSE_READ);

                }else{
                    cis_data_t tmpdata[2];

                    tmpdata[0].type = cis_data_type_bool;
                    tmpdata[0].value.asBoolean = inst->instance.boolValue;
                    uri->resourceId = attributeA_boolValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                    tmpdata[1].type = cis_data_type_string;
                    tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeA_stringValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_READ);
                }
            }
            break;
            case SAMPLE_OID_B:
            {
                if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                    return;
                }
                st_instance_b *inst = &g_instList_b[uri->instanceId];
                if(inst == NULL || inst->enabled == false){
                    return;
                }

                if(CIS_URI_IS_SET_RESOURCE(uri)){
                    if(uri->resourceId == attributeB_floatValue)
                    {
                        extern float SHT20_GetTempreture(void);
                        inst->instance.floatValue = SHT20_GetTempreture();

                        value.type = cis_data_type_float;
                        value.value.asFloat = inst->instance.floatValue;
                    }
                    else if(uri->resourceId == attributeB_stringValue)
                    {
                        value.type = cis_data_type_string;
                        value.asBuffer.length = utils_strlen(inst->instance.strValue);
                        value.asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    }else{
                        return;
                    }

                    cis_response(context,uri,&value,mid,CIS_RESPONSE_READ);

                }else{
                    cis_data_t tmpdata[2];

                    tmpdata[0].type = cis_data_type_float;
                    tmpdata[0].value.asFloat = inst->instance.floatValue;
                    uri->resourceId = attributeB_floatValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[0],mid,CIS_RESPONSE_CONTINUE);

                    tmpdata[1].type = cis_data_type_string;
                    tmpdata[1].asBuffer.length = utils_strlen(inst->instance.strValue);
                    tmpdata[1].asBuffer.buffer = (uint8_t*)(inst->instance.strValue);//(uint8_t*)utils_strdup(inst->instance.strValue);
                    uri->resourceId = attributeB_stringValue;
                    cis_uri_update(uri);
                    cis_response(context,uri,&tmpdata[1],mid,CIS_RESPONSE_READ);
                }
            }
            break;
        }
    }
}


void prv_discoverResponse(void* context,cis_uri_t* uri,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectList[index].oid ==  uri->objectId){
            object = &g_objectList[index];
        }
    }

    if(object == NULL){
        return;
    }


    switch(uri->objectId)
    {
        case SAMPLE_OID_A:
        {

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeA_boolValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeA_stringValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

        }
        break;
        case SAMPLE_OID_B:
        {
            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeB_floatValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = attributeB_stringValue;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);

            uri->objectId = URI_INVALID;
            uri->instanceId = URI_INVALID;
            uri->resourceId = actionB_1;
            cis_uri_update(uri);
            cis_response(context,uri,NULL,mid,CIS_RESPONSE_CONTINUE);
        }
        break;
    }
    cis_response(context,NULL,NULL,mid,CIS_RESPONSE_DISCOVER);
}


void prv_writeResponse          (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t count,cis_mid_t mid)
{

    uint8_t index;
    st_sample_object* object = NULL;


    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return;
    }

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectList[index].oid ==  uri->objectId){
            object = &g_objectList[index];
        }
    }

    if(object == NULL){
        return;
    }


    switch(object->oid)
    {
        case SAMPLE_OID_A:
        {
            if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                return;
            }
            st_instance_a *inst = &g_instList_a[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            for (int i=0;i<count;i++)
            {
                printf("write %d/%d/%d\n",uri->objectId,uri->instanceId,value[i].id);
                if (value[i].id == attributeA_boolValue)
                {
                    if(value[i].type == cis_data_type_string){
                        inst->instance.boolValue = atoi((const char*)value[i].asBuffer.buffer);
                    }else{
                        inst->instance.boolValue = value[i].value.asBoolean;
                    }
                    if (inst->instance.boolValue) {
                        LOS_EvbLedControl(LOS_LED1, LED_ON);
                    } else {
                        LOS_EvbLedControl(LOS_LED1, LED_OFF);
                    }

                }
            }
        }
        break;
        case SAMPLE_OID_B:
        {
            if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                return;
            }
            st_instance_b *inst = &g_instList_b[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            for (int i=0;i<count;i++)
            {
                printf("write %d/%d/%d\n",uri->objectId,uri->instanceId,value[i].id);
                switch(value[i].id)
                {
                    case attributeB_floatValue:
                    {
                        if(value[i].type == cis_data_type_opaque){
                            inst->instance.floatValue = atof((const char*)value[i].asBuffer.buffer);
                        }else{
                            inst->instance.floatValue = value[i].value.asFloat;
                        }
                    }
                    break;
                    case  attributeB_stringValue:
                    {
                        memset(inst->instance.strValue,0,sizeof(inst->instance.strValue));
                        strncpy(inst->instance.strValue,(char*)value[i].asBuffer.buffer,value[i].asBuffer.length);
                    }
                    break;
                }
            }
        }
        break;
    }

    cis_response(context,NULL,NULL,mid,CIS_RESPONSE_WRITE);
}


void prv_execResponse              (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid)
{

    uint8_t index;
    st_sample_object* object = NULL;


    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectList[index].oid ==  uri->objectId){
            object = &g_objectList[index];
        }
    }

    if(object == NULL){
        return;
    }

    switch(object->oid)
    {
        case SAMPLE_OID_A:
        {
            if(uri->instanceId > SAMPLE_A_INSTANCE_COUNT){
                return;
            }
            st_instance_a *inst = &g_instList_a[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }
        }
        break;
        case SAMPLE_OID_B:
        {
            if(uri->instanceId > SAMPLE_B_INSTANCE_COUNT){
                return;
            }
            st_instance_b *inst = &g_instList_b[uri->instanceId];
            if(inst == NULL || inst->enabled == false){
                return;
            }

            if(uri->resourceId == actionB_1)
            {
                printf("exec actionB_1\n");
                cis_response(context,NULL,NULL,mid,CIS_RESPONSE_EXECUTE);
            }else{
                return;
            }
        }
        break;
    }
}


void prv_paramsResponse         (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid)
{
    uint8_t index;
    st_sample_object* object = NULL;


    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("prv_params:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return;
    }

    for (index = 0;index< SAMPLE_OBJECT_MAX;index++)
    {
        if(g_objectList[index].oid ==  uri->objectId){
            object = &g_objectList[index];
        }
    }

    if(object == NULL){
        return;
    }

    /*set parameter to observe resource*/
    /*do*/

    cis_response(context,NULL,NULL,mid,CIS_RESPONSE_OBSERVE_PARAMS);

}



static cis_data_t* prv_dataDup(const cis_data_t* value,cis_attrcount_t attrcount)
{
    cis_data_t* newData;
    newData =(cis_data_t*)cis_malloc(attrcount * sizeof(cis_data_t));
    if(newData == NULL)
    {
        return NULL;
    }
    cis_attrcount_t index;
    for (index =0;index<attrcount;index++)
    {
        newData[index].id = value[index].id;
        newData[index].type = value[index].type;
        newData[index].asBuffer.length = value[index].asBuffer.length;
        newData[index].asBuffer.buffer = (uint8_t*)cis_malloc(value[index].asBuffer.length);
        cissys_assert(newData[index].asBuffer.buffer !=NULL);
        memcpy(newData[index].asBuffer.buffer,value[index].asBuffer.buffer,value[index].asBuffer.length);


        memcpy(&newData[index].value.asInteger,&value[index].value.asInteger,sizeof(newData[index].value));
    }
    return newData;
}


//////////////////////////////////////////////////////////////////////////
cis_coapret_t cis_onRead		        (void* context,cis_uri_t* uri,cis_mid_t mid)
{
    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
    cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_READ;
    newNode->mid = mid;
    newNode->uri = *uri;
    g_callbackList = (struct st_callback_info*)CIS_LIST_ADD(g_callbackList,newNode);

    printf("cis_onRead:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);

    return CIS_CALLBACK_CONFORM;
}

cis_coapret_t cis_onDiscover(void* context,cis_uri_t* uri,cis_mid_t mid)
{

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
    cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_DISCOVER;
    newNode->mid = mid;
    newNode->uri = *uri;
    g_callbackList = (struct st_callback_info*)CIS_LIST_ADD(g_callbackList,newNode);

    printf("cis_onDiscover:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);

    return CIS_CALLBACK_CONFORM;
}

cis_coapret_t cis_onWrite		        (void* context,cis_uri_t* uri,const cis_data_t* value,cis_attrcount_t attrcount,cis_mid_t mid)
{

    st_sample_object* object = NULL;

    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("cis_onWrite:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }
    else{
        printf("cis_onWrite:(%d/%d)\n",uri->objectId,uri->instanceId);
    }
    printf("rcv write %s\r\n",value->asBuffer.buffer);

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
    cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_WRITE;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asWrite.count = attrcount;
    newNode->param.asWrite.value = prv_dataDup(value,attrcount);
    g_callbackList = (struct st_callback_info*)CIS_LIST_ADD(g_callbackList,newNode);

    return CIS_CALLBACK_CONFORM;
}


cis_coapret_t cis_onExec              (void* context,cis_uri_t* uri,const uint8_t* value,uint32_t length,cis_mid_t mid)
{
    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("cis_onExec:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }
    else{
        return CIS_CALLBACK_METHOD_NOT_ALLOWED;
    }

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return CIS_CALLBACK_BAD_REQUEST;
    }

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
    cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_EXECUTE;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asExec.buffer = (uint8_t*)cis_malloc(length);
    cissys_assert(newNode->param.asExec.buffer !=NULL);
    newNode->param.asExec.length = length;
    memcpy(newNode->param.asExec.buffer,value,length);
    g_callbackList = (struct st_callback_info*)CIS_LIST_ADD(g_callbackList,newNode);

    return CIS_CALLBACK_CONFORM;
}


cis_coapret_t cis_onObserve         (void* context,cis_uri_t* uri,bool flag,cis_mid_t mid)
{
    st_sample_object* object = NULL;

    printf("cis_onObserve mid:%d uri:(%d/%d/%d)\n",mid,uri->objectId,uri->instanceId,uri->resourceId);

    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return CIS_CALLBACK_BAD_REQUEST;
    }

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
    cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_OBSERVE;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asObserve.flag = flag;
    
    g_callbackList = (struct st_callback_info*)CIS_LIST_ADD(g_callbackList,newNode);

    return CIS_CALLBACK_CONFORM;
}


cis_coapret_t cis_onParams         (void* context,cis_uri_t* uri,cis_observe_attr_t parameters,cis_mid_t mid)
{
    if(CIS_URI_IS_SET_RESOURCE(uri)){
        printf("cis_on_params:(%d/%d/%d)\n",uri->objectId,uri->instanceId,uri->resourceId);
    }
    
    if(!CIS_URI_IS_SET_INSTANCE(uri))
    {
        return CIS_CALLBACK_BAD_REQUEST;
    }

    struct st_callback_info* newNode = (struct st_callback_info*)cis_malloc(sizeof(struct st_callback_info));
    cissys_assert(newNode !=NULL);
    newNode->next = NULL;
    newNode->flag = SAMPLE_CALLBACK_SETPARAMS;
    newNode->mid = mid;
    newNode->uri = *uri;
    newNode->param.asObserveParam.params = parameters;
    g_callbackList = (struct st_callback_info*)CIS_LIST_ADD(g_callbackList,newNode);

    return CIS_CALLBACK_CONFORM;
}

void cis_onEvent             (void* context,cis_evt_t eid,void* param)
{
    printf("cis_on_event id: %d : ",eid);

    switch(eid)
    {
        case CIS_EVENT_RESPONSE_FAILED:
        printf("CIS_EVENT_RESPONSE_FAILED: %d\n",(int32_t)param);
        break;
        case CIS_EVENT_NOTIFY_FAILED:
        printf("CIS_EVENT_NOTIFY_FAILED: %d\n",(int32_t)param);
        break;
        case CIS_EVENT_UPDATE_NEED:
        printf("CIS_EVENT_UPDATE_NEED: %d\n",(int32_t)param);
        cis_update_reg(g_context,g_lifetime,false);
        break;
        case CIS_EVENT_CONNECT_SUCCESS:
        printf("CIS_EVENT_CONNECT_SUCCESS: %d\n",(int32_t)param);
        break;
        case CIS_EVENT_REG_SUCCESS:
        printf("CIS_EVENT_REG_SUCCESS: %d\n",(int32_t)param);
        break;
        case CIS_EVENT_REG_TIMEOUT:
        printf("CIS_EVENT_REG_TIMEOUT: %d\n",(int32_t)param);
        break;
        case CIS_EVENT_UPDATE_SUCCESS:
        printf("CIS_EVENT_UPDATE_SUCCESS: %d\n",(int32_t)param);
        break;
        case CIS_EVENT_UPDATE_TIMEOUT:
        printf("CIS_EVENT_UPDATE_TIMEOUT: %d\n",(int32_t)param);
        break;				

        default:
        break;
    }
}
