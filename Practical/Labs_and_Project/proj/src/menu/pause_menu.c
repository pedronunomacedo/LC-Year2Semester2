#include "pause_menu.h"

#include "../assets/mouse/mouse.xpm"
#include "../assets/pause_menu/background.xpm"
#include "../assets/pause_menu/continue_button.xpm"
#include "../assets/pause_menu/continue_button_hover.xpm"
#include "../assets/pause_menu/back_button.xpm"
#include "../assets/pause_menu/back_button_hover.xpm"

static sprite * mouse_sprite;
static sprite * background_sprite;
static sprite * continue_button;
static sprite * continue_button_hover;
static sprite * back_button;
static sprite * back_button_hover;

int (initialize_pause_menu)(uint32_t x, uint32_t y) {
    mouse_sprite = sprite_new(mouse_xpm, x - MOUSE_SIDE, y + MOUSE_SIDE);
    background_sprite = sprite_new(background_xpm, 0, 0);
    back_button = sprite_new(back_button_xpm, PAUSE_BACK_BTN_X, PAUSE_BACK_BTN_Y);    
    back_button_hover = sprite_new(back_button_hover_xpm, PAUSE_BACK_BTN_X, PAUSE_BACK_BTN_Y);
    continue_button = sprite_new(continue_button_xpm, CONTINUE_BTN_X, CONTINUE_BTN_Y);    
    continue_button_hover = sprite_new(continue_button_hover_xpm, CONTINUE_BTN_X, CONTINUE_BTN_Y);
    if (mouse_sprite == NULL || 
        background_sprite == NULL ||
        back_button == NULL || 
        back_button_hover == NULL ||
        continue_button == NULL ||
        continue_button_hover == NULL) {
        return !OK;
    }
    return OK;
}

void (destroy_pause_menu)(void) {
    sprite_destroy(mouse_sprite);
    sprite_destroy(background_sprite);
    sprite_destroy(back_button);    
    sprite_destroy(back_button_hover);
    sprite_destroy(continue_button);    
    sprite_destroy(continue_button_hover);
}

int (draw_pause_menu)(void) {
    if (vg_draw_block_sprite_without_checks(background_sprite) != OK) { return !OK; }

    if (draw_date_font() != OK) { return !OK; }

    if (check_mouse_in_button(mouse_sprite, back_button)) {
        if (vg_draw_block_sprite_without_checks(back_button_hover) != OK) { return !OK; }
    } else {
        if (vg_draw_block_sprite_without_checks(back_button) != OK) { return !OK; }
    }

    if (check_mouse_in_button(mouse_sprite, continue_button)) {
        if (vg_draw_block_sprite_without_checks(continue_button_hover) != OK) { return !OK; }
    } else {
        if (vg_draw_block_sprite_without_checks(continue_button) != OK) { return !OK; }
    }

    if (vg_draw_sprite(mouse_sprite) != OK) { return !OK; }
    return OK;
}

sprite * get_mouse_sprite_pause_menu() { return mouse_sprite; }

sprite * get_back_sprite_pause_menu() { return back_button; }

sprite * get_continue_sprite_pause_menu() { return continue_button; }
