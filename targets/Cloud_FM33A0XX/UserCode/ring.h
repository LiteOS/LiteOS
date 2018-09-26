//i think we need a ring buffer,you must take care of the sync and mutex during read and write
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
typedef struct
{
    char           *buf;  //which means the buffer
    int            buflen; //which means the buffer limit
    int            datalen; //which means how many data in the buffer
    int            dataoff; //which means the valid data offset in the buffer
}tagRingBuf;

//ring:which to be initialized
//buf:you supplied for the ring
//len:the buf length
//return:0 means ok while -1 failed
int RingInit(tagRingBuf *ring,char *buf, int buflen,int offset,int datalen);

//write len bytes data to the ring
//return:how many bytes has been written while -1 means something err
//write only changes the datalen and  data in  the ring
int RingWrite(tagRingBuf *ring,char *buf,unsigned int len);
//read len bytes data from the ring
//return:how many bytes has been read while -1 means something err
//read effect the offset datalen and data in the ring
int RingRead(tagRingBuf *ring,char *buf,unsigned int len);
int RingReset(tagRingBuf *ring);  //clear the ring data
int RingDataLen(tagRingBuf *ring);//show me how many data in the ring

