/** 
  ******************************************************************************
  * @文件   NBxx_01_Udp.h
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-03 
  * @说明   
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************            
  *
***/

#include "lierda_server.h"
#include "App_config.h"
#include "nbxx_01_udp.h"
#include "nb-iot_app.h"
#include "string.h"
//Udp_info  udp_info;
static uint16_t socket_num=0;
static char asc_buffer[NBxx_BUFFER_SIZE];

NB_RECV_PACK  nb_recv_pack;
Udp_info UDP_run_info;
static uint8_t SERVER_cmd_num=0;
static uint16_t SendUdpdata_len=0;

static void set_Udpdata_len(uint16_t len);


void initUdp(uint8_t *ip,uint16_t port)
{
  nbiot_basic_connect();
  log_s("start create socket\n");
  createUDPSocket(port);
  
   UDP_run_info.UDP_server_port=port;
   for(uint8_t i=0;i<16;i++)
   {
      UDP_run_info.UDP_server_ip[i]=ip[i];
   }
   UDP_run_info.ack_en=1;  //默认有应答，
   UDP_run_info.rev_timeout=0;
}

void createUDPSocket(uint16_t port)
{  
  nbiot_at_command_send(NB_AT_NSOCL,"AT+NSOCL=%d\r\n",socket_num);
   
  while(1)
  {
    nbiot_at_command_send(NB_AT_NSOCR,"AT+NSOCR=DGRAM,17,%d,1\r\n",port);
    if(BACK_TRUE==nbiot_cmd_data[nbxx_01_current_cmd_get()].revresult)
    {
       break;
    }
  }
  
  log_s("create socket success! \n");
}

void RetrysendUdpData(void)
{
    send_data_to_server(NB_SEND_DATA,"AT+NSOST=%d,%s,%d,%d,%s\r\n",socket_num,(const uint8_t *) Sys_parameters_working.server_ip,atoi(Sys_parameters_working.server_port),SendUdpdata_len,asc_buffer); 
}

void set_Udpdata_len(uint16_t len)
{
   SendUdpdata_len=len;
}

void sendUdpData(const uint8_t * ip,uint16_t dst_port,const uint8_t * hex,uint16_t datalen,uint8_t ack)
{ 
  memset(asc_buffer,0,sizeof(asc_buffer));
  uint16_t ass_len=  nbiot_batch_hexs_to_ascii((uint8_t *)hex,asc_buffer,datalen);
  
  if(NO_ACK==ack)
  { 
    nbiot_at_command_send(NB_SEND_DATA,"AT+NSOST=%d,%s,%d,%d,%s\r\n",socket_num,ip,dst_port,datalen,asc_buffer);
  }
  else if(ACK_EN==ack)
  {
    send_data_to_server(NB_SEND_DATA,"AT+NSOST=%d,%s,%d,%d,%s\r\n",socket_num,ip,dst_port,datalen,asc_buffer); 
  }
}
 

void udp_ACK_to_Server(void)
{
   uint16_t len= configNbAckPackage(aTxBuffer);

   sendUdpData((const uint8_t *) Sys_parameters_working.server_ip,atoi(Sys_parameters_working.server_port),aTxBuffer,len,NO_ACK); 
}

RevStatus convert_rec_data(char *buf)
{
   char *words[5] = { NULL };
   int i;
   i= nbiot_at_get_para(',',buf,words,5);
   if(i>4)
   {
     uint16_t socket=atoi(words[0]);
     uint16_t port=atoi(words[2]);
     uint16_t len=atoi(words[3]);
     
     log_s("socket=%d port=%d  len=%d \r\n",socket,port,len);
    
     nb_recv_pack.port=port;
     nb_recv_pack.len=len;
     nb_recv_pack.socket=socket;
     
     nbiot_ascii_to_hexs(words[4],(uint8_t *)nb_recv_pack.data,len*2); 
     return RESPONSE_SUCCESS;
   }
   else
   {
      return RESPONSE_FAILD;
   }
}
void setServer_down_CmdNum(uint8_t num)
{
  if(SERVER_DOWN_MAX_CMDNUM>=num)
      SERVER_cmd_num=num;
  else
  {
     SERVER_cmd_num=SERVER_DOWN_MAX_CMDNUM;
  }
}
uint8_t getServer_down_CmdNum(void)
{
  return SERVER_cmd_num;
}
bool receive_udp_data(uint8_t ack_en)
{
    nbiot_at_command_send(NB_AT_NSORF,"AT+NSORF=0,256\r\n"); //从nb取出数据
    if(BACK_TIMEOUT==nbiot_cmd_data[NB_AT_NSORF].revresult )//取数据失败，返回
    {
       return false;
    }
    if(RESPONSE_SUCCESS==convert_rec_data((char *)aRxBuffer))//格式识别正确
    {
         if(parseServerAckPackage(nb_recv_pack.data,nb_recv_pack.len )==true)
         {
//            hal_DISABLE_TIMER(&TimHandle_NB_REV_TIMEOUT);//关闭接收超时定时器
            clearFirstPowerOn_flag();
            set_AckedNum_add_1();//记录收到服务器应答个数
            nbiot_cmd_data[NB_SEND_DATA_ACK].revresult = BACK_TRUE; // 标志
            while(getCurrent_rty_num()<getMax_rty_num())//设置当前已重发次数为最大值，防止重发。
            {
               current_rty_num_add_1();
            }
            if(getServer_down_CmdNum()!=0)
            {
               if(ack_en==ACK_EN)
               {   udp_ACK_to_Server(); }
            }

            return true;
         }
    }
    //数据不正确
    current_rty_num_add_1();//重发次数加1
    if(getCurrent_rty_num()<getMax_rty_num())//小于最大重发次数
    {
       RetrysendUdpData();//重发
    }

    return false;
}


void report_udp(void)
{
 
  nbiot_querySignal();
//  HAL_Delay(50);  //??????? ZUOYONG? 
  memset(aRxBuffer, 0, NBxx_BUFFER_SIZE);
  uint16_t all_len=get_lierda_report_Package();
  memset(aTxBuffer, 0, NBxx_BUFFER_SIZE);
  memcpy(aTxBuffer,&nb_to_lierda_sever_package,all_len);
  memcpy(aTxBuffer+all_len,&app_data,APP_DATA_LEN);
  aTxBuffer[all_len+APP_DATA_LEN]=nbiot_checksum(aTxBuffer+2,all_len+APP_DATA_LEN-2);
  ClearCurrent_rty_num();
  set_Udpdata_len(all_len+APP_DATA_LEN+1);//以防发送失败，记录长度，便于重发
  
  sendUdpData((const uint8_t *) Sys_parameters_working.server_ip,atoi(Sys_parameters_working.server_port),aTxBuffer,all_len+APP_DATA_LEN+1,ACK_EN);
   
}
