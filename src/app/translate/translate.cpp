#include"app/translate/translate.h"

String translate_path="http://fanyi.youdao.com/translate?&doctype=json&type=AUTO&i=";
lv_obj_t*translate_result;
bool translate_start=0;
String translate_word="";
const char* translate_word_result="加载中";
HTTPClient translate_http;
bool translate_get(String url)
{  
    DynamicJsonDocument doc(256);
    translate_http.begin(url);    //请求网址
    
    int httpGet = translate_http.GET();    //获得服务器状态码
    if(httpGet > 0)
    {
        //Serial.printf("HTTPGET is %d",httpGet);    //打印服务器状态码

        if(httpGet == HTTP_CODE_OK)    //判断是否请求成功
        {

            String json = translate_http.getString();    //获得响应体信息
            Serial.println(json);    //打印响应体信息

            deserializeJson(doc, json);    //Json解析
            translate_word_result=doc["translateResult"][0][0]["tgt"];
        }
        else
        {
            translate_http.end();
            return false;
        }
    }
    translate_http.end();
    return true;
}
void translate_task(void *p)
{
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        if(translate_start)
        {
            if(translate_get(translate_path+translate_word))
            {
                if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
                {
                    lv_event_send(translate_result,LV_EVENT_VALUE_CHANGED,NULL);
                    translate_start=0;
                    xSemaphoreGive(lv_handler_lock);
                }
            }
        }
        delay(20);
    }
    vTaskDelete(NULL);
}
void translate_update(lv_event_t*event)
{
    lv_label_set_text(lv_event_get_target(event),translate_word_result);
}
void translate_event(lv_event_t*event)
{
    lv_obj_t *word=(lv_obj_t*)lv_event_get_user_data(event);
    translate_word=lv_textarea_get_text(word);
    translate_start=1;
}
void input_text_event(lv_event_t*event)
{
    int code = lv_event_get_code(event);
    lv_obj_t* target=lv_event_get_target(event);
    lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(event);
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
lv_obj_t* translate_load()
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

    lv_obj_t*input_text = lv_textarea_create(now_screen);
    lv_textarea_set_one_line(input_text, true);
    lv_obj_set_size(input_text,150,40);
    lv_obj_set_style_text_font(input_text, &lv_font_montserrat_16, 0);
    lv_obj_align_to(input_text,now_screen, LV_ALIGN_TOP_LEFT, 20, 30);

    translate_result=lv_label_create(now_screen);
    lv_obj_set_size(translate_result,130,40);
    lv_obj_set_style_text_color(translate_result,text_color,0);
    lv_obj_set_style_text_font(translate_result, &chinese_hei_16, 0);
    lv_obj_align_to(translate_result,input_text, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);
    lv_label_set_text(translate_result,"...");
    
    lv_obj_t *start=lv_btn_create(now_screen);
    lv_obj_set_size(start,50,40);
    lv_obj_align_to(start,input_text,LV_ALIGN_OUT_RIGHT_MID,5,0);
    lv_obj_set_style_outline_opa(start, 255, LV_PART_MAIN);
    lv_obj_set_style_outline_width(start, 2, LV_PART_MAIN);
    lv_obj_set_style_outline_color(start, lv_color_hex(0x2f8edd), LV_PART_MAIN);
    lv_obj_t *start_text=lv_label_create(start);
    lv_obj_add_style(start_text,&style_chinese_16_white,LV_PART_MAIN);
    lv_obj_align(start_text,LV_ALIGN_CENTER,0,1);
    lv_label_set_text(start_text,"翻译");
    
    lv_obj_t * kb = lv_keyboard_create(now_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
 
    lv_obj_add_event_cb(input_text,input_text_event,LV_EVENT_ALL,kb);
    lv_obj_add_event_cb(kb,kb_event1,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(start,translate_event,LV_EVENT_CLICKED,input_text);
    lv_obj_add_event_cb(translate_result,translate_update,LV_EVENT_VALUE_CHANGED,NULL);
    xTaskCreatePinnedToCore(translate_task,NULL,4096,NULL,10,NULL,0);
    gesture(0,1,0,1);
    return now_screen;
}