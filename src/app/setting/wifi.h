#pragma once
#include "head.h"
extern String WiFi_SSID;
extern String WiFi_Password;
extern bool wifi_s;
lv_obj_t*setting_wifi_load();
void kb_event1(lv_event_t* event);