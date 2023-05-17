#include "app/remotecontrol/remotecontrol.h"
String remotecon_btn_path="/system/app/remotecontrol/button";
String remotecon_drive_path="/system/app/remotecontrol/drives/connect.txt";
String remotecon_ip="192.168.1.1";
String remotecon_wifiname="";
String remotecon_password="";
uint8_t remotecon_send_data=0;
bool remotecon_send=0;
WiFiClient remote_client;
#define remotecon_wifiname_textarea time_display[0]
#define remotecon_password_textarea time_display[1]
#define remotecon_wifiIP_textarea time_display[2]
void remotecon_task(void *p)
{
    bool connected=0;
    bool oldMODE=RUNMODE;
                         
  String url = "/update?data=" + String(remotecon_send_data);
                         
  // 建立字符串，用于HTTP请求
  String httpRequest =  String("GET ") + url + " HTTP/1.1\r\n" +
                        "Host: " + remotecon_ip + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
    lv_obj_t*label=(lv_obj_t*)p;
    while(oldMODE==RUNMODE)
    {
        if(remote_client.connect(remotecon_ip.c_str(), 80))
        {
            String url = "/update?data=" + String(remotecon_send_data);
                                    
            // 建立字符串，用于HTTP请求
            String httpRequest =  String("GET ") + url + " HTTP/1.1\r\n" +
                                    "Host: " + remotecon_ip + "\r\n" +
                                    "Connection: close\r\n" +
                                    "\r\n";
            Serial.println("connecting");
            remote_client.print(httpRequest);
            remote_client.stop();
            if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
            {
                lv_obj_set_style_text_color(label,lv_color_make(40,215,92),LV_PART_MAIN);
                xSemaphoreGive(lv_handler_lock);
            }
            delay(100);
            remotecon_send_data=0;
        }
        else
        {
            if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
            {
                lv_obj_set_style_text_color(label,lv_color_make(237,28,36),LV_PART_MAIN);
                xSemaphoreGive(lv_handler_lock);
            }
        }
        delay(100);
    }
    vTaskDelete(NULL);
}
void remotecon_btn_event(lv_event_t*event)
{
    uint8_t code=lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    if(code==LV_EVENT_PRESSING||code==LV_EVENT_PRESSED)
    {
        remotecon_send_data=String(lv_label_get_text(lv_obj_get_child(btn,0))).toInt();
        remotecon_send=1;
    }
    else if(code==LV_EVENT_RELEASED)
    {
        remotecon_send=0;
    }
}
void load_btn()
{
    File root = SD.open(remotecon_btn_path);
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
        String name=remotecon_btn_path+"/"+file.name();
        lv_obj_t *temp  =lv_btn_create(now_screen);
        lv_obj_t *temp_label =lv_label_create(temp);
        //lv_obj_set_style_bg_color(temp, lv_color_hex(0x5F5F5F), LV_PART_MAIN);
        //lv_obj_set_style_bg_color(temp, lv_color_hex(0x2196f3),LV_STATE_CHECKED);
        lv_obj_set_style_shadow_width(temp, 0, LV_PART_MAIN);
        //lv_obj_set_style_text_font(temp_label,&chinese_hei_16,0);
        //lv_obj_set_style_text_color(temp_label,text_color,0);
        lv_obj_set_size(temp,35,35);
        lv_obj_set_pos(temp,tf.readFileLine(name.c_str(),1).toInt(),tf.readFileLine(name.c_str(),2).toInt());
        
        lv_label_set_text(temp_label,tf.readFileLine(name.c_str(),3).c_str());
        lv_obj_align_to(temp_label,temp,LV_ALIGN_BOTTOM_MID,0,-5);
        //lv_obj_add_state(temp,server_data);
        lv_obj_add_flag(temp_label,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(temp,remotecon_btn_event,LV_EVENT_ALL,NULL);
        file = root.openNextFile();
    }
}
void remotecon_back_event(lv_event_t*event)
{
    WiFi.disconnect();
    tft.setRotation(0);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    lv_disp_drv_update( lv_disp_get_default(),&disp_drv );
    tp.setRotation(1);
    app_return(LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
    WiFi.begin(WiFi_SSID.c_str(),WiFi_Password.c_str());
}
void remotecon_edit_event(lv_event_t*event)
{
    app_open(remotecontrol_config_load,LV_SCR_LOAD_ANIM_MOVE_LEFT,200,10);
}
lv_obj_t*remotecontrol_load()
{
    WiFi.disconnect();
    if(tft.getRotation()!=1)
    {
        tft.setRotation(1);
        disp_drv.hor_res = screenHeight;
        disp_drv.ver_res = screenWidth;
        lv_disp_drv_update( lv_disp_get_default(),&disp_drv );
        tp.setRotation(2);
    }
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
    lv_obj_set_size(now_screen,screenHeight, screenWidth); 
    remotecon_wifiname=tf.readFileLine(remotecon_drive_path.c_str(),1);
    remotecon_password=tf.readFileLine(remotecon_drive_path.c_str(),2);
    remotecon_ip=tf.readFileLine(remotecon_drive_path.c_str(),3);
    WiFi.begin(remotecon_wifiname.c_str(),remotecon_password.c_str());
    load_btn();
    lv_obj_t*remotecon_wifiname_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(remotecon_wifiname_label,text_color,LV_PART_MAIN);
    lv_label_set_text(remotecon_wifiname_label,remotecon_wifiname.c_str());
    lv_obj_align_to(remotecon_wifiname_label,now_screen,LV_ALIGN_TOP_MID,0,15);


    lv_obj_t*back=lv_btn_create(now_screen);
    lv_obj_set_size(back,50,30);
    lv_obj_align_to(back,now_screen,LV_ALIGN_TOP_LEFT,15,10);
    lv_obj_set_style_outline_opa(back, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(back, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(back, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *back_text=lv_label_create(back);
    lv_obj_set_style_text_font(back_text,&lv_font_montserrat_16,LV_PART_MAIN);
    lv_obj_align(back_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(back_text,LV_SYMBOL_LEFT);

    lv_obj_t*edit=lv_btn_create(now_screen);
    lv_obj_set_size(edit,50,30);
    lv_obj_align_to(edit,now_screen,LV_ALIGN_TOP_RIGHT,-15,10);
    lv_obj_set_style_outline_opa(edit, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(edit, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(edit, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *edit_text=lv_label_create(edit);
    lv_obj_add_style(edit_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(edit_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(edit_text,"编辑");

    lv_obj_add_event_cb(back,remotecon_back_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(edit,remotecon_edit_event,LV_EVENT_CLICKED,NULL);
    xTaskCreatePinnedToCore(remotecon_task,NULL,4096,remotecon_wifiname_label,10,NULL,0);
    return now_screen;
}

void remotecon_config_back_event(lv_event_t*event)
{
    app_return(LV_SCR_LOAD_ANIM_NONE,0,10);
}
void remotecon_config_save_event(lv_event_t*event)
{    
    remotecon_wifiname=lv_textarea_get_text(remotecon_wifiname_textarea);
    remotecon_password=lv_textarea_get_text(remotecon_password_textarea);
    remotecon_ip=lv_textarea_get_text(remotecon_wifiIP_textarea);
    tf.writeFile(remotecon_drive_path.c_str(),(remotecon_wifiname+"\r\n"+remotecon_password+"\r\n"+remotecon_ip+"\r\n").c_str());

    app_return(LV_SCR_LOAD_ANIM_NONE,0,10);
}

void edit_text_event(lv_event_t*event)
{
    static int scroll_d=0;
    int code = lv_event_get_code(event);
    lv_obj_t* target=lv_event_get_target(event);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(event);
    if(code==LV_EVENT_FOCUSED)
    {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD)
        {
            if(target==remotecon_wifiIP_textarea)
            {
                lv_keyboard_set_mode(kb,LV_KEYBOARD_MODE_NUMBER);
                lv_textarea_set_accepted_chars(target, "0123456789.");
            }
            else
            {
                lv_keyboard_set_mode(kb,LV_KEYBOARD_MODE_SPECIAL);
            }
            lv_keyboard_set_textarea(kb, target);
            lv_obj_align_to(kb,target,LV_ALIGN_OUT_BOTTOM_MID,0,20);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            scroll_d=lv_obj_get_y(kb)+lv_obj_get_height(kb);
            lv_obj_scroll_to_y(now_screen,scroll_d,LV_ANIM_ON);
        }
    }
    else if(code==LV_EVENT_DEFOCUSED)
    {
        lv_obj_scroll_to_y(now_screen,-1*scroll_d,LV_ANIM_ON);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
    else if(code == LV_EVENT_CANCEL)
    {
        lv_obj_scroll_to_y(now_screen,-1*scroll_d,LV_ANIM_ON);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, target);
    }
    scroll_d=0;
}
lv_obj_t *remotecontrol_config_load()
{
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
    now_screen =lv_obj_create(NULL);
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen,screenHeight, screenWidth); 

    
    remotecon_wifiname_textarea = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(remotecon_wifiname_textarea, true);
    lv_obj_set_size(remotecon_wifiname_textarea,140,40);
    lv_obj_set_style_text_font(remotecon_wifiname_textarea, &chinese_hei_16, 0);
    lv_obj_align(remotecon_wifiname_textarea, LV_ALIGN_TOP_LEFT, 90, 50);
    lv_textarea_set_text(remotecon_wifiname_textarea,remotecon_wifiname.c_str());
    
    lv_obj_t*name_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(name_label,text_color,0);
    lv_obj_set_style_text_font(name_label, &chinese_hei_16, 0);
    lv_label_set_text(name_label,"WiFi:");
    lv_obj_align_to(name_label,now_screen,LV_ALIGN_TOP_LEFT,20,10+lv_obj_get_y(remotecon_wifiname_textarea));
    
    remotecon_password_textarea = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(remotecon_password_textarea, true);
    lv_obj_set_size(remotecon_password_textarea,140,40);
    lv_obj_set_style_text_font(remotecon_password_textarea, &chinese_hei_16, 0);
    lv_obj_align_to(remotecon_password_textarea,remotecon_wifiname_textarea, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_textarea_set_text(remotecon_password_textarea,remotecon_password.c_str());
        
    lv_obj_t*password_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(password_label,text_color,0);
    lv_obj_set_style_text_font(password_label, &chinese_hei_16, 0);
    lv_label_set_text(password_label,"密码:");
    lv_obj_align_to(password_label,now_screen,LV_ALIGN_TOP_LEFT,20,10+lv_obj_get_y(remotecon_password_textarea));
    
    remotecon_wifiIP_textarea = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(remotecon_wifiIP_textarea, true);
    lv_obj_set_size(remotecon_wifiIP_textarea,140,40);
    lv_obj_set_style_text_font(remotecon_wifiIP_textarea, &lv_font_montserrat_14, 0);
    lv_obj_align_to(remotecon_wifiIP_textarea,remotecon_password_textarea, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_textarea_set_text(remotecon_wifiIP_textarea,remotecon_ip.c_str());
        
    lv_obj_t*ip_label=lv_label_create(now_screen);
    lv_obj_set_style_text_color(ip_label,text_color,0);
    lv_obj_set_style_text_font(ip_label, &chinese_hei_16, 0);
    lv_label_set_text(ip_label,"IP地址:");
    lv_obj_align_to(ip_label,now_screen,LV_ALIGN_TOP_LEFT,20,10+lv_obj_get_y(remotecon_wifiIP_textarea));
    
    lv_obj_t*back=lv_btn_create(now_screen);
    lv_obj_set_size(back,50,30);
    lv_obj_align_to(back,now_screen,LV_ALIGN_TOP_LEFT,15,10);
    lv_obj_set_style_outline_opa(back, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(back, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(back, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *back_text=lv_label_create(back);
    lv_obj_set_style_text_font(back_text,&lv_font_montserrat_16,LV_PART_MAIN);
    lv_obj_align(back_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(back_text,LV_SYMBOL_LEFT);

    lv_obj_t*save=lv_btn_create(now_screen);
    lv_obj_set_size(save,50,30);
    lv_obj_align_to(save,now_screen,LV_ALIGN_TOP_RIGHT,-15,10);
    lv_obj_set_style_outline_opa(save, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(save, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(save, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *save_text=lv_label_create(save);
    lv_obj_add_style(save_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(save_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(save_text,"保存");

    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(back,remotecon_config_back_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(save,remotecon_config_save_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(remotecon_wifiname_textarea,edit_text_event,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(remotecon_password_textarea,edit_text_event,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(remotecon_wifiIP_textarea,edit_text_event,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);

    return now_screen;
}