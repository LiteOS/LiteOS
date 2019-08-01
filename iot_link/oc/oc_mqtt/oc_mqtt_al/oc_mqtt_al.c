/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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

#include <oc_mqtt_al.h>


#if  CONFIG_OC_MQTT_ENABLE

static tag_oc_mqtt_ops *s_oc_mqtt = NULL;


///////////////////////OC AGENT INSTALL INTERFACE///////////////////////////////////////
int oc_mqtt_register(const tag_oc_mqtt_ops *opt)
{
    int ret = -1;

    if(NULL != opt)
    {
        s_oc_mqtt = (tag_oc_mqtt_ops *) opt;

        ret = 0;
    }

    return ret;
}

//////////////////////////APPLICATION INTERFACE/////////////////////////////////
void* oc_mqtt_config(tag_oc_mqtt_config *param)
{
    void *ret = NULL;

    if((NULL != s_oc_mqtt) &&(NULL != s_oc_mqtt->config))
    {
       ret = s_oc_mqtt->config(param);
    }

    return ret;
}

int oc_mqtt_deconfig(void *handle)
{
    int ret = -1;

    if((NULL != handle)&&(NULL != s_oc_mqtt) \
       &&(NULL != s_oc_mqtt->deconfig))
    {
       ret = s_oc_mqtt->deconfig(handle);
    }

    return ret;
}

int oc_mqtt_report(void *handle, char *msg, int len, en_mqtt_al_qos_t qos)
{
    int ret = -1;

    if((NULL != handle)&&(NULL!= msg)&&\
       (NULL != s_oc_mqtt) &&(NULL != s_oc_mqtt->report))
    {
       ret = s_oc_mqtt->report(handle,msg,len,qos);
    }

    return ret;
}


///< format the report data to json string mode
static cJSON  *json_fmt_value(tag_key_value  *disc)
{
    cJSON  *ret = NULL;
    switch (disc->type)
    {
        case en_key_value_type_int:
            ret = cJSON_CreateNumber((double)(*(int *)disc->buf));
            break;
        case en_key_value_type_string:
            ret = cJSON_CreateString((const char *)disc->buf);
            break;
        default:
            break;
    }

    return ret;
}

cJSON *oc_mqtt_json_fmt_report(tag_oc_mqtt_report  *report)
{
    cJSON  *ret = NULL;
    cJSON  *service_data =  NULL;
    cJSON  *data_array = NULL;
    cJSON  *service = NULL;
    cJSON  *root;
    cJSON  *tmp;

    tag_key_value_list  *kvlst;

    ///< create the root object
    root = cJSON_CreateObject();
    if(NULL == root)
    {
        goto EXIT_CJSON_ERR;
    }
    ///< create the msg type object and add it to the root
    tmp = cJSON_CreateString(cn_msg_type_device_req);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_msg_type_name,tmp);
    ///< create the has more item and add it to the root
    tmp = cJSON_CreateNumber((double)report->hasmore);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_has_more_name,tmp);
    ///< create the data array and add it to the root
    data_array = cJSON_CreateArray();
    if(NULL == data_array)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_data_name,data_array);

    ///< create the service and add it to the data_array
    service = cJSON_CreateObject();
    if(NULL == service)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToArray(data_array,service);

    ///< create the service id and add to the service
    tmp = cJSON_CreateString(report->serviceid);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(service,cn_service_id_name,tmp);

    ///< create the service_data object and add it  to the service
    service_data = cJSON_CreateObject();
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(service,cn_service_data_name,service_data);

    ///< loop the report data and make the corresponding k-v object,then add it to the service_data
    kvlst = report->paralst;
    while(NULL != kvlst)
    {
        ///< fmt the value to a json object
        tmp = json_fmt_value(&kvlst->item);
        if(NULL == tmp)
        {
            goto EXIT_CJSON_ERR;
        }
        ///< add the object to the serice data
        cJSON_AddItemToObject(service_data,kvlst->item.name,tmp);

        kvlst= kvlst->next;
    }

    ///< create the time service_data object and add it to the service
    if(NULL != report->eventtime)
    {
        tmp = cJSON_CreateString(report->eventtime);
        if(NULL == tmp)
        {
            goto EXIT_CJSON_ERR;
        }
        cJSON_AddItemToObject(service,cn_service_time_name,tmp);
    }

    ret = root;
    return ret;


EXIT_CJSON_ERR:
    if(root)
    {
        cJSON_Delete(root);
    }

    return ret;
}

cJSON *oc_mqtt_json_fmt_response(tag_oc_mqtt_response  *response)
{
    cJSON  *ret = NULL;
    cJSON  *body =  NULL;
    cJSON  *root;
    cJSON  *tmp;

    tag_key_value_list  *kvlst;

    ///< create the root object
    root = cJSON_CreateObject();
    if(NULL == root)
    {
        goto EXIT_CJSON_ERR;
    }
    ///< create the msg type object and add it to the root
    tmp = cJSON_CreateString(cn_msg_type_device_resp);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_msg_type_name,tmp);
    ///< create the mid item and add it to the root
    tmp = cJSON_CreateNumber((double)response->mid);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_mid_name,tmp);
    ///< create the err code item and add it to the root
    tmp = cJSON_CreateNumber((double)response->errcode);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_err_code_name,tmp);
    ///< create the has more item and add it to the root
    tmp = cJSON_CreateNumber((double)response->hasmore);
    if(NULL == tmp)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_has_more_name,tmp);

    ///< create the body object and add it to root
    body = cJSON_CreateObject();
    if(NULL == body)
    {
        goto EXIT_CJSON_ERR;
    }
    cJSON_AddItemToObject(root,cn_body_name,body);

    ///< create the body paras object and add it to body
    kvlst = response->bodylst;

    while(NULL != kvlst)
    {
        ///< fmt the value to a json object
        tmp = json_fmt_value(&kvlst->item);
        if(NULL == tmp)
        {
            goto EXIT_CJSON_ERR;
        }
        ///< add the object to the serice data
        cJSON_AddItemToObject(body,kvlst->item.name,tmp);

        kvlst= kvlst->next;
    }

    ret = root;
    return ret;

EXIT_CJSON_ERR:
    if(root)
    {
        cJSON_Delete(root);
    }

    return ret;
}



///////////////////////OC LWM2M AGENT INITIALIZE////////////////////////////////
int oc_mqtt_init()
{
    return 0;
}


#endif

