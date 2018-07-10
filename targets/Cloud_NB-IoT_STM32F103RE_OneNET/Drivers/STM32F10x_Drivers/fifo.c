#include "fifo.h"
#include "memb.h"
#include "string.h"
pipe_t pipe[pipe_num]={0}; 
int8_t fifo_init(struct memb *ptr)
{
   int8_t i=0;
   for(i=0;i<pipe_num;i++)
     if(pipe[i].used==0){
        pipe[i].used=1;
        pipe[i].mem_b=ptr;
        memb_init(ptr);
        return i;
     }
   return -1;
}
int8_t fifo_put( int8_t pipe_id, uint16_t datalen, uint8_t data_buf[])
{
   uint8_t *ptr=NULL;
   uint16_t validlen=0,maxlen=0;
   if(pipe_id<0||pipe_id>=pipe_num)
      return -1;
    ptr=memb_alloc(pipe[pipe_id].mem_b);
   if(ptr==NULL)
      return -2;
    maxlen=pipe[pipe_id].mem_b->size-sizeof(uint16_t);
    validlen=(datalen>maxlen)?maxlen:datalen;
    *(uint16_t*)ptr=validlen;
    ptr=ptr+sizeof(uint16_t);
    memcpy(ptr,data_buf,validlen);
    return 0;
}
int16_t fifo_get(int8_t pipe_id,uint8_t *buf)
{
    uint8_t *ptr=NULL,i=0; 
    int16_t  datalen=0;
    struct memb *queue=NULL;
    if(pipe_id<0||pipe_id>=pipe_num)
        return -1;
     queue=pipe[pipe_id].mem_b;
     for(i=0;i<queue->num;i++)
     {
       if((queue->count)[i]!=0){
       
         ptr=(uint8_t *)queue->mem + (i * queue->size);
         datalen=*(uint16_t *) ptr;
         memcpy(buf,ptr+sizeof(uint16_t),datalen);
         memb_free(queue,ptr);
         return datalen;
       }
             
     }     
        return 0; 
}
