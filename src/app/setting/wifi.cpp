#include "app/setting/setting.h"
#include "app/setting/wifi.h"
String WiFi_SSID="LCCWiFi";
String WiFi_Password="123123123";
bool wifi_s=1;
lv_obj_t*wifi_ssid;
lv_obj_t*wifi_password;
lv_obj_t*login;
lv_obj_t*ip;
lv_obj_t *refresh_icon;
void kb_event1(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * kb = lv_event_get_target(event);
    lv_obj_t * ta = lv_keyboard_get_textarea(kb);      // 获取与键盘绑定的输入缓冲区的对象
    if(code==LV_EVENT_READY)
    {
        lv_event_send(ta,LV_EVENT_VALUE_CHANGED,NULL);
        lv_keyboard_set_textarea(kb, NULL);                 // 取消键盘的关联
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // 添加键盘隐藏标志，隐藏键盘
    
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        
        lv_obj_scroll_to_y(now_screen,lv_obj_get_y(ta)+lv_obj_get_height(ta),LV_ANIM_ON);

    }
}

void wifi_password_change(lv_event_t *e)
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
        lv_textarea_set_text(wifi_ssid,WiFi.SSID().c_str());
        lv_textarea_set_text(wifi_password,WiFi_Password.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}
void wifi_login(lv_event_t*e)
{
    WiFi_SSID=lv_textarea_get_text(wifi_ssid);
    WiFi_Password=lv_textarea_get_text(wifi_password);
    tf.writeFile("/system/app/setting/config/wifi.txt",(WiFi_SSID+"\r\n"+WiFi_Password+"\r\n").c_str());
    WiFi.begin(WiFi_SSID.c_str(),WiFi_Password.c_str());
    lv_label_set_text(ip,("IP地址:  "+WiFi.gatewayIP().toString()).c_str());
    if(WiFi.status()==WL_CONNECTED)
    {
        lv_label_set_text(setting_title,"WiFi(已连接)");
    }
    else
    {
        lv_label_set_text(setting_title,"WiFi(未连接)");
    }
}
void ip_refresh(lv_event_t*e)
{
    lv_label_set_text(ip,("IP地址:  "+WiFi.gatewayIP().toString()).c_str());
    if(WiFi.status()==WL_CONNECTED)
    {
        lv_label_set_text(setting_title,"WiFi(已连接)");
    }
    else
    {
        lv_label_set_text(setting_title,"WiFi(未连接)");
    }
}
void wifi_switch_event(lv_event_t*e)
{
    wifi_s=!wifi_s;
    if(wifi_s)
    {
        lv_obj_clear_flag(wifi_ssid, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(wifi_password, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(login, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ip, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(refresh_icon, LV_OBJ_FLAG_HIDDEN);

        
        if(wifiAP_s) WiFi.mode(WIFI_AP_STA);
        else WiFi.mode(WIFI_STA);
        WiFi.begin(lv_textarea_get_text(wifi_ssid),lv_textarea_get_text(wifi_password));

        
        if(WiFi.status()==WL_CONNECTED)
        {
            lv_label_set_text(setting_title,"WiFi(已连接)");
        }
        else
        {
            lv_label_set_text(setting_title,"WiFi(未连接)");
        }
    }
    else
    {
        lv_obj_add_flag(wifi_ssid, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_password, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(login, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ip, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(refresh_icon, LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text(setting_title,"WiFi(已关闭)");
        if(wifiAP_s) WiFi.mode(WIFI_AP);
        else WiFi.mode(WIFI_OFF);
    }
}
lv_obj_t*setting_wifi_load()
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

    lv_obj_t *wifi_icon=lv_label_create(now_screen);
    lv_obj_set_style_text_color(wifi_icon,text_color,LV_PART_MAIN);
    lv_label_set_text(wifi_icon,LV_SYMBOL_WIFI);
    lv_obj_align(wifi_icon,LV_ALIGN_TOP_LEFT,20,30);

    setting_title=lv_label_create(now_screen);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align_to(setting_title,wifi_icon,LV_ALIGN_OUT_RIGHT_MID,10,0);
    
    

    wifi_ssid=lv_textarea_create(now_screen);
    lv_textarea_set_one_line(wifi_ssid, true);
    //lv_obj_add_style(wifi_ssid,&style_chinese_16_white,0);
    lv_obj_set_style_text_font(wifi_ssid,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(wifi_ssid,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_size(wifi_ssid,220,40);
    lv_obj_align_to(wifi_ssid,wifi_icon,LV_ALIGN_OUT_BOTTOM_LEFT,-10,30);
    lv_obj_clear_state(wifi_ssid,LV_STATE_FOCUSED);
    
    wifi_password=lv_textarea_create(now_screen);
    lv_textarea_set_one_line(wifi_password, true);
    lv_obj_add_style(wifi_password,&style_chinese_16_white,0);
    lv_obj_set_style_text_font(wifi_password,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(wifi_password,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_size(wifi_password,220,40);
    lv_obj_align_to(wifi_password,wifi_ssid,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_clear_state(wifi_password,LV_STATE_FOCUSED);
    
    ip =lv_label_create(now_screen);
    lv_obj_set_size(ip,220,40);
    lv_obj_add_style(ip,&style_chinese_16_white,0);
    lv_obj_align_to(ip,wifi_password,LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    lv_obj_set_style_text_color(ip,text_color,LV_PART_MAIN);
    lv_label_set_text(ip,("IP地址:  "+WiFi.gatewayIP().toString()).c_str());

    refresh_icon=lv_label_create(now_screen);
    lv_obj_set_style_text_color(refresh_icon,text_color,LV_PART_MAIN);
    lv_obj_set_size(refresh_icon,40,40);

    login=lv_btn_create(now_screen);
    lv_obj_set_size(login,80,30);
    lv_obj_align_to(login,wifi_password,LV_ALIGN_OUT_BOTTOM_MID,0,40);
    lv_obj_t *login_text=lv_label_create(login);
    lv_obj_add_style(login_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(login_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(login_text,"连接");

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t*wifi_switch=lv_switch_create(now_screen);
    lv_obj_align_to(wifi_switch,wifi_icon,LV_ALIGN_OUT_RIGHT_MID,140,0);
    
    
    lv_label_set_text(refresh_icon,LV_SYMBOL_REFRESH);
    lv_obj_align_to(refresh_icon,ip,LV_ALIGN_OUT_RIGHT_MID,-30,0);
    lv_obj_add_flag(refresh_icon,LV_OBJ_FLAG_CLICKABLE);

    
    if(wifi_s) 
    {
        lv_textarea_set_text(wifi_ssid,WiFi_SSID.c_str());
        lv_textarea_set_text(wifi_password,WiFi_Password.c_str());
        lv_textarea_set_password_mode(wifi_password, true);
        lv_obj_add_state(wifi_switch,LV_STATE_CHECKED);
        
        if(WiFi.status()==WL_CONNECTED)
        {
            lv_label_set_text(setting_title,"WiFi(已连接)");
        }
        else
        {
            lv_label_set_text(setting_title,"WiFi(未连接)");
        }
    }
    else 
    {
        lv_obj_add_flag(wifi_ssid, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(wifi_password, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ip, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(login, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(refresh_icon, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(setting_title,"WiFi(已关闭)");
        lv_obj_clear_state(wifi_switch,LV_STATE_CHECKED);
    }
    


    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(wifi_switch,wifi_switch_event,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_add_event_cb(refresh_icon,ip_refresh,LV_EVENT_CLICKED,NULL);
    
    lv_obj_add_event_cb(wifi_ssid, wifi_password_change, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(wifi_password,wifi_password_change,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(login,wifi_login,LV_EVENT_CLICKED,NULL);
    gesture(0,1,0,1);
    
    return now_screen;
}