/**
  ******************************************************************************
  * @file    SDIO/Read Write/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    6-Sep-2014
  * @brief   Main program 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "usart1.h"
#include "sdcard.h"

/* Exported variables --------------------------------------------------------*/
SD_CardInformation SDIOInfo;
uint32_t Scr[2] = {0, 0};

/* Private variables ---------------------------------------------------------*/
SD_Err Status = SD_OK;

uint32_t Memory_Size;
uint32_t Block_Size;
uint32_t Block_Count;

uint32_t Write_Buf[512];
uint32_t Read_Buf[512];

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void Get_Card_Information(void);
SD_Err SD_IO_Init(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    u16 i;
    u8 *p;
    SD_Err err;

    /* Configure the clock as 72Mhz */
    SystemInit();

    /* Configure the USART peripheral */
    USART1_Configuration();

    /* Configure the nested vectored interrupt controller */
    NVIC_Configuration();

    /* Init SD card */
    do
    {
        err = SD_IO_Init();
    }
    while ( err != SD_OK );

    /* Get the information of the card and print it by the USART */
    Get_Card_Information();

    /* Init write buffer */
    for(i=0;i<512;i++)
    Write_Buf[i]=i;

    printf("\r\n Card test:\n");

    /* Single block operation test */
    err = SD_WriteBlock(100*512, Write_Buf, 512);
    if (err != SD_OK)
    {
        printf(" Block write fail\n");
    }
    else 
    {
        printf(" Block write success\n");
    }
    err = SD_ReadBlock(100*512, Read_Buf, 512);
    if (err != SD_OK)
    {
        printf(" Block read fail\n");
    }
    else
    {
        printf(" Block read success\n");
        p=(u8 *)Read_Buf;
//        for(i=0;i<128;i++)
//        {
//             printf(" %3d %3d %3d %3d ",*p,*(p+1),*(p+2),*(p+3));
//             p+=4;
//             if((i+1)%4==0) printf("\r\n");
//        }
    }

    /* Lock and Unlock operation test */
    if(SD_CCC_LOCK_UNLOCK&SDIOInfo.SD_csd.CardComdClasses) 
    {
        err = SD_Lock_Unlock(SD_LOCK);
        if (err != SD_OK)
        {
            printf(" Lock failed \n");
        }
        else 
        {
            printf(" The card is locked \n");
        }

        err = SD_Erase(100*512, 101*512);
        if (err != SD_OK)
        {
            printf(" Erase failed\n");
        }
        else 
        {
            printf(" Erase success\n");
        }

        err = SD_Lock_Unlock(SD_UNLOCK);
        if (err != SD_OK)
        {
            printf(" Unlock failed \n");
            while(1);
        }
        else 
        {
            printf(" The card is unlocked \n");
        }

        err = SD_Erase(100*512, 101*512);
        if (err != SD_OK)
        {
            printf(" Erase failed\n");
        }
        else 
        {
            printf(" Erase success\n");
        }

        err = SD_ReadBlock(100*512, Read_Buf, 512);
        if (err != SD_OK)
        {
            printf(" Block read fail\n");
            while(1);
        }
        else
        {
            printf(" Block read success\n");
            p=(u8 *)Read_Buf;
//          for(i=0;i<128;i++)
//          {
//              printf(" %3d %3d %3d %3d ",*p,*(p+1),*(p+2),*(p+3));
//              p+=4;
//              if((i+1)%4==0) printf("\r\n");
//          }
        }
    }
    /* Multiple blocks operation test */
    err = SD_WriteMultiBlocks(200*512, Write_Buf, 512, 4);
    if (err != SD_OK)
    {
        printf(" Mul block write fail\n");
        while(1);
    }
    else printf(" Mul block write success\n");
    err = SD_ReadMultiBlocks(200*512, Read_Buf, 512, 4);
    if (err != SD_OK)
    {
        printf(" Mul block read fail\n");
        while(1);
    }
    else
    { 
        printf(" Mul block read success\n");
        p=(u8 *)Read_Buf;
//        for(i=0;i<512;i++)
//        {
//            printf(" %3d %3d %3d %3d ",*p,*(p+1),*(p+2),*(p+3));
//            p+=4;
//            if((i+1)%4==0) printf("\r\n");
//        }
    }

    while (1){}
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);

    /* Enable the SDIO Interrupt */
    NVIC_InitStructure.NVIC_IRQ = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
}

  /**
  * @brief  Init SD card.
  * @param  None
  * @retval SD_Error
  */
SD_Err SD_IO_Init(void)
{
    SD_Err Status = SD_OK;
    u32 cardstate;
    u32 sdstate[16];

    /* SD card Init */
    Status = SD_Init();
    
    if (Status == SD_OK)
    {
        /* Read CSD and CID register */
        Status = SD_Get_Card_Information( &SDIOInfo );
    }
    
    if (Status == SD_OK)
    {
        /* Select Card */
        Status = SD_Select_Deselect_Card( (u32) (SDIOInfo.RCA << 16) );
    }
    
    Status = SD_Send_Status(&cardstate);
    if(cardstate&0x02000000) printf("the card is locked");
    if (Status == SD_OK&&(!(cardstate&0x02000000)))
    {
        /* set bus wide */
        Status = SD_BusWide_Configure( SDIO_BUSMODE_4B );
    }
    
    if (Status == SD_OK)
    {
        /* Read SD status registers */
        Status = SD_Send_SDStatus(sdstate);
    }
    
    /* Set Device Transfer Mode */
    if (Status == SD_OK)
    { 
        Status = SD_Set_Devicemode( SD_DMA_MODE );
//      Status = SD_Set_Devicemode( SD_POLLING_MODE );
    }
    return ( Status );
}

  /**
  * @brief  Get the information of the card.
  * @param  None
  * @retval None
  */
void Get_Card_Information(void)
{
    u8 sd2,sd3,sd4,cardtype;

    printf("\n Card information:\n");
    
    sd2 = (Scr[1]&0x0f000000)>>24;
    sd3 = (Scr[1]&0x00008000)>>15;
    sd4 = (Scr[1]&0x00000400)>>10;
    if(sd2 == 2)
    {
        if(sd3)
        {
            if(sd4) 
            {
                printf(" Card version 4.xx \n");
            }
            else 
            {
                printf(" Card version 3.0x \n");
            }
        }
        else 
        {
            printf(" Card version 2.00 \n");
        }
    }
    else if(sd2 == 1) 
    {
        printf(" Card version 1.10 \n");
    }
    else if(sd2 == 0) 
    {
        printf(" Card version 1.0x \n");
    }

    cardtype = (Scr[1]&0x00700000)>>20;
    if(cardtype == 2) 
    {
        printf(" SDSC card \n");
    }
    else if(cardtype == 3) 
    {   
        printf(" SDHC card \n");
    }
    else if(cardtype == 4) 
    {
        printf(" SDXC card \n");
    }

    Block_Count = (SDIOInfo.SD_csd.DeviceSize + 1)*1024;
    Block_Size = 512;
    printf(" Device size is  %dKB \n" ,Block_Count/2);
    printf(" Block size is %dB \n" ,Block_Size);
    printf(" Block count is %d \n" ,Block_Count);

    if(SDIOInfo.SD_csd.PartBlockRead) 
    {
        printf("\r\n partial blocks for read allowed \n" );
    }
    if(SDIOInfo.SD_csd.WriteBlockPaPartial) 
    {
        printf("\r\n partial blocks for write allowed \n" );
    }

    printf("\r\n CardComdClasses is: %x \n",SDIOInfo.SD_csd.CardComdClasses);
    if((SD_CCC_STREAM_READ&SDIOInfo.SD_csd.CardComdClasses)&&(SD_CCC_STREAM_WRITE&SDIOInfo.SD_csd.CardComdClasses)) 
    {
        printf(" Stream operation supported\n");
    }
    if((SD_CCC_BLOCK_READ&SDIOInfo.SD_csd.CardComdClasses)&&(SD_CCC_BLOCK_WRITE&SDIOInfo.SD_csd.CardComdClasses)) 
    {
        printf(" Block operation supported\n");
    }
    if(SD_CCC_LOCK_UNLOCK&SDIOInfo.SD_csd.CardComdClasses) 
    {
        printf(" Lock&unlock supported\n");
    }
    if(SD_CCC_WRITE_PROT&SDIOInfo.SD_csd.CardComdClasses) 
    {
        printf(" Partial blocks operation supported\n");
    }
    if(SD_CCC_ERASE&SDIOInfo.SD_csd.CardComdClasses) 
    {
        printf(" Erase supported\n");
    }
    if(SD_CCC_APP_SPECIFIC&SDIOInfo.SD_csd.CardComdClasses) 
    {
        printf(" Application specific operation supported\n");
    }
    if(SD_CCC_IO_MODE&SDIOInfo.SD_csd.CardComdClasses) 
    {
        printf(" I/O mode supported\n");
    }
    if(SD_CCC_SWITCH&SDIOInfo.SD_csd.CardComdClasses) 
    {
        printf(" Switch function supported\n");
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
