#ifndef GUI_H
#define GUI_H
#include "head.h"
#include "app/home.h"
#include "app/menu/menu.h"
#include "app/time/time.h"
#include "app/weather/weather.h"
#include "app/conpanel/conpanel.h"
#include "app/setting/setting.h"
#include "app/calendar/calendar.h"
#include "app/bilibili/bilibili.h"
#include "app/photo/photo.h"
#include "app/filemanage/filemanage.h"
#include "app/paint/paint.h"
#include "app/calculator/calculator.h"
#include "app/upload/upload.h"
#include "app/linkhome/linkhome.h"
#include "app/remotecontrol/remotecontrol.h"
#include "app/sweep/sweep.h"
#include"app/game1024/game1024.h"
#include"app/translate/translate.h"

extern bool use_lvgl;
typedef lv_obj_t* (*load)();
enum MODE
{
    HOME=0,
    TIME,
    WEATHER,
    CALENDAR,
    BILIBILI,
    CALCULATOR,
    SETTING,
    MENU,
    CONPANEL,
};
extern bool RUNMODE;
extern lv_obj_t* now_screen;
extern bool MODE_revise;
void null_func();
void gui_init(uint8_t runMODE=HOME);
void app_open(load app_load,lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool auto_del=true,void (*Other_app)()=NULL);
void app_return(lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool auto_del=true);
//void gesture_left_return(lv_event_t*event);
void gesture(bool top,bool bottom,bool left,bool right,void(*back_task)()=null_func);
#endif

