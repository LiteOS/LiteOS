### 关于shell的使用说明

#### 如何加载shell组件

* 1，需要在target_config.h中配置LOSCFG_ENABLE_SHELL为1
* 2，需要在系统初始化完毕后，调用los_shell_init函数（shell.h中定义）
* 3，目前支持tab补全（找到最像的一条）；支持上下键切换历史命令
* 4，如果需要重定向输入输出，请修改shell_get_char和shell_put_char,默认使用getchar和putchar

#### 如何修改编译器设置
以KEIL为例，务必在settings->linker栏目下，misc control添加：--keep *.o(oshell)
该选项会让链接器保留oshell段的内容。
#### 如何添加shell命令

  需要包含头文件<shell.h>

  OSSHELL_EXPORT_CMD(cmdentry，cmdname,cmdhelp)

  其中： cmdentry，shell命令入口函数，函数模型为typedef int (*fn)(int argc, const char *argv[]); 

  	    cmdname，shell命令名字（执行该命令时需要输入的字符串）

  ​        cmdhelp, shell命令的帮助信息

#### 如何添加全局变量

​    可以添加全局变量，可以使用shell入口为全局变量赋值
    添加方法如下：
    需要包含头文件<shell.h>
    定义方式如下：
    OSSHELL_EXPORT_VAR(var,varname,varhelp)
    var为需要添加的全局变量
    varname为该全局变量的名字
    varhelp为该全局变量的描述

  #### 使用范例
```
#include <shell.h>

int shell_test_func(int argc, const char *argv[]){
    int i = 0;
    printf("argc:%d",argc);
    for(i=0;i<argc;i++){
        printf("%s ",argv[i]);
    }
    return 0;
}
static u32_t gs_shell_test_var =123;
//here we export to shell command,you could add your own shell like this
OSSHELL_EXPORT_VAR(gs_shell_test_var,"shell_test_var","shell test var");
OSSHELL_EXPORT_CMD(shell_test_func,"shell_test_func","shell test func");
```


  ​              

  

