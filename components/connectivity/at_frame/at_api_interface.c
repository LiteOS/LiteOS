#if defined(WITH_AT_FRAMEWORK)
#include "at_api_interface.h"
#include "atadapter.h"


static at_adaptor_api  *gp_at_adaptor_api = NULL;

int32_t at_api_register(at_adaptor_api *api)
{
    if (NULL == gp_at_adaptor_api)
    {
    	gp_at_adaptor_api = api;
    }
	if (gp_at_adaptor_api && gp_at_adaptor_api->init)
	{
		return gp_at_adaptor_api->init();
	}
	return AT_FAILED;
}

int32_t at_api_connect(const char* host, const char* port, int proto)
{
    int32_t ret = AT_FAILED;

	if (gp_at_adaptor_api && gp_at_adaptor_api->connect)
	{
		ret = gp_at_adaptor_api->connect((int8_t *)host, (int8_t *)port, proto);
	}
	return ret;
}

int32_t at_api_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
	if (gp_at_adaptor_api && gp_at_adaptor_api->send)
	{
		return gp_at_adaptor_api->send(id, buf, len);
	}
	return AT_FAILED;
}

int32_t at_api_recv(int32_t id, unsigned char* buf, size_t len)
{
	if (gp_at_adaptor_api && gp_at_adaptor_api->recv)
	{
		return gp_at_adaptor_api->recv(id, (int8_t*)buf, len);
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
		return gp_at_adaptor_api->close(fd);
	}
	return AT_FAILED;
}

#endif
