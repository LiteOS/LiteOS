
#if defined(WITH_AT_FRAMEWORK)

#include "atadapter.h"
#include "at_hal.h"


/* FUNCTION */
void at_init();
int32_t at_read(int32_t id, int8_t * buf, uint32_t len, int32_t timeout);
int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
int32_t at_get_unuse_linkid();
void at_listener_list_add(at_listener * p);
void at_listner_list_del(at_listener * p);
int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
int32_t at_oob_register(char* featurestr,int cmdlen, oob_callback callback);

//init function for at struct
at_task at = {
    .init = at_init,
    .cmd = at_cmd,
    .write = at_write,
    .oob_register = at_oob_register,
    .get_id = at_get_unuse_linkid,
};
at_oob_t at_oob;

//add p to tail;
void at_listener_list_add(at_listener * p)
{
    at_listener * head = at.head;

    if (NULL == head)
    {
        at.head = p;
        p->next = NULL;
        return;
    }
}

void at_listner_list_del(at_listener * p)
{
    at_listener * head = at.head;

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

    if (i >= 0 && i < at_user_conf.linkid_num)
        at.linkid[i].usable = AT_LINK_INUSE;
    return i;
}


void store_resp_buf(int8_t * resp_buf, int8_t * buf, uint32_t len)
{
    if (NULL == buf || 0 == len || len > at_user_conf.user_buf_len)
        return;

    strcat((char*)resp_buf, (char *)buf);
}

int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
{
    at_listener listener;
    int ret = AT_FAILED;

    listener.suffix = (int8_t *)suffix;
    listener.resp = (int8_t*)rep_buf;

    LOS_MuxPend(at.cmd_mux, LOS_WAIT_FOREVER);
    at_listener_list_add(&listener);

    at_transmit((uint8_t*)cmd, len,1);
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

int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len)
{
    at_listener listener;
    int ret = AT_FAILED;

    listener.suffix = (int8_t *)suffix;
    listener.resp = NULL;

    LOS_MuxPend(at.cmd_mux, LOS_WAIT_FOREVER);
    at_listener_list_add(&listener);

    at_transmit((uint8_t*)cmd, strlen((char*)cmd), 1);
    LOS_TaskDelay(200);

    at_transmit((uint8_t*)buf, len, 0);
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

int cloud_cmd_matching(int8_t * buf, int32_t len)
{
    int32_t ret = 0;
    int i;
//    char* buf = (char*)at.userdata;
    for(i=0;i<at_oob.oob_num;i++){
        //ret = strstr(at_oob.oob[i].featurestr,buf);
        if(memcmp(at_oob.oob[i].featurestr,buf,at_oob.oob[i].len) == 0)
        {
            AT_LOG("cloud match:%s buf:%s",at_oob.oob[i].featurestr,buf);
            if(at_oob.oob[i].cb != NULL)
                ret = at_oob.oob[i].cb(at_oob.oob[i].arg,buf,len);
            return ret;
        }
    }
    return 0;
}

void at_recv_task(uint32_t p)
{
    uint32_t recv_len = 0;
    uint8_t * tmp = at.userdata; //[MAX_USART_BUF_LEN] = {0};
    int ret = 0;
    at_listener * listener = NULL;

    while(1){
        LOS_SemPend(at.recv_sem, LOS_WAIT_FOREVER);

        memset(tmp, 0, at_user_conf.user_buf_len);
        recv_len = read_resp(tmp);

        if (0 >= recv_len)
            continue;

        int32_t data_len = 0;
        char * p1, * p2;
        AT_LOG_DEBUG("get recv_len = %lu buf = %s", recv_len, tmp);

        p1 = (char *)tmp;
        p2 = (char *)(tmp + recv_len);
        for (;;){

            data_len = p2 - p1;
            if (data_len <= 0)
                break;

            char * suffix;
            ret = cloud_cmd_matching((int8_t*)p1, data_len);
            if(ret > 0)
            {
                p1 += ret;
                continue;
            }

            listener = at.head;
            if (NULL == listener)
                break;

            if(listener->suffix == NULL)//用户不需要得到回应，分两种情况，一种rbuf直接丢弃，一种rbuf交给下一上报任务
            {
                //listener->resp这里存的时候要判断是否非空
                //store_resp_buf((int8_t *)listener->resp, (int8_t*)p1, p2 - p1);
                LOS_SemPost(at.resp_sem);
                listener = NULL;
                break;
            }

            suffix = strstr(p1, (const char*)listener->suffix);
            AT_LOG_DEBUG("GOT suffix (%s) at %p", listener->suffix, suffix);

            if (NULL == suffix)
            {
                if(NULL != listener->resp)
                    store_resp_buf((int8_t*)listener->resp, (int8_t*)p1, p2 - p1);
            }
            else 
            {
                if(NULL != listener->resp)
                    store_resp_buf((int8_t *)listener->resp, (int8_t*)p1, suffix + strlen((char*)listener->suffix) - p1);
                LOS_SemPost(at.resp_sem);
            } 
            break;
        }
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

    uwRet = LOS_TaskCreate((UINT32*)&at.tsk_hdl, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}

void at_init_oob(void)
{
    at_oob.oob_num = 0;
    memset(at_oob.oob,0,OOB_MAX_NUM * sizeof(struct oob_s));
}

int32_t at_struct_init(at_task * at)
{
    int ret = -1;
    if (NULL == at)
    {
        AT_LOG("invaild param!");
        return ret;
    }

    ret = LOS_SemCreate(0, (UINT32*)&at->recv_sem);
    if (ret != LOS_OK)
    {
        AT_LOG("init at_recv_sem failed!");
        goto at_recv_sema_failed;
    }


    ret = LOS_MuxCreate((UINT32*)&at->cmd_mux);
    if (ret != LOS_OK)
    {
        AT_LOG("init cmd_mux failed!");
        goto at_cmd_mux_failed;
    }

    ret = LOS_SemCreate(0, (UINT32*)&at->resp_sem);
    if (ret != LOS_OK)
    {
        AT_LOG("init resp_sem failed!");
        goto at_resp_sem_failed;
    }

    at->recv_buf = atiny_malloc(at_user_conf.user_buf_len);
    if (NULL == at->recv_buf)
    {
        AT_LOG("malloc recv_buf failed!");
        goto malloc_recv_buf;
    }

    at->cmdresp = atiny_malloc(at_user_conf.user_buf_len);
    if (NULL == at->cmdresp)
    {
        AT_LOG("malloc cmdresp failed!");
        goto malloc_resp_buf;
    }

    at->userdata = atiny_malloc(at_user_conf.user_buf_len);
    if (NULL == at->userdata)
    {
        AT_LOG("malloc userdata failed!");
        goto malloc_userdata_buf;
    }

    at->linkid = (at_link*)atiny_malloc(at_user_conf.linkid_num * sizeof(at_link));
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
        atiny_free(at->userdata);
    malloc_userdata_buf:
        atiny_free(at->cmdresp);
    malloc_resp_buf:
        atiny_free(at->recv_buf);
    malloc_recv_buf:
        LOS_SemDelete(at->resp_sem);
    at_resp_sem_failed:
        LOS_MuxDelete(at->cmd_mux);
    at_cmd_mux_failed:
        LOS_SemDelete(at->recv_sem);
    at_recv_sema_failed:
        return AT_FAILED;
}

void at_init()
{    
    AT_LOG("Config %s......\n", at_user_conf.name);

    LOS_TaskDelay(200);
    if (AT_OK != at_struct_init(&at))
    {
        AT_LOG("prepare AT struct failed!");
        return;
    }
    at_init_oob();
    at_usart_config();
    create_at_recv_task();

    AT_LOG("Config complete!!\n");

}

int32_t at_oob_register(char* featurestr,int cmdlen, oob_callback callback)
{
    oob_t *oob;
    if(at_oob.oob_num == OOB_MAX_NUM || cmdlen >= OOB_CMD_LEN - 1)
        return -1;
    oob = &(at_oob.oob[at_oob.oob_num++]);
    memcpy(oob->featurestr, featurestr, cmdlen);
    oob->len = strlen(featurestr);
    oob->cb = callback;
    return 0;
}
#endif
