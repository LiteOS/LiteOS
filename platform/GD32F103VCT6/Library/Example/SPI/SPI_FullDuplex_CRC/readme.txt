/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_CRC/readme.txt
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Description of the SPI_FullDuplex_CRC example.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107-EVAL board,The function of this demo is 
  exchange data between SPI1 and SPI3.SPI1 is set as master and SPI3 is slave.They
  are work in fullduplex mode and followed by CRC transmission.

  @note
    Connect SPI1 SCK  PIN(PA5) TO SPI3 SCK  PIN(PC10)
    Connect SPI1 MISO PIN(PA6) TO SPI3 MISO PIN(PC11)
    Connect SPI1 MOSI PIN(PA7) TO SPI3 MOSI PIN(PC12)

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/