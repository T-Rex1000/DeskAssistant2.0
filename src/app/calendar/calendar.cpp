#include "app/calendar/calendar.h"
lv_obj_t*calendar;
void display_date_refresh(void*p)
{
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
        {
            lv_event_send(calendar,LV_EVENT_VALUE_CHANGED,NULL);  
            xSemaphoreGive(lv_handler_lock);
        }
        if(time_date.tm_year+1900>=2023)break;
        if(!Delay(1000))break;
    }
    vTaskDelete(NULL);
}
void calendar_event(lv_event_t*e)
{
    bool frist=1;
    uint8_t code=lv_event_get_code(e);
    if(code==LV_EVENT_VALUE_CHANGED)
    {
        lv_calendar_set_today_date(calendar,time_date.tm_year+1900,time_date.tm_mon+1,time_date.tm_mday);
        lv_calendar_set_showed_date(calendar,time_date.tm_year+1900,time_date.tm_mon+1);
    }
}
lv_obj_t* calendar_load()
{
    now_screen =lv_obj_create(NULL);
    if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_bg_color(now_screen,lv_color_make(228,210,124), LV_STATE_DEFAULT);
        lv_obj_set_size(now_screen, screenWidth, screenHeight); 

        calendar=lv_calendar_create(now_screen);
        lv_obj_set_size(calendar,screenWidth,screenHeight);
        lv_calendar_set_today_date(calendar,time_date.tm_year+1900,time_date.tm_mon+1,time_date.tm_mday);
        lv_calendar_set_showed_date(calendar,time_date.tm_year+1900,time_date.tm_mon+1);
        lv_obj_set_style_bg_color(calendar, lv_color_make(239,228,176), LV_PART_MAIN);
        lv_obj_set_style_border_color(calendar, lv_color_make(239,228,176), LV_PART_MAIN);
        lv_obj_set_style_border_color(lv_calendar_get_btnmatrix(calendar),lv_color_make(239,228,176), LV_PART_ITEMS);
        lv_obj_set_style_bg_color(lv_calendar_get_btnmatrix(calendar), lv_color_make(239,228,176), LV_PART_ITEMS);
        
        lv_obj_set_style_text_color(calendar, lv_color_hex(0x000000), LV_PART_MAIN);

        const char* daysName[7] = { "日", "一", "二",  "三", "四", "五", "六" };
        lv_obj_set_style_text_font(calendar, &chinese_hei_16, LV_PART_MAIN);
        
        lv_calendar_set_day_names(calendar, daysName);
        lv_obj_set_style_text_font(lv_calendar_header_arrow_create(calendar) ,&lv_font_montserrat_16, LV_PART_MAIN);
        
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_bg_color(now_screen,lv_color_make(0,0,0), LV_STATE_DEFAULT);
        lv_obj_set_size(now_screen, screenWidth, screenHeight); 

        calendar=lv_calendar_create(now_screen);
        lv_obj_set_size(calendar,screenWidth,screenHeight);
        lv_calendar_set_today_date(calendar,time_date.tm_year+1900,time_date.tm_mon+1,time_date.tm_mday);
        lv_calendar_set_showed_date(calendar,time_date.tm_year+1900,time_date.tm_mon+1);
        lv_obj_set_style_bg_color(calendar, lv_color_make(0,0,0), LV_PART_MAIN);
        lv_obj_set_style_border_color(calendar, lv_color_make(0,0,0), LV_PART_MAIN);
        lv_obj_set_style_border_color(lv_calendar_get_btnmatrix(calendar),lv_color_make(0,0,0), LV_PART_ITEMS);
        lv_obj_set_style_bg_color(lv_calendar_get_btnmatrix(calendar), lv_color_make(0,0,0), LV_PART_ITEMS);

        lv_obj_set_style_text_color(calendar, lv_color_hex(0xffffff), LV_PART_MAIN);

        const char* daysName[7] = { "日", "一", "二",  "三", "四", "五", "六" };
        lv_obj_set_style_text_font(calendar, &chinese_hei_16, LV_PART_MAIN);
        
        lv_calendar_set_day_names(calendar, daysName);
        lv_obj_set_style_text_font(lv_calendar_header_arrow_create(calendar) ,&lv_font_montserrat_16, LV_PART_MAIN);
        
        
    }
    lv_obj_add_event_cb(calendar,calendar_event,LV_EVENT_ALL,NULL);
    gesture(0,1,0,1);
    xTaskCreatePinnedToCore(time_update,NULL,4096,NULL,14,NULL,0);
    xTaskCreatePinnedToCore(display_date_refresh,NULL,4096,NULL,15,NULL,0);
    return now_screen;
}