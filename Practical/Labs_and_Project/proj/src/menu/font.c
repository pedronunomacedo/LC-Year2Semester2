#include "font.h"

#include "assets/font/0.xpm"
#include "assets/font/1.xpm"
#include "assets/font/2.xpm"
#include "assets/font/3.xpm"
#include "assets/font/4.xpm"
#include "assets/font/5.xpm"
#include "assets/font/6.xpm"
#include "assets/font/7.xpm"
#include "assets/font/8.xpm"
#include "assets/font/9.xpm"
#include "assets/font/date_sep.xpm"
#include "assets/font/time_sep.xpm"

static sprite * num0;
static sprite * num1;
static sprite * num2;
static sprite * num3;
static sprite * num4;
static sprite * num5;
static sprite * num6;
static sprite * num7;
static sprite * num8;
static sprite * num9;
static sprite * date_sep;
static sprite * time_sep;


int (initialize_font)(void) {
    if ((num0 = sprite_new(num_0_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num1 = sprite_new(num_1_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num2 = sprite_new(num_2_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num3 = sprite_new(num_3_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num4 = sprite_new(num_4_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num5 = sprite_new(num_5_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num6 = sprite_new(num_6_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num7 = sprite_new(num_7_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num8 = sprite_new(num_8_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((num9 = sprite_new(num_9_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((date_sep = sprite_new(date_sep_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    if ((time_sep = sprite_new(time_sep_xpm,0,FIRST_Y)) == NULL)  { return !OK; }
    return OK;
}

void (destroy_font)(void) {
    sprite_destroy(num0);
    sprite_destroy(num1);
    sprite_destroy(num2);
    sprite_destroy(num3);
    sprite_destroy(num4);
    sprite_destroy(num5);
    sprite_destroy(num6);
    sprite_destroy(num7);
    sprite_destroy(num8);
    sprite_destroy(num9);
    sprite_destroy(date_sep);
    sprite_destroy(time_sep);
}

int (draw_date_font)(void) {
    char * current_date = rtc_get_current_date();

    int x = FIRST_X;

    for (int i = 0; i < 19; i++) {
        if (current_date[i] == ' ') {
            x += 30;
            continue;
        }
        sprite * sp = get_sprite_font(current_date[i]);
        if (sp == NULL) { continue; }
        sprite_set_pos(sp, x, FIRST_Y);
        if (vg_draw_sprite(sp) != OK) return !OK;
        x += sp->img.width;
    }
    return OK;
}

sprite * (get_sprite_font)(char c) {
    switch (c)
    {
    case '0':
        return num0;
        break;
    case '1':
        return num1;
        break;
    case '2':
        return num2;
        break;
    case '3':
        return num3;
        break;
    case '4':
        return num4;
        break;
    case '5':
        return num5;
        break;
    case '6':
        return num6;
        break;
    case '7':
        return num7;
        break;
    case '8':
        return num8;
        break;
    case '9':
        return num9;
        break;
    case '/':
        return date_sep;
        break;
    case ':':
        return time_sep;
        break;
    default:
        return NULL;
        break;
    }
}


