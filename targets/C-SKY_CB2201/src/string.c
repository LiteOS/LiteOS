



#include "osport.h"
#include "string.h"//消除警告warning: implicit declaration of function 'strlen' 

//这个函数用于：将字符串转换成argc和argv。
//argv[0]在外面作为命令
//this function is used to format the char string to the argc mode

//会修改原字符串，在使用的时候一定要小心
//this function will changed the original string, used it carefully

//返回很多的参数
//return how many arguments has been 

int string2arg(int *argc, const char *argv[],char *string)
{
	int argvlen = 0;
	int paramnum = 0;
	char *tmp = NULL;
	char bak;
	int len;

	argvlen = *argc;
	*argc = paramnum;
	
	if(NULL == string)//判空
	{
		return paramnum;//0个参数 argc=0
	}

	//use the '\0' to replace the ' '
	len = strlen(string);
	tmp = string;
	while(tmp < (string + len))
	{
		if(*tmp == ' ')
		{
			*tmp = '\0';
		}
		tmp++;
	}
	
	//计算argc，字符串分段保存到argv中
	bak = '\0';
	tmp = string;
	while(tmp < (string + len))
	{
		if((*tmp != '\0')&&(bak =='\0'))
		{
			if(paramnum < argvlen)
			{
				argv[paramnum] = tmp;
				paramnum++;
			}
		}
		bak = *tmp;
		tmp++;
	}
	*argc = paramnum;

	return paramnum;
}