/** 
  ******************************************************************************
  * @文件   bsp_nbxx_01.c
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-03 
  * @说明   
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************           
  *
***/


#include "nbxx_01.h"
#include <string.h>

static void s_nb_recvbuffer_init(void);
static bool s_check_strstr(char *data1,char *data2,uint16_t len);
static void s_uart_data_send(const uint8_t *fmt, uint16_t len);
static uint16_t s_uart_data_read(uint8_t *fmt);
static void s_uart_rx_complete(void);
static void s_timeout_timer_enable(void);
static void s_timeout_timer_disable(void);
static void s_timeout_process(void);

static uint16_t s_nb_ack_timeout = 0;
static uint16_t s_foundstr_timeout = 0;
static char     *s_pfoundstr;
static uint8_t  *s_pfoundresult;
static uint16_t s_nb_revlen = 0; 
static uint8_t  s_current_nb_cmd = NB_AT_CFUN0;
static uint8_t  s_nb_recvpacket_num = 0;

uint8_t aRxBuffer[NBxx_BUFFER_SIZE];

Nb_rec_buffer nb_rec_buffer={
  {0},
  NULL,
  NULL,
  DATA_REC_NO_FINISH,
  NBxx_BUFFER_SIZE*3
};

volatile NBIOT_CMD_Data_t nbiot_cmd_data[20]=
{
//cmdstr,    revtimeout(s),   cmdtruebackstr,   revresult
  {"AT+CFUN=0\r\n",  5,       "OK" ,NO_BACK,1},           
  {"AT+CGSN=1\r\n",  2,       "OK" ,NO_BACK,3}, 
  {"AT+NRB\r\n",     10,      "OK" ,NO_BACK,1}, 
  {"AT+CFUN=1\r\n",  6,      "OK" ,NO_BACK,3}, 
  {"AT+CIMI\r\n",    2,        "OK" ,NO_BACK,3}, 
  {"AT+CMEE=1\r\n",  2,        "OK" ,NO_BACK,1}, 
  {"AT+CGDCONT=1,\"IP\",\"ctnet\"\r\n",  2,        "OK" ,NO_BACK,1}, 
  {"AT+NNMI=1\r\n",  2,        "OK" ,NO_BACK,1}, 
  {"AT+CGATT=1\r\n",  2,        "OK" ,NO_BACK,1}, 
  {"AT+CSCON=1\r\n",  20,        "+CSCON:1" ,NO_BACK,1}, 
  {"AT+CGPADDR=1\r\n", 3,        "+CGPADDR:1," ,NO_BACK,60}, 
  {"AT+NUESTATS\r\n" ,  2,        "OK" ,NO_BACK,1},
  {"CMDSTR_NOUSE",  2,        "OK" ,NO_BACK,1}, 
 // {"AT+NSOCL=1",  2,        "OK" ,NO_BACK,1}, 
   
  {"CMDSTR_NOUSE",  2,         "OK" ,NO_BACK,1}, //AT+NSOCR  CMD
  {"CMDSTR_NOUSE",  5,        "OK" ,NO_BACK,1},//NB_SEND_DATA  CMD
  {"CMDSTR_NOUSE",  8,       "NSONMI" ,NO_BACK,1},//NB_SEND_DATA with ack  CMD
  {"CMDSTR_NOUSE",  10,       "OK" ,NO_BACK,1},//REV_NB_DATA CMD  NB_AT_NSORF
};

void nbxx_01_recvbuffer_clear(void)
{
    nb_rec_buffer.p_read=nb_rec_buffer.p_write;
    nb_rec_buffer.rec_data_finish_flag = DATA_REC_NO_FINISH;
    memset(aRxBuffer, 0, NBxx_BUFFER_SIZE);
}
/**
  *函数:  void uart_NBxx_01_init(void)
  *功能：uart_NBxx_01初始化
  *
  *
***/
void nbxx_01_uart_init(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(NBxx_01_USART_CLK);
    rcu_periph_clock_enable(NBxx_01_USART_RX_GPIO_CLK);
    rcu_periph_clock_enable(NBxx_01_USART_TX_GPIO_CLK);
    /* connect port to USARTx_Tx */
    gpio_af_set(NBxx_01_USART_TX_GPIO_PORT, NBxx_01_USART_TX_AF, NBxx_01_USART_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_af_set(NBxx_01_USART_RX_GPIO_PORT, NBxx_01_USART_RX_AF, NBxx_01_USART_RX_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(NBxx_01_USART_TX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, NBxx_01_USART_TX_PIN);
    gpio_output_options_set(NBxx_01_USART_TX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, NBxx_01_USART_TX_PIN);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(NBxx_01_USART_RX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, NBxx_01_USART_RX_PIN);
    gpio_output_options_set(NBxx_01_USART_RX_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, NBxx_01_USART_RX_PIN);
    
    
    usart_deinit(NBxx_01_USART);
    usart_baudrate_set(NBxx_01_USART, 9600U);
    usart_receive_config(NBxx_01_USART, USART_RECEIVE_ENABLE);
    usart_transmit_config(NBxx_01_USART, USART_TRANSMIT_ENABLE);

    
    usart_interrupt_enable(NBxx_01_USART, USART_INTEN_IDLEIE);
    usart_flag_get(NBxx_01_USART, USART_FLAG_IDLEF); 
    usart_data_receive(NBxx_01_USART);

    s_nb_recvbuffer_init();
    
    nvic_irq_enable(NBxx_01_USART_IRQn, 5, 0);

    usart_interrupt_enable(NBxx_01_USART, USART_INTEN_RBNEIE);

    usart_enable(NBxx_01_USART);
}

void nbxx_01_found_result_set(uint8_t cmdnum,char *findstr,NB_back_result *result)
{
   s_current_nb_cmd = cmdnum;
   s_foundstr_timeout = nbiot_cmd_data[cmdnum].revtimeout  ;
   nbxx_01_set_ack_timeout(s_foundstr_timeout);
   s_pfoundstr = findstr;
   s_pfoundresult = result;
}

uint8_t nbxx_01_uart_send(const uint8_t *fmt, uint16_t len)
{ 
  log_s("NB_SEND:%s",fmt);
  s_uart_data_send(fmt, len);
    
  return 1; //lgf0503
}

void nbxx_01_uart_read(void)
{ 
    s_nb_revlen = s_uart_data_read(aRxBuffer);

    if(s_check_strstr((char *)aRxBuffer,s_pfoundstr, s_nb_revlen) == true)
    {
        *s_pfoundresult = BACK_TRUE;
        s_timeout_timer_disable();
    }

    if(s_check_strstr((char *)aRxBuffer,"NSONMI", s_nb_revlen) == true) //如果NB接收到服务器数据，通知主循环读出
    {
        if((s_current_nb_cmd==NB_SEND_DATA_ACK) && (*s_pfoundresult==NO_BACK))//如果当前正在等待接收服务器数据
        {
            *s_pfoundresult = NO_BACK; // 仅起通知作用 
            s_timeout_timer_disable();
        }
        
        s_nb_recvpacket_num++;  
    }
}

uint8_t nbxx_01_current_cmd_get(void)
{
  return s_current_nb_cmd;
}

void nbxx_01_recvpacket_num_down_1(void)
{
   if(nbxx_01_recvpacket_num_get())
   {
      s_nb_recvpacket_num--;
   }
}
uint8_t nbxx_01_recvpacket_num_get(void)
{
  return s_nb_recvpacket_num;
}

uint16_t nbxx_01_revlen_get(void)
{
   return s_nb_revlen;
}

static void s_uart_data_send(const uint8_t *fmt, uint16_t len)
{
    while(len)
    {    
        while(RESET == usart_flag_get(NBxx_01_USART, USART_FLAG_TC));
        usart_data_transmit(NBxx_01_USART, *fmt);  
        fmt++;
        len--;
    }
}

static void s_uart_rx_complete(void)
{
    if(nb_rec_buffer.p_write!=nb_rec_buffer.p_read)
    {
        nb_rec_buffer.rec_data_finish_flag = DATA_REC_FINISH;
        nbxx_01_uart_read();
    }
}

static uint16_t s_uart_data_read(uint8_t *fmt)
{
    char *p=nb_rec_buffer.p_read;
    char *m=nb_rec_buffer.p_write;
    bool complete=false;
    uint16_t i=0;
    if(p<m){
        while(p<m){
            *(fmt+i)=*p;
      
             /* if(*p=='\n'){
                p++;
                i++;
                complete=true;
                break;
              }*/
            p++;
            i++;
            if(i >= NBxx_BUFFER_SIZE){ //缓存满
                break;
            }
        }
        complete=true;
    }
    else if(p>m){//写指针回去了
        while(p<nb_rec_buffer.rec_buffer+nb_rec_buffer.max_len){
            *(fmt+i)=*(p);

            /* if(*p=='\n'){
            p++;
            i++;
            complete=true;
            break;
            }*/
            p++;
            i++;
            if(i >= NBxx_BUFFER_SIZE){ //缓存满
                break;
            }
        }
        
        if(complete==false&&i<NBxx_BUFFER_SIZE){//读指针回头
            p=nb_rec_buffer.rec_buffer;
            while(i<NBxx_BUFFER_SIZE){
                //  i++; //这里可能导致问题

                *(fmt+i)=*(p);
                
                /*if(*p=='\n'){
                  p++;
                  i++;
                  complete=true;
                  break;
                }*/
                p++;
                i++;
                if(p==m){
                    complete=true;
                    break;
                }
            }
        }
    }
    
    if(complete){
        nb_rec_buffer.p_read=p;
        log_s("NB_REC:%s", fmt);
        return i;
    }
    
    return 0;
}

void NBxx_01_USART_IRQHandler(void)
{
    if((RESET != usart_interrupt_flag_get(NBxx_01_USART, USART_INT_RBNEIE)) && 
       (RESET != usart_flag_get(NBxx_01_USART, USART_FLAG_RBNE))){
        /* receive data */
        *nb_rec_buffer.p_write = (usart_data_receive(NBxx_01_USART) & 0x7F);
           
        nb_rec_buffer.rec_data_finish_flag = DATA_REC_IS_ING; 
        
        if(++(nb_rec_buffer.p_write)>=nb_rec_buffer.rec_buffer+nb_rec_buffer.max_len)
        {
            nb_rec_buffer.p_write=nb_rec_buffer.rec_buffer;
        }
    }
       
    if((RESET != usart_flag_get(NBxx_01_USART, USART_FLAG_IDLEF)) && 
       (RESET != usart_interrupt_flag_get(NBxx_01_USART, USART_INT_IDLEIE))){
        /* clear the IDLE flag */
        usart_flag_get(NBxx_01_USART, USART_FLAG_IDLEF); 
        usart_data_receive(NBxx_01_USART);  

        s_uart_rx_complete();
    }
}

void nbxx_01_timeout_timer_init(uint16_t time) //unit:0.1ms
{
    uint32_t uwPrescalerValue = 0;
    timer_parameter_struct timer_initpara;
    
    rcu_periph_clock_enable(NBxx_01_TIMEOUT_TIMER_CLK);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(NBxx_01_TIMEOUT_TIMER);
    
    /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;
    /* Set TIMx instance */


    /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
    */
    timer_initpara.prescaler         = uwPrescalerValue;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = time - 1;     //0.1s触发一次中断
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(NBxx_01_TIMEOUT_TIMER, &timer_initpara);
    
    nvic_irq_enable(NBxx_01_TIMEOUT_TIMER_IRQn, 5, 0);
}

void nbxx_01_set_ack_timeout(uint16_t time)
{
   s_timeout_timer_enable();
   s_nb_ack_timeout = time; 
}

void NBxx_01_TIMEOUT_TIMER_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(NBxx_01_TIMEOUT_TIMER, TIMER_INT_UP)){
        timer_interrupt_flag_clear(NBxx_01_TIMEOUT_TIMER, TIMER_INT_UP);
        
        if(s_nb_ack_timeout > 0){
            s_nb_ack_timeout--;
        }
        else{
            s_timeout_timer_disable();
            s_timeout_process();
        }
    }    
}

static void s_timeout_timer_enable(void)
{
    timer_counter_value_config(NBxx_01_TIMEOUT_TIMER , 1);
    timer_interrupt_flag_clear(NBxx_01_TIMEOUT_TIMER, TIMER_INT_UP);
    timer_interrupt_enable(NBxx_01_TIMEOUT_TIMER, TIMER_INT_UP);
    timer_enable(NBxx_01_TIMEOUT_TIMER);
}

static void s_timeout_timer_disable(void)
{
    timer_counter_value_config(NBxx_01_TIMEOUT_TIMER , 1);
    timer_interrupt_flag_clear(NBxx_01_TIMEOUT_TIMER, TIMER_INT_UP);
    timer_interrupt_disable(NBxx_01_TIMEOUT_TIMER, TIMER_INT_UP);
    timer_disable(NBxx_01_TIMEOUT_TIMER);
}

static void s_timeout_process(void)
{
    *s_pfoundresult = BACK_TIMEOUT;
}

static void s_nb_recvbuffer_init(void)
{
  nb_rec_buffer.rec_data_finish_flag = DATA_REC_NO_FINISH;
  nb_rec_buffer.p_read=nb_rec_buffer.rec_buffer;
  nb_rec_buffer.p_write=nb_rec_buffer.rec_buffer;
}
static bool s_check_strstr(char *data1,char *data2,uint16_t len)
{
   uint16_t i=0;
   char *p;
   p=data2;
   do
   {
      if(*data1==*p)
      { 
         p++;
         if(*p=='\0')
         {
            return true;
         }
         
      }
      else
      { 
        p=data2;
      }
      data1++;
      if(*data1=='\0')
      {
         return false;
      }
      i++;
   }while(i<len);
   return false;
}






