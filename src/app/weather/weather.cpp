#include "weather.h"
lv_obj_t*weather_ico[4];
lv_obj_t*weather_temp[7];
lv_obj_t*weather_day[3];
bool Delay(uint32_t time)
{
    time/=10;
    time*=10;
    bool oldMODE=RUNMODE;
    while(time>0)
    {
        delay(10);
        time-=10;
        if(oldMODE!=RUNMODE)
        {
            return false;
        }
    }
    return true;
}
void weather_update(void*p)
{
    uint8_t wait_time=5;
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        if(weather.update())
            wait_time=100;
        if(!Delay(100*wait_time)) break;
        Serial.println(weather.location);
    }
    vTaskDelete(NULL);
}


void weather_refresh(void*p)
{
    int code[4]={0,0,0,0};
    int oldcode[4]={-1,-1,-1,-1};
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        code[0]=weather.code;
        code[1]=weather.forecast_code_day[0];
        code[2]=weather.forecast_code_day[1];
        code[3]=weather.forecast_code_day[2];
        if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
        {
            lv_event_send(weather_temp[0],LV_EVENT_VALUE_CHANGED,NULL);
            xSemaphoreGive(lv_handler_lock);
        }
        for(uint8_t i=0;i<4;i++)
        {
            if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
            {
                oldcode[i]=code[i];
                lv_event_send(weather_ico[i],LV_EVENT_VALUE_CHANGED,&code[i]);
                xSemaphoreGive(lv_handler_lock);
            }
            delay(20);
        }
        if(!Delay(1000))break;
    }
    vTaskDelete(NULL);
}
void weather_code_icon(lv_obj_t*icon,int code)
{
    switch(code)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 38:
        {
            lv_img_set_src(icon,&sun);
        }
        break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        {
            lv_img_set_src(icon,&suncloudy);
        }
        break;
        case 9:
        {
            lv_img_set_src(icon,&cloudy);
        }
        break;
        case 11:
        case 12:
        {
            lv_img_set_src(icon,&thunder);
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
            lv_img_set_src(icon,&rain);
        }
        break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        {
            lv_img_set_src(icon,&snow);
        }
        break;
        case 26:
        case 27:
        case 28:
        case 29:
        {
            lv_img_set_src(icon,&sand);
        }
        break;
        case 30:
        case 31:
        {
            lv_img_set_src(icon,&cloudy);
        }
        break;
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        {
            lv_img_set_src(icon,&wind);
        }
        break;
        default:
            lv_img_set_src(icon,&suncloudy);
    }
}
void weather_icons_event(lv_event_t* event)
{
    lv_event_code_t code=lv_event_get_code(event);
    lv_obj_t*weather_icon_t=lv_event_get_target(event);
    switch(code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            if(weather_icon_t==weather_ico[0]||weather_icon_t==weather_ico[1]||weather_icon_t==weather_ico[2]||weather_icon_t==weather_ico[3])
            {
                int weather_code=*((int*)lv_event_get_user_data(event));
                weather_code_icon(weather_icon_t,weather_code);
            }
            else 
            {
                lv_label_set_text(weather_temp[0],(String(weather.temperature)+"°C "+weather.location_zh).c_str());
                lv_label_set_text(weather_temp[1],(String(weather.forecast_high[0])).c_str());
                lv_label_set_text(weather_temp[2],(String(weather.forecast_low[0])).c_str());
                lv_label_set_text(weather_temp[3],(String(weather.forecast_high[1])).c_str());
                lv_label_set_text(weather_temp[4],(String(weather.forecast_low[1])).c_str());
                lv_label_set_text(weather_temp[5],(String(weather.forecast_high[2])).c_str());
                lv_label_set_text(weather_temp[6],(String(weather.forecast_low[2])).c_str());

                lv_obj_align_to(weather_temp[0],weather_ico[0],LV_ALIGN_OUT_BOTTOM_MID,0,0);
                lv_obj_align_to(weather_temp[1],weather_ico[1],LV_ALIGN_OUT_RIGHT_TOP,30,10);
                lv_obj_align_to(weather_temp[2],weather_ico[1],LV_ALIGN_OUT_RIGHT_BOTTOM,30,-10);
                lv_obj_align_to(weather_temp[3],weather_ico[2],LV_ALIGN_OUT_RIGHT_TOP,30,10);
                lv_obj_align_to(weather_temp[4],weather_ico[2],LV_ALIGN_OUT_RIGHT_BOTTOM,30,-10);
                lv_obj_align_to(weather_temp[5],weather_ico[3],LV_ALIGN_OUT_RIGHT_TOP,30,10);
                lv_obj_align_to(weather_temp[6],weather_ico[3],LV_ALIGN_OUT_RIGHT_BOTTOM,30,-10);
                
                lv_label_set_text(weather_day[0],(String(time_date.tm_mday)).c_str());
                lv_label_set_text(weather_day[1],(String(time_date.tm_mday+1)).c_str());
                lv_label_set_text(weather_day[2],(String(time_date.tm_mday+2)).c_str());
                
                lv_obj_set_style_text_align(weather_day[0],LV_TEXT_ALIGN_CENTER,0);
                lv_obj_set_style_text_align(weather_day[1],LV_TEXT_ALIGN_CENTER,0);
                lv_obj_set_style_text_align(weather_day[2],LV_TEXT_ALIGN_CENTER,0);
            }
        }
        break;
    }
}

void gesture_weather(lv_event_t* event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
            app_open(weather_config_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,50);
    }
    else if(dir == LV_DIR_RIGHT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320)
            //app_close(home_load,LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,50);
            app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,50);
    }
    else if (dir == LV_DIR_TOP)
    {
    }
    else if (dir == LV_DIR_BOTTOM)
    {
        app_open(conpanel_load,LV_SCR_LOAD_ANIM_MOVE_BOTTOM,200,10);
    }
}


lv_obj_t* weather_load()
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

    weather_ico[0]=lv_img_create(now_screen);
    weather_ico[1]=lv_img_create(now_screen);
    weather_ico[2]=lv_img_create(now_screen);
    weather_ico[3]=lv_img_create(now_screen);

    lv_obj_set_size(weather_ico[0],90,60);
    lv_obj_set_size(weather_ico[1],90,60);
    lv_obj_set_size(weather_ico[2],90,60);
    lv_obj_set_size(weather_ico[3],90,60);

    lv_obj_set_align(weather_ico[0],LV_ALIGN_TOP_MID);
    lv_obj_set_y(weather_ico[0],10);
    lv_obj_set_align(weather_ico[1],LV_ALIGN_TOP_LEFT);
    lv_obj_set_y(weather_ico[1],110);
    lv_obj_align_to(weather_ico[2],weather_ico[1],LV_ALIGN_TOP_LEFT,0,65);
    lv_obj_align_to(weather_ico[3],weather_ico[2],LV_ALIGN_TOP_LEFT,0,65);

    lv_obj_add_event_cb(weather_ico[0],weather_icons_event,LV_EVENT_ALL,&weather.code);
    lv_obj_add_event_cb(weather_ico[1],weather_icons_event,LV_EVENT_ALL,&weather.forecast_code_day[0]);
    lv_obj_add_event_cb(weather_ico[2],weather_icons_event,LV_EVENT_ALL,&weather.forecast_code_day[1]);
    lv_obj_add_event_cb(weather_ico[3],weather_icons_event,LV_EVENT_ALL,&weather.forecast_code_day[2]);


    weather_temp[0]=lv_label_create(now_screen);
    weather_temp[1]=lv_label_create(now_screen);
    weather_temp[2]=lv_label_create(now_screen);
    weather_temp[3]=lv_label_create(now_screen);
    weather_temp[4]=lv_label_create(now_screen);
    weather_temp[5]=lv_label_create(now_screen);
    weather_temp[6]=lv_label_create(now_screen);

    lv_obj_set_style_text_font(weather_temp[0],&chinese_hei_16,0);
    //lv_obj_set_style_text_font(weather_temp[0],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_font(weather_temp[1],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_font(weather_temp[2],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_font(weather_temp[3],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_font(weather_temp[4],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_font(weather_temp[5],&lv_font_montserrat_16,0);
    lv_obj_set_style_text_font(weather_temp[6],&lv_font_montserrat_16,0);
    
    lv_obj_set_style_text_color(weather_temp[0],text_color,0);
    lv_obj_set_style_text_color(weather_temp[1],lv_color_hex(0xff7f27),0);
    lv_obj_set_style_text_color(weather_temp[2],lv_color_hex(0x00a2e8),0);
    lv_obj_set_style_text_color(weather_temp[3],lv_color_hex(0xff7f27),0);
    lv_obj_set_style_text_color(weather_temp[4],lv_color_hex(0x00a2e8),0);
    lv_obj_set_style_text_color(weather_temp[5],lv_color_hex(0xff7f27),0);
    lv_obj_set_style_text_color(weather_temp[6],lv_color_hex(0x00a2e8),0);

    lv_label_set_text(weather_temp[0],(String(weather.temperature)+"°C "+weather.location).c_str());
    lv_label_set_text(weather_temp[1],(String(weather.forecast_high[0])).c_str());
    lv_label_set_text(weather_temp[2],(String(weather.forecast_low[0])).c_str());
    lv_label_set_text(weather_temp[3],(String(weather.forecast_high[1])).c_str());
    lv_label_set_text(weather_temp[4],(String(weather.forecast_low[1])).c_str());
    lv_label_set_text(weather_temp[5],(String(weather.forecast_high[2])).c_str());
    lv_label_set_text(weather_temp[6],(String(weather.forecast_low[2])).c_str());

    lv_obj_align_to(weather_temp[0],weather_ico[0],LV_ALIGN_OUT_BOTTOM_MID,0,0);
    lv_obj_align_to(weather_temp[1],weather_ico[1],LV_ALIGN_OUT_RIGHT_TOP,30,10);
    lv_obj_align_to(weather_temp[2],weather_ico[1],LV_ALIGN_OUT_RIGHT_BOTTOM,30,-10);
    lv_obj_align_to(weather_temp[3],weather_ico[2],LV_ALIGN_OUT_RIGHT_TOP,30,10);
    lv_obj_align_to(weather_temp[4],weather_ico[2],LV_ALIGN_OUT_RIGHT_BOTTOM,30,-10);
    lv_obj_align_to(weather_temp[5],weather_ico[3],LV_ALIGN_OUT_RIGHT_TOP,30,10);
    lv_obj_align_to(weather_temp[6],weather_ico[3],LV_ALIGN_OUT_RIGHT_BOTTOM,30,-10);


    weather_day[0]=lv_label_create(now_screen);
    weather_day[1]=lv_label_create(now_screen);
    weather_day[2]=lv_label_create(now_screen);

    lv_obj_set_width(weather_day[0],70);
    lv_obj_set_width(weather_day[1],70);
    lv_obj_set_width(weather_day[2],70);

    lv_obj_set_style_text_font(weather_day[0],&lv_font_montserrat_38,0);
    lv_obj_set_style_text_font(weather_day[1],&lv_font_montserrat_38,0);
    lv_obj_set_style_text_font(weather_day[2],&lv_font_montserrat_38,0);
    
    lv_obj_set_style_text_color(weather_day[0],text_color,0);
    lv_obj_set_style_text_color(weather_day[1],text_color,0);
    lv_obj_set_style_text_color(weather_day[2],text_color,0);
    
    lv_label_set_text(weather_day[0],(String(time_date.tm_mday)).c_str());
    lv_label_set_text(weather_day[1],(String(time_date.tm_mday+1)).c_str());
    lv_label_set_text(weather_day[2],(String(time_date.tm_mday+2)).c_str());

    lv_obj_set_style_text_align(weather_day[0],LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(weather_day[1],LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(weather_day[2],LV_TEXT_ALIGN_CENTER,0);
    
    lv_obj_align_to(weather_day[0],weather_ico[1],LV_ALIGN_OUT_RIGHT_MID,80,0);
    lv_obj_align_to(weather_day[1],weather_ico[2],LV_ALIGN_OUT_RIGHT_MID,80,0);
    lv_obj_align_to(weather_day[2],weather_ico[3],LV_ALIGN_OUT_RIGHT_MID,80,0);
    lv_obj_add_event_cb(weather_temp[0],weather_icons_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(now_screen,gesture_weather,LV_EVENT_GESTURE,NULL);

    xTaskCreatePinnedToCore(weather_update,NULL,4096,NULL,10,NULL,0);
    xTaskCreatePinnedToCore(weather_refresh,NULL,4096,weather_ico,11,NULL,0);

    return now_screen;
}
bool location_changed=0;
lv_obj_t * location_text;
lv_obj_t * key_text;
static void location_text_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * text = lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(e);

    if(code == LV_EVENT_FOCUSED) 
    {
        location_changed=1;
        lv_obj_set_style_text_font(location_text, &lv_font_montserrat_16, 0);
        lv_textarea_set_text(location_text,weather.location.c_str());
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            lv_keyboard_set_textarea(kb, location_text);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if(code==LV_EVENT_DEFOCUSED)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(location_text, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, text);   /*To forget the last clicked object to make it focusable again*/
    }
    else if(code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(location_text, LV_STATE_FOCUSED);
        lv_obj_set_style_text_font(location_text, &lv_font_montserrat_16, 0);
        lv_textarea_set_text(location_text,weather.location.c_str());
        lv_indev_reset(NULL, location_text);   /*To forget the last clicked object to make it focusable again*/
    }
}
void key_text_event_cb(lv_event_t*event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * text = lv_event_get_target(event);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(event);
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_textarea_set_text(key_text,weather._reqUserKey.c_str());
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            lv_keyboard_set_textarea(kb, key_text);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else if(code==LV_EVENT_DEFOCUSED||code == LV_EVENT_CANCEL)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(key_text, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, key_text);   /*To forget the last clicked object to make it focusable again*/

        
        String password_t="";
        for(uint8_t i=0;i<weather._reqUserKey.length();i++)
        {
            password_t+="*";
        }
        lv_textarea_set_text(key_text,password_t.c_str());
    }
}

void gesture_weather_config(lv_event_t *event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
    }
    else if(dir == LV_DIR_RIGHT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320)
            app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,50);
    }
    else if (dir == LV_DIR_TOP)
    {
    }
    else if (dir == LV_DIR_BOTTOM)
    {
    }
}
String temp_key="";
void kb_event(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * kb = lv_event_get_target(event);
    lv_obj_t * ta = lv_keyboard_get_textarea(kb);      // 获取与键盘绑定的输入缓冲区的对象
    if(code==LV_EVENT_READY)
    {
        if(ta==key_text)
        {
            temp_key=lv_textarea_get_text(key_text);
        }
        String password_text="";
        for(uint8_t i=0;i<strlen(lv_textarea_get_text(key_text));i++)
        {
            password_text+="*";
        }
        lv_textarea_set_text(key_text,password_text.c_str());
        
        lv_keyboard_set_textarea(kb, NULL);                 // 取消键盘的关联
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // 添加键盘隐藏标志，隐藏键盘
    
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
    }
}
void weather_save_event(lv_event_t*e)
{
    weather.location_zh="加载中";
    if(location_changed) 
    {
        tf.writeFile("/system/app/weather/config/location.txt",(String(lv_textarea_get_text(location_text))+"\r\n").c_str());
        if(temp_key!="")tf.writeFile("/system/app/weather/config/key.txt",(temp_key+"\r\n").c_str());
        weather.config(lv_textarea_get_text(location_text),temp_key);
    }
    else 
    {
        if(temp_key!="")tf.writeFile("/system/app/weather/config/key.txt",(temp_key+"\r\n").c_str());
        weather.config("",temp_key);
    }
    Serial.println(lv_textarea_get_text(location_text));
    Serial.println(temp_key);
    app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,50);
}
lv_obj_t*weather_config_load()
{
    location_changed=0;
    now_screen =lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
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

    setting_title=lv_label_create(now_screen);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align(setting_title,LV_ALIGN_TOP_LEFT,20,30);
    lv_label_set_text(setting_title,"设置");

    location_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(location_text, true);
    lv_obj_set_size(location_text,170,40);
    lv_obj_set_style_text_font(location_text, &lv_font_montserrat_16, 0);
    lv_obj_align(location_text, LV_ALIGN_TOP_LEFT, 50, 60);
    //lv_textarea_set_accepted_chars(location_text, "abcdefghijklmnopqrstuvwxyz");

    key_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(key_text, true);
    lv_obj_set_size(key_text,170,40);
    lv_obj_set_style_text_font(key_text, &lv_font_montserrat_16, 0);
    lv_obj_align_to(key_text,location_text, LV_ALIGN_BOTTOM_MID, 0, 75);

    lv_obj_t *location_logo=lv_label_create(now_screen);
    lv_obj_set_style_text_color(location_logo,text_color,0);
    lv_label_set_text(location_logo,LV_SYMBOL_GPS);
    lv_obj_align_to(location_logo,location_text,LV_ALIGN_OUT_LEFT_MID,-20,0);

    lv_obj_t *key_logo=lv_label_create(now_screen);
    lv_obj_set_style_text_color(key_logo,text_color,0);
    lv_label_set_text(key_logo,LV_SYMBOL_EYE_CLOSE);
    lv_obj_align_to(key_logo,key_text,LV_ALIGN_OUT_LEFT_MID,-20,0);

    
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
    
    
    //lv_obj_add_event_cb(refresh_location,refresh_location_event,LV_EVENT_CLICKED,location_text);
    
    lv_obj_add_event_cb(save,weather_save_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(location_text, location_text_event_cb, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(key_text, key_text_event_cb, LV_EVENT_ALL, kb);
    gesture(0,1,0,1);
    
    lv_obj_add_event_cb(kb,kb_event,LV_EVENT_ALL,NULL);

    lv_obj_set_style_text_font(location_text, &chinese_hei_16, 0);
    lv_textarea_set_text(location_text,weather.location_zh.c_str());
    String password_text="";
    for(uint8_t i=0;i<weather._reqUserKey.length();i++)
    {
        password_text+="*";
    }
    lv_textarea_set_text(key_text,password_text.c_str());
    
    return now_screen;
}
