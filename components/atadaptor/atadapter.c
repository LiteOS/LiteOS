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
    LOS_HwiCreate(AT_USART_IRQn, 0, 0, at_irq_handler,NULL);
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);

}

//if p not in list, add it at head;
void at_listener_list_add(at_listner * p)
{
    at_listner * tmp = at.head;

    if (NULL == tmp)
    {
        at.head = p;
        p->next = NULL; 
        return;
    }

    while(tmp)
    {
        if (0 == strncmp((char *)tmp->perfix, (char *)p->perfix, strlen((char*)p->perfix)))
        {
            AT_LOG("pasher(%s) already in list!", p->perfix);
            return;
        }
        if (NULL == tmp->next)
        {    
        //add it at tail;
            tmp->next = p;
            p->next = NULL;
            break;
        }
       tmp = tmp->next;
    }
}

void at_listner_list_del(at_listner * p)
{
    at_listner * tmp = at.head;
    if (tmp == p)
    {
        at.head = p->next;
        return;
    }

    while(tmp->next)
    {
        if (tmp->next == p)
        {
            tmp->next = p->next;
            break;
        }
        tmp = tmp->next;
    }

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


void store_resp_buf(int8_t * resp_buf, int8_t * buf, uint32_t len)
{
    if (NULL == buf || 0 == len || len > MAX_AT_RESP_LEN)
        return;

    strcat((char*)resp_buf, (char *)buf);
}

int32_t at_cmd(int8_t * buf, int32_t len, char * rep1, char * rep2)
{
    uint32_t i;
    at_listner listener;
    char * line_end = AT_LINE_END;
    char rep_b[512] = {0};

    listener.perfix = buf;
    listener.suffix = (int8_t *)rep1;
    listener.callback = NULL;
    listener.resp = (int8_t*)rep_b;

    at_listener_list_add(&listener);

    for (i = 0; i < len ; i++){
        at_usart.Instance->DR = (buf[i] & (uint16_t)0x00FF);
        while (__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_TXE) == RESET);
    }

    for (i = 0; i < strlen(AT_LINE_END) ; i++){
        at_usart.Instance->DR = (line_end[i] & (uint16_t)0x00FF);
        while (__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_TXE) == RESET);
    }

    LOS_SemPend(at.resp_sem, LOS_WAIT_FOREVER);
    at_listner_list_del(&listener); 

    AT_LOG("repb:%s", rep_b);

    return len;
}

int32_t at_write(int8_t * buf, int32_t len)
{
	uint32_t i;
    at_listner listener;
    char rep_b[512] = {0};

    listener.perfix = "\r\nRecv";
    listener.suffix = "SEND OK";
    listener.callback = NULL;
    listener.resp = (int8_t*)rep_b;

    at_listener_list_add(&listener);

    for (i = 0; i < len ; i++){
        at_usart.Instance->DR = (buf[i] & (uint16_t)0x00FF);
		while (__HAL_UART_GET_FLAG(&at_usart, UART_FLAG_TXE) == RESET);
    }

    LOS_SemPend(at.resp_sem, LOS_WAIT_FOREVER);
    at_listner_list_del(&listener); 

    AT_LOG("repb:%s", rep_b);

	return len;
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
    uint32_t suffix_catching = 0;
    uint8_t * tmp = at.userdata; //[MAX_USART_BUF_LEN] = {0};
    int ret = 0;
    at_listner * listener = NULL;

    QUEUE_BUFF  buf;

    while(1){
        LOS_SemPend(at.recv_sem, LOS_WAIT_FOREVER);
        if (NULL == listener) listener = at.head;

        memset(tmp, 0, MAX_AT_USERDATA_LEN);
        recv_len = read_resp(tmp);

        if (0 >= recv_len)
            continue;

        int32_t data_len = 0;
        int32_t linkid = 0;
        char * p1, * p2;
        AT_LOG("get recv_len = %d buf:%s", recv_len, tmp);

        p1 = (char *)tmp;
        p2 = (char *)(tmp + recv_len);
        for (;listener != NULL;){
            data_len = p2 - p1;
            if (data_len <= 0)
                break;

            AT_LOG("get data_len = %d", data_len);

            if (suffix_catching)
            {
                char * suffix;
                suffix = strstr(p1, (const char*)listener->suffix);
                AT_LOG("GOT suffix (%s) at %p", listener->suffix, suffix);

                if (NULL == suffix)
                {
                    suffix_catching = 1;
                    store_resp_buf((int8_t*)listener->resp, (int8_t*)p1, p2 - p1);
                }
                else 
                {
                    suffix_catching = 0;
                    store_resp_buf((int8_t *)listener->resp, (int8_t*)p1, suffix - p1);
                    LOS_SemPost(at.resp_sem);
                    listener = NULL;
                } 
                break;
            }

            AT_LOG("responding for cmd:%s", listener->perfix);
            if (data_len > strlen((char*)listener->perfix) && 0 == strncmp(p1, (char*)listener->perfix, strlen((char*)listener->perfix)))
            {
                AT_LOG("Get resp for cmd:%s, suffix = %s", listener->perfix, listener->suffix);
                if (NULL == listener->suffix && listener->callback)
                {
                    //process in callback, data format , like +IPD/+CIPEVENT, etc
                    int32_t t_len = listener->callback((int8_t *)p1, data_len);
                    if (data_len <= 0)
                    {
                        break;
                    }
                    p1 +=  t_len;
                }

                if (NULL != listener->suffix){
                    // cmd resp, end with OK/ERROR etc
                    char * suffix;
                    suffix = strstr(p1 + strlen((char*)listener->perfix), (const char*)listener->suffix);
                    AT_LOG("GOT suffix (%s) at %p", listener->suffix, suffix);
                    if (NULL == suffix)
                    {
                        suffix_catching = 1;
                        store_resp_buf(listener->resp, (int8_t *)p1, (int32_t)(p2 - p1));
                        break;
                    }
                    else 
                    {
                        suffix_catching = 0;
                        store_resp_buf(listener->resp, (int8_t*)p1, suffix - p1);
                        LOS_SemPost(at.resp_sem);
                        p1 = suffix + strlen((char*)listener->suffix) + 1;  //end with "\r\n"
                    }
                }
                listener = at.head;
                continue;
            }
            AT_LOG("responding next...");
            listener = listener->next;
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

    at->head = NULL;
    at->cmd = at_cmd,
    at->write = at_write;
    at->mux_mode = 1;
    return AT_OK;


//        atiny_free(at->userdata);
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
