#include <cis_if_net.h>
#include <cis_list.h>
#include <cis_log.h>
#include <cis_if_sys.h>
#include "M5310.h"
#include "cis_internals.h"

#define MAX_PACKET_SIZE			(1024)


struct st_cisnet_context
{
    int sock;
    char host[128];
    uint16_t port;
    int state;
};

static uint8_t gAttached = 1;
cisnet_callback_t g_callback;
cisnet_config_t g_netconfig;

uint8_t  cisnet_attached_state(void)
{
    return gAttached;
}

cis_ret_t cisnet_init(const cisnet_config_t* config,cisnet_callback_t cb)
{
    printf("fall in cisnet_init\r\n");
    memcpy(&g_netconfig,config,sizeof(cisnet_config_t));
    g_callback.onEvent = cb.onEvent;
    g_callback.userData = cb.userData;
    gAttached = 1;
    return CIS_RET_OK;
}

cis_ret_t cisnet_create(cisnet_t* ctx,const char* host)
{     
    if(gAttached != 1)
			return CIS_RET_ERROR;

    (*ctx) = (cisnet_t)cis_malloc(sizeof(struct st_cisnet_context));
		cissys_assert(ctx!=NULL);
    (*ctx)->sock = 0;
    (*ctx)->port = 5683;
    (*ctx)->state = 0;
    utils_stringCopy((*ctx)->host,sizeof((*ctx)->host),host);

    return CIS_RET_OK;
}

void  cisnet_destroy(cisnet_t ctx)
{
    cis_free(ctx);
}

#define M5310    1

cis_ret_t cisnet_connect(cisnet_t ctx)
{
#if M5310
    ctx->sock = 1;
	  g_callback.onEvent(ctx,cisnet_event_connected,NULL,g_callback.userData);
#else
  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_port = htons(5683);
  address.sin_addr.s_addr = inet_addr(ctx->host);

  if ((ctx->sock = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		return CIS_RET_ERROR;

  if (Connect(ctx->sock, (struct sockaddr*) &address, sizeof(address)) < 0)
  {
    Close(ctx->sock);
		ctx->sock = 0;
		return CIS_RET_ERROR;
  }
	g_callback.onEvent(ctx,cisnet_event_connected,NULL,g_callback.userData);

#endif
	
    return CIS_RET_OK;
}

cis_ret_t cisnet_disconnect(cisnet_t ctx)
{
#if M5310

#else
	
	if (ctx->sock != 0) {
	  Close(ctx->sock);
		ctx->sock = 0;
	}
#endif
	return 1;
}

static char dlbuf[1024];

cis_ret_t cisnet_write(cisnet_t ctx, const uint8_t * buffer, uint32_t length)
{
#if M5310
  char tmp[5];
	cis_memset(dlbuf, 0, sizeof(dlbuf));
  strcat(dlbuf, "AT+NSOST=0,");
	strcat(dlbuf, ctx->host);
	strcat(dlbuf, ",5683,");
	utils_itoa(length, tmp, 5);
	strcat(dlbuf, tmp);
	strcat(dlbuf, ",");
  ip_SendData((char *)buffer, dlbuf, length);

#else
	int rc = Send(ctx->sock, buffer, length, 0);
	if (rc <= 0) {
		printf("socket send rc == %d\n", rc);
	}
#endif
  return CIS_RET_OK;
}

cis_ret_t cisnet_read(cisnet_t ctx,uint8_t* buffer,uint32_t *length)
{
#if M5310
	netif_rx(buffer, (uint16_t *)length);
	if(*length > 0) {
		return CIS_RET_OK;
	} else
		return CIS_RET_ERROR;
#else
	
	struct timeval interval = {0, 0};
	Setsockopt(ctx->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

	int len = Recv(ctx->sock, buffer, *length, 0);
	if (len > 0) {
		*length = len;
	  return CIS_RET_OK;
	}
	return CIS_RET_ERROR;
#endif
}

cis_ret_t cisnet_free(cisnet_t ctx,uint8_t* buffer,uint32_t length)
{
    cis_free(buffer);
    return CIS_RET_ERROR;
}
