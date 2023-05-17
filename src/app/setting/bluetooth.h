#pragma once
#include "head.h"
#define SERVICE_UUID "08e060d7-183b-d232-7f4d-19f5079b6c2c"
#define CHARACTERISTIC_UUID "413d2492-f93e-6871-9202-8ede7f290bfa"
extern String BLUETOOTH_name;
extern bool bluetooth_s;
#if 0
void bluetooth_init();
void bluetooth_close();
lv_obj_t*setting_bluetooth_load();
#endif