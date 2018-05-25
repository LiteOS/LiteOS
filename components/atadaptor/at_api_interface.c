
#include "at_api_interface.h"
#include "atadapter.h"

static at_adaptor_api  *gp_at_adaptor_api = NULL;
extern at_link g_at_link[AT_MAX_LINK_NUM];

int32_t at_api_register(at_adaptor_api *api)
{
    if (NULL == gp_at_adaptor_api)
    {
    	gp_at_adaptor_api = api;
    }
    return AT_OK;
}

int32_t at_api_init()
{
	if (gp_at_adaptor_api && gp_at_adaptor_api->init)
	{
		return gp_at_adaptor_api->init();
	}
	return AT_FAILED;
}

int32_t at_api_connect(const char* host, const char* port, int proto)
{
    int32_t ret = AT_FAILED;
    int32_t id = 0;

	if (gp_at_adaptor_api && gp_at_adaptor_api->connect)
	{
		id = at_get_unuse_linkid();
		if (id < 0 || id >= AT_MAX_LINK_NUM)
		{
			AT_LOG("no vailed linkid for use(id = %d)", id);
			return -1;
		}
        ret = LOS_QueueCreate("dataQueue", 16, &g_at_link[id].qid, 0, sizeof(QUEUE_BUFF));
	    if (ret != LOS_OK)
	    {
	        AT_LOG("init dataQueue failed!");
	        return  -1;
	    }
	    g_at_link[id].usable = 1;
		ret = gp_at_adaptor_api->connect(id, (int8_t *)host, (int8_t *)port, proto);
	}
	return id;
}

int32_t at_api_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
	if (gp_at_adaptor_api && gp_at_adaptor_api->send)
	{
		return gp_at_adaptor_api->send(id, buf, len);
	}
	return AT_FAILED;
}

int32_t at_api_recv()
{
//	if (gp_at_adaptor_api && gp_at_adaptor_api->recv)
	{
//		return gp_at_adaptor_api->recv();
	}
	return AT_FAILED;
}

int32_t at_api_recv_timeout(int32_t id, unsigned char* buf, size_t len, uint32_t timeout)
{
	if (gp_at_adaptor_api && gp_at_adaptor_api->recv_timeout)
	{
		return gp_at_adaptor_api->recv_timeout(id, (int8_t*)buf, len, timeout);
	}
	return AT_FAILED;
}

int32_t at_api_close(int32_t fd)
{
	if (gp_at_adaptor_api && gp_at_adaptor_api->close)
	{
		LOS_QueueDelete(g_at_link[fd].qid);
		g_at_link[fd].usable = 0;
		return gp_at_adaptor_api->close(fd);
	}
	return AT_FAILED;
}
