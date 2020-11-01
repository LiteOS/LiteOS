/**
 * @file indev.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "tft/tft.h"
#include "lvgl.h"

#include "stm32f7xx.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_ts.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool touchpad_read_cb(lv_indev_drv_t * drv, lv_indev_data_t *data);

/**********************
 *  STATIC VARIABLES
 **********************/
static TS_StateTypeDef  TS_State;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize your input devices here
 */
void touchpad_init(void)
{
    BSP_TS_Init(TFT_HOR_RES, TFT_VER_RES);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touchpad_read_cb;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool touchpad_read_cb(lv_indev_drv_t * drv, lv_indev_data_t *data)
{
    static int16_t last_x = 0;
    static int16_t last_y = 0;
    BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected != 0) {
        data->point.x = TS_State.touchX[0];
        data->point.y = TS_State.touchY[0];
        last_x = data->point.x;
        last_y = data->point.y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
    }

    return false;
}
