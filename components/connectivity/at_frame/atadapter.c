/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
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

#if defined(WITH_AT_FRAMEWORK)

#include "los_memory.h"
#include "atadapter.h"
#include "at_hal.h"
#ifdef WITH_SOTA
#include "at_fota.h"
#endif
extern uint8_t buff_full;
/* FUNCTION */
void at_init();
//int32_t at_read(int32_t id, int8_t * buf, uint32_t len, int32_t timeout);
int32_t at_write(int8_t *cmd, int8_t *suffix, int8_t *buf, int32_t len);
int32_t at_get_unuse_linkid();
void at_listener_list_add(at_listener *p);
void at_listner_list_del(at_listener *p);
int32_t at_cmd(int8_t *cmd, int32_t len, const char *suffix, char *rep_buf);
int32_t at_oob_register(char *featurestr, int cmdlen, oob_callback callback);

void at_deinit();
//init function for at struct
at_task at =
{
    .tsk_hdl = 0xFFFF,
    .recv_buf = NULL,
    .cmdresp = NULL,
    .userdata = NULL,
    .linkid = NULL,
    .head = NULL,

    .init = at_init,
    .deinit = at_deinit,
    .cmd = at_cmd,
    .write = at_write,
    .oob_register = at_oob_register,
    .get_id = at_get_unuse_linkid,
};
at_oob_t at_oob;
char rbuf[AT_DATA_LEN] = {0};
char wbuf[AT_DATA_LEN] = {0};

//add p to tail;
void at_listener_list_add(at_listener *p)
{
    at_listener *head = at.head;

    if (NULL == head)
    {
        at.head = p;
        p->next = NULL;
        return;
    }
}

void at_listner_list_del(at_listener *p)
{
    at_listener *head = at.head;

    if (p == head)
    {
        at.head = head->next;
        return;
    }
}

int32_t at_get_unuse_linkid()
{
    int i = 0;

    if (AT_MUXMODE_MULTI == at.mux_mode)
    {
        for (i = 0; i < at_user_conf.linkid_num; i++)
        {
            if (AT_LINK_UNUSE == at.linkid[i].usable)
            {
                break;
            }
        }
    }

    if (i < at_user_conf.linkid_num)
        at.linkid[i].usable = AT_LINK_INUSE;

    return i;
}


void store_resp_buf(int8_t *resp_buf, int8_t *buf, uint32_t len)
{
    if (NULL == buf || 0 == len || len > at_user_conf.user_buf_len)
        return;

    strcat((char *)resp_buf, (char *)buf);
}

int32_t at_cmd(int8_t *cmd, int32_t len, const char *suffix, char *rep_buf)
{
    at_listener listener;
    int ret = AT_FAILED;

    listener.suffix = (int8_t *)suffix;
    listener.resp = (int8_t *)rep_buf;
    AT_LOG("cmd:%s", cmd);

    LOS_MuxPend(at.cmd_mux, LOS_WAIT_FOREVER);
    at_listener_list_add(&listener);

    at_transmit((uint8_t *)cmd, len, 1);
    ret = LOS_SemPend(at.resp_sem, at.timeout);

    at_listner_list_del(&listener);
    LOS_MuxPost(at.cmd_mux);

    if (ret != LOS_OK)
    {
        AT_LOG("LOS_SemPend for listener.resp_sem failed(ret = %x)!", ret);
        return AT_FAILED;
    }
    return AT_OK;
}

int32_t at_write(int8_t *cmd, int8_t *suffix, int8_t *buf, int32_t len)
{
    at_listener listener;
    int ret = AT_FAILED;

    listener.suffix = (int8_t *)">";
    listener.resp = NULL;

    LOS_MuxPend(at.cmd_mux, LOS_WAIT_FOREVER);
    at_listener_list_add(&listener);

    at_transmit((uint8_t *)cmd, strlen((char *)cmd), 1);
    (void)LOS_SemPend(at.resp_sem, 200);
    listener.suffix = (int8_t *)suffix;

    at_listner_list_del(&listener);
    at_listener_list_add(&listener);
    at_transmit((uint8_t *)buf, len, 0);
    ret = LOS_SemPend(at.resp_sem, at.timeout);

    at_listner_list_del(&listener);
    LOS_MuxPost(at.cmd_mux);

    if (ret != LOS_OK)
    {
        AT_LOG("LOS_SemPend for listener.resp_sem failed(ret = %x)!", ret);
        return AT_FAILED;
    }
    return len;
}

int cloud_cmd_matching(int8_t *buf, int32_t len)
{
    int32_t ret = 0;
    char *cmp = NULL;
    int i;
    //    int rlen;
    //    memset(wbuf, 0, AT_DATA_LEN);

    for(i = 0; i < at_oob.oob_num; i++)
    {
        cmp = strstr((char *)buf, at_oob.oob[i].featurestr);
        if(cmp != NULL)
        {
            AT_LOG("cloud send cmd:%s", at_oob.oob[i].featurestr);
            cmp += at_oob.oob[i].len;
            //            sscanf(cmp,"%d,%s",&rlen,wbuf);
            if(at_oob.oob[i].callback != NULL)
                ret = at_oob.oob[i].callback(at_oob.oob[i].arg, (int8_t *)buf, (int32_t)len);
            return ret;
        }
    }
    return 0;
}

void at_recv_task(uint32_t p)
{
    uint32_t recv_len = 0;
    uint8_t *tmp = at.userdata;  //[MAX_USART_BUF_LEN] = {0};
    int ret = 0;
    at_listener *listener = NULL;

    while(1)
    {
        (void)LOS_SemPend(at.recv_sem, LOS_WAIT_FOREVER);
        do /*DMA方式接收消息队列最大为8，因此会循环*/
        {
            memset(tmp, 0, at_user_conf.user_buf_len);
            recv_len = read_resp(tmp);

            if (0 >= recv_len)
                continue;

            int32_t data_len = 0;
            char *p1, * p2;
            AT_LOG_DEBUG("recv len = %lu buf = %s buff_full = %d", recv_len, tmp, buff_full);

            p1 = (char *)tmp;
            p2 = (char *)(tmp + recv_len);
            for (;;)
            {

                data_len = p2 - p1;
                if (data_len <= 0)
                    break;

                char *suffix;
                ret = cloud_cmd_matching((int8_t *)p1, data_len);
                if(ret > 0)
                {
                    p1 += ret;
                    continue;
                }

                listener = at.head;
                if (NULL == listener)
                    break;

                if(listener->suffix == NULL)
                {

                    //store_resp_buf((int8_t *)listener->resp, (int8_t*)p1, p2 - p1);
                    (void)LOS_SemPost(at.resp_sem);
                    listener = NULL;
                    break;
                }

                suffix = strstr(p1, (const char *)listener->suffix);
                AT_LOG_DEBUG("GOT suffix (%s) at %p", listener->suffix, suffix);

                if (NULL == suffix)
                {
                    if(NULL != listener->resp)
                        store_resp_buf((int8_t *)listener->resp, (int8_t *)p1, p2 - p1);
                }
                else
                {
                    if(NULL != listener->resp)
                        store_resp_buf((int8_t *)listener->resp, (int8_t *)p1, suffix + strlen((char *)listener->suffix) - p1);
                    (void)LOS_SemPost(at.resp_sem);
                }
                break;
            }
        }
        while(recv_len > 0);
    }
}

uint32_t create_at_recv_task()
{
    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "at_recv_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)at_recv_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate((UINT32 *)&at.tsk_hdl, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}

void at_init_oob(void)
{
    at_oob.oob_num = 0;
    memset(at_oob.oob, 0, OOB_MAX_NUM * sizeof(struct oob_s));
}

int32_t at_struct_init(at_task *at)
{
    int ret = -1;
    if (NULL == at)
    {
        AT_LOG("invaild param!");
        return ret;
    }

    ret = LOS_SemCreate(0, (UINT32 *)&at->recv_sem);
    if (ret != LOS_OK)
    {
        AT_LOG("init at_recv_sem failed!");
        goto at_recv_sema_failed;
    }


    ret = LOS_MuxCreate((UINT32 *)&at->cmd_mux);
    if (ret != LOS_OK)
    {
        AT_LOG("init cmd_mux failed!");
        goto at_cmd_mux_failed;
    }

    ret = LOS_SemCreate(0, (UINT32 *)&at->resp_sem);
    if (ret != LOS_OK)
    {
        AT_LOG("init resp_sem failed!");
        goto at_resp_sem_failed;
    }
#ifndef USE_USARTRX_DMA
    at->recv_buf = at_malloc(at_user_conf.user_buf_len);
    if (NULL == at->recv_buf)
    {
        AT_LOG("malloc recv_buf failed!");
        goto malloc_recv_buf;
    }
#else
    at->recv_buf = at_malloc(at_user_conf.recv_buf_len);
    if (NULL == at->recv_buf)
    {
        AT_LOG("malloc recv_buf failed!");
        goto malloc_recv_buf;
    }
#endif

    at->cmdresp = at_malloc(at_user_conf.user_buf_len);
    if (NULL == at->cmdresp)
    {
        AT_LOG("malloc cmdresp failed!");
        goto malloc_resp_buf;
    }

    at->userdata = at_malloc(at_user_conf.user_buf_len);
    if (NULL == at->userdata)
    {
        AT_LOG("malloc userdata failed!");
        goto malloc_userdata_buf;
    }

    at->linkid = (at_link *)at_malloc(at_user_conf.linkid_num * sizeof(at_link));
    if (NULL == at->linkid)
    {
        AT_LOG("malloc for at linkid array failed!");
        goto malloc_linkid_failed;
    }

    at->head = NULL;
    at->mux_mode = at_user_conf.mux_mode;
    at->timeout = at_user_conf.timeout;
    return AT_OK;

    //        atiny_free(at->linkid);
malloc_linkid_failed:
    at_free(at->userdata);
malloc_userdata_buf:
    at_free(at->cmdresp);
malloc_resp_buf:
    at_free(at->recv_buf);
malloc_recv_buf:
    (void)LOS_SemDelete(at->resp_sem);
at_resp_sem_failed:
    (void)LOS_MuxDelete(at->cmd_mux);
at_cmd_mux_failed:
    (void)LOS_SemDelete(at->recv_sem);
at_recv_sema_failed:
    return AT_FAILED;
}

int32_t at_struct_deinit(at_task *at)
{
    int32_t ret = AT_OK;

    if(at == NULL)
    {
        AT_LOG("invaild param!");
        return AT_FAILED;
    }

    if (LOS_SemDelete(at->recv_sem) != LOS_OK)
    {
        AT_LOG("delete at.recv_sem failed!");
        ret = AT_FAILED;
    }

    if (LOS_MuxDelete(at->cmd_mux) != LOS_OK)
    {
        AT_LOG("delete at.cmd_mux failed!");
        ret = AT_FAILED;
    }

    if (LOS_SemDelete(at->resp_sem) != LOS_OK)
    {
        AT_LOG("delete at.resp_sem failed!");
        ret = AT_FAILED;
    }

    if (NULL != at->recv_buf)
    {
        at_free(at->recv_buf);
        at->recv_buf = NULL;
    }

    if (NULL != at->cmdresp)
    {
        at_free(at->cmdresp);
        at->cmdresp = NULL;
    }

    if (NULL != at->userdata)
    {
        at_free(at->userdata);
        at->userdata = NULL;
    }

    if (NULL != at->linkid)
    {
        at_free(at->linkid);
        at->linkid = NULL;
    }

    at->tsk_hdl = 0xFFFF;
    at->head = NULL;
    at->mux_mode = AT_MUXMODE_SINGLE;
    at->timeout = 0;

    return ret;
}

void at_init()
{
    AT_LOG("Config %s(buffer total is %lu)......\n", at_user_conf.name, at_user_conf.user_buf_len);

    LOS_TaskDelay(200);
    if (AT_OK != at_struct_init(&at))
    {
        AT_LOG("prepare AT struct failed!");
        return;
    }
    at_init_oob();

    if(AT_OK != at_usart_init())
    {
        AT_LOG("at_usart_init failed!");
        (void)at_struct_deinit(&at);
        return;
    }
    if(LOS_OK != create_at_recv_task())
    {
        AT_LOG("create_at_recv_task failed!");
        at_usart_deinit();
        (void)at_struct_deinit(&at);
        return;
    }
    //at_ota_init("+NNMI:",strlen("+NNMI:"));

    AT_LOG("Config complete!!\n");
}

void at_deinit()
{
    if(LOS_OK != LOS_TaskDelete(at.tsk_hdl))
    {
        AT_LOG("at_recv_task delete failed!");
    }
    at_usart_deinit();
    if(AT_OK != at_struct_deinit(&at))
    {
        AT_LOG("at_struct_deinit failed!");
    }
    at_init_oob();
    //if(at_fota_timer!=-1)
    //    LOS_SwtmrDelete(at_fota_timer);
}

int32_t at_oob_register(char *featurestr, int cmdlen, oob_callback callback)
{
    oob_t *oob;
    if(featurestr == NULL || at_oob.oob_num == OOB_MAX_NUM || cmdlen >= OOB_CMD_LEN - 1)
        return -1;
    oob = &(at_oob.oob[at_oob.oob_num++]);
    memcpy(oob->featurestr, featurestr, cmdlen);
    oob->len = strlen(featurestr);
    oob->callback = callback;
    return 0;
}

void *at_malloc(size_t size)
{
    void *pMem = LOS_MemAlloc(m_aucSysMem0, size);
    if(NULL != pMem)
    {
        memset(pMem, 0, size);
    }
    return pMem;
}

void at_free(void *ptr)
{
    (void)LOS_MemFree(m_aucSysMem0, ptr);
}

#endif
