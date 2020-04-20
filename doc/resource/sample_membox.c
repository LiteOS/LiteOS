/*************************************************************************
	> File Name: it_membox_performance.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Jul 2015 04:55:25 PM CST
 ************************************************************************/

#include<stdio.h>
#include "los_membox.h"

VOID los_membox_test(void) {
    UINT32 *p_num = NULL;
    UINT32 uwBlkSize = 10, uwBoxSize = 100;
    UINT32 uwRet;
    UINT32 pBoxMem[1000];
    uwRet = LOS_MemboxInit( &pBoxMem[0], uwBoxSize, uwBlkSize);
    if(uwRet != LOS_OK)
    {
        dprintf("内存池初始化失败!\n");
        return;
    }
    else{
        dprintf("内存池初始化成功!\n");
    }
    
    /*申请内存块*/
    p_num = (int*)LOS_MemboxAlloc(pBoxMem);
    if (NULL == p_num) {
        dprintf("内存分配失败!\n");
        return;
    }
    dprintf("内存分配成功\n");
    /*赋值*/
    *p_num = 828;
    dprintf("*p_num = %d\n", *p_num);
     /*清除内存内容*/
     LOS_MemboxClr(pBoxMem, p_num);
     dprintf("清除内存内容成功\n *p_num = %d\n", *p_num);
    /*释放内存*/
    uwRet = LOS_MemboxFree(pBoxMem, p_num);
    if (LOS_OK == uwRet) {
        dprintf("内存释放成功!\n");
    }
    else{
        dprintf("内存释放失败!\n");
    }
	return;
}


