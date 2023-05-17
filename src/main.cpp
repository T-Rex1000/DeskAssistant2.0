#include "head.h"
xSemaphoreHandle lv_handler_lock;
lv_style_t style_chinese_16_white;
lv_style_t style_chinese_16_black;
bool open_finish=0;
void open_anim(void *p)
{
    uint8_t red=63;
    uint8_t green=72;
    uint8_t blue=204;
    uint32_t color=tft.color565(red, green, blue);
    int x = screenWidth/2, y = screenHeight / 3, R = 25;
     double x1 = 0, y1 = 0, d = 0;
     double x2 = 0, y2 = 0;
     double x3 = 0, y3 = 0;
     double x1b = 0, y1b = 0;
     double x2b = 0, y2b = 0;
     double x3b = 0, y3b = 0;
    double pi = 3.1415926535;
    double a1 = -90, a2 = 30, a3 = 150;
    double a1b = -90, a2b = 30, a3b = 150;
    
    double a1_v = 0;
    double a2_v = 0;
    double a3_v = 0;

    double a1b_v = 0;
    double a2b_v = 0;
    double a3b_v = 0;
    while (a1 < 30)
    {
        x1 = R * cos(a1 * pi / 180) + x;
        y1 = R * sin(a1 * pi / 180) + y;
        tft.drawPixel((int)x1, (int)y1, color);
        x2 = R * cos(a2 * pi / 180) + x;
        y2 = R * sin(a2 * pi / 180) + y;
        tft.drawPixel((int)x2, (int)y2, color);
        x3 = R * cos(a3 * pi / 180) + x;
        y3 = R * sin(a3 * pi / 180) + y;
        tft.drawPixel((int)x3, (int)y3, color);
        delay(1);
        a1_v+=0.001;
        a2_v+=0.001;
        a3_v+=0.001;
        a1 += a1_v+4 * pi / 180;
        a2 += a2_v+4 * pi / 180;
        a3 += a3_v+4 * pi / 180;

    }
    delay(300);
    while (y3 > y - 2 * R)
    {
        tft.drawPixel((int)x3, (int)y3, color);
        tft.drawPixel((int)x2, (int)y2, color);
        tft.drawPixel((int)x1, (int)y1, color);
        y3--;
        x1 += pow(3, 0.5) / 2;
        y1 += 0.5;
        x2 -= pow(3, 0.5) / 2;
        y2 += 0.5;
        delay(10);
    }
    delay(100);
    while (a1b < 0)
    {
        x1b = 2 * R * cos(a1b * pi / 180) + x;
        y1b = 2 * R * sin(a1b * pi / 180) + y;
        tft.drawPixel((int)x1b, (int)y1b, color);
        x2b = 2 * R * cos(a2b * pi / 180) + x;
        y2b = 2 * R * sin(a2b * pi / 180) + y;
        tft.drawPixel((int)x2b, (int)y2b, color);
        x3b = 2 * R * cos(a3b * pi / 180) + x;
        y3b = 2 * R * sin(a3b * pi / 180) + y;
        tft.drawPixel((int)x3b, (int)y3b, color);
        delay(1);
        a1b_v+=0.01;
        a2b_v+=0.01;
        a3b_v+=0.01;
        a1b += a1b_v+2 * pi / 180;
        a2b += a1b_v+2 * pi / 180;
        a3b += a1b_v+2 * pi / 180;
    }
    open_finish=1;
    vTaskDelete(NULL);
}
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
void init_config()
{
    int wait_wifi=0;
    WiFi.mode(WIFI_STA);
    WiFi_SSID=tf.readFileLine("/system/app/setting/config/wifi.txt",1);
    WiFi_Password=tf.readFileLine("/system/app/setting/config/wifi.txt",2);
    WiFi.begin(WiFi_SSID.c_str(),WiFi_Password.c_str());
    while (wait_wifi<=10&&WiFi.status()!=WL_CONNECTED)
    {
        wait_wifi++;
        delay(500);
    }
    wifiAP_SSID=tf.readFileLine("/system/app/setting/config/wifiAP.txt",1);
    wifiAP_Password=tf.readFileLine("/system/app/setting/config/wifiAP.txt",2);

    blk_value=tf.readFileLine("/system/app/setting/config/backlight.txt",1).toInt();

    home_bg_photo=tf.readFileLine("/system/app/photo/home_background.txt",1);
    Theme_color=tf.readFileLine("/system/app/setting/config/theme.txt",1).toInt();
    home_show=tf.readFileLine("/system/app/setting/config/theme.txt",2).toInt();
    open_first=tf.readFileLine("/system/app/setting/config/theme.txt",3).toInt();

    weather.config(tf.readFileLine("/system/app/weather/config/location.txt",1),tf.readFileLine("/system/app/weather/config/key.txt",1));
    
    bilibili.UID=tf.readFileLine("/system/app/bilibili/config/UID.txt",1);
    bilibili_theme=tf.readFileLine("/system/app/bilibili/config/UID.txt",2).toInt();
}

void init_default()
{
    int wait_wifi=0;
    WiFi.mode(WIFI_STA);
    WiFi.begin("LCCWiFi","123123123");
    while (wait_wifi<=10&&WiFi.status()!=WL_CONNECTED)
    {
        wait_wifi++;
        delay(500);
    }
    wifiAP_SSID="ESPWiFi";
    wifiAP_Password="";
    blk_value=pow(2,6);
    Theme_color=Theme_color_BLACK;
    home_show=home_show_PHOTO;
    open_first=HOME;
    weather.config("zhuhai","null");
    bilibili.UID="00000000";
    bilibili_theme=BILIBILI_PINK;
}
void setup()
{
    Serial.begin(115200);
    //lv_log_register_print_cb(my_print);
    //LittleFS.begin();
    bool tfinited=0;
    SPIFFS.begin();
    delay(100);
    if(tf.init())
        tfinited=1;
    display_init();
    xTaskCreatePinnedToCore(open_anim,NULL,4096,NULL,1,NULL,0);
    touch_init();
    load_photo();
    //SD_init();
        
    if(tfinited)
        init_config();
    else 
        init_default();
    ledcWrite(blk,blk_value);
    configTime(8 * 3600,0,"ntp.sjtu.edu.cn","ntp4.aliyun.com","s1c.time.edu.cn");

    lv_style_init(&style_chinese_16_white);
    lv_style_set_text_font(&style_chinese_16_white,&chinese_hei_16);
    lv_style_set_text_color(&style_chinese_16_white,lv_color_hex(0xffffff));
    lv_style_init(&style_chinese_16_black);
    lv_style_set_text_font(&style_chinese_16_black,&chinese_hei_16);
    lv_style_set_text_color(&style_chinese_16_black,lv_color_hex(0x000000));
    lv_handler_lock=xSemaphoreCreateMutex();
    //vTaskStartScheduler();
    if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
    {
        gui_init(open_first);
        while(1)
        {
            if(open_finish)
            {
                break;
            }
            delay(50);
        }
        delay(2000);
        xSemaphoreGive(lv_handler_lock);
    }
}
long time_wait=0;
void (*other_app)()=painting;
void loop()
{
    if(xSemaphoreTake(lv_handler_lock,portMAX_DELAY))
    {
        lv_timer_handler();
        xSemaphoreGive(lv_handler_lock);
    }
    if(!use_lvgl)
    {
        if(!time_wait)time_wait=millis();
        if(millis()-time_wait>500)
        {
            time_wait=0;
            other_app();
        }
    }
}