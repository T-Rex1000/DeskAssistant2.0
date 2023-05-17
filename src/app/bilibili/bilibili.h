#pragma once
#include "head.h"
LV_IMG_DECLARE(bilibili_fans);
LV_IMG_DECLARE(bilibili_likes);
LV_IMG_DECLARE(bilibili_views);
LV_IMG_DECLARE(bilibili_follow);

extern lv_obj_t*fans_icon;
extern lv_obj_t*like_icon;
extern lv_obj_t*video_icon;
extern lv_obj_t*start_icon;
extern lv_obj_t*fans_num;
extern lv_obj_t*like_num;
extern lv_obj_t*video_num;
extern lv_obj_t*start_num;
extern uint8_t bilibili_theme;
enum
{
    BILIBILI_PINK,
    BILIBILI_BLUE,
};
void bilibili_update(void *p);
lv_obj_t* bilibili_load();
lv_obj_t*bilibili_config_load();