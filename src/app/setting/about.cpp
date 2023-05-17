
#include "app/setting/about.h"
lv_obj_t*about_load()
{
    now_screen =lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        text_color=lv_color_make(0,0,0);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_make(245,245,245);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    lv_obj_t *logo=lv_img_create(now_screen);
    lv_img_set_src(logo,&LOGO);
    lv_obj_align_to(logo,now_screen,LV_ALIGN_TOP_MID,0,15);

    lv_obj_t*core=lv_label_create(now_screen);
    lv_obj_set_style_text_font(core,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(core,text_color,LV_PART_MAIN);
    lv_obj_align_to(core,now_screen,LV_ALIGN_TOP_LEFT,20,115);
    lv_label_set_text(core,"处理器:");
    
    lv_obj_t*ram=lv_label_create(now_screen);
    lv_obj_set_style_text_font(ram,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(ram,text_color,LV_PART_MAIN);
    lv_obj_align_to(ram,core,LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    lv_label_set_text(ram,"运存:");
    
    lv_obj_t*rom=lv_label_create(now_screen);
    lv_obj_set_style_text_font(rom,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(rom,text_color,LV_PART_MAIN);
    lv_obj_align_to(rom,ram,LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    lv_label_set_text(rom,"储存:");

    lv_obj_t*screen=lv_label_create(now_screen);
    lv_obj_set_style_text_font(screen,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(screen,text_color,LV_PART_MAIN);
    lv_obj_align_to(screen,rom,LV_ALIGN_OUT_BOTTOM_LEFT,0,30);
    lv_label_set_text(screen,"屏幕:");

    lv_obj_t*author=lv_label_create(now_screen);
    lv_obj_set_style_text_font(author,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(author,text_color,LV_PART_MAIN);
    lv_obj_align_to(author,screen,LV_ALIGN_OUT_BOTTOM_LEFT,0,30);
    lv_label_set_text(author,"作者:");

    
    lv_obj_t*core_detail=lv_label_create(now_screen);
    lv_obj_set_style_text_font(core_detail,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(core_detail,text_color,LV_PART_MAIN);
    lv_obj_align_to(core_detail,core,LV_ALIGN_OUT_RIGHT_MID,20,0);
    lv_label_set_text(core_detail,"ESP32");

    lv_obj_t*ram_detail=lv_label_create(now_screen);
    lv_obj_set_style_text_font(ram_detail,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(ram_detail,text_color,LV_PART_MAIN);
    lv_obj_align_to(ram_detail,core_detail,LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    lv_label_set_text(ram_detail,"520KB");

    lv_obj_t*rom_detail=lv_label_create(now_screen);
    lv_obj_set_style_text_font(rom_detail,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(rom_detail,text_color,LV_PART_MAIN);
    lv_obj_align_to(rom_detail,ram_detail,LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    lv_label_set_text(rom_detail,("flash: 4MB\nTF:"+String(SD.cardSize() / (1024 * 1024))+"MB").c_str());

    lv_obj_t*screen_detail=lv_label_create(now_screen);
    lv_obj_set_style_text_font(screen_detail,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(screen_detail,text_color,LV_PART_MAIN);
    lv_obj_align_to(screen_detail,rom_detail,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_label_set_text(screen_detail,"2.8寸LCD(240X320)\n电容触摸屏");
    
    lv_obj_t*author_name=lv_label_create(now_screen);
    lv_obj_set_style_text_font(author_name,&chinese_hei_16,LV_PART_MAIN);
    lv_obj_set_style_text_color(author_name,text_color,LV_PART_MAIN);
    lv_obj_align_to(author_name,screen_detail,LV_ALIGN_OUT_BOTTOM_LEFT,0,7);
    lv_label_set_text(author_name,"T-Rex1000(LCC)");

    gesture(0,1,0,1);
    return now_screen;
}