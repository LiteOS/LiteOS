/** 
  ******************************************************************************
  * @文件   lierda_server.c
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-10
  * @说明   利尔达服务器通信协议
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************            
  *
***/

#include "lierda_server.h"
#include "App_config.h"
#include "nbxx_01_udp.h"
#include "nb-iot_app.h"
#include "platform.h"
#include "string.h"

#define DAY_TIME  24*3600*1000  //单位ms

static uint8_t report_ver=0;
static uint8_t send_num=0;//帧序号
static uint8_t acked_num=0;//收到服务器应答数据包个数
static uint64_t unix_time_ms=0;
static uint64_t current_day=0;
static uint64_t last_time=0;
static uint8_t server_currentcmd_num=0; //准确识别的服务器下发命令个数
 
Lierda_sever_UP_package   nb_to_lierda_sever_package;
Lierda_sever_DOWN_package lierda_sever_to_nb_package;
Nb_ack_Lierda_sever_package  nb_ack_sever_package;
APP_data_t  app_data;

Nb_ack_cmd server_down_cmd[SERVER_DOWN_MAX_CMDNUM];

 
void setUnixTime(uint64_t time);
uint64_t getCurUnixTime(void);
uint32_t getSendNum(void);
uint32_t getAckedNum(void);


void setUnixTime(uint64_t time)
{

//  if((int64_t)(unix_time_ms-last_time)>DAY_TIME)
//  {

//    memcpy((uint64_t *)(&current_day),&time,8);
//    last_time=unix_time_ms;
//  }
//  unix_time_ms=time-HAL_GetTick();
}
 
uint64_t getCurUnixTime(void)
{
//  if(unix_time_ms==0)
//  {
//      memcpy(&unix_time_ms,(uint64_t *)(&current_day),8);
//      if(unix_time_ms>HAL_GetTick())
//      {
//        unix_time_ms-=HAL_GetTick();
//      }
//      last_time=unix_time_ms;
//  }
//  return unix_time_ms+HAL_GetTick();
}


uint32_t getSendNum(void)
{ 
   return send_num++; 
}
uint32_t getAckedNum(void)
{ 
   return acked_num; 
}
void set_AckedNum_add_1(void)
{
   acked_num++;
}
uint8_t getCurReportVer(void)
{
  return report_ver; 
}
uint16_t get_lierda_ReportPackageLen(void)
{
   uint16_t all_len=0;
  all_len = sizeof(nb_to_lierda_sever_package)+sizeof(APP_data_t);
  
  return all_len;
  
}
void updata_to_lierda_server_package(void)
{
  uint16_t unTemp16;
  uint32_t ulnTemp32;
 
//    app_data.type = 0x02;
//    unTemp16=getTemper();
//    app_data.temper = BigtoLittle16(unTemp16);
//    unTemp16=getHumidity();
//    app_data.humidity = BigtoLittle16(unTemp16);
//    ulnTemp32=getLight();
//    app_data.light = BigtoLittle32(ulnTemp32);
//    ulnTemp32=getAirPressure();
//    app_data.pressure = BigtoLittle32(ulnTemp32);
//    app_data.led_state = getLedState();
//    Gps *gps=getGps();
//    app_data.gps_flag = gps->flag;
//    app_data.longitude = BigtoLittle32(gps->longitude);
//    app_data.latitude = BigtoLittle32(gps->latitude); 
//    app_data.battery = get_battery_power();
}
uint16_t get_lierda_report_Package(void)
{ 
  uint16_t data_len=0;
  uint32_t temp32;
  uint16_t temp16;
  uint64_t temp64;
  data_len=sizeof(nb_to_lierda_sever_package);
  nb_to_lierda_sever_package.start=BigtoLittle16(0xffaa); //起始符
  nb_to_lierda_sever_package.cmd=BigtoLittle16(0xD0D0);   //命令
  nb_to_lierda_sever_package.version=getCurReportVer();   //版本
  temp16=get_lierda_ReportPackageLen();   
  nb_to_lierda_sever_package.len=BigtoLittle16(temp16);    //长度
  nb_to_lierda_sever_package.device_sn_len= nbiot_get_imei(nb_to_lierda_sever_package.device_sn); //设备编号（IMEI） 
  temp32= getSendNum();
  nb_to_lierda_sever_package.mesg_send_num=BigtoLittle32(temp32);//数据包序号
  temp32= getAckedNum();
  nb_to_lierda_sever_package.mesg_acked_num=BigtoLittle32(temp32);//服务器应答数据包数量
  temp64= getCurUnixTime();
  nb_to_lierda_sever_package.unix_time=BigtoLittle64(temp64);//UNIX时间
  temp16= nbiot_getRspr();
  nb_to_lierda_sever_package.RSPR=BigtoLittle16(temp16);
  temp16= nbiot_getSinr();
  nb_to_lierda_sever_package.SINR=BigtoLittle16(temp16);
  nb_to_lierda_sever_package.cover_level= nbiot_getCoverLevel();
  temp16=getCurrent_rty_num();
  nb_to_lierda_sever_package.repat_num=BigtoLittle16(temp16);
  
  nb_to_lierda_sever_package.flag&=~0x01;//需要应答
 
  if(isFirstPowerOn())
  {
    nb_to_lierda_sever_package.flag|=0x02;
  }
  else
  {
    nb_to_lierda_sever_package.flag&=~0x02; 
  }  
  nb_to_lierda_sever_package.ap_data_len=BigtoLittle16(APP_DATA_LEN);
  return data_len;

}
uint16_t parseServerCmd0xC1(uint8_t *data,uint16_t len)
{
  Server_cmd  node;
  uint16_t temp16;
  uint32_t temp32;
//  log_i("parseServerCmd0xC1 flag=%d len=%d\r\n",*data,len);  
  if(*data==0xc1&&len>=CMD0XC1_LEN)
  {
    //= data;//createASeverCmd(getServerCmdListLastNode());   
    //server_cmd_content.cmd_sn =*((uint32_t *)(data+2));
    node.cmd =*data;
    temp32=*((uint32_t *)(data+1));
    node.cmd_sn=temp32;
    server_down_cmd[server_currentcmd_num].cmd_sn=node.cmd_sn;
    server_down_cmd[server_currentcmd_num].flag =1;
    server_currentcmd_num++;
    temp16=*((uint16_t *)(data+5));
    node.package_len=BigtoLittle16(temp16); 
   // setReportPackageLen( node->package_len);
    //*((uint32_t *)(data+7));//心跳时间
    temp32=*((uint32_t *)(data+11));
    node.report_duaration=BigtoLittle32(temp32);
//    set_timer_report_period(node.report_duaration); //上传时间周期
    temp16=*((uint16_t *)(data+15));
    node.reserve_byte=BigtoLittle16(temp16); //保留字节
//    setReserveByte(node.reserve_byte);
    node.max_retry=*(data+17); //最大重试次数
    setMaxRetryCnt(node.max_retry);
    log_s("!!!!!!!!!sever cmd0xC1 report_duaration=%d max_retry=%d \r\n",node.report_duaration,node.max_retry);
    return CMD0XC1_LEN; //0xc0的长度为9
  }
  return 0;  
}

uint16_t parseServerCmd0xC2(uint8_t *data,uint16_t len)
{
  uint8_t ctrled_state;
 // Server_cmd node;
 // uint16_t temp16;
 // uint32_t temp32;
 // log_i("parseServerCmd0xC2 flag=%d len=%d\r\n",*data,len);  
  if(*data==0xc2&&len>=CMD0XC2_LEN)
  {
   // Server_cmd * node= createASeverCmd(getServerCmdListLastNode());   
    //server_cmd_content.cmd_sn =*((uint32_t *)(data+2));
  //   node.cmd=0xc2;
   // temp32=*((uint32_t *)(data+1));
   // node.cmd_sn=BigtoLittle32(temp32);
    server_down_cmd[server_currentcmd_num].cmd_sn=(*((uint32_t *)(data+1)));
    server_down_cmd[server_currentcmd_num].flag =1;
    server_currentcmd_num++;
//    temp16=*((uint16_t *)(data+5));
//    node.package_len=BigtoLittle16(temp16);
   // setReportPackageLen( node.package_len);
    ctrled_state=*((uint8_t *)(data+7));
    
    if(ctrled_state&0x01)
    {
       gd_eval_led_on(LED1); 
    }
    else
    {
       gd_eval_led_off(LED1);
    }
    if(ctrled_state&0x02)
    {
       gd_eval_led_on(LED2);
    }
    else
    {
       gd_eval_led_off(LED2);; 
    }
    ///////LGF////ADD LED PROCESS///////////// 
    //here set maxtry!!!!!!!!!!!!!!
    log_s("!!!!!!!!!sever cmd0xC2  led_state=%d\r\n",ctrled_state);
    return CMD0XC2_LEN; //0xc0的长度为9
  }
  return 0;  
}

 
void parseSeverAckCmd(uint8_t *cmdspackage,uint16_t cmds_len)
{
   
  uint8_t *data=cmdspackage; 
  uint16_t left_len=cmds_len;//package->cmd_len-(sizeof(Sever_to_nb_package)-4)+5;//+5是因为他们的前五个没算长度
  uint16_t curCmd_len;
 
  for(int i=0;i<cmds_len;i++)
  {  
    if(*data==0xc1)
    { 
      curCmd_len= parseServerCmd0xC1(data,left_len);
      data=data+curCmd_len;
      left_len-=curCmd_len;
      if(left_len==0||curCmd_len==0)
      {
        break;
      }
    }
    else if(*data==0xc2)
    {
      curCmd_len= parseServerCmd0xC2(data,left_len);
      data=data+curCmd_len;
      left_len-=curCmd_len;
      if(left_len==0||curCmd_len==0)
      {
        break;
      }
    }
    else
    {
       log_s("parseSeverAckCmd is error!");
       break;
    }
  }
}
//分析服务器的数据
bool parseServerAckPackage(char *data,uint16_t len)
{
  uint16_t cmd_len=0;
  uint16_t fixed_len=0;
  uint8_t cmd_num;
  if(nbiot_checksum((uint8_t *)(data+2),len-3)!=data[len-1])//校验正确
  {
    log_s("--parseServerAckPackage checksum failed!!!\r\n");
    return false;   
  }
#if 1
  data[len] = data[len-1];
  data[len-1] = 0;
  len++;
#endif
  fixed_len=sizeof(lierda_sever_to_nb_package);
  if(len>=(fixed_len+2))
     cmd_len=len-fixed_len-2; //减1是减去校验位
  else
  {
    log_s("!!parseServerAckPackage cmd_len<0!,return false\r\n");
    return false;
  } 
  memcpy((uint8_t *)&lierda_sever_to_nb_package,(uint8_t *)data,fixed_len);//先赋值固定部分数值
  lierda_sever_to_nb_package.cmd =BigtoLittle16(lierda_sever_to_nb_package.cmd);
  lierda_sever_to_nb_package.len =BigtoLittle16(lierda_sever_to_nb_package.len); 
  lierda_sever_to_nb_package.unix_time =BigtoLittle64(lierda_sever_to_nb_package.unix_time);
  setUnixTime(lierda_sever_to_nb_package.unix_time);
  cmd_num=lierda_sever_to_nb_package.cmd_num;
  setServer_down_CmdNum(cmd_num);
  server_currentcmd_num=0;  
  if((cmd_len>0)&&(cmd_num!=0))
  {
     parseSeverAckCmd((uint8_t *)&data[fixed_len],(uint16_t)cmd_len);
  }
  return true;
}


uint16_t configNbAckPackage(uint8_t *buf)
{
  // uint16_t temp16;
  nb_ack_sever_package.start=BigtoLittle16(0xffaa);
  nb_ack_sever_package.cmd=0xD2D2; 
  nb_ack_sever_package.version=nb_to_lierda_sever_package.version;
  nb_ack_sever_package.device_sn_len= nbiot_get_imei(nb_ack_sever_package.device_sn);
  nb_ack_sever_package.cmd_num=getServer_down_CmdNum();   
  uint16_t fixed_len=sizeof(nb_ack_sever_package); 
  uint16_t len=sizeof(Nb_ack_cmd);
  uint16_t all_len=fixed_len+len*getServer_down_CmdNum()+1;//加上1个校验位  
  log_s("configNbAckPackage:fixed_len=%d,cmd num=%d  all_len=%d\r\n",fixed_len,getServer_down_CmdNum(),all_len);
     
  nb_ack_sever_package.len=BigtoLittle16(all_len);
  //先复制前部分固定长度数据
  memcpy(buf,(uint8_t *)(&nb_ack_sever_package),fixed_len);
  for(int i=0;i<server_currentcmd_num;i++)
  {  
      memcpy(buf+fixed_len+i*len,(uint8_t *)(&server_down_cmd[i]),len);
  }
  buf[all_len-1] = nbiot_checksum(buf+2,all_len-3);
  return all_len;
  
}













