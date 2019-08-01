/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-05-14 17:22  zhangqianfu  The first version
 *
 */

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
10,support insert dynamicly, which implemented  by lynus, thanks
*******************************************************************************/

/**************************************FILE INCLIUDES**************************/


#if CONFIG_SHELL_ENABLE


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <osal.h>




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
#define CN_KEY_BS                   (0X000008)    //BACK
#define CN_KEY_ES                   (0X00001B)    //ESPACE
//DEFINES FOR THE COMMAND CACHE BUFFER
#define CN_CMDLEN_MAX  64  //THE COMMAND MAXLENGTH COULD CACHED
#define CN_CMD_CACHE   4   //THE COMMAND CACHED DEPTH
//DEFINES FOR THE SHELL SERVER TASK STACK SIZE
#define CN_SHELL_STACKSIZE  0x600//FOR ALL THE SHELL COMMAND WILL BE EXECUTED IN THE TASK CONTEXT


/**************************************FILE DATA STRUCTURE*********************/
//data structure used for cache the command
struct shell_buffer_t
{
    char             curcmd[CN_CMDLEN_MAX];               //current command edited
    unsigned char    curoffset;                           //position to be edit
    char             tab[CN_CMD_CACHE][CN_CMDLEN_MAX];    //command cache table
    unsigned char    taboffset;                           //position to be cached in table
};

/**************************************FILE VARS*******************************/
//define your global variables here
static  char *gs_welcome_info= "WELCOME TO IOT_LINK SHELL";
/**************************************FILE FUNCTIONS**************************/
//functions import
extern int shell_cmd_execute(char *param);            //execute the command
extern int shell_cmd_init(void);                      //do the command table load
extern const struct shell_tab_matches *shell_cmd_index(const char *index);  //find the most like command


/**************************************FUNCTION IMPLEMENT**********************/
/*******************************************************************************
function     :function used to get char from the input device
parameters   :
instruction  :you could reimplement by redirection,make sure it is block mode
*******************************************************************************/
static int shell_get_char()
{
    return getchar();
}
/*******************************************************************************
function     :function used to put char to the output device
parameters   :
instruction  :you could reimplement by redirection
*******************************************************************************/
static void shell_put_char(int ch)
{
    putchar(ch);
    fflush (stdout) ; //if you use the glibc, then the you must use this to make it
                      //output immediately
}

/*******************************************************************************
function     :function used to put a string
parameters   :
instruction  :you could reimplement by redirection
*******************************************************************************/
static void  shell_put_string(const char *str)
{
    int len;
    int i;
    len = strlen(str);
    for(i =0;i<len;i++)
    {
        shell_put_char(*str);
        str++;
    }
}

/*******************************************************************************
function     :function used to print the path index
parameters   :
instruction  :if you has a getcwd function,you could modify this
*******************************************************************************/
static void shell_put_index()
{
    shell_put_string("\n\rLiteOS:/>");
}

/*******************************************************************************
function     :function used to delete a string in the user display
parameters   :
instruction  :
*******************************************************************************/
static void shell_put_backspace(int times)
{
    int i =0;
    char *str = "\b \b";
    for(i =0;i<times;i++)
    {
        shell_put_string(str);
    }
}

/*******************************************************************************
function     :function used to put a space to display
parameters   :
instruction  :
*******************************************************************************/
static void shell_put_space(int times)
{
    int i =0;
    for(i =0;i<times;i++)
    {
        shell_put_char(' ');
    }
}
/*******************************************************************************
function     :function used to move cursor right
parameters   :
instruction  :
*******************************************************************************/
static void  shell_moves_cursor_right(int times,unsigned int vk)
{
    char ch;
    int i =0;
    for(i =0;i<times;i++)
    {
        ch = (char)(vk&0xff);
        shell_put_char(ch);
        ch = (char)((vk>>8)&0xff);
        shell_put_char(ch);
        ch = (char)((vk>>16)&0xff);
        shell_put_char(ch);
    }
}
/*******************************************************************************
function     :function used ring the terminal's bell
parameters   :
instruction  :
*******************************************************************************/
static void shell_bell() {
    shell_put_char('\a');
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
    for(i = 0;i < CN_CMD_CACHE;i++){
        if(0 == strcmp(tab->curcmd,tab->tab[i])){
            break;
        }
    }
    if(i == CN_CMD_CACHE){
        offset=tab->taboffset;
        memset(tab->tab[offset],0,CN_CMDLEN_MAX);
        strncpy(tab->tab[offset],tab->curcmd,CN_CMDLEN_MAX);
        offset = (offset +1)%CN_CMD_CACHE;
        tab->taboffset = offset;
    }
    return;
}

/*******************************************************************************
function     :insert string at the cursor's position
parameters   :
instruction  :check if it has been cached.
*******************************************************************************/
static void shell_insert_string(struct shell_buffer_t *tab, const char *str) {
    char *cursor;
    cursor = tab->curcmd + tab->curoffset;
    int cursor_len = strlen(cursor);
    int str_len = strlen(str);
    shell_put_string(str);
    shell_put_string(cursor);
    shell_moves_cursor_left(cursor_len);
    
    tab->curoffset += str_len;
    cursor = tab->curcmd + strlen(tab->curcmd) + str_len;
    *(cursor--) = 0;
    for (int i = 0; i < cursor_len; i++) {
        *cursor = *(cursor - str_len);
        cursor--;
    }
    cursor = cursor - (str_len - 1);
    strncpy(cursor, str, str_len);
}
/*******************************************************************************
function     :this is the  shell server task entry
parameters   :
instruction  :we get character from the input device each time,cached it in the 
              current command line,if enter key received, then do command execute;
              if other virtual key received, then do the specified action.
*******************************************************************************/
static int shell_server_entry(void *args)
{
    int    ch;
    int   len;
    unsigned char    offset;
    unsigned int   vk = CN_VIRTUAL_KEY_NULL;
    unsigned int   vkmask = CN_VIRTUAL_KEY_NULL;
    struct shell_buffer_t shell_cmd_cache; 

    memset(&shell_cmd_cache,0,sizeof(shell_cmd_cache));  //initialize the buffer
    shell_put_string(gs_welcome_info);     //put the welcome information
    shell_put_index();                                   //do initialize
    while(1){
        ch= shell_get_char();
        if((ch == CN_VIRTUAL_KEY_NULL)||(EOF == ch)||(ch == 0xFF)){
            continue;
        }
        if((vk&0xFF) != 0)  {
            if(((vk>>8)&0xff) == CN_VIRTUAL_KEY_NULL)  {
                vk|=(ch<<8); 
                continue;    
            }
            else  {
                vk |= (ch <<16);//this is the vk code
                vkmask |= (ch <<16);
            }
        }
        else{
            vkmask  = ch;
        }
        switch (vkmask)
        {
            case CN_VIRTUAL_KEY_ARROWL:        //left,make the cursor moved left
                if(shell_cmd_cache.curoffset > 0){
                    shell_cmd_cache.curoffset--;
                    //push back the left cursor and make the terminal display know what has happened
                    shell_moves_cursor_left(1);
                }else
                    shell_bell();
                //flush the vk
                vk = CN_VIRTUAL_KEY_NULL;
                vkmask = CN_VIRTUAL_KEY_NULL;
                break;
            case CN_VIRTUAL_KEY_ARROWR:        //right,make the cursor moved right,if the buffer has the data
                len = strlen(shell_cmd_cache.curcmd);
                if(shell_cmd_cache.curoffset < len){
                    shell_cmd_cache.curoffset++;
                    //push back the right cursor and make the terminal display know what has happened
                    shell_moves_cursor_right(1,vk);
                } else
                shell_bell();
                //flush the vk
                vk = CN_VIRTUAL_KEY_NULL;
                vkmask = CN_VIRTUAL_KEY_NULL;
                break;
            case CN_VIRTUAL_KEY_ARROWU:        //moves to the previous command
                //first,we should clear what we has get,do the reset current command
                //first moves to the left,then putsnxt to the next to the right,then back
                len = strlen(shell_cmd_cache.curcmd);
                if(shell_cmd_cache.curoffset < len){
                    shell_put_space(len - shell_cmd_cache.curoffset);
                    shell_cmd_cache.curoffset = len;
                }
                shell_put_backspace(len);
                memset(shell_cmd_cache.curcmd,0,CN_CMDLEN_MAX);
                shell_cmd_cache.curoffset = 0;
                //then copy the previous command to current and echo all the info
                offset = (shell_cmd_cache.taboffset +CN_CMD_CACHE -1)%CN_CMD_CACHE;
                strncpy(shell_cmd_cache.curcmd,shell_cmd_cache.tab[offset],CN_CMDLEN_MAX);
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
                if(shell_cmd_cache.curoffset > 0) {
                    char cursor = shell_cmd_cache.curcmd[shell_cmd_cache.curoffset];
                    shell_cmd_cache.curcmd[shell_cmd_cache.curoffset] = 0;
                    const struct shell_tab_matches *matches;
                    matches = shell_cmd_index((const char*)shell_cmd_cache.curcmd);
                    shell_cmd_cache.curcmd[shell_cmd_cache.curoffset] = cursor;
                    if (matches->len == 1) {
                        //only one cmd matched, insert the matched cmd at current cursor
                        shell_insert_string(&shell_cmd_cache, matches->matches[0] + shell_cmd_cache.curoffset);
                    } else if (matches->len > 1) {
                        printf("\n\r");
                        for (int i = 0; i < matches->len; i++) {
                            shell_put_string(matches->matches[i]);
                            shell_put_char('\t');
                        }
                        printf("\n\r");
                        shell_put_index();
                        shell_put_string(shell_cmd_cache.curcmd);
                        shell_moves_cursor_left(strlen(shell_cmd_cache.curcmd) - shell_cmd_cache.curoffset);
                    } else 
                        shell_bell();
                } else
                    shell_bell();
                //flush the vk
                vk = CN_VIRTUAL_KEY_NULL;
                vkmask = CN_VIRTUAL_KEY_NULL;
                break;
            case CN_KEY_LF:      //execute the command here, and push the command to the history cache
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
                    char *substr = shell_cmd_cache.curcmd + shell_cmd_cache.curoffset;
                    int len = strlen(substr);
                    shell_put_char('\b');
                    shell_put_string(substr);
                    shell_put_char(' ');
                    shell_moves_cursor_left(len + 1);
                    
                    strcpy(substr - 1, substr);
                    shell_cmd_cache.curoffset--;
                } else
                    shell_bell();
                //flush the vk
                vk = CN_VIRTUAL_KEY_NULL;
                vkmask = CN_VIRTUAL_KEY_NULL;
                break;
            case CN_KEY_ES:      //esc key,delete all the input here
                vk= CN_KEY_ES;
                vkmask = CN_KEY_ES;
                break;
            default: //other control character will be ignored
                //push the character to the buffer until its full and the '\n' comes
                if(shell_cmd_cache.curoffset <(CN_CMDLEN_MAX-1)){
                    char *p = (char *)&ch;
                    if (*p == 0)
                        // this architecture is big-endian
                        *p = (char)ch;    
                    shell_insert_string(&shell_cmd_cache, p);
                } else
                    shell_bell();
                //flush the vk
                vk = CN_VIRTUAL_KEY_NULL;
                vkmask = CN_VIRTUAL_KEY_NULL;
                break;
        }
    }

    return -1;
}

/*******************************************************************************
function     :this is the  shell module initialize function
parameters   :
instruction  :if you want use shell,you should do two things
              1,make CFG_SHELL_ENABLE true in target_config.h
              2,call shell_init in your process:make sure after the system has
                been initialized
*******************************************************************************/
void shell_init()
{
    shell_cmd_init();

    osal_task_create("shell_server",shell_server_entry,NULL,\
                      CN_SHELL_STACKSIZE+CN_CMD_CACHE*CN_CMDLEN_MAX,NULL,10);
}

#endif



