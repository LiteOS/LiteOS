#include "los_bsp_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <samr21.h>


int _sercom_pm_init(void) {
    PM->APBCMASK.bit.SERCOM0_ |= 1;
    
    return 0;
}

int _system_gclk_chan_disable(
        const uint8_t channel)
{
    /* Select the requested generator channel */
    *((uint8_t*)&GCLK->CLKCTRL.reg) = channel;

    /* Sanity check WRTLOCK */
    if(GCLK->CLKCTRL.bit.WRTLOCK == 1) return -1;

    /* Switch to known-working source so that the channel can be disabled */
    uint32_t prev_gen_id = GCLK->CLKCTRL.bit.GEN;
    GCLK->CLKCTRL.bit.GEN = 0;

    /* Disable the generic clock */
    GCLK->CLKCTRL.reg &= ~GCLK_CLKCTRL_CLKEN;
    while (GCLK->CLKCTRL.reg & GCLK_CLKCTRL_CLKEN) {
        /* Wait for clock to become disabled */
    }

    /* Restore previous configured clock generator */
    GCLK->CLKCTRL.bit.GEN = prev_gen_id;
    
    return 1;
}

void _system_gclk_chan_set_config(const uint8_t channel)
{
    /* Cache the new config to reduce sync requirements */
    uint32_t new_clkctrl_config = (channel << GCLK_CLKCTRL_ID_Pos);

    /* Select the desired generic clock generator */
    //new_clkctrl_config |= config->source_generator << GCLK_CLKCTRL_GEN_Pos;

    /* Disable generic clock channel */
    _system_gclk_chan_disable(channel);

    /* Write the new configuration */
    GCLK->CLKCTRL.reg = new_clkctrl_config;
}

void _system_gclk_chan_enable(const uint8_t channel)
{
    /* Select the requested generator channel */
    *((uint8_t*)&GCLK->CLKCTRL.reg) = channel;

    /* Enable the generic clock */
    GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN;
}

int _sercom_gclk_init(void) {
    /* step 1: set */
    _system_gclk_chan_set_config(0x14);
    
    /* step 2: Enable the generic clock */
    _system_gclk_chan_enable(0x14);
    
    /* step 3: set */
    _system_gclk_chan_set_config(0x13);
    
    /* step 4: Enable the generic clock */
    _system_gclk_chan_enable(0x13);
    
    return 0;
}

static inline unsigned char usart_is_syncing(void) {
    return (SERCOM0->USART.SYNCBUSY.reg);
}

static inline void _usart_wait_for_sync(void) {
    while (usart_is_syncing()) {
        /* Wait until the synchronization is complete */
    }
}

int _sercom_usart_init(void) {
    _usart_wait_for_sync();
    SERCOM0->USART.BAUD.reg = 0xFF2E;//0xFB15;//0xD8AD; //65536*(1-16*9600/48000000)
    
    _usart_wait_for_sync();
    SERCOM0->USART.CTRLB.reg = 0x00030000;
    
    _usart_wait_for_sync();
    SERCOM0->USART.CTRLA.reg = 0x40100084; 
    
    return 0;
}

int _sercom_pin_init(void) {
    PORT->Group[0].DIRCLR.reg |= 0x00000010;
    PORT->Group[0].WRCONFIG.reg |= 0x53030010;
    
    PORT->Group[0].DIRCLR.reg |= 0x00000020;
    PORT->Group[0].WRCONFIG.reg |= 0x53030020;
    
    _usart_wait_for_sync();
    SERCOM0->USART.CTRLA.bit.ENABLE = 1;

    return 0;
}

int sercom_uart_init(void) {
    if (REG_SERCOM0_USART_CTRLA & 0x00000001) {
        /* The module is busy resetting itself */
        return -1;
    }

    if (REG_SERCOM0_USART_CTRLA & SERCOM_USART_CTRLA_ENABLE) {
        /* Check the module is enabled */
        return -2;
    }
    
    /* step 1: SERCOM0 APB CLOCK ENABLE */
    _sercom_pm_init();
    
    /* step 2: GCLK Setting */
    _sercom_gclk_init();
    
    /* step 3: USART SETTING */
    _sercom_usart_init();
    
    /* step 4: PORT SETTING */
    _sercom_pin_init();
    
    return 0;
}

int usart_write_wait(const unsigned short tx_data) {
    /* Check if USART is ready for new data */
    if (!(SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE)) {
        /* Return error code */
        return -1;
    }

    /* Wait until synchronization is complete */
    _usart_wait_for_sync();

    /* Write data to USART module */
    SERCOM0->USART.DATA.reg = tx_data;

    while (!(SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_TXC)) {
        /* Wait until data is sent */
    }

    return 0;
}

int usart_read_wait(unsigned short *const rx_data) {
    uint8_t error_code;
    
    /* Check if USART has new data */
    if (!(SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC)) {
        /* Return error code */
        return -1;
    }

    /* Wait until synchronization is complete */
    _usart_wait_for_sync();

    /* Read out the status code and mask away all but the 3 LSBs*/
    error_code = (uint8_t)(SERCOM0->USART.STATUS.reg & SERCOM_USART_STATUS_MASK);

    /* Check if an error has occurred during the receiving */
    if (error_code) {
        /* Check which error occurred */
        if (error_code & SERCOM_USART_STATUS_FERR) {
            /* Clear flag by writing a 1 to it and
             * return with an error code */
            SERCOM0->USART.STATUS.reg = SERCOM_USART_STATUS_FERR;

            return -2;
        } else if (error_code & SERCOM_USART_STATUS_BUFOVF) {
            /* Clear flag by writing a 1 to it and
             * return with an error code */
            SERCOM0->USART.STATUS.reg = SERCOM_USART_STATUS_BUFOVF;

            return -3;
        } else if (error_code & SERCOM_USART_STATUS_PERR) {
            /* Clear flag by writing a 1 to it and
             * return with an error code */
            SERCOM0->USART.STATUS.reg = SERCOM_USART_STATUS_PERR;

            return -4;
        }
    }

    /* Read data from USART module */
    *rx_data = SERCOM0->USART.DATA.reg;

    return 0;
}

int usart_write_buffer_wait(
        const unsigned char *tx_data,
        unsigned short length)
{
    /* Check if the buffer length is valid */
    if (length == 0) {
        return -1;
    }

    /* Wait until synchronization is complete */
    _usart_wait_for_sync();

    uint16_t tx_pos = 0;

    /* Blocks while buffer is being transferred */
    while (length--) {
        /* Wait for the USART to be ready for new data and abort
        * operation if it doesn't get ready within the timeout*/
        for (uint32_t i = 0; i <= 0xFFFF; i++) {
            if (SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE) {
                break;
            } else if (i == 0xFFFF) {
                return -2;
            }
        }

        /* Data to send is at least 8 bits long */
        uint16_t data_to_send = tx_data[tx_pos++];

        /* Send the data through the USART module */
        usart_write_wait(data_to_send);
    }

    /* Wait until Transmit is complete or timeout */
    for (uint32_t i = 0; i <= 0xFFFF; i++) {
        if (SERCOM0->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_TXC) {
            break;
        } else if (i == 0xFFFF) {
            return -3;
        }
    }

    return 0;
}

void LOS_EvbUartInit(void)
{
    sercom_uart_init();
    return;
}

/*************************************************************************************************
 *  功能：向串口1发送一个字符                                                                    *
 *  参数：(1) 需要被发送的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
    usart_write_wait((unsigned short)c);
    return;
}

/*************************************************************************************************
 *  功能：向串口1发送一个字符串                                                                  *
 *  参数：(1) 需要被发送的字符串                                                                 *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
    usart_write_buffer_wait((const unsigned char *)str, sizeof(str));
    return;
}

/*************************************************************************************************
 *  功能：从串口1接收一个字符                                                                    *
 *  参数：(1) 存储接收到的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
    unsigned short data;
    usart_read_wait(&data);
    *c = (char)data;
    return;
}


#ifndef LOS_KERNEL_TEST_KEIL_SWSIMU
int fputc(int ch, FILE *f)
{
    LOS_EvbUartWriteByte((char)ch);

    return (ch);
}

int fgetc(FILE *f) {
    uint16_t data;
    
    usart_read_wait(&data);
    
    return (int)data;
}

#endif
