#include "app/photo/photo.h"

lv_timer_t *photo_timer;

typedef struct Photo
{
    struct Photo*next;
    struct Photo*last;
    String path;
    String name;
}PHOTO;
PHOTO *photos=NULL;
PHOTO *now_photo=NULL;
String photo_path_head="/data/photo";
lv_obj_t*photo;
lv_obj_t*photo_name;
lv_obj_t* delete_msg;

lv_obj_t*photo_bg;
lv_obj_t*photo_del;
lv_obj_t*photo_rename;

void gesture_photo(lv_event_t* event)
{
    lv_obj_t*photo_name_bg=(lv_obj_t*)lv_event_get_user_data(event);
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
        if(now_photo!=NULL&&now_photo->next!=NULL)
        {
            now_photo=now_photo->next;
        }
    }
    else if(dir == LV_DIR_RIGHT)
    {
        if(tp.points[0].y>80&&tp.points[0].y<320&&((now_photo==NULL)||(now_photo->last==NULL)||(tp.points[0].x>=0&&tp.points[0].x<=screenWidth/2)))
        {
            if((now_photo==NULL)||(now_photo->last==NULL)||!lv_obj_has_flag(photo_name_bg,LV_OBJ_FLAG_HIDDEN))
            {
                lv_timer_del(photo_timer);
                lv_img_set_src(photo,"null");
                app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,10);
            }
        }
        if(now_photo!=NULL&&now_photo->last!=NULL)
        {
            now_photo=now_photo->last;
        }
    }
    else if (dir == LV_DIR_TOP)
    {
    }
    else if (dir == LV_DIR_BOTTOM)
    {
    }
}
void photo_event(lv_timer_t *e)
{
    if(now_photo!=NULL)
    {
        lv_img_set_src(photo,("S:"+now_photo->path).c_str());
        lv_label_set_text(photo_name,now_photo->name.c_str());
    }
    else 
    {
        lv_obj_clear_flag(photo_bg,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(photo_del,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(photo_rename,LV_OBJ_FLAG_CLICKABLE);
        lv_img_set_src(photo,"NULL");
        lv_obj_set_size(photo,180,180);
        lv_obj_t *empty_label=lv_label_create(photo);
        lv_obj_set_style_text_font(empty_label,&chinese_hei_16,0);
        if(Theme_color==Theme_color_BLACK) lv_obj_set_style_text_color(empty_label,lv_color_make(255,255,255),0);
        else if(Theme_color==Theme_color_WHITE) lv_obj_set_style_text_color(empty_label,lv_color_make(0,0,0),0);
        lv_label_set_text(empty_label,"空");
        lv_obj_align(empty_label,LV_ALIGN_CENTER,0,0);
        lv_label_set_text(photo_name,"");
    }
    lv_obj_align(photo,LV_ALIGN_CENTER,0,0);
}
lv_obj_t *name_text;
lv_obj_t*lv_rename_top;
void rename_event(lv_event_t*e)
{
    lv_obj_t*kb=(lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_add_flag(photo_name,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(name_text,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(lv_rename_top,LV_OBJ_FLAG_HIDDEN);
    lv_event_send(name_text,LV_EVENT_FOCUSED,kb);
    lv_obj_add_state(name_text, LV_STATE_FOCUSED);
}

void rename_text(lv_event_t *e)
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
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
}
void rename_save_event(lv_event_t*e)
{
    lv_obj_t*kb=(lv_obj_t*)lv_event_get_user_data(e);
    String old_path="";
    old_path=now_photo->path;
    now_photo->name=lv_textarea_get_text(name_text);
    now_photo->path=photo_path_head+"/"+now_photo->name;
    tf.renameFile(old_path.c_str(),now_photo->path.c_str());
    lv_event_send(name_text,LV_EVENT_DEFOCUSED,kb);
    lv_obj_clear_state(lv_rename_top, LV_STATE_FOCUSED);
    lv_obj_add_flag(lv_rename_top,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(photo_name,LV_OBJ_FLAG_HIDDEN);
}
void rename_back_event(lv_event_t*e)
{
    lv_obj_t*kb=(lv_obj_t*)lv_event_get_user_data(e);
    lv_event_send(name_text,LV_EVENT_DEFOCUSED,kb);
    lv_obj_clear_state(lv_rename_top, LV_STATE_FOCUSED);
    lv_obj_add_flag(lv_rename_top,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(photo_name,LV_OBJ_FLAG_HIDDEN);
}
void delete_event(lv_event_t*e)
{
    lv_obj_clear_flag(delete_msg,LV_OBJ_FLAG_HIDDEN);
}
void delete_delete_event(lv_event_t*e)
{
    tf.deleteFile(now_photo->path.c_str());
    PHOTO *temp=now_photo;
    if(now_photo->next!=NULL&&now_photo->last!=NULL)
    {
        now_photo->last->next=now_photo->next;
        now_photo->next->last=now_photo->last;
        now_photo=now_photo->next;
    }
    else if(now_photo->next!=NULL)
    {
        now_photo->next->last=NULL;
        now_photo=now_photo->next;
    }
    else if(now_photo->last!=NULL)
    {
        now_photo->last->next=NULL;
        now_photo=now_photo->last;
    }
    else
    {
        now_photo=NULL;
    }
    delete temp;
    lv_obj_add_flag(delete_msg,LV_OBJ_FLAG_HIDDEN);
}
void delete_cancel_event(lv_event_t*e)
{
    lv_obj_add_flag(delete_msg,LV_OBJ_FLAG_HIDDEN);
}
void photo_bg_event(lv_event_t*e)
{
    lv_obj_t*msg=(lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_clear_flag(msg,LV_OBJ_FLAG_HIDDEN);
}
void bg_save_event(lv_event_t*e)
{
    lv_obj_t*msg=(lv_obj_t*)lv_event_get_user_data(e);
    home_bg_photo=now_photo->path;
    tf.writeFile("/system/app/photo/home_background.txt",(home_bg_photo+"\r\n").c_str());
    lv_obj_add_flag(msg,LV_OBJ_FLAG_HIDDEN);
}
void bg_cancel_event(lv_event_t*e)
{
    lv_obj_t*msg=(lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_add_flag(msg,LV_OBJ_FLAG_HIDDEN);
}
void photo_clicked_event(lv_event_t*e)
{
    lv_obj_t*photo_name_bg=(lv_obj_t*)lv_event_get_user_data(e);
    if(lv_obj_has_flag(photo_name_bg,LV_OBJ_FLAG_HIDDEN))
    {
        lv_obj_clear_flag(photo_name_bg,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(photo_bg,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(photo_rename,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(photo_del,LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(photo_name_bg,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(photo_bg,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(photo_rename,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(photo_del,LV_OBJ_FLAG_HIDDEN);
    }
}
void load_photo(String path)
{
    Serial.printf("Listing directory: %s\n", path);

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
        Serial.print("!!!!");
        if(photos==NULL)
        {
            photos=new PHOTO();//直接使用malloc会因为String类未执行构造函数出现内存问题
            photos->last=NULL;
            photos->next=NULL;
            photos->name=file.name();
            photos->path=path;
            if(file.name()[0]!='/')photos->path+="/";
            photos->path+=file.name();
            now_photo=photos;
        }
        else
        {
            if(photos->next==NULL)
                photos->next=new PHOTO();
            photos->next->last=photos;
            photos->next->next=NULL;
            photos->next->name=file.name();
            photos->next->path=path;
            if(file.name()[0]!='/')photos->next->path+="/";
            photos->next->path+=file.name();
            photos=photos->next;
        }
        file = root.openNextFile();
    }
}

lv_obj_t* photo_load()
{
    now_screen =lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        text_color=lv_color_make(0,0,0);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_make(245,245,245);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    photo=lv_img_create(now_screen);
    lv_obj_t*photo_name_bg=lv_label_create(now_screen);
    photo_name=lv_label_create(photo_name_bg);
    photo_bg=lv_btn_create(now_screen);
    photo_rename=lv_btn_create(now_screen);
    photo_del=lv_btn_create(now_screen);
    lv_obj_add_flag(photo,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(photo_bg,0,0);
    lv_obj_set_style_radius(photo_rename,0,0);
    lv_obj_set_style_radius(photo_del,0,0);
    lv_obj_t*photo_bg_t=lv_label_create(photo_bg);
    lv_obj_t*photo_del_t=lv_label_create(photo_del);
    lv_obj_t*photo_rename_t=lv_label_create(photo_rename);
    if(now_photo!=NULL)
    {
        lv_obj_add_flag(photo_bg,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(photo_del,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(photo_rename,LV_OBJ_FLAG_CLICKABLE);
    }
    else
    {
        lv_obj_clear_flag(photo_bg,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(photo_del,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(photo_rename,LV_OBJ_FLAG_CLICKABLE);
    }
    
    lv_obj_set_size(photo_name_bg,240,40);
    lv_obj_set_size(photo_bg,80,50);
    lv_obj_set_size(photo_del,80,50);
    lv_obj_set_size(photo_rename,80,50);
    lv_obj_set_style_shadow_width(photo_bg,0,LV_PART_MAIN);
    lv_obj_set_style_shadow_width(photo_del,0,LV_PART_MAIN);
    lv_obj_set_style_shadow_width(photo_rename,0,LV_PART_MAIN);
    
    lv_obj_set_style_bg_opa(photo_name_bg,255,0);
    lv_obj_set_style_bg_color(photo_name_bg,main_color,0);
    lv_obj_set_style_bg_color(photo_bg,main_color,0);
    lv_obj_set_style_bg_color(photo_del,main_color,0);
    lv_obj_set_style_bg_color(photo_rename,main_color,0);

    lv_obj_align(photo,LV_ALIGN_CENTER,0,0);
    lv_obj_align(photo_name_bg,LV_ALIGN_TOP_MID,0,0);
    lv_obj_align(photo_name,LV_ALIGN_TOP_MID,0,10);
    lv_obj_align(photo_bg,LV_ALIGN_BOTTOM_LEFT,0,0);
    lv_obj_align(photo_del,LV_ALIGN_BOTTOM_MID,0,0);
    lv_obj_align(photo_rename,LV_ALIGN_BOTTOM_RIGHT,0,0);

    lv_obj_set_style_text_color(photo_name,text_color,0);
    lv_obj_set_style_text_color(photo_bg_t,text_color,0);
    lv_obj_set_style_text_color(photo_del_t,text_color,0);
    lv_obj_set_style_text_color(photo_rename_t,text_color,0);

    lv_obj_align(photo_bg_t,LV_ALIGN_CENTER,0,0);
    lv_obj_align(photo_del_t,LV_ALIGN_CENTER,0,0);
    lv_obj_align(photo_rename_t,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_text_align(photo_name,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(photo_bg_t,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(photo_del_t,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(photo_rename_t,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(photo_name_bg,"");
    lv_img_set_src(photo,"NULL");

    lv_label_set_text(photo_bg_t,LV_SYMBOL_HOME);
    lv_label_set_text(photo_del_t,LV_SYMBOL_TRASH);
    lv_label_set_text(photo_rename_t,LV_SYMBOL_EDIT);

    lv_obj_t*bg_msg = lv_label_create(now_screen);
    lv_obj_t*bg_title=lv_label_create(bg_msg);
    lv_obj_set_size(bg_msg,200,120);
    lv_obj_set_style_text_font(bg_title, &chinese_hei_16, 0);
    lv_obj_align(bg_msg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(bg_title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_bg_opa(bg_msg,200,0);
    lv_obj_set_style_bg_color(bg_msg,lv_color_make(236,236,236),0);
    lv_obj_set_style_text_color(bg_title,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_align(bg_title,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(bg_msg,"");
    lv_label_set_text(bg_title,"是否确认设置为主页背景");
    lv_obj_t*bg_cancel_btn=lv_label_create(bg_msg);
    lv_obj_t*bg_save_btn=lv_label_create(bg_msg);
    lv_obj_add_flag(bg_cancel_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(bg_save_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(bg_cancel_btn,90,40);
    lv_obj_set_size(bg_save_btn,90,40);
    lv_obj_align(bg_cancel_btn,LV_ALIGN_BOTTOM_LEFT,0,0);
    lv_obj_align(bg_save_btn,LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_obj_set_style_text_font(bg_cancel_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_font(bg_save_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_color(bg_cancel_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_color(bg_save_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_align(bg_cancel_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(bg_save_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(bg_cancel_btn,"取消");
    lv_label_set_text(bg_save_btn,"确定");
    lv_obj_add_flag(bg_msg,LV_OBJ_FLAG_HIDDEN);
//delete
    delete_msg = lv_label_create(now_screen);
    lv_obj_t*delete_title=lv_label_create(delete_msg);
    lv_obj_set_size(delete_msg,180,120);
    lv_obj_set_style_text_font(delete_title, &chinese_hei_16, 0);
    lv_obj_align(delete_msg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(delete_title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_bg_opa(delete_msg,200,0);
    lv_obj_set_style_bg_color(delete_msg,lv_color_make(236,236,236),0);
    lv_obj_set_style_text_color(delete_title,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_align(delete_title,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(delete_msg,"");
    lv_label_set_text(delete_title,"是否确认删除");
    lv_obj_t*cancel_btn=lv_label_create(delete_msg);
    lv_obj_t*delete_btn=lv_label_create(delete_msg);
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
    lv_obj_add_flag(delete_msg,LV_OBJ_FLAG_HIDDEN);
    
    //lv_msgbox_close(delete_msg);

//rename
    lv_rename_top=lv_label_create(now_screen);
    lv_obj_set_style_bg_opa(lv_rename_top,255,0);
    lv_obj_set_style_bg_color(lv_rename_top,main_color,0);
    lv_obj_set_size(lv_rename_top,screenWidth,90);
    lv_obj_align_to(lv_rename_top,now_screen,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_add_flag(lv_rename_top,LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(lv_rename_top,"");

    lv_obj_t*back_label=lv_label_create(lv_rename_top);
    lv_obj_set_style_text_color(back_label,text_color,0);
    lv_obj_set_style_text_font(back_label, &lv_font_montserrat_16, 0);
    lv_label_set_text(back_label,LV_SYMBOL_LEFT);
    lv_obj_set_style_text_align(back_label,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_size(back_label,30,30);
    lv_obj_align_to(back_label,lv_rename_top,LV_ALIGN_TOP_LEFT,15,10);
    lv_obj_add_flag(back_label,LV_OBJ_FLAG_CLICKABLE);

    setting_title=lv_label_create(lv_rename_top);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align_to(setting_title,back_label,LV_ALIGN_OUT_RIGHT_TOP,10,5);
    lv_label_set_text(setting_title,"重命名");

    lv_obj_t *save=lv_btn_create(lv_rename_top);
    lv_obj_set_size(save,50,30);
    lv_obj_align_to(save,setting_title,LV_ALIGN_OUT_RIGHT_MID,80,0);
    lv_obj_set_style_outline_opa(save, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(save, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(save, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *save_text=lv_label_create(save);
    lv_obj_add_style(save_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(save_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(save_text,"保存");

    name_text=lv_textarea_create(lv_rename_top);
    lv_textarea_set_one_line(name_text, true);
    lv_obj_set_size(name_text,220,40);
    lv_obj_set_style_text_font(name_text, &lv_font_montserrat_16, 0);
    lv_obj_align_to(name_text,lv_rename_top, LV_ALIGN_BOTTOM_MID, 0, 0);
    if(now_photo!=NULL)lv_textarea_set_text(name_text,(now_photo->name).c_str());
    lv_obj_add_flag(name_text, LV_OBJ_FLAG_HIDDEN);
    

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);


    //lv_obj_add_event_cb(photo,gesture_photo,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(photo,photo_clicked_event,LV_EVENT_CLICKED,photo_name_bg);
    lv_obj_add_event_cb(photo_bg,photo_bg_event,LV_EVENT_CLICKED,bg_msg);
    lv_obj_add_event_cb(bg_cancel_btn,bg_cancel_event,LV_EVENT_CLICKED,bg_msg);
    lv_obj_add_event_cb(bg_save_btn,bg_save_event,LV_EVENT_CLICKED,bg_msg);

    lv_obj_add_event_cb(photo_del,delete_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(cancel_btn,delete_cancel_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(delete_btn,delete_delete_event,LV_EVENT_CLICKED,NULL);

    lv_obj_add_event_cb(back_label,rename_back_event,LV_EVENT_CLICKED,kb);
    lv_obj_add_event_cb(photo_rename,rename_event,LV_EVENT_CLICKED,kb);
    lv_obj_add_event_cb(name_text,rename_text,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(save,rename_save_event,LV_EVENT_CLICKED,kb);
    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);

    photo_timer=lv_timer_create(photo_event,300,NULL);
    lv_obj_add_event_cb(now_screen,gesture_photo,LV_EVENT_GESTURE,photo_name_bg);

    return now_screen;
}