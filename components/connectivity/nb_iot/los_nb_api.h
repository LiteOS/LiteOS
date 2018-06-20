#ifndef __NB_IOT_H__
#define __NB_IOT_H__
#include "atadapter.h"
#include "bc95.h"

typedef struct sec_param{
char* psk;
char* pskid;
uint8_t psk_len;
}sec_param_s;

extern at_task at;

int los_nb_init(const int8_t* host, const int8_t* port, sec_param_s* psk);
int los_nb_report(const char* buf, int buflen);
int los_nb_notify(oob_callback callback);
int los_nb_deinit(void);
#endif
