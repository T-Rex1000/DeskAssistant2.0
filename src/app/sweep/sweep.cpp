#include "app/sweep/sweep.h"
#define bomb_code 9
#define bomb_num 10
#define sweep_obj_get_num(obj) sweep_map[(lv_obj_get_x(obj)-0)/30][(lv_obj_get_y(obj)-40)/30]
uint8_t sweep_map[8][8]={{0}};
lv_obj_t* sweep_btn_map[8][8];
struct BOMB
{
	uint8_t x;
	uint8_t y;
}bomb[bomb_num];
uint8_t open_num=0;
void back_event(lv_event_t*event)
{
    app_return(LV_SCR_LOAD_ANIM_MOVE_RIGHT,200,10);
}
void sweep_renew_event(lv_event_t*event)
{
    lv_scr_load_anim(sweep_load(),LV_SCR_LOAD_ANIM_NONE,0,50,true);
}
void sweep_cancel_event(lv_event_t*event)
{
    lv_obj_t*obj=lv_event_get_target(event);
    lv_obj_del_async(lv_obj_get_parent(obj));
}
void sweep_win()
{
    lv_obj_t*game_msg = lv_label_create(now_screen);
    lv_obj_t*win_title=lv_label_create(game_msg);
    lv_obj_set_size(game_msg,180,120);
    lv_obj_set_style_text_font(win_title, &chinese_hei_16, 0);
    lv_obj_align(game_msg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(win_title, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_bg_opa(game_msg,200,0);
    lv_obj_set_style_bg_color(game_msg,lv_color_make(220,220,220),0);
    lv_obj_set_style_text_color(win_title,lv_color_make(0,0,0),0);
    lv_obj_set_style_text_align(win_title,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(game_msg,"");
    lv_label_set_text(win_title,"恭喜通关!");
    lv_obj_t*back_btn=lv_label_create(game_msg);
    lv_obj_t*renew_btn=lv_label_create(game_msg);
    lv_obj_add_flag(back_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(renew_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(back_btn,90,40);
    lv_obj_set_size(renew_btn,90,40);
    lv_obj_align(back_btn,LV_ALIGN_BOTTOM_LEFT,0,0);
    lv_obj_align(renew_btn,LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_obj_set_style_text_font(back_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_font(renew_btn, &chinese_hei_16, 0);
    lv_obj_set_style_text_color(back_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_color(renew_btn,lv_color_make(0,162,232),0);
    lv_obj_set_style_text_align(back_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_obj_set_style_text_align(renew_btn,LV_TEXT_ALIGN_CENTER,0);
    lv_label_set_text(back_btn,"退出游戏");
    lv_label_set_text(renew_btn,"重新开始");

    lv_obj_t*cancel_btn=lv_btn_create(game_msg);
    lv_obj_set_size(cancel_btn,30,30);
    lv_obj_align_to(cancel_btn,game_msg,LV_ALIGN_TOP_RIGHT,-5,5);
    lv_obj_t*cancel_btn_label=lv_label_create(cancel_btn);
    lv_label_set_text(cancel_btn_label,LV_SYMBOL_CLOSE);
    lv_obj_align_to(cancel_btn_label,cancel_btn,LV_ALIGN_CENTER,0,0);

    lv_obj_add_event_cb(back_btn,back_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(renew_btn,sweep_renew_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(cancel_btn,sweep_cancel_event,LV_EVENT_CLICKED,NULL);
}

void sweep_open(uint8_t x,uint8_t y)
{
    if(sweep_map[x][y] == 0)
    {
        if (y - 1 >= 0 &&sweep_btn_map[x][y - 1]!=NULL&& sweep_map[x][y - 1] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x][y - 1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x][y - 1]);
            sweep_btn_map[x][y - 1]=NULL;
            sweep_open(x,y-1);
            open_num++;
        }
        else if(y - 1 >= 0 &&sweep_btn_map[x][y - 1]!=NULL&& sweep_map[x][y - 1] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x][y - 1]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x][y - 1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x][y - 1]);
            sweep_btn_map[x][y- 1]=NULL;
            open_num++;
        }
        if (y + 1 < 8 &&sweep_btn_map[x][y + 1]!=NULL&& sweep_map[x][y + 1] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x][y + 1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x][y + 1]);
            sweep_btn_map[x][y + 1]=NULL;
            sweep_open(x,y+1);
            open_num++;
        }
        else if(y + 1 <8 &&sweep_btn_map[x][y + 1]!=NULL&& sweep_map[x][y + 1] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x][y + 1]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x][y + 1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x][y + 1]);
            sweep_btn_map[x][y + 1]=NULL;
            open_num++;
        }
        if (x - 1 >= 0 &&sweep_btn_map[x-1][y]!=NULL&& sweep_map[x - 1][y] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x-1][y],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x-1][y]);
            sweep_btn_map[x-1][y]=NULL;
            sweep_open(x-1,y);
            open_num++;
        }
        else if(x - 1 >= 0 &&sweep_btn_map[x-1][y]!=NULL&& sweep_map[x - 1][y] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x-1][y]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x-1][y],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x-1][y]);
            sweep_btn_map[x-1][y]=NULL;
            open_num++;
        }
        if (x + 1 < 8 &&sweep_btn_map[x+1][y]!=NULL&& sweep_map[x + 1][y] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x+1][y],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x+1][y]);
            sweep_btn_map[x+1][y]=NULL;
            sweep_open(x+1,y);
            open_num++;
        }
        else if(x + 1 < 8 &&sweep_btn_map[x + 1][y]!=NULL&& sweep_map[x + 1][y] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x+1][y]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x+1][y],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x+1][y]);
            sweep_btn_map[x+1][y]=NULL;
            open_num++;
        }
        if (x - 1 >= 0 && y - 1 >= 0 &&sweep_btn_map[x-1][y-1]!=NULL&& sweep_map[x - 1][y - 1] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x-1][y-1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x-1][y-1]);
            sweep_btn_map[x-1][y-1]=NULL;
            sweep_open(x-1,y-1);
            open_num++;
        }
        else if(x - 1 >= 0 && y - 1 >= 0 &&sweep_btn_map[x-1][y-1]!=NULL&& sweep_map[x - 1][y - 1] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x-1][y-1]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x-1][y-1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x-1][y-1]);
            sweep_btn_map[x-1][y-1]=NULL;
            open_num++;
        }
        if (x + 1 < 8 && y - 1 >= 0 &&sweep_btn_map[x+1][y-1]!=NULL&& sweep_map[x + 1][y - 1] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x+1][y-1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x+1][y-1]);
            sweep_btn_map[x+1][y-1]=NULL;
            sweep_open(x+1,y-1);
            open_num++;
        }
        else if(x + 1 < 8 && y - 1 >= 0 &&sweep_btn_map[x+1][y-1]!=NULL&& sweep_map[x+1][y-1] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x+1][y-1]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x+1][y-1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x+1][y-1]);
            sweep_btn_map[x+1][y-1]=NULL;
            open_num++;
        }
        if (x - 1 >= 0 && y + 1 < 8 &&sweep_btn_map[x-1][y+1]!=NULL&& sweep_map[x - 1][y + 1] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x-1][y+1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x-1][y+1]);
            sweep_btn_map[x-1][y+1]=NULL;
            sweep_open(x-1,y+1);
            open_num++;
        }
        else if(x - 1 >= 0 && y + 1 < 8 &&sweep_btn_map[x-1][y+1]!=NULL&& sweep_map[x-1][y+1] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x-1][y+1]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x-1][y+1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x-1][y+1]);
            sweep_btn_map[x-1][y+1]=NULL;
            open_num++;
        }
        if (x + 1 < 8 && y + 1 < 8 &&sweep_btn_map[x+1][y+1]!=NULL&& sweep_map[x + 1][y + 1] == 0)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label,"");
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x+1][y+1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x+1][y+1]);
            sweep_btn_map[x+1][y+1]=NULL;
            sweep_open(x+1,y+1);
            open_num++;
        }
        else if(x + 1 < 8 && y + 1 < 8 &&sweep_btn_map[x+1][y+1]!=NULL&& sweep_map[x+1][y + 1] != bomb_code)
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(sweep_map[x+1][y+1]).c_str());
            lv_obj_align_to(bomb_num_label,sweep_btn_map[x+1][y+1],LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(sweep_btn_map[x+1][y+1]);
            sweep_btn_map[x+1][y+1]=NULL;
            open_num++;
        }
    }
}

void open_btn(lv_event_t*event)
{
    uint8_t code=lv_event_get_code(event);
    lv_obj_t*btn=lv_event_get_target(event);
    if(code==LV_EVENT_CLICKED)
    {
        uint8_t num=sweep_obj_get_num(btn);
        uint8_t i=(lv_obj_get_x(btn)-0)/30;
        uint8_t j=(lv_obj_get_y(btn)-40)/30;
        if(num>=1&&num<=8) 
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_obj_set_style_text_color(bomb_num_label,text_color,LV_PART_MAIN);
            lv_label_set_text(bomb_num_label,String(num).c_str());
            lv_obj_align_to(bomb_num_label,btn,LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(btn);
            sweep_btn_map[i][j]=NULL;
            open_num++;
            if(open_num>=64-bomb_num)
            {
                for(int k=0;k<bomb_num;k++)
                {
                    lv_obj_add_state(sweep_btn_map[bomb[k].x][bomb[k].y],LV_STATE_DISABLED);
                }
                sweep_win();
            }
        }
        else if(num==0) //continue open
        {
            lv_obj_t *bomb_num_label=lv_label_create(now_screen);
            lv_label_set_text(bomb_num_label," ");
            lv_obj_align_to(bomb_num_label,btn,LV_ALIGN_CENTER,0,0);

            lv_obj_del_async(btn);
            sweep_btn_map[i][j]=NULL;
            open_num++;
            sweep_open(i,j);
            if(open_num>=64-bomb_num)
            {
                for(int k=0;k<bomb_num;k++)
                {
                    lv_obj_add_state(sweep_btn_map[bomb[k].x][bomb[k].y],LV_STATE_DISABLED);
                }
                sweep_win();
            }
        }
        else if(num==bomb_code) //over
        {
            for(int k=0;k<bomb_num;k++)
            {
                lv_obj_t *bomb_num_label=lv_label_create(now_screen);
                lv_obj_set_style_text_color(bomb_num_label,text_color,0);
                lv_label_set_text(bomb_num_label,"X");
                lv_obj_align_to(bomb_num_label,sweep_btn_map[bomb[k].x][bomb[k].y],LV_ALIGN_CENTER,0,0);
                lv_obj_del_async(sweep_btn_map[bomb[k].x][bomb[k].y]);
                sweep_btn_map[bomb[k].x][bomb[k].y]=NULL;
            }
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    if(sweep_btn_map[i][j]!=NULL)
                    {
                        lv_obj_add_state(sweep_btn_map[i][j],LV_STATE_DISABLED);
                    }
                }
            }
            lv_obj_t*game_msg = lv_label_create(now_screen);
            lv_obj_t*win_title=lv_label_create(game_msg);
            lv_obj_set_size(game_msg,180,120);
            lv_obj_set_style_text_font(win_title, &chinese_hei_16, 0);
            lv_obj_align(game_msg, LV_ALIGN_CENTER, 0, 0);
            lv_obj_align(win_title, LV_ALIGN_TOP_MID, 0, 30);
            lv_obj_set_style_bg_opa(game_msg,200,0);
            lv_obj_set_style_bg_color(game_msg,lv_color_make(220,220,220),0);
            lv_obj_set_style_text_color(win_title,lv_color_make(0,0,0),0);
            lv_obj_set_style_text_align(win_title,LV_TEXT_ALIGN_CENTER,0);
            lv_label_set_text(game_msg,"");
            lv_label_set_text(win_title,"是否重新开始");
            lv_obj_t*back_btn=lv_label_create(game_msg);
            lv_obj_t*renew_btn=lv_label_create(game_msg);
            lv_obj_add_flag(back_btn,LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(renew_btn,LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_size(back_btn,90,40);
            lv_obj_set_size(renew_btn,90,40);
            lv_obj_align(back_btn,LV_ALIGN_BOTTOM_LEFT,0,0);
            lv_obj_align(renew_btn,LV_ALIGN_BOTTOM_RIGHT,0,0);
            lv_obj_set_style_text_font(back_btn, &chinese_hei_16, 0);
            lv_obj_set_style_text_font(renew_btn, &chinese_hei_16, 0);
            lv_obj_set_style_text_color(back_btn,lv_color_make(0,162,232),0);
            lv_obj_set_style_text_color(renew_btn,lv_color_make(0,162,232),0);
            lv_obj_set_style_text_align(back_btn,LV_TEXT_ALIGN_CENTER,0);
            lv_obj_set_style_text_align(renew_btn,LV_TEXT_ALIGN_CENTER,0);
            lv_label_set_text(back_btn,"退出游戏");
            lv_label_set_text(renew_btn,"重新开始");

            lv_obj_t*cancel_btn=lv_btn_create(game_msg);
            lv_obj_set_size(cancel_btn,30,30);
            lv_obj_align_to(cancel_btn,game_msg,LV_ALIGN_TOP_RIGHT,-5,5);
            lv_obj_t*cancel_btn_label=lv_label_create(cancel_btn);
            lv_label_set_text(cancel_btn_label,LV_SYMBOL_CLOSE);
            lv_obj_align_to(cancel_btn_label,cancel_btn,LV_ALIGN_CENTER,0,0);

            lv_obj_add_event_cb(back_btn,back_event,LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(renew_btn,sweep_renew_event,LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(cancel_btn,sweep_cancel_event,LV_EVENT_CLICKED,NULL);
        }
    }
}

void map()
{
    open_num=0;
    for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			sweep_map[x][y] = 0;
            sweep_btn_map[x][y]=lv_btn_create(now_screen);
            lv_obj_set_style_radius(sweep_btn_map[x][y],5,LV_PART_MAIN);
            lv_obj_set_size(sweep_btn_map[x][y],29,29);
            lv_obj_set_pos(sweep_btn_map[x][y],0+x*30,40+y*30);
            lv_obj_add_event_cb(sweep_btn_map[x][y],open_btn,LV_EVENT_ALL,NULL);
		}
	}
    
	int i, x, y;
	srand((unsigned int)time(NULL));
	for (i = 0; i < bomb_num; i++)
	{
		x = rand() % 8;
		y = rand() % 8;
		if (sweep_map[x][y] == 0)
		{
			sweep_map[x][y] = bomb_code;
			bomb[i].x = x;
			bomb[i].y = y;
		}
		else
			i--;
	}
    uint8_t b=0;
    for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if(sweep_map[x][y] == 0)
            {
				if (y - 1 >= 0 && sweep_map[x][y - 1] == bomb_code)
					b++;
				if (y + 1 < 8 && sweep_map[x][y + 1] == bomb_code)
					b++;
				if (x - 1 >= 0 && sweep_map[x - 1][y] == bomb_code)
					b++;
				if (x + 1 < 8 && sweep_map[x + 1][y] == bomb_code)
					b++;
				if (x - 1 >= 0 && y - 1 >= 0 && sweep_map[x - 1][y - 1] == bomb_code)
					b++;
				if (x + 1 < 8 && y - 1 >= 0 && sweep_map[x + 1][y - 1] == bomb_code)
					b++;
				if (x - 1 >= 0 && y + 1 < 8 && sweep_map[x - 1][y + 1] == bomb_code)
					b++;
				if (x + 1 < 8 && y + 1 < 8 && sweep_map[x + 1][y + 1] == bomb_code)
					b++;

				sweep_map[x][y] = b;

				b = 0;
            }
		}
	}
}

lv_obj_t*sweep_load()
{
    now_screen =lv_obj_create(NULL);
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

    map();

    gesture(0,0,0,1);
    return now_screen;
}