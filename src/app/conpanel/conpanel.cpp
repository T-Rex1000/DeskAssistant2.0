#if 1
#include "conpanel.h"
lv_obj_t *wifi_btn;
lv_obj_t *bluetooth_btn;
lv_obj_t *wifiAP_btn;
void wifi_event(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    switch(code)
    {
        case LV_EVENT_CLICKED:
        {
            wifi_s=!wifi_s;
            if(wifi_s||wifiAP_s)
            {
                lv_obj_add_state(bluetooth_btn,LV_STATE_DISABLED);
            }
            else
            {
                lv_obj_clear_state(bluetooth_btn,LV_STATE_DISABLED);
            }
            if(bluetooth_s)
            {
                lv_obj_add_state(wifi_btn,LV_STATE_DISABLED);
                lv_obj_add_state(wifiAP_btn,LV_STATE_DISABLED);
            }
            else
            {
                lv_obj_clear_state(wifi_btn,LV_STATE_DISABLED);
                lv_obj_clear_state(wifiAP_btn,LV_STATE_DISABLED);
            }
            if(wifi_s)
            {
                if(wifiAP_s) WiFi.mode(WIFI_AP_STA);
                else WiFi.mode(WIFI_STA);

                WiFi.begin(WiFi_SSID.c_str(),WiFi_Password.c_str());
            }
            else
            {
                if(wifiAP_s) WiFi.mode(WIFI_AP);
                else WiFi.mode(WIFI_OFF);
            }
        }
    }
}
void bluetooth_event(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    switch(code)
    {
        case LV_EVENT_CLICKED:
        {
            bluetooth_s=!bluetooth_s;
            if(wifi_s||wifiAP_s)
            {
                lv_obj_add_state(bluetooth_btn,LV_STATE_DISABLED);
            }
            else
            {
                lv_obj_clear_state(bluetooth_btn,LV_STATE_DISABLED);
            }
            if(bluetooth_s)
            {
                lv_obj_add_state(wifi_btn,LV_STATE_DISABLED);
                lv_obj_add_state(wifiAP_btn,LV_STATE_DISABLED);
            }
            else
            {
                lv_obj_clear_state(wifi_btn,LV_STATE_DISABLED);
                lv_obj_clear_state(wifiAP_btn,LV_STATE_DISABLED);
            }
            if(bluetooth_s)
            {
                //bluetooth_init();
            }
            else
            {
                //bluetooth_close();
            }
        }
    }
}
void wifiAP_event(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    switch(code)
    {
        case LV_EVENT_CLICKED:
        {
            wifiAP_s=!wifiAP_s;
            if(wifi_s||wifiAP_s)
            {
                lv_obj_add_state(bluetooth_btn,LV_STATE_DISABLED);
            }
            else
            {
                lv_obj_clear_state(bluetooth_btn,LV_STATE_DISABLED);
            }
            if(bluetooth_s)
            {
                lv_obj_add_state(wifi_btn,LV_STATE_DISABLED);
                lv_obj_add_state(wifiAP_btn,LV_STATE_DISABLED);
            }
            else
            {
                lv_obj_clear_state(wifi_btn,LV_STATE_DISABLED);
                lv_obj_clear_state(wifiAP_btn,LV_STATE_DISABLED);
            }
            if(wifiAP_s)
            {
                if(wifi_s) WiFi.mode(WIFI_AP_STA);
                else WiFi.mode(WIFI_AP);

                WiFi.softAP(wifiAP_SSID.c_str(),wifiAP_Password.c_str());
            }
            else
            {
                if(wifi_s) WiFi.mode(WIFI_STA);
                else WiFi.mode(WIFI_OFF);
            }
        }
    }
}
lv_obj_t*backlight_scroll;
void btn_backlight_scroll_event(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t*backlight_scroll=lv_event_get_target(event);
    switch(code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            ledcWrite(blk,lv_slider_get_value(backlight_scroll));
            lv_slider_set_value(backlight_scroll,lv_slider_get_value(backlight_scroll),LV_ANIM_ON);
        }
        break;
    }
}
void backlight_save()
{
    if(blk_value!=lv_slider_get_value(backlight_scroll))
    {
        blk_value=lv_slider_get_value(backlight_scroll);
        tf.writeFile("/system/app/setting/config/backlight.txt",(String(blk_value)+"\r\n").c_str());
    }
}
lv_obj_t* conpanel_load()
{
    now_screen=lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        text_color=lv_color_hex(0x000000);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_hex(0xeeeeee);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    wifi_btn=lv_btn_create(now_screen);
    bluetooth_btn=lv_btn_create(now_screen);
    wifiAP_btn=lv_btn_create(now_screen);

    lv_obj_set_size(wifi_btn,60,50);
    lv_obj_set_size(bluetooth_btn,60,50);
    lv_obj_set_size(wifiAP_btn,60,50);

    lv_obj_align_to(wifi_btn,now_screen,LV_ALIGN_TOP_LEFT,10,30);
    lv_obj_align_to(bluetooth_btn,now_screen,LV_ALIGN_TOP_MID,0,30);
    lv_obj_align_to(wifiAP_btn,now_screen,LV_ALIGN_TOP_RIGHT,-10,30);

    lv_obj_add_flag(wifi_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(bluetooth_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(wifiAP_btn, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_set_style_bg_color(wifi_btn, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bluetooth_btn, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
    lv_obj_set_style_bg_color(wifiAP_btn, lv_color_hex(0x5F5F5F), LV_PART_MAIN);

    lv_obj_set_style_bg_color(wifi_btn, lv_color_hex(0x2196f3),LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(bluetooth_btn, lv_color_hex(0x2196f3),LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(wifiAP_btn, lv_color_hex(0x2196f3),LV_STATE_CHECKED);

    lv_obj_add_state(wifi_btn,wifi_s);
    lv_obj_add_state(bluetooth_btn,bluetooth_s);
    lv_obj_add_state(wifiAP_btn,wifiAP_s);

    if(wifi_s||wifiAP_s)
    {
        lv_obj_add_state(bluetooth_btn,LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_clear_state(bluetooth_btn,LV_STATE_DISABLED);
    }
    if(bluetooth_s)
    {
        lv_obj_add_state(wifi_btn,LV_STATE_DISABLED);
        lv_obj_add_state(wifiAP_btn,LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_clear_state(wifi_btn,LV_STATE_DISABLED);
        lv_obj_clear_state(wifiAP_btn,LV_STATE_DISABLED);
    }

    lv_obj_add_event_cb(wifi_btn,wifi_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(bluetooth_btn,bluetooth_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(wifiAP_btn,wifiAP_event,LV_EVENT_ALL,NULL);
    
    lv_obj_t *wifi_btn_icon=lv_img_create(wifi_btn);
    lv_obj_t *bluetooth_btn_icon=lv_img_create(bluetooth_btn);
    lv_obj_t *wifiAP_btn_icon=lv_img_create(wifiAP_btn);
    
    lv_obj_set_style_bg_color(wifi_btn_icon, lv_color_hex(0xeeeeee),0);
    lv_obj_set_style_bg_color(bluetooth_btn_icon, lv_color_hex(0xeeeeee),0);
    lv_obj_set_style_bg_color(wifiAP_btn_icon, lv_color_hex(0xeeeeee),0);

    lv_img_set_src(wifi_btn_icon,LV_SYMBOL_WIFI);
    lv_img_set_src(bluetooth_btn_icon,LV_SYMBOL_BLUETOOTH);
    lv_img_set_src(wifiAP_btn_icon,LV_SYMBOL_GPS);

    lv_obj_set_align(wifi_btn_icon,LV_ALIGN_CENTER);
    lv_obj_set_align(bluetooth_btn_icon,LV_ALIGN_CENTER);
    lv_obj_set_align(wifiAP_btn_icon,LV_ALIGN_CENTER);

    backlight_scroll=lv_slider_create(now_screen);
    lv_obj_remove_style(backlight_scroll, NULL, LV_PART_KNOB);
    
    if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_bg_color(backlight_scroll, lv_color_hex(0x5F5F5F), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(backlight_scroll, lv_color_hex(0xC3C3C3), LV_PART_MAIN);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_bg_color(backlight_scroll, lv_color_hex(0xC3C3C3), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(backlight_scroll, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
    }
    lv_obj_set_size(backlight_scroll,200,35);
    //lv_slider_set_mode(backlight_scroll);
    lv_slider_set_mode(backlight_scroll,LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(backlight_scroll,pow(2,8),LV_ANIM_ON);
    lv_slider_set_range(backlight_scroll,pow(2,1),pow(2,11));
    lv_obj_add_flag(backlight_scroll, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_set_align(backlight_scroll,LV_ALIGN_TOP_MID);
    lv_obj_set_y(backlight_scroll,115);
    lv_slider_set_value(backlight_scroll,blk_value,LV_ANIM_ON);

    lv_obj_add_event_cb(backlight_scroll,btn_backlight_scroll_event,LV_EVENT_ALL,NULL);
    gesture(1,0,0,0,backlight_save);
    
    return now_screen;
}
#endif