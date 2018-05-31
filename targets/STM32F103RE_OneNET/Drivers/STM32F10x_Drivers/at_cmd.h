#ifndef AT_CMD_H
#define AT_CMD_H
#include <stdint.h>
typedef void (*sendmsg)(uint8_t * buf, uint32_t len);
void register_cmd_handler(sendmsg func,void *result_buf,volatile char *flag);
char SendCmd(char* cmd, uint8_t *result,uint16_t timeout,uint8_t retry,uint16_t waittime);
void SentData(char* cmd, uint8_t *result,uint16_t timeout);
#endif
