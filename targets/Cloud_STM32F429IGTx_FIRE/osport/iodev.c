#include "osport.h"


typedef struct
{
    u32_t  debugrxmode:2;   //1means ascii 2 hex while others means no debug
    u32_t  debugtxmode:2;   //1means ascii 2 hex while others means no debug
}tagIOCB;

static tagIOCB  gIOCB;
//import the uart here
extern s32_t uart_read(u8_t *buf,s32_t len,s32_t timeout);
extern s32_t uart_write(u8_t *buf,s32_t len,s32_t timeout);


#pragma weak uart_read
s32_t uart_read(u8_t *buf,s32_t len,s32_t timeout)
{
    return 0;
}
#pragma weak uart_write
s32_t uart_write(u8_t *buf,s32_t len,s32_t timeout)
{
    return 0;
}


//we do some port here:we port the uart
s32_t iodev_open(const char *name,s32_t flags,s32_t mode)
{
    s32_t fd =1;
    return fd;
}

s32_t iodev_read(s32_t fd,u8_t *buf,s32_t len,s32_t timeout)
{
    s32_t i = 0;
    s32_t ret;
    ret =uart_read(buf,len,timeout);
    if(ret > 0)
    {
        printf("RCV:%02x Bytes:",ret);
        for(i =0;i < ret;i++)
        {
            if(gIOCB.debugrxmode == 1)
            {
                printf("%c",buf[i]);
            }
            else if(gIOCB.debugrxmode  == 2)
            {
                printf(" %02x",buf[i]);
            }
            else
            {
            
            }
        }
        printf("\n\r");
    }
    return ret;
}
s32_t iodev_write(s32_t fd,u8_t *buf,s32_t len,s32_t timeout)
{
    s32_t ret;
    s32_t i;
    printf("SND:%02x Bytes:",len);
    for(i =0;i < len;i++)
    {
        if(gIOCB.debugtxmode == 1)
        {
            printf("%c",buf[i]);
        }
        else if(gIOCB.debugtxmode == 2)
        {
            printf(" %02x",buf[i]);
        }
        else
        {
        
        }
    }
    printf("\n\r");
    ret = uart_write(buf,len,timeout);
    return ret;
}
s32_t iodev_close(s32_t fd)
{    
    return 0;
}
s32_t iodev_flush(s32_t fd)
{
    unsigned char buf;
    s32_t ret;
    do{
        ret = iodev_read(fd,&buf,1,0);
    }while(ret > 0);
    return 0;
}
void  iodev_debugmode(s32_t rxtx,u32_t mode)
{
    if(rxtx == 0)
    {
        gIOCB.debugrxmode = mode;
    }
    else if(rxtx == 1)
    {
        gIOCB.debugtxmode = mode;
    }
    else if(rxtx == 2)
    {
        gIOCB.debugrxmode = mode;
        gIOCB.debugtxmode = mode;
    }
    else  //do nothing here
    {
    }
    return;
}

