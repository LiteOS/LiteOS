#include "los_typedef.h"
#include "los_task.h"

void uart_init(void);
int uart_read(unsigned char *buf,int len,int timeout);
int uart_write(unsigned char *buf,int len);
UINT32 creat_maintask(const char *taskname,TSK_ENTRY_FUNC fnTask,UINT32 stackisize,UINT32 uwArg,UINT16 prior);
