#include "setting.h"
lv_obj_t *setting_title;
int setting_scroll_d=0;
enum
{
    WIFI_B=1,
    WIFIAP_B,
    BLUETOOTH_B,
    THEME_B,
    SAFE_B,
    UPDATE_B,
    ABOUT_B,
};

void setting_btn_event(lv_event_t *event)
{
    setting_scroll_d=lv_obj_get_scroll_y(now_screen);
    load app_load=(load)lv_event_get_user_data(event);
    if(app_load!=NULL)
        app_open(app_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
}
void setting_back_task()
{
    setting_scroll_d=0;
}
lv_obj_t* setting_load()
{
    now_screen =lv_obj_create(NULL);
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

    lv_obj_t *class_link=lv_label_create(now_screen);
    lv_obj_t *class_theme=lv_label_create(now_screen);
    lv_obj_t *class_safe=lv_label_create(now_screen);
    lv_obj_t *class_drive=lv_label_create(now_screen);

    lv_obj_set_style_text_font(class_link,&chinese_hei_16,0);
    lv_obj_set_style_text_font(class_theme,&chinese_hei_16,0);
    lv_obj_set_style_text_font(class_safe,&chinese_hei_16,0);
    lv_obj_set_style_text_font(class_drive,&chinese_hei_16,0);
    lv_obj_set_style_text_color(class_link,text_color,0);
    lv_obj_set_style_text_color(class_theme,text_color,0);
    lv_obj_set_style_text_color(class_safe,text_color,0);
    lv_obj_set_style_text_color(class_drive,text_color,0);

    lv_label_set_text(class_link,"连接");
    lv_label_set_text(class_theme,"个性化");
    lv_label_set_text(class_safe,"安全");
    lv_label_set_text(class_drive,"设备");

    lv_obj_align(class_link,LV_ALIGN_TOP_LEFT,30,40);
    lv_obj_align(class_theme,LV_ALIGN_TOP_LEFT,30,250);
    lv_obj_align(class_safe,LV_ALIGN_TOP_LEFT,30,350);
    lv_obj_align(class_drive,LV_ALIGN_TOP_LEFT,30,450);

    lv_obj_t *wifi_btn  =lv_btn_create(now_screen);lv_obj_t *wifi_icon      =lv_label_create(wifi_btn);
    lv_obj_t *wifiAP_btn=lv_btn_create(now_screen);lv_obj_t *wifiAP_icon    =lv_label_create(wifiAP_btn);
    lv_obj_t *blue_btn  =lv_btn_create(now_screen);lv_obj_t *blue_icon      =lv_label_create(blue_btn);
    lv_obj_t *theme_btn =lv_btn_create(now_screen);lv_obj_t *theme_icon     =lv_label_create(theme_btn);
    lv_obj_t *safe_btn  =lv_btn_create(now_screen);lv_obj_t *safe_icon      =lv_label_create(safe_btn);
    lv_obj_t *update_btn=lv_btn_create(now_screen);lv_obj_t *update_icon    =lv_label_create(update_btn);
    lv_obj_t *about_btn =lv_btn_create(now_screen);lv_obj_t *about_icon     =lv_label_create(about_btn);
    lv_obj_t *bottom=lv_label_create(now_screen);
    lv_obj_set_style_bg_color(wifi_btn, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(wifiAP_btn, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(blue_btn, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(theme_btn, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(safe_btn, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(update_btn, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(about_btn, main_color, LV_PART_MAIN);

    if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_bg_color(wifi_btn, lv_color_make(28,187,255),LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(wifiAP_btn, lv_color_make(28,187,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(blue_btn, lv_color_make(28,187,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(theme_btn, lv_color_make(28,187,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(safe_btn, lv_color_make(28,187,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(update_btn, lv_color_make(28,187,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(about_btn, lv_color_make(28,187,255), LV_STATE_PRESSED);
        
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_bg_color(wifi_btn, lv_color_make(23,186,255),LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(wifiAP_btn, lv_color_make(23,186,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(blue_btn, lv_color_make(23,186,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(theme_btn, lv_color_make(23,186,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(safe_btn, lv_color_make(23,186,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(update_btn, lv_color_make(23,186,255), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(about_btn, lv_color_make(23,186,255), LV_STATE_PRESSED);

    }
    lv_obj_set_style_shadow_width(wifi_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(wifiAP_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(blue_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(theme_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(safe_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(update_btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(about_btn, 0, LV_PART_MAIN);

    lv_obj_set_style_text_color(wifi_icon,text_color,0);
    lv_obj_set_style_text_color(wifiAP_icon,text_color,0);
    lv_obj_set_style_text_color(blue_icon,text_color,0);
    lv_obj_set_style_text_color(theme_icon,text_color,0);
    lv_obj_set_style_text_color(safe_icon,text_color,0);
    lv_obj_set_style_text_color(update_icon,text_color,0);
    lv_obj_set_style_text_color(about_icon,text_color,0);
    lv_label_set_text(wifi_icon,LV_SYMBOL_WIFI);
    lv_label_set_text(wifiAP_icon,LV_SYMBOL_SHUFFLE);
    lv_label_set_text(blue_icon,LV_SYMBOL_BLUETOOTH);
    lv_label_set_text(theme_icon,LV_SYMBOL_HOME);
    lv_label_set_text(safe_icon,LV_SYMBOL_EYE_CLOSE);
    lv_label_set_text(update_icon,LV_SYMBOL_REFRESH);
    lv_label_set_text(about_icon,LV_SYMBOL_SETTINGS);

    lv_obj_align(wifi_icon,LV_ALIGN_LEFT_MID,5,0);
    lv_obj_align(wifiAP_icon,LV_ALIGN_LEFT_MID,5,0);
    lv_obj_align(blue_icon,LV_ALIGN_LEFT_MID,5,0);
    lv_obj_align(theme_icon,LV_ALIGN_LEFT_MID,5,0);
    lv_obj_align(safe_icon,LV_ALIGN_LEFT_MID,5,0);
    lv_obj_align(update_icon,LV_ALIGN_LEFT_MID,5,0);
    lv_obj_align(about_icon,LV_ALIGN_LEFT_MID,5,0);

    lv_obj_t *wifi_text =lv_label_create(wifi_btn);
    lv_obj_t *wifiAP_text=lv_label_create(wifiAP_btn);
    lv_obj_t *blue_text  =lv_label_create(blue_btn);
    lv_obj_t *theme_text =lv_label_create(theme_btn);
    lv_obj_t *safe_text  =lv_label_create(safe_btn);
    lv_obj_t *update_text=lv_label_create(update_btn);
    lv_obj_t *about_text =lv_label_create(about_btn);

    lv_obj_set_style_text_font(wifi_text,&chinese_hei_16,0);
    lv_obj_set_style_text_font(wifiAP_text,&chinese_hei_16,0);
    lv_obj_set_style_text_font(blue_text,&chinese_hei_16,0);
    lv_obj_set_style_text_font(theme_text,&chinese_hei_16,0);
    lv_obj_set_style_text_font(safe_text,&chinese_hei_16,0);
    lv_obj_set_style_text_font(update_text,&chinese_hei_16,0);
    lv_obj_set_style_text_font(about_text,&chinese_hei_16,0);
    lv_obj_set_style_text_color(wifi_text,text_color,0);
    lv_obj_set_style_text_color(wifiAP_text,text_color,0);
    lv_obj_set_style_text_color(blue_text,text_color,0);
    lv_obj_set_style_text_color(theme_text,text_color,0);
    lv_obj_set_style_text_color(safe_text,text_color,0);
    lv_obj_set_style_text_color(update_text,text_color,0);
    lv_obj_set_style_text_color(about_text,text_color,0);

    lv_obj_align(wifi_text,LV_ALIGN_LEFT_MID,30,5);
    lv_obj_align(wifiAP_text,LV_ALIGN_LEFT_MID,30,5);
    lv_obj_align(blue_text,LV_ALIGN_LEFT_MID,30,5);
    lv_obj_align(theme_text,LV_ALIGN_LEFT_MID,30,5);
    lv_obj_align(safe_text,LV_ALIGN_LEFT_MID,30,5);
    lv_obj_align(update_text,LV_ALIGN_LEFT_MID,30,5);
    lv_obj_align(about_text,LV_ALIGN_LEFT_MID,30,5);

    if(WiFi.status()==WL_CONNECTED)lv_label_set_text(wifi_text,WiFi.SSID().c_str());
    else lv_label_set_text(wifi_text,"WiFi");
    lv_label_set_text(wifiAP_text,"WiFi热点");
    lv_label_set_text(blue_text,"蓝牙");
    lv_label_set_text(theme_text,"主题");
    lv_label_set_text(safe_text,"锁屏");
    lv_label_set_text(update_text,"系统更新");
    lv_label_set_text(about_text,"此设备");
    lv_label_set_text(bottom,"");


    lv_obj_set_size(wifi_btn,200,40);
    lv_obj_set_size(wifiAP_btn,200,40);
    lv_obj_set_size(blue_btn,200,40);
    lv_obj_set_size(theme_btn,200,40);
    lv_obj_set_size(safe_btn,200,40);
    lv_obj_set_size(update_btn,200,40);
    lv_obj_set_size(about_btn,200,40);
    lv_obj_set_size(bottom,200,20);
    
    lv_obj_align_to(wifi_btn,class_link,LV_ALIGN_OUT_TOP_LEFT,0,70);
    lv_obj_align_to(wifiAP_btn,wifi_btn,LV_ALIGN_TOP_MID,0,50);
    lv_obj_align_to(blue_btn,wifiAP_btn,LV_ALIGN_TOP_MID,0,50);

    lv_obj_align_to(theme_btn,class_theme,LV_ALIGN_OUT_TOP_LEFT,0,70);

    lv_obj_align_to(safe_btn,class_safe,LV_ALIGN_OUT_TOP_LEFT,0,70);

    lv_obj_align_to(update_btn,class_drive,LV_ALIGN_OUT_TOP_LEFT,0,70);
    lv_obj_align_to(about_btn,update_btn,LV_ALIGN_TOP_MID,0,50);

    lv_obj_align_to(bottom,about_btn,LV_ALIGN_OUT_BOTTOM_MID,0,0);

    lv_obj_add_event_cb(wifi_btn,setting_btn_event,LV_EVENT_CLICKED,(void*)setting_wifi_load);
    lv_obj_add_event_cb(wifiAP_btn,setting_btn_event,LV_EVENT_CLICKED,(void*)setting_wifiAP_load);
    lv_obj_add_event_cb(blue_btn,setting_btn_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(theme_btn,setting_btn_event,LV_EVENT_CLICKED,(void*)setting_theme_load);
    lv_obj_add_event_cb(safe_btn,setting_btn_event,LV_EVENT_CLICKED,(void*)setting_lock_load);
    lv_obj_add_event_cb(update_btn,setting_btn_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(about_btn,setting_btn_event,LV_EVENT_CLICKED,(void*)about_load);

    lv_obj_scroll_to_y(now_screen,setting_scroll_d,LV_ANIM_OFF);
    gesture(0,1,0,1,setting_back_task);
    return now_screen;
}

