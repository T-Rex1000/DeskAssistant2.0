#include "home.h"
lv_obj_t *weather_display[2];
lv_obj_t *Home_show;
lv_timer_t *home_timer;
String home_bg_photo="/tree.bin";
void display_time_refresh(void*p)
{
    uint8_t second=0;
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        if((second!=time_date.tm_sec))
        {
            if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
            {
                lv_event_send(time_display[0],LV_EVENT_VALUE_CHANGED,NULL);
                xSemaphoreGive(lv_handler_lock);
            }
        }
        delay(100);
    }
    vTaskDelete(NULL);
}
void display_time_event(lv_event_t* event)
{
    //lv_obj_t *time_display[3];
    lv_obj_t*target=lv_event_get_target(event);
    //time_display[1]=lv_obj_get_child(time_display[0],0);
    lv_event_code_t event_code = lv_event_get_code(event);
    switch(event_code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            lv_label_set_text(time_display[0],(String(time_date.tm_hour)+":"+((time_date.tm_min<10)?("0"+String(time_date.tm_min)):(String(time_date.tm_min)))).c_str());
            lv_label_set_text(time_display[1],(String(time_date.tm_year+1900)+":"+String(time_date.tm_mon+1)+":"+String(time_date.tm_mday)).c_str());
            lv_obj_set_align(time_display[1],LV_ALIGN_BOTTOM_MID);
            lv_obj_set_style_text_align(time_display[0],LV_TEXT_ALIGN_RIGHT,0);
            lv_obj_align_to(time_display[0],time_display[2],LV_ALIGN_OUT_LEFT_TOP,-5,-10);
            switch (time_date.tm_wday)
            {
                case 0:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0xffffff),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
                    lv_label_set_text(time_display[3],"日");
                }
                break;
                case 1:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0xffc90e),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
                    lv_label_set_text(time_display[3],"一");
                }
                break;
                case 2:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0xff1111),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0xffffff),0);
                    lv_label_set_text(time_display[3],"二");
                }
                break;
                case 3:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0x99d9ea),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
                    lv_label_set_text(time_display[3],"三");
                }
                break;
                case 4:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0x11ff11),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
                    lv_label_set_text(time_display[3],"四");
                }
                break;
                case 5:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0x00a2e8),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
                    lv_label_set_text(time_display[3],"五");
                }
                break;
                case 6:
                {
                    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0xff7f27),0);
                    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
                    lv_label_set_text(time_display[3],"六");
                }
            break;
            }
            lv_obj_align(time_display[3],LV_ALIGN_CENTER,-1,1);
        }
        break;
        case LV_EVENT_CLICKED:
        {
            if(target==time_display[0])
            {
                if(home_show==home_show_PHOTO)
                {
                    lv_img_set_src(Home_show,"null");
                }
                lv_timer_del(home_timer);
                app_open(time_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,0);
            }
            if(target==time_display[1]||target==time_display[2])
            {
                if(home_show==home_show_PHOTO)
                {
                    lv_img_set_src(Home_show,"null");
                }
                lv_timer_del(home_timer);
                app_open(calendar_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,0);
            }
        }
        break;
    }
}
void gesture_home(lv_event_t* event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320)
        {
            if(home_show==home_show_PHOTO)
            {
                lv_img_set_src(Home_show,"null");
            }
            lv_timer_del(home_timer);
            app_open(menu_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
        }
    }
    else if(dir == LV_DIR_RIGHT)
    {
    }
    else if (dir == LV_DIR_TOP)
    {
    }
    else if (dir == LV_DIR_BOTTOM)
    {
        if(home_show==home_show_PHOTO)
        {
            lv_img_set_src(Home_show,"null");
        }
        lv_timer_del(home_timer);
        app_open(conpanel_load,LV_SCR_LOAD_ANIM_MOVE_BOTTOM,200,10);
    }
}
void home_weather_refresh(void*p)
{
    int oldcode=999;
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
        {
            lv_event_send(weather_display[1],LV_EVENT_VALUE_CHANGED,NULL);  
            xSemaphoreGive(lv_handler_lock);
        }
        if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
        {
            lv_event_send(weather_display[0],LV_EVENT_VALUE_CHANGED,NULL);
            
            xSemaphoreGive(lv_handler_lock);
        }
        if(!Delay(1000))break;
    }
    vTaskDelete(NULL);
}
void weather_icon_event(lv_event_t * p)
{
    lv_obj_t *target=lv_event_get_target(p);
    lv_event_code_t event_code = lv_event_get_code(p);
    switch(event_code) 
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            if(target==weather_display[1])
            {
                lv_label_set_text(weather_display[1],(weather.location_zh+" "+String(weather.temperature)+"°C").c_str());

                lv_obj_align_to(weather_display[1],weather_display[0],LV_ALIGN_OUT_BOTTOM_MID,0,0);
            }   
            else if(target==weather_display[0])
            {
                switch(weather.code)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 38:
                    {
                        lv_img_set_src(target,&sun);
                    }
                    break;
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    {
                        lv_img_set_src(target,&suncloudy);
                    }
                    break;
                    case 9:
                    {
                        lv_img_set_src(target,&cloudy);
                    }
                    break;
                    case 11:
                    case 12:
                    {
                        lv_img_set_src(target,&thunder);
                    }
                    break;
                    case 10:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    {
                        lv_img_set_src(target,&rain);
                    }
                    break;
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    {
                        lv_img_set_src(target,&snow);
                    }
                    break;
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    {
                        lv_img_set_src(target,&sand);
                    }
                    break;
                    case 30:
                    case 31:
                    {
                        lv_img_set_src(target,&cloudy);
                    }
                    break;
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    {
                        lv_img_set_src(target,&wind);
                    }
                    break;
                }
            }
        }
        break;
        case LV_EVENT_CLICKED:
        {
            if(home_show==home_show_PHOTO)
            {
                lv_img_set_src(Home_show,"null");
            }
            lv_timer_del(home_timer);
            app_open(weather_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
        }
        break;
    }  
}
void home_photo_event(lv_timer_t *e)
{
    lv_img_set_src(Home_show,("S:"+home_bg_photo).c_str());
    if((((lv_img_t*)Home_show)->h)>screenHeight||(((lv_img_t*)Home_show)->w)>screenWidth)
    {
        lv_img_set_src(Home_show,"NULL");
    }
    lv_obj_align(Home_show,LV_ALIGN_BOTTOM_MID,0,0);
}
void home_bilibili_event(lv_timer_t *e)
{
    lv_label_set_text(fans_num,String(bilibili.fans).c_str());
    lv_label_set_text(like_num,String(bilibili.likes).c_str());
    lv_label_set_text(video_num,String(bilibili.archive_count).c_str());
    lv_label_set_text(start_num,String(bilibili.follow).c_str());
    
    lv_obj_align_to(fans_num,fans_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(like_num,like_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(video_num,video_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
    lv_obj_align_to(start_num,start_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);

    //lv_obj_align(Home_show,LV_ALIGN_BOTTOM_MID,0,0);
}
lv_obj_t* home_load()
{
    now_screen = lv_obj_create(NULL);
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
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT); // obj背景色设成黄色
    lv_obj_set_size(now_screen, screenWidth, screenHeight);       			// 设置到屏幕大小
    if(home_show==home_show_PHOTO)
    {
        Home_show=lv_img_create(now_screen);
        lv_obj_align(Home_show,LV_ALIGN_BOTTOM_MID,0,0);
        lv_obj_add_flag(Home_show,LV_OBJ_FLAG_CLICKABLE);
        home_timer=lv_timer_create(home_photo_event,400,NULL);
    }
    else if(home_show==home_show_BILIBILI)
    {
        lv_color_t text_color;
        if(bilibili_theme==BILIBILI_PINK) text_color=lv_color_make(255,83,139);
        else if(bilibili_theme==BILIBILI_BLUE) text_color=lv_color_make(28,187,255);

        Home_show=lv_label_create(now_screen);
        lv_obj_set_style_bg_opa(Home_show, 255, LV_PART_MAIN);
        lv_obj_set_style_bg_color(Home_show,main_color,LV_PART_MAIN);
        lv_obj_set_size(Home_show,180,180);
        lv_obj_align(Home_show,LV_ALIGN_BOTTOM_MID,0,0);
        lv_obj_add_flag(Home_show,LV_OBJ_FLAG_CLICKABLE);
        lv_label_set_text(Home_show,"");
        
        fans_icon=lv_img_create(now_screen);
        like_icon=lv_img_create(now_screen);
        video_icon=lv_img_create(now_screen);
        start_icon=lv_img_create(now_screen);

        lv_img_set_src(fans_icon,&bilibili_fans);
        lv_img_set_src(like_icon,&bilibili_likes);
        lv_img_set_src(video_icon,&bilibili_views);
        lv_img_set_src(start_icon,&bilibili_follow);

        lv_obj_align_to(fans_icon,now_screen,LV_ALIGN_TOP_LEFT,35,135);
        lv_obj_align_to(like_icon,now_screen,LV_ALIGN_TOP_RIGHT,-35,135);
        lv_obj_align_to(video_icon,fans_icon,LV_ALIGN_OUT_BOTTOM_MID,0,40);
        lv_obj_align_to(start_icon,like_icon,LV_ALIGN_OUT_BOTTOM_MID,0,40);

        fans_num=lv_label_create(now_screen);
        like_num=lv_label_create(now_screen);
        video_num=lv_label_create(now_screen);
        start_num=lv_label_create(now_screen);

        lv_obj_set_style_text_color(fans_num,text_color,0);
        lv_obj_set_style_text_color(like_num,text_color,0);
        lv_obj_set_style_text_color(video_num,text_color,0);
        lv_obj_set_style_text_color(start_num,text_color,0);

        lv_label_set_text(fans_num,String(bilibili.fans).c_str());
        lv_label_set_text(like_num,String(bilibili.likes).c_str());
        lv_label_set_text(video_num,String(bilibili.archive_count).c_str());
        lv_label_set_text(start_num,String(bilibili.follow).c_str());

        lv_obj_align_to(fans_num,fans_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
        lv_obj_align_to(like_num,like_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
        lv_obj_align_to(video_num,video_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);
        lv_obj_align_to(start_num,start_icon,LV_ALIGN_OUT_BOTTOM_MID,0,5);

        home_timer=lv_timer_create(home_bilibili_event,500,NULL);
        xTaskCreatePinnedToCore(bilibili_update,NULL,6000,NULL,11,NULL,0);
    }

    time_display[0] = lv_label_create(now_screen);
    time_display[1] = lv_label_create(time_display[0]);
    time_display[2] = lv_btn_create(now_screen);
    time_display[3] = lv_label_create(time_display[2]);

    lv_obj_set_size(time_display[2],30,30);
    lv_obj_align_to(time_display[2],now_screen,LV_ALIGN_TOP_RIGHT,-7,60);
    lv_obj_align(time_display[3],LV_ALIGN_CENTER,0,1);
    lv_obj_set_style_bg_color(time_display[2],lv_color_hex(0xff7f27),0);
    lv_obj_set_style_text_font(time_display[3],&chinese_hei_16,0);
    lv_obj_set_style_text_color(time_display[3],lv_color_hex(0x000000),0);
    
    lv_obj_set_height(time_display[0],53);
    lv_obj_set_style_text_align(time_display[0],LV_TEXT_ALIGN_RIGHT,0);
    lv_obj_set_style_text_font(time_display[0],&lv_font_montserrat_38,0);
    lv_obj_set_style_text_color(time_display[0],text_color,0);
    lv_label_set_text(time_display[0], (String(0)+":"+String(0)).c_str());
    lv_obj_align_to(time_display[0],time_display[2],LV_ALIGN_OUT_LEFT_TOP,-5,-10);

    lv_obj_set_style_text_font(time_display[1],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(time_display[1],text_color,0);
    lv_label_set_text(time_display[0],(String(time_date.tm_hour)+":"+((time_date.tm_min<10)?("0"+String(time_date.tm_min)):(String(time_date.tm_min)))).c_str());
    lv_label_set_text(time_display[1],(String(time_date.tm_year+1900)+":"+String(time_date.tm_mon+1)+":"+String(time_date.tm_mday)).c_str());
    lv_obj_set_align(time_display[1],LV_ALIGN_BOTTOM_MID);

    lv_obj_add_flag(time_display[0], LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(time_display[1], LV_OBJ_FLAG_CLICKABLE);
    


    weather_display[0]=lv_img_create(now_screen);
    weather_display[1]=lv_label_create(now_screen);
    weather_code_icon(weather_display[0],weather.code);
    lv_obj_set_size(weather_display[0],86,60);
    lv_obj_set_pos(weather_display[0],10,30);
    lv_obj_add_flag(weather_display[0],LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_text_font(weather_display[1],&chinese_hei_16,0);
    lv_obj_set_style_text_color(weather_display[1],text_color,0);
    lv_obj_align_to(weather_display[1],weather_display[0],LV_ALIGN_OUT_BOTTOM_MID,0,0);
    lv_label_set_text(weather_display[1],(weather.location_zh+" "+String(weather.temperature)+"°C").c_str());



    //lv_obj_add_event_cb(Home_show,home_photo_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(time_display[0], display_time_event, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(time_display[1], display_time_event, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(time_display[2], display_time_event, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(weather_display[0], weather_icon_event, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(weather_display[1], weather_icon_event, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(now_screen,gesture_home,LV_EVENT_GESTURE,NULL);
    
    xTaskCreatePinnedToCore(time_update,NULL,4096,NULL,14,NULL,0);
    xTaskCreatePinnedToCore(display_time_refresh,NULL,4096,NULL,15,NULL,0);
    xTaskCreatePinnedToCore(weather_update,NULL,4096,NULL,10,NULL,0);
    xTaskCreatePinnedToCore(home_weather_refresh,NULL,4096,NULL,11,NULL,0);
    
    return now_screen;
}
