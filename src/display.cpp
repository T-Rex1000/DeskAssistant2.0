#include "display.h"
TFT_eSPI tft = TFT_eSPI();
lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 100 ];
uint32_t blk_value=pow(2,6);
//static lv_color_t buf1[ screenWidth * 80 ];
//static lv_color_t buf2[ screenWidth * 80 ];
void disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

void display_init()
{
    pinMode(33, OUTPUT);
    digitalWrite(33,1);
    ledcSetup(blk, 5000, 12);
    ledcAttachPin(33, blk);
    ledcWrite(blk,blk_value);
    lv_init();
    tft.begin();
    tft.setRotation(0);
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 100 );
    //lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * 80);
    
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
    tft.fillScreen(TFT_BLACK);
}