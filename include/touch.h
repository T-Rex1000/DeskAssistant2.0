#ifndef TOUCH_H
#define TOUCH_H

#include "head.h"

//#define FT6336U
#define GT911


#ifdef FT6336U

        #include <FT6336U.h>
        #include <stdio.h>

        #define I2C_SDA 22
        #define I2C_SCL 23
        #define RST_N_PIN 21
        #define INT_N_PIN 34

        FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);

        FT6336U_TouchPointType tp;

    #endif

    #ifdef GT911
        
        #include <TAMC_GT911.h>

        #define TOUCH_RST 34
        #define TOUCH_INT 35
        #define TOUCH_SDA  21
        #define TOUCH_SCL  22
        #define TOUCH_WIDTH  320
        #define TOUCH_HEIGHT 240

        extern TAMC_GT911 tp;

    #endif


void touch_init();

#endif