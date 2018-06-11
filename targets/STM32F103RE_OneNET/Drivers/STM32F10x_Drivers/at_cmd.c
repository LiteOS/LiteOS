#include "string.h"
#include "ringbuf.h"
#include "at_cmd.h"
#include "delay.h"
#include <stdio.h>
#include <time.h>
sendmsg callback=NULL;
volatile char *flag_ok=NULL;
struct ringbuf *result_ptr=NULL;
void register_cmd_handler(sendmsg func,void *result_buf, volatile char *flag)
{
  callback=func;
  result_ptr=(struct ringbuf *)result_buf;
  flag_ok=flag;
}
 char SendCmd(char* cmd, uint8_t *result,uint16_t timeout,uint8_t retry,uint16_t waittime)
{
//	printf("send cmd:%s",cmd);	
	char *msg_p=NULL;
	time_t  nowtime=0,newtime=0,sum=0;
	uint8_t retry_num=0,retryflag=0;
   if(callback==NULL||result_ptr==NULL||flag_ok==NULL)
     return 2 ;
	 *flag_ok=0;
   ringbuf_clear(result_ptr);//清除之前可能残留的信息
	 printf("cmd:%s\r\n",cmd);
   callback((uint8_t*)cmd, strlen((const char *)cmd));
  // ringbuf_clear(result_ptr);//清除之前可能残留的信息
	   nowtime= nbiot_time();
     while(1)
   {
      if(sum>waittime)
			{
				if(++retry_num>retry)
					return 0; 
				retryflag=1;
			}
      if(*flag_ok==1)
      {
         printf("cmd_rsp:%s",result_ptr->data);
         *flag_ok=0;
         msg_p=strstr(result_ptr->data,result);
         if(msg_p!=NULL){
			   	ringbuf_clear(result_ptr);
          break;
				 }else{
					   
					   msg_p=strstr(result_ptr->data,"ERROR");
					   ringbuf_clear(result_ptr);	
					   if(msg_p!=NULL)
							return 2; 
					/*	 
             if(retry==1){
						   mDelay(3000);
				       printf("retry cmd:%s",cmd);
				       callback((uint8_t*)cmd, strlen((const char *)cmd));							   
						 }	
          */						 
			  }
      }else{
			    newtime=nbiot_time();
				  sum=newtime-nowtime;					
			}
			if(retryflag==1){
				retryflag=0;
				sum=0;
				nowtime=nbiot_time();
      if(retry_num>0&&retry_num<retry+1){
			
			  	printf("retry cmd:%s",cmd);
				  callback((uint8_t*)cmd, strlen((const char *)cmd));	
			}
		}	
   }
   mDelay(timeout);
	 return 1;
}
void SentData(char* cmd, uint8_t *result,uint16_t timeout)
{
  	char *msg_p=NULL;
	  time_t  nowtime=0,newtime=0,sum=0;
   if(callback==NULL||result_ptr==NULL||flag_ok==NULL)
     return ;
	 *flag_ok=0;
   ringbuf_clear(result_ptr);//清除之前可能残留的信息
	 printf("Data:%s\r\n",cmd);
   callback((uint8_t*)cmd, strlen((const char *)cmd));
  // ringbuf_clear(result_ptr);//清除之前可能残留的信息
	  nowtime= nbiot_time();
	  
     while(1)
   {
      if(sum>2)
				break;
      if(*flag_ok==1)
      {			 
         *flag_ok=0;
				  printf("data_rsp:%s\r\n",result_ptr->data);
				/*
         msg_p=strstr(result_ptr->data,result);
         if(msg_p!=NULL){
			   	ringbuf_clear(result_ptr);
          break;
				 }*/
			  	break;
      }else{
			
			    newtime=nbiot_time();
				  sum=newtime-nowtime;
			}
   
   }
   mDelay(timeout);
}
