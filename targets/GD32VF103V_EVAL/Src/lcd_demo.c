/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2019>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "los_base.h"
#include "los_typedef.h"
#include "los_tick.h"

#include "mem.h"

#include "osal.h"


#include <osal.h>
#include <oc_mqtt_al.h>
#include "picture.h"
#include "touch_panel.h"
#include "gd32v103v_eval.h"
#include "gd32v103v_lcd_eval.h"
#include "gd25qxx.h"
#include "systick.h"


#define  SFLASH_ID                     0xC84015


extern UINT16 touch_ad_x,touch_ad_y;
static UINT16 a1=20;
static UINT16 a2=120;
static UINT16 a3=220;
static UINT16 b1=10;
static UINT16 b2=100;
static UINT16 b3=200;
static UINT16 device_code;
static UINT8 error_string[]="Please press the button!";
char_format_struct char_format;



/*!
    \brief		find the position of max value in array
    \param[in]	none
    \param[out] none
    \retval 	the position of max value in num_array array
*/
UINT8 find_max(UINT16* num_array)
{
    int id;
    id=0;
    if(num_array[id]<num_array[1]){
        id=1;
    }
    if(num_array[id]<num_array[2]){
        id=2;
    }
    if(num_array[id]<num_array[3]){
        id=3;
    }
    if(num_array[id]==0){
        id=4;
    }
    return id;
}

/*!
	\brief		get the touch area
	\param[in]	x: the row-coordinate
	\param[in]	y: the column-coordinate
	\param[out] num_array: touch area array pointer
	\retval 	none
*/
void get_touch_area(UINT16 x,UINT16 y,UINT16 *num_array)
{
    if((x<a1+70)&&(x>a1+30)&&(y>b1+30)&&(y<b1+70)){
        num_array[0]++;
    }else if((x<a2+70)&&(x>a2+30)&&(y>b1+30)&&(y<b1+70)){
        num_array[1]++;
    }else if((x<a1+70)&&(x>a1+30)&&(y>b2+30)&&(y<b2+70)){
        num_array[2]++;
    }else if((x<a2+70)&&(x>a2+30)&&(y>b2+30)&&(y<b2+70)){
        num_array[3]++;
    }
}

/*!
	\brief		change the picture of LED
	\param[in]	button_id: the id of button you just pressed
	\param[out] none
	\retval 	none
*/
void change_picture(UINT8 button_id)
{
    UINT8 len_s;
    UINT8 *str;
    UINT16 i;

    /* draw picture on LCD screen */
    lcd_picture_draw(a1+30,b1+40,a1+30+40-1,b1+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    lcd_picture_draw(a2+30,b1+40,a2+30+40-1,b1+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    lcd_picture_draw(a1+30,b2+40,a1+30+40-1,b2+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    lcd_picture_draw(a2+30,b2+40,a2+30+40-1,b2+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    /* draw character on LCD screen */
    len_s=sizeof(error_string)-1;
    str = error_string;

    char_format.char_color = LCD_COLOR_WHITE;
    for (i = 0; i < len_s; i++){
        lcd_char_display((30+8*i), 190, *str++, char_format);
    }

    switch(button_id){
        case 0:
            lcd_picture_draw(a1+30,b1+40,a1+30+40-1,b1+40+40-1,(UINT16 *)(image_on + BMP_HEADSIZE));
            break;
        case 1:
            lcd_picture_draw(a2+30,b1+40,a2+30+40-1,b1+40+40-1,(UINT16 *)(image_on + BMP_HEADSIZE));
            break;
        case 2:
            lcd_picture_draw(a1+30,b2+40,a1+30+40-1,b2+40+40-1,(UINT16 *)(image_on + BMP_HEADSIZE));
            break;
        case 3:
            lcd_picture_draw(a2+30,b2+40,a2+30+40-1,b2+40+40-1,(UINT16 *)(image_on + BMP_HEADSIZE));
            break;
        default:
            str = error_string;
            char_format.char_color = LCD_COLOR_RED;
            for (i = 0; i < len_s; i++){
                lcd_char_display((30+8*i), 190, *str++, char_format);
            }
            break;
    }
}

/*!
    \brief		turn on the LED according the pressed button
    \param[in]	button_id: the id of button you just pressed
    \param[out] none
    \retval 	none
*/
void turn_on_led(UINT8 button_id)
{
    /* firstly,turn off all the LEDs */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);

    switch(button_id){
        case 0:
            gd_eval_led_on(LED1);  /*!< turn on LED2 only */
            break;
        case 1:
            gd_eval_led_on(LED2);  /*!< turn on LED3 only */
            break;
        case 2:
            gd_eval_led_on(LED3);  /*!< turn on LED4 only */
            break;
        case 3:
            gd_eval_led_on(LED4);  /*!< turn on LED5 only */
            break;
        default:
            /* turn on all the LEDs */
            gd_eval_led_on(LED1);
            gd_eval_led_on(LED2);
            gd_eval_led_on(LED3);
            gd_eval_led_on(LED4);
            break;
    }
}




VOID lcd_handle(VOID)
{
    UINT8 count=0;
    UINT8 button_id=0;
    UINT32 led1_switch = 0;
    UINT32 led2_switch = 0;
    UINT32 led3_switch = 0;
    UINT32 led4_switch = 0;
    UINT16 num[4]={0,0,0,0}; /* store the touch point number on four buttons */

    tag_oc_mqtt_report  report;
    tag_key_value_list  lst1;
    tag_key_value_list  lst2;
    tag_key_value_list  lst3;
    tag_key_value_list  lst4;
    cJSON *root = NULL;
    char  *buf = NULL;

    while(1){
        /* get the position of touch on LCD screen */
        if(SUCCESS == touch_scan()){
            count++;
            get_touch_area(touch_coordinate_x_get(touch_ad_x),(LCD_Y - touch_coordinate_y_get(touch_ad_y)),num);
        }
        /*  generate response to the touch(turn on LED and change picture )*/
        if(count==20){
            button_id = find_max(num);
            turn_on_led(button_id);
            change_picture(button_id);
            num[0]=num[1]=num[2]=num[3]=0;
            count=0;

            if(button_id == 0)
                led1_switch = 1;
            else
                led1_switch = 0;
            lst1.item.name = "LED1";
            lst1.item.buf = (char *)&led1_switch;
            lst1.item.len = sizeof(led1_switch);
            lst1.item.type = en_key_value_type_int;
            lst1.next = &lst2;

            if(button_id == 1)
                led2_switch = 1;
            else
                led2_switch = 0;
            lst2.item.name = "LED2";
            lst2.item.buf = (char *)&led2_switch;
            lst2.item.len = sizeof(led2_switch);
            lst2.item.type = en_key_value_type_int;
            lst2.next = &lst3;

            if(button_id == 2)
                led3_switch = 1;
            else
                led3_switch = 0;
            lst3.item.name = "LED3";
            lst3.item.buf = (char *)&led3_switch;
            lst3.item.len = sizeof(led3_switch);
            lst3.item.type = en_key_value_type_int;
            lst3.next = &lst4;

            if(button_id == 3)
                led4_switch = 1;
            else
                led4_switch = 0;
            lst4.item.name = "LED4";
            lst4.item.buf = (char *)&led4_switch;
            lst4.item.len = sizeof(led4_switch);
            lst4.item.type = en_key_value_type_int;
            lst4.next = NULL;

            report.hasmore = en_oc_mqtt_has_more_no;
            report.paralst= &lst1;
            report.serviceid = "LED";
            //report.eventtime = "20190508T112020Z";

            root = oc_mqtt_json_fmt_report(&report);
            if(NULL != root)
            {
                buf = cJSON_Print(root);
                printf("buf:%s %d\n\n", buf, strlen(buf));
                if(NULL != buf)
                {
                    if(0 == oc_mqtt_report((void *)0xff,buf,strlen(buf),en_mqtt_al_qos_1));
                    osal_free(buf);
                }

                cJSON_Delete(root);
            }
        }
        osal_task_sleep(10);
    }
}

int lcd_demo()
{
    int ret = -1;
    UINT32 uwRet = LOS_OK;
    UINT32  handle;
    TSK_INIT_PARAM_S task_init_param;

    UINT16 i;
    UINT8 led_string[4][4]={"LED1","LED2","LED3","LED4"} ;
    
    /* initialize LEDs */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);
    
    /* configure the EXMC access mode */
    exmc_lcd_init();

    /* configure SPI0 GPIO and parameter */
    spi_flash_init();
    if(SFLASH_ID != spi_flash_read_id()){
        while(1);
    }

    delay_1ms(50);  

    /* read the LCD controller device code:(0x8989) */
    device_code = lcd_register_read(0x0000);

    /* initialize the LCD */
    lcd_init();

    /* clear the LCD screen */
    lcd_clear(LCD_COLOR_WHITE);

    /* draw the picture of Gigadevice logo */
    /* if you don't want to draw the picture, you should modify the macro on
       the line 422th of picture.c file and comment the next line */
    //lcd_picture_draw(40,200,40+160-1,200+87-1,(UINT16 *)(picture + BMP_HEADSIZE));
    lcd_picture_draw_ex(40,200,40+160-1,200+87-1,0);

    /* configure the GPIO of SPI touch panel */
    touch_panel_gpio_configure();


    /* draw a rectangle */
    lcd_rectangle_draw(10,10,230,310,LCD_COLOR_BLUE);

    /* configure char format */
    char_format.char_color = LCD_COLOR_BLUE;
    char_format.bk_color = LCD_COLOR_WHITE;
    char_format.direction = CHAR_DIRECTION_VERTICAL;
    char_format.font = CHAR_FONT_8_16;
    
    /* draw character on LCD screen */
    for (i = 0; i < 4; i++){
        lcd_char_display((a1+35+8*i), b1+20, *(led_string[0]+i), char_format);
        lcd_char_display((a2+35+8*i), b1+20, *(led_string[1]+i), char_format);
        lcd_char_display((a1+35+8*i), b2+20, *(led_string[2]+i), char_format);
        lcd_char_display((a2+35+8*i), b2+20, *(led_string[3]+i), char_format);
    }
    
    /* draw picture of button on LCD screen */
    lcd_picture_draw(a1+30,b1+40,a1+30+40-1,b1+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    lcd_picture_draw(a2+30,b1+40,a2+30+40-1,b1+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    lcd_picture_draw(a1+30,b2+40,a1+30+40-1,b2+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));
    lcd_picture_draw(a2+30,b2+40,a2+30+40-1,b2+40+40-1,(UINT16 *)(image_off + BMP_HEADSIZE));




    memset (&task_init_param, 0, sizeof (TSK_INIT_PARAM_S));
    task_init_param.uwArg = (unsigned int)NULL;
    task_init_param.usTaskPrio = 2;
    task_init_param.pcName =(char *) "lcd_handle";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)lcd_handle;
    task_init_param.uwStackSize = 0x1200;
    uwRet = LOS_TaskCreate(&handle, &task_init_param);
    if(LOS_OK == uwRet){
        ret = 0;
    }
    return ret;


}

