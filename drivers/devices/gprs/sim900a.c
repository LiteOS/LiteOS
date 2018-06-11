
#if defined(WITH_AT_FRAMEWORK) && defined(USE_SIM900A)
#include "sim900a.h"
#include "atadapter.h"
#include "main.h"
#include "at_api_interface.h"
#include "atiny_socket.h"


extern at_task at;
at_adaptor_api at_interface;
char prefix_name[15];

int32_t sim900a_echo_off(void)
{
    return at.cmd((int8_t*)AT_CMD_ECHO_OFF, strlen(AT_CMD_ECHO_OFF), "OK\r\n", NULL);
}
int32_t sim900a_echo_on(void)
{
    return at.cmd((int8_t*)AT_CMD_ECHO_ON, strlen(AT_CMD_ECHO_OFF), "OK\r\n", NULL);
}
int32_t sim900a_reset(void)
{
    int32_t ret = 0;
    //at.cmd((int8_t*)AT_CMD_CLOSE,strlen(AT_CMD_CLOSE),"CLOSE OK","ERROR");
    ret = at.cmd((int8_t*)AT_CMD_SHUT,strlen(AT_CMD_SHUT),"SHUT OK", NULL);
    return ret;
}

int32_t sim900a_set_mux_mode(int32_t m)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d", AT_CMD_MUX, (int)m);
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK", NULL);
}

int32_t sim900a_connect(const int8_t * host, const int8_t * port, int proto)
{
    int32_t ret = AT_FAILED;
    int32_t id = at.get_id();
    sim900a_reset();
    char cmd1[64] = {0};
    snprintf(cmd1, 64, "%s=\"B\"", AT_CMD_CLASS);
    at.cmd((int8_t*)cmd1,strlen(cmd1),"OK", NULL);
    char cmd2[64] = {0};
    snprintf(cmd2, 64, "%s=1,\"IP\",\"CMNET\"", AT_CMD_PDP_CONT);
    at.cmd((int8_t*)cmd2,strlen(cmd2),"OK",NULL);
    char cmd3[64] = {0};
    snprintf(cmd3, 64, "%s=1", AT_CMD_PDP_ATT);
    at.cmd((int8_t*)cmd3,strlen(cmd3),"OK",NULL);
    char cmd4[64] = {0};
    snprintf(cmd4, 64, "%s=1", AT_CMD_CIPHEAD);
    at.cmd((int8_t*)cmd4,strlen(cmd4),"OK",NULL);
    char cmd5[64] = {0};

    AT_LOG("host:%s, port:%s", host, port);

    if (AT_MUXMODE_SINGLE == at.mux_mode)
    {
        snprintf(cmd5, 64, "%s=\"%s\",\"%s\",\"%s\"", AT_CMD_CONN, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
    }
    else
    {
        at.cmd((int8_t*)(AT_CMD_PDP_ACT"=1,1"),strlen(AT_CMD_PDP_ACT"=1,1"),"OK",NULL);
        at.cmd((int8_t*)AT_CMD_CSTT, strlen(AT_CMD_CSTT),"OK",NULL);
        at.cmd((int8_t*)AT_CMD_CIICR, strlen(AT_CMD_CIICR),"OK",NULL);
        at.cmd((int8_t*)AT_CMD_CIFSR, strlen(AT_CMD_CIFSR),"OK",NULL);
        snprintf(cmd5, 64, "%s=%d,\"%s\",\"%s\",\"%s\"", AT_CMD_CONN, id, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
    }
    if (id < 0 || id >= AT_MAX_LINK_NUM)
    {
        AT_LOG("no vailed linkid for use(id = %d)", id);
        return -1;
    }
    ret = LOS_QueueCreate("dataQueue", 16, &at.linkid[id].qid, 0, sizeof(QUEUE_BUFF));
    if (ret != LOS_OK)
    {
        AT_LOG("init dataQueue failed!");
        at.linkid[id].usable = AT_LINK_UNUSE;
        return  -1;
    }
    at.cmd((int8_t *)cmd5, strlen(cmd5), "CONNECT OK", NULL);
    return id;
}

int32_t  sim900a_recv(int32_t id, int8_t * buf, uint32_t len)
{
    uint32_t qlen = sizeof(QUEUE_BUFF);

    QUEUE_BUFF  qbuf = {0, NULL};
    int ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, LOS_WAIT_FOREVER);
    AT_LOG("ret = %x, len = %d", ret, qbuf.len);

    if (qbuf.len){
        memcpy(buf, qbuf.addr, qbuf.len);
        atiny_free(qbuf.addr);
    }
    return qbuf.len;
}

int32_t  sim900a_recv_timeout(int32_t id, int8_t * buf, uint32_t len, int32_t timeout)
{
    uint32_t qlen = sizeof(QUEUE_BUFF);

    QUEUE_BUFF  qbuf = {0, NULL};
    printf("****at.linkid[id].qid=%d***\n",at.linkid[id].qid);
    int ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, timeout);
    AT_LOG("ret = %x, len = %d, id = %d", ret, qbuf.len, id);

    if (qbuf.len){
        memcpy(buf, qbuf.addr, qbuf.len);
        atiny_free(qbuf.addr);
    }
    return qbuf.len;
}
int32_t sim900a_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
    int32_t ret = -1;
    char cmd[64] = {0};
    if (AT_MUXMODE_SINGLE == at.mux_mode)
    {
        snprintf(cmd, 64, "%s=%d", AT_CMD_SEND, len);
    }
    else
    {
        snprintf(cmd, 64, "%s=%d,%d", AT_CMD_SEND, id, len);
    }

    ret = at.write((int8_t *)cmd, "SEND OK", (int8_t*)buf, len);
    return ret;
}

void sim900a_check(void)
{
	//检测模块响应是否正常
    while(AT_FAILED == at.cmd((int8_t*)AT_CMD_AT,strlen(AT_CMD_AT),"OK",NULL))
    {
      printf("\r\n模块响应测试不正常！！\r\n");
      printf("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
      SIM900A_DELAY((osMs2Tick(500)));
    }
    if(AT_FAILED != at.cmd((int8_t*)AT_CMD_CPIN,strlen(AT_CMD_CPIN),"OK",NULL))
    {
      printf("检测到SIM卡\n");
    }
    if(AT_FAILED != at.cmd((int8_t*)AT_CMD_COPS,strlen(AT_CMD_COPS),"CHINA MOBILE",NULL))
    {
      printf("已注册到网络\n");
    }
}

int32_t sim900a_recv_cb(int32_t id)
{
    return -1;
}

int32_t sim900a_deinit(void)
{
    return AT_FAILED;
}
int32_t sim900a_close(int32_t id)
{
    char cmd[64] = {0};
    if (AT_MUXMODE_SINGLE == at.mux_mode)
    {
        snprintf(cmd, 64, "%s", AT_CMD_CLOSE);
    }
    else
    {
        LOS_QueueDelete(at.linkid[id].qid);
        at.linkid[id].usable = 0;
        snprintf(cmd, 64, "%s=%d", AT_CMD_CLOSE, id);
    }
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK", NULL);
}
int32_t sim900a_data_handler(void * arg, int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("entry!");

    //process data frame ,like +IPD,linkid,len:data
    int32_t ret = 0;
    int32_t linkid = 0, data_len = 0;
    char * p1, *p2;
    QUEUE_BUFF qbuf;
    p1 = (char *)buf;

    if (0 == memcmp(p1, prefix_name, strlen(prefix_name)))
    {
        p2 = strstr(p1, ",");
        if (NULL == p2)
        {
            AT_LOG("got data prefix invailed!");
            goto END;
        }

        if (AT_MUXMODE_MULTI == at.mux_mode)
        {
            linkid = 0;
            for (p2++; *p2 <= '9' && *p2 >= '0'; p2++)
            {
                linkid = linkid * 10 + (*p2 - '0');
            }
        }

        for (p2++; *p2 <= '9' && *p2 >= '0' ;p2++)
        {
            data_len = (data_len * 10 + (*p2 - '0'));
        }
        p2++; //over ':'

        qbuf.addr = atiny_malloc(data_len);
        if (NULL == qbuf.addr)
        {
            AT_LOG("malloc for qbuf failed!");
            goto END;
        }

        qbuf.len = data_len;
        if(AT_MUXMODE_MULTI == at.mux_mode)
        {
            p2++;
            p2++;//multi-connect prefix is +RECEIVE,0,13:\r\n+packet content
        }
        memcpy(qbuf.addr, p2, data_len);

        if (LOS_OK != (ret = LOS_QueueWriteCopy(at.linkid[linkid].qid, &qbuf, sizeof(QUEUE_BUFF), 0)))
        {
            AT_LOG("LOS_QueueWriteCopy  failed! ret = %x", ret);
            atiny_free(qbuf.addr);
            goto END;
        }
        ret = (p2 + data_len - (char *)buf);
    }
END:
    return ret;
}
int32_t sim900a_ini()
{
    at.init();
    //single and multi connect prefix is different
    if (AT_MUXMODE_MULTI == at.mux_mode)
    {
        memcpy(prefix_name, AT_DATAF_PREFIX_MULTI, sizeof(AT_DATAF_PREFIX_MULTI));
    }
    else
    {
        memcpy(prefix_name, AT_DATAF_PREFIX, sizeof(AT_DATAF_PREFIX));
    }
    at.oob_register((char*)prefix_name, strlen((char*)prefix_name), sim900a_data_handler);
    sim900a_echo_on();
    sim900a_check();
    sim900a_reset();
    sim900a_set_mux_mode(at.mux_mode);
    at.cmd((int8_t*)("AT+CIPMUX?"),strlen("AT+CIPMUX?"),"OK",NULL);
    return AT_OK;
}

at_config at_user_conf = {
    .name = AT_MODU_NAME,
    .usart = USART2,
    .buardrate = AT_BUARDRATE,
    .irqn = AT_USART_IRQn,
    .linkid_num = AT_MAX_LINK_NUM,
    .user_buf_len = MAX_AT_USERDATA_LEN,
    .cmd_begin = AT_CMD_BEGIN,
    .line_end = AT_LINE_END,
    .mux_mode = 1, //support multi connection mode
    .timeout = AT_CMD_TIMEOUT,   //  ms
};

at_adaptor_api at_interface = {
    .init = sim900a_ini,
//    .get_id = NULL, /*get connect id, only in multiconnect*/
    /*TCP or UDP connect*/
    .connect = sim900a_connect,
    /*send data, if no response, retrun error*/
    .send = sim900a_send,
    .recv_timeout = sim900a_recv_timeout,
    .recv = sim900a_recv,
    .close = sim900a_close,/*close connect*/
    .recv_cb = sim900a_recv_cb,/*receive event handle, no available by now */
    .deinit = sim900a_deinit,
};

#endif //#if NETWORK_TYPE == SIM_900A
