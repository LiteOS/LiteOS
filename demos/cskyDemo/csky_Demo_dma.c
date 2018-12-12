

#include "drv_dmac.h"
#include "stdio.h"
#include "soc.h"
#include <string.h>

#include "csky_All_Demo.h"
#include "los_hwi.h"


static  volatile uint8_t dma_cb_flag = 0;
#ifndef DMA_M2M_SIZE
#define DMA_M2M_SIZE    512
#endif
static uint8_t p_src[DMA_M2M_SIZE] = {0};
static uint8_t p_dst[DMA_M2M_SIZE] = {0};

static void sleep(uint32_t k)
{
    int i, j;

    for (i = 0; i < 1000; i++) {
        for (j = 0; j < k; j++);
    }
}


static void dma_event_cb_fun(int32_t ch, dma_event_e event)
{
	printf("[info]进入DMA回调函数\n");
    dma_cb_flag = 1;
}

static int32_t dma_test_mem2mem(dmac_handle_t dma_handle, uint8_t ch)
{
    uint32_t i;
    dma_config_t config;
    uint32_t ret;

    for (i = 0; i < DMA_M2M_SIZE; i++) {
        p_src[i] = i;
    }

    memset(p_dst, 0, DMA_M2M_SIZE);

    ch = csi_dma_alloc_channel(dma_handle, ch);

    if (ch < 0) {
        printf("[error]csi_dma_alloc_channel error\n");
        return -1;
    }
	printf("[success]csi_dma_alloc_channel success");
	
    config.src_inc  = DMA_ADDR_INC;
    config.dst_inc  = DMA_ADDR_INC;
    config.src_endian = DMA_ADDR_LITTLE;
    config.dst_endian = DMA_ADDR_LITTLE;
    config.src_tw   = 1;
    config.dst_tw   = 1;
    config.group_len = 8;
    config.mode = DMA_BLOCK_TRIGGER;
    config.type     = DMA_MEM2MEM;
    config.ch_mode  = DMA_MODE_SOFTWARE;
    ret = csi_dma_config_channel(dma_handle, ch, &config, dma_event_cb_fun);
    if (ret < 0) {
        printf("[error]csi_dma_config_channel error\n");
        return 0;
    }
	printf("[success]csi_dma_config_channel success\n");
	
    ret = csi_dma_start(dma_handle, ch, p_src, p_dst, DMA_M2M_SIZE);
    if (ret < 0) {
        printf("[error]csi_dma_start error\n");
        return 0;
    }
	printf("[success]csi_dma_start success\n");
	
    printf("[info]sleep or do other things while data in transfermation using DMA\n");
    sleep(1);

    //while (csi_dma_get_status(dma_handle, ch) != DMA_STATE_DONE) ;
    while (!dma_cb_flag);

	//比较dma传输的数值，是否一样
    for (i = 0; i < DMA_M2M_SIZE; i++) {
        if (p_dst[i] != p_src[i]) {
            return -1;
        }
    }
	printf("[success] dma传输的数值正确\n");
	
    ret = csi_dma_stop(dma_handle, ch);
    if (ret < 0) {
        printf("[error]csi_dma_stop error\n");
        return 0;
    }
	printf("[success]csi_dma_stop success\n");

    ret = csi_dma_release_channel(dma_handle, ch);

    if (ret < 0) {
        printf("[error]csi_dma_release_channel error\n");
        return 0;
    }
	printf("[success]csi_dma_release_channel success\n");
	
    ret = csi_dma_uninitialize(dma_handle);

    if (ret < 0) {
        printf("csi_dma_uninitialize error\n");
        return 0;
    }
	printf("[success]csi_dma_uninitialize success\n");	
	

    return 0;
}
static int32_t test_dmac(uint8_t ch)
{
    int32_t ret;
    dmac_handle_t dma_handle = NULL;

    dma_handle = csi_dma_initialize(0);

    if (dma_handle == NULL) {
        printf("[error]csi_dma_initialize error\n");
        return -1;
    }
	printf("[success]csi_dma_initialize success\n");
	
    ret = dma_test_mem2mem(dma_handle, ch);
    if (ret < 0) {
        printf("[error]test dma mem to mem error\n");
        return -1;
    }
	printf("[success]dma_mem2mem_test_func success\n");
	
    return 0;
}

int example_dmac(uint8_t ch)
{
    int ret;
    ret = test_dmac(ch);

    if (ret < 0) {
        printf("[error]test dmac failed\n");
        return -1;
    }

    printf("[success]test dmac successfully\n");
    return 0;
}

int task_DMA(void)
{
	
#if LOSCFG_PLATFORM_HWI		//接管中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("接管中断模式 --> DMA")
#endif
	UINTPTR uvIntSave;										//用于保存所有的中断情况
	uvIntSave = LOS_IntLock();								//禁用所有中断，并返回原先中断情况
	LOS_HwiCreate (SEU_DMAC_IRQn, 0, 0,(void (*) (uintptr_t))dma_event_cb_fun,0);//将硬件中断向量号绑定到系统中，指向中断服务函数
	LOS_IntRestore(uvIntSave);								//恢复所有中断情况
#else						//直接中断模式
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> DMA")
#endif
	csi_vic_enable_irq(SEU_DMAC_IRQn);						//底层层序的中断使能，这里不需要使用。该系统只需要底层给的中断信号。
#endif
	
	HopeRun_printf("开始DMA测试............\n");
    return example_dmac(0);									//0 SEU_DMAC_IRQn 1 NONSEU_DMAC_IRQn
}

