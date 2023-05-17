#include "app/paint/paint.h"
uint16_t tx[2]={0},ty[2]={0};
uint16_t ltx[2]={0},lty[2]={0};
bool pressed=0;
uint8_t release_time=0;
void read_touch(void*p)
{
    bool oldMODE=RUNMODE;
    while(oldMODE==RUNMODE)
    {
        tp.read();
        delay(30);
    }
    vTaskDelete(NULL);
}
void painting()
{
    while(!xSemaphoreTake(lv_handler_lock,portMAX_DELAY));
    uint16_t red=0,green=0,blue=0;
    pressed=0;
    bool oldpressed=0;
    tx[0]=0,ty[0]=0;
    tx[1]=0,ty[1]=0;
    uint16_t color=0;
    bool drawed=0;
    bool erase=0;
    tft.fillScreen(TFT_WHITE);
    tft.drawChar(8,10,'<',TFT_BLACK,TFT_WHITE,4);
    for(int i=0;i<153;i++)
    {
        tft.drawLine(i+43,0,i+43,15,tft.color565((uint16_t)i*1.67,0,0));
    }
    tft.drawCircle(43,7,3,TFT_WHITE);
    for(int i=0;i<153;i++)
    {
        tft.drawLine(i+43,15,i+43,30,tft.color565(0,(uint16_t)i*1.67,0));
    }
    tft.drawCircle(43,22,3,TFT_WHITE);
    for(int i=0;i<153;i++)
    {
        tft.drawLine(i+43,30,i+43,45,tft.color565(0,0,(uint16_t)i*1.67));
    }
    tft.drawCircle(43,37,3,TFT_WHITE);
    tft.fillRect(207,5,20,5,tft.color565(239,228,176));
    tft.fillRect(206,11,22,25,tft.color565(255,174,201));
    xTaskCreatePinnedToCore(read_touch,NULL,4096,NULL,10,NULL,0);
    tp.points[0].x=0;
    tp.points[0].y=0;
    tp.points[1].x=0;
    tp.points[1].y=0;
    while(1)
    {
        pressed = tp.isTouched;
        tx[0]=tp.points[0].x;
        ty[0]=tp.points[0].y;
        tx[1]=tp.points[1].x;
        ty[1]=tp.points[1].y;
        if(pressed&&ty[0]>55)
        {
            if(pow(pow(tx[0]-ltx[0],2)+pow(ty[0]-lty[0],2),0.5)<=50)
            {
                if(erase)
                {
                    tft.fillCircle(tx[0],ty[0],10,TFT_WHITE);
                    if(oldpressed&&lty[0]>55) 
                    {
                        for(int i=0;i<7;i++)
                        {
                            tft.drawLine(tx[0]-i,ty[0],ltx[0]-i,lty[0],TFT_WHITE);
                            tft.drawLine(tx[0]+i,ty[0],ltx[0]+i,lty[0],TFT_WHITE);
                            tft.drawLine(tx[0],ty[0]-i,ltx[0],lty[0]-i,TFT_WHITE);
                            tft.drawLine(tx[0],ty[0]+i,ltx[0],lty[0]+i,TFT_WHITE);
                        }
                    }
                    else
                        Serial.print("nnnnnnnnnnnnnnnnnn");
                    
                }
                else
                {
                    //tft.drawPixel(tx[0],ty[0],color);
                    tft.fillCircle(tx[0],ty[0],2,color);
                    
                    if(oldpressed&&lty[0]>55) 
                    {
                        for(int i=0;i<3;i++)
                        {
                            tft.drawLine(tx[0]-i,ty[0],ltx[0]-i,lty[0],color);
                            tft.drawLine(tx[0]+i,ty[0],ltx[0]+i,lty[0],color);
                            tft.drawLine(tx[0],ty[0]-i,ltx[0],lty[0]-i,color);
                            tft.drawLine(tx[0],ty[0]+i,ltx[0],lty[0]+i,color);
                        }
                    }
                    
                    drawed=1;
                }
            }

            if(tp.touches>=2&&(pow(tx[1]-ltx[1],2)+pow(ty[1]-lty[1],2),0.5)<=50)
            {
                if(erase)
                {
                    tft.fillCircle(tx[1],ty[1],10,TFT_WHITE);
                    if(oldpressed&&lty[1]>55) 
                    {
                        for(int i=0;i<7;i++)
                        {
                            tft.drawLine(tx[1]-i,ty[1],ltx[1]-i,lty[1],TFT_WHITE);
                            tft.drawLine(tx[1]+i,ty[1],ltx[1]+i,lty[1],TFT_WHITE);
                            tft.drawLine(tx[1],ty[1]-i,ltx[1],lty[1]-i,TFT_WHITE);
                            tft.drawLine(tx[1],ty[1]+i,ltx[1],lty[1]+i,TFT_WHITE);
                        }
                    }
                    else
                        Serial.print("nnnnnnnnnnnnnnnnnn");
                    
                }
                else
                {
                    //tft.drawPixel(tx[0],ty[0],color);
                    tft.fillCircle(tx[1],ty[1],2,color);
                    
                    if(oldpressed&&lty[1]>55) 
                    {
                        for(int i=0;i<3;i++)
                        {
                            tft.drawLine(tx[1]-i,ty[1],ltx[1]-i,lty[1],color);
                            tft.drawLine(tx[1]+i,ty[1],ltx[1]+i,lty[1],color);
                            tft.drawLine(tx[1],ty[1]-i,ltx[1],lty[1]-i,color);
                            tft.drawLine(tx[1],ty[1]+i,ltx[1],lty[1]+i,color);
                        }
                    }
                    
                    drawed=1;
                }
            }
        }
        else if(pressed)
        {
            if(tx[0]>43&&tx[0]<196)
            {
                if(ty[0]>0&&ty[0]<15) 
                {
                    red=(uint16_t)((tx[0]-43)*1.67);
                    for(int i=0;i<153;i++)
                    {
                        tft.drawLine(i+43,0,i+43,15,tft.color565((uint16_t)i*1.67,0,0));
                    }
                    tft.drawCircle(tx[0],7,3,TFT_WHITE);
                }
                if(ty[0]>15&&ty[0]<30)
                {
                    green=(uint16_t)((tx[0]-43)*1.67);
                    for(int i=0;i<153;i++)
                    {
                        tft.drawLine(i+43,15,i+43,30,tft.color565(0,(uint16_t)i*1.67,0));
                    }
                    tft.drawCircle(tx[0],22,3,TFT_WHITE);
                }
                if(ty[0]>30&&ty[0]<45)
                {
                    blue=(uint16_t)((tx[0]-43)*1.67);
                    for(int i=0;i<153;i++)
                    {
                        tft.drawLine(i+43,30,i+43,45,tft.color565(0,0,(uint16_t)i*1.67));
                    }
                    tft.drawCircle(tx[0],37,3,TFT_WHITE);
                }
                color=tft.color565(red,green,blue);
            }
        }
        else if(oldpressed&&!pressed)
        {
            if(lty[0]>0&&lty[0]<45)
            {
                if(ltx[0]>0&&ltx[0]<40)
                {
                    if(!drawed)
                    {
                        ltx[0]=lty[0]=0;
                        use_lvgl=1;
                        xSemaphoreGive(lv_handler_lock);
                        app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,300,50);
                        return;
                    }
                    else
                    {
                        drawed=0;
                        tft.fillRect(0,45,239,274,TFT_WHITE);
                    }
                }
                if(ltx[0]>194&&ltx[0]<240)
                {
                    if(erase)
                    {
                        erase=0;
                        tft.drawRect(196,1,43,43,TFT_WHITE);
                    }
                    else 
                    {
                        erase=1;
                        tft.drawRect(196,1,43,43,TFT_VIOLET);
                    }
                }
            }
        }
        oldpressed=pressed;
        if(oldpressed&&pressed)
        {
            ltx[0]=tx[0];
            lty[0]=ty[0];
            if(tp.touches>=2)
            {
                ltx[1]=tx[1];
                lty[1]=ty[1];
            }
        }
        if(!oldpressed)
            Serial.println("release");
    }
}
lv_obj_t*paint_load()
{
    now_screen =lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        text_color=lv_color_hex(0x000000);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_hex(0xeeeeee);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 
    
    return now_screen;
}