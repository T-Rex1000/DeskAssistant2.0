#include "app/filemanage/filemanage.h"
int directory_num=0;
int file_num=0;
lv_obj_t *directory_all;
lv_obj_t *file_all;
lv_color_t main_color;
lv_color_t text_color;
String path_chains="/";
lv_obj_t*file_delete_msg;
void gesture_filemanage(lv_event_t* event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_RIGHT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320)
        {
            uint8_t end=0;
            for(end=path_chains.length()-1;end>0;end--)
            {
                if(path_chains.c_str()[end]=='/')
                {
                    path_chains=path_chains.substring(0,end);
                    Serial.println(path_chains);
                    break;
                }
            }
            app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,10);
        }
    }
}
void directory_event(lv_event_t*event)
{
    uint8_t code =lv_event_get_code(event);
    lv_obj_t*target=lv_event_get_target(event);
    static bool long_press=0;
    if(code==LV_EVENT_CLICKED)
    {
        if(!long_press)
        {
            path_chains+="/";
            path_chains+=lv_label_get_text(lv_obj_get_child(target,0));
            app_open(filemanage_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
        }
        long_press=0;
    }
    else if(code==LV_EVENT_LONG_PRESSED)
    {
        long_press=1;
        path_chains+="/";
        path_chains+=lv_label_get_text(lv_obj_get_child(target,0));
        
        for(int i=0;i<directory_num;i++)
        {
            lv_obj_clear_flag(lv_obj_get_child(directory_all,i),LV_OBJ_FLAG_CLICKABLE);
        }
        for(int i=0;i<file_num;i++)
        {
            lv_obj_clear_flag(lv_obj_get_child(file_all,i),LV_OBJ_FLAG_CLICKABLE);
        }
        lv_obj_clear_flag(now_screen,LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_align_to(file_delete_msg, lv_scr_act(),LV_ALIGN_CENTER, 0, 0);
        lv_obj_clear_flag(file_delete_msg,LV_OBJ_FLAG_HIDDEN);
    }
}

void file_delete_delete_event(lv_event_t*e)
{
    File root = SD.open(path_chains.c_str());
    if(!root.isDirectory()) tf.deleteFile(path_chains.c_str());
    else tf.removeDir(path_chains.c_str());
    lv_obj_add_flag(file_delete_msg,LV_OBJ_FLAG_HIDDEN);
    uint8_t end=0;
    for(end=path_chains.length()-1;end>0;end--)
    {
        if(path_chains.c_str()[end]=='/')
        {
            path_chains=path_chains.substring(0,end);
            Serial.println(path_chains);
            break;
        }
    }
    
    for(int i=0;i<directory_num;i++)
    {
        lv_obj_add_flag(lv_obj_get_child(directory_all,i),LV_OBJ_FLAG_CLICKABLE);
    }
    for(int i=0;i<file_num;i++)
    {
        lv_obj_add_flag(lv_obj_get_child(file_all,i),LV_OBJ_FLAG_CLICKABLE);
    }
    lv_obj_add_flag(now_screen,LV_OBJ_FLAG_SCROLLABLE);
    lv_scr_load_anim(filemanage_load(),LV_SCR_LOAD_ANIM_NONE,0,0,true);
}
void file_delete_cancel_event(lv_event_t*e)
{
    uint8_t end=0;
    for(end=path_chains.length()-1;end>0;end--)
    {
        if(path_chains.c_str()[end]=='/')
        {
            path_chains=path_chains.substring(0,end);
            Serial.println(path_chains);
            break;
        }
    }
    
    for(int i=0;i<directory_num;i++)
    {
        lv_obj_add_flag(lv_obj_get_child(directory_all,i),LV_OBJ_FLAG_CLICKABLE);
    }
    for(int i=0;i<file_num;i++)
    {
        lv_obj_add_flag(lv_obj_get_child(file_all,i),LV_OBJ_FLAG_CLICKABLE);
    }
    lv_obj_add_flag(now_screen,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(file_delete_msg,LV_OBJ_FLAG_HIDDEN);
}
void file_event(lv_event_t*event)
{
    uint8_t code =lv_event_get_code(event);
    lv_obj_t*target=lv_event_get_target(event);
    if(code==LV_EVENT_LONG_PRESSED)
    {
        path_chains+="/";
        path_chains+=lv_label_get_text(lv_obj_get_child(target,0));
        for(int i=0;i<directory_num;i++)
        {
            lv_obj_clear_flag(lv_obj_get_child(directory_all,i),LV_OBJ_FLAG_CLICKABLE);
        }
        for(int i=0;i<file_num;i++)
        {
            lv_obj_clear_flag(lv_obj_get_child(file_all,i),LV_OBJ_FLAG_CLICKABLE);
        }
        lv_obj_clear_flag(now_screen,LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_align_to(file_delete_msg, lv_scr_act(),LV_ALIGN_CENTER, 0, 0);
        lv_obj_clear_flag(file_delete_msg,LV_OBJ_FLAG_HIDDEN);
    }
}
void file_load(String path)
{
    directory_num=0;
    file_num=0;
    directory_all=lv_label_create(now_screen);
    file_all=lv_label_create(now_screen);
    
    lv_obj_set_size(directory_all,screenWidth,45*1);
    lv_obj_set_size(file_all,screenWidth,45*1);
    lv_obj_set_style_bg_opa(directory_all, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_color(directory_all,main_color,LV_PART_MAIN);
    lv_obj_set_style_bg_opa(directory_all, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_color(file_all,main_color,LV_PART_MAIN);
    lv_label_set_text(directory_all,"");
    lv_label_set_text(file_all,"");
    File root = SD.open(path);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());

            lv_obj_t *temp  =lv_btn_create(directory_all);
            lv_obj_t *temp_label =lv_label_create(temp);
            lv_obj_set_size(temp,200,40);
            lv_obj_set_style_bg_color(temp, main_color, LV_PART_MAIN);
            lv_obj_set_style_bg_color(temp, lv_color_make(28,187,255),LV_STATE_PRESSED);
            lv_obj_set_style_shadow_width(temp, 0, LV_PART_MAIN);
            lv_obj_set_style_text_font(temp_label,&chinese_hei_16,0);
            lv_obj_set_style_text_color(temp_label,text_color,0);

            lv_label_set_text(temp_label,file.name());
            if(directory_num>0)lv_obj_align_to(temp,lv_obj_get_child(directory_all,directory_num-1),LV_ALIGN_OUT_BOTTOM_MID,0,10);
            else lv_obj_align_to(temp,directory_all,LV_ALIGN_TOP_MID,0,10);
            lv_obj_align_to(temp_label,temp,LV_ALIGN_LEFT_MID,10,0);
            
            lv_obj_add_event_cb(temp,directory_event,LV_EVENT_ALL,NULL);
            directory_num++;
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());


            lv_obj_t *temp  =lv_btn_create(file_all);
            lv_obj_t *temp_label =lv_label_create(temp);
            lv_obj_set_size(temp,200,40);
            lv_obj_set_style_bg_color(temp, main_color, LV_PART_MAIN);
            lv_obj_set_style_bg_color(temp, lv_color_make(28,187,255),LV_STATE_PRESSED);
            lv_obj_set_style_shadow_width(temp, 0, LV_PART_MAIN);
            lv_obj_set_style_text_font(temp_label,&chinese_hei_16,0);
            lv_obj_set_style_text_color(temp_label,text_color,0);

            lv_label_set_text(temp_label,file.name());
            if(file_num>0)lv_obj_align_to(temp,lv_obj_get_child(file_all,file_num-1),LV_ALIGN_OUT_BOTTOM_MID,0,10);
            else lv_obj_align_to(temp,file_all,LV_ALIGN_TOP_MID,0,5);
            lv_obj_align_to(temp_label,temp,LV_ALIGN_LEFT_MID,10,0);
            
            lv_obj_add_event_cb(temp,file_event,LV_EVENT_ALL,NULL);
            file_num++;
        }
        file = root.openNextFile();
        lv_obj_set_size(directory_all,screenWidth,5+50*directory_num);
        lv_obj_set_size(file_all,screenWidth,5+50*file_num);
        lv_obj_align_to(directory_all,now_screen,LV_ALIGN_TOP_MID,0,0);
        lv_obj_align_to(file_all,directory_all,LV_ALIGN_OUT_BOTTOM_MID,0,0);
        if(directory_num+file_num==0)
        {
            lv_obj_t*empty_label=lv_label_create(now_screen);
            lv_obj_set_style_text_font(empty_label,&chinese_hei_16,0);
            if(Theme_color==Theme_color_BLACK) lv_obj_set_style_text_color(empty_label,lv_color_make(255,255,255),0);
            else if(Theme_color==Theme_color_WHITE) lv_obj_set_style_text_color(empty_label,lv_color_make(0,0,0),0);
            lv_label_set_text(empty_label,"空");
            lv_obj_align(empty_label,LV_ALIGN_CENTER,0,0);
        }
    }
} 

lv_obj_t* filemanage_load()
{
    now_screen =lv_obj_create(NULL);
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
    file_load(path_chains);
    
    file_delete_msg = lv_label_create(now_screen);
    lv_obj_t*delete_title=lv_label_create(file_delete_msg);
    lv_obj_set_size(file_delete_msg,180,120);
    lv_obj_set_style_text_font(delete_title, &chinese_hei_16, 0);
    lv_obj_align(file_delete_msg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(delete_title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_bg_opa(file_delete_msg,230,0);
    lv_obj_set_style_bg_color(file_delete_msg,lv_color_make(220,220,220),0);
    lv_obj_set_style_text_color(delete_title,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_align(delete_title,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(file_delete_msg,"");
    lv_label_set_text(delete_title,"是否确认删除");
    lv_obj_t*cancel_btn=lv_label_create(file_delete_msg);
    lv_obj_t*delete_btn=lv_label_create(file_delete_msg);
    lv_obj_add_flag(cancel_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(delete_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(cancel_btn,90,40);
    lv_obj_set_size(delete_btn,90,40);
    lv_obj_align(cancel_btn,LV_ALIGN_BOTTOM_LEFT,0,0);
    lv_obj_align(delete_btn,LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_obj_set_style_text_font(cancel_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_font(delete_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_color(cancel_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_color(delete_btn,lv_color_make(255,21,21),0);
    lv_obj_set_style_text_align(cancel_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(delete_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(cancel_btn,"取消");
    lv_label_set_text(delete_btn,"删除");
    lv_obj_add_flag(file_delete_msg,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cancel_btn,file_delete_cancel_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(delete_btn,file_delete_delete_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(now_screen,gesture_filemanage,LV_EVENT_GESTURE,NULL);
    return now_screen;
}