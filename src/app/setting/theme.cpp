#include"app/setting/theme.h"


uint8_t Theme_color=Theme_color_BLACK;
uint8_t home_show=home_show_PHOTO;
uint8_t open_first=open_first_CALENDAR;

lv_obj_t *theme_opt;
lv_obj_t *home_opt;
lv_obj_t *open_first_opt;

void save_event(lv_event_t*e)
{
    home_show=lv_dropdown_get_selected(home_opt);
    open_first=lv_dropdown_get_selected(open_first_opt);
    tf.writeFile("/system/app/setting/config/theme.txt",(String(Theme_color)+"\r\n"+String(home_show)+"\r\n"+String(open_first)+"\r\n").c_str());

    if(Theme_color!=lv_dropdown_get_selected(theme_opt))
    {
        Theme_color=lv_dropdown_get_selected(theme_opt);
        tf.writeFile("/system/app/setting/config/theme.txt",(String(Theme_color)+"\r\n"+String(home_show)+"\r\n"+String(open_first)+"\r\n").c_str());
        lv_scr_load_anim(setting_theme_load(),LV_SCR_LOAD_ANIM_NONE,0,0,true);
    }
}
void lv_opt_event(lv_event_t* e)
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
lv_obj_t*setting_theme_load()
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

    lv_obj_t *theme_icon=lv_label_create(now_screen);
    lv_obj_set_style_text_color(theme_icon,text_color,LV_PART_MAIN);
    lv_label_set_text(theme_icon,LV_SYMBOL_SHUFFLE);
    lv_obj_align(theme_icon,LV_ALIGN_TOP_LEFT,20,30);

    setting_title=lv_label_create(now_screen);

    lv_obj_add_style(setting_title,&style_chinese_16_white,0);
    lv_obj_align_to(setting_title,theme_icon,LV_ALIGN_OUT_RIGHT_MID,10,0);
    lv_label_set_text(setting_title,"主题");

    lv_obj_t *theme_title=lv_label_create(now_screen);
    lv_obj_t *home_title=lv_label_create(now_screen);
    lv_obj_t *open_first_title=lv_label_create(now_screen);
    lv_obj_set_width(theme_title,110);
    lv_obj_set_width(home_title,110);
    lv_obj_set_width(open_first_title,110);
    lv_obj_set_style_text_color(theme_title,text_color,0);
    lv_obj_set_style_text_color(home_title,text_color,0);
    lv_obj_set_style_text_color(open_first_title,text_color,0);
    lv_obj_set_style_text_font(theme_title,&chinese_hei_16,0);
    lv_obj_set_style_text_font(home_title,&chinese_hei_16,0);
    lv_obj_set_style_text_font(open_first_title,&chinese_hei_16,0);

    lv_obj_align_to(theme_title,theme_icon,LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_obj_align_to(home_title,theme_title,LV_ALIGN_OUT_BOTTOM_MID,0,40);
    lv_obj_align_to(open_first_title,home_title,LV_ALIGN_OUT_BOTTOM_MID,0,40);

    lv_label_set_text(theme_title,"主题");
    lv_label_set_text(home_title,"主页显示");
    lv_label_set_text(open_first_title,"开机显示");

    theme_opt = lv_dropdown_create(now_screen);
    home_opt = lv_dropdown_create(now_screen);
    open_first_opt = lv_dropdown_create(now_screen);

    lv_dropdown_set_symbol(theme_opt,NULL);
    lv_dropdown_set_symbol(home_opt,NULL);
    lv_dropdown_set_symbol(open_first_opt,NULL);
    if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_border_color(theme_opt,lv_color_hex(0x222222),0);
        lv_obj_set_style_border_opa(theme_opt,255,0);
        lv_obj_set_style_border_width(theme_opt,2,0);
        lv_obj_set_style_border_color(home_opt,lv_color_hex(0x222222),0);
        lv_obj_set_style_border_opa(home_opt,255,0);
        lv_obj_set_style_border_width(home_opt,2,0);
        lv_obj_set_style_border_color(open_first_opt,lv_color_hex(0x222222),0);
        lv_obj_set_style_border_opa(open_first_opt,255,0);
        lv_obj_set_style_border_width(open_first_opt,2,0);

        lv_obj_set_style_bg_color(theme_opt,lv_color_hex(0x323232),LV_PART_MAIN);
        lv_obj_set_style_bg_color(home_opt,lv_color_hex(0x323232),LV_PART_MAIN);
        lv_obj_set_style_bg_color(open_first_opt,lv_color_hex(0x323232),LV_PART_MAIN);
    }
    else if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_border_color(theme_opt,lv_color_hex(0xdddddd),0);
        lv_obj_set_style_border_opa(theme_opt,255,0);
        lv_obj_set_style_border_width(theme_opt,2,0);
        lv_obj_set_style_border_color(home_opt,lv_color_hex(0xdddddd),0);
        lv_obj_set_style_border_opa(home_opt,255,0);
        lv_obj_set_style_border_width(home_opt,2,0);
        lv_obj_set_style_border_color(open_first_opt,lv_color_hex(0xdddddd),0);
        lv_obj_set_style_border_opa(open_first_opt,255,0);
        lv_obj_set_style_border_width(open_first_opt,2,0);

        lv_obj_set_style_bg_color(theme_opt,lv_color_hex(0xffffff),LV_PART_MAIN);
        lv_obj_set_style_bg_color(home_opt,lv_color_hex(0xffffff),LV_PART_MAIN);
        lv_obj_set_style_bg_color(open_first_opt,lv_color_hex(0xffffff),LV_PART_MAIN);
    }
    lv_obj_set_style_text_color(theme_opt,text_color,0);
    lv_obj_set_style_text_color(home_opt,text_color,0);
    lv_obj_set_style_text_color(open_first_opt,text_color,0);
    lv_obj_set_style_text_font(theme_opt,&chinese_hei_16,0);
    lv_obj_set_style_text_font(home_opt,&chinese_hei_16,0);
    lv_obj_set_style_text_font(open_first_opt,&chinese_hei_16,0);

    lv_dropdown_set_options_static(theme_opt, "非黑\n即白");
    lv_dropdown_set_options_static(home_opt, "相册\n哔哩哔哩");
    lv_dropdown_set_options_static(open_first_opt, "主页\n时钟\n天气\n日历\n哔哩哔哩\n计算器");

    lv_obj_set_size(theme_opt,90,35);
    lv_obj_set_size(home_opt,90,35);
    lv_obj_set_size(open_first_opt,90,35);
    
    lv_obj_align_to(theme_opt,theme_title,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align_to(home_opt,home_title,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align_to(open_first_opt,open_first_title,LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    lv_dropdown_set_selected(theme_opt,Theme_color);
    lv_dropdown_set_selected(home_opt,home_show);
    lv_dropdown_set_selected(open_first_opt,open_first);

    lv_obj_t *save=lv_btn_create(now_screen);
    lv_obj_set_size(save,50,30);
    lv_obj_align_to(save,setting_title,LV_ALIGN_OUT_RIGHT_MID,100,0);
    lv_obj_set_style_outline_opa(save, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(save, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(save, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *save_text=lv_label_create(save);
    lv_obj_add_style(save_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(save_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(save_text,"保存");


    lv_obj_add_event_cb(theme_opt, lv_opt_event, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(home_opt, lv_opt_event, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(open_first_opt, lv_opt_event, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(save,save_event,LV_EVENT_CLICKED,NULL);
    gesture(0,1,0,1);

    return now_screen;
}