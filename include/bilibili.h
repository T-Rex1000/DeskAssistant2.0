#ifndef BILIBILI_H
#define BILIBILI_H
#include "head.h"
#include <ArduinoJson.h>    //引用两个需要的库
#include <HTTPClient.h>


class Bilibili
{
public:
    String UID;
    long fans;
    int follow;
    String name;
    String face;
    uint8_t level;
    String sign;
    int archive_count;
    long likes;
    bool update();
  
private:
    void ParseData(String url);
 
    String GitURL(String uid);
  
};
extern Bilibili bilibili;

#endif
