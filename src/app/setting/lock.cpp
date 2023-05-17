#include "app/setting/lock.h"

bool open_lock=0;
bool open_password=0;
String Password="";

lv_obj_t*password_text;
lv_obj_t*password_switch;
lv_obj_t*password_ta;
lv_obj_t*password_save;
void password_save_event(lv_event_t*e)
{
    tf.writeFile("/system/app/setting/config/lock.txt",(String(open_lock)+"\r\n"+String(open_password)+"\r\n"+Password+"\r\n").c_str());
    Password=lv_textarea_get_text(password_ta);
}
void password_change(lv_event_t*e)
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
        lv_textarea_set_text(target,Password.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}
void lock_switch_event(lv_event_t*e)
{
    open_lock=!open_lock;
    if(open_lock)
    {
        lv_obj_clear_flag(password_text, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(password_switch, LV_OBJ_FLAG_HIDDEN);
        if(open_password)
        {
            lv_obj_clear_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(password_save, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_state(password_switch,LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_state(password_switch,LV_STATE_CHECKED);
            lv_obj_add_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(password_save, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else
    {
        lv_obj_add_flag(password_text, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_switch, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_save, LV_OBJ_FLAG_HIDDEN);
    }
}
void password_switch_event(lv_event_t*e)
{
    open_password=!open_password;
    if(open_password)
    {
        lv_obj_clear_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(password_save, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_save, LV_OBJ_FLAG_HIDDEN);
    }
}
lv_obj_t * setting_lock_load()
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

    lv_obj_t*lock_icon=lv_label_create(now_screen);
    lv_obj_set_style_text_color(lock_icon,text_color,LV_PART_MAIN);
    lv_label_set_text(lock_icon,LV_SYMBOL_EYE_CLOSE);
    lv_obj_align(lock_icon,LV_ALIGN_TOP_LEFT,20,30);

    setting_title=lv_label_create(now_screen);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align_to(setting_title,lock_icon,LV_ALIGN_OUT_RIGHT_MID,10,0);
    lv_label_set_text(setting_title,"锁屏");

    lv_obj_t*lock_switch=lv_switch_create(now_screen);
    lv_obj_align_to(lock_switch,lock_icon,LV_ALIGN_OUT_RIGHT_MID,140,0);


    password_text=lv_label_create(now_screen);
    lv_obj_set_style_text_color(password_text,text_color,0);
    lv_obj_set_style_text_font(password_text,&chinese_hei_16,0);
    lv_obj_align_to(password_text,lock_icon,LV_ALIGN_OUT_BOTTOM_LEFT,0,30);
    lv_label_set_text(password_text,"密码");

    password_switch=lv_switch_create(now_screen);
    lv_obj_align_to(password_switch,lock_switch,LV_ALIGN_OUT_BOTTOM_MID,0,30);

    password_ta=lv_textarea_create(now_screen);
    lv_textarea_set_one_line(password_ta, true);
    //lv_obj_add_style(wifiAP_ssid,&style_chinese_16_white,0);
    lv_obj_set_style_text_font(password_ta,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(password_ta,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_size(password_ta,220,40);
    lv_obj_align_to(password_ta,password_text,LV_ALIGN_OUT_BOTTOM_LEFT,-10,30);
    lv_obj_clear_state(password_ta,LV_STATE_FOCUSED);
    
    lv_textarea_set_text(password_ta,Password.c_str());
    lv_textarea_set_password_mode(password_ta, true);


    password_save=lv_btn_create(now_screen);
    lv_obj_set_size(password_save,80,30);
    lv_obj_align_to(password_save,password_ta,LV_ALIGN_OUT_BOTTOM_MID,0,40);
    lv_obj_t *password_save_text=lv_label_create(password_save);
    lv_obj_add_style(password_save_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(password_save_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(password_save_text,"保存");

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    if(open_lock)
    {
        lv_obj_add_state(lock_switch,LV_STATE_CHECKED);
        if(open_password)
        {
            lv_obj_add_state(password_switch,LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_state(password_switch,LV_STATE_CHECKED);
            lv_obj_add_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(password_save, LV_OBJ_FLAG_HIDDEN); 
        }
    }
    else
    {
        lv_obj_add_flag(password_text, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_switch, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_ta, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(password_save, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(lock_switch,LV_STATE_CHECKED);
    }

    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(password_ta, password_change, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(lock_switch,lock_switch_event,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_add_event_cb(password_switch,password_switch_event,LV_EVENT_VALUE_CHANGED,NULL);
    lv_obj_add_event_cb(password_save,password_save_event,LV_EVENT_CLICKED,NULL);
    gesture(0,1,0,1);

    return now_screen;
}