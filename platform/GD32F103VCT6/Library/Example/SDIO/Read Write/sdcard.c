/**
  ******************************************************************************
  * @file    SDIO/Read Write/sdcard.c 
  * @author  MCU SD
  * @version V1.0
  * @date    6-Sep-2014
  * @brief   Sdcard driver.
  ******************************************************************************
  */

  /* Includes ------------------------------------------------------------------*/
#include "sdcard.h"

/* Private define --------------------------------------------------------------*/
#define NULL 0
#define SDIO_STATIC_FLAGS               ((uint32_t)0x00c007FF)
#define SDIO_FIFO_ADDR                  ((uint32_t)0x40018080)

/* Mask for errors Card Status R1 */
#define SD_R1_ADDR_OUT_OF_RANGE         ((uint32_t)0x80000000)
#define SD_R1_ADDR_MISALIGNED           ((uint32_t)0x40000000)
#define SD_R1_BLOCK_LEN_ERR             ((uint32_t)0x20000000)
#define SD_R1_ERASE_SEQ_ERR             ((uint32_t)0x10000000)
#define SD_R1_BAD_ERASE_PARAM           ((uint32_t)0x08000000)
#define SD_R1_WRITE_PROT_VIOLATION      ((uint32_t)0x04000000)
#define SD_R1_LOCK_UNLOCK_FAILED        ((uint32_t)0x01000000)
#define SD_R1_COM_CRC_FAILED            ((uint32_t)0x00800000)
#define SD_R1_ILLEGAL_CMD               ((uint32_t)0x00400000)
#define SD_R1_CARD_ECC_FAILED           ((uint32_t)0x00200000)
#define SD_R1_CC_ERROR                  ((uint32_t)0x00100000)
#define SD_R1_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00080000)
#define SD_R1_STREAM_READ_UNDERRUN      ((uint32_t)0x00040000)
#define SD_R1_STREAM_WRITE_OVERRUN      ((uint32_t)0x00020000)
#define SD_R1_CID_CSD_OVERWRIETE        ((uint32_t)0x00010000)
#define SD_R1_WP_ERASE_SKIP             ((uint32_t)0x00008000)
#define SD_R1_CARD_ECC_DISABLED         ((uint32_t)0x00004000)
#define SD_R1_ERASE_RESET               ((uint32_t)0x00002000)
#define SD_R1_AKE_SEQ_ERROR             ((uint32_t)0x00000008)
#define SD_R1_ERRORBITS                 ((uint32_t)0xFDFFE008)

/* Masks for R6 Response */
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)

/* The host 3.3V request */
#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)

/* Parameter for ACMD41(voltage validation) */
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000)

/* Check pattern for CMD8 */
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

/* The maximum times of voltage validation */
#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)

/* Bus width */
#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)

/* Card state */
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000)
#define SD_CARD_PROGRAMMING             ((uint32_t)0x00000007)
#define SD_CARD_RECEIVING               ((uint32_t)0x00000006)

/* DSM data timeout */
#define SD_DATATIMEOUT                  ((uint32_t)0x0000FFFF)

/* Masks for SCR Register */
#define SD_0TO7BITS                     ((uint32_t)0x000000FF)
#define SD_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                   ((uint32_t)0xFF000000)

#define SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO                     ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                ((uint32_t)0x00000020)

#define SD_ALLZERO                      ((uint32_t)0x00000000)

/* Clock division factor used for init the card*/
#define SDIO_INIT_CLK_DIV               ((uint8_t)0xB2)

/* Private macro -------------------------------------------------------------*/
extern uint32_t Scr[2];
extern SD_CardInformation SDIOInfo;

/* Private variables ---------------------------------------------------------*/
static uint32_t CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;
static uint32_t TransmissionMode = SD_POLLING_MODE;
static uint32_t TotalNumberOfBytes = 0, StopCondition = 0;
uint32_t *SrcBuffer, *DestBuffer;
volatile SD_Err TransferError = SD_OK;
__IO uint32_t TransferEnd = 0;
__IO uint32_t NumberOfBytes = 0;

SDIO_InitPara SDIO_InitStructure;
SDIO_CmdInitPara SDIO_CmdInitStructure;
SDIO_DataInitPara SDIO_DataInitStructure;

/* Private function prototypes -----------------------------------------------*/
static SD_Err Check_Err(void);
static SD_Err Check_Err_R1(uint8_t cmd);
static SD_Err Check_Err_R7(void);
static SD_Err Check_Err_R3(void);
static SD_Err Check_Err_R2(void);
static SD_Err Check_Err_R6(uint8_t cmd, uint16_t *prca);
static SD_Err WideBus_Enable(TypeState NewState);
static SD_Err IsCardProgramming(uint8_t *pstatus);
static SD_Err Get_SCR(uint16_t rca, uint32_t *pscr);
static uint8_t Bytes_To_PowerTwo(uint16_t NumberOfBytes);
static void GPIO_Configuration(void);
static void DMA_TxConfiguration(uint32_t *BufferSRC, uint32_t BufferSize);
static void DMA_RxConfiguration(uint32_t *BufferDST, uint32_t BufferSize);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Init SD card, make it in the standby state.
  * @param  None
  * @retval SD_Error
  */
SD_Err SD_Init(void)
{
    SD_Err Status = SD_OK;
    
    /* Configure GPIO about SDIO interface */
    GPIO_Configuration();
    
    /* Enable the SDIO and DMA2 Clock */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_SDIO | RCC_AHBPERIPH_DMA2, ENABLE);
    
    /* Deinitialize the SDIO */
    SDIO_DeInit();
    
    /* Configure the communication clock and the work voltage */
    Status = SD_PWR_ON();
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* Init the card */
    Status = SD_Init_Card();
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* Configure the SDIO peripheral */
    /* HCLK = SDIOCLK = 72 MHz, SDIO_CLK = HCLK/(2 + 1) = 24 MHz */  
    SDIO_InitStructure.SDIO_ClockDiv = 0x01; 
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_CLOCKEDGE_RISING;
    SDIO_InitStructure.SDIO_ClockBypassState = SDIO_CLOCKBYPASSSTATE_DISABLE;
    SDIO_InitStructure.SDIO_ClockPWRSave = SDIO_CLOCKPWRSAVE_DISABLE;
    SDIO_InitStructure.SDIO_BusMode = SDIO_BUSMODE_1B;
    SDIO_InitStructure.SDIO_HWFlowCtrlState = SDIO_HWFLOWCTRLSTATE_ENABLE;
    SDIO_Init(&SDIO_InitStructure);
    
    return(Status);
}


/**
  * @brief  Configure the communication clock and the work voltage.
  * @param  None
  * @retval SD_Error
  */
SD_Err SD_PWR_ON(void)
{
    SD_Err Status = SD_OK;
    uint32_t response = 0, count = 0;
    uint8_t busy =0;
    uint32_t SDType = SD_STD_CAPACITY;
    
    /* Configure the SDIO peripheral */
    SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV; /* HCLK = 72MHz, SDIOCLK = 72MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz */
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_CLOCKEDGE_RISING ;
    SDIO_InitStructure.SDIO_ClockBypassState = SDIO_CLOCKBYPASSSTATE_DISABLE;
    SDIO_InitStructure.SDIO_ClockPWRSave = SDIO_CLOCKPWRSAVE_DISABLE;
    SDIO_InitStructure.SDIO_BusMode = SDIO_BUSMODE_1B;
    SDIO_InitStructure.SDIO_HWFlowCtrlState = SDIO_HWFLOWCTRLSTATE_DISABLE;
    SDIO_Init(&SDIO_InitStructure);
    
    /* Configure Power State to ON */
    SDIO_SetPWRState(SDIO_PWRSTATE_ON);
    
    /* Enable SDIO Clock */
    SDIO_Clock_Enable(ENABLE);
    
    /* Send CMD0 to reset the card(No response required) */
    SDIO_CmdInitStructure.SDIO_CMDParameter = 0x0;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_GO_IDLE_STATE;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_NO;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);
    
    /* Check CMD errors */
    Status = Check_Err();
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* Send CMD8 to verify SD card interface operating condition(R7 response required) */
    SDIO_CmdInitStructure.SDIO_CMDParameter = SD_CHECK_PATTERN;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_IF_COND;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    if (Check_Err_R7() == SD_OK)
    {
        CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; /* SD Card 2.0 */
        SDType = SD_HIGH_CAPACITY;
    }

    /* Send CMD55, the next CMD is Application Specific Commands */
    SDIO_CmdInitStructure.SDIO_CMDParameter = 0x00;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);
    
    
    /* It is SD memory card */
    if (Check_Err_R1(SDIO_APP_CMD) == SD_OK)
    {
        /* Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
        while ((!busy) && (count < SD_MAX_VOLT_TRIAL))
        {
            /* SEND CMD55 APP_CMD with RCA as 0 */
            SDIO_CmdInitStructure.SDIO_CMDParameter = 0x00;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            /* Check CMD errors */
            Status = Check_Err_R1(SDIO_APP_CMD);
            if (Status != SD_OK)
            {
                return(Status);
            }

            /* Send ACMD41 to know the work voltage */
            SDIO_CmdInitStructure.SDIO_CMDParameter = SD_VOLTAGE_WINDOW_SD | SDType;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SD_APP_OP_COND;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            /* Check CMD errors */
            Status = Check_Err_R3();
            if (Status != SD_OK)
            {
                return(Status);
            }
            response = SDIO_GetResponse(SDIO_RESP1);
            busy = (uint8_t) (((response >> 31) == 1) ? 1 : 0);
            count++;
        }
        if (count >= SD_MAX_VOLT_TRIAL)
        {
            Status = SD_INVALID_VOLTRANGE;
            return(Status);
        }
        /* It is SDHC card */
        if (response &= SD_HIGH_CAPACITY)
        {
            CardType = SDIO_HIGH_CAPACITY_SD_CARD;
        }
    }/* else MMC Card */
  return(Status);
}

 /**
  * @brief  Close the power of SDIO.
  * @param  None
  * @retval SD_Error
  */
SD_Err SD_PowerOFF(void)
{
    SD_Err Status = SD_OK;

    /* Set Power State to OFF */
    SDIO_SetPWRState(SDIO_PWRSTATE_OFF);

    return(Status);
}

/**
  * @brief  Get CID and CSD of the card, and make it into standby state.
  * @param  None
  * @retval SD_Error
  */
SD_Err SD_Init_Card(void)
{
    SD_Err Status = SD_OK;
    uint16_t rca = 0x01;
    
    if (SDIO_GetPWRState() == SDIO_PWRSTATE_OFF)
    {
        Status = SD_REQUEST_NOT_APPLICABLE;
        return(Status);
    }
    
    /* The card is not I/O only card */
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)       
    {
        /* Send CMD2 to get the CID */
        SDIO_CmdInitStructure.SDIO_CMDParameter = 0x0;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_ALL_SEND_CID;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_LONG;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);
        
        /* Check CMD errors */
        Status = Check_Err_R2();
        if (SD_OK != Status)
        {
          return(Status);
        }

        CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
        CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
        CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
        CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
    }

    /* The card is SD memory or the I/O card has the memory portion */
    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)
     || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
    {
        /* Send CMD3 to set the RCA */
        SDIO_CmdInitStructure.SDIO_CMDParameter = 0x00;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_REL_ADDR;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);
        
        /* Check CMD errors */
        Status = Check_Err_R6(SDIO_SET_REL_ADDR, &rca);
        if (SD_OK != Status)
        {
            return(Status);
        }
    }
    
    /* The card is not I/O only card */
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
    {
        RCA = rca;

        /* Send CMD9 SEND_CSD with argument as card's RCA */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)(rca << 16);
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_CSD;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_LONG;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);
        
        /* Check CMD errors */
        Status = Check_Err_R2();
        if (SD_OK != Status)
        {
            return(Status);
        }
        
        CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
        CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
        CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
        CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
    }

    Status = SD_OK;

    return(Status);
}

 /**
  * @brief  Get the information of the card from CID and CSD.
  * @param  The pointer to struct SD_CardInfo
  * @retval SD_Error
  */
SD_Err SD_Get_Card_Information(SD_CardInformation *cardinfo)
{
    SD_Err Status = SD_OK;
    uint8_t tmp = 0;
    
    /* Card type */
    cardinfo->CardType = (uint8_t)CardType;

    /* Relative card address */
    cardinfo->RCA = (uint16_t)RCA;
    
    /* CSD Bytes */
    /* Byte 0 */
    tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
    cardinfo->SD_csd.Reserved1 = tmp & 0x03;
    
    /* Byte 1 */
    tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.TAAC = tmp;
    
    /* Byte 2 */
    tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.NSAC = tmp;
    
    /* Byte 3 */
    tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
    cardinfo->SD_csd.MaxBusClkFrec = tmp;
    
    /* Byte 4 */
    tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CardComdClasses = tmp << 4;

    /* Byte 5 */
    tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
    cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;
    
    /* Byte 6 */
    tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.Reserved2 = 0; /* Reserved */

    /* The card is SDSC card */
    if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
    {
        cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;
        
        /* Byte 7 */
        tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
        cardinfo->SD_csd.DeviceSize |= (tmp) << 2;
        
        /* Byte 8 */
        tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
        cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;
        
        cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
        cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);
        
        /* Byte 9 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
        cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
        cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
        cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
        /* Byte 10 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
        cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
        
        cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
        cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
        cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
        cardinfo->CardCapacity *= cardinfo->CardBlockSize;
    }
    /* The card is SDHC card */
    else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        /* Byte 7 */
        tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
        cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;
        
        /* Byte 8 */
        tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
        
        cardinfo->SD_csd.DeviceSize |= (tmp << 8);
        
        /* Byte 9 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
        
        cardinfo->SD_csd.DeviceSize |= (tmp);
        
        /* Byte 10 */
        tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
        
        cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
        cardinfo->CardBlockSize = 512;
    }
    

    cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;
    
    /* Byte 11 */
    tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
    cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);
    
    /* Byte 12 */
    tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
    cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;
    
    /* Byte 13 */
    tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.Reserved3 = 0;
    cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);
    
    /* Byte 14 */
    tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
    cardinfo->SD_csd.ECC = (tmp & 0x03);
    
    /* Byte 15 */
    tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
    cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SD_csd.Reserved4 = 1;
    
    /* CID Bytes */
    /* Byte 0 */
    tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ManufacturerID = tmp;
    
    /* Byte 1 */
    tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.OEM_AppliID = tmp << 8;
    
    /* Byte 2 */
    tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
    cardinfo->SD_cid.OEM_AppliID |= tmp;
    
    /* Byte 3 */
    tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
    cardinfo->SD_cid.ProdName1 = tmp << 24;
    
    /* Byte 4 */
    tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdName1 |= tmp << 16;
    
    /* Byte 5 */
    tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdName1 |= tmp << 8;
    
    /* Byte 6 */
    tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdName1 |= tmp;
    
    /* Byte 7 */
    tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
    cardinfo->SD_cid.ProdName2 = tmp;
    
    /* Byte 8 */
    tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdRev = tmp;
    
    /* Byte 9 */
    tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdSN = tmp << 24;
    
    /* Byte 10 */
    tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdSN |= tmp << 16;
    
    /* Byte 11 */
    tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
    cardinfo->SD_cid.ProdSN |= tmp << 8;
    
    /* Byte 12 */
    tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdSN |= tmp;
    
    /* Byte 13 */
    tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
    cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;
    
    /* Byte 14 */
    tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ManufactDate |= tmp;
    
    /* Byte 15 */
    tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
    cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
    cardinfo->SD_cid.Reserved2 = 1;
    
    return(Status);
}

/**
  * @brief  Configure the bus width.
  * @param  BusWide parameter
  * @retval SD_Error
  */
SD_Err SD_BusWide_Configure(uint32_t WideMode)
{
    SD_Err Status = SD_OK;

    /* MMC Card doesn't support this feature */
    if (SDIO_MULTIMEDIA_CARD == CardType)
    {
        Status = SD_UNSUPPORTED_FEATURE;
        return(Status);
    }
    else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
    {
        if (SDIO_BUSMODE_8B == WideMode)
        {
            Status = SD_UNSUPPORTED_FEATURE;
            return(Status);
        }
        else if (SDIO_BUSMODE_4B == WideMode)
        {
            Status = WideBus_Enable(ENABLE);
            if (SD_OK == Status)
            {
                /* Configure the SDIO peripheral */
                SDIO_InitStructure.SDIO_ClockDiv = 0x01; 
                SDIO_InitStructure.SDIO_ClockEdge = SDIO_CLOCKEDGE_RISING;
                SDIO_InitStructure.SDIO_ClockBypassState = SDIO_CLOCKBYPASSSTATE_DISABLE;
                SDIO_InitStructure.SDIO_ClockPWRSave = SDIO_CLOCKPWRSAVE_DISABLE;
                SDIO_InitStructure.SDIO_BusMode = SDIO_BUSMODE_4B;
                SDIO_InitStructure.SDIO_HWFlowCtrlState = SDIO_HWFLOWCTRLSTATE_DISABLE;
                SDIO_Init(&SDIO_InitStructure);
            }
        }
        else
        {
            Status = WideBus_Enable(DISABLE);

            if (SD_OK == Status)
            {
                /* Configure the SDIO peripheral */
                SDIO_InitStructure.SDIO_ClockDiv = 0x01; 
                SDIO_InitStructure.SDIO_ClockEdge = SDIO_CLOCKEDGE_RISING;
                SDIO_InitStructure.SDIO_ClockBypassState = SDIO_CLOCKBYPASSSTATE_DISABLE;
                SDIO_InitStructure.SDIO_ClockPWRSave = SDIO_CLOCKPWRSAVE_DISABLE;
                SDIO_InitStructure.SDIO_BusMode = SDIO_BUSMODE_1B;
                SDIO_InitStructure.SDIO_HWFlowCtrlState = SDIO_HWFLOWCTRLSTATE_DISABLE;
                SDIO_Init(&SDIO_InitStructure);
            }
        }
    }

    return(Status);
}

/**
  * @brief  Configure the mode of transformation.
  * @param  SD_DMA_MODE/SD_INTERRUPT_MODE/SD_POLLING_MODE
  * @retval SD_Error
  */
SD_Err SD_Set_Devicemode(uint32_t Mode)
{
    SD_Err Status = SD_OK;

    /* Select the transfer mode */
    if ((Mode == SD_DMA_MODE) || (Mode == SD_POLLING_MODE))
    {
       TransmissionMode = Mode;
    }
    else
    {
        Status = SD_INVALID_PARAMETER;
    }
    return(Status);
}

/**
  * @brief  Selects od Deselects the corresponding card.
  * @param  The RCA of the card.
  * @retval SD_Error
  */
SD_Err SD_Select_Deselect_Card(uint32_t addr)
{
    SD_Err Status = SD_OK;

    /* Send CMD7 SDIO_SEL_DESEL_CARD */
    SDIO_CmdInitStructure.SDIO_CMDParameter =  addr;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEL_DESEL_CARD;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    Status = Check_Err_R1(SDIO_SEL_DESEL_CARD);

    return(Status);
}

/**
  * @brief  Read one block from the specified address in a card.
  * @param  addr: The address will be read in the card.
  * @param  readbuff: Pointer to the buffer which will save the received data.
  * @param  BlockSize: Data block size.
  * @retval SD_Error
  */
SD_Err SD_ReadBlock(uint32_t addr, uint32_t *readbuff, uint16_t BlockSize)
{
    SD_Err Status = SD_OK;
    uint32_t count = 0, *tempbuff = readbuff;
    uint8_t power = 0;

    if (NULL == readbuff)
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    TransferError = SD_OK;
    TransferEnd = 0;
    TotalNumberOfBytes = 0;

    /* Clear all DSM configuration */
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0;
    SDIO_DataInitStructure.SDIO_DataLength = 0;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_1B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_DISABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);
    SDIO_DMA_Enable(DISABLE);

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }

    /* SDHC card the blocksize is fixed in 512B */
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        BlockSize = 512;
        addr /= 512;
    }
    if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
    {
        power = Bytes_To_PowerTwo(BlockSize);

        /* CMD16 Set Block Size for Card */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) BlockSize;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_SET_BLOCKLEN);
        if (SD_OK != Status)
        {
            return(Status);
        }
    }
    else
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    /* SDIO data transmisson config */
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) power << 4;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOSDIO;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    TotalNumberOfBytes = BlockSize;
    StopCondition = 0;
    DestBuffer = readbuff;

    /* Send CMD17 READ_SINGLE_BLOCK */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)addr;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_READ_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_READ_SINGLE_BLOCK);
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* In case of single block transfer, no need of stop transfer at all.*/
    if (TransmissionMode == SD_POLLING_MODE)
    {
    /* Polling mode */
        while (!(SDIO->STR &(SDIO_FLAG_RXORE | SDIO_FLAG_DTCRCFAIL | SDIO_FLAG_DTTMOUT | SDIO_FLAG_DTBLKEND | SDIO_FLAG_STBITE)))
        {
            if (SDIO_GetBitState(SDIO_FLAG_RXFIFOHF) != RESET)
            {
                for (count = 0; count < 8; count++)
                {
                    *(tempbuff + count) = SDIO_ReadData();
                }
                tempbuff += 8;
            }
        }

        if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
            Status = SD_DATA_TIMEOUT;
            return(Status);
        }
        else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
            Status = SD_DATA_CRC_FAIL;
            return(Status);
        }
        else if (SDIO_GetBitState(SDIO_FLAG_RXORE) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_RXORE);
            Status = SD_RX_OVERRUN;
            return(Status);
        }
        else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_STBITE);
            Status = SD_START_BIT_ERR;
            return(Status);
        }
        while (SDIO_GetBitState(SDIO_FLAG_RXDTVAL) != RESET)
        {
            *tempbuff = SDIO_ReadData();
            tempbuff++;
        }

        /* Clear all the static flags */
        SDIO_ClearBitState(SDIO_STATIC_FLAGS);
    }
    else if (TransmissionMode == SD_DMA_MODE)
    {
        /* DMA mode */
        /* Enable SDIO corresponding interrupts */
        SDIO_INTConfig(SDIO_INT_CCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND | SDIO_INT_RXORE | SDIO_INT_STBITE, ENABLE);

        /* Enable the DMA of SDIO */
        SDIO_DMA_Enable(ENABLE);

        DMA_RxConfiguration(readbuff, BlockSize);

        while (DMA_GetBitState(DMA2_FLAG_TC4) == RESET)
        {}
    }
    return(Status);
}

/**
  * @brief  Read multiple blocks from the specified address in a card.
  * @param  addr: The address will be read in the card.
  * @param  readbuff: Pointer to the buffer which will save the received data.
  * @param  BlockSize: Data block size.
  * @param  NumberOfBlocks: number of blocks will be read.
  * @retval SD_Error
  */
SD_Err SD_ReadMultiBlocks(uint32_t addr, uint32_t *readbuff, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    SD_Err Status = SD_OK;
    uint32_t count = 0, *tempbuff = readbuff;
    uint8_t power = 0;

    if (NULL == readbuff)
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    TransferError = SD_OK;
    TransferEnd = 0;
    TotalNumberOfBytes = 0;

    /* Clear all DSM configuration */
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0;
    SDIO_DataInitStructure.SDIO_DataLength = 0;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_1B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_DISABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);
    SDIO_DMA_Enable(DISABLE);

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }
        /* SDHC card the blocksize is fixed in 512B */
        if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        BlockSize = 512;
        addr /= 512;
    }

    if ((BlockSize > 0) && (BlockSize <= 2048) && (0 == (BlockSize & (BlockSize - 1))))
    {
        power = Bytes_To_PowerTwo(BlockSize);
        
        /* CMD16 Set Block Size for Card */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) BlockSize;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);
        
        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_SET_BLOCKLEN);
        if (SD_OK != Status)
        {
            return(Status);
        }
    }
    else
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    if (NumberOfBlocks > 1)
    {
        /* Common to all modes */
        if (NumberOfBlocks * BlockSize > SD_MAX_DATA_LENGTH)
        {
          Status = SD_INVALID_PARAMETER;
          return(Status);
        }

        TotalNumberOfBytes = NumberOfBlocks * BlockSize;
        StopCondition = 1;
        DestBuffer = readbuff;

        /* SDIO data transmisson config */
        SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
        SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
        SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) power << 4;
        SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOSDIO;
        SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
        SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
        SDIO_DataConfig(&SDIO_DataInitStructure);

        /* Send CMD18 READ_MULT_BLOCK */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)addr;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_READ_MULT_BLOCK;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_READ_MULT_BLOCK);
        if (Status != SD_OK)
        {
            return(Status);
        }

        if (TransmissionMode == SD_POLLING_MODE)
        {
            /* Polling mode */
            while (!(SDIO->STR &(SDIO_FLAG_RXORE | SDIO_FLAG_DTCRCFAIL | SDIO_FLAG_DTEND | SDIO_FLAG_DTTMOUT | SDIO_FLAG_STBITE)))
            {
                if (SDIO_GetBitState(SDIO_FLAG_RXFIFOHF) != RESET)
                {
                    for (count = 0; count < SD_HALFFIFO; count++)
                    {
                        *(tempbuff + count) = SDIO_ReadData();
                    }
                    tempbuff += SD_HALFFIFO;
                }
            }

            if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
                Status = SD_DATA_TIMEOUT;
                return(Status);
            }
            else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
                Status = SD_DATA_CRC_FAIL;
                return(Status);
            }
            else if (SDIO_GetBitState(SDIO_FLAG_RXORE) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_RXORE);
                Status = SD_RX_OVERRUN;
                return(Status);
            }
            else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_STBITE);
                Status = SD_START_BIT_ERR;
                return(Status);
            }
            while (SDIO_GetBitState(SDIO_FLAG_RXDTVAL) != RESET)
            {
                *tempbuff = SDIO_ReadData();
                tempbuff++;
            }

            if (SDIO_GetBitState(SDIO_FLAG_DTEND) != RESET)
            {
                /* In Case Of sdcard Send Command STOP_TRANSMISSION */
                if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType))
                {
                    /* Send CMD12 STOP_TRANSMISSION */
                    SDIO_CmdInitStructure.SDIO_CMDParameter = 0x0;
                    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_STOP_TRANSMISSION;
                    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
                    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
                    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
                    SDIO_SendCMD(&SDIO_CmdInitStructure);

                    /* Check CMD errors */
                    Status = Check_Err_R1(SDIO_STOP_TRANSMISSION);
                    if (Status != SD_OK)
                    {
                        return(Status);
                    }
                }
            }
            /* Clear all the static flags */
            SDIO_ClearBitState(SDIO_STATIC_FLAGS);
        }
        else if (TransmissionMode == SD_DMA_MODE)
        {
            /* DMA mode */
            /* Enable SDIO corresponding interrupts */
            SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND | SDIO_INT_RXORE | SDIO_INT_STBITE, ENABLE);

            /* Enable the DMA of SDIO */
            SDIO_DMA_Enable(ENABLE);
      
            DMA_RxConfiguration(readbuff, (NumberOfBlocks * BlockSize));
      
            while (DMA_GetBitState(DMA2_FLAG_TC4) == RESET)
            {}
      
            while ((TransferEnd == 0) && (TransferError == SD_OK))
            {}
      
            if (TransferError != SD_OK)
            {
                return(TransferError);
            }
        }
    }
    return(Status);
}

/**
  * @brief  Wtite one blocks to the specified address in a card.
  * @param  addr: The address will be write in the card.
  * @param  writebuff: Pointer to the buffer which will be transferred.
  * @param  BlockSize: Data block size.
  * @retval SD_Error
  */
SD_Err SD_WriteBlock(uint32_t addr, uint32_t *writebuff, uint16_t BlockSize)
{
    SD_Err Status = SD_OK;
    uint8_t  power = 0, cardstate = 0;
    uint32_t timeout = 0, bytestransferred = 0;
    uint32_t cardstatus = 0, count = 0, restwords = 0;
    uint32_t *tempbuff = writebuff;

    if (writebuff == NULL)
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    TransferError = SD_OK;
    TransferEnd = 0;
    TotalNumberOfBytes = 0;

    /* Clear all DSM configuration */
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0;
    SDIO_DataInitStructure.SDIO_DataLength = 0;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_1B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_DISABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);
    SDIO_DMA_Enable(DISABLE);

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }

    /* SDHC card the blocksize is fixed in 512B */
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        BlockSize = 512;
        addr /= 512;
    }

    /* Set the block size, both on controller and card */
    if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0)) 
    {
        power = Bytes_To_PowerTwo(BlockSize);

        /* CMD16 Set Block Size for Card */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) BlockSize;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_SET_BLOCKLEN);
        if (Status != SD_OK)
        {
            return(Status);
        }
    }
    else
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    /* Wait till card is ready for writing data */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) (RCA << 16);
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_SEND_STATUS);
    if (Status != SD_OK)
    {
        return(Status);
    }

    cardstatus = SDIO_GetResponse(SDIO_RESP1);

    timeout = 10000;

    while (((cardstatus & 0x00000100) == 0) && (timeout > 0))
    {
        /* Card is busy, continue to send CMD13 to polling the state of the card */
        timeout--;
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) (RCA << 16);
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_SEND_STATUS);
        if (Status != SD_OK)
        {
            return(Status);
        }
        cardstatus = SDIO_GetResponse(SDIO_RESP1);
    }

    /* Polling timeout */
    if (timeout == 0)
    {
        return(SD_ERROR);
    }

    /* Send CMD24 WRITE_SINGLE_BLOCK */
    SDIO_CmdInitStructure.SDIO_CMDParameter = addr;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_WRITE_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_WRITE_SINGLE_BLOCK);
    if (Status != SD_OK)
    {
        return(Status);
    }

    TotalNumberOfBytes = BlockSize;
    StopCondition = 0;
    SrcBuffer = writebuff;

    /* SDIO data transmisson config */
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) power << 4;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);


    /* In case of single data block transfer no need of stop command at all */
    if (TransmissionMode == SD_POLLING_MODE)
    {
        /* Polling mode */
        while (!(SDIO->STR & (SDIO_FLAG_DTBLKEND | SDIO_FLAG_TXURE | SDIO_FLAG_DTCRCFAIL | SDIO_FLAG_DTTMOUT | SDIO_FLAG_STBITE)))
        {
            if (SDIO_GetBitState(SDIO_FLAG_TXFIFOHE) != RESET)
            {
                if ((TotalNumberOfBytes - bytestransferred) < 32)
                {
                    restwords = ((TotalNumberOfBytes - bytestransferred) % 4 == 0) ? ((TotalNumberOfBytes - bytestransferred) / 4) : (( TotalNumberOfBytes -  bytestransferred) / 4 + 1);

                    for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
                    {
                        SDIO_WriteData(*tempbuff);
                    }
                }
                else
                {
                    for (count = 0; count < 8; count++)
                    {
                        SDIO_WriteData(*(tempbuff + count));
                    }
                    tempbuff += 8;
                    bytestransferred += 32;
                }
            }
        }
        if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
            Status = SD_DATA_TIMEOUT;
            return(Status);
        }
        else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
            Status = SD_DATA_CRC_FAIL;
            return(Status);
        }
        else if (SDIO_GetBitState(SDIO_FLAG_TXURE) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_TXURE);
            Status = SD_TX_UNDERRUN;
            return(Status);
        }
        else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
        {
            SDIO_ClearBitState(SDIO_FLAG_STBITE);
            Status = SD_START_BIT_ERR;
            return(Status);
        }
    }
    else if (TransmissionMode == SD_DMA_MODE)
    {
        /* DMA mode */
        /* Enable SDIO corresponding interrupts */
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND | SDIO_INT_TXURE | SDIO_INT_STBITE, ENABLE);

        DMA_TxConfiguration(writebuff, BlockSize);

        /* Enable the DMA of SDIO */
        SDIO_DMA_Enable(ENABLE);

        while (DMA_GetBitState(DMA2_FLAG_TC4) == RESET)
        {}

        while ((TransferEnd == 0) && (TransferError == SD_OK))
        {}

        if (TransferError != SD_OK)
        {
            return(TransferError);
        }
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    /* Wait till the card is in programming state */
    Status = IsCardProgramming(&cardstate);
    while ((Status == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
    {
        Status = IsCardProgramming(&cardstate);
    }

    return(Status);
}

/**
  * @brief  Wtite multiple blocks to the specified address in a card.
  * @param  addr: The address will be write in the card.
  * @param  writebuff: Pointer to the buffer which will be transferred.
  * @param  BlockSize: Data block size.
  * @param  NumberOfBlocks: number of blocks to be written.
  * @retval SD_Error
  */
SD_Err SD_WriteMultiBlocks(uint32_t addr, uint32_t *writebuff, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    SD_Err Status = SD_OK;
    uint8_t  power = 0, cardstate = 0;
    uint32_t bytestransferred = 0;
    uint32_t count = 0, restwords = 0;
    uint32_t *tempbuff = writebuff;

    if (writebuff == NULL)
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    TransferError = SD_OK;
    TransferEnd = 0;
    TotalNumberOfBytes = 0;

    /* Clear all DSM configuration */
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0;
    SDIO_DataInitStructure.SDIO_DataLength = 0;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_1B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_DISABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);
    SDIO_DMA_Enable(DISABLE);

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }

    /* SDHC card the blocksize is fixed in 512B */
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        BlockSize = 512;
        addr /= 512;
    }

    /* Set the block size, both on controller and card */
    if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
    {
        power = Bytes_To_PowerTwo(BlockSize);

        /* CMD16 Set Block Size for Card */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) BlockSize;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_SET_BLOCKLEN);
        if (Status != SD_OK)
        {
            return(Status);
        }
    }
    else
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    /* Wait till card is ready for data writing */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) (RCA << 16);
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_SEND_STATUS);
    if (Status != SD_OK)
    {
        return(Status);
    }

    if (NumberOfBlocks > 1)
    {
        /* Common to all modes */
        if (NumberOfBlocks * BlockSize > SD_MAX_DATA_LENGTH)
        {
            Status = SD_INVALID_PARAMETER;
            return(Status);
        }

        if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
        {
            /* CMD55 the next command is Application Specific Commands */
            SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) (RCA << 16);
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            /* Check CMD errors */
            Status = Check_Err_R1(SDIO_APP_CMD);
            if (Status != SD_OK)
            {
                return(Status);
            }

            /*ACMD23 Pre-erased to improve performance */
            SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)NumberOfBlocks;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCK_COUNT;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            /* Check CMD errors */
            Status = Check_Err_R1(SDIO_SET_BLOCK_COUNT);
            if (Status != SD_OK)
            {
                return(Status);
            }
        }

        /* Send CMD25 WRITE_MULT_BLOCK with argument data address */
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)addr;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_WRITE_MULT_BLOCK;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_WRITE_MULT_BLOCK);
        if (SD_OK != Status)
        {
            return(Status);
        }

        TotalNumberOfBytes = NumberOfBlocks * BlockSize;
        StopCondition = 1;
        SrcBuffer = writebuff;

        /* SDIO data transmisson config */
        SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
        SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
        SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) power << 4;
        SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
        SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
        SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
        SDIO_DataConfig(&SDIO_DataInitStructure);

        if (TransmissionMode == SD_POLLING_MODE)
        {
            /* Polling mode */
            while (!(SDIO->STR & (SDIO_FLAG_TXURE | SDIO_FLAG_DTCRCFAIL | SDIO_FLAG_DTEND | SDIO_FLAG_DTTMOUT | SDIO_FLAG_STBITE)))
            {
                if (SDIO_GetBitState(SDIO_FLAG_TXFIFOHE) != RESET)
                {
                    if (!((TotalNumberOfBytes - bytestransferred) < SD_HALFFIFOBYTES))
                    {
                        for (count = 0; count < SD_HALFFIFO; count++)
                        {
                            SDIO_WriteData(*(tempbuff + count));
                        }
                        tempbuff += SD_HALFFIFO;
                        bytestransferred += SD_HALFFIFOBYTES;
                    }
                    else
                    {
                        restwords = ((TotalNumberOfBytes - bytestransferred) % 4 == 0) ? ((TotalNumberOfBytes - bytestransferred) / 4) :
                                    ((TotalNumberOfBytes - bytestransferred) / 4 + 1);

                        for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
                        {
                          SDIO_WriteData(*tempbuff);
                        }
                    }
                }
            }

            if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
                Status = SD_DATA_TIMEOUT;
                return(Status);
            }
            else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
                Status = SD_DATA_CRC_FAIL;
                return(Status);
            }
            else if (SDIO_GetBitState(SDIO_FLAG_TXURE) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_TXURE);
                Status = SD_TX_UNDERRUN;
                return(Status);
            }
            else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
            {
                SDIO_ClearBitState(SDIO_FLAG_STBITE);
                Status = SD_START_BIT_ERR;
                return(Status);
            }

            if (SDIO_GetBitState(SDIO_FLAG_DTEND) != RESET)
            {
                if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
                {
                    /* Send CMD12 STOP_TRANSMISSION */
                    SDIO_CmdInitStructure.SDIO_CMDParameter = 0x0;
                    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_STOP_TRANSMISSION;
                    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
                    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
                    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
                    SDIO_SendCMD(&SDIO_CmdInitStructure);

                    /* Check CMD errors */
                    Status = Check_Err_R1(SDIO_STOP_TRANSMISSION);
                    if (Status != SD_OK)
                    {
                        return(Status);
                    }
                }
            }
        }
        else if (TransmissionMode == SD_DMA_MODE)
        {
            /* DMA mode */
            /* Enable SDIO corresponding interrupts */
            SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND | SDIO_INT_TXURE | SDIO_INT_STBITE, ENABLE);

            /* Enable the DMA of SDIO */
            SDIO_DMA_Enable(ENABLE);

            DMA_TxConfiguration(writebuff, (NumberOfBlocks * BlockSize));

            while (DMA_GetBitState(DMA2_FLAG_TC4) == RESET)
            {}

            while ((TransferEnd == 0) && (TransferError == SD_OK))
            {}

            if (TransferError != SD_OK)
            {
                return(TransferError);
            }
        }
    }
    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    /* Wait till the card is in programming state */
    Status = IsCardProgramming(&cardstate);

    while ((Status == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
    {
        Status = IsCardProgramming(&cardstate);
    }

    return(Status);
}

/**
  * @brief  Gets the current data transfer state.
  * @param  None
  * @retval SDTransferState: Data Transfer state.
  *   This value can be: 
  *         SD_NO_TRANSFER: No data transfer is acting
  *         SD_TRANSFER_IN_PROGRESS: Data transfer is acting
  */
SDTransferState SD_GetTransferState(void)
{
    if (SDIO->STR & (SDIO_FLAG_TXRUN | SDIO_FLAG_RXRUN))
    {
        return(SD_TRANSFER_IN_PROGRESS);
    }
    else
    {
        return(SD_NO_TRANSFER);
    }
}

/**
  * @brief  Aborts an ongoing data transfer.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Err SD_Stop_Transfer(void)
{
    SD_Err Status = SD_OK;

    /* Send CMD12 STOP_TRANSMISSION  */
    SDIO_CmdInitStructure.SDIO_CMDParameter = 0x0;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_STOP_TRANSMISSION;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    Status = Check_Err_R1(SDIO_STOP_TRANSMISSION);

    return(Status);
}

/**
  * @brief  Erase memory area specified for the given card.
  * @param  startaddr: the start address.
  * @param  endaddr: the end address.
  * @retval SD_Error: SD Card Error code.
  */
SD_Err SD_Erase(uint32_t startaddr, uint32_t endaddr)
{
    SD_Err Status = SD_OK;
    uint32_t delay = 0;
    __IO uint32_t maxdelay = 0;
    uint8_t cardstate = 0;

    /* Check if the card command class supports erase command */
    if ((SD_CCC_ERASE&SDIOInfo.SD_csd.CardComdClasses) == 0)
    {
        Status = SD_REQUEST_NOT_APPLICABLE;
        return(Status);
    }

    maxdelay = 72000 / ((SDIO->CLKCTLR & 0xFF) + 2);

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }

    /* SDHC card the blocksize is fixed in 512B */
    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        startaddr /= 512;
        endaddr /= 512;
    }

    /* According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
    {
        /* Send CMD32 SD_ERASE_GRP_START with argument as addr  */
        SDIO_CmdInitStructure.SDIO_CMDParameter = startaddr;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SD_ERASE_GRP_START;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        Status = Check_Err_R1(SDIO_SD_ERASE_GRP_START);
        if (Status != SD_OK)
        {
            return(Status);
        }

        /* Send CMD33 SD_ERASE_GRP_END with argument as addr  */
        SDIO_CmdInitStructure.SDIO_CMDParameter = endaddr;
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SD_ERASE_GRP_END;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        Status = Check_Err_R1(SDIO_SD_ERASE_GRP_END);
        if (Status != SD_OK)
        {
            return(Status);
        }
    }

    /* Send CMD38 ERASE */
    SDIO_CmdInitStructure.SDIO_CMDParameter = 0;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_ERASE;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    Status = Check_Err_R1(SDIO_ERASE);
    
    if (Status != SD_OK)
    {
        return(Status);
    }

    for (delay = 0; delay < maxdelay; delay++)
    {}

    /* Wait till the card is in programming state */
    Status = IsCardProgramming(&cardstate);

    while ((Status == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate)))
    {
        Status = IsCardProgramming(&cardstate);
    }

    return(Status);
}

/**
  * @brief  Returns the current card's status.
  * @param  pcardstatus: pointer to the buffer that will contain the SD card status (Card Status register).
  * @retval SD_Error: SD Card Error code.
  */
SD_Err SD_Send_Status(uint32_t *pcardstatus)
{
    SD_Err Status = SD_OK;

    if (pcardstatus == NULL)
    {
        Status = SD_INVALID_PARAMETER;
        return(Status);
    }

    /* CMD13 to polling the state of the card */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) RCA << 16;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_SEND_STATUS);
    if (Status != SD_OK)
    {
        return(Status);
    }

    *pcardstatus = SDIO_GetResponse(SDIO_RESP1);

    return(Status);
}

/**
  * @brief  Returns the current SD card's status.
  * @param  psdstatus: pointer to the buffer that will contain the SD 
  *   card status (SD Status register).
  * @retval SD_Error: SD Card Error code.
  */
SD_Err SD_Send_SDStatus(uint32_t *psdstatus)
{
    SD_Err Status = SD_OK;
    uint32_t count = 0;

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }

    /* Set block size for card if it is not equal to current block size for card. */
    SDIO_CmdInitStructure.SDIO_CMDParameter = 64;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_SET_BLOCKLEN);
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* CMD55 the next command is Application Specific Command*/
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) RCA << 16;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);
  
    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_APP_CMD);
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* SDIO data transmisson config */
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = 64;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_64B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOSDIO;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    /* Send ACMD13 SD_APP_STAUS  with argument as card's RCA.*/
    SDIO_CmdInitStructure.SDIO_CMDParameter = 0;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SD_APP_STAUS;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);
    Status = Check_Err_R1(SDIO_SD_APP_STAUS);

    if (Status != SD_OK)
    {
        return(Status);
    }

    while (!(SDIO->STR &(SDIO_FLAG_RXORE | SDIO_FLAG_DTCRCFAIL | SDIO_FLAG_DTTMOUT | SDIO_FLAG_DTBLKEND | SDIO_FLAG_STBITE)))
    {
        if (SDIO_GetBitState(SDIO_FLAG_RXFIFOHF) != RESET)
        {
            for (count = 0; count < 8; count++)
            {
                *(psdstatus + count) = SDIO_ReadData();
            }
            psdstatus += 8;
        }
    }

    if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
        Status = SD_DATA_TIMEOUT;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
        Status = SD_DATA_CRC_FAIL;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_RXORE) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_RXORE);
        Status = SD_RX_OVERRUN;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_STBITE);
        Status = SD_START_BIT_ERR;
        return(Status);
    }
    
    while (SDIO_GetBitState(SDIO_FLAG_RXDTVAL) != RESET)
    {
        *psdstatus = SDIO_ReadData();
        psdstatus++;
    }

    /* Clear all the static status flags*/
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);
    psdstatus -= 16;
    for (count = 0; count < 16; count++)
    {
        psdstatus[count] = ((psdstatus[count] & SD_0TO7BITS) << 24) |((psdstatus[count] & SD_8TO15BITS) << 8) |
                           ((psdstatus[count] & SD_16TO23BITS) >> 8) |((psdstatus[count] & SD_24TO31BITS) >> 24);
    }

    return(Status);
}

/**
  * @brief  Allows to process all the interrupts that are high.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Err SD_IRQ(void)
{
    if (SDIO_GetIntBitState(SDIO_INT_DTEND) != RESET)
    {
        /* Receive data in polling mode, get the remainning words in the FIFO*/
        if (TransmissionMode != SD_DMA_MODE)
        {
            while ((SDIO_GetBitState(SDIO_FLAG_RXDTVAL) != RESET)  &&  (NumberOfBytes < TotalNumberOfBytes))
            {
                *DestBuffer = SDIO_ReadData();
                DestBuffer++;
                NumberOfBytes += 4;
            }
        }
        /* Mul blocks operation, must send CMD12 to stop data transfer*/
        if (StopCondition == 1)
        {
            TransferError = SD_Stop_Transfer();
        }
        else
        {
            TransferError = SD_OK;
        }
        SDIO_ClearIntBitState(SDIO_INT_DTEND);

        /* Disenable all the interrupts*/
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND |
                       SDIO_INT_TXFIFOHE | SDIO_INT_RXFIFOHF | SDIO_INT_TXURE |
                       SDIO_INT_RXORE | SDIO_INT_STBITE, DISABLE);
        TransferEnd = 1;
        NumberOfBytes = 0;
        return(TransferError);
    }

    if (SDIO_GetIntBitState(SDIO_INT_DTCRCFAIL) != RESET)
    {
        SDIO_ClearIntBitState(SDIO_INT_DTCRCFAIL);
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND |
                      SDIO_INT_TXFIFOHE | SDIO_INT_RXFIFOHF | SDIO_INT_TXURE |
                      SDIO_INT_RXORE | SDIO_INT_STBITE, DISABLE);
        NumberOfBytes = 0;
        TransferError = SD_DATA_CRC_FAIL;
        return(SD_DATA_CRC_FAIL);
    }

    if (SDIO_GetIntBitState(SDIO_INT_DTTMOUT) != RESET)
    {
        SDIO_ClearIntBitState(SDIO_INT_DTTMOUT);
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND |
                      SDIO_INT_TXFIFOHE | SDIO_INT_RXFIFOHF | SDIO_INT_TXURE |
                      SDIO_INT_RXORE | SDIO_INT_STBITE, DISABLE);
        NumberOfBytes = 0;
        TransferError = SD_DATA_TIMEOUT;
        return(SD_DATA_TIMEOUT);
    }

    if (SDIO_GetIntBitState(SDIO_INT_RXORE) != RESET)
    {
        SDIO_ClearIntBitState(SDIO_INT_RXORE);
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND |
                      SDIO_INT_TXFIFOHE | SDIO_INT_RXFIFOHF | SDIO_INT_TXURE |
                      SDIO_INT_RXORE | SDIO_INT_STBITE, DISABLE);
        NumberOfBytes = 0;
        TransferError = SD_RX_OVERRUN;
        return(SD_RX_OVERRUN);
    }

    if (SDIO_GetIntBitState(SDIO_INT_TXURE) != RESET)
    {
        SDIO_ClearIntBitState(SDIO_INT_TXURE);
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND |
                      SDIO_INT_TXFIFOHE | SDIO_INT_RXFIFOHF | SDIO_INT_TXURE |
                      SDIO_INT_RXORE | SDIO_INT_STBITE, DISABLE);
        NumberOfBytes = 0;
        TransferError = SD_TX_UNDERRUN;
        return(SD_TX_UNDERRUN);
    }

    if (SDIO_GetIntBitState(SDIO_INT_STBITE) != RESET)
    {
        SDIO_ClearIntBitState(SDIO_INT_STBITE);
        SDIO_INTConfig(SDIO_INT_DTCRCFAIL | SDIO_INT_DTTMOUT | SDIO_INT_DTEND |
                      SDIO_INT_TXFIFOHE | SDIO_INT_RXFIFOHF | SDIO_INT_TXURE |
                      SDIO_INT_RXORE | SDIO_INT_STBITE, DISABLE);
        NumberOfBytes = 0;
        TransferError = SD_START_BIT_ERR;
        return(SD_START_BIT_ERR);
    }

    return(SD_OK);
}

/**
  * @brief  Checks for error conditions for CMD0.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Check_Err(void)
{
    SD_Err Status = SD_OK;
    uint32_t timeout;

    timeout = 10000;

    while ((timeout > 0) && (SDIO_GetBitState(SDIO_FLAG_CMDSENT) == RESET))
    {
        timeout--;
    }

    if (timeout == 0)
    {
        Status = SD_CMD_RSP_TIMEOUT;
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    return(Status);
}

/**
  * @brief  Checks for error conditions for R7 response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Check_Err_R7(void)
{
    SD_Err Status = SD_OK;
    uint32_t status;
    uint32_t timeout = 10000;

    status = SDIO->STR;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CMDTMOUT)) && (timeout > 0))
    {
        timeout--;
        status = SDIO->STR;
    }

    if ((timeout == 0) || (status & SDIO_FLAG_CMDTMOUT))
    {
        /* Card is not V2.0 complient or card does not support the set voltage range */
        Status = SD_CMD_RSP_TIMEOUT;
        SDIO_ClearBitState(SDIO_FLAG_CMDTMOUT);
        return(Status);
    }

    if (status & SDIO_FLAG_CMDREND)
    {
        /* Card is SD V2.0 compliant */
        Status = SD_OK;
        SDIO_ClearBitState(SDIO_FLAG_CMDREND);
        return(Status);
    }

    return(Status);
}

/**
  * @brief  Checks for error conditions for R1 response.
  * @param  cmd: The sent command index.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Check_Err_R1(uint8_t cmd)
{
    SD_Err Status = SD_OK;
    uint32_t status;
    uint32_t response_r1;

    status = SDIO->STR;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CMDTMOUT)))
    {
        status = SDIO->STR;
    }

    if (status & SDIO_FLAG_CMDTMOUT)
    {
        Status = SD_CMD_RSP_TIMEOUT;
        SDIO_ClearBitState(SDIO_FLAG_CMDTMOUT);
        return(Status);
    }
    else if (status & SDIO_FLAG_CCRCFAIL)
    {
        Status = SD_CMD_CRC_FAIL;
        SDIO_ClearBitState(SDIO_FLAG_CCRCFAIL);
        return(Status);
    }

    /* Check response received is of desired command */
    if (SDIO_GetCMDResponse() != cmd)
    {
        Status = SD_ILLEGAL_CMD;
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    /* We have received response, retrieve it for analysis  */
    response_r1 = SDIO_GetResponse(SDIO_RESP1);

    if ((response_r1 & SD_R1_ERRORBITS) == SD_ALLZERO)//no error
    {
        return(Status);
    }

    if (response_r1 & SD_R1_ADDR_OUT_OF_RANGE)
    {
        return(SD_ADDR_OUT_OF_RANGE);
    }

    if (response_r1 & SD_R1_ADDR_MISALIGNED)
    {
        return(SD_ADDR_MISALIGNED);
    }

    if (response_r1 & SD_R1_BLOCK_LEN_ERR)
    {
        return(SD_BLOCK_LEN_ERR);
    }

    if (response_r1 & SD_R1_ERASE_SEQ_ERR)
    {
        return(SD_ERASE_SEQ_ERR);
    }

    if (response_r1 & SD_R1_BAD_ERASE_PARAM)
    {
        return(SD_BAD_ERASE_PARAM);
    }

    if (response_r1 & SD_R1_WRITE_PROT_VIOLATION)
    {
        return(SD_WRITE_PROT_VIOLATION);
    }

    if (response_r1 & SD_R1_LOCK_UNLOCK_FAILED)
    {
        return(SD_LOCK_UNLOCK_FAILED);
    }

    if (response_r1 & SD_R1_COM_CRC_FAILED)
    {
        return(SD_COM_CRC_FAILED);
    }

    if (response_r1 & SD_R1_ILLEGAL_CMD)
    {
        return(SD_ILLEGAL_CMD);
    }

    if (response_r1 & SD_R1_CARD_ECC_FAILED)
    {
        return(SD_CARD_ECC_FAILED);
    }

    if (response_r1 & SD_R1_CC_ERROR)
    {
        return(SD_CC_ERROR);
    }

    if (response_r1 & SD_R1_GENERAL_UNKNOWN_ERROR)
    {
    return(SD_GENERAL_UNKNOWN_ERROR);
    }

    if (response_r1 & SD_R1_STREAM_READ_UNDERRUN)
    {
        return(SD_STREAM_READ_UNDERRUN);
    }

    if (response_r1 & SD_R1_STREAM_WRITE_OVERRUN)
    {
    return(SD_STREAM_WRITE_OVERRUN);
    }

    if (response_r1 & SD_R1_CID_CSD_OVERWRIETE)
    {
        return(SD_CID_CSD_OVERWRITE);
    }

    if (response_r1 & SD_R1_WP_ERASE_SKIP)
    {
        return(SD_WP_ERASE_SKIP);
    }

    if (response_r1 & SD_R1_CARD_ECC_DISABLED)
    {
        return(SD_CARD_ECC_DISABLED);
    }

    if (response_r1 & SD_R1_ERASE_RESET)
    {
        return(SD_ERASE_RESET);
    }

    if (response_r1 & SD_R1_AKE_SEQ_ERROR)
    {
        return(SD_AKE_SEQ_ERROR);
    }

    return(Status);
}

/**
  * @brief  Checks for error conditions for R3 (OCR).
  *   response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Check_Err_R3(void)
{
    SD_Err Status = SD_OK;
    uint32_t status;

    status = SDIO->STR;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CMDTMOUT)))
    {
        status = SDIO->STR;
    }

    if (status & SDIO_FLAG_CMDTMOUT)
    {
        Status = SD_CMD_RSP_TIMEOUT;
        SDIO_ClearBitState(SDIO_FLAG_CMDTMOUT);
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);
    return(Status);
}

/**
  * @brief  Checks for error conditions for R2 (CID or CSD) response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Check_Err_R2(void)
{
    SD_Err Status = SD_OK;
    uint32_t status;

    status = SDIO->STR;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CMDTMOUT)))
    {
        status = SDIO->STR;
    }

    if (status & SDIO_FLAG_CMDTMOUT)
    {
        Status = SD_CMD_RSP_TIMEOUT;
        SDIO_ClearBitState(SDIO_FLAG_CMDTMOUT);
        return(Status);
    }
    else if (status & SDIO_FLAG_CCRCFAIL)
    {
        Status = SD_CMD_CRC_FAIL;
        SDIO_ClearBitState(SDIO_FLAG_CCRCFAIL);
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    return(Status);
}

/**
  * @brief  Checks for error conditions for R6 (RCA) response.
  * @param  cmd: The sent command index.
  * @param  prca: pointer to the variable that will contain the SD
  *   card relative address RCA. 
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Check_Err_R6(uint8_t cmd, uint16_t *prca)
{
    SD_Err Status = SD_OK;
    uint32_t status;
    uint32_t response_r1;

    status = SDIO->STR;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CMDTMOUT)))
    {
        status = SDIO->STR;
    }

    if (status & SDIO_FLAG_CMDTMOUT)
    {
        Status = SD_CMD_RSP_TIMEOUT;
        SDIO_ClearBitState(SDIO_FLAG_CMDTMOUT);
        return(Status);
    }
    else if (status & SDIO_FLAG_CCRCFAIL)
    {
        Status = SD_CMD_CRC_FAIL;
        SDIO_ClearBitState(SDIO_FLAG_CCRCFAIL);
        return(Status);
    }

    /* Check response received is of desired command */
    if (SDIO_GetCMDResponse() != cmd)
    {
        Status = SD_ILLEGAL_CMD;
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    /* We have received response, retrieve it.  */
    response_r1 = SDIO_GetResponse(SDIO_RESP1);

    if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
    {
        *prca = (uint16_t) (response_r1 >> 16);
        return(Status);
    }

    if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
    {
        return(SD_GENERAL_UNKNOWN_ERROR);
    }
    
    if (response_r1 & SD_R6_ILLEGAL_CMD)
    {
        return(SD_ILLEGAL_CMD);
    }
    
    if (response_r1 & SD_R6_COM_CRC_FAILED)
    {
        return(SD_COM_CRC_FAILED);
    }

    return(Status);
}

/**
  * @brief  Enables or disables the SDIO wide bus mode.
  * @param  NewState: new state of the SDIO wide bus mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err WideBus_Enable(TypeState NewState)
{
    SD_Err Status = SD_OK;

    /* Check whether the card is locked */
    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
    {
        Status = SD_LOCK_UNLOCK_FAILED;
        return(Status);
    }

    /* Get SCR Register */
    Status = Get_SCR(RCA, Scr);

    if (Status != SD_OK)
    {
        return(Status);
    }

    /* If wide bus operation to be enabled */
    if (NewState == ENABLE)
    {
        /* If requested card supports wide bus operation */
        if ((Scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
        {
            /* Send CMD55 APP_CMD with argument as card's RCA.*/
            SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) RCA << 16;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            Status = Check_Err_R1(SDIO_APP_CMD);

            if (Status != SD_OK)
            {
                return(Status);
            }

            /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
            SDIO_CmdInitStructure.SDIO_CMDParameter = 0x2;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_SD_SET_BUSWIDTH;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);
            
            Status = Check_Err_R1(SDIO_APP_SD_SET_BUSWIDTH);
            
            if (Status != SD_OK)
            {
                return(Status);
            }
            return(Status);
        }
        else
        {
            Status = SD_REQUEST_NOT_APPLICABLE;
            return(Status);
        }
    }   /* If wide bus operation to be disabled */
    else
    {
        /* If requested card supports 1 bit mode operation */
        if ((Scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
        {
            /* Send CMD55 APP_CMD with argument as card's RCA.*/
            SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) RCA << 16;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            /* Check CMD errors */
            Status = Check_Err_R1(SDIO_APP_CMD);
            if (Status != SD_OK)
            {
                return(Status);
            }

            /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
            SDIO_CmdInitStructure.SDIO_CMDParameter = 0x00;
            SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_SD_SET_BUSWIDTH;
            SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
            SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
            SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
            SDIO_SendCMD(&SDIO_CmdInitStructure);

            /* Check CMD errors */
            Status = Check_Err_R1(SDIO_APP_SD_SET_BUSWIDTH);
            if (Status != SD_OK)
            {
                return(Status);
            }
            return(Status);
        }
        else
        {
            Status = SD_REQUEST_NOT_APPLICABLE;
            return(Status);
        }
    }
}

/**
  * @brief  Checks if the SD card is in programming state.
  * @param  pstatus: pointer to the variable that will contain the SD
  *   card state.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err IsCardProgramming(uint8_t *pstatus)
{
    SD_Err Status = SD_OK;
    __IO uint32_t respR1 = 0, status = 0;

    /* CMD13 to polling the state of the card */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) RCA << 16;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    status = SDIO->STR;
    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CMDTMOUT)))
    {
        status = SDIO->STR;
    }

    if (status & SDIO_FLAG_CMDTMOUT)
    {
        Status = SD_CMD_RSP_TIMEOUT;
        SDIO_ClearBitState(SDIO_FLAG_CMDTMOUT);
        return(Status);
    }
    else if (status & SDIO_FLAG_CCRCFAIL)
    {
        Status = SD_CMD_CRC_FAIL;
        SDIO_ClearBitState(SDIO_FLAG_CCRCFAIL);
        return(Status);
    }

    status = (uint32_t)SDIO_GetCMDResponse();

    /* Check response received is of desired command */
    if (status != SDIO_SEND_STATUS)
    {
        Status = SD_ILLEGAL_CMD;
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);


    /* We have received response, retrieve it for analysis  */
    respR1 = SDIO_GetResponse(SDIO_RESP1);

    /* Find out card status */
    *pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);

    if ((respR1 & SD_R1_ERRORBITS) == SD_ALLZERO)
    {
        return(Status);
    }

    if (respR1 & SD_R1_ADDR_OUT_OF_RANGE)
    {
        return(SD_ADDR_OUT_OF_RANGE);
    }

    if (respR1 & SD_R1_ADDR_MISALIGNED)
    {
        return(SD_ADDR_MISALIGNED);
    }

    if (respR1 & SD_R1_BLOCK_LEN_ERR)
    {
        return(SD_BLOCK_LEN_ERR);
    }

    if (respR1 & SD_R1_ERASE_SEQ_ERR)
    {
        return(SD_ERASE_SEQ_ERR);
    }

    if (respR1 & SD_R1_BAD_ERASE_PARAM)
    {
        return(SD_BAD_ERASE_PARAM);
    }

    if (respR1 & SD_R1_WRITE_PROT_VIOLATION)
    {
        return(SD_WRITE_PROT_VIOLATION);
    }

    if (respR1 & SD_R1_LOCK_UNLOCK_FAILED)
    {
        return(SD_LOCK_UNLOCK_FAILED);
    }

    if (respR1 & SD_R1_COM_CRC_FAILED)
    {
        return(SD_COM_CRC_FAILED);
    }

    if (respR1 & SD_R1_ILLEGAL_CMD)
    {
        return(SD_ILLEGAL_CMD);
    }

    if (respR1 & SD_R1_CARD_ECC_FAILED)
    {
        return(SD_CARD_ECC_FAILED);
    }

    if (respR1 & SD_R1_CC_ERROR)
    {
        return(SD_CC_ERROR);
    }

    if (respR1 & SD_R1_GENERAL_UNKNOWN_ERROR)
    {
        return(SD_GENERAL_UNKNOWN_ERROR);
    }

    if (respR1 & SD_R1_STREAM_READ_UNDERRUN)
    {
        return(SD_STREAM_READ_UNDERRUN);
    }

    if (respR1 & SD_R1_STREAM_WRITE_OVERRUN)
    {
        return(SD_STREAM_WRITE_OVERRUN);
    }

    if (respR1 & SD_R1_CID_CSD_OVERWRIETE)
    {
        return(SD_CID_CSD_OVERWRITE);
    }

    if (respR1 & SD_R1_WP_ERASE_SKIP)
    {
        return(SD_WP_ERASE_SKIP);
    }

    if (respR1 & SD_R1_CARD_ECC_DISABLED)
    {
        return(SD_CARD_ECC_DISABLED);
    }

    if (respR1 & SD_R1_ERASE_RESET)
    {
        return(SD_ERASE_RESET);
    }

    if (respR1 & SD_R1_AKE_SEQ_ERROR)
    {
        return(SD_AKE_SEQ_ERROR);
    }

    return(Status);
}

/**
  * @brief  Find the SD card SCR register value.
  * @param  rca: selected card address.
  * @param  pscr: pointer to the buffer that will contain the SCR value.
  * @retval SD_Error: SD Card Error code.
  */
static SD_Err Get_SCR(uint16_t rca, uint32_t *pscr)
{
    uint32_t index = 0;
    SD_Err Status = SD_OK;
    uint32_t tempscr[2] = {0, 0};

    /* Set Block Size To 8 Bytes */
    /* Send CMD8 SDIO_SET_BLOCKLEN to set block size */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)8;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    Status = Check_Err_R1(SDIO_SET_BLOCKLEN);

    if (Status != SD_OK)
    {
        return(Status);
    }

    /* CMD55 APP_CMD the next command is Application Specific Command */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) RCA << 16;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_APP_CMD;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_APP_CMD);
    if (Status != SD_OK)
    {
        return(Status);
    }

    /* SDIO data transmisson config */
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = 8;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_8B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOSDIO;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    /* Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
    SDIO_CmdInitStructure.SDIO_CMDParameter = 0x0;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SD_APP_SEND_SCR;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_SD_APP_SEND_SCR);
    if (Status != SD_OK)
    {
        return(Status);
    }

    while (!(SDIO->STR & (SDIO_FLAG_RXORE | SDIO_FLAG_DTCRCFAIL | SDIO_FLAG_DTTMOUT | SDIO_FLAG_DTBLKEND | SDIO_FLAG_STBITE)))
    {
        if (SDIO_GetBitState(SDIO_FLAG_RXDTVAL) != RESET)
        {
            *(tempscr + index) = SDIO_ReadData();
            index++;
        }
    }

    if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
        Status = SD_DATA_TIMEOUT;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
        Status = SD_DATA_CRC_FAIL;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_RXORE) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_RXORE);
        Status = SD_RX_OVERRUN;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_STBITE);
        Status = SD_START_BIT_ERR;
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    *(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

    *(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

    return(Status);
}

/**
  * @brief  Converts the number of bytes in power of two and returns the
  *   power.
  * @param  NumberOfBytes: number of bytes.
  * @retval None
  */
static uint8_t Bytes_To_PowerTwo(uint16_t NumberOfBytes)
{
    uint8_t count = 0;

    while (NumberOfBytes != 1)
    {
        NumberOfBytes >>= 1;
        count++;
    }
    return(count);
}

/**
  * @brief  Configures the SDIO Corresponding GPIO Ports
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
    GPIO_InitPara  GPIO_InitStructure;

    /* Enable GPIOC and GPIOD clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOD, ENABLE);

    /* Configure relative ports(PC8, PC9, PC10, PC11, PC12: D0, D1, D2, D3, CLK pin) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PD2(CMD line) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO transfer request.
  * @param  BufferSRC: pointer to the source buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
static void DMA_TxConfiguration(uint32_t *BufferSRC, uint32_t BufferSize)
{
    DMA_InitPara DMA_InitStructure;

    DMA_ClearBitState(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

    /* DMA2 Channel4 disable */
    DMA_Enable(DMA2_CHANNEL4, DISABLE);

    /* DMA2 Channel4 Config */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_WORD;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_WORD;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA2_CHANNEL4, &DMA_InitStructure);

    /* DMA2 Channel4 enable */
    DMA_Enable(DMA2_CHANNEL4, ENABLE);

}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO reveive request.
  * @param  BufferDST: pointer to the destination buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
static void DMA_RxConfiguration(uint32_t *BufferDST, uint32_t BufferSize)
{
    DMA_InitPara DMA_InitStructure;

    DMA_ClearBitState(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

    /* DMA2 Channel4 disable */
    DMA_Enable(DMA2_CHANNEL4, DISABLE);

    /* DMA2 Channel4 Config */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_WORD;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_WORD;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA2_CHANNEL4, &DMA_InitStructure);

    /* DMA2 Channel4 enable */
    DMA_Enable(DMA2_CHANNEL4, ENABLE);
}

/**
  * @brief  Lock/Unlock the card.
  * @param  lockstate: lock or unlock
  * @retval None
  */
SD_Err SD_Lock_Unlock(uint8_t lockstate)
{
    SD_Err Status = SD_OK;
    u8 cardstate = 0;
    u32 pwd1,pwd2,cardstatus,timeout;

    /* Check if the card command class supports Lock/Unlock command */
    if ((SD_CCC_LOCK_UNLOCK&SDIOInfo.SD_csd.CardComdClasses) == 0)
    {
        Status = SD_REQUEST_NOT_APPLICABLE;
        return(Status);
    }
    /* PWD pattern */
    pwd1 = (0x01020600|lockstate);
    pwd2 = 0x03040506;

    /* Clear all DSM configuration */
    SDIO_DataInitStructure.SDIO_DataTimeOut = 0;
    SDIO_DataInitStructure.SDIO_DataLength = 0;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_1B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_DISABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);
    SDIO_DMA_Enable(DISABLE);

    /* CMD16 Set Block Size for Card */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) 8;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_SET_BLOCKLEN);
    if (Status != SD_OK)
    {
        return(Status);
    }

    cardstatus = SDIO_GetResponse(SDIO_RESP1);

    /* Wait till card is ready for data writing */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) (RCA << 16);
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    Status = Check_Err_R1(SDIO_SEND_STATUS);

    if (Status != SD_OK)
    {
        return(Status);
    }

    cardstatus = SDIO_GetResponse(SDIO_RESP1);

    timeout = 10000;

    while (((cardstatus & 0x00000100) == 0) && (timeout > 0))
    {
        /* CMD13 to polling the state of the card */
        timeout--;
        SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t) (RCA << 16);
        SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_SEND_STATUS;
        SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
        SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
        SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
        SDIO_SendCMD(&SDIO_CmdInitStructure);

        /* Check CMD errors */
        Status = Check_Err_R1(SDIO_SEND_STATUS);
        if (Status != SD_OK)
        {
            return(Status);
        }

        cardstatus = SDIO_GetResponse(SDIO_RESP1);
    }

    if (timeout == 0)
    {
        return(SD_ERROR);
    }

     /* CMD42 LOCK_UNLOCK */
    SDIO_CmdInitStructure.SDIO_CMDParameter = (uint32_t)0x00000000;
    SDIO_CmdInitStructure.SDIO_CMDIndex = SDIO_LOCK_UNLOCK;
    SDIO_CmdInitStructure.SDIO_ResponseType = SDIO_RESPONSETYPE_SHORT;
    SDIO_CmdInitStructure.SDIO_WaitINTState = SDIO_WAITINTSTATE_NO;
    SDIO_CmdInitStructure.SDIO_CSMState = SDIO_CSMSTATE_ENABLE;
    SDIO_SendCMD(&SDIO_CmdInitStructure);

    /* Check CMD errors */
    Status = Check_Err_R1(SDIO_LOCK_UNLOCK);
    if (Status != SD_OK)
    {
        return(Status);
    }

    cardstatus = SDIO_GetResponse(SDIO_RESP1);

    /* SDIO data transmisson config */
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = 8;
    SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DATABLOCKSIZE_8B;
    SDIO_DataInitStructure.SDIO_TransDirection = SDIO_TRANSDIRECTION_TOCARD;
    SDIO_DataInitStructure.SDIO_TransMode = SDIO_TRANSMODE_BLOCK;
    SDIO_DataInitStructure.SDIO_DSMState = SDIO_DSMSTATE_ENABLE;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    /* Write PWD pattern */
    SDIO_WriteData(pwd1);
    SDIO_WriteData(pwd2);

    if (SDIO_GetBitState(SDIO_FLAG_DTTMOUT) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_DTTMOUT);
        Status = SD_DATA_TIMEOUT;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_DTCRCFAIL) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_DTCRCFAIL);
        Status = SD_DATA_CRC_FAIL;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_TXURE) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_TXURE);
        Status = SD_TX_UNDERRUN;
        return(Status);
    }
    else if (SDIO_GetBitState(SDIO_FLAG_STBITE) != RESET)
    {
        SDIO_ClearBitState(SDIO_FLAG_STBITE);
        Status = SD_START_BIT_ERR;
        return(Status);
    }

    /* Clear all the static flags */
    SDIO_ClearBitState(SDIO_STATIC_FLAGS);

    /* Wait till the card is in programming state */
    Status = IsCardProgramming(&cardstate);
    while ((Status == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
    {
        Status = IsCardProgramming(&cardstate);
    }

    return(Status);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
