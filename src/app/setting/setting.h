#pragma once
#include "head.h"
#include "app/setting/wifi.h"
#include "app/setting/wifiAP.h"
#include "app/setting/bluetooth.h"
#include "app/setting/theme.h"
#include "app/setting/lock.h"
#include "app/setting/about.h"

extern lv_obj_t *setting_title;
lv_obj_t* setting_load();