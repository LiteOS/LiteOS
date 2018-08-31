#include <string.h>
#include "ring.h"
//ring:which to be initialized
//buf:you supplied for the ring
//len:the buf length
//return:0 means ok while -1 failed
int RingInit(tagRingBuf *ring,char *buf, int buflen,int offset,int datalen)
{
    int ret = -1;
    if((NULL == ret))
    {
        return ret;
    }
    ring->buf     = buf;
    ring->buflen  = buflen;
    ring->datalen = datalen;
    ring->dataoff = offset;
    ret = 0;
    return ret;
}

//write len bytes data to the ring
//return:how many bytes has been written while -1 means something err
//write only changes the datalen and  data in  the ring
int RingWrite(tagRingBuf *ring,char *buf,unsigned int len)
{
	int ret = -1;
	int cpylen;  //the current time we should move
	int lenleft;  //and how many data still left to move
	int offset;
	unsigned char *src;
	unsigned char *dst;
	if((NULL == ring)||(NULL == buf)||(0 == len))
	{
		return ret;//which means parameters error
	}
	if(ring->datalen == ring->buflen)
	{
		ret = 0;
		return  ret;//which means you could copy nothing here
	}
	ret = len > (ring->buflen-ring->datalen)?(ring->buflen-ring->datalen):len;
	//now let us think the method to fill the data,take care of the roll back
	lenleft = ret;
	src = buf;
	if((ring->dataoff+ring->datalen)>ring->buflen) //which means the data has roll back
	{
		offset = (ring->dataoff+ring->datalen)%ring->buflen; //we could move it one time
		cpylen = lenleft;
		dst = ring->buf + offset;
		memcpy(dst,src,cpylen);
		ring->datalen += cpylen;
		lenleft -= cpylen;
	}
	else if((ring->dataoff+ring->datalen + lenleft)>ring->buflen) //which means the data will be roll back
	{
		//which means roll back,we should copy some here to the tail
		offset = ring->dataoff + ring->datalen;
		cpylen = ring->buflen - offset;
		dst = ring->buf + offset;
		memcpy(dst,src,cpylen);
		src += cpylen;
		ring->datalen += cpylen;
		lenleft -= cpylen;
	}
	//here means we could move it by one time
	if(lenleft > 0)
	{
		offset = (ring->dataoff+ring->datalen)%ring->buflen; //we could move it one time
		cpylen = lenleft;
		dst = ring->buf + offset;
		memcpy(dst,src,cpylen);
		ring->datalen += cpylen;
	}
	return ret;
}
//read len bytes data from the ring
//return:how many bytes has been read while -1 means something err
//read effect the offset datalen and data in the ring
int RingRead(tagRingBuf *ring,char *buf,unsigned int len)
{
	int ret = -1;
	int cpylen;  //the current time we should move
	int lenleft;  //and how many data still left to move
	int offset;
	unsigned char *src;
	unsigned char *dst;
	if((NULL == ring)||(NULL == buf)||(0 == len))
	{
		return ret;//which means parameters error
	}
	if(ring->datalen == 0)
	{
		ret = 0;
		return  ret;//which means you could copy nothing here
	}
	ret = len > ring->datalen?ring->datalen:len;
	//now let us think the method to fill the data,take care of the roll back
	lenleft = ret;
	dst = buf;
	if((ring->dataoff+ lenleft)>=ring->buflen) //which means the data has roll back
	{
		offset =ring->dataoff; //we cpy part
		cpylen = ring->buflen - ring->dataoff;
		src = ring->buf + offset;
		memcpy(dst,src,cpylen);
		ring->dataoff = (ring->dataoff + cpylen)%ring->buflen;
		ring->datalen -= cpylen;
		lenleft -= cpylen;
		dst += cpylen;
	}
	//here means we could move it by one time
	if(lenleft > 0)
	{
		offset =ring->dataoff; //we cpy part
		cpylen = lenleft;
		src = ring->buf + offset;
		memcpy(dst,src,cpylen);
		ring->dataoff = ring->dataoff + cpylen;
		ring->datalen -= cpylen;
	}
	return ret;
}

int RingDataLen(tagRingBuf *ring)
{
	int ret = -1;
	if(NULL != ring)
	{
		ret = ring->datalen;
	}
	return ret;
}
int RingReset(tagRingBuf *ring)
{
	int ret = -1;
	if(NULL != ring)
	{
		ring->datalen = 0;
		ring->dataoff = 0;
		ret = 0;
	}
	return ret;
}


