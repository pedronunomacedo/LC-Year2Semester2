#include "rules_menu.h"

#include "../assets/mouse/mouse.xpm"
#include "../assets/rules_menu/background.xpm"
#include "../assets/rules_menu/back_button.xpm"
#include "../assets/rules_menu/back_button_hover.xpm"

static sprite * mouse_sprite;
static sprite * background_sprite;
static sprite * back_button;
static sprite * back_button_hover;

int (initialize_rules_menu)(uint32_t x, uint32_t y) {
    mouse_sprite = sprite_new(mouse_xpm, x - MOUSE_SIDE, y + MOUSE_SIDE);
    background_sprite = sprite_new(background_xpm, 0, 0);
    back_button = sprite_new(back_button_xpm, BACK_BTN_X, BACK_BTN_Y);    
    back_button_hover = sprite_new(back_button_hover_xpm, BACK_BTN_X, BACK_BTN_Y);
    if (mouse_sprite == NULL || 
        background_sprite == NULL ||
        back_button == NULL || 
        back_button_hover == NULL) {
        return !OK;
    }
    return OK;
}

void (destroy_rules_menu)(void) {
    sprite_destroy(mouse_sprite);
    sprite_destroy(background_sprite);
    sprite_destroy(back_button);    
    sprite_destroy(back_button_hover);
}

int (draw_rules_menu)(void) {
    if (vg_draw_block_sprite_without_checks(background_sprite) != OK) { return !OK; }

    if (draw_date_font() != OK) { return !OK; }

    if (check_mouse_in_button(mouse_sprite, back_button)) {
        if (vg_draw_block_sprite_without_checks(back_button_hover) != OK) { return !OK; }
    } else {
        if (vg_draw_block_sprite_without_checks(back_button) != OK) { return !OK; }
    }

    if (vg_draw_sprite(mouse_sprite) != OK) { return !OK; }
    return OK;
}

sprite * get_mouse_sprite_rules_menu() { return mouse_sprite; }

sprite * get_back_sprite_rules_menu() { return back_button; }
