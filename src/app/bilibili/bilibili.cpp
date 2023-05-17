#include"app/bilibili/bilibili.h"

lv_obj_t*bilibili_name;
lv_obj_t*bilibili_sign;
lv_obj_t*fans_num;
lv_obj_t*like_num;
lv_obj_t*video_num;
lv_obj_t*start_num;
lv_obj_t*Bilibili_icon;
lv_obj_t*fans_icon;
lv_obj_t*like_icon;
lv_obj_t*video_icon;
lv_obj_t*start_icon;
uint8_t bilibili_theme;
void gesture_bilibili(lv_event_t* event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
            app_open(bilibili_config_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
    }
    else if(dir == LV_DIR_RIGHT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320)
            //app_close(home_load,LV_SCR_LOAD_ANIM_MOVE_RIGHT,300,50);
            app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,10);
    }
    else if (dir == LV_DIR_TOP)
    {
    }
    else if (dir == LV_DIR_BOTTOM)
    {
        app_open(conpanel_load,LV_SCR_LOAD_ANIM_MOVE_BOTTOM,200,10);
    }
}
void bilibili_update(void*p)
{
    bool oldMODE=RUNMODE;
    delay(500);
    while(oldMODE==RUNMODE)
    {
        bilibili.update();
        if(!Delay(500))break;
    }
    vTaskDelete(NULL);
}
void bilibili_refresh(void*p)
{
    bool oldMODE=RUNMODE;
    delay(500);
    while(oldMODE==RUNMODE)
    {
        if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
        {
            lv_event_send(bilibili_name,LV_EVENT_VALUE_CHANGED,NULL);
            xSemaphoreGive(lv_handler_lock);
        }
        if(!Delay(200))break;
    }
    vTaskDelete(NULL);
}
void bilibili_event(lv_event_t*e)
{
    lv_obj_t*target=lv_event_get_target(e);
    lv_event_code_t event_code = lv_event_get_code(e);
    switch(event_code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            lv_label_set_text(bilibili_name,bilibili.name.c_str());
            lv_label_set_text(bilibili_sign,bilibili.sign.c_str());
            lv_label_set_text(fans_num,String(bilibili.fans).c_str());
            lv_label_set_text(like_num,String(bilibili.likes).c_str());
            lv_label_set_text(video_num,String(bilibili.archive_count).c_str());
            lv_label_set_text(start_num,String(bilibili.follow).c_str());
            
            lv_obj_align_to(bilibili_name,Bilibili_icon,LV_ALIGN_OUT_BOTTOM_MID,0,0);
            lv_obj_align_to(bilibili_sign,bilibili_name,LV_ALIGN_OUT_BOTTOM_MID,0,5);
            lv_obj_align_to(fans_num,fans_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
            lv_obj_align_to(like_num,like_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
            lv_obj_align_to(video_num,video_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
            lv_obj_align_to(start_num,start_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
        }
    }
}
lv_obj_t* bilibili_load()
{
    now_screen =lv_obj_create(NULL);
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        if(bilibili_theme==BILIBILI_PINK) text_color=lv_color_make(255,83,139);
        else if(bilibili_theme==BILIBILI_BLUE) text_color=lv_color_make(28,187,255);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        if(bilibili_theme==BILIBILI_PINK) text_color=lv_color_make(255,83,139);
        else if(bilibili_theme==BILIBILI_BLUE) text_color=lv_color_make(28,187,255);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    Bilibili_icon=lv_img_create(now_screen);
    fans_icon=lv_img_create(now_screen);
    like_icon=lv_img_create(now_screen);
    video_icon=lv_img_create(now_screen);
    start_icon=lv_img_create(now_screen);

    lv_img_set_src(Bilibili_icon,&bilibili_icon);
    lv_img_set_src(fans_icon,&bilibili_fans);
    lv_img_set_src(like_icon,&bilibili_likes);
    lv_img_set_src(video_icon,&bilibili_views);
    lv_img_set_src(start_icon,&bilibili_follow);

    lv_obj_align_to(Bilibili_icon,now_screen,LV_ALIGN_TOP_MID,0,20);
    lv_obj_align_to(fans_icon,now_screen,LV_ALIGN_TOP_LEFT,35,135);
    lv_obj_align_to(like_icon,now_screen,LV_ALIGN_TOP_RIGHT,-35,135);
    lv_obj_align_to(video_icon,fans_icon,LV_ALIGN_OUT_BOTTOM_MID,0,40);
    lv_obj_align_to(start_icon,like_icon,LV_ALIGN_OUT_BOTTOM_MID,0,40);

    bilibili_name=lv_label_create(now_screen);
    bilibili_sign=lv_label_create(now_screen);
    fans_num=lv_label_create(now_screen);
    like_num=lv_label_create(now_screen);
    video_num=lv_label_create(now_screen);
    start_num=lv_label_create(now_screen);

    lv_obj_set_style_text_font(bilibili_name,&chinese_hei_16,0);
    lv_obj_set_style_text_font(bilibili_sign,&chinese_hei_16,0);
    lv_obj_set_style_text_color(bilibili_name,text_color,0);
    if(Theme_color==Theme_color_BLACK)lv_obj_set_style_text_color(bilibili_sign,lv_color_make(255,255,255),0);
    else if(Theme_color==Theme_color_WHITE)lv_obj_set_style_text_color(bilibili_sign,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_color(fans_num,text_color,0);
    lv_obj_set_style_text_color(like_num,text_color,0);
    lv_obj_set_style_text_color(video_num,text_color,0);
    lv_obj_set_style_text_color(start_num,text_color,0);

    lv_label_set_text(bilibili_name,bilibili.name.c_str());
    lv_label_set_text(bilibili_sign,bilibili.sign.c_str());
    lv_label_set_text(fans_num,String(bilibili.fans).c_str());
    lv_label_set_text(like_num,String(bilibili.likes).c_str());
    lv_label_set_text(video_num,String(bilibili.archive_count).c_str());
    lv_label_set_text(start_num,String(bilibili.follow).c_str());

    lv_obj_align_to(bilibili_name,Bilibili_icon,LV_ALIGN_OUT_BOTTOM_MID,0,0);
    lv_obj_align_to(bilibili_sign,bilibili_name,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(fans_num,fans_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(like_num,like_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(video_num,video_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(start_num,start_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);


    lv_obj_add_event_cb(bilibili_name,bilibili_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(now_screen,gesture_bilibili,LV_EVENT_GESTURE,NULL);
    xTaskCreatePinnedToCore(bilibili_update,NULL,6000,NULL,10,NULL,0);
    xTaskCreatePinnedToCore(bilibili_refresh,NULL,6000,NULL,11,NULL,0);
    return now_screen;
}
lv_obj_t*UID_text;
lv_obj_t*bilibili_theme_opt;
bool uid_changed=0;
void UID_change(lv_event_t *e)
{
    int code = lv_event_get_code(e);
    lv_obj_t* target=lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(e);
    if(code==LV_EVENT_FOCUSED)
    {
        uid_changed=1;
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            lv_keyboard_set_mode(kb,LV_KEYBOARD_MODE_NUMBER);
            lv_textarea_set_accepted_chars(UID_text, "0123456789");
            lv_textarea_set_text(UID_text,bilibili.UID.c_str());
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
        lv_textarea_set_text(UID_text,bilibili.UID.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}
void bilibili_save_event(lv_event_t*e)
{
    if(uid_changed) 
    {
        bilibili.UID=lv_textarea_get_text(UID_text);
        bilibili.name="加载中";
    }
    if(bilibili_theme!=lv_dropdown_get_selected(bilibili_theme_opt))
    {
        bilibili_theme=lv_dropdown_get_selected(bilibili_theme_opt);
        lv_textarea_set_text(UID_text,bilibili.name.c_str());
    }
    tf.writeFile("/system/app/bilibili/config/UID.txt",(bilibili.UID+"\r\n"+String(bilibili_theme)+"\r\n").c_str());
    app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,50);
}
void bilibili_theme_opt_event(lv_event_t*e)
{
    lv_obj_t*dropdown=lv_event_get_target(e);
    lv_obj_t* list = lv_dropdown_get_list(dropdown);
    if (list != NULL)
    {
        if(Theme_color==Theme_color_BLACK)
        {
            lv_obj_set_style_bg_color(list,lv_color_hex(0x323232),LV_PART_MAIN);
            lv_obj_add_style(list,&style_chinese_16_white,LV_PART_MAIN);
            lv_obj_set_style_border_color(list,lv_color_hex(0x323232),0);
        }
        else if(Theme_color==Theme_color_WHITE)
        {
            lv_obj_set_style_bg_color(list,lv_color_hex(0xffffff),LV_PART_MAIN);
            lv_obj_add_style(list,&style_chinese_16_black,LV_PART_MAIN);
            lv_obj_set_style_border_color(list,lv_color_hex(0xdddddd),0);
        }
        lv_obj_set_style_border_opa(list,255,0);
        lv_obj_set_style_border_width(list,2,0);
    }
}
lv_obj_t*bilibili_config_load()
{
    uid_changed=0;
    now_screen =lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        if(bilibili_theme==BILIBILI_PINK) text_color=lv_color_make(255,83,139);
        else if(bilibili_theme==BILIBILI_BLUE) text_color=lv_color_make(28,187,255);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        if(bilibili_theme==BILIBILI_PINK) text_color=lv_color_make(255,83,139);
        else if(bilibili_theme==BILIBILI_BLUE) text_color=lv_color_make(28,187,255);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    setting_title=lv_label_create(now_screen);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align(setting_title,LV_ALIGN_TOP_LEFT,20,30);
    lv_label_set_text(setting_title,"设置");
    
    UID_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(UID_text, true);
    lv_obj_set_size(UID_text,140,40);
    lv_obj_set_style_text_font(UID_text, &chinese_hei_16, 0);
    lv_obj_align(UID_text, LV_ALIGN_TOP_LEFT, 90, 100);
    lv_textarea_set_text(UID_text,bilibili.name.c_str());
    
    lv_obj_t*UID_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(UID_label,text_color,0);
    lv_obj_set_style_text_font(UID_label, &lv_font_montserrat_16, 0);
    lv_label_set_text(UID_label,"UID:");
    lv_obj_align_to(UID_label,UID_text,LV_ALIGN_OUT_LEFT_MID,-30,0);
    
    lv_obj_t *theme_title=lv_label_create(now_screen);
    lv_obj_set_width(theme_title,110);
    lv_obj_set_style_text_color(theme_title,text_color,0);
    lv_obj_set_style_text_font(theme_title,&chinese_hei_16,0);
    lv_obj_align_to(theme_title,UID_label,LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(theme_title,"主题");
    bilibili_theme_opt = lv_dropdown_create(now_screen);
    lv_dropdown_set_symbol(bilibili_theme_opt,NULL);
    if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_border_color(bilibili_theme_opt,lv_color_hex(0x222222),0);
        lv_obj_set_style_border_opa(bilibili_theme_opt,255,0);
        lv_obj_set_style_border_width(bilibili_theme_opt,2,0);
        
        lv_obj_set_style_bg_color(bilibili_theme_opt,lv_color_hex(0x323232),LV_PART_MAIN);
    }
    else if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_border_color(bilibili_theme_opt,lv_color_hex(0xdddddd),0);
        lv_obj_set_style_border_opa(bilibili_theme_opt,255,0);
        lv_obj_set_style_border_width(bilibili_theme_opt,2,0);
        lv_obj_set_style_bg_color(bilibili_theme_opt,lv_color_hex(0xffffff),LV_PART_MAIN);
    }
    lv_obj_set_style_text_color(bilibili_theme_opt,text_color,0);
    lv_obj_set_style_text_font(bilibili_theme_opt,&chinese_hei_16,0);

    lv_dropdown_set_options_static(bilibili_theme_opt, "粉色\n蓝色");
    lv_obj_set_size(bilibili_theme_opt,90,35);
    lv_obj_align_to(bilibili_theme_opt,theme_title,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_dropdown_set_selected(bilibili_theme_opt,bilibili_theme);
    
    lv_obj_t *save=lv_btn_create(now_screen);
    lv_obj_set_size(save,50,30);
    lv_obj_align_to(save,setting_title,LV_ALIGN_OUT_RIGHT_MID,130,0);
    lv_obj_set_style_outline_opa(save, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(save, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(save, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *save_text=lv_label_create(save);
    lv_obj_add_style(save_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(save_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(save_text,"保存");

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    
    lv_obj_add_event_cb(bilibili_theme_opt, bilibili_theme_opt_event, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(UID_text,UID_change,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(save,bilibili_save_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);
    gesture(0,1,0,1);

    //xTaskCreatePinnedToCore(bilibili_update,NULL,8192,NULL,10,NULL,0);
    //xTaskCreatePinnedToCore(bilibili_refresh,NULL,4096,NULL,10,NULL,0);

    return now_screen;

}