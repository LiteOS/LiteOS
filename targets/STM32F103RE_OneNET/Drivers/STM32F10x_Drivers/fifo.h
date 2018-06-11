#ifndef FIFO_H
#define FIFO_H
#define pipe_num 10
#include <stdint.h>
#include "memb.h"
typedef struct{
  uint8_t used;
  struct memb *mem_b;
}pipe_t;
#define FIFO(name,queuelen,buflen) \
      struct name##_fifo{\
         uint16_t datalen;\
         uint8_t  databuf[buflen];\
      };\
     MEMB(name, struct name##_fifo, queuelen)     
int8_t fifo_init(struct memb *ptr);
int8_t fifo_put( int8_t pipe_id, uint16_t dataLen, uint8_t data_buf[]);
int16_t fifo_get(int8_t queue_id,uint8_t *buf);

#endif
		 
		 