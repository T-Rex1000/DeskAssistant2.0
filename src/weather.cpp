#include "weather.h"

void Weather::config(String Location)
{
  if(Location!="") location = Location;
}
void Weather::config(String Location, String userKey)
{
  if(Location!="") location = Location;
  if(userKey!="") _reqUserKey = userKey;
}

/* 配置心知天气请求信息
 * @param userKey  用户心知天气私钥
 * @param location 获取信息的城市参数
 * @param location 获取信息的温度单位(摄氏/华氏)
 */
bool Weather::update()
{
    if(update1())
    {
        delay(100);
        if(update2())
        {
            delay(100);
            if(get_name(location))
            {
                delay(100);
                return true;
            }
            return false;
        }
    }
    return false;
}


bool Weather::get_name(String location_t)
{
  WiFiClient _wifiClient;
  String reqRes = "/v3/location/search.json?key="+ _reqUserKey +"&q="+location_t;

  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + _host + "\r\n" + 
                              "Connection: close\r\n\r\n";
                
    if (_wifiClient.connect(_host, 80)){
 
    // 向服务器发送http请求信息
    _wifiClient.print(httpRequest);
    
 
    // 获取并显示服务器响应状态行 
    String status_response = _wifiClient.readStringUntil('\n');
    
    // 查验服务器是否响应200 OK
    response_code = status_response.substring(9, 12);
    if (response_code != "200")
    {
      _wifiClient.stop();    
      return false;
    }  

    // 使用find跳过HTTP响应头
    _wifiClient.find("\r\n\r\n");       
    
    
    const size_t capacity = 768;
  DynamicJsonDocument doc(capacity);
  
  deserializeJson(doc, _wifiClient);
  
  JsonArray results = doc["results"];
  
  JsonObject results_0 = results[0];

  location_zh=results_0["name"].as<String>();

    _wifiClient.stop();
    return true;
  } 
  else 
  {  
    _wifiClient.stop();
    return false;

  }                           
  
}
bool Weather::update1()
{
  WiFiClient _wifiClient;
  
  String reqRes = "/v3/weather/now.json?key=" + _reqUserKey +
                  + "&location=" + location + 
                  "&language=en&unit=c";
                  
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + _host + "\r\n" + 
                              "Connection: close\r\n\r\n";
                  
  
  if (_wifiClient.connect(_host, 80)){
 
    // 向服务器发送http请求信息
    _wifiClient.print(httpRequest);
    
 
    // 获取并显示服务器响应状态行 
    String status_response = _wifiClient.readStringUntil('\n');
    
    // 查验服务器是否响应200 OK
    response_code = status_response.substring(9, 12);
    if (response_code != "200")
    {
      _wifiClient.stop();    
      return false;
    }  

    // 使用find跳过HTTP响应头
    _wifiClient.find("\r\n\r\n");       
    
    
    _parseNowInfo(_wifiClient); 
    _wifiClient.stop();
    return true;
  } 
  else 
  {  
    _wifiClient.stop();
    return false;

  }                           
}

bool Weather::update2(){
  WiFiClient _wifiClient;
  
  String reqRes = "/v3/weather/daily.json?key=" +  _reqUserKey +
                  + "&location=" + location + "&language=en&unit=c&start=0&days=3";                  
                  
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + _host + "\r\n" + 
                              "Connection: close\r\n\r\n";
              
  
  if (_wifiClient.connect(_host, 80))
  {
 
    // 向服务器发送http请求信息
    _wifiClient.print(httpRequest);
    
 
    // 获取并显示服务器响应状态行 
    String status_response = _wifiClient.readStringUntil('\n');
    
    // 查验服务器是否响应200 OK
    response_code = status_response.substring(9, 12);
    if (response_code != "200")
    {
      _wifiClient.stop();    
      return false;
    }  

    // 使用find跳过HTTP响应头
    _wifiClient.find("\r\n\r\n");
    
    // 解析服务器响应信息
    _parseForecastInfo(_wifiClient); 
    _wifiClient.stop();
      return true;
  } 
  else 
  {
      _wifiClient.stop();
      return false;
  }                           
}
// 配置心知天气请求信息
void Weather::_parseNowInfo(WiFiClient httpClient){
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  
  deserializeJson(doc, httpClient);
  
  JsonObject results_0 = doc["results"][0];
  
  JsonObject results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"];
  const char* results_0_now_code = results_0_now["code"];
  const char* results_0_now_temperature = results_0_now["temperature"]; 
  
  const char* results_0_last_update = results_0["last_update"]; 

  now_text = results_0_now["text"].as<String>(); 
  code = results_0_now["code"].as<String>()!=NULL?results_0_now["code"].as<int>():-1;
  temperature = results_0_now["temperature"].as<int>(); 
  
  last_update = results_0["last_update"].as<String>();   

}

// 解析服务器响应信息
void Weather::_parseForecastInfo(WiFiClient httpClient)
{
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 3*JSON_OBJECT_SIZE(14) + 860;
    
    DynamicJsonDocument doc(capacity);
    
    deserializeJson(doc, httpClient);
    
    JsonObject results_0 = doc["results"][0];
    
    JsonArray results_0_daily = results_0["daily"];
    
    JsonObject results_0_daily_0 = results_0_daily[0]; 
    JsonObject results_0_daily_1 = results_0_daily[1]; 
    JsonObject results_0_daily_2 = results_0_daily[2];
    
    // 从以上信息中解析数值
    forecast_day[0] = results_0_daily_0["text_day"].as<String>(); 
    forecast_code_day[0] = results_0_daily_0["code_day"].as<int>(); 
    forecast_night[0] = results_0_daily_0["text_night"].as<String>(); 
    forecast_code_night[0] = results_0_daily_0["code_night"].as<int>(); 
    forecast_high[0] = results_0_daily_0["high"].as<int>();
    forecast_low[0] = results_0_daily_0["low"].as<int>();
    /*
    _rainfall[0] = results_0_daily_0["rainfall"].as<float>();
    _wind_direction[0] = results_0_daily_0["wind_direction"].as<String>();
    _wind_speed[0] = results_0_daily_0["wind_speed"].as<float>();
    _wind_scale[0] = results_0_daily_0["wind_scale"].as<int>();
    _humidity[0] = results_0_daily_0["humidity"].as<int>();   
    */
    forecast_day[1] = results_0_daily_1["text_day"].as<String>(); 
    forecast_code_day[1] = results_0_daily_1["code_day"].as<int>(); 
    forecast_night[1] = results_0_daily_1["text_night"].as<String>(); 
    forecast_code_night[1] = results_0_daily_1["code_night"].as<int>(); 
    forecast_high[1] = results_0_daily_1["high"].as<int>();
    forecast_low[1] = results_0_daily_1["low"].as<int>();
    /*
    _rainfall[1] = results_0_daily_1["rainfall"].as<float>();
    _wind_direction[1] = results_0_daily_1["wind_direction"].as<String>();
    _wind_speed[1] = results_0_daily_1["wind_speed"].as<float>();
    _wind_scale[1] = results_0_daily_1["wind_scale"].as<int>();
    _humidity[1] = results_0_daily_1["humidity"].as<int>();   
    */
    forecast_day[2] = results_0_daily_2["text_day"].as<String>(); 
    forecast_code_day[2] = results_0_daily_2["code_day"].as<int>(); 
    forecast_night[2] = results_0_daily_2["text_night"].as<String>(); 
    forecast_code_night[2] = results_0_daily_2["code_night"].as<int>(); 
    forecast_high[2] = results_0_daily_2["high"].as<int>();
    forecast_low[2] = results_0_daily_2["low"].as<int>();
    /*
    _rainfall[2] = results_0_daily_2["rainfall"].as<float>();
    _wind_direction[2] = results_0_daily_2["wind_direction"].as<String>();
    _wind_speed[2] = results_0_daily_2["wind_speed"].as<float>();
    _wind_scale[2] = results_0_daily_2["wind_scale"].as<int>();
    _humidity[2] = results_0_daily_2["humidity"].as<int>();   
    */
    last_update = results_0["last_update"].as<String>();
}

Weather weather;