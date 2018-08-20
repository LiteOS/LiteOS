/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/* Includes -----------------------------------------------------------------*/
#include "wiznet.h"

#include "w5500.h"
#include "w5500_conf.h"
#include "utility.h"
#include "socket.h"

#include "atiny_socket.h"

#include "los_queue.h"
#include "los_task.h"
#include "los_sem.h"
#include "los_memory.h"

#include <stdlib.h>

/* Defines ------------------------------------------------------------------*/
#define WIZ_LINK_UNUSE      0
#define WIZ_LINK_INUSE      1

#define WIZ_FAILED          -1
#define WIZ_OK              0
/* Typedefs -----------------------------------------------------------------*/
typedef struct
{
    uint32_t    id;         // socket channel/port
    uint32_t    qid;        // queue id
    uint32_t    usable;     // 
    uint8_t     server_ip[4];     // remote ip
    uint16_t    server_port;      // remote port
    uint16_t    client_port;
    uint8_t     protocol;
    uint8_t     flag;
}WizLink_t;

typedef struct {
    uint32_t len;
    uint8_t *addr;
}WizQueueBuf_t;

typedef struct
{
    uint32_t  tsk_hdl;
	uint32_t  recv_sem;
}WizConfig_t;

/* Macros -------------------------------------------------------------------*/
#define WIZ_ASSERT_ID(id) \
    if(id < 0 || id >= WIZ_MAX_SOCKET_NUM) \
    {\
        WIZ_ERROR("id=%ld is invalid!",id);\
        return WIZ_FAILED;\
    }
/* Local variables ----------------------------------------------------------*/
static uint8_t rx_buffer[WIZ_RX_BUFFER_SIZE];
static WizLink_t wiz_link[WIZ_MAX_SOCKET_NUM];

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
WizConfig_t g_wizConfig;
/* Private function prototypes ----------------------------------------------*/
static int32_t wiz_get_unuse_linkid(void)
{
    int i = 0;
    
    for (i = 0; i < WIZ_MAX_SOCKET_NUM; i++){
        if (WIZ_LINK_UNUSE == wiz_link[i].usable)
            break;
    }
    if(i < WIZ_MAX_SOCKET_NUM){
        wiz_link[i].id = i;
        wiz_link[i].usable = WIZ_LINK_INUSE;
    }
    
    return i;
}
static void* wiz_malloc(size_t size)
{
    void *pMem = LOS_MemAlloc(m_aucSysMem0, size);
    if(NULL != pMem){
        memset(pMem, 0, size);
    }
    return pMem;
}

static void wiz_free(void* ptr)
{
    (void)LOS_MemFree(m_aucSysMem0, ptr);
}

/* Public functions ---------------------------------------------------------*/
int32_t wiznet_data_handler(int32_t id)
{
    uint16_t len = 0;
    int32_t ret = -1;
    WizQueueBuf_t qbuf;
    
    WIZ_ASSERT_ID(id);

    len = getSn_RX_RSR(id);
    if(len == 0)
        return ret;
    
    if(wiz_link[id].protocol == Sn_MR_TCP){
        recv(id, rx_buffer, len);
    }else if(wiz_link[id].protocol == Sn_MR_UDP){
        recvfrom(id, rx_buffer, len, wiz_link[id].server_ip, &wiz_link[id].server_port);
        len -= 8;
    }
    
    qbuf.addr = wiz_malloc(len);
    if (NULL == qbuf.addr)
    {
        WIZ_ERROR("malloc for qbuf failed!");
        goto END;
    }

    qbuf.len = len;
    memcpy(qbuf.addr, rx_buffer, len);

    if (LOS_OK != (ret = LOS_QueueWriteCopy(wiz_link[id].qid, &qbuf, sizeof(WizQueueBuf_t), 0)))
    {
        WIZ_ERROR("LOS_QueueWriteCopy() failed! ret = %lx", ret);
        wiz_free(qbuf.addr);
        goto END;
    }
END:
    return ret;
}
void wiznet_recv_task(void)
{
    int i;
    
    while(1)
    {
        LOS_SemPend(g_wizConfig.recv_sem, LOS_WAIT_FOREVER);

        for(i = 0; i < WIZ_MAX_SOCKET_NUM; i++)
        {
            if(wiz_link[i].usable == WIZ_LINK_UNUSE)
                continue;
            if(getSn_IR(i) & Sn_IR_RECV)
            {
                setSn_IR(i, Sn_IR_RECV);
                wiznet_data_handler(i);
            }
        }
    }
}

uint32_t wiznet_create_recv_task(void)
{
    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "wiznet_recv_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)wiznet_recv_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate((UINT32*)&g_wizConfig.tsk_hdl, &task_init_param);
    return uwRet;
}

static int wiznet_init_data(void)
{
    int i, ret;
        
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(&wiz_link, 0, WIZ_MAX_SOCKET_NUM*sizeof(WizLink_t));
    for(i = 0; i < WIZ_MAX_SOCKET_NUM; i++){
        wiz_link[i].client_port = local_port++;
        wiz_link[i].qid = LOS_ERRNO_QUEUE_NOT_CREATE;
    }

    g_wizConfig.tsk_hdl = LOS_ERRNO_TSK_NOT_CREATED;
    g_wizConfig.recv_sem = LOS_ERRNO_SEM_INVALID;

    ret = LOS_SemCreate(0, (UINT32*)&g_wizConfig.recv_sem);
    if (ret != LOS_OK)
    {
        WIZ_LOG("LOS_SemCreate() recv sem failed!");
        goto create_recv_sem_failed;
    }

    ret = wiznet_create_recv_task();
    if(ret != LOS_OK)
    {
        WIZ_LOG("LOS_TaskCreate() recv task failed!");
        goto create_recv_task_failed;
    }
    return WIZ_OK;
    
create_recv_task_failed:
    LOS_SemDelete(g_wizConfig.recv_sem);
create_recv_sem_failed:
    return WIZ_FAILED;
}

static void wiznet_deinit_data(void)
{
    // delete task
    if(g_wizConfig.tsk_hdl != LOS_ERRNO_TSK_NOT_CREATED){
        if(LOS_OK != LOS_TaskDelete(g_wizConfig.tsk_hdl)){
            WIZ_LOG("LOS_TaskDelete(%ld) failed!", g_wizConfig.tsk_hdl);
        }
        g_wizConfig.tsk_hdl = LOS_ERRNO_TSK_NOT_CREATED;
    }
    // delete semaphore
    if(g_wizConfig.recv_sem != LOS_ERRNO_SEM_INVALID){
        if(LOS_OK != LOS_SemDelete(g_wizConfig.recv_sem)){
            WIZ_LOG("LOS_SemDelete(%ld) failed!", g_wizConfig.recv_sem);
        }
        g_wizConfig.recv_sem = LOS_ERRNO_SEM_INVALID;
    }
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(&wiz_link, 0, WIZ_MAX_SOCKET_NUM*sizeof(WizLink_t));
}

void wiznet_init(void)
{
    wiznet_init_data();
    
    w5500_init();
    w5500_set_mac();   
    w5500_set_ip();

    socket_buf_init(txsize, rxsize);
}

void wiznet_deinit(void)
{
    int id = 0;

    for(id = 0; id < WIZ_MAX_SOCKET_NUM; id++)
    {
        if(wiz_link[id].usable == WIZ_LINK_INUSE){
            if(WIZ_OK != wiznet_close(id)){
                WIZ_LOG("wiznet_close(%d) failed!", id);
            }
        }
    }
    w5500_deinit();
    wiznet_deinit_data();
}

int32_t wiznet_connect(const char* host, const char* port, int protocol)
{
    int32_t ret = WIZ_FAILED;
    int32_t id = -1;
    int ip[4], i, result;
    int connected = 0;
    int32_t status;

    WIZ_LOG("host:%s, port:%s", host, port);

    result = sscanf(host, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
    if(result != 4)
    {
        WIZ_LOG("host:%s format error!", host);
        return WIZ_FAILED;
    }
    result = atoi(port);
    if(result == 0)
    {
        WIZ_LOG("port:%s format error!", port);
        return WIZ_FAILED;
    }
    
    id = wiz_get_unuse_linkid();
    if(id < 0 || id >= WIZ_MAX_SOCKET_NUM)
    {
        WIZ_LOG("no vaild linkid for use(id = %ld)", id);
        return WIZ_FAILED;
    }

    if(ATINY_PROTO_TCP == protocol){
        wiz_link[id].protocol = Sn_MR_TCP;
        wiz_link[id].flag = Sn_MR_ND;
    }else{ // default udp
        wiz_link[id].protocol = Sn_MR_UDP;
        wiz_link[id].flag = Sn_MR_CLOSE;
    }
    
    for(i = 0; i < 4; i++){
        wiz_link[id].server_ip[i] = ip[i];
    }
    wiz_link[id].server_port = result;

    ret = LOS_QueueCreate("wizQueue", 16, (UINT32 *)&wiz_link[id].qid, 0, sizeof(WizQueueBuf_t));
    if (ret != LOS_OK)
    {
        WIZ_LOG("init wizQueue failed!");
        wiz_link[id].usable = WIZ_LINK_UNUSE;
        return WIZ_FAILED;
    }
  
    while(0 == connected)
    {
        status = getSn_SR(id);
        switch ( status )
        {
            case SOCK_CLOSED :
                socket(id, wiz_link[id].protocol, wiz_link[id].client_port, wiz_link[id].flag);
                break;
            case SOCK_INIT :
                connect(id, wiz_link[id].server_ip, wiz_link[id].server_port);
                break;
            case SOCK_UDP :
            case SOCK_ESTABLISHED :
                connected = 1;
                break;
            case SOCK_CLOSE_WAIT :
                wiznet_close(id);
                return WIZ_FAILED;
            default:
                break;
        }
    }
    return id;
}

int32_t wiznet_send(int32_t id, const uint8_t *buf, uint32_t len)
{
    int32_t ret = WIZ_OK;
    
    WIZ_ASSERT_ID(id);
    
    if(wiz_link[id].usable == WIZ_LINK_UNUSE){
        WIZ_ERROR("id=%ld is unusable!", id);
        return WIZ_FAILED;
    }
    if(wiz_link[id].protocol == Sn_MR_TCP){
        ret = send(id, buf, len);
    }else if(wiz_link[id].protocol == Sn_MR_UDP){
        ret = sendto(id, buf, len, wiz_link[id].server_ip, wiz_link[id].server_port);
    }
    return ret;
}

int32_t wiznet_recv(int32_t id, unsigned char* buf, size_t len)
{
    WIZ_ASSERT_ID(id);
    return wiznet_recv_timeout(id, buf, len, LOS_WAIT_FOREVER);
}

int32_t wiznet_recv_timeout(int32_t id, unsigned char* buf, size_t len, uint32_t timeout)
{
    uint32_t qlen = sizeof(WizQueueBuf_t);
    uint32_t rxlen = 0;
    
    WIZ_ASSERT_ID(id);
    
    WizQueueBuf_t qbuf = {0, NULL};
    int ret = LOS_QueueReadCopy(wiz_link[id].qid, (void*)&qbuf, (UINT32*)&qlen, timeout);
    WIZ_LOG("ret = %x, len = %ld, id = %ld", ret, qbuf.len, id);
    if (ret != LOS_OK)
    {
        return WIZ_FAILED;
    }

    if (qbuf.len){
        rxlen = (len < qbuf.len) ? len : qbuf.len;
        memcpy(buf, qbuf.addr, rxlen);
        wiz_free(qbuf.addr);
    }
    return rxlen;
}

int32_t wiznet_close(int32_t id)
{
    int timeout = 0;
    WIZ_ASSERT_ID(id);

    if(wiz_link[id].usable == WIZ_LINK_UNUSE){
        WIZ_LOG("linkid=%ld is unused!", id);
        return WIZ_FAILED;
    }
    
    close(id);
    wiz_link[id].id = 0;
    wiz_link[id].usable = WIZ_LINK_UNUSE;
    
    // delete queue
    if(wiz_link[id].qid != LOS_ERRNO_QUEUE_NOT_CREATE)
    {
        uint32_t qlen = sizeof(WizQueueBuf_t);
        WizQueueBuf_t qbuf = {0, NULL};
        // clear queue data
        while(LOS_OK == LOS_QueueReadCopy(wiz_link[id].qid, (void*)&qbuf, (UINT32*)&qlen, 10)){
            if (qbuf.len){
                wiz_free(qbuf.addr);
                memset(&qbuf, 0, sizeof(WizQueueBuf_t)); // don't use qlen
            }
        }
        while(LOS_OK != LOS_QueueDelete(wiz_link[id].qid) || timeout > 10){
            WIZ_ERROR("LOS_QueueDelete() failed, qid=%ld", wiz_link[id].qid);
            timeout++;
        }
        wiz_link[id].qid = LOS_ERRNO_QUEUE_NOT_CREATE;
    }
    return WIZ_OK;
}

void wiznet_irq_handler(void)
{
    LOS_SemPost(g_wizConfig.recv_sem);
}

/* Private functions --------------------------------------------------------*/

