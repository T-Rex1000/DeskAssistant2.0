#include"app/setting/wifiAP.h"

String wifiAP_SSID="ESPWiFi";
String wifiAP_Password="123123123";
bool wifiAP_s=0;


lv_obj_t*wifiAP_ssid;
lv_obj_t*wifiAP_password;
lv_obj_t*wifiAP_ssid_label;
lv_obj_t*wifiAP_password_label;



void wifiAP_password_change(lv_event_t *e)
{
    int code = lv_event_get_code(e);
    lv_obj_t* target=lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(e);
    if(code==LV_EVENT_FOCUSED)
    {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            lv_keyboard_set_textarea(kb, target);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if(code==LV_EVENT_DEFOCUSED)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
    else if(code == LV_EVENT_CANCEL)
    {
        lv_textarea_set_text(wifiAP_ssid,wifiAP_SSID.c_str());
        lv_textarea_set_text(wifiAP_password,wifiAP_Password.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}
void wifiAP_switch_event(lv_event_t*e)
{
    wifiAP_s=!wifiAP_s;
    if(wifiAP_s)
    {
        wifiAP_SSID=lv_textarea_get_text(wifiAP_ssid);
        wifiAP_Password=lv_textarea_get_text(wifiAP_password);
        lv_label_set_text(wifiAP_ssid_label,wifiAP_SSID.c_str());
        lv_label_set_text(wifiAP_password_label,wifiAP_Password.c_str());
        lv_obj_add_flag(wifiAP_ssid, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifiAP_password, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifiAP_ssid_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifiAP_password_label, LV_OBJ_FLAG_HIDDEN);
        
        if(wifi_s) WiFi.mode(WIFI_AP_STA);
        else WiFi.mode(WIFI_AP);
        WiFi.softAP(wifiAP_SSID.c_str(),wifiAP_Password.c_str());

        lv_label_set_text(setting_title,"WiFi热点(已开启)");
        tf.writeFile("/system/app/setting/config/wifiAP.txt",(wifiAP_SSID+"\r\n"+wifiAP_Password+"\r\n").c_str());
    }
    else
    {
        lv_obj_add_flag(wifiAP_ssid_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifiAP_password_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifiAP_ssid, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifiAP_password, LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text(setting_title,"WiFi热点(已关闭)");
        if(wifi_s) WiFi.mode(WIFI_STA);
        else WiFi.mode(WIFI_OFF);
    }
}
lv_obj_t*setting_wifiAP_load()
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

    lv_obj_t *wifiAP_icon=lv_label_create(now_screen);
    lv_obj_set_style_text_color(wifiAP_icon,text_color,LV_PART_MAIN);
    lv_label_set_text(wifiAP_icon,LV_SYMBOL_SHUFFLE);
    lv_obj_align(wifiAP_icon,LV_ALIGN_TOP_LEFT,20,30);

    setting_title=lv_label_create(now_screen);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align_to(setting_title,wifiAP_icon,LV_ALIGN_OUT_RIGHT_MID,10,0);
    
    

    wifiAP_ssid=lv_textarea_create(now_screen);
    lv_textarea_set_one_line(wifiAP_ssid, true);
    //lv_obj_add_style(wifiAP_ssid,&style_chinese_16_white,0);
    lv_obj_set_style_text_font(wifiAP_ssid,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(wifiAP_ssid,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_size(wifiAP_ssid,220,40);
    lv_obj_align_to(wifiAP_ssid,wifiAP_icon,LV_ALIGN_OUT_BOTTOM_LEFT,-10,30);
    lv_obj_clear_state(wifiAP_ssid,LV_STATE_FOCUSED);
    
    wifiAP_password=lv_textarea_create(now_screen);
    lv_textarea_set_one_line(wifiAP_password, true);
    //lv_obj_add_style(wifiAP_password,&style_chinese_16_white,0);
    lv_obj_set_style_text_font(wifiAP_password,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(wifiAP_password,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_size(wifiAP_password,220,40);
    lv_obj_align_to(wifiAP_password,wifiAP_ssid,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_clear_state(wifiAP_password,LV_STATE_FOCUSED);

    
    lv_textarea_set_text(wifiAP_ssid,wifiAP_SSID.c_str());
    lv_textarea_set_text(wifiAP_password,wifiAP_Password.c_str());

    wifiAP_ssid_label=lv_label_create(now_screen);
    lv_obj_set_style_text_font(wifiAP_ssid_label,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(wifiAP_ssid_label,text_color,LV_PART_MAIN);
    lv_obj_align_to(wifiAP_ssid_label,wifiAP_ssid,LV_ALIGN_TOP_LEFT,0,5);
    lv_label_set_text(wifiAP_ssid_label,wifiAP_SSID.c_str());

    wifiAP_password_label=lv_label_create(now_screen);
    lv_obj_set_style_text_font(wifiAP_password_label,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(wifiAP_password_label,text_color,LV_PART_MAIN);
    lv_obj_add_style(wifiAP_password_label,&style_chinese_16_white,0);
    lv_obj_align_to(wifiAP_password_label,wifiAP_password,LV_ALIGN_TOP_LEFT,0,5);
    lv_label_set_text(wifiAP_password_label,wifiAP_Password.c_str());

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t*wifiAP_switch=lv_switch_create(now_screen);
    lv_obj_align_to(wifiAP_switch,wifiAP_icon,LV_ALIGN_OUT_RIGHT_MID,140,0);
    

    
    if(wifiAP_s) 
    {
        lv_obj_add_state(wifiAP_switch,LV_STATE_CHECKED);
        lv_obj_add_flag(wifiAP_ssid, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifiAP_password, LV_OBJ_FLAG_HIDDEN);
        
        lv_label_set_text(setting_title,"WiFi热点(已开启)");
    }
    else 
    {
        lv_obj_add_flag(wifiAP_ssid_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifiAP_password_label, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(setting_title,"WiFi热点(已关闭)");
        lv_obj_clear_state(wifiAP_switch,LV_STATE_CHECKED);
    }
    


    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(wifiAP_switch,wifiAP_switch_event,LV_EVENT_VALUE_CHANGED,NULL);
    
    lv_obj_add_event_cb(wifiAP_ssid, wifiAP_password_change, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(wifiAP_password,wifiAP_password_change,LV_EVENT_ALL,kb);
    gesture(0,1,0,1);
    
    return now_screen;
}