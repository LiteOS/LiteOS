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

/**********************************README***************************************
this file implement the shell for the system.the following instruction you must take care!
1,anyway,you should implement the getchar and puchar first,maybe you could do the redirection here
  i don't care what the stdin and stdout is,that depends you!
2,you could call the shell_install after the system is up!for we will create task here
3,we will do the echo in the shell server:only for the isprint charactor
4,1B XX 41 move the current cursor up
5,1B XX 42 move the current cursor down
6,1B XX 44 move the current cursor left
7,1B XX 43 move the current cursor right
8,'\B' move the current cursor left
9,1B delete all the input and make the following code transfer
10,not support the command insert dynamic yet
*******************************************************************************/
/**************************************FILE INCLIUDES**************************/
#include <target_config.h>
#if CN_OS_SHELL
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "osport.h"


/**************************************FILE DEFINES****************************/
//DEFINE VIRTUAL KEYS:VIRTUAL KEYS ARE COMBINED BY:0X1b 0xXX CODE
#define CN_VIRTUAL_KEY_NULL         (0x000000)    //NULL
#define CN_VIRTUAL_KEY_ARROWU       (0X41001B)    //UP
#define CN_VIRTUAL_KEY_ARROWD       (0X42001B)    //DOWN
#define CN_VIRTUAL_KEY_ARROWR       (0X43001B)    //RIGHT
#define CN_VIRTUAL_KEY_ARROWL       (0X44001B)    //LEFT
//DEFINE SOME SPECIAL KEYS 
#define CN_KEY_LF                   (0X00000D)    //ENTER
#define CN_KEY_CR                   (0X00000A)    //NEW
#define CN_KEY_TAB                  (0X000009)    //TAB
#define CN_KEY_BS           		(0X000008)    //BACK
#define CN_KEY_ES           		(0X00001B)    //ESPACE
//DEFINES FOR THE COMMAND CACHE BUFFER
#define CN_CMDLEN_MAX  32  //THE COMMAND MAXLENGTH COULD CACHED
#define CN_CMD_CACHE   8   //THE COMMAND CACHED DEPTH
//DEFINES FOR THE SHELL SERVER TASK STACK SIZE
#define CN_SHELL_STACKSIZE  512//FOR ALL THE SHELL COMMAND WILL BE EXECUTED IN THE TASK CONTEXT


/**************************************FILE DATA STRUCTURE*********************/
//data structure used for cache the command
struct shell_buffer_t{
	char    curcmd[CN_CMDLEN_MAX];               //current command edited
	u8_t    curoffset;                           //position to be edit
	char    tab[CN_CMD_CACHE][CN_CMDLEN_MAX];    //command cache table
	u8_t    taboffset;                           //position to be cached in table
};

/**************************************FILE VARS*******************************/
//define your global variables here
static  char *gs_welcome_info= "WELCOME TO LITEOS SHELL";

/**************************************FILE FUNCTIONS**************************/
//functions import
extern s32_t shell_cmd_execute(char *param);            //execute the command
extern s32_t shell_cmd_init(void);                      //do the command table load
extern const char *shell_cmd_index(const char *index);  //find the most like command
//functions export
void shell_install(void);                                   //install the shell component

/**************************************FUNCTION IMPLEMENT**********************/
/*******************************************************************************
function     :function used to get char from the input device
parameters   :
instruction  :you could reimplement by redirection,make sure it is block mode
*******************************************************************************/
static int shell_get_char(){
    return getchar();
}
/*******************************************************************************
function     :function used to put char to the output device
parameters   :
instruction  :you could reimplement by redirection
*******************************************************************************/
static void shell_put_char(int ch){
    putchar(ch);
}

/*******************************************************************************
function     :function used to put a string
parameters   :
instruction  :you could reimplement by redirection
*******************************************************************************/
static void  shell_put_string(char *str){
    int len;
    int i;
    len = strlen(str);
    for(i =0;i<len;i++){
        shell_put_char(*str);
        str++;
    }
}

/*******************************************************************************
function     :function used to print the path index
parameters   :
instruction  :if you has a getcwd function,you could modify this
*******************************************************************************/
static void shell_put_index(){
	shell_put_string("\n\rLiteOS:/>");
}

/*******************************************************************************
function     :function used to delete a string in the user display
parameters   :
instruction  :
*******************************************************************************/
static void shell_put_backspace(int times){
	int i =0;
	char *str = "\b \b";
	for(i =0;i<times;i++){
		shell_put_string(str);
	}
}

/*******************************************************************************
function     :function used to put a space to display
parameters   :
instruction  :
*******************************************************************************/
static void shell_put_space(int times){
	int i =0;
	for(i =0;i<times;i++){
		shell_put_char(' ');
	}
}
/*******************************************************************************
function     :function used to move cursor right
parameters   :
instruction  :
*******************************************************************************/
static void  shell_moves_cursor_right(int times,u32_t vk){
	char ch;
	int i =0;
	for(i =0;i<times;i++){
		ch = (char)(vk&0xff);
		shell_put_char(ch);
		ch = (char)((vk>>8)&0xff);
		shell_put_char(ch);
		ch = (char)((vk>>16)&0xff);
		shell_put_char(ch);
	}
}
/*******************************************************************************
function     :function used to move cursor left
parameters   :
instruction  :
*******************************************************************************/
static void  shell_moves_cursor_left(int times){
	int i =0;
	for(i =0;i<times;i++){
		shell_put_char('\b');
	}
}


/*******************************************************************************
function     :function used cache the current command
parameters   :
instruction  :check if it has been cached.
*******************************************************************************/
static void shell_cachecmd(struct shell_buffer_t *tab){
	int i = 0;
	int offset;
	
	//找寻当前命令字符组在不在历史缓存中
	for(i = 0;i < CN_CMD_CACHE;i++)
	{
		if(0 == strcmp(tab->curcmd,tab->tab[i]))
		{
			break;//如果找到一样的，则i!=CN_CMD_CACHE
		}
	}
	
	//在历史缓存中未找到该命令，则需要把该命令存放到命令历史中
	if(i == CN_CMD_CACHE)
	{
        offset=tab->taboffset;
		memset(tab->tab[offset],0,CN_CMDLEN_MAX);
		strncpy(tab->tab[offset],tab->curcmd,CN_CMDLEN_MAX);//保存到当前偏移指向的空间
		
		offset = (offset +1)%CN_CMD_CACHE;//指向下一条，为之后的命令保存做准备
		tab->taboffset = offset;
	}
	return;
}

/*******************************************************************************
function     :this is the  shell server task entry
parameters   :
instruction  :we get character from the input device each time,cached it in the 
              current command line,if enter key received, then do command execute;
			  if other virtual key received, then do the specified action.
*******************************************************************************/
static u32_t shell_server_entry(void *args){
	s32_t    ch;
	s32_t   len;
	u8_t    offset;
	u32_t   vk = CN_VIRTUAL_KEY_NULL;    //VIRTUAL_KEY 虚拟按键值（0x000000）
	u32_t   vkmask = CN_VIRTUAL_KEY_NULL;
    struct shell_buffer_t shell_cmd_cache; 
	const char *cmdindex;

	//清空shell输入字符的缓存空间
	memset(&shell_cmd_cache,0,sizeof(shell_cmd_cache));  //initialize the buffer
	
	//显示shell的欢迎文字
	shell_put_string(gs_welcome_info);     //put the welcome information
    shell_put_index();                      //do initialize，输出命令行前缀 LiteOS:/>
	
    while(1){
    	ch= shell_get_char();				//得到输入的字符
    	if((ch == CN_VIRTUAL_KEY_NULL)||(EOF == ch)||(ch == 0xFF))//如果输入的字符为 NULL，-1,0xFF，则忽略本次输入
		{
    		continue;
    	}
		
		//如果VIRTUAL_KEY（0x0000AA）第一字节已经被赋值
    	if((vk&0xFF) != 0)  
		{
			//如果第二字节为空，则在第二字节赋值,(0x00BBAA)
    		if(((vk>>8)&0xff) == CN_VIRTUAL_KEY_NULL)  
			{
    			vk|=(ch<<8); 
    			continue;    
    		}
    		else //否则在第三字节赋值(0xCCBBAA)
			{
    			vk |= (ch <<16);//this is the vk code
    			vkmask |= (ch <<16);
    		}
    	}
    	else//如果VIRTUAL_KEY（0x000000）第一字节未被赋值，则在先在mask里保存该值
		{
    		vkmask  = ch;
    	}
		
		
    	switch (vkmask)
		{
			//光标左移
			case CN_VIRTUAL_KEY_ARROWL:        //left,make the cursor moved left
				if(shell_cmd_cache.curoffset > 0)
				{
					shell_cmd_cache.curoffset--;
					//push back the left cursor and make the terminal display know what has happened
					shell_moves_cursor_left(1);
				}
				//flush the vk （刷掉VK值）
				vk = CN_VIRTUAL_KEY_NULL;		//(0x000000)
				vkmask = CN_VIRTUAL_KEY_NULL;	//(0x000000)
				break;
				
			//光标右移
			case CN_VIRTUAL_KEY_ARROWR:        //right,make the cursor moved right,if the buffer has the data
				len = strlen(shell_cmd_cache.curcmd);
				if(shell_cmd_cache.curoffset < len)
				{
					shell_cmd_cache.curoffset++;
					//push back the right cursor and make the terminal display know what has happened
					shell_moves_cursor_right(1,vk);
				}
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
				
			//光标上移，调出前一个命令字符
			case CN_VIRTUAL_KEY_ARROWU:        //moves to the previous command
				//first,we should clear what we has get,do the reset current command
				//first moves to the left,then putsnxt to the next to the right,then back
				len = strlen(shell_cmd_cache.curcmd);//当前命令的长度
				if(shell_cmd_cache.curoffset < len)//如果上一条命令比现在显示的短
				{
					shell_put_space(len - shell_cmd_cache.curoffset);//则需要输出空格覆盖
					shell_cmd_cache.curoffset = len;
				}
				shell_put_backspace(len);
				memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
				shell_cmd_cache.curoffset = 0;
				
				
				//then copy the previous command to current and echo all the info
				offset = (shell_cmd_cache.taboffset +CN_CMD_CACHE -1)%CN_CMD_CACHE;//得到上一条的命令偏移
				strncpy(shell_cmd_cache.curcmd,shell_cmd_cache.tab[offset],CN_CMDLEN_MAX);//将上一条命令复制到current
				shell_cmd_cache.taboffset = offset;
				shell_cmd_cache.curoffset = strlen(shell_cmd_cache.curcmd);
				
				
				//OK,now puts all the current character to the terminal
				shell_put_string(shell_cmd_cache.curcmd);
				
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				
				break;
			case CN_VIRTUAL_KEY_ARROWD:        //moves to the next command
				//first,we should clear what we has get,do the reset current command
				//first moves to the right,then backspace all the input
				len = strlen(shell_cmd_cache.curcmd);
				if(shell_cmd_cache.curoffset < len){
					shell_put_space(len - shell_cmd_cache.curoffset);
					shell_cmd_cache.curoffset = len;
				}
				shell_put_backspace(len);
				memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
				shell_cmd_cache.curoffset = 0;
				//then copy the next command to current
				offset = (shell_cmd_cache.taboffset +CN_CMD_CACHE +1)%CN_CMD_CACHE;
				strncpy(shell_cmd_cache.curcmd,shell_cmd_cache.tab[offset],CN_CMDLEN_MAX);
				shell_cmd_cache.taboffset = offset;
				shell_cmd_cache.curoffset = strlen(shell_cmd_cache.curcmd);
				//OK,now puts all the current character to the terminal
				shell_put_string(shell_cmd_cache.curcmd);
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
			case CN_KEY_TAB:      //tab,should auto complete the command
				//should search the command we has installed if some index has get
				len = strlen(shell_cmd_cache.curcmd);
				if(len > 0) {
					cmdindex = shell_cmd_index((const char*)shell_cmd_cache.curcmd);
					if(NULL != cmdindex) {
						//first,we should clear what we has get,do the reset current command
						//first moves to the right,then backspace all the input
						len = strlen(shell_cmd_cache.curcmd);
						if(shell_cmd_cache.curoffset < len){
							shell_put_space(len - shell_cmd_cache.curoffset);
							shell_cmd_cache.curoffset = len;
						}
						shell_put_backspace(len);
						memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
						shell_cmd_cache.curoffset = 0;
						//then copy the command and puts the string here
						strncpy(shell_cmd_cache.curcmd,cmdindex,CN_CMDLEN_MAX);
						shell_cmd_cache.curoffset = strlen(shell_cmd_cache.curcmd);
						//OK,now puts all the current character to the terminal
						shell_put_string(shell_cmd_cache.curcmd);
					}
				}
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
			case CN_KEY_LF:      //execute the command here, and push the command to the history cache
	            if(strlen(shell_cmd_cache.curcmd) != 0)
				{
					printf("\n\r");
		        	//copy the current to the history cache if the current command is not exist
					//must do before the execute,execute will split the string
					//在执行之前的必要操作，将当前命令保存到缓存区（如果是历史命令则不做保存）
                    shell_cachecmd(&shell_cmd_cache);
					
	                shell_cmd_execute(shell_cmd_cache.curcmd);  //execute the command
					
					//收尾工作
	        		memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
	        		shell_cmd_cache.curoffset = 0;
	            }
	            shell_put_index();
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
			case CN_KEY_CR:      //execute the command here, and push the command to the history cache
	            if(strlen(shell_cmd_cache.curcmd) != 0){
					printf("\n\r");
		        	//copy the current to the history cache if the current command is not none
					shell_cachecmd(&shell_cmd_cache);//must do before the execute,execute will split the string
	                shell_cmd_execute(shell_cmd_cache.curcmd);  //execute the command
	        		memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
	        		shell_cmd_cache.curoffset = 0;
	            }
	            shell_put_index();
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
			case CN_KEY_BS:      //should delete the current character,move all the following character 1 position to before
				if(shell_cmd_cache.curoffset >0){
					shell_cmd_cache.curcmd[shell_cmd_cache.curoffset] = 0;
					shell_cmd_cache.curoffset--;
					shell_put_backspace(1);
				}
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
			case CN_KEY_ES:      //esc key,delete all the input here
				len = strlen(shell_cmd_cache.curcmd);
				if(shell_cmd_cache.curoffset < len){
					shell_put_space(len - shell_cmd_cache.curoffset);
					shell_cmd_cache.curoffset = len;
				}
				shell_put_backspace(len);
				memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
				shell_cmd_cache.curoffset = 0;
				//this is also the transfer code
				vk= CN_KEY_ES;
				vkmask = CN_KEY_ES;
				break;
			default: //other control character will be ignored
				//push the character to the buffer until its full and the '\n' comes
				if(shell_cmd_cache.curoffset <(CN_CMDLEN_MAX-1)){
					shell_cmd_cache.curcmd[shell_cmd_cache.curoffset] = ch;
					shell_cmd_cache.curoffset++;
					shell_put_char(ch);   //should do the echo
				}
				//flush the vk
				vk = CN_VIRTUAL_KEY_NULL;
				vkmask = CN_VIRTUAL_KEY_NULL;
				break;
		}
    }
	return 0;//新增，消除警告：warning: no return statement in function returning non-void 
}

/*******************************************************************************
function     :this is the  shell module initialize function
parameters   :
instruction  :if you want use shell,you should do two things
              1,make CN_OS_SHELL true in target_config.h
			  2,call shell_install in your process:make sure after the system has
			    been initialized
*******************************************************************************/
void shell_install()
{
	//启动打印
	printf("###### >>>>>>>>>>>> LiteOS on CH2201 <<<<<<<<<<<< ######\n");
	printf("###### >>>>>>>>>>>> 20181119         <<<<<<<<<<<< ######\n");
    shell_cmd_init();
    task_create("shell_server",(fnTaskEntry)shell_server_entry,CN_SHELL_STACKSIZE+CN_CMDLEN_MAX*CN_CMDLEN_MAX,NULL,NULL,10);
}

#endif




