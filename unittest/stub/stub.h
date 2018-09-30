#ifndef _STUB_H_
#define _STUB_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct stubInfo {  
    void *funcAddr; //保存地址  
    unsigned char byteCode[5]; //保存地址对应的指令  
} stubInfo;  

void setStub(void *funcAddr, void *stubAddr, stubInfo *si);
void setIntStub(int *funcAddr, int *stubAddr, stubInfo *si);
void cleanStub(stubInfo *si);

#ifdef __cplusplus
}
#endif
#endif