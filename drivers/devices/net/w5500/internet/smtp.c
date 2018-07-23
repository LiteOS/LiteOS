/**
******************************************************************************
* @file   			smtp.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   			smtp 协议
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500.h"
#include "smtp.h"
#include "socket.h"

extern CONFIG_MSG ConfigMsg;
uint8 mail_send_ok=0;
char RX_BUF[2048];
char hello[50]="HELO localhost";      								/*身份标识命令*/
char hello_reply[]="250 OK";          								/*身份标识成功响应*/
char AUTH[50]="AUTH LOGIN";           								/*认证请求*/
char AUTH_reply[]="334 dXNlcm5hbWU6";       						/*认证请求发送成功响应*/
char name_126[100]="wiznet2013@126.com";    						/*126登录邮箱名*/ 
char base64name_126[200];                   						/*126登录邮箱名的base64编码*/
char name_reply[]="334 UGFzc3dvcmQ6";       						/*发送登录名成功响应*/
char password_126[50]="hello123";           						/*126 登陆邮箱密码*/
char base64password_126[100];               						/*base64 126登录邮箱密码*/
char password_reply[]="235 Authentication successful";      		/*登陆成功响应*/
char from[]="cnapool@163.com";             						    /*发件人邮箱*/
char from_reply[]="250 Mail OK";
char to[]="1074867723@qq.com";            							/*收件人邮箱*/
char to_reply[]="250 Mail OK";
char data_init[10]="data";                							/*请求数据传输*/
char data_reply[]="354";                  							/*请求成功响应 HEAD*/
char Cc[]="ma_minjie@hoperun.com";         							/*抄送人邮箱*/
char subject[]="Hello!硬石YS-F1Pro开发板!www.ing10bbs.com";    			/*主题*/
char content[]="www.ing10bbs.com";           						/*正文*/
char mime_reply[]="250 Mail OK queued as";							/*邮件发送成功响应*/
char mailfrom[50]="MAIL FROM:<>";
char rcptto[50]="rcpt to:<>";
char mime[200]="From:\r\n";
char mime1[50]="To:\r\n";
char mime2[50]="Cc:\r\n";
char mime3[50]="Subject:\r\n";
char mime4[50]="MIME-Version:1.0\r\nContent-Type:text/plain\r\n\r\n";
char mime5[50]="\r\n.\r\n";

/**
*@brief		邮件命令信息处理
*@param		无
*@return	无
*/
void mail_message(void)  
{
	uint16 len_from=strlen(from);
	uint16 len_to=strlen(to);
	uint16 len_Cc=strlen(Cc);
	uint16 len_sub=strlen(subject);
	strcat (hello,"\r\n");
	strcat (AUTH,"\r\n");
	base64encode(name_126,base64name_126);
	base64encode(password_126,base64password_126);
	strcat(base64name_126,"\r\n");
	strcat(base64password_126,"\r\n");
	str_insert(mailfrom,from,11);
	strcat(mailfrom,"\r\n");
	str_insert(rcptto,to,9);
	strcat(rcptto,"\r\n");
	strcat(data_init,"\r\n");
	
	str_insert(mime,from,5);
	str_insert(mime1,to,3);
	str_insert(mime2,Cc,3);
	str_insert(mime3,subject,8);
	str_insert(mime5,content,0);
	strcat(mime,mime1);
	strcat(mime,mime2);
	strcat(mime,mime3);
	strcat(mime,mime4);
	strcat(mime,mime5);
}

/**
*@brief		发送邮件
*@param		无
*@return	无
*/
uint8 SMTP_STATE=waitfor220;
void send_mail(void)           
{
	switch(SMTP_STATE)
	{
		case waitfor220:													/*等待连接成功的正确响应状态*/
			if(strstr((const char *)RX_BUF,"220")!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)hello,strlen(hello));			/*发送hello命令*/
				SMTP_STATE=waitforHELO250;   
			}
		break;
		case waitforHELO250:												/*等待hello命令的正确响应状态*/
			if(strstr((const char *)RX_BUF,hello_reply)!=NULL&&strstr((const char *)RX_BUF,"Mail")==NULL)
			{
				send(SOCK_SMTP,(const uint8 *)AUTH,strlen(AUTH));			/*发送AUTH认证请求*/
				SMTP_STATE=waitforAUTH334;
			}
		break;
		case waitforAUTH334:												/*等待AUTH认证请求的正确响应*/
			if(strstr((const char *)RX_BUF,AUTH_reply)!=NULL)
			{
				/*发送邮箱登录名*/
				send(SOCK_SMTP,(const uint8 *)base64name_126,strlen(base64name_126));
				SMTP_STATE=waitforuser334;
			}
		break;
		case waitforuser334:												/*等待邮箱登录名登陆成功的正确响应*/
			if(strstr((const char *)RX_BUF,name_reply)!=NULL)
			{ 
				/*发送邮箱登录密码*/
				send(SOCK_SMTP,(const uint8 *)base64password_126,strlen(base64password_126));
				SMTP_STATE=waitforpassword235;
			}
		break;
		case waitforpassword235:											/*等待邮箱密码登陆成功正确响应*/
			if(strstr((const char *)RX_BUF,password_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)mailfrom,strlen(mailfrom));	/*发送发件人信息*/
				SMTP_STATE=waitforsend250;
			}
		break;
		case waitforsend250:												/*等待发件人信息发送成功正确响应*/
			if(strstr((const char *)RX_BUF,from_reply)!=NULL&&strstr((const char *)RX_BUF,"queued as")==NULL)
			{
				send(SOCK_SMTP,(const uint8 *)rcptto,strlen(rcptto));		/*发送收件人信息*/
				SMTP_STATE=waitforrcpt250;
			}
		break;
		case waitforrcpt250:												/*等待收件人信息发送成功正确响应*/
			if(strstr((const char *)RX_BUF,to_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)data_init,strlen(data_init));	/*发送数据传输请求命令*/
				SMTP_STATE=waitfordate354;
			}
		break;
		case waitfordate354:												/*等待发送数据传输请求命令发送成功正确响应*/
			if(strstr((const char *)RX_BUF,data_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)mime,strlen(mime));  			/*发送mime类型命令*/
				SMTP_STATE=waitformime250;
			}
		break;
		case waitformime250:												/*等待mime的正确响应*/
			if(strstr((const char *)RX_BUF,mime_reply)!=NULL)
			{
				mail_send_ok=1;												/*邮件发送成功*/
				printf("mail send OK\r\n");    
			}
		break;
		default:
		break;
	}
}

/**
*@brief		SMTP 主函数
*@param		无
*@return	无
*/
void do_smtp(void)                   
{
	uint8 ch=SOCK_SMTP;														/*定义一个变量并赋值SMTP通信socket号*/
	uint16 len;																												/*定义SMTP Client的通信端口号*/
	uint16 anyport=5000;													/*SMTP Server 的端口号，默认为25*/
	uint8 Smtp_PORT=25;
	memset(RX_BUF,0,sizeof(RX_BUF));
	switch(getSn_SR(ch)) 												 	/*读取W5500的socket状态*/
	{
		case SOCK_INIT:														/*初始化完成*/
			connect(ch, ConfigMsg.rip ,Smtp_PORT );	 						/*连接SMTP Server*/
		break;
		case SOCK_ESTABLISHED: 												/*socket建立成功*/
			if(getSn_IR(ch) & Sn_IR_CON)
			{
				setSn_IR(ch, Sn_IR_CON);									/*清除接收中断标志*/
			}
			if ((len = getSn_RX_RSR(ch)) > 0)		
			{
				while(!mail_send_ok)										/*如果邮件没有发送成功*/
				{
					memset(RX_BUF,0,sizeof(RX_BUF));						/*接受缓存的内存空间清零*/
					len = recv(ch, (uint8*)RX_BUF,len);    					/*W5500接收数据并存入RX_BUF*/
					send_mail();																							/*发送邮件*/
				}
				disconnect(ch);												/*断开socket连接*/
			}
		break;
		case SOCK_CLOSE_WAIT:   /*等待socket关闭*/
			if ((len = getSn_RX_RSR(ch)) > 0)
			{       
				while(!mail_send_ok)										/*如果邮件没有发送成功*/
				{
					len = recv(ch, (uint8*)RX_BUF, len);					/*W5500接收数据并存入RX_BUF*/
					send_mail();																							/*发送邮件*/
				} 
			}
			disconnect(ch);													/*断开socket连接*/
		break;
		case SOCK_CLOSED:                   
			socket(ch, Sn_MR_TCP,anyport++, 0x00);							/*打开socket*/
		break;
		default:
		break;
	} 
}

/**
*@brief		base64编码转换函数
*@param		*s: 需要转化的字符 
*@param		*r:存放转化后的字符
*@return	无
*/
void base64encode(char *s, char *r)   
{
	char padstr[4];
	char base64chars[]=	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint16 i=0,c;
	uint32 n;
	c=strlen(s)%3;
	if(c>0)   
	{
		for(i=0;c<3;c++)
		{
	 		padstr[i++]='=';
		}
	}
	padstr[i]=0;
	i=0;
	for(c=0;c<strlen(s);c+=3)
	{
		n=s[c];
		n<<=8;
		n+=s[c+1];
		if(c+2>strlen(s))
		{
			n&=0xff00;
		}
		n<<=8;
		n+=s[c+2];
		if(c+1>strlen(s))
		{
		  n&=0xffff00;
		}
		r[i++]=base64chars[(n>>18)&63];
		r[i++]=base64chars[(n>>12)&63];
		r[i++]=base64chars[(n>>6)&63];
		r[i++]=base64chars[n&63];
	}
		i-=strlen(padstr);
	for(c=0;c<strlen(padstr);c++)
	{
		r[i++]=padstr[c];
	}
	r[i]=0;
}

/**
*@brief		把字符串s2插在s1的pos位开始的地方
*@param		*s1: 字符串1  *s2:字符串2   pos:起始地址
*@return	无
*/
void str_insert(char *s1,char *s2,int pos)
{	
	int i;
	int len=strlen(s2);
	for(i=0;i<len;i++)
	{
		*(s1+pos+len+i)=s1[pos+i];
		*(s1+pos+i)=s2[i];
	}
}
