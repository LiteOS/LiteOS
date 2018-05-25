#include "esp8266.h"
#include "atadapter.h"
#include "at_api_interface.h"
#include "atiny_socket.h"

extern at_task at;
at_adaptor_api at_interface;

int32_t esp8266_reset(void)
{
	return at.write((int8_t*)AT_CMD_RST, strlen(AT_CMD_RST), "ready", NULL);
}

int32_t esp8266_choose_net_mode(enum_net_mode m)
{
	char cmd[64] = {0};
	snprintf(cmd, 64, "%s=%d\r\n", AT_CMD_CWMODE, (int)m);
	return at.write((int8_t*)cmd, strlen(cmd), "OK", "no change"); 
}

int32_t esp8266_set_mux_mode(int32_t m)
{
	char cmd[64] = {0};
	snprintf(cmd, 64, "%s=%d\r\n", AT_CMD_MUX, (int)m);
	return at.write((int8_t*)cmd, strlen(cmd), "OK", NULL);
}
int32_t esp8266_joinap(char * pssid, char * ppasswd)
{
	char cmd[64] = {0};
	snprintf(cmd, 64, "%s=\"%s\",\"%s\"\r\n", AT_CMD_JOINAP, pssid, ppasswd);
	return at.write((int8_t*)cmd, strlen(cmd), "OK", NULL); 
}

int32_t esp8266_init()
{
	at.init();
	esp8266_reset();
	esp8266_choose_net_mode(STA);
	while(0 == esp8266_joinap(WIFI_SSID, WIFI_PASSWD));
	esp8266_set_mux_mode(at.mux_mode);
    return AT_OK;
}


int32_t esp8266_connect(int32_t id, const int8_t * host, const int8_t *port, int32_t proto)
{
	char cmd[64] = {0};
	if (AT_MUXMODE_SIGNEL == at.mux_mode)
	{
		snprintf(cmd, 64, "%s=\"%s\",\"%s\",%s\r\n", AT_CMD_CONN, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
	}
	else 
	{
		snprintf(cmd, 64, "%s=%d,\"%s\",\"%s\",%s\r\n", AT_CMD_CONN, id, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
	}
    return at.write((int8_t *)cmd, strlen(cmd), "OK", NULL);
}

int32_t esp8266_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
	char cmd[64] = {0};
	if (AT_MUXMODE_SIGNEL == at.mux_mode)
	{
		snprintf(cmd, 64, "%s=%d\r\n", AT_CMD_SEND, len);
	}
	else
	{
		snprintf(cmd, 64, "%s=%d,%d\r\n", AT_CMD_SEND, id, len);
	}
	at.write((int8_t*)cmd, strlen(cmd), ">", NULL);

	return at.write((int8_t*)buf, len, "SEND OK", NULL);

}

int32_t esp8266_recv()
{
    return -1;
}

int32_t esp8266_recv_timeout(int32_t id, int8_t * buf, uint32_t len, int32_t timeout)
{
    return at.read(id, buf, len, timeout); 
}

int32_t esp8266_close(int32_t id)
{
	char cmd[64] = {0};
	if (AT_MUXMODE_SIGNEL == at.mux_mode)
	{
		snprintf(cmd, 64, "%s\r\n", AT_CMD_CLOSE);
	}
	else
	{
		snprintf(cmd, 64, "%s=%d\r\n", AT_CMD_CLOSE, id);
	}
	return at.write((int8_t*)cmd, strlen(cmd), "OK", NULL);
}

int8_t* esp8266_get_localip(void)/*获取本地IP*/
{
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

at_adaptor_api at_interface = {
	.name = (int8_t*)"esp8266",
//	.mux_mode = 0,     //use multi connect mode
	.timeout = 2000,   // 2000 tick
	.init = esp8266_init,
	

//	int32_t  (*get_id)(void);/*获取连接id，仅多连接时需要*/
	.get_localip = esp8266_get_localip,/*获取本地IP*/
	/*建立TCP或者UDP连接*/
	.connect = esp8266_connect,
	/*发送，当命令发送后，如果超过一定的时间没收到应答，要返回错误*/
	.send = esp8266_send,
    .recv_timeout = esp8266_recv_timeout,

	.close = esp8266_close,/*关闭连接*/
	.recv_cb = esp8266_recv_cb,/*收到各种事件处理，暂不实现 

	*/
	.deinit = esp8266_deinit,
};

