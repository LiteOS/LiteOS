#ifndef _NEUL_BC95_H
#define _NEUL_BC95_H


typedef int (*neul_read)(char *buf, int maxrlen, int mode, int timeout);
typedef int (*neul_write)(char *buf, int writelen, int mode);

typedef struct _neul_dev_operation_t 
{
    neul_read dev_read;
    neul_write dev_write;
} neul_dev_operation_t;

typedef struct _remote_info_t
{
    unsigned short port;
    int socket;
    char ip[16];
}remote_info;

typedef struct _neul_dev_info_t 
{
    char *rbuf;
    char *wbuf;
    int remotecount;
    remote_info *addrinfo;
    neul_dev_operation_t *ops;
    char cdpip[16];
} neul_dev_info_t;

int neul_bc95_get_time(char *buf);
int neul_bc95_get_csq(void);
int neul_bc95_get_cfun(void);
int neul_bc95_get_cscon(void);
int neul_bc95_get_cereg(void);
int neul_bc95_get_cgatt(void);
int neul_bc95_set_npsmr(void);
int neul_bc95_get_npsmr(void);
int neul_bc95_get_id(char *buf);
int neul_bc95_get_nuestats(void);
int neul_bc95_set_upd_param(const char *rip, const unsigned short port, const int socket);
int neul_bc95_hw_detect(void);
int neul_bc95_set_imei(const char *imei, int len);
int neul_bc95_set_band(int band);
int neul_bc95_active_network(void);
int neul_bc95_get_netstat(void);
int neul_bc95_query_ip(void);
int neul_bc95_reboot(void);
int neul_bc95_set_cdpserver(const char *ipaddr);
int neul_bc95_send_coap_paylaod(const char *buf, int sendlen);
int neul_bc95_get_unrmsg_count(void);
int neul_bc95_read_coap_msg(char *outbuf, int maxrlen);
int neul_bc95_create_udpsocket(unsigned short localport);
int neul_bc95_close_udpsocket(int socket);
int neul_bc95_socket_config_remoteinfo(int socket, const char *remoteip, unsigned short port);
int neul_bc95_udp_send(int socket, const char *buf, int sendlen);
int neul_bc95_udp_read(int socket,char *buf, int maxrlen, int mode);
int neul_bc95_hw_init(void);
int neul_bc95_create_data_task(void);
int neul_bc95_set_auto_connect(int flag);
int neul_bc95_sleep(int ticks);
int neul_bc95_prepare_transmit(void);
int uart_data_flush(void);
#endif 

