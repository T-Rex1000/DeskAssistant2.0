#include "gui.h"

bool RUNMODE=0;
lv_obj_t *now_screen;
bool use_lvgl=true;

bool gesture_top=false;
bool gesture_bottom=false;
bool gesture_left=false;
bool gesture_right=false;

typedef struct APP_CHAINS
{
    struct APP_CHAINS*next;
    struct APP_CHAINS*last;
    load app_load;
}App_chains;
App_chains *app_chains;

void gui_init(uint8_t runMODE)
{
    app_chains=(App_chains*)malloc(sizeof(App_chains));
    app_chains->last=NULL;
    app_chains->next=NULL;
    app_chains->app_load=home_load;
    switch(runMODE)
    {
        case TIME:
        {
            app_open(time_load,LV_SCR_LOAD_ANIM_NONE,0,10);
        }
        break;
        case WEATHER:
        {
            app_open(weather_load,LV_SCR_LOAD_ANIM_NONE,0,10);
        }
        break;
        case CALENDAR:
        {
            app_open(calendar_load,LV_SCR_LOAD_ANIM_NONE,0,10);
        }
        break;
        case BILIBILI:
        {
            app_open(bilibili_load,LV_SCR_LOAD_ANIM_NONE,0,10);
        }
        break;
        case CALCULATOR:
        {
            app_open(calculator_load,LV_SCR_LOAD_ANIM_NONE,0,10);
        }
        break;
        default:
            lv_scr_load(app_chains->app_load());
    }
}

void app_open(load app_load,lv_scr_load_anim_t anim_type,uint32_t time,uint32_t delay_time,bool auto_del,void (*Other_app)())
{
    RUNMODE=!RUNMODE;
    app_chains->next=(App_chains*)malloc(sizeof(App_chains));
    app_chains->next->last=app_chains;
    app_chains->next->next=NULL;
    app_chains->next->app_load=app_load;
    app_chains=app_chains->next;

    gesture_bottom=false;
    gesture_left=false;
    gesture_right=false;
    gesture_top=false;

    lv_scr_load_anim((*app_chains->app_load)(),anim_type,time,delay_time,auto_del);

    if(Other_app!=NULL)
    {
        other_app=Other_app;
        use_lvgl=0;
    }
}

void app_return(lv_scr_load_anim_t anim_type,uint32_t time,uint32_t delay_time,bool auto_del)
{
    RUNMODE=!RUNMODE;
    App_chains *temp;
    temp=app_chains;
    app_chains=app_chains->last;
    app_chains->next=NULL;
    free(temp);

    gesture_bottom=false;
    gesture_left=false;
    gesture_right=false;
    gesture_top=false;

    lv_scr_load_anim((*app_chains->app_load)(),anim_type,time,delay_time,auto_del);
}

void null_func()
{

}

void gesture_classical(lv_event_t*event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir=lv_indev_get_gesture_dir(lv_indev_get_act());

    load back_task=(load)lv_event_get_user_data(event);

    if(gesture_left&&dir==LV_DIR_LEFT)
    {

    }
    else if(gesture_right&&dir==LV_DIR_RIGHT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320)
        {
            back_task();
            app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,20);
        }
    }
    else if(gesture_top&&dir==LV_DIR_TOP)
    {
        if(!gesture_left&&!gesture_right&&!gesture_bottom)
        {
            back_task();
            app_return(LV_SCR_LOAD_ANIM_MOVE_TOP,200,20);
        }
    }
    else if(gesture_bottom&&dir==LV_DIR_BOTTOM)
    {
        back_task();
        app_open(conpanel_load,LV_SCR_LOAD_ANIM_MOVE_BOTTOM,200,10);
    }
}

void gesture(bool top,bool bottom,bool left,bool right,void(*back_task)())
{
    gesture_top=top;
    gesture_bottom=bottom;
    gesture_left=left;
    gesture_right=right;

    lv_obj_add_event_cb(now_screen,gesture_classical,LV_EVENT_GESTURE,(void*)back_task);
}