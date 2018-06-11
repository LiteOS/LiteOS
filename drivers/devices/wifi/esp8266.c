
#if defined(WITH_AT_FRAMEWORK) && defined(USE_ESP8266)
#include "esp8266.h"
#include "atadapter.h"
#include "at_api_interface.h"
#include "atiny_socket.h"

extern at_task at;



int32_t esp8266_echo_off(void)
{
    return at.cmd((int8_t*)AT_CMD_ECHO_OFF, strlen(AT_CMD_ECHO_OFF), "OK\r\n", NULL);
}

int32_t esp8266_reset(void)
{
    return at.cmd((int8_t*)AT_CMD_RST, strlen(AT_CMD_RST), "ready\r\n", NULL);
}

int32_t esp8266_choose_net_mode(enum_net_mode m)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d", AT_CMD_CWMODE, (int)m);
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK\r\n", NULL); 
}

int32_t esp8266_set_mux_mode(int32_t m)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d", AT_CMD_MUX, (int)m);
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK\r\n", NULL);
}
int32_t esp8266_joinap(char * pssid, char * ppasswd)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=\"%s\",\"%s\"", AT_CMD_JOINAP, pssid, ppasswd);
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK\r\n", NULL); 
}

int32_t esp8266_connect(const int8_t * host, const int8_t *port, int32_t proto)
{
    int32_t ret = AT_FAILED;
    int32_t id = 0;
    char cmd[64] = {0};

    AT_LOG("host:%s, port:%s", host, port);

    if (AT_MUXMODE_SINGLE == at.mux_mode)
    {
        snprintf(cmd, 64, "%s=\"%s\",\"%s\",%s", AT_CMD_CONN, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
    }
    else 
    {
        id = at.get_id();
        if (id < 0 || id >= AT_MAX_LINK_NUM)
        {
            AT_LOG("no vailed linkid for use(id = %d)", id);
            return -1;
        }
        snprintf(cmd, 64, "%s=%d,\"%s\",\"%s\",%s", AT_CMD_CONN, id, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
    }

    ret = LOS_QueueCreate("dataQueue", 16, &at.linkid[id].qid, 0, sizeof(QUEUE_BUFF));
    if (ret != LOS_OK)
    {
        AT_LOG("init dataQueue failed!");
        at.linkid[id].usable = AT_LINK_UNUSE;
        return -1;
    }
    ret = at.cmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL);
    if (AT_FAILED == ret)
    {
        AT_LOG("at.cmd return failed!");
        return -1;
    }
    return id;
}

int32_t esp8266_send(int32_t id , const uint8_t  *buf, uint32_t len)
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

 //   at.cmd(cmd, strlen(cmd), ">", NULL);
    ret = at.write((int8_t *)cmd, "SEND OK\r\n", (int8_t*)buf, len);

    return ret;

}

int32_t esp8266_recv(int32_t id, int8_t * buf, uint32_t len)
{
   uint32_t qlen = sizeof(QUEUE_BUFF);

    QUEUE_BUFF  qbuf = {0, NULL};
    int ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, LOS_WAIT_FOREVER);
    AT_LOG("ret = %x, len = %d", ret, qbuf.len);
    if (ret != LOS_OK)
    {
        return -1;
    }

    if (qbuf.len){
        memcpy(buf, qbuf.addr, qbuf.len);
        atiny_free(qbuf.addr);
    }
    return qbuf.len;
}

int32_t esp8266_recv_timeout(int32_t id, int8_t * buf, uint32_t len, int32_t timeout)
{
   uint32_t qlen = sizeof(QUEUE_BUFF);

    QUEUE_BUFF  qbuf = {0, NULL};
    int ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, timeout);
    AT_LOG("ret = %x, len = %d, id = %d", ret, qbuf.len, id);
    if (ret != LOS_OK)
    {
        return -1;
    }

    if (qbuf.len){
        memcpy(buf, qbuf.addr, qbuf.len);
        atiny_free(qbuf.addr);
    }
    return qbuf.len;
}

int32_t esp8266_close(int32_t id)
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
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK\r\n", NULL);
}

int32_t esp8266_data_handler(void * arg, int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("entry!");

    //process data frame ,like +IPD,linkid,len:data
    int32_t ret = -1;
    int32_t linkid = 0, data_len = 0;
    char * p1, *p2;
    QUEUE_BUFF qbuf;
    p1 = (char *)buf;

    if (0 == memcmp(p1, AT_DATAF_PREFIX, strlen(AT_DATAF_PREFIX)))
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
        memcpy(qbuf.addr, p2, data_len);

        if (LOS_OK != (ret = LOS_QueueWriteCopy(at.linkid[linkid].qid, &qbuf, sizeof(QUEUE_BUFF), 0)))
        {
            AT_LOG("LOS_QueueWriteCopy  failed! ret = %x", ret);
            atiny_free(qbuf.addr);
            goto END;
        }
        ret = (p2 + data_len - (char*)buf);
    }
END:
    return ret;
}

int8_t esp8266_get_localip(int8_t * ip, int8_t * gw, int8_t * mask)/*获取本地IP*/
{
    char resp[512] = {0};
    at.cmd((int8_t*)AT_CMD_CHECK_IP, strlen((char*)AT_CMD_CHECK_IP), "OK", resp);

    AT_LOG("resp:%s", resp);
    char * p1, *p2;
    p1 = strstr(resp, "ip");
    if (ip && p1)
    {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(ip, p1 + 1, p2 - p1 -1);
    }

    p1 = strstr(resp, "gateway");
    if (gw && p1)
    {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(gw, p1 + 1, p2 - p1 -1);
    }

    p1 = strstr(resp, "netmask");
    if (mask && p1)
    {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(mask, p1 + 1, p2 - p1 -1);
    }

//    printf("get ip :%s", resp);
    return NULL;
}

int8_t esp8266_get_localmac(int8_t * mac)/*获取本地IP*/
{
    char resp[512] = {0};    
    char * p1, *p2;

    at.cmd((int8_t*)AT_CMD_CHECK_MAC, strlen((char*)AT_CMD_CHECK_MAC), "OK", resp);
    AT_LOG("resp:%s", resp);

    p1 = strstr(resp, ":");
    if (mac && p1)
    {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(mac, p1 + 1, p2 - p1 -1);
    }


//    printf("get ip :%s", resp);
    return NULL;
}

int32_t esp8266_recv_cb(int32_t id)
{
    return -1;
}

int32_t esp8266_deinit(void)
{
    return AT_FAILED;
}

int32_t esp8266_init()
{
    at.init();
    //at.add_listener((int8_t*)AT_DATAF_PREFIX, NULL, esp8266_data_handler);
    at.oob_register(AT_DATAF_PREFIX, strlen(AT_DATAF_PREFIX), esp8266_data_handler);
#ifdef 	USE_USARTRX_DMA
    HAL_UART_Receive_DMA(&at_usart,&at.recv_buf[0],MAX_AT_RECV_LEN-1);
#endif
    esp8266_reset();  
    esp8266_echo_off();

    esp8266_choose_net_mode(STA);
    while(AT_FAILED == esp8266_joinap(WIFI_SSID, WIFI_PASSWD))
    {
        AT_LOG("connect ap failed, repeat...");
    };
    esp8266_set_mux_mode(at.mux_mode);

    static int8_t ip[32];
    static int8_t gw[32];
    static int8_t mac[32];
    esp8266_get_localip(ip, gw, NULL);
    esp8266_get_localmac(mac);
    AT_LOG("get ip:%s, gw:%s mac:%s", ip, gw, mac);
    return AT_OK;
}
at_config at_user_conf = {
    .name = AT_MODU_NAME,
    .usart = USART3,
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

    .init = esp8266_init,    
    .get_localmac = esp8266_get_localmac, /*获取本地MAC*/
    .get_localip = esp8266_get_localip,/*获取本地IP*/
    /*建立TCP或�UDP连接*/
    .connect = esp8266_connect,

    .send = esp8266_send,

    .recv_timeout = esp8266_recv_timeout,
    .recv = esp8266_recv,

    .close = esp8266_close,/*关闭连接*/
    .recv_cb = esp8266_recv_cb,/*收到各种事件处理，暂不实�?*/

    .deinit = esp8266_deinit,
};
#endif
