#include "app/linkhome/linkhome.h"
#define PREOBJ 0
#define LINKHOME_LIGHT "\xEE\x98\x8F"
#define LINKHOME_FAN "\xEE\x98\x95"
LV_FONT_DECLARE(linkhome_type_icon);
WiFiClient client;

uint8_t switch_num=0;
lv_obj_t*switch_all;
lv_obj_t*link_edit_msg;
String link_path="/system/app/linkhome/config/drives";
enum
{
    btn1_open=1,
    btn2_open,
    btn3_open,
    btn4_open,
    btn1_close,
    btn2_close,
    btn3_close,
    btn4_close,

};
typedef struct Link
{
    struct Link*next;
    struct Link*last;
    String ip;
    String data;
    lv_obj_t*btn;
}LINK;
LINK *link_drives;
bool change_server=0;
String now_ip="192.168.188.123";
uint8_t send_data=255;
void connect_task(void *p)
{
    bool connected=0;
    bool oldMODE=RUNMODE;
                         
  String url = "/update?data=" + String(send_data);
                         
  // 建立字符串，用于HTTP请求
  String httpRequest =  String("GET ") + url + " HTTP/1.1\r\n" +
                        "Host: " + now_ip + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
    while(oldMODE==RUNMODE)
    {
        if(change_server&&client.connect(now_ip.c_str(), 80))
        {        
            String url = "/update?data=" + String(send_data);
                                    
            // 建立字符串，用于HTTP请求
            String httpRequest =  String("GET ") + url + " HTTP/1.1\r\n" +
                                    "Host: " + now_ip + "\r\n" +
                                    "Connection: close\r\n" +
                                    "\r\n";
            Serial.println("connecting");
            client.print(httpRequest);
            client.stop();
            delay(100);   
            change_server=0;
            send_data=255;
        }
        delay(100);
    }
    vTaskDelete(NULL);
}
void linkhome_btn_event(lv_event_t*event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    switch(code)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            String drive_name =link_path+"/"+lv_label_get_text(lv_obj_get_child(btn,0))+".txt";
            
            if(now_ip!=tf.readFileLine(drive_name.c_str(),2))
            {
                now_ip=tf.readFileLine(drive_name.c_str(),2);
                change_server=1;
            }
            //send_data=(send_data==btn1_open?btn1_close:btn1_open);
            send_data=tf.readFileLine(drive_name.c_str(),3).toInt();
            change_server=1;
        }
        break;
    }
}

lv_obj_t*add_btn;
String now_selete_btn="";
void add_event(lv_event_t*event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    static bool long_press=0;
    switch(code)
    {
        case LV_EVENT_CLICKED:
        {
            if(!long_press)
            {
                if(btn!=now_screen)
                {
                    lv_obj_t *temp  =lv_btn_create(now_screen);
                    lv_obj_t *temp_label =lv_label_create(temp);
                    lv_obj_set_size(temp,90,80);
                    lv_obj_set_style_bg_color(temp, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
                    lv_obj_set_style_bg_color(temp, lv_color_hex(0x2196f3),LV_STATE_CHECKED);
                    lv_obj_set_style_shadow_width(temp, 0, LV_PART_MAIN);
                    lv_obj_set_style_text_font(temp_label,&chinese_hei_16,0);
                    lv_obj_set_style_text_color(temp_label,text_color,0);
                    lv_obj_add_flag(temp,LV_OBJ_FLAG_CHECKABLE);

                    lv_obj_align_to(temp,add_btn,LV_ALIGN_CENTER,0,0);
                    lv_label_set_text(temp_label,String(switch_num).c_str());
                    lv_obj_align_to(temp_label,temp,LV_ALIGN_BOTTOM_MID,0,-5);

                    //lv_obj_add_state(temp,server_data);
                    lv_obj_add_event_cb(temp,linkhome_btn_event,LV_EVENT_CLICKED,NULL);
                    lv_obj_add_event_cb(temp,add_event,LV_EVENT_LONG_PRESSED,add_btn);
                    tf.writeFile((link_path+"/"+lv_label_get_text(temp_label)+".txt").c_str(),"0\r\n0\r\n0\r\n");
                    switch_num++;
                    lv_obj_move_foreground(add_btn);
                    lv_obj_move_foreground(link_edit_msg);
                    if(switch_num%2==0)lv_obj_align_to(btn,lv_obj_get_child(now_screen,PREOBJ+switch_num-2),LV_ALIGN_OUT_BOTTOM_MID,0,10);
                    else lv_obj_align_to(btn,lv_obj_get_child(now_screen,PREOBJ+switch_num-1),LV_ALIGN_OUT_RIGHT_MID,20,0);
                    
                }
                else
                {
                    lv_obj_add_flag(add_btn,LV_OBJ_FLAG_HIDDEN);
                }
            }
            lv_obj_add_flag(add_btn,LV_OBJ_FLAG_CLICKABLE);
            long_press=0;
        }
        break;
        case LV_EVENT_LONG_PRESSED:
        {
            long_press=1;
            if(btn!=now_screen&&btn!=add_btn)
            {
                for(int i=0;i<switch_num;i++)
                {
                    lv_obj_add_state(lv_obj_get_child(now_screen,PREOBJ+i),LV_STATE_DISABLED);
                }
                now_selete_btn=lv_label_get_text(lv_obj_get_child(btn,0));
                now_selete_btn+=".txt";
                lv_obj_clear_flag(now_screen,LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_align_to(link_edit_msg, lv_scr_act(),LV_ALIGN_CENTER, 0, 0);
                lv_obj_clear_flag(link_edit_msg,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_state(btn,lv_obj_get_state(btn));
                lv_obj_move_foreground(link_edit_msg);
            }
            else
            {
                lv_obj_clear_flag(add_btn,LV_OBJ_FLAG_CLICKABLE);
                lv_obj_clear_flag(add_btn,LV_OBJ_FLAG_HIDDEN);
            }
        }
        break;
    }
}
void link_edit_cancel_event(lv_event_t*event)
{
    for(int i=0;i<switch_num;i++)
    {
        lv_obj_clear_state(lv_obj_get_child(now_screen,PREOBJ+i),LV_STATE_DISABLED);
    }
    lv_obj_add_flag(now_screen,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(link_edit_msg,LV_OBJ_FLAG_HIDDEN);
}
void link_edit_event(lv_event_t*event)
{
    app_open(linkhome_config_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
}
void link_drives_load()
{
    switch_num=0;
    File root = SD.open(link_path);
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
        lv_obj_t *temp  =lv_btn_create(now_screen);
        lv_obj_t *temp_label =lv_label_create(temp);
        lv_obj_set_size(temp,90,80);
        lv_obj_set_style_bg_color(temp, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
        lv_obj_set_style_bg_color(temp, lv_color_hex(0x2196f3),LV_STATE_CHECKED);
        lv_obj_set_style_shadow_width(temp, 0, LV_PART_MAIN);
        lv_obj_set_style_text_font(temp_label,&chinese_hei_16,0);
        lv_obj_set_style_text_color(temp_label,text_color,0);
        lv_obj_add_flag(temp,LV_OBJ_FLAG_CHECKABLE);
        
        lv_obj_t *temp_icon =lv_label_create(temp);
        lv_obj_set_style_text_font(temp_icon,&linkhome_type_icon,0);
        int type=tf.readFileLine((link_path+"/"+file.name()).c_str(),1).toInt();
        if(type==0) lv_label_set_text(temp_icon,LINKHOME_LIGHT);
        else lv_label_set_text(temp_icon,LINKHOME_FAN);
        lv_obj_align_to(temp_icon,temp,LV_ALIGN_TOP_MID,0,10);

        if(switch_num==0)lv_obj_align_to(temp,now_screen,LV_ALIGN_TOP_LEFT,20,20);
        else if(switch_num%2==0)lv_obj_align_to(temp,lv_obj_get_child(now_screen,PREOBJ+switch_num-2),LV_ALIGN_OUT_BOTTOM_MID,0,10);
        else lv_obj_align_to(temp,lv_obj_get_child(now_screen,PREOBJ+switch_num-1),LV_ALIGN_OUT_RIGHT_MID,20,0);
        //lv_label_set_text(temp_label,String(switch_num).c_str());
        String name=file.name();
        lv_label_set_text(temp_label,name.substring(0,name.length()-4).c_str());
        lv_obj_align_to(temp_label,temp,LV_ALIGN_BOTTOM_MID,0,-5);
        //lv_obj_add_state(temp,server_data);
        lv_obj_add_event_cb(temp,linkhome_btn_event,LV_EVENT_ALL,NULL);
        lv_obj_add_event_cb(temp,add_event,LV_EVENT_LONG_PRESSED,NULL);
        switch_num++;
        file = root.openNextFile();
    }
}
lv_obj_t*linkhome_load()
{
    now_screen =lv_obj_create(NULL);
    main_color;
    text_color;
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

/*
{
    lv_obj_t *temp  =lv_btn_create(now_screen);
    lv_obj_t *temp_label =lv_label_create(temp);
    lv_obj_set_size(temp,90,80);
    lv_obj_set_style_bg_color(temp, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
    lv_obj_set_style_bg_color(temp, lv_color_hex(0x2196f3),LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(temp, 0, LV_PART_MAIN);
    lv_obj_set_style_text_font(temp_label,&chinese_hei_16,0);
    lv_obj_set_style_text_color(temp_label,text_color,0);
    lv_obj_add_flag(temp,LV_OBJ_FLAG_CHECKABLE);
    lv_obj_align_to(temp,now_screen,LV_ALIGN_TOP_LEFT,20,20);
    lv_label_set_text(temp_label,"1111");
    lv_obj_align_to(temp_label,temp,LV_ALIGN_BOTTOM_MID,0,-5);
    //lv_obj_add_state(temp,server_data);
    lv_obj_add_event_cb(temp,linkhome_btn_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(temp,add_event,LV_EVENT_LONG_PRESSED,NULL);
    switch_num++;
}
*/
    link_drives_load();
    
    add_btn  = lv_btn_create(now_screen);
    link_edit_msg = lv_label_create(now_screen);
    lv_obj_t*delete_title=lv_label_create(link_edit_msg);
    lv_obj_set_size(link_edit_msg,180,120);
    lv_obj_set_style_text_font(delete_title, &chinese_hei_16, 0);
    lv_obj_align(link_edit_msg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(delete_title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_bg_opa(link_edit_msg,230,0);
    lv_obj_set_style_bg_color(link_edit_msg,lv_color_make(220,220,220),0);
    lv_obj_set_style_text_color(delete_title,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_align(delete_title,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(link_edit_msg,"");
    lv_label_set_text(delete_title,"是否编辑按钮");
    lv_obj_t*cancel_btn=lv_label_create(link_edit_msg);
    lv_obj_t*delete_btn=lv_label_create(link_edit_msg);
    lv_obj_add_flag(cancel_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(delete_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(cancel_btn,90,40);
    lv_obj_set_size(delete_btn,90,40);
    lv_obj_align(cancel_btn,LV_ALIGN_BOTTOM_LEFT,0,0);
    lv_obj_align(delete_btn,LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_obj_set_style_text_font(cancel_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_font(delete_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_color(cancel_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_color(delete_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_align(cancel_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(delete_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(cancel_btn,"取消");
    lv_label_set_text(delete_btn,"编辑");
    lv_obj_add_flag(link_edit_msg,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cancel_btn,link_edit_cancel_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(delete_btn,link_edit_event,LV_EVENT_CLICKED,NULL);
    
    lv_obj_t *add_label =lv_label_create(add_btn);
    lv_obj_set_size(add_btn,90,80);
    lv_obj_set_style_bg_color(add_btn, lv_color_make(220,220,220), LV_PART_MAIN);
    lv_obj_set_style_bg_color(add_btn, lv_color_hex(0x2196f3),LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(add_btn, 0, LV_PART_MAIN);
    if(switch_num==0)lv_obj_align_to(add_btn,now_screen,LV_ALIGN_TOP_LEFT,20,20);
    else if(switch_num%2==0)lv_obj_align_to(add_btn,lv_obj_get_child(now_screen,PREOBJ+switch_num-2),LV_ALIGN_OUT_BOTTOM_MID,0,10);
    else lv_obj_align_to(add_btn,lv_obj_get_child(now_screen,PREOBJ+switch_num-1),LV_ALIGN_OUT_RIGHT_MID,20,0);
    lv_obj_set_style_text_color(add_label,text_color,0);
    lv_label_set_text(add_label,LV_SYMBOL_PLUS);
    lv_obj_align_to(add_label,add_btn,LV_ALIGN_CENTER,0,0);
    lv_obj_add_flag(add_btn,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(add_btn,add_event,LV_EVENT_ALL,add_btn);
    lv_obj_add_event_cb(now_screen,add_event,LV_EVENT_ALL,add_btn);
    //if(switch_num)
    xTaskCreatePinnedToCore(connect_task,NULL,4096,NULL,10,NULL,0);
    gesture(0,1,0,1);
    return now_screen;
}
lv_obj_t*drive_name_text;
lv_obj_t*drive_ip_text;
lv_obj_t*drive_data_text;
void drive_type_opt_event(lv_event_t*e)
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
void btn_change(lv_event_t *e)
{
    int code = lv_event_get_code(e);
    lv_obj_t* target=lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(e);
    if(code==LV_EVENT_FOCUSED)
    {
        //uid_changed=1;
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            //lv_textarea_set_text(UID_text,bilibili.UID.c_str());
            lv_keyboard_set_textarea(kb, target);
            lv_obj_align_to(kb,target,LV_ALIGN_OUT_BOTTOM_RIGHT,10,20);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            if(target==drive_ip_text)
            {
                lv_keyboard_set_mode(kb,LV_KEYBOARD_MODE_NUMBER);
                lv_textarea_set_accepted_chars(target, "0123456789.");
                lv_obj_scroll_to_y(now_screen,lv_obj_get_y(kb)+lv_obj_get_height(kb),LV_ANIM_ON);
            }
            else if(target==drive_data_text)
            {
                lv_keyboard_set_mode(kb,LV_KEYBOARD_MODE_NUMBER);
                lv_textarea_set_accepted_chars(target, "0123456789");
                lv_obj_scroll_to_y(now_screen,lv_obj_get_y(kb)+lv_obj_get_height(kb),LV_ANIM_ON);
            }
            else
            {
                lv_keyboard_set_mode(kb,LV_KEYBOARD_MODE_TEXT_LOWER);
            }
        }
    }
    else if(code==LV_EVENT_DEFOCUSED)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
        
        if(target==drive_ip_text||target==drive_data_text)
        {
            lv_obj_scroll_to_y(now_screen,lv_obj_get_y(drive_data_text)+lv_obj_get_height(drive_data_text),LV_ANIM_ON);
        }
    }
    else if(code == LV_EVENT_CANCEL)
    {
        //lv_textarea_set_text(UID_text,bilibili.UID.c_str());
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
        
        if(target==drive_ip_text||target==drive_data_text)
        {
            lv_obj_scroll_to_y(now_screen,lv_obj_get_y(drive_data_text)+lv_obj_get_height(drive_data_text),LV_ANIM_ON);
        }
    }
}
void edit_save_event(lv_event_t*e)
{
    lv_obj_t*type_opt=(lv_obj_t*)lv_event_get_user_data(e);
    //bilibili_theme=lv_dropdown_get_selected(type_opt);
    //if(!SD.exists(link_path+"/"+lv_textarea_get_text(drive_name_text)))
    {
        tf.renameFile((link_path+"/"+now_selete_btn).c_str(),(link_path+"/"+lv_textarea_get_text(drive_name_text)).c_str());
        now_selete_btn=lv_textarea_get_text(drive_name_text);
        tf.writeFile((link_path+"/"+now_selete_btn).c_str(),(String(lv_dropdown_get_selected(type_opt))+"\r\n"+lv_textarea_get_text(drive_ip_text)+"\r\n"+lv_textarea_get_text(drive_data_text)+"\r\n").c_str());
        app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,10);
    }
    //else
    {
        //lv_obj_clear_flag(link_edit_msg,LV_OBJ_FLAG_HIDDEN);
    }
}
void drive_delete_event(lv_event_t*event)
{
    tf.deleteFile((link_path+"/"+now_selete_btn).c_str());
    app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,10);
}
void link_exist_name_msg(lv_event_t*event)
{
    lv_obj_add_flag(link_edit_msg,LV_OBJ_FLAG_HIDDEN);
}
lv_obj_t*linkhome_config_load()
{
    Serial.println(now_selete_btn);
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

    setting_title=lv_label_create(now_screen);
    lv_obj_set_style_text_color(setting_title,text_color,0);
    lv_obj_set_style_text_font(setting_title,&chinese_hei_16,0);
    lv_obj_align(setting_title,LV_ALIGN_TOP_LEFT,20,30);
    lv_label_set_text(setting_title,"编辑");
    
    drive_name_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(drive_name_text, true);
    lv_obj_set_size(drive_name_text,140,40);
    lv_obj_set_style_text_font(drive_name_text, &chinese_hei_16, 0);
    lv_obj_align(drive_name_text, LV_ALIGN_TOP_LEFT, 90, 100);
    lv_textarea_set_text(drive_name_text,now_selete_btn.c_str());
    
    lv_obj_t*name_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(name_label,text_color,0);
    lv_obj_set_style_text_font(name_label, &chinese_hei_16, 0);
    lv_label_set_text(name_label,"名称:");
    lv_obj_align_to(name_label,drive_name_text,LV_ALIGN_OUT_LEFT_MID,-30,0);
    
    drive_ip_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(drive_ip_text, true);
    lv_obj_set_size(drive_ip_text,140,40);
    lv_obj_set_style_text_font(drive_ip_text, &chinese_hei_16, 0);
    lv_obj_align_to(drive_ip_text,drive_name_text, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 70);
    lv_textarea_set_text(drive_ip_text,tf.readFileLine((link_path+"/"+now_selete_btn).c_str(),2).c_str());
        Serial.println(tf.readFileLine((link_path+"/"+now_selete_btn).c_str(),2));
    lv_obj_t*ip_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(ip_label,text_color,0);
    lv_obj_set_style_text_font(ip_label, &chinese_hei_16, 0);
    lv_label_set_text(ip_label,"IP地址:");
    lv_obj_align_to(ip_label,drive_ip_text,LV_ALIGN_OUT_LEFT_MID,-18,0);
    
    drive_data_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(drive_data_text, true);
    lv_obj_set_size(drive_data_text,140,40);
    lv_obj_set_style_text_font(drive_data_text, &chinese_hei_16, 0);
    lv_obj_align_to(drive_data_text,drive_ip_text, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 30);
    lv_textarea_set_text(drive_data_text,tf.readFileLine((link_path+"/"+now_selete_btn).c_str(),3).c_str());
        Serial.println(tf.readFileLine((link_path+"/"+now_selete_btn).c_str(),3));
    
    lv_obj_t*data_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(data_label,text_color,0);
    lv_obj_set_style_text_font(data_label, &chinese_hei_16, 0);
    lv_label_set_text(data_label,"数据:");
    lv_obj_align_to(data_label,drive_data_text,LV_ALIGN_OUT_LEFT_MID,-30,0);
    
    lv_obj_t *type_title=lv_label_create(now_screen);
    lv_obj_set_width(type_title,110);
    lv_obj_set_style_text_color(type_title,text_color,0);
    lv_obj_set_style_text_font(type_title,&chinese_hei_16,0);
    lv_obj_align_to(type_title,name_label,LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_label_set_text(type_title,"类型");
    lv_obj_t *type_opt = lv_dropdown_create(now_screen);
    lv_dropdown_set_symbol(type_opt,NULL);
    if(Theme_color==Theme_color_BLACK)
    {
        lv_obj_set_style_border_color(type_opt,lv_color_hex(0x222222),0);
        lv_obj_set_style_border_opa(type_opt,255,0);
        lv_obj_set_style_border_width(type_opt,2,0);
        
        lv_obj_set_style_bg_color(type_opt,lv_color_hex(0x323232),LV_PART_MAIN);
    }
    else if(Theme_color==Theme_color_WHITE)
    {
        lv_obj_set_style_border_color(type_opt,lv_color_hex(0xdddddd),0);
        lv_obj_set_style_border_opa(type_opt,255,0);
        lv_obj_set_style_border_width(type_opt,2,0);
        lv_obj_set_style_bg_color(type_opt,lv_color_hex(0xffffff),LV_PART_MAIN);
    }
    lv_obj_set_style_text_color(type_opt,text_color,0);
    lv_obj_set_style_text_font(type_opt,&chinese_hei_16,0);

    lv_dropdown_set_options_static(type_opt, "灯光\n风扇");
    lv_obj_set_size(type_opt,90,35);
    lv_obj_align_to(type_opt,type_title,LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    
    lv_dropdown_set_selected(type_opt,tf.readFileLine((link_path+"/"+now_selete_btn).c_str(),1).toInt());
    
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

    lv_obj_t*dekete_btn=lv_label_create(now_screen);
    lv_obj_add_flag(dekete_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(dekete_btn,215,40);
    lv_obj_align_to(dekete_btn,drive_data_text,LV_ALIGN_OUT_BOTTOM_MID,-40,20);
    lv_obj_set_style_text_font(dekete_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_color(dekete_btn,lv_color_make(255,38,28),0);
    lv_obj_set_style_text_align(dekete_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(dekete_btn,"删除");
    lv_obj_add_event_cb(dekete_btn,drive_delete_event,LV_EVENT_CLICKED,NULL);
    

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    
    link_edit_msg = lv_label_create(now_screen);
    lv_obj_t*delete_title=lv_label_create(link_edit_msg);
    lv_obj_set_size(link_edit_msg,180,120);
    lv_obj_set_style_text_font(delete_title, &chinese_hei_16, 0);
    lv_obj_align(link_edit_msg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(delete_title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_bg_opa(link_edit_msg,230,0);
    lv_obj_set_style_bg_color(link_edit_msg,lv_color_make(220,220,220),0);
    lv_obj_set_style_text_color(delete_title,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_align(delete_title,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(link_edit_msg,"");
    lv_label_set_text(delete_title,"存在同名按钮");
    lv_obj_t*cancel_btn=lv_label_create(link_edit_msg);
    lv_obj_add_flag(cancel_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(cancel_btn,180,40);
    lv_obj_align(cancel_btn,LV_ALIGN_BOTTOM_MID,0,0);
    lv_obj_set_style_text_font(cancel_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_color(cancel_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_align(cancel_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(cancel_btn,"确定");
    lv_obj_add_flag(link_edit_msg,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cancel_btn,link_exist_name_msg,LV_EVENT_CLICKED,NULL);
    
    lv_obj_add_event_cb(drive_name_text,btn_change,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(drive_ip_text,btn_change,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(drive_data_text,btn_change,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(type_opt, drive_type_opt_event, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(save,edit_save_event,LV_EVENT_CLICKED,type_opt);
    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);

    gesture(0,0,0,1);

    return now_screen;
}