#include "atadapter.h"
#include "at_api_interface.h"
#include "stm32f4xx_hal.h"
#include "los_task.h"
#include "los_sem.h"

UART_HandleTypeDef at_usart;
at_link g_at_link[AT_MAX_LINK_NUM];

uint32_t wi = 0;
uint32_t wi_bak= 0;
uint32_t ri = 0;

at_task at = {
	.init = at_init,
	.write = at_write,
	.read = at_read,
    .get_id = at_get_unuse_linkid,
};

void at_irq_handler(void)
{
	uint8_t __clear;

    __clear = __clear;

    if(__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_RXNE) != RESET)
    {
		at.recv_buf[wi++] = (uint8_t)(at_usart.Instance->DR & 0x00FF);
		if (wi >=MAX_AT_RECV_LEN)wi = 0;
    }
    else if (__HAL_UART_GET_FLAG(&at_usart,UART_FLAG_IDLE) != RESET)
    {
		__clear = at_usart.Instance->SR;
        __clear = at_usart.Instance->DR;
        wi_bak = wi;
        LOS_SemPost(at.recv_sem);
    }
}

void at_usart_config(UART_HandleTypeDef * usart)
{
    usart->Instance = AT_USART;
    usart->Init.BaudRate = AT_BUARDRATE;

    usart->Init.WordLength = UART_WORDLENGTH_8B;
    usart->Init.StopBits = UART_STOPBITS_1;
    usart->Init.Parity = UART_PARITY_NONE;
    usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart->Init.Mode = UART_MODE_RX | UART_MODE_TX;
    if(HAL_UART_Init(usart) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
    __HAL_UART_CLEAR_FLAG(usart,UART_FLAG_TC);
    LOS_HwiCreate(AT_USART_IRQn, 0,0,at_irq_handler,NULL);
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);

}

int32_t at_get_unuse_linkid()
{
	int i;

    if (AT_MUXMODE_SIGNEL == at.mux_mode)
    {
        return 0;    // use queue in link 0 when in signel mode
    }

	for (i = 0; i < AT_MAX_LINK_NUM; i++)
	{
		if (AT_LINK_UNUSE == g_at_link[i].usable)
		{
			return i;
		}
	}
	return -1;
}

uint32_t wait_for_resp(char * reply1, char *reply2)
{
    uint32_t ret = 0;
    char * pp1, *pp2;
    uint32_t retry_t;

    // got 10 resp for reply
    for(retry_t = 0; retry_t < 10; retry_t++){
        LOS_SemPend(at.resp_sem, LOS_WAIT_FOREVER);
        AT_LOG("get resp :%s ", at.cmdresp);

        if (NULL != reply1 && NULL != reply2){
            pp1 = strstr((const char *)at.cmdresp, (const char *)reply1);
            pp2 = strstr((const char *)at.cmdresp, (const char *)reply2);
            ret = (NULL != pp1 || NULL != pp2);
        } else if (NULL != reply1) {
            pp1 = strstr((const char *)at.cmdresp, (const char *)reply1);
            ret = (NULL != pp1);
        } else if (NULL != reply2) {
            pp2 = strstr((const char *)at.cmdresp, (const char *)reply2);
            ret = (NULL != pp2);
        }

        if (ret){
            memset(at.cmdresp, 0, MAX_AT_RESP_LEN);
            return ret;
        }
    }
		return ret;

}
void store_resp_buf(uint8_t * buf, uint32_t len)
{
    if (NULL == buf || 0 == len || len > MAX_AT_RESP_LEN)
        return;

    strcat((char*)at.cmdresp, (char *)buf);
}
int32_t at_read(int32_t id, int8_t * buf, uint32_t len, int32_t timeout)
{
    uint32_t qlen = sizeof(QUEUE_BUFF);

    QUEUE_BUFF  qbuf = {0, NULL};
    int ret = LOS_QueueReadCopy(g_at_link[id].qid, &qbuf, &qlen, timeout);
    AT_LOG("ret = %x, len = %d", ret, qbuf.len);

    if (qbuf.len){
        memcpy(buf, qbuf.addr, qbuf.len);
        atiny_free(qbuf.addr);
    }
    return qbuf.len;
}

int32_t at_write(const int8_t * buf, int32_t len, char * rep1, char * rep2)
{
	uint32_t i, ret = AT_OK;

    for (i = 0; i < len ; i++){
        at_usart.Instance->DR = (buf[i] & (uint16_t)0x00FF);
		while (__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_TXE) == RESET);
    }
    if ((!rep1 && !rep2) || wait_for_resp(rep1, rep2)){
        ret = len; //get excepted reply, return send len
    }
	return ret;
}


int read_resp(uint8_t * buf)
{
    uint32_t len = 0;
    uint32_t wi = wi_bak;
    if (NULL == buf){
        return -1;
    }

    if (wi == ri){
        return 0;
	}

    if (wi > ri){
        len = wi - ri;
        memcpy(buf, &at.recv_buf[ri], len);
    } else {
        uint32_t tmp_len = MAX_AT_RECV_LEN - ri;
        memcpy(buf, &at.recv_buf[ri], tmp_len);
        memcpy(buf + tmp_len, at.recv_buf, wi);
        len = wi + tmp_len;
    }
    ri = wi;
    return len;
}

void at_recv_task(uint32_t p)
{
    uint32_t recv_len = 0;
    uint8_t * tmp = at.userdata; //[MAX_USART_BUF_LEN] = {0};
    int ret = 0;
    QUEUE_BUFF  buf;

    while(1){
        LOS_SemPend(at.recv_sem, LOS_WAIT_FOREVER);
        memset(tmp, 0, MAX_AT_USERDATA_LEN);
        recv_len = read_resp(tmp);

        if (0 >= recv_len)
            continue;

        uint32_t data_len = 0;
        uint32_t linkid = 0;
        char * p1, * p2;
        AT_LOG("get recv_len = %d", recv_len);

        p1 = (char *)tmp;
        for (;;){
            if (0 == memcmp(p1, AT_DATAF_PREFIX, strlen(AT_DATAF_PREFIX)))
            {
                //data format "+IPD,len:xxxxx"
                p2 = strstr(p1, ",");
                if (NULL == p2)
                {
                    AT_LOG("got +IPD format invaild!");
                    p1 += 4;
                    continue;
                }

                if (AT_MUXMODE_MULTI == at.mux_mode)
                {
                    linkid = 0;
                    for (p2++; *p2 <= '9' && *p2 >= '0' ;p2++)
                    {
                        linkid = (linkid * 10 + (*p2 - '0'));
                    }
                    AT_LOG("got +IPD linkid id = %d!", linkid);
                }

                for (p2++; *p2 <= '9' && *p2 >= '0' ;p2++)
                {
                    data_len = (data_len * 10 + (*p2 - '0'));
                }
                p2++; //over ':'

                buf.len = data_len;
                buf.addr = atiny_malloc(data_len);
                if (NULL == buf.addr)
                {
                    AT_LOG("atiny_malloc failed!");
                    p1 = p2 + data_len;
                    data_len = 0;
                    continue;
                }

                AT_LOG("get +IPD data, len = %d, store to :%p", data_len, buf.addr);
                memcpy(buf.addr, p2, data_len);
                if (LOS_OK != (ret = LOS_QueueWriteCopy(g_at_link[linkid].qid, &buf, sizeof(QUEUE_BUFF), 0)))
                {
                    AT_LOG("LOS_QueueWriteCopy  failed! ret = %x", ret);
                    atiny_free(buf.addr);
                }

                p1 = p2 + data_len;
                data_len = 0;
            }
            else
            {
                p2 = strstr(p1, AT_DATAF_PREFIX);
                if (NULL == p2) {
                    p2 = (char *)tmp + recv_len;//end
                    store_resp_buf((uint8_t*)p1 , p2 - p1);
                    LOS_SemPost(at.resp_sem);
                    break;
                }
                store_resp_buf((uint8_t*)p1 , p2 - p1);
                LOS_SemPost(at.resp_sem);
                p1 = p2;
            }

        }
    }
}

uint32_t create_at_recv_task()
{
    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "at_recv_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)at_recv_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&at.tsk_hdl, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}
int32_t at_struct_init(at_task * at)
{
	int ret = -1;
	if (NULL == at)
	{
		AT_LOG("invaild param!");
		return ret;
	}

	ret = LOS_SemCreate(0, &at->recv_sem);
    if (ret != LOS_OK)
    {
        AT_LOG("init at_recv_sem failed!");
        goto at_recv_sema_failed;
    }


    ret = LOS_SemCreate(0, &at->resp_sem);
    if (ret != LOS_OK)
    {
        AT_LOG("init at_resp_sem failed!");
        goto at_resp_sema_failed;
    }

    at->recv_buf = atiny_malloc(MAX_AT_RECV_LEN);
    if (NULL == at->recv_buf)
    {
    	AT_LOG("malloc recv_buf failed!");
        goto malloc_recv_buf;
    }

	at->cmdresp = atiny_malloc(MAX_AT_RESP_LEN);
    if (NULL == at->cmdresp)
    {
    	AT_LOG("malloc cmdresp failed!");
        goto malloc_resp_buf;
    }

	at->userdata = atiny_malloc(MAX_AT_USERDATA_LEN);
    if (NULL == at->userdata)
    {
    	AT_LOG("malloc userdata failed!");
        goto malloc_userdata_buf;
    }

    at->read = at_read;
    at->write = at_write;
    at->mux_mode = 1;
    return AT_OK;

    malloc_userdata_buf:
	    atiny_free(at->cmdresp);
	malloc_resp_buf:
		atiny_free(at->recv_buf);
    malloc_recv_buf:
        LOS_SemDelete(at->resp_sem);
    at_resp_sema_failed:
        LOS_SemDelete(at->recv_sem);
    at_recv_sema_failed:
        return AT_FAILED;
}

void at_init()
{
    AT_LOG("Config %s......\n", AT_MODU_NAME);

    osDelay(2000);
	if (AT_OK != at_struct_init(&at))
	{
		AT_LOG("prepare at struct failed!");
		return;
	}

    at_usart_config(&at_usart);
    create_at_recv_task();

    AT_LOG("Config complete!!\n");

}
