#include"app/game1024/game1024.h"
uint16_t max_num=0;
uint16_t num_map_1024[4][4]={{0}};
lv_obj_t *btn_map_1024[4][4]={{NULL}};
lv_obj_t*base_1024;
void gesture_1024(lv_event_t*event);
void HSL2RGB(double h, double s, double L, unsigned char *rr, unsigned char *gg,

             unsigned char *bb)

{

    double v, m, sv, vs0, c1, c2, xiaoshu, r, g, b;

    int ih;
    
    L>=0?:L*=-1;
    r = L;
    g = L;
    b = L;

    if (L <= 0.5)
        v = L * (1.0 + s);
    else
        v = L + s - L * s;

    if (v > 0)
    {

        m = L + L - v;
        sv = (v - m) / v;

        h = h * 6.0;
        ih = (int)h;

        xiaoshu = h - ih;

        vs0 = v * sv * xiaoshu;
        c1 = m + vs0;
        c2 = v - vs0;

        switch (ih)
        {

        case 0:
            r = v;
            g = c1;
            b = m;
            break;

        case 1:
            r = c2;
            g = v;
            b = m;
            break;

        case 2:
            r = m;
            g = v;
            b = c1;
            break;

        case 3:
            r = m;
            g = c2;
            b = v;
            break;

        case 4:
            r = c1;
            g = m;
            b = v;
            break;

        case 5:
            r = v;
            g = m;
            b = c2;
            break;

        default:
            break;
        }
    }

    *rr = (int)(r * 0xff);
    *gg = (int)(g * 0xff);
    *bb = (int)(b * 0xff);
}
void size_1024(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t*)var, v, v);
}
int logs(int a,int x)
{
    return log(x)/log(a);
}

void game1024_renew_event(lv_event_t*event)
{
    lv_scr_load_anim(game1024_load(),LV_SCR_LOAD_ANIM_NONE,0,50,true);
}
void game1024_cancel_event(lv_event_t*event)
{
    lv_obj_t*obj=lv_event_get_target(event);
    lv_obj_del_async(lv_obj_get_parent(obj));
}
void random_create_1024()
{
    uint8_t code=random((unsigned int)time(NULL))%16;
    uint8_t x=code%4;
    uint8_t y=code/4;
    if(num_map_1024[x][y]==0&&btn_map_1024[x][y]==NULL)
    {
        uint16_t num=(uint8_t)random(1,3)*2;
        num_map_1024[x][y]=num;
        btn_map_1024[x][y]=lv_label_create(now_screen);
        lv_obj_t*num_label=lv_label_create(btn_map_1024[x][y]);
        lv_obj_set_size(btn_map_1024[x][y],60,60);
        lv_obj_set_style_radius(btn_map_1024[x][y],5,0);
        lv_obj_set_style_bg_opa(btn_map_1024[x][y],255,0);
        uint8_t r,g,b;
        
        HSL2RGB(132/240.0,240/240.0,(200-logs(2,num)*10)/240.0,&r,&g,&b);
        lv_obj_set_style_bg_color(btn_map_1024[x][y],lv_color_make(r,g,b),0);

        lv_obj_set_pos(btn_map_1024[x][y],x*60,80+y*60);
        lv_label_set_text(btn_map_1024[x][y],"");
        lv_label_set_text(num_label,String(num).c_str());
        lv_obj_set_style_text_align(num_label,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN);
        lv_obj_align_to(num_label,btn_map_1024[x][y],LV_ALIGN_CENTER,0,0);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, btn_map_1024[x][y]);
        lv_anim_set_values(&a, 0, 58);
        lv_anim_set_time(&a, 200);
        lv_anim_set_delay(&a,300);
        lv_anim_set_exec_cb(&a, size_1024);
        lv_anim_set_path_cb(&a, lv_anim_path_bounce);
        lv_anim_start(&a);
    }
    else
    {
        random_create_1024();
    }
    for(int y=0;y<4;y++)
    {
        for(int x=0;x<4;x++)
        {
            if(num_map_1024[x][y]==0)
            {
                return;
            }
        }
    }
    lv_obj_remove_event_cb(now_screen,gesture_1024);
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
    lv_obj_add_event_cb(renew_btn,game1024_renew_event,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(cancel_btn,game1024_cancel_event,LV_EVENT_CLICKED,NULL);
}
void move_1024_x(void * var, int32_t v)
{
    lv_obj_set_x((lv_obj_t*)var, v);
}
void move_1024_y(void * var, int32_t v)
{
    lv_obj_set_y((lv_obj_t*)var, v);
}

void gesture_1024(lv_event_t*event)
{
    lv_indev_wait_release(lv_indev_get_act());
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_LEFT)
    {
        bool create=0;
        for(int x=1;x<4;x++)
        {
            for(int y=0;y<4;y++)
            {
                if(btn_map_1024[x][y]!=NULL)
                {
                    int i=1;
                    while(x-i>=0&&btn_map_1024[x-i][y]==NULL)
                    {
                        //lv_obj_move_to(btn_map_1024[x][y-i],(x)*60,80+(y-i)*60);
                        i++;
                        create=1;
                    }
                    i--;
                    if(i)
                    {
                        num_map_1024[x-i][y]=num_map_1024[x][y];
                        num_map_1024[x][y]=0;
                        btn_map_1024[x-i][y]=btn_map_1024[x][y];
                        btn_map_1024[x][y]=NULL;
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x-i][y]);
                        lv_anim_set_values(&a, lv_obj_get_x(btn_map_1024[x-i][y]), (x-i)*60);
                        lv_anim_set_time(&a, 300);
                        lv_anim_set_exec_cb(&a, move_1024_x);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                    }
                    if(x-i-1>=0&&num_map_1024[x-i][y]==num_map_1024[x-i-1][y])
                    {
                        num_map_1024[x-i-1][y]+=num_map_1024[x-i][y];
                        num_map_1024[x-i][y]=0;
                        
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x-i][y]);
                        lv_anim_set_values(&a, 70, 58);
                        lv_anim_set_time(&a, 200);
                        lv_anim_set_exec_cb(&a, size_1024);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce);
                        lv_anim_start(&a);
                        lv_anim_set_values(&a, lv_obj_get_x(btn_map_1024[x-i][y]), (x-i-1)*60);
                        lv_anim_set_exec_cb(&a, move_1024_x);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                        
                        uint8_t r,g,b;
                        
                        HSL2RGB(132/240.0,240/240.0,(200-logs(2,num_map_1024[x-i-1][y])*10)/240.0,&r,&g,&b);
                        lv_obj_set_style_bg_color(btn_map_1024[x-i][y],lv_color_make(r,g,b),0);

                        lv_label_set_text(lv_obj_get_child(btn_map_1024[x-i][y],0),String(num_map_1024[x-i-1][y]).c_str());
                        lv_obj_del_async(btn_map_1024[x-i-1][y]);
                        btn_map_1024[x-i-1][y]=btn_map_1024[x-i][y];
                        btn_map_1024[x-i][y]=NULL;
                        //lv_obj_move_to(btn_map_1024[x-1][y],(x-1)*60,80+y*60);
                        create=1;
                    }
                }
            }
        }
        if(create)
        {
            random_create_1024();
        }
    }
    else if(dir == LV_DIR_RIGHT)
    {
        bool create=0;
        for(int x=2;x>=0;x--)
        {
            for(int y=0;y<4;y++)
            {
                if(btn_map_1024[x][y]!=NULL)
                {
                    int i=1;
                    while(x+i<=3&&btn_map_1024[x+i][y]==NULL)
                    {
                        //lv_obj_move_to(btn_map_1024[x][y-i],(x)*60,80+(y-i)*60);
                        i++;
                        create=1;
                    }
                    i--;
                    if(i)
                    {
                        num_map_1024[x+i][y]=num_map_1024[x][y];
                        num_map_1024[x][y]=0;
                        btn_map_1024[x+i][y]=btn_map_1024[x][y];
                        btn_map_1024[x][y]=NULL;
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x+i][y]);
                        lv_anim_set_values(&a, lv_obj_get_x(btn_map_1024[x+i][y]), (x+i)*60);
                        lv_anim_set_time(&a, 300);
                        lv_anim_set_exec_cb(&a, move_1024_x);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce);
                        lv_anim_start(&a);
                    }
                    if(x+i<=2&&num_map_1024[x+i][y]==num_map_1024[x+i+1][y])
                    {
                        num_map_1024[x+i+1][y]+=num_map_1024[x+i][y];
                        num_map_1024[x+i][y]=0;
                        
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x+i][y]);
                        lv_anim_set_values(&a, 70, 58);
                        lv_anim_set_time(&a, 200);
                        lv_anim_set_exec_cb(&a, size_1024);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce);
                        lv_anim_start(&a);
                        lv_anim_set_values(&a, lv_obj_get_x(btn_map_1024[x+i][y]), (x+i+1)*60);
                        lv_anim_set_exec_cb(&a, move_1024_x);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                        
                        uint8_t r,g,b;
                        
                        HSL2RGB(132/240.0,240/240.0,(200-logs(2,num_map_1024[x+i+1][y])*10)/240.0,&r,&g,&b);
                        lv_obj_set_style_bg_color(btn_map_1024[x+i][y],lv_color_make(r,g,b),0);


                        lv_label_set_text(lv_obj_get_child(btn_map_1024[x+i][y],0),String(num_map_1024[x+i+1][y]).c_str());
                        lv_obj_del_async(btn_map_1024[x+i+1][y]);
                        btn_map_1024[x+i+1][y]=btn_map_1024[x+i][y];
                        btn_map_1024[x+i][y]=NULL;
                        //lv_obj_move_to(btn_map_1024[x-1][y],(x-1)*60,80+y*60);
                        create=1;
                    }
                }
            }
        }
        if(create)
        {
            random_create_1024();
        }
    }
    else if (dir == LV_DIR_TOP)
    {
        bool create=0;
        for(int y=1;y<4;y++)
        {
            for(int x=0;x<4;x++)
            {
                if(btn_map_1024[x][y]!=NULL)
                {
                    int i=1;
                    while(y-i>=0&&btn_map_1024[x][y-i]==NULL)
                    {
                        //lv_obj_move_to(btn_map_1024[x][y-i],(x)*60,80+(y-i)*60);
                        i++;
                        create=1;
                    }
                    i--;
                    if(i)
                    {
                        num_map_1024[x][y-i]=num_map_1024[x][y];
                        num_map_1024[x][y]=0;
                        btn_map_1024[x][y-i]=btn_map_1024[x][y];
                        btn_map_1024[x][y]=NULL;
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x][y-i]);
                        lv_anim_set_values(&a, lv_obj_get_y(btn_map_1024[x][y-i]), 80+(y-i)*60);
                        lv_anim_set_time(&a, 300);
                        lv_anim_set_exec_cb(&a, move_1024_y);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                    }
                    if(y-i-1>=0&&num_map_1024[x][y-i]==num_map_1024[x][y-i-1])
                    {
                        num_map_1024[x][y-i-1]+=num_map_1024[x][y-i];
                        num_map_1024[x][y-i]=0;
                        
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x][y-i]);
                        lv_anim_set_values(&a, 70, 58);
                        lv_anim_set_time(&a, 200);
                        lv_anim_set_exec_cb(&a, size_1024);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce);
                        lv_anim_start(&a);
                        lv_anim_set_values(&a, lv_obj_get_y(btn_map_1024[x][y-i]), 80+(y-i-1)*60);
                        lv_anim_set_exec_cb(&a, move_1024_y);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                        uint8_t r,g,b;
                        
                        HSL2RGB(132/240.0,240/240.0,(200-logs(2,num_map_1024[x][y-i-1])*10)/240.0,&r,&g,&b);
                        lv_obj_set_style_bg_color(btn_map_1024[x][y-i],lv_color_make(r,g,b),0);

                        
                        lv_label_set_text(lv_obj_get_child(btn_map_1024[x][y-i],0),String(num_map_1024[x][y-i-1]).c_str());
                        lv_obj_del_async(btn_map_1024[x][y-i-1]);
                        btn_map_1024[x][y-i-1]=btn_map_1024[x][y-i];
                        btn_map_1024[x][y-i]=NULL;
                        //lv_obj_move_to(btn_map_1024[x-1][y],(x-1)*60,80+y*60);
                        create=1;
                    }
                }
            }
        }
        if(create)
        {
            random_create_1024();
        }
    }
    else if (dir == LV_DIR_BOTTOM)
    {
        bool create=0;
        for(int y=2;y>=0;y--)
        {
            for(int x=0;x<4;x++)
            {
                if(btn_map_1024[x][y]!=NULL)
                {
                    int i=1;
                    while(y+i<=3&&btn_map_1024[x][y+i]==NULL)
                    {
                        //lv_obj_move_to(btn_map_1024[x][y-i],(x)*60,80+(y-i)*60);
                        i++;
                        create=1;
                    }
                    i--;
                    if(i)
                    {
                        num_map_1024[x][y+i]=num_map_1024[x][y];
                        num_map_1024[x][y]=0;
                        btn_map_1024[x][y+i]=btn_map_1024[x][y];
                        btn_map_1024[x][y]=NULL;
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x][y+i]);
                        lv_anim_set_values(&a, lv_obj_get_y(btn_map_1024[x][y+i]), 80+(y+i)*60);
                        lv_anim_set_time(&a, 300);
                        lv_anim_set_exec_cb(&a, move_1024_y);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                    }
                    if(y+i<=2&&num_map_1024[x][y+i]==num_map_1024[x][y+i+1])
                    {
                        num_map_1024[x][y+i+1]+=num_map_1024[x][y+i];
                        num_map_1024[x][y+i]=0;
                        
                        lv_anim_t a;
                        lv_anim_init(&a);
                        lv_anim_set_var(&a, btn_map_1024[x][y+i]);
                        lv_anim_set_values(&a, 70, 58);
                        lv_anim_set_time(&a, 200);
                        lv_anim_set_exec_cb(&a, size_1024);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce);
                        lv_anim_start(&a);
                        lv_anim_set_values(&a, lv_obj_get_y(btn_map_1024[x][y+i]), 80+(y+i+1)*60);
                        lv_anim_set_exec_cb(&a, move_1024_y);
                        lv_anim_set_path_cb(&a, lv_anim_path_bounce );
                        lv_anim_start(&a);
                        uint8_t r,g,b;
                        
                        HSL2RGB(132/240.0,240/240.0,(200-logs(2,num_map_1024[x][y+i+1])*10)/240.0,&r,&g,&b);
                        lv_obj_set_style_bg_color(btn_map_1024[x][y+i],lv_color_make(r,g,b),0);

                        
                        lv_label_set_text(lv_obj_get_child(btn_map_1024[x][y+i],0),String(num_map_1024[x][y+i+1]).c_str());
                        lv_obj_del_async(btn_map_1024[x][y+i+1]);
                        btn_map_1024[x][y+i+1]=btn_map_1024[x][y+i];
                        btn_map_1024[x][y+i]=NULL;
                        //lv_obj_move_to(btn_map_1024[x-1][y],(x-1)*60,80+y*60);
                        create=1;
                    }
                }
            }
        }
        if(create)
        {
            random_create_1024();
        }
    }
    
    for(int y=0;y<4;y++)
    {
        for(int x=0;x<4;x++)
        {
            Serial.print("  ");
            Serial.print(num_map_1024[x][y]);
        }
        Serial.println();
    }
}

lv_obj_t*game1024_load()
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
    base_1024=lv_label_create(now_screen);
    lv_obj_set_style_radius(base_1024,5,0);
    lv_obj_set_style_bg_opa(base_1024,255,0);
    lv_obj_set_style_bg_color(base_1024,lv_color_make(195,195,195),LV_PART_MAIN);
    lv_label_set_text(base_1024,"");
    lv_obj_set_size(base_1024,240,245);
    lv_obj_align_to(base_1024,now_screen,LV_ALIGN_BOTTOM_MID,0,0);
    
    for(int y=0;y<4;y++)
    {
        for(int x=0;x<4;x++)
        {
            num_map_1024[x][y]=0;
            btn_map_1024[x][y]=NULL;
        }
    }
    random_create_1024();

    lv_obj_t*back=lv_btn_create(now_screen);
    lv_obj_set_size(back,45,45);
    lv_obj_align_to(back,now_screen,LV_ALIGN_TOP_LEFT,10,10);
    lv_obj_t*back_label=lv_label_create(back);
    lv_label_set_text(back_label,LV_SYMBOL_LEFT);
    lv_obj_align_to(back_label,back,LV_ALIGN_CENTER,0,0);
    lv_obj_add_event_cb(back,back_event,LV_EVENT_CLICKED,NULL);


    lv_obj_add_event_cb(now_screen,gesture_1024,LV_EVENT_GESTURE,NULL);
    return now_screen;
}