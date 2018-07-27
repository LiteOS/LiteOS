#ifndef _SMTPUTIL_H_
#define _SMTPUTIL_H_

#include "w5500_conf.h"

extern char from[];             //发件人邮箱
extern char to[];            //收件人邮箱
//SMTP_STATE

#define waitfor220 					0
#define waitforHELO250 			1
#define waitforAUTH334 			2
#define waitforuser334 			3
#define waitforpassword235 	4
#define waitforsend250 			5
#define waitforrcpt250 			6
#define waitfordate354 			7
#define waitformime250 			8



void send_mail(void);
void mail_message(void);
void base64encode(char *s, char *r);
void str_insert(char *s1,char *s2,int pos);
void do_smtp(void);

#endif

