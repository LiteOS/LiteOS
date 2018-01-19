/*!
    \file  gd32f1x0_dma.h
    \brief definitions for the dma
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef GD32F1X0_DMA_H
#define GD32F1X0_DMA_H

#include "gd32f1x0.h"

/* DMA definitions */
#define DMA                               DMA_BASE               /*!< DMA base address */

/* registers definitions */
#define DMA_INTF                          REG32(DMA + 0x00)     /*!< DMA interrupt flag register */
#define DMA_INTC                          REG32(DMA + 0x04)     /*!< DMA interrupt flag clear register */
#define DMA_CH0CTL                        REG32(DMA + 0x08)     /*!< DMA channel 0 control register */
#define DMA_CH0CNT                        REG32(DMA + 0x0C)     /*!< DMA channel 0 counter register */
#define DMA_CH0PADDR                      REG32(DMA + 0x10)     /*!< DMA channel 0 peripheral base address register */
#define DMA_CH0MADDR                      REG32(DMA + 0x14)     /*!< DMA channel 0 memory base address register */
#define DMA_CH1CTL                        REG32(DMA + 0x1C)     /*!< DMA channel 1 control register */
#define DMA_CH1CNT                        REG32(DMA + 0x20)     /*!< DMA channel 1 counter register */
#define DMA_CH1PADDR                      REG32(DMA + 0x24)     /*!< DMA channel 1 peripheral base address register */
#define DMA_CH1MADDR                      REG32(DMA + 0x28)     /*!< DMA channel 1 memory base address register */
#define DMA_CH2CTL                        REG32(DMA + 0x30)     /*!< DMA channel 2 control register */
#define DMA_CH2CNT                        REG32(DMA + 0x34)     /*!< DMA channel 2 counter register */
#define DMA_CH2PADDR                      REG32(DMA + 0x38)     /*!< DMA channel 2 peripheral base address register */
#define DMA_CH2MADDR                      REG32(DMA + 0x3C)     /*!< DMA channel 2 memory base address register */
#define DMA_CH3CTL                        REG32(DMA + 0x44)     /*!< DMA channel 3 control register */
#define DMA_CH3CNT                        REG32(DMA + 0x48)     /*!< DMA channel 3 counter register */
#define DMA_CH3PADDR                      REG32(DMA + 0x4C)     /*!< DMA channel 3 peripheral base address register */
#define DMA_CH3MADDR                      REG32(DMA + 0x50)     /*!< DMA channel 3 memory base address register */
#define DMA_CH4CTL                        REG32(DMA + 0x58)     /*!< DMA channel 4 control register */
#define DMA_CH4CNT                        REG32(DMA + 0x5C)     /*!< DMA channel 4 counter register */
#define DMA_CH4PADDR                      REG32(DMA + 0x60)     /*!< DMA channel 4 peripheral base address register */
#define DMA_CH4MADDR                      REG32(DMA + 0x64)     /*!< DMA channel 4 memory base address register */
#define DMA_CH5CTL                        REG32(DMA + 0x6C)     /*!< DMA channel 5 control register */
#define DMA_CH5CNT                        REG32(DMA + 0x70)     /*!< DMA channel 5 counter register */
#define DMA_CH5PADDR                      REG32(DMA + 0x74)     /*!< DMA channel 5 peripheral base address register */
#define DMA_CH5MADDR                      REG32(DMA + 0x78)     /*!< DMA channel 5 memory base address register */
#define DMA_CH6CTL                        REG32(DMA + 0x80)     /*!< DMA channel 6 control register */
#define DMA_CH6CNT                        REG32(DMA + 0x84)     /*!< DMA channel 6 counter register */
#define DMA_CH6PADDR                      REG32(DMA + 0x88)     /*!< DMA channel 6 peripheral base address register */
#define DMA_CH6MADDR                      REG32(DMA + 0x8C)     /*!< DMA channel 6 memory base address register */

/* bits definitions */
/* DMA_CHxCTL,x=0..6 */
#define DMA_CHXCTL_CHEN                   BIT(0)      /*!< channel x enable */
#define DMA_CHXCTL_FTFIE                 BIT(1)      /*!< enable bit for channel x transfer complete interrupt */
#define DMA_CHXCTL_HTFIE                 BIT(2)      /*!< enable bit for channel x transfer half complete interrupt */
#define DMA_CHXCTL_TAEIE                 BIT(3)      /*!< enable bit for channel x error interrupt */
#define DMA_CHXCTL_TM                     BIT(4)      /*!< direction of the data transfer on the channel */
#define DMA_CHXCTL_CMEN                   BIT(5)      /*!< circulation mode */
#define DMA_CHXCTL_PNAGA                  BIT(6)      /*!< next address generation algorithm of peripheral */
#define DMA_CHXCTL_MNAGA                  BIT(7)      /*!< next address generation algorithm of memory */
#define DMA_CHXCTL_PWIDTH                 BITS(8,9)   /*!< transfer data size of peripheral */
#define DMA_CHXCTL_MWIDTH                 BITS(10,11) /*!< transfer data size of memory */
#define DMA_CHXCTL_PRIO                   BITS(12,13) /*!< priority level of channelx */
#define DMA_CHXCTL_M2M                    BIT(14)     /*!< memory to memory mode */

/* constants definitions */
/* DMA channel select */
typedef enum 
{
    DMA_CH0 = 0, /*!< DMA Channel0 */
    DMA_CH1,     /*!< DMA Channel1 */ 
    DMA_CH2,     /*!< DMA Channel2 */ 
    DMA_CH3,     /*!< DMA Channel3 */ 
    DMA_CH4,     /*!< DMA Channel4 */ 
    DMA_CH5,     /*!< DMA Channel5 */ 
    DMA_CH6      /*!< DMA Channel6 */
} dma_channel_enum;

/* DMA initialize struct */
typedef struct
{
    uint32_t periph_addr;   /*!< peripheral base address */
    uint32_t periph_width;  /*!< transfer data size of peripheral */
    uint8_t periph_inc;     /*!< peripheral increasing mode */
    uint32_t memory_addr;   /*!< memory base address */
    uint32_t memory_width;  /*!< transfer data size of memory */
    uint8_t memory_inc;     /*!< memory increasing mode */
    uint8_t direction;      /*!< channel data transfer direction */
    uint32_t number;        /*!< channel transfer number */
    uint32_t priority;      /*!< channel priority level */
} dma_parameter_struct;

/* DMA_INTF */
#define DMA_INTF_ANYIF                    BIT(0)  /*!< global interrupt flag of channel */
#define DMA_CHXCTL_FTFIF                  BIT(1)  /*!< transfer complete flag of channel */
#define DMA_CHXCTL_HTFIF                  BIT(2)  /*!< half transfer complete flag of channel */
#define DMA_CHXCTL_TAEIF                  BIT(3)  /*!< error flag of channel */

/* DMA_INTC */
#define DMA_INTF_ANYIC                    BIT(0)  /*!< clear global interrupt flag of channel */
#define DMA_CHXCTL_FTFIF                  BIT(1)  /*!< clear transfer complete flag of channel */
#define DMA_CHXCTL_HTFIFC                 BIT(2)  /*!< clear half transfer complete flag of channel */
#define DMA_CHXCTL_TAEIFC                 BIT(3)  /*!< clear error flag of channel */

/* DMA_CHCTL base address */
#define DMA_CHXCTL_BASE                   (DMA + 0x08)          /*!< the base address of DMA channel CHXCTL register */
#define DMA_CHXCNT_BASE                   (DMA + 0x0C)          /*!< the base address of DMA channel CHXCNT register */
#define DMA_CHXPADDR_BASE                 (DMA + 0x10)          /*!< the base address of DMA channel CHXPADDR register */
#define DMA_CHXMADDR_BASE                 (DMA + 0x14)          /*!< the base address of DMA channel CHXMADDR register */
#define DMA_FLAG_ADD(flag,shift)          ((flag)<<((shift)*4)) /*!< DMA channel flag shift */

/* DMA channel shift bit */
#define DMA_CHCTL(channel)                REG32(DMA_CHXCTL_BASE + 0x14*channel)    /*!< the address of DMA channel CHXCTL register  */
#define DMA_CHCNT(channel)                REG32(DMA_CHXCNT_BASE + 0x14*channel)    /*!< the address of DMA channel CHXCNT register */
#define DMA_CHPADDR(channel)              REG32(DMA_CHXPADDR_BASE + 0x14*channel)  /*!< the address of DMA channel CHXPADDR register */
#define DMA_CHMADDR(channel)              REG32(DMA_CHXMADDR_BASE + 0x14*channel)  /*!< the address of DMA channel CHXMADDR register */

/* channel priority level */
#define CHCTL_PRIO(regval)                (BITS(12,13) & ((regval) << 12))  /*!< DMA channel priority level */
#define DMA_PRIORITY_LOW                  CHCTL_PRIO(0)                     /*!< low priority */
#define DMA_PRIORITY_MEDIUM               CHCTL_PRIO(1)                     /*!< medium priority */
#define DMA_PRIORITY_HIGH                 CHCTL_PRIO(2)                     /*!< high priority */
#define DMA_PRIORITY_ULTRA_HIGH           CHCTL_PRIO(3)                     /*!< ultra high priority */

/* transfer data size of memory */
#define CHCTL_MSIZE(regval)               (BITS(10,11) & ((regval) << 10))  /*!< transfer data size of memory */
#define DMA_MEMORY_WIDTH_8BIT              CHCTL_MSIZE(0)                   /*!< transfer data size of memory is 8-bit */
#define DMA_MEMORY_WIDTH_16BIT             CHCTL_MSIZE(1)                   /*!< transfer data size of memory is 16-bit */
#define DMA_MEMORY_WIDTH_32BIT             CHCTL_MSIZE(2)                   /*!< transfer data size of memory is 32-bit */

/* transfer data size of peripheral */
#define CHCTL_PSIZE(regval)               (BITS(8,9) & ((regval) << 8))  /*!< transfer data size of peripheral */
#define DMA_PERIPHERAL_WIDTH_8BIT          CHCTL_PSIZE(0)                /*!< transfer data size of peripheral is 8-bit */
#define DMA_PERIPHERAL_WIDTH_16BIT         CHCTL_PSIZE(1)                /*!< transfer data size of peripheral is 16-bit */
#define DMA_PERIPHERAL_WIDTH_32BIT         CHCTL_PSIZE(2)                /*!< transfer data size of peripheral is 32-bit */

/* channel data transfer direction */
#define DMA_PERIPHERA_TO_MEMORY           0x00  /*!< read from peripheral and write to memory */
#define DMA_MEMORY_TO_PERIPHERA           0x01  /*!< read from memory and write to peripheral */

/* peripheral increasing mode */
#define DMA_PERIPH_INCREASE_ENABLE        0x00  /*!< next address of peripheral is increasing address mode */
#define DMA_PERIPH_INCREASE_DISABLE       0x01  /*!< next address of peripheral is fixed address mode */

/* memory increasing mode */
#define DMA_MEMORY_INCREASE_ENABLE        0x00  /*!< next address of memory is increasing address mode */
#define DMA_MEMORY_INCREASE_DISABLE       0x01  /*!< next address of memory is fixed address mode */

/* DMA reset value */
#define DMA_CHCTL_RESET_VALUE             ((uint32_t)0)       /*!< the reset value of DMA channel CHXCTL register  */
#define DMA_CHCNT_RESET_VALUE             ((uint32_t)0)       /*!< the reset value of DMA channel CHXCNT register  */
#define DMA_CHPADDR_RESET_VALUE           ((uint32_t)0)       /*!< the reset value of DMA channel CHXPADDR register  */
#define DMA_CHMADDR_RESET_VALUE           ((uint32_t)0)       /*!< the reset value of DMA channel CHXMADDR register  */
#define DMA_CHINTF_RESET_VALUE            ((uint32_t)0x0F)    /*!< clear DMA channel CHXINTFS register  */

/* function declarations */
/* deinitialize DMA a channel registers */
void dma_deinit(dma_channel_enum channelx);
/* initialize DMA channel */
void dma_init(dma_channel_enum channelx,dma_parameter_struct init_struct);
/* enable DMA circulation mode */
void dma_circulation_enable(dma_channel_enum channelx);
/* disable DMA circulation mode */
void dma_circulation_disable(dma_channel_enum channelx);
/* enable memory to memory mode */
void dma_memory_to_memory_enable(dma_channel_enum channelx);
/* disable memory to memory mode */
void dma_memory_to_memory_disable(dma_channel_enum channelx);
/* enable DMA channel */
void dma_channel_enable(dma_channel_enum channelx);
/* disable DMA channel */
void dma_channel_disable(dma_channel_enum channelx);

/* set DMA peripheral base address */
void dma_periph_address_config(dma_channel_enum channelx,uint32_t address);
/* set DMA Memory base address */
void dma_memory_address_config(dma_channel_enum channelx,uint32_t address);
/* set the number of remaining data to be transferred by the DMA */
void dma_transfer_number_config(dma_channel_enum channelx,uint32_t number);
/* get the number of remaining data to be transferred by the DMA */
uint32_t dma_transfer_number_get(dma_channel_enum channelx);
/* configure priority level of DMA channel */
void dma_priority_config(dma_channel_enum channelx,uint32_t priority);
/* configure transfer data size of memory */
void dma_memory_width_config (dma_channel_enum channelx,uint32_t msize);
/* configure transfer data size of peripheral */
void dma_periph_width_config (dma_channel_enum channelx,uint32_t psize);
/* enable next address increasement algorithm of memory */
void dma_memory_increase_enable(dma_channel_enum channelx);
/* disable next address increasement algorithm of memory */
void dma_memory_increase_disable(dma_channel_enum channelx);
/* enable next address increasement algorithm of peripheral */
void dma_periph_increase_enable(dma_channel_enum channelx);
/* disable next address increasement algorithm of peripheral */
void dma_periph_increase_disable(dma_channel_enum channelx);
/* configure the direction of data transfer on the channel */
void dma_transfer_direction_config(dma_channel_enum channelx,uint8_t direction);

/* check DMA flag is set or not */
FlagStatus dma_interrupt_flag_get(dma_channel_enum channelx,uint32_t flag);
/* clear DMA a channel flag */
void dma_interrupt_flag_clear(dma_channel_enum channelx,uint32_t flag);
/* enable DMA interrupt */
void dma_interrupt_enable(dma_channel_enum channelx,uint32_t source);
/* disable DMA interrupt */
void dma_interrupt_disable(dma_channel_enum channelx,uint32_t source);

#endif /* GD32F1X0_DMA_H */
