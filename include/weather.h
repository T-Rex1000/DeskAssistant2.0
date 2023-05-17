#ifndef WEATHER_H
#define WEATHER_H
 
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "head.h"
#include "lvgl.h"
    class Weather {
        public:
            String location;  // 城市
            String location_zh="加载中";  // 城市
            void config(String Location);
            void config(String location,String userKey);
            bool update();

            String now_text = "no_init";    
            int code = 999; 
            int temperature = 999;    

            String last_update = "no_init";   
            String  forecast_day[3] = {"no_init", "no_init", "no_init"};   // 白天天气(字符串)
            int forecast_code_day[3] = {999, 999, 999};                         // 白天天气（代码）
            String forecast_night[3] = {"no_init", "no_init", "no_init"};  // 晚上天气（字符串）
            int forecast_code_night[3] = {999, 999, 999};      // 晚上天气（代码）
            int forecast_high[3] = {999, 999, 999};     // 最高气温
            int forecast_low[3] = {999, 999, 999};      // 最低气温
            
            String status_response =  "no_init"; // 服务器响应状态行
            String response_code =  "no_init";   // 服务器响应状态码
            bool get_name(String location_t);
            String _reqUserKey;   // 私钥
          
        private:  
            const char* _host = "api.seniverse.com";     // 服务器地址  
                
            bool update1();
            bool update2();
            void _parseNowInfo(WiFiClient client);      // 解析实时天气信息信息
            void _parseForecastInfo(WiFiClient httpClient);
        
    };

extern Weather weather;
#endif
