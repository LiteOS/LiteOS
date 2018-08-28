#include <sys/mman.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdio.h>  
#include "stub.h"
  
#define JMP_CMD 0xe9    //相对跳转指令 jmp xxx (总共长5个字节，后四个字节为地址)  
                        //0xff 绝对跳转指令：FF   
#if 0
typedef struct stubInfo {  
    void *funcAddr; //保存地址  
    unsigned char byteCode[5]; //保存地址对应的指令  
} stubInfo;  
  #endif
static void setJumpCode(void *codeAddr, char jumpCode[5])  
{  
   int pagesize = sysconf(_SC_PAGE_SIZE);//4096=0x1000  
   if (mprotect((void*) ((unsigned long) codeAddr & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {   //修改页面为可写  
      return;  
   }  
   memcpy(codeAddr, jumpCode, 5);  
}  
  
void setStub(void *funcAddr, void *stubAddr, stubInfo *si)  
{  
    char jumpCode[5] = {0xe9}; //跳转指令  
    int  dist = stubAddr - funcAddr - 5;    //相对偏移  
  
    memcpy((void *)&jumpCode[1], (void *)&dist, sizeof(void *));  
    si->funcAddr = funcAddr; //保存原函数的地址  
    memcpy((void *)&si->byteCode[0], (void *)funcAddr, 5);   //保存原地址处的指令  
      
    setJumpCode(funcAddr, jumpCode); //用跳转指令替换 原地址处的指令  
    //此函数相当于：  
    //*(char*)funcAddr=0xe9;  
    //*(int*)((char*)funcAddr+1)=stubAddr-funcAddr-5;  
}  

void setIntStub(int *funcAddr, int *stubAddr, stubInfo *si)  
{  
    char jumpCode[5] = {0xe9}; //跳转指令  
    int  dist = stubAddr - funcAddr - 5;    //相对偏移  
  
    memcpy((void *)&jumpCode[1], (void *)&dist, sizeof(void *));  
    si->funcAddr = funcAddr; //保存原函数的地址  
    memcpy((void *)&si->byteCode[0], (void *)funcAddr, 5);   //保存原地址处的指令  
      
    setJumpCode(funcAddr, jumpCode); //用跳转指令替换 原地址处的指令  
    //此函数相当于：  
    //*(char*)funcAddr=0xe9;  
    //*(int*)((char*)funcAddr+1)=stubAddr-funcAddr-5;  
}  

  
void cleanStub(stubInfo *si)  
{  
    char   jumpCode[5]; //用来存放原地处的指令  
    memcpy((void *)&jumpCode, (void *)&si->byteCode[0], 5);  
    setJumpCode(si->funcAddr, jumpCode); //恢复原地址处的指令  
    //(省略了还原原地址所在页的保护属性)  
}  
 #if 0
int stub(){ //桩函数  
        printf("this is stub func !\n");  
        return 0;  
}  
  
int a(){  //测试函数，打桩点  
        printf("this is a func !\n");  
        return 0;  
}  
  
int main(){  
        stubInfo si;  
        setStub(a,stub,&si);  
        a(); //此时会调用桩函数      
    cleanStub(&si);  
        a();    
  
} 
#endif 
