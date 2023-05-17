#pragma once
#include "head.h"
LV_IMG_DECLARE(sun);
LV_IMG_DECLARE(suncloudy);
LV_IMG_DECLARE(cloudy);
LV_IMG_DECLARE(rain);
LV_IMG_DECLARE(thunder);
LV_IMG_DECLARE(snow);
LV_IMG_DECLARE(sand);
LV_IMG_DECLARE(wind);
bool Delay(uint32_t time);
lv_obj_t* weather_load();
lv_obj_t*weather_config_load();
void weather_update(void *p);
void weather_code_icon(lv_obj_t*icon,int code);