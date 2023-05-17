#include "menu.h"
int menu_scroll_d=0;
void menu_event(lv_event_t * event)
{
    lv_obj_t*app=lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    load app_load=(load)lv_event_get_user_data(event);
    switch(code)
    {
        case LV_EVENT_CLICKED:
        {
            menu_scroll_d=lv_obj_get_scroll_y(now_screen);
            if(app_load==paint_load)app_open(app_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10,true,painting);
            else if(app_load==game1024_load) app_open(app_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,50);
            else app_open(app_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
        }
        break;
    }
}

void menu_back_task()
{
    menu_scroll_d=0;
}

lv_obj_t* menu_load()
{
    now_screen =lv_obj_create(NULL);
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xffffff);
        text_color=lv_color_hex(0x000000);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_hex(0xffffff);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    lv_obj_t*app_icon[15];
    app_icon[0]=lv_img_create(now_screen);
    app_icon[1]=lv_img_create(now_screen);
    app_icon[2]=lv_img_create(now_screen);
    app_icon[3]=lv_img_create(now_screen);
    app_icon[4]=lv_img_create(now_screen);
    app_icon[5]=lv_img_create(now_screen);
    app_icon[6]=lv_img_create(now_screen);
    app_icon[7]=lv_img_create(now_screen);
    app_icon[8]=lv_img_create(now_screen);
    app_icon[9]=lv_img_create(now_screen);
    app_icon[10]=lv_img_create(now_screen);
    app_icon[11]=lv_img_create(now_screen);
    app_icon[12]=lv_img_create(now_screen);
    app_icon[13]=lv_img_create(now_screen);
    app_icon[14]=lv_img_create(now_screen);

    //lv_obj_set_size(app_icon[0],80,60);


    lv_img_set_src(app_icon[0],&setting_icon);
    lv_img_set_src(app_icon[1],&clock_icon);
    lv_img_set_src(app_icon[2],&calendar_icon);
    lv_img_set_src(app_icon[3],&bilibili_icon);
    lv_img_set_src(app_icon[4],&calculator_icon);
    lv_img_set_src(app_icon[5],&photo_icon);
    lv_img_set_src(app_icon[6],&weather_icon);
    lv_img_set_src(app_icon[7],&filemanage_icon);
    lv_img_set_src(app_icon[8],&paint_icon);
    lv_img_set_src(app_icon[9],&upload_icon);
    lv_img_set_src(app_icon[10],&linkhome_icon);
    lv_img_set_src(app_icon[11],&remotecon_icon);
    lv_img_set_src(app_icon[12],&translate_icon);
    lv_img_set_src(app_icon[13],&sweep_icon);
    lv_img_set_src(app_icon[14],&game1024_icon);

    //lv_img_set_src(app_icon[0],"S:/system/app/setting/icon/setting.bin");

    lv_obj_align_to(app_icon[0],now_screen,LV_ALIGN_TOP_LEFT,0,40);
    lv_obj_align_to(app_icon[3],app_icon[0],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[6],app_icon[3],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[9],app_icon[6],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[12],app_icon[9],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);

    lv_obj_align_to(app_icon[1],now_screen,LV_ALIGN_TOP_MID,0,40);
    lv_obj_align_to(app_icon[4],app_icon[1],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[7],app_icon[4],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[10],app_icon[7],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[13],app_icon[10],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);

    lv_obj_align_to(app_icon[2],now_screen,LV_ALIGN_TOP_RIGHT,0,40);
    lv_obj_align_to(app_icon[5],app_icon[2],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[8],app_icon[5],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[11],app_icon[8],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);
    lv_obj_align_to(app_icon[14],app_icon[11],LV_ALIGN_OUT_BOTTOM_LEFT,0,40);


    lv_obj_t *bottom=lv_label_create(now_screen);
    lv_label_set_text(bottom,"");
    lv_obj_set_height(bottom,20);
    lv_obj_align_to(bottom,app_icon[12],LV_ALIGN_OUT_BOTTOM_MID,0,0);


    
    lv_obj_add_flag(app_icon[0],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[1],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[2],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[3],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[4],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[5],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[6],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[7],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[8],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[9],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[10],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[11],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[12],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[13],LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(app_icon[14],LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(app_icon[0],menu_event,LV_EVENT_ALL,(void*)setting_load);
    lv_obj_add_event_cb(app_icon[1],menu_event,LV_EVENT_ALL,(void*)time_load);
    lv_obj_add_event_cb(app_icon[2],menu_event,LV_EVENT_ALL,(void*)calendar_load);
    lv_obj_add_event_cb(app_icon[3],menu_event,LV_EVENT_ALL,(void*)bilibili_load);
    lv_obj_add_event_cb(app_icon[4],menu_event,LV_EVENT_ALL,(void*)calculator_load);
    lv_obj_add_event_cb(app_icon[5],menu_event,LV_EVENT_ALL,(void*)photo_load);
    lv_obj_add_event_cb(app_icon[6],menu_event,LV_EVENT_ALL,(void*)weather_load);
    lv_obj_add_event_cb(app_icon[7],menu_event,LV_EVENT_ALL,(void*)filemanage_load);
    lv_obj_add_event_cb(app_icon[8],menu_event,LV_EVENT_ALL,(void*)paint_load);
    lv_obj_add_event_cb(app_icon[9],menu_event,LV_EVENT_ALL,(void*)upload_load);
    lv_obj_add_event_cb(app_icon[10],menu_event,LV_EVENT_ALL,(void*)linkhome_load);
    lv_obj_add_event_cb(app_icon[11],menu_event,LV_EVENT_ALL,(void*)remotecontrol_load);
    lv_obj_add_event_cb(app_icon[12],menu_event,LV_EVENT_ALL,(void*)translate_load);
    lv_obj_add_event_cb(app_icon[13],menu_event,LV_EVENT_ALL,(void*)sweep_load);
    lv_obj_add_event_cb(app_icon[14],menu_event,LV_EVENT_ALL,(void*)game1024_load);

    lv_obj_scroll_to_y(now_screen,menu_scroll_d,LV_ANIM_OFF);
    gesture(0,0,0,1,menu_back_task);
    return now_screen;
}
