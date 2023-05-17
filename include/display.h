#ifndef DISPLAY_H
#define DISPLAY_H

#include "head.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

#define blk 0

extern TFT_eSPI tft;
extern lv_disp_drv_t disp_drv;
extern uint32_t blk_value;
#define screenWidth 240
#define screenHeight 320

void display_init();
void touch_init();


#endif