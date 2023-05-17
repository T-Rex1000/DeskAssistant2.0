#pragma once
#include "head.h"
extern struct tm time_date;
extern lv_obj_t * time_display[5];
lv_obj_t* time_load();
void time_update(void *p);