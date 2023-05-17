#include "touch.h"

#ifdef FT6336U

void touch_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    tp = ft6336u.scan(); 
    if(tp.status()== release )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = tp.tp[0].x;
        data->point.y = tp.tp[0].y;

        Serial.print( "Data x " );
        Serial.println( tp.tp[0].x );

        Serial.print( "Data y " );
        Serial.println( tp.tp[0].y );
    }
}

void touch_init()
{
    ft6336u.begin();
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    lv_indev_drv_register( &indev_drv );  
}
#endif

#ifdef GT911

TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);

void touch_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    tp.read();
    //static uint8_t release_val=0;
    if( !tp.isTouched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        //Serial.println(release_val);
        //release_val=0;
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = tp.points[0].x;
        data->point.y = tp.points[0].y;
        
        Serial.print( "Data x " );
        Serial.println( tp.points[0].x );

        Serial.print( "Data y " );
        Serial.println( tp.points[0].y );
        
    }
}

void touch_init()
{
    tp.begin();
    tp.setRotation(1);
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    lv_indev_drv_register( &indev_drv );    
}
#endif