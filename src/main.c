/**
 * @file main.c
 *
 */
#include <unistd.h>
#include <sys/time.h>

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "png_to_rgb.h"
#include "client.h"

#define LV_HOR_RES_MAX          (320)
#define LV_VER_RES_MAX          (170)
#define PORT 11112

typedef struct
{
    char *res_path;
    lv_img_dsc_t *img;
} res;

static res data[] = {
    {
        .res_path = "./res/icon_resize/0.png",
    },
    {
        .res_path = "./res/icon_resize/1.png",
    },
    {
        .res_path = "./res/icon_resize/2.png",
    },
    {
        .res_path = "./res/icon_resize/3.png",
    },
    {
        .res_path = "./res/icon_resize/4.png",
    },
    {
        .res_path = "./res/icon_resize/5.png",
    }
};



lv_img_dsc_t *icon_img_dsc_load_from_file(char *file_path)
{
    int32_t rev;
    png_convert_info info;
    rev = png_convert_2_rgb_filepath(file_path, &info);

    lv_img_dsc_t *img_dsc = malloc(sizeof *img_dsc);
    if (rev != 0)
    {
        free(img_dsc);
        return NULL;
    }

    img_dsc->header.always_zero = 0;
    img_dsc->header.w = info.width;
    img_dsc->header.h = info.height;
    img_dsc->data_size = info.rgb_data_len;
    img_dsc->data = info.rgb_data;
    img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;

    return img_dsc;
}


lv_obj_t *ui_home_creat()
{

    // 以屏幕为父对象创建 obj
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);
    lv_obj_t *main_obj = lv_obj_create(scr, NULL);
    lv_obj_set_size(main_obj, lv_obj_get_width(scr), lv_obj_get_height(scr));
    lv_obj_align(main_obj, scr, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    // obj style set
    static lv_style_t main_style;
    lv_style_init(&main_style);
    lv_style_set_border_width(&main_style, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&main_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_add_style(main_obj, LV_OBJ_PART_MAIN, &main_style);

    // app page
    int line_num = 6;
    int apps_par_scrl_w = 98 + 192 * line_num;
    int apps_par_w = LV_HOR_RES;
    int apps_par_h = LV_VER_RES_MAX;

    lv_obj_t *apps_par = lv_page_create(main_obj, NULL);
    lv_page_glue_obj(apps_par, true);
    // apps_par->user_data = state;
    lv_obj_set_size(apps_par, apps_par_w, apps_par_h);
    lv_page_set_scrl_width(apps_par, apps_par_scrl_w);
    lv_page_set_scrl_height(apps_par, apps_par_h);
    lv_page_set_scrlbar_mode(apps_par, LV_SCRLBAR_MODE_OFF);
    lv_page_set_edge_flash(apps_par, false);
    lv_obj_align(apps_par, main_obj, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    static lv_style_t apps_par_style;
    lv_style_init(&apps_par_style);
    lv_style_set_bg_color(&apps_par_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&apps_par_style, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(apps_par, LV_OBJ_PART_MAIN, &apps_par_style);

    static lv_style_t app_item_style;
    lv_style_init(&app_item_style);
    lv_style_set_bg_color(&app_item_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&app_item_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&app_item_style, LV_STATE_DEFAULT, 0);

    static lv_style_t app_btn_style;
    lv_style_init(&app_btn_style);
    lv_style_set_pad_top(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_top(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_bottom(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_left(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_margin_right(&app_btn_style, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&app_btn_style, LV_STATE_DEFAULT, 0);

    for (long i = 0; i < line_num; i++)
    {
        lv_obj_t *app_item = lv_cont_create(apps_par, NULL);
        lv_page_glue_obj(app_item, true);
        lv_obj_add_style(app_item, LV_OBJ_PART_MAIN, &app_item_style);

        lv_obj_set_pos(app_item, 47 + 192 * i, 0);
        lv_obj_set_width(app_item, 100);
        lv_obj_set_height(app_item, 100);

        lv_obj_t *item_obj = app_item;

        lv_obj_t *button = lv_imgbtn_create(item_obj, NULL);
        lv_page_glue_obj(button, true);
        lv_obj_add_style(button, LV_OBJ_PART_MAIN, &app_btn_style);

        lv_imgbtn_set_src(button, LV_BTN_STATE_RELEASED, (data[i]).img);
        lv_imgbtn_set_src(button, LV_BTN_STATE_PRESSED, (data[i]).img);
        lv_obj_align(button, item_obj, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_set_parent_event(button, true);
    }

    return main_obj;
}


int main(void)
{
    sim_lcd_init(PORT);

    // icon read
    int num = sizeof(data) / sizeof(res);
    for (int i = 0; i < num; i++)
    {
        data[i].img = icon_img_dsc_load_from_file(data[i].res_path);
    }

    /* LittlevGL init */
    lv_init();
	
    // 初始化 display
    lv_port_disp_init();

    // 初始化 input
    lv_port_indev_init();

    // 设置屏幕背景
    lv_obj_t *scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);

    static lv_style_t screen_style;
    lv_style_init(&screen_style);
    lv_style_set_bg_color(&screen_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_add_style(scr, LV_OBJ_PART_MAIN, &screen_style);

    // 添加图标
    ui_home_creat();

    while(1) {
        lv_task_handler();
        usleep(5*1000);
		lv_tick_inc(5);
    }

    sim_lcd_release();

    return 0;
}


