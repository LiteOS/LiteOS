/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     at25f512a.c
 * @brief    the main function for the at25f512a driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include "at25f512a.h"
#include "stdio.h"
#include "drv_spi.h"
#include <string.h>
#include "soc.h"
#include "drv_gpio.h"
#include "pin.h"
extern void mdelay(uint32_t ms);

//#define HopeRun_printf (printf("[%s]--->[%d]",__FUNCTION__, __LINE__ ), printf) 
static int32_t spi_norflash_read_status_register(spi_handle_t handle, uint8_t *status)
{
    uint8_t command[2] = {0x05};
    uint8_t data[2] = {0};

    /* read status register*/
    //ret = csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8);

    csi_spi_transfer(handle, command, data, 2, 2);

    while (csi_spi_get_status(handle).busy);

    *status = data[1];
    return 0;
}
#if 0
static int32_t spi_norflash_write_status_register(spi_handle_t handle, uint8_t *status)
{
    uint8_t reg1 = 0xff;
    uint8_t command_enable_write = 0x06;
    uint8_t command[2] = {0x01, 0x0};
    uint8_t ret = 0;

    //ret = csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8);

    command[1] = *status;
    ret = csi_spi_send(handle, &command_enable_write, 1);

    while (csi_spi_get_status(handle).busy);

    ret = csi_spi_send(handle, command, 2, 0);

    while (csi_spi_get_status(handle).busy);

    while ((reg1 & 0x1) == 1) {
        spi_norflash_read_status_register(handle, &reg1);
    }

    return 0;
}
#endif
extern int g_flags;
static int32_t spi_norflash_write_page(spi_handle_t handle, uint32_t dst_addr, uint8_t *src_buf, uint32_t len)
{
    uint8_t reg1 = 0xff;
    uint8_t reg = 0x00;
    uint8_t command_enable_write = 0x06;
    uint8_t command_page_write[132] = {0x02, 0x0, 0x0, 0x0};
    spi_status_t status = {0};

    if (len == 0 || src_buf == NULL) {
        return -1;
    }

    csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8);

    command_page_write[1] = (dst_addr >> 16) & 0xff;
    command_page_write[2] = (dst_addr >> 8) & 0xff;
    command_page_write[3] = dst_addr & 0xff;
    memcpy(&command_page_write[4], src_buf, len);
    csi_spi_send(handle, &command_enable_write, 1);

    while (csi_spi_get_status(handle).busy);

    while ((reg & 0x2) == 0) {
        spi_norflash_read_status_register(handle, &reg);
    }

    csi_spi_send(handle, command_page_write, len + 4);

    while (1) {
        status = csi_spi_get_status(handle);

        if (status.busy == 0) {
            break;
        }
    }

    while (((reg1 & 0x1) == 1) || ((reg1 & 0x2) == 0x2)) {
        spi_norflash_read_status_register(handle, &reg1);
    }

    return len;
}

/**
  \fn          int32_t CSKY_Flash_ReadData (CSKY_DRIVER_FLASH *instance, uint32_t addr, void *data, uint32_t cnt)
  \brief       Read data from Flash.
  \param[in]   instance     Pointer to flash's instance
  \param[in]   addr  Data address.
  \param[out]  data  Pointer to a buffer storing the data read from Flash.
  \param[in]   cnt   Number of data items to read.
  \return      number of data items read or \ref execution_status
*/
int32_t at25flash_read_data(spi_handle_t handle, uint32_t addr, void *data, uint32_t cnt)
{

    uint8_t command[4] = {0x3, 0x0, 0x0, 0x0};
    spi_status_t status = {0};
    //uint8_t rxdata[260] = {0};

    if (!IS_FLASH_ADDR(addr) || !(IS_FLASH_ADDR(addr + cnt)) || data == NULL) {
        return -1;
    }

    command[1] = (addr >> 16) & 0xff;
    command[2] = (addr >> 8) & 0xff;
    command[3] = addr & 0xff;

#if 0
    ret = csi_spi_config(handle, SYSTEM_CLOCK, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 7);
    ret = csi_spi_transfer(handle, command, rxdata, 4, cnt + 4, 0);

    if (ret < 0) {
        printf("error\n");
    }

#endif

#ifndef CONFIG_CHIP_ZX297100
    gpio_pin_handle_t pgpio_pin_handle;
#endif

    csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8);
#ifndef CONFIG_CHIP_ZX297100

    drv_pinmux_config(EXAMPLE_PIN_SPI_CS, PIN_FUNC_GPIO);
    pgpio_pin_handle = csi_gpio_pin_initialize(EXAMPLE_PIN_SPI_CS, NULL);
    csi_gpio_pin_config_mode(pgpio_pin_handle, GPIO_MODE_PULLNONE);
    csi_gpio_pin_config_direction(pgpio_pin_handle, GPIO_DIRECTION_OUTPUT);
    csi_gpio_pin_write(pgpio_pin_handle, true);
    csi_gpio_pin_write(pgpio_pin_handle, false);
#endif

//    csi_spi_transfer(handle, command, data, cnt, cnt, 0);
#ifndef CONFIG_CHIP_ZX297100
    csi_spi_send(handle, command, 4);
#else
    csi_spi_config_block_mode(handle, 1);
    csi_spi_send(handle, command, 4);
    csi_spi_config_block_mode(handle, 0);
#endif

    while (csi_spi_get_status(handle).busy);

    csi_spi_receive(handle, (uint8_t *)data, cnt);


    while (1) {
        status = csi_spi_get_status(handle);

        if (status.busy == 0) {
            break;
        }
    }

#ifndef CONFIG_CHIP_ZX297100
    csi_gpio_pin_write(pgpio_pin_handle, true);
#endif
    return cnt;
}

/**
  \fn          int32_t CSKY_Flash_ReadID (CSKY_DRIVER_FLASH *instance, uint32_t *id_num)
  \brief       Read the FlashID.
  \param[in]   instance     Pointer to flash's instance
  \param[in]   id_num    Pointer to the id.
  \return      \ref execution_status
*/
int32_t at25flash_read_id(spi_handle_t handle, uint32_t *id_num)
{
    uint8_t command[3] = {0x15, 0x0, 0x0};
	uint8_t timeout = 0;
	int8_t ret=0;
    //uint8_t rxdata[3] = {0};
	
#ifndef CONFIG_CHIP_ZX297100
	printf("[info]未定义CONFIG_CHIP_ZX297100\n");
    gpio_pin_handle_t pgpio_pin_handle;
#endif
    /* read the ID*/
	
    ret=csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8);
	if(ret!=0)
	{
		printf("[error]csi_spi_config error\n");
		return -1;
	}
	printf("[success]csi_spi_config success\n");


#ifndef CONFIG_CHIP_ZX297100
    ret = drv_pinmux_config(EXAMPLE_PIN_SPI_CS, PIN_FUNC_GPIO);
	if(ret!=0){printf("[error]drv_pinmux_config error\n");return -1;}printf("[success]drv_pinmux_config success\n");
	
    pgpio_pin_handle = csi_gpio_pin_initialize(EXAMPLE_PIN_SPI_CS, NULL);
	if(pgpio_pin_handle==NULL){printf("[error]csi_gpio_pin_initialize error\n");return -1;}printf("[success]csi_gpio_pin_initialize success\n");
	
    ret = csi_gpio_pin_config_mode(pgpio_pin_handle, GPIO_MODE_PULLNONE);//设置gpio 不操作模式（不操作，上啦，下拉，开漏，推完）
	if(ret<0)printf("[error]csi_gpio_pin_config_mode DRV_ERROR_UNSUPPORTED(该驱动函数未被编写)\n");
	//printf("ret==%d\n",ret);
	
    ret=csi_gpio_pin_config_direction(pgpio_pin_handle, GPIO_DIRECTION_OUTPUT);
	if(ret<0){printf("[error]csi_gpio_pin_config_direction error\n");return -1;}printf("[success]csi_gpio_pin_config_direction success\n");
	
    ret=csi_gpio_pin_write(pgpio_pin_handle, true);
	if(ret<0){printf("[error]csi_gpio_pin_write(true) error\n");return -1;}printf("[success]csi_gpio_pin_write(true) success\n");
	
    ret=csi_gpio_pin_write(pgpio_pin_handle, false);
	if(ret<0){printf("[error]csi_gpio_pin_write(false) error\n");return -1;}printf("[success]csi_gpio_pin_write(false) success\n");
#endif

	//------------------------------------------
#ifndef CONFIG_CHIP_ZX297100
    ret=csi_spi_send(handle, command, 1);
	if(ret<0){printf("[error]csi_spi_send error\n");return -1;}printf("[success]csi_spi_send success\n");
#else
    csi_spi_config_block_mode(handle, 1);
    csi_spi_send(handle, command, 1);
    csi_spi_config_block_mode(handle, 0);
#endif
	printf("[info]csi_spi_get_status,waiting......\n");
    while (csi_spi_get_status(handle).busy)	
	{
		mdelay(1000);
		timeout++;
		if(timeout>=3)
		{
			printf("[error]timeout:SPI发送数据超时，请检查硬件连接\n");
			return -1;
		}
	}
	
    ret=csi_spi_receive(handle, (uint8_t *)id_num, 2);
	if(ret<0){printf("[error]csi_spi_receive error\n");return -1;}printf("[success]csi_spi_receive success\n");
	printf("[info]csi_spi_get_status,waiting......\n");
    while (csi_spi_get_status(handle).busy)
	{
		mdelay(1000);
		timeout++;
		if(timeout>=3)
		{
			printf("[error]timeout:SPI接受读取超时，请检查硬件连接\n");
			return -1;
		}
	}
	
	//------------------------------------------
#ifndef CONFIG_CHIP_ZX297100
    ret = csi_gpio_pin_write(pgpio_pin_handle, true);
	if(ret<0){printf("[error]csi_gpio_pin_write error\n");return -1;}printf("[success]csi_gpio_pin_write success\n");
#endif

    return 0;
}

/**
  \fn          int32_t CSKY_Flash_EraseSector (CSKY_DRIVER_FLASH *instance, uint32_t addr)
  \brief       Erase Flash Sector.
  \param[in]   instance     Pointer to flash's instance
  \param[in]   addr  Sector address
  \return      \ref execution_status
*/
int32_t at25flash_erase_sector(spi_handle_t handle, uint32_t addr)
{
    uint8_t reg1 = 0xff;
    uint8_t reg = 0x0;
    uint8_t command_enable_write = 0x06;
    uint8_t command_erase_sector[4] = {0x52, 0x0, 0x0, 0x0};

    if (!IS_FLASH_ADDR(addr)) {
        return -1;
    }

    csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8);

    command_erase_sector[1] = (addr >> 16) & 0xff;
    command_erase_sector[2] = (addr >> 8) & 0xff;
    command_erase_sector[3] = addr & 0xff;

    csi_spi_send(handle, &command_enable_write, 1);

    while (csi_spi_get_status(handle).busy);

    while ((reg & 0x2) == 0) {
        spi_norflash_read_status_register(handle, &reg);
    }

    csi_spi_send(handle, command_erase_sector, 4);

    while (csi_spi_get_status(handle).busy);

    while (((reg1 & 0x1) == 1) || ((reg1 & 0x2) == 0x2)) {
        spi_norflash_read_status_register(handle, &reg1);
    }

    return 0;
}

/**
  \fn          int32_t CSKY_Flash_EraseChip (CSKY_DRIVER_FLASH *instance)
  \brief       Erase complete Flash.
               Optional function for faster full chip erase.
  \param[in]   instance     Pointer to flash's instance
  \return      \ref execution_status
*/
int32_t at25flash_erase_chip(spi_handle_t handle)
{
    uint8_t reg1 = 0xff;
    uint8_t reg = 0x0;
    uint8_t command_enable_write = 0x06;
    uint8_t command_erase_chip = 0x62;


    csi_spi_config(handle, AT25F512D_CLK_RATE, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8);

    csi_spi_send(handle, &command_enable_write, 1);

    while (csi_spi_get_status(handle).busy);

    while ((reg & 0x2) == 0) {
        spi_norflash_read_status_register(handle, &reg);
    }

    csi_spi_send(handle, &command_erase_chip, 1);

    while (csi_spi_get_status(handle).busy);

    while (((reg1 & 0x1) == 1) || ((reg1 & 0x2) == 0x2)) {
        spi_norflash_read_status_register(handle, &reg1);
    }

    return 0;
}

/**
  \fn          int32_t CSKY_Flash_ProgramData (CSKY_DRIVER_FLASH *instance, uint32_t addr, const void *data, uint32_t cnt)
  \brief       Program data to Flash.
  \param[in]   instance     Pointer to flash's instance
  \param[in]   addr  Data address.
  \param[in]   data  Pointer to a buffer containing the data to be programmed to Flash.
  \param[in]   cnt   Number of data items to program.
  \return      number of data items programmed or \ref execution_status
*/
int32_t at25flash_program_data(spi_handle_t handle, uint32_t addr, const void *data, uint32_t cnt)
{
    uint32_t first_page_space;
    uint8_t *p = (uint8_t *)data;
    uint32_t page_num;
    uint8_t i;
    uint8_t ret = 0;
    uint32_t program_length = 0;
    uint8_t tmp[AT25F512F_PAGE_SIZE];

    if (!IS_FLASH_ADDR(addr) || !(IS_FLASH_ADDR(addr + cnt)) || data == NULL) {
        return -1;
    }

    if (cnt < AT25F512F_PAGE_SIZE) {
        first_page_space = cnt;
    } else {
        first_page_space = AT25F512F_PAGE_SIZE - (addr % (AT25F512F_PAGE_SIZE));
    }

    if (first_page_space < AT25F512F_PAGE_SIZE) {
        at25flash_read_data(handle, addr, tmp, AT25F512F_PAGE_SIZE);
        memcpy(&tmp[AT25F512F_PAGE_SIZE - first_page_space], p, first_page_space);
        spi_norflash_write_page(handle, (addr / AT25F512F_PAGE_SIZE) * AT25F512F_PAGE_SIZE, tmp, AT25F512F_PAGE_SIZE);
    } else {
        spi_norflash_write_page(handle, addr, p, AT25F512F_PAGE_SIZE);
    }

    program_length += first_page_space;

    addr += first_page_space;
    p += first_page_space;
    cnt -= first_page_space;

    page_num = cnt / AT25F512F_PAGE_SIZE;

    for (i = 0; i < page_num; i++) {
        ret = spi_norflash_write_page(handle, addr, p, AT25F512F_PAGE_SIZE);
        program_length += ret + i * AT25F512F_PAGE_SIZE;

        if (ret != AT25F512F_PAGE_SIZE) {
            return program_length;
        }

        addr += AT25F512F_PAGE_SIZE;
        p += AT25F512F_PAGE_SIZE;
        cnt -= AT25F512F_PAGE_SIZE;
    }

    if (cnt) {
        at25flash_read_data(handle, addr, tmp, AT25F512F_PAGE_SIZE);
        memcpy(tmp, p, cnt);
        spi_norflash_write_page(handle, addr, tmp, AT25F512F_PAGE_SIZE);
        program_length += cnt;
    }

    return program_length;
}

