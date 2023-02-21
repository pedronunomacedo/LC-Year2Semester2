#include "game_end_menu.h"

#include "assets/game/win_player1.xpm"
#include "assets/game/win_player2.xpm"
#include "assets/game/back_button.xpm"
#include "assets/game/back_button_hover.xpm"
#include "assets/game/draw.xpm"
#include "assets/mouse/mouse.xpm"

static sprite * mouse;
static sprite * win_player1;
static sprite * win_player2;
static sprite * back_button;
static sprite * back_button_hover;
static sprite * draw;
static int result;

int (initialize_game_end_menu)(void) {
    mouse = sprite_new(mouse_xpm, 0, 0);
    win_player1 = sprite_new(win_player1_xpm,0,0);
    win_player2 = sprite_new(win_player2_xpm,0,0);
    back_button = sprite_new(back_button_xpm,GAME_END_BTN_X,GAME_END_BTN_Y);
    back_button_hover = sprite_new(back_button_hover_xpm,GAME_END_BTN_X,GAME_END_BTN_Y);
    draw = sprite_new(draw_xpm,0,0);
    result = DRAW;
    if (mouse == NULL ||
        win_player1 == NULL || 
        win_player2 == NULL ||
        back_button == NULL ||
        back_button_hover == NULL ||
        draw == NULL) {
        return !OK;
    }
    return OK;
}

void (destroy_game_end_menu)(void) {
    sprite_destroy(mouse);
    sprite_destroy(win_player1);
    sprite_destroy(win_player2);
    sprite_destroy(back_button);
    sprite_destroy(back_button_hover);
    sprite_destroy(draw);
}

int (draw_game_end_menu)(void) {
    if (result == DRAW) {
        if (vg_draw_block_sprite_without_checks(draw) != OK) { return !OK; }
    } else if (result == PLAYER1) {
        if (vg_draw_block_sprite_without_checks(win_player1) != OK) { return !OK; }
    } else if (result == PLAYER2) {
        if (vg_draw_block_sprite_without_checks(win_player2) != OK) { return !OK; }
    } else { return !OK; }

    if (check_mouse_in_button(mouse, back_button)) {
        if (vg_draw_block_sprite_without_checks(back_button_hover) != OK) { return !OK; }
    } else {
        if (vg_draw_block_sprite_without_checks(back_button) != OK) { return !OK; }
    }

    if (vg_draw_sprite(mouse) != OK) { return !OK; }
    return OK;
}

void (set_result_game_end_menu)(int res) {
    result = res;
}

sprite * (get_mouse_sprite_game_end_menu)() {
    return mouse;
}

sprite * (get_back_sprite_game_end_menu)() {
    return back_button;
}
