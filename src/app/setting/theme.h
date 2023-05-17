#pragma once
#include "head.h"
enum
{
    Theme_color_BLACK,
    Theme_color_WHITE
};
enum
{
    home_show_PHOTO,
    home_show_BILIBILI
};
enum
{
    open_first_HOME,
    open_first_CLOCK,
    open_first_WEATHER,
    open_first_CALENDAR,
    open_first_BILIBILI
};

extern uint8_t Theme_color;
extern uint8_t home_show;
extern uint8_t open_first;

lv_obj_t*setting_theme_load();