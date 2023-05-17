#include "time.h"
#define second_long 80
struct tm time_date;
lv_obj_t * time_display[5];
void time_update(void *p)
{
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        getLocalTime(&time_date,10);
        delay(100);
    }
    vTaskDelete(NULL); 
}
float now_angle[3]={0,0,0};
lv_point_t target[3][2]; 
void display_time_task(void *p)
{
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        now_angle[0]+=((int)abs((time_date.tm_sec*6)-now_angle[0]+360+0.5)%360)/3;
        now_angle[1]+=((int)abs((time_date.tm_min*6)+time_date.tm_sec/10.0-now_angle[1]+360+0.5)%360)/5;
        now_angle[2]+=((int)abs((time_date.tm_hour%12)*30+time_date.tm_min/2.0-now_angle[2]+360+0.5)%360)/5;
        now_angle[0]=(int)(now_angle[0]+0.5)%360;
        now_angle[1]=(int)(now_angle[1]+0.5)%360;
        now_angle[2]=(int)(now_angle[2]+0.5)%360;
        
        target[0][1]={(short)(screenWidth/2+sin((float)now_angle[0]*3.1415/180.0)*(second_long)),(short)(screenHeight/2-cos((float)now_angle[0]*3.1415/180.0)*(second_long))};
        target[1][1]={(short)(screenWidth/2+sin((float)now_angle[1]*3.1415/180.0)*(second_long-10)),(short)(screenHeight/2-cos((float)now_angle[1]*3.1415/180.0)*(second_long-10))};
        target[2][1]={(short)(screenWidth/2+sin((float)now_angle[2]*3.1415/180.0)*(second_long-20)),(short)(screenHeight/2-cos((float)now_angle[2]*3.1415/180.0)*(second_long-20))};
        if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
        {
            lv_event_send(time_display[0],LV_EVENT_VALUE_CHANGED,NULL);
            xSemaphoreGive(lv_handler_lock);
        }
        delay(100);
    }
    vTaskDelete(NULL);
}
void time_refresh(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    switch(code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            lv_line_set_points(time_display[4],target[2],2);
            lv_line_set_points(time_display[3],target[1],2);
            lv_line_set_points(time_display[2],target[0],2);
            lv_label_set_text(time_display[0],(String(time_date.tm_hour)+":"+((time_date.tm_min<10)?("0"+String(time_date.tm_min)):(String(time_date.tm_min)))+":"+((time_date.tm_sec<10)?("0"+String(time_date.tm_sec)):(String(time_date.tm_sec)))).c_str());
        }
        break;
        case LV_EVENT_CLICKED:
        {
            //app_open(time_task_handle,HOME,home_load,LV_SCR_LOAD_ANIM_FADE_ON,500,10);
        }
        break;
    }
}

lv_obj_t* time_load()
{
    now_screen =lv_obj_create(NULL);
    if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_bg_color(now_screen,lv_color_hex(0xeeeeee), LV_STATE_DEFAULT);
        lv_obj_set_size(now_screen, screenWidth, screenHeight); 

        time_display[0]=lv_label_create(now_screen);
        lv_obj_set_style_text_font(time_display[0],&lv_font_montserrat_38,0);
        lv_label_set_text(time_display[0], (String(0)+":"+String(0)+":"+String(0)).c_str());
        lv_label_set_text(time_display[0],(String(time_date.tm_hour)+":"+((time_date.tm_min<10)?("0"+String(time_date.tm_min)):(String(time_date.tm_min)))+":"+((time_date.tm_sec<10)?("0"+String(time_date.tm_sec)):(String(time_date.tm_sec)))).c_str());
        
        lv_obj_set_style_text_color(time_display[0],lv_color_hex(0x2f8edd),0);
        lv_obj_set_align(time_display[0],LV_ALIGN_TOP_MID);
        lv_obj_set_y(time_display[0],270);

        time_display[1]=lv_arc_create(now_screen);
        lv_arc_set_bg_angles(time_display[1], 0, 360);
        lv_obj_set_size(time_display[1],200,200);
        lv_obj_set_align(time_display[1],LV_ALIGN_CENTER);
        lv_obj_remove_style(time_display[1], NULL, LV_PART_KNOB);
        lv_obj_clear_flag(time_display[1], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_color(time_display[1], lv_color_hex(0x2f8edd), LV_PART_INDICATOR);
        lv_obj_set_style_arc_color(time_display[1], lv_color_hex(0x2f8edd), LV_PART_MAIN);
        

        time_display[4]=lv_line_create(now_screen);
        time_display[3]=lv_line_create(now_screen);
        time_display[2]=lv_line_create(now_screen);
        lv_obj_set_pos(time_display[2],0,0);
        lv_obj_set_pos(time_display[3],0,0);
        lv_obj_set_pos(time_display[4],0,0);
        lv_obj_set_style_line_width(time_display[2],2,0);
        lv_obj_set_style_line_width(time_display[3],4,0);
        lv_obj_set_style_line_width(time_display[4],6,0);
        lv_obj_set_style_line_color(time_display[2], lv_color_hex(0x2f8edd), LV_PART_MAIN);
        lv_obj_set_style_line_color(time_display[3], lv_color_hex(0xaaaaaa), LV_PART_MAIN);
        lv_obj_set_style_line_color(time_display[4], lv_color_hex(0xcccccc), LV_PART_MAIN);
        lv_obj_set_style_line_rounded(time_display[2],true,LV_PART_MAIN);
        lv_obj_set_style_line_rounded(time_display[3],true,LV_PART_MAIN);
        lv_obj_set_style_line_rounded(time_display[4],true,LV_PART_MAIN);

        lv_obj_add_flag(time_display[0], LV_OBJ_FLAG_CLICKABLE);

        now_angle[0]=0;
        now_angle[1]=0;
        now_angle[2]=0;
        target[0][0]={screenWidth/2,screenHeight/2};
        target[1][0]={screenWidth/2,screenHeight/2};
        target[2][0]={screenWidth/2,screenHeight/2};
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_bg_color(now_screen,lv_color_hex(0x000000), LV_STATE_DEFAULT);
        lv_obj_set_size(now_screen, screenWidth, screenHeight); 

        time_display[0]=lv_label_create(now_screen);
        lv_obj_set_style_text_font(time_display[0],&lv_font_montserrat_38,0);
        lv_label_set_text(time_display[0], (String(0)+":"+String(0)+":"+String(0)).c_str());
        lv_label_set_text(time_display[0],(String(time_date.tm_hour)+":"+((time_date.tm_min<10)?("0"+String(time_date.tm_min)):(String(time_date.tm_min)))+":"+((time_date.tm_sec<10)?("0"+String(time_date.tm_sec)):(String(time_date.tm_sec)))).c_str());
        
        lv_obj_set_style_text_color(time_display[0],lv_color_hex(0x2f8edd),0);
        lv_obj_set_align(time_display[0],LV_ALIGN_TOP_MID);
        lv_obj_set_y(time_display[0],270);

        time_display[1]=lv_arc_create(now_screen);
        lv_arc_set_bg_angles(time_display[1], 0, 360);
        lv_obj_set_size(time_display[1],200,200);
        lv_obj_set_align(time_display[1],LV_ALIGN_CENTER);
        lv_obj_remove_style(time_display[1], NULL, LV_PART_KNOB);
        lv_obj_clear_flag(time_display[1], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_color(time_display[1], lv_color_hex(0x2f8edd), LV_PART_INDICATOR);
        lv_obj_set_style_arc_color(time_display[1], lv_color_hex(0x2f8edd), LV_PART_MAIN);
        

        time_display[4]=lv_line_create(now_screen);
        time_display[3]=lv_line_create(now_screen);
        time_display[2]=lv_line_create(now_screen);
        lv_obj_set_pos(time_display[2],0,0);
        lv_obj_set_pos(time_display[3],0,0);
        lv_obj_set_pos(time_display[4],0,0);
        lv_obj_set_style_line_width(time_display[2],2,0);
        lv_obj_set_style_line_width(time_display[3],4,0);
        lv_obj_set_style_line_width(time_display[4],6,0);
        lv_obj_set_style_line_color(time_display[2], lv_color_hex(0x2f8edd), LV_PART_MAIN);
        lv_obj_set_style_line_color(time_display[3], lv_color_hex(0xffffff), LV_PART_MAIN);
        lv_obj_set_style_line_color(time_display[4], lv_color_hex(0xeeeeee), LV_PART_MAIN);
        lv_obj_set_style_line_rounded(time_display[2],true,LV_PART_MAIN);
        lv_obj_set_style_line_rounded(time_display[3],true,LV_PART_MAIN);
        lv_obj_set_style_line_rounded(time_display[4],true,LV_PART_MAIN);

        lv_obj_add_flag(time_display[0], LV_OBJ_FLAG_CLICKABLE);

        now_angle[0]=0;
        now_angle[1]=0;
        now_angle[2]=0;
        target[0][0]={screenWidth/2,screenHeight/2};
        target[1][0]={screenWidth/2,screenHeight/2};
        target[2][0]={screenWidth/2,screenHeight/2};
    }
    lv_obj_add_event_cb(time_display[0],time_refresh,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(time_display[1],time_refresh,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(time_display[2],time_refresh,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(time_display[3],time_refresh,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(time_display[4],time_refresh,LV_EVENT_ALL,NULL);
    gesture(0,1,0,1);

    xTaskCreatePinnedToCore(time_update,NULL,4096,NULL,10,NULL,0);
    xTaskCreatePinnedToCore(display_time_task,NULL,4096,time_display,1,NULL,0);
    return now_screen;
}