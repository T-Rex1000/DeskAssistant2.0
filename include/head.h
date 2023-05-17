#ifndef HEAD_H
#define HEAD_H


//HEAD
#include "pins_arduino.h"
#include <Arduino.h>
//wifi
#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include "esp_wifi.h"
#include <esp_now.h>
//bluetooth
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEService.h>
//flash
//#include <LittleFS.h>
#include <SPIFFS.h>

#include <ArduinoJson.h>
//devices
#include "display.h"
#include "touch.h"
#include "SDcard.h"

#include "bilibili.h"
#include "weather.h"

#include "gui.h"

extern xSemaphoreHandle lv_handler_lock;

extern void (*other_app)();

LV_FONT_DECLARE(chinese_hei_16);

extern lv_style_t style_chinese_16_white;
extern lv_style_t style_chinese_16_black;

#endif