/**************************************************************************//**
 * @file
 * @brief EFM32GG_STK3700 nandflash driver
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/



#include <stddef.h>

#include "em_device.h"
#include "em_dma.h"
#include "em_ebi.h"
#include "dmactrl.h"
#include "nandflash.h"

/**************************************************************************//**
* @addtogroup NandFlash
* @{ Implements a NAND flash device driver.
*    The driver has basic functions to read and write data to a nand flash
*    device.
*
*    When writing to the nand flash, an ECC is generated and written in the
*    page spare area. This ECC is checked when the page is read and the data
*    read is corrected when possible. Read and write operations can be
*    done using DMA (configurable) at initialization.
*
*    The module is easily adapted to other NAND flash devices.
** @} ************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Define how NAND flash control signals is connected to the EFM32GG. */
#define NAND_POWER_PORT         1
#define NAND_POWER_PIN          (1 << 15)
#define NAND_READY_PORT         3
#define NAND_READY_PIN          (1 << 15)
#define NAND_CE_PORT            3
#define NAND_CE_PIN             (1 << 14)
#define NAND_WP_PORT            3
#define NAND_WP_PIN             (1 << 13)
#define NAND_ALE_BIT            24
#define NAND_CLE_BIT            25

/* Values secifically for Numonyx NAND256W3A. */
#define NAND256W3A_SIGNATURE    0x7520
#define NAND256W3A_SIZE         (32 * 1024 * 1024)
#define NAND256W3A_PAGESIZE     512
#define NAND256W3A_BLOCKSIZE    (16 * 1024)

/* Generic NAND flash definitions. */
#define NAND_PAGEADDR_MASK      (NAND256W3A_PAGESIZE - 1)
#define NAND_BLOCKADDR_MASK     (NAND256W3A_BLOCKSIZE - 1)

#define NAND_RDA_CMD            0x00
#define NAND_RDB_CMD            0x01
#define NAND_RDC_CMD            0x50
#define NAND_RDSIGN_CMD         0x90
#define NAND_RDSTATUS_CMD       0x70
#define NAND_PAGEPROG1_CMD      0x80
#define NAND_PAGEPROG2_CMD      0x10
#define NAND_CPBPROG1_CMD       0x00
#define NAND_CPBPROG2_CMD       0x8A
#define NAND_CPBPROG3_CMD       0x10
#define NAND_BLOCKERASE1_CMD    0x60
#define NAND_BLOCKERASE2_CMD    0xD0
#define NAND_RST_CMD            0xFF

#define NAND_STATUS_SR7         0x80
#define NAND_STATUS_SR6         0x40
#define NAND_STATUS_SR0         0x01

/* NAND access macros. */
#define NAND_DATA8              *pNandData8
#define NAND_DATA16             *pNandData16
#define NAND_DATA32             *pNandData32
#define NAND_ADDR               *pNandAddr
#define NAND_CMD                *pNandCmd

/* DMA configuration structures. */
static const DMA_Init_TypeDef dmaInit =
{
  .hprot        = 0,
  .controlBlock = dmaControlBlock
};

static const DMA_CfgChannel_TypeDef chnCfg =
{
  .highPri   = false,     /* Default priority */
  .enableInt = false,     /* No interrupt on transfer completion */
  .select    = 0,         /* Memory-memory transfers */
  .cb        = NULL       /* No transfer completion callback */
};

static const DMA_CfgDescr_TypeDef descCfgWr =
{
  .dstInc  = dmaDataIncNone,
  .srcInc  = dmaDataInc4,
  .size    = dmaDataSize4,
  .arbRate = dmaArbitrate1,
  .hprot   = 0
};

static const DMA_CfgDescr_TypeDef descCfgRd =
{
  .dstInc  = dmaDataInc4,
  .srcInc  = dmaDataIncNone,
  .size    = dmaDataSize4,
  .arbRate = dmaArbitrate1,
  .hprot   = 0
};

/* Private variables. */
static NANDFLASH_Info_TypeDef flashInfo;
static bool                   flashInitialized = false;

static uint8_t volatile       *pNandData8;
static uint16_t volatile      *pNandData16;
static uint32_t volatile      *pNandData32;
static uint8_t volatile       *pNandAddr;
static uint8_t volatile       *pNandCmd;

/* Private function prototypes. */
__STATIC_INLINE void      chipEnable(bool enable);
static int       flashInterrogate(void);
__STATIC_INLINE void      powerEnable(bool enable);
static uint16_t  readSignature(void);
static uint8_t   readStatus(void);
static void      reset(void);
static void      dmaRead(uint8_t *dst, int count);
static void      dmaWrite(uint8_t *src, int count);
__STATIC_INLINE void      waitReady(void);
__STATIC_INLINE void      writeProtect(bool enable);

/** @endcond */

/***************************************************************************//**
 * @brief
 *   Check if an address is valid for the nand flash device.
 *
 * @param[in] address
 *   The address value to check.
 *
 * @return
 *   True if address value is valid, false otherwise.
 ******************************************************************************/
bool NANDFLASH_AddressValid(uint32_t address)
{
  if (flashInitialized)
  {
    if ((address >= flashInfo.baseAddress) &&
        (address < (flashInfo.baseAddress + flashInfo.deviceSize)))
    {
      return true;
    }
  }

  return false;
}

/***************************************************************************//**
 * @brief
 *   Copy a page within the device to a new location.
 *
 * @param[in] dstAddr
 *   The address of the destination page.
 *
 * @note
 *   The copy operation is performed inside the nand flash. No data is read
 *   from or written to the device.
 *
 * @param[in] srcAddr
 *   The address of the source page.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_CopyPage(uint32_t dstAddr, uint32_t srcAddr)
{
  int status;

  if (!flashInitialized)
  {
    EFM_ASSERT(false);
    return NANDFLASH_NOT_INITIALIZED;
  }

  dstAddr &= ~NAND_PAGEADDR_MASK;
  srcAddr &= ~NAND_PAGEADDR_MASK;

  if (!NANDFLASH_AddressValid(dstAddr) ||
      !NANDFLASH_AddressValid(srcAddr) ||
      /* Address bit 24 must be equal for source and destination page. */
      ((dstAddr & (1 << 24)) != (srcAddr & (1 << 24))))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_ADDRESS;
  }

  writeProtect(false);
  chipEnable(true);

  NAND_CMD  = NAND_CPBPROG1_CMD;
  NAND_ADDR = (uint8_t) srcAddr;
  /* Address bit 8 is not used, implicitely defined by NAND_RDA_CMD. */
  NAND_ADDR = (uint8_t)(srcAddr >> 9);
  NAND_ADDR = (uint8_t)(srcAddr >> 17);

  waitReady();

  NAND_CMD  = NAND_CPBPROG2_CMD;
  NAND_ADDR = (uint8_t) dstAddr;
  /* Address bit 8 is not used, implicitely defined by NAND_RDA_CMD. */
  NAND_ADDR = (uint8_t)(dstAddr >> 9);
  NAND_ADDR = (uint8_t)(dstAddr >> 17);
  NAND_CMD  = NAND_CPBPROG3_CMD;

  waitReady();

  status = (readStatus() & NAND_STATUS_SR0) ?
           NANDFLASH_WRITE_ERROR : NANDFLASH_STATUS_OK;

  chipEnable(false);
  writeProtect(true);

  return status;
}

/***************************************************************************//**
 * @brief
 *   Return a pointer to a @ref NANDFLASH_Info_TypeDef structure, which contain
 *   vital nand flash device information.
 *
 * @return
 *   NULL if no prior call to @ref NANDFLASH_Init() has been made.
 *   Pointer to a @ref NANDFLASH_Info_TypeDef on success.
 ******************************************************************************/
NANDFLASH_Info_TypeDef *NANDFLASH_DeviceInfo(void)
{
  if (flashInitialized)
  {
    return &flashInfo;
  }
  return NULL;
}

/***************************************************************************//**
 * @brief
 *   Check generated ECC against ECC read from device and correct data if
 *   possible.
 *
 * @param[in] generatedEcc
 *   An ECC value originating from the hardware ECC generator.
 *
 * @param[in] readEcc
 *   An ECC value read from a page spare area.
 *
 * @param[in] data
 *   Pointer to a page of data read from device.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_EccCorrect(uint32_t generatedEcc, uint32_t readEcc, uint8_t *data)
{
  /** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
  #define ECC_MASK24    0x00FFFFFF          /* 24 valid ECC parity bits. */
  #define ECC_MASK      0x00555555          /* 12 ECC parity bits.       */
  /** @endcond */

  int      count, bitNum, byteAddr;
  uint32_t mask;
  uint32_t syndrome;
  uint32_t eccP;                            /* 12 even ECC parity bits. */
  uint32_t eccPn;                           /* 12 odd ECC parity bits.  */

  syndrome = (generatedEcc ^ readEcc) & ECC_MASK24;

  if (syndrome == 0)
    return NANDFLASH_STATUS_OK;             /* No errors in data. */

  eccPn = syndrome & ECC_MASK;              /* Get twelve odd parity bits.  */
  eccP  = (syndrome >> 1) & ECC_MASK;       /* Get twelve even parity bits. */

  if ((eccPn ^ eccP) == ECC_MASK)           /* 1-bit correctable error ? */
  {
    bitNum = (eccP & 0x01) |
             ((eccP >> 1) & 0x02) |
             ((eccP >> 2) & 0x04);

    byteAddr = ((eccP >> 6) & 0x001) |
               ((eccP >> 7) & 0x002) |
               ((eccP >> 8) & 0x004) |
               ((eccP >> 9) & 0x008) |
               ((eccP >> 10) & 0x010) |
               ((eccP >> 11) & 0x020) |
               ((eccP >> 12) & 0x040) |
               ((eccP >> 13) & 0x080) |
               ((eccP >> 14) & 0x100);

    data[ byteAddr ] ^= 1 << bitNum;

    return NANDFLASH_STATUS_OK;
  }

  /* Count number of one's in the syndrome. */
  count = 0;
  mask  = 0x00800000;
  while (mask)
  {
    if (syndrome & mask)
      count++;
    mask >>= 1;
  }

  if (count == 1)                           /* Error in the ECC itself. */
    return NANDFLASH_ECC_ERROR;

  return NANDFLASH_ECC_UNCORRECTABLE;       /* Unable to correct data. */

  /** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
  #undef ECC_MASK
  #undef ECC_MASK24
  /** @endcond */
}

/***************************************************************************//**
 * @brief
 *   Erase a block in the nand flash.
 *
 * @note
 *   If this function returns @ref NANDFLASH_WRITE_ERROR, the block should
 *   be considered as "bad" and never used again.
 *
 * @param[in] address
 *   Block address.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_EraseBlock(uint32_t address)
{
  int status;

  if (!flashInitialized)
  {
    EFM_ASSERT(false);
    return NANDFLASH_NOT_INITIALIZED;
  }

  address &= ~NAND_BLOCKADDR_MASK;

  if (!NANDFLASH_AddressValid(address))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_ADDRESS;
  }

  writeProtect(false);
  chipEnable(true);

  NAND_CMD = NAND_BLOCKERASE1_CMD;
  /* Coloumn address, bit 8 is not used, implicitely defined by NAND_RDA_CMD. */
  NAND_ADDR = (uint8_t)(address >> 9);
  NAND_ADDR = (uint8_t)(address >> 17);
  NAND_CMD  = NAND_BLOCKERASE2_CMD;

  waitReady();

  status = (readStatus() & NAND_STATUS_SR0) ?
           NANDFLASH_WRITE_ERROR : NANDFLASH_STATUS_OK;

  chipEnable(false);
  writeProtect(true);

  return status;
}

/***************************************************************************//**
 * @brief
 *   Initialize the NANDFLASH module.
 *
 * @note
 *   This function should be called before any other NANDFLASH_xxx() functions
 *   are used.
 *
 * @param[in] dmaCh
 *   The DMA channel to use when transfering data to/from the device.
 *   DMA will not be used for data transfers if dmaCh is set to -1.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_Init(int dmaCh)
{
  if ((dmaCh < -1) || (dmaCh >= DMA_CHAN_COUNT))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_SETUP;
  }

  flashInfo.dmaCh = dmaCh;

  if (dmaCh >= 0)
  {
    DMA_Init((void*) &dmaInit);              /* Initialize the DMA */
    DMA_CfgChannel(dmaCh, (void*) &chnCfg);  /* Configure the DMA channel */
  }

  return flashInterrogate();
}

/***************************************************************************//**
 * @brief
 *   Mark a block as bad.
 *
 * @note
 *   This function writes a bad-block marker in the spare area of the
 *   first page in the block. The marker is written in the 6th byte of the
 *   spare area. A bad-block marker is any value different from 0xFF.
 *
 * @param[in] address
 *   Block address.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_MarkBadBlock(uint32_t address)
{
  if (!flashInitialized)
  {
    EFM_ASSERT(false);
    return NANDFLASH_NOT_INITIALIZED;
  }

  address &= ~NAND_BLOCKADDR_MASK;

  if (!NANDFLASH_AddressValid(address))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_ADDRESS;
  }

  writeProtect(false);
  chipEnable(true);

  NAND_CMD  = NAND_RDC_CMD;
  NAND_CMD  = NAND_PAGEPROG1_CMD;
  NAND_ADDR = (uint8_t) address;
  /* Address bit 8 is not used, implicitely defined by NAND_RDC_CMD. */
  NAND_ADDR = (uint8_t)(address >> 9);
  NAND_ADDR = (uint8_t)(address >> 17);

  /* Write bad block marker 0x00 to the 6th byte in the spare area */
  NAND_DATA32 = 0xFFFFFFFF;
  NAND_DATA16 = 0x00FF;
  NAND_CMD    = NAND_PAGEPROG2_CMD;

  waitReady();
  readStatus();

  chipEnable(false);
  writeProtect(true);

  return NANDFLASH_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Read a page from nand device. Ecc errors will be detected and corrected
 *   if possible. @ref NANDFLASH_Info_TypeDef.ecc will be set to the
 *   ecc generated while reading the page data.
 *   @ref NANDFLASH_Info_TypeDef.spare will be set to the content of the page
 *   spare area.
 *
 * @param[in] address
 *   Page address.
 *
 * @param[in] buffer
 *   Pointer to page data buffer.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_ReadPage(uint32_t address, uint8_t *buffer)
{
  uint32_t i, readEcc, *p;

  if (!flashInitialized)
  {
    EFM_ASSERT(false);
    return NANDFLASH_NOT_INITIALIZED;
  }

  address &= ~NAND_PAGEADDR_MASK;

  if (!NANDFLASH_AddressValid(address))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_ADDRESS;
  }

  chipEnable(true);

  NAND_CMD  = NAND_RDA_CMD;
  NAND_ADDR = (uint8_t) address;
  /* Address bit 8 is not used, implicitely defined by NAND_RDA_CMD. */
  NAND_ADDR = (uint8_t)(address >> 9);
  NAND_ADDR = (uint8_t)(address >> 17);

  waitReady();

  EBI_StartNandEccGen();

  if (flashInfo.dmaCh == -1)
  {
    p = (uint32_t*) buffer;
    for (i = 0; i < flashInfo.pageSize / 4; i++)
    {
      *p++ = NAND_DATA32;
    }
  }
  else
  {
    dmaRead(buffer, flashInfo.pageSize);
  }

  flashInfo.ecc = EBI_StopNandEccGen();

  if (flashInfo.dmaCh == -1)
  {
    p = (uint32_t*) flashInfo.spare;
    for (i = 0; i < flashInfo.spareSize / 4; i++)
    {
      *p++ = NAND_DATA32;
    }
  }
  else
  {
    dmaRead(flashInfo.spare, flashInfo.spareSize);
  }

  chipEnable(false);

  readEcc  = flashInfo.spare[ NAND_SPARE_ECC0_POS ];
  readEcc += flashInfo.spare[ NAND_SPARE_ECC1_POS ] << 8;
  readEcc += flashInfo.spare[ NAND_SPARE_ECC2_POS ] << 16;

  return NANDFLASH_EccCorrect(flashInfo.ecc, readEcc, buffer);
}

/***************************************************************************//**
 * @brief
 *   Read the spare area content of a page.
 *
 * @param[in] address
 *   Page address.
 *
 * @param[in] buffer
 *   Pointer to page spare area data buffer.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_ReadSpare(uint32_t address, uint8_t *buffer)
{
  uint32_t i, *p;

  if (!flashInitialized)
  {
    EFM_ASSERT(false);
    return NANDFLASH_NOT_INITIALIZED;
  }

  address &= ~NAND_PAGEADDR_MASK;

  if (!NANDFLASH_AddressValid(address))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_ADDRESS;
  }

  chipEnable(true);

  NAND_CMD  = NAND_RDC_CMD;
  NAND_ADDR = (uint8_t) address;
  /* Address bit 8 is not used, implicitely defined by NAND_RDC_CMD. */
  NAND_ADDR = (uint8_t)(address >> 9);
  NAND_ADDR = (uint8_t)(address >> 17);

  waitReady();

  if (flashInfo.dmaCh == -1)
  {
    p = (uint32_t*) buffer;
    for (i = 0; i < flashInfo.spareSize / 4; i++)
    {
      *p++ = NAND_DATA32;
    }
  }
  else
  {
    dmaRead(buffer, flashInfo.spareSize);
  }

  chipEnable(false);

  return NANDFLASH_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Write a page in nand device. The ECC generated while writing the page
 *   data is written in the spare area at byte positions 7, 8 and 9.
 *
 * @note
 *   If this function returns @ref NANDFLASH_WRITE_ERROR, the block should
 *   be considered as "bad" and never used again.
 *
 * @param[in] address
 *   Page address.
 *
 * @param[in] buffer
 *   Pointer to page data buffer.
 *
 * @return
 *   @ref NANDFLASH_STATUS_OK on success, an error code enumerated in
 *   @ref NANDFLASH_Status_TypeDef on failure.
 ******************************************************************************/
int NANDFLASH_WritePage(uint32_t address, uint8_t *buffer)
{
  int      status;
  uint32_t i, *p;

  if (!flashInitialized)
  {
    EFM_ASSERT(false);
    return NANDFLASH_NOT_INITIALIZED;
  }

  address &= ~NAND_PAGEADDR_MASK;

  if (!NANDFLASH_AddressValid(address))
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_ADDRESS;
  }

  writeProtect(false);
  chipEnable(true);

  NAND_CMD  = NAND_RDA_CMD;
  NAND_CMD  = NAND_PAGEPROG1_CMD;
  NAND_ADDR = (uint8_t) address;
  /* Address bit 8 is not used, implicitely defined by NAND_RDA_CMD. */
  NAND_ADDR = (uint8_t)(address >> 9);
  NAND_ADDR = (uint8_t)(address >> 17);

  /* Wait for EBI idle in case of EBI writeBuffer is enabled */
  while (EBI->STATUS & EBI_STATUS_AHBACT)
  {
  }
  EBI_StartNandEccGen();

  if (flashInfo.dmaCh == -1)
  {
    p = (uint32_t*) buffer;
    for (i = 0; i < flashInfo.pageSize / 4; i++)
    {
      NAND_DATA32 = *p++;
    }
  }
  else
  {
    dmaWrite(buffer, flashInfo.pageSize);
  }

  /* Wait for EBI idle in case of EBI writeBuffer is enabled */
  while (EBI->STATUS & EBI_STATUS_AHBACT)
  {
  }
  flashInfo.ecc = EBI_StopNandEccGen();

  /* Write ECC to spare area */
  NAND_DATA32 = 0xFFFFFFFF;
  NAND_DATA16 = 0xFFFF;
  NAND_DATA8  = flashInfo.ecc;
  NAND_DATA8  = flashInfo.ecc >> 8;
  NAND_DATA8  = flashInfo.ecc >> 16;
  NAND_CMD    = NAND_PAGEPROG2_CMD;

  waitReady();

  status = (readStatus() & NAND_STATUS_SR0) ?
           NANDFLASH_WRITE_ERROR : NANDFLASH_STATUS_OK;

  chipEnable(false);
  writeProtect(true);

  return status;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief Activate/deactivate the chip enable (CE#) signal.
 ******************************************************************************/
__STATIC_INLINE void chipEnable(bool enable)
{
  if (enable)
  {
    GPIO->P[NAND_CE_PORT].DOUTCLR = NAND_CE_PIN;
  }
  else
  {
    GPIO->P[NAND_CE_PORT].DOUTSET = NAND_CE_PIN;
  }
}

/***************************************************************************//**
 * @brief Identify nand device and fill in device information structure.
 ******************************************************************************/
static int flashInterrogate(void)
{
  flashInfo.baseAddress = EBI_BankAddress(EBI_BANK0);

  pNandData8  = (uint8_t volatile*) flashInfo.baseAddress;
  pNandData16 = (uint16_t volatile*) pNandData8;
  pNandData32 = (uint32_t volatile*) pNandData8;
  pNandAddr   = pNandData8 + (1 << NAND_ALE_BIT);
  pNandCmd    = pNandData8 + (1 << NAND_CLE_BIT);

  powerEnable(true);
  waitReady();
  chipEnable(true);
  reset();

  if (readSignature() != NAND256W3A_SIGNATURE)
  {
    EFM_ASSERT(false);
    return NANDFLASH_INVALID_DEVICE;
  }

  chipEnable(false);

  flashInfo.manufacturerCode = (uint8_t) NAND256W3A_SIGNATURE;
  flashInfo.deviceCode       = (uint8_t)(NAND256W3A_SIGNATURE >> 8);
  flashInfo.deviceSize       = NAND256W3A_SIZE;
  flashInfo.pageSize         = NAND256W3A_PAGESIZE;
  flashInfo.spareSize        = NAND256W3A_SPARESIZE;
  flashInfo.blockSize        = NAND256W3A_BLOCKSIZE;

  flashInitialized = true;

  return NANDFLASH_STATUS_OK;
}

/***************************************************************************//**
 * @brief Turn on/off nand device power.
 ******************************************************************************/
__STATIC_INLINE void powerEnable(bool enable)
{
  if (enable)
  {
    GPIO->P[NAND_POWER_PORT].DOUTSET = NAND_POWER_PIN;
  }
  else
  {
    GPIO->P[NAND_POWER_PORT].DOUTCLR = NAND_POWER_PIN;
  }
}

/***************************************************************************//**
 * @brief Read nand flash signature (manufacturer and device id).
 ******************************************************************************/
static uint16_t readSignature(void)
{
  NAND_CMD = NAND_RDSIGN_CMD;
  return NAND_DATA16;
}

/***************************************************************************//**
 * @brief Read nand flash status.
 ******************************************************************************/
static uint8_t readStatus(void)
{
  NAND_CMD = NAND_RDSTATUS_CMD;
  return NAND_DATA8;
}

/***************************************************************************//**
 * @brief Reset nand flash.
 ******************************************************************************/
static void reset(void)
{
  NAND_CMD = NAND_RST_CMD;
  waitReady();
}

/***************************************************************************//**
 * @brief Perform a DMA read to nand flash.
 ******************************************************************************/
static void dmaRead(uint8_t *dst, int count)
{
  DMA_CfgDescr(flashInfo.dmaCh, true, (void*) &descCfgRd);
  DMA_ActivateAuto(flashInfo.dmaCh, true, dst, (void*) pNandData32, (count / 4) - 1);
  while ((dmaControlBlock[flashInfo.dmaCh].CTRL & _DMA_CTRL_CYCLE_CTRL_MASK)
         != DMA_CTRL_CYCLE_CTRL_INVALID)
  {
  }
}

/***************************************************************************//**
 * @brief Perform a DMA write to nand flash.
 ******************************************************************************/
static void dmaWrite(uint8_t *src, int count)
{
  DMA_CfgDescr(flashInfo.dmaCh, true, (void*) &descCfgWr);
  DMA_ActivateAuto(flashInfo.dmaCh, true, (void*) pNandData32, src, (count / 4) - 1);
  while ((dmaControlBlock[flashInfo.dmaCh].CTRL & _DMA_CTRL_CYCLE_CTRL_MASK)
         != DMA_CTRL_CYCLE_CTRL_INVALID)
  {
  }
}

/***************************************************************************//**
 * @brief Poll Ready/Busy (R/B#) signal and wait for device ready.
 ******************************************************************************/
__STATIC_INLINE void waitReady(void)
{
  /* Wait for EBI idle in case of EBI writeBuffer is enabled */
  while (EBI->STATUS & EBI_STATUS_AHBACT)
  {
  }
  /* Wait on Ready/Busy pin to become high */
  while ((GPIO->P[NAND_READY_PORT].DIN & NAND_READY_PIN) == 0)
  {
  }
}

/***************************************************************************//**
 * @brief Activate/deactivate the write protect (WP#) signal.
 ******************************************************************************/
__STATIC_INLINE void writeProtect(bool enable)
{
  if (enable)
  {
    GPIO->P[NAND_WP_PORT].DOUTCLR = NAND_WP_PIN;
  }
  else
  {
    GPIO->P[NAND_WP_PORT].DOUTSET = NAND_WP_PIN;
  }
}

/** @endcond */
