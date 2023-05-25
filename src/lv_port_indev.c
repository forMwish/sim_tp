/**
 * @file lv_port_indev_templ.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

#include "lv_port_indev.h"

static void touchpad_init(void);
static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

lv_indev_t * indev_touchpad;

static lv_indev_drv_t indev_drv_touchpad;

void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv_touchpad);
    indev_drv_touchpad.type = LV_INDEV_TYPE_POINTER;
    indev_drv_touchpad.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv_touchpad);

}

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
    low_ts_init();
}

static uint32_t time_touchpad_read = 0;
uint32_t touchpad_get_read_times()
{
    return time_touchpad_read;
}

/* Will be called by the library to read the touchpad */
static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
        time_touchpad_read++;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

int32_t touchx, touchy;

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    // if(low_ts_load(&touchx, &touchy))
    if(sim_ts_load(&touchx, &touchy))
    {
        // printf("===== cord:%d %d\n", touchx, touchy);
        return true;
    }
    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/
    (*x) = touchx;
    (*y) = touchy;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
