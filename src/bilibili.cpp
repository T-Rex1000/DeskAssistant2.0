#include "bilibili.h"

HTTPClient http;    //创建HTTPClient 实例
String url_bilibili = "";    //资源定位符

bool Bilibili::update()
{
    if(UID!="")
    {
        ParseData(GitURL(UID));
        return true;
    }
    return false;
}
 
String Bilibili::GitURL(String uid)    //拼接资源定位符，获取API链接
{
  //url_bilibili =  "https://api.bilibili.com/x/relation/stat?vmid=";
  url_bilibili =  "https://api.bilibili.com/x/web-interface/card?mid=";
  url_bilibili += uid;
  //url_bilibili="https://api.bilibili.com/x/web-interface/view?bvid=BV1R54y1u7kD";
 
  return url_bilibili;
}
 
void Bilibili::ParseData(String url)
{  
    DynamicJsonDocument doc(3072);  //分配内存
    http.begin(url);    //请求网址
    
    int httpGet = http.GET();    //获得服务器状态码
    if(httpGet > 0)
    {
        //Serial.printf("HTTPGET is %d",httpGet);    //打印服务器状态码

        if(httpGet == HTTP_CODE_OK)    //判断是否请求成功
        {

            String json = http.getString();    //获得响应体信息
            Serial.println(json);    //打印响应体信息

            deserializeJson(doc, json);    //Json解析
            
            fans=doc["data"]["card"]["fans"].as<long>();
            face=doc["data"]["card"]["face"].as<String>();
            name=doc["data"]["card"]["name"].as<String>();
            level=doc["data"]["card"]["level_info"]["current_level"].as<uint8_t>();
            follow=doc["data"]["card"]["attention"].as<int>();
            likes=doc["data"]["like_num"].as<long>();
            archive_count=doc["data"]["archive_count"].as<int>();
            sign=doc["data"]["card"]["sign"].as<String>();
        }
    }
    http.end();

}
Bilibili bilibili;