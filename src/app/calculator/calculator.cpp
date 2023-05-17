#include "app/calculator/calculator.h"
bool symbol_add=1;
String calculator_temp="";
String calculator_all="";
bool point_add=1;
char mode=0;
double num[5]={0.0};
int firstend=0,secondstart=0;
bool first_input=1;
lv_obj_t*num_text;
static void calculator_event(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_add_state(num_text,LV_STATE_FOCUSED);
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        String temp_key = lv_btnmatrix_get_btn_text(obj, id);
        temp_key=temp_key.substring(temp_key.length()-1,temp_key.length());
        Serial.println(temp_key);
        if(temp_key=="0"||temp_key=="1"||temp_key=="2"||temp_key=="3"||temp_key=="4"||temp_key=="5"||temp_key=="6"||temp_key=="7"||temp_key=="8"||temp_key=="9")
        {
            if(first_input) 
            {
                calculator_all.clear();
                first_input=0;
            }
            calculator_all+=temp_key;
        }
        else if(temp_key==".")
        {
            if(first_input) 
            {
                calculator_all.clear();
                first_input=0;
            }
            calculator_all+=temp_key;
        }
        else if(temp_key=="+"||temp_key=="-"||temp_key=="*"||temp_key=="/"||temp_key=="^"||temp_key=="%")
        {
            first_input=0;
            calculator_all+=temp_key;
        }
        else if(temp_key=="<")
        {
            if(first_input) 
            {
                calculator_all.clear();
                first_input=0;
            }
            calculator_all=calculator_all.substring(0, calculator_all.length() - 1);
        }
        else if(temp_key=="C")
        {
            calculator_all.clear();
        }
        else if(temp_key=="=")
        {
            first_input=1;
            char *strc=(char*)calculator_all.c_str();
            int len=0;
            while((strc[len]>='0'&&strc[len]<='9')||strc[len]=='.')
            {
                len++;
            }
            double first=atof(calculator_all.substring(0,len).c_str());
            double second=atof(calculator_all.substring(len+1,calculator_all.length()).c_str());
            switch(strc[len])
            {
                case '+':
                {
                    auto end=first+second;
                    calculator_all=String(end,5);
                }
                break;
                case '-':
                {
                    auto end=first-second;
                    calculator_all=String(end,5);
                }
                break;
                case '*':
                {
                    auto end=first*second;
                    calculator_all=String(end,5);
                }
                break;
                case '%':
                {
                    auto end=(int)(first)%(int)(second);
                    calculator_all=String(end,5);
                }
                break;
                case '/':
                {
                    if(second!=0)
                    {
                        auto end=first/second;
                        calculator_all=String(end,5);
                    }
                    else
                        calculator_all="error";
                }
                break;
                case '^':
                {
                    auto end=pow(first,second);
                    calculator_all=String(end,5);
                }
                break;
            }
            for(int i=calculator_all.length()-1;i>0&&(calculator_all.c_str()[i]=='0');i--)
            {
                calculator_all=calculator_all.substring(0,calculator_all.length()-1);
            }
            if((calculator_all.c_str()[calculator_all.length()-1]=='.')) 
                calculator_all=calculator_all.substring(0,calculator_all.length()-1);
        }
        lv_textarea_set_text(num_text,calculator_all.c_str());
        lv_textarea_set_align(num_text,LV_TEXT_ALIGN_RIGHT);
    }
    else if (code == LV_EVENT_DRAW_PART_BEGIN)
    {
        lv_obj_draw_part_dsc_t* dsc = lv_event_get_draw_part_dsc(e);
        if (dsc->class_p == &lv_btnmatrix_class && dsc->type == LV_BTNMATRIX_DRAW_PART_BTN)
        {
            if (dsc->id == 19)
            {
                if (lv_btnmatrix_get_selected_btn(obj) == dsc->id)
                    dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_BLUE, 3);
                else
                    dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_BLUE);
 
            }
            else if (dsc->id == 11)
            {
                if (lv_btnmatrix_get_selected_btn(obj) == dsc->id)
                    dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_RED, 3);
                else
                    dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_RED);
 
            }
        }
    }

}

lv_obj_t* calculator_load()
{
    now_screen =lv_obj_create(NULL);
    lv_color_t main_color;
    lv_color_t text_color;
    lv_color_t item_main_color;
    lv_color_t item_text_color;
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        text_color=lv_color_make(0,0,0);
        item_main_color=lv_color_make(222,222,222);
        item_text_color=lv_color_make(0,0,0);
        
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_hex(0xeeeeee);
        item_main_color=lv_color_make(50,50,50);
        item_text_color=lv_color_make(231,231,231);
    }
    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    lv_obj_t*keyboard=lv_btnmatrix_create(now_screen);
    static const char* keyname[] = {
    "#2f8edd +",    "#2f8edd -",    "#2f8edd *",    "#2f8edd /","\n",
    "7",            "8",            "9",            "#ff1c24 <","\n",
    "4",            "5",            "6",            "C",        "\n",
    "1",            "2",            "3",            "#2f8edd ^","\n",
    "#2f8edd %",    "0",            ".",            "=",        "" };
    lv_btnmatrix_set_map(keyboard,keyname);
    lv_obj_set_style_bg_color(keyboard,main_color,0);
    lv_obj_set_style_border_color(keyboard, main_color, LV_PART_MAIN);
    lv_obj_set_style_bg_color(keyboard, item_main_color, LV_PART_ITEMS);
    lv_obj_set_style_text_color(keyboard,item_text_color,LV_PART_ITEMS);
    lv_obj_set_style_shadow_width(keyboard,0,LV_PART_ITEMS);
    lv_btnmatrix_set_btn_ctrl_all(keyboard,LV_BTNMATRIX_CTRL_RECOLOR);
    
    
    num_text=lv_textarea_create(now_screen);
    lv_textarea_set_one_line(num_text, true);
    lv_obj_set_style_text_font(num_text,&lv_font_montserrat_16,0);
    lv_obj_set_style_text_color(num_text,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_size(num_text,220,40);
    lv_obj_align_to(num_text,now_screen,LV_ALIGN_TOP_MID,0,20);
    lv_textarea_set_align(num_text,LV_TEXT_ALIGN_RIGHT);
    lv_obj_add_state(num_text,LV_STATE_FOCUSED);

    lv_obj_set_size(keyboard,screenWidth,(screenHeight/3)*2);
    lv_obj_align(keyboard,LV_ALIGN_BOTTOM_MID,0,0);
    lv_obj_add_event_cb(keyboard, calculator_event, LV_EVENT_ALL, NULL);
    gesture(0,1,0,1);
    return now_screen;
}