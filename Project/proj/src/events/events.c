#include "events.h"

GAME_STATE (handle_timer_main_menu)() {
    if (draw_main_menu() == OK) {
        vg_swap_buffers();
    }
    return MAIN_MENU;
}

GAME_STATE (handle_keyboard_main_menu)(uint16_t scancode) {
    if (scancode == KBC_ESQ_BC) {
        return QUIT;
    }
    return MAIN_MENU;
}

GAME_STATE (handle_mouse_main_menu)(struct packet pp) {
    struct sprite * mouse = get_mouse_sprite_main_menu();
    sprite_set_pos_delta(mouse, pp.delta_x, pp.delta_y);
    if (pp.lb && !pp.rb && !pp.mb) {
        if (check_mouse_in_button(mouse, get_quit_sprite_main_menu())) {
            return QUIT;
        } else if (check_mouse_in_button(mouse, get_rules_sprite_main_menu())) {
            sprite_set_pos(get_mouse_sprite_rules_menu(), mouse->x, mouse->y);
            return RULES_MENU;
        } else if (check_mouse_in_button(mouse, get_play_sprite_main_menu())) {
            start_game();
            sprite_set_pos(get_mouse_sprite_pause_menu(), mouse->x, mouse->y);
            return IN_GAME;
        }
    }
    return MAIN_MENU;
}

GAME_STATE (handle_timer_rules_menu)() {
    if (draw_rules_menu() == OK) {
        vg_swap_buffers();
    }
    return RULES_MENU;
}

GAME_STATE (handle_keyboard_rules_menu)(uint16_t scancode) {
    if (scancode == KBC_ESQ_BC) {
        return MAIN_MENU;
    }
    return RULES_MENU;
}

GAME_STATE (handle_mouse_rules_menu)(struct packet pp) {
    struct sprite * mouse = get_mouse_sprite_rules_menu();
    sprite_set_pos_delta(mouse, pp.delta_x, pp.delta_y);
    if (pp.lb && !pp.rb && !pp.mb) {
        if (check_mouse_in_button(mouse, get_back_sprite_rules_menu())) {
            sprite_set_pos(get_mouse_sprite_main_menu(), mouse->x, mouse->y);
            return MAIN_MENU;
        }
    }
    return RULES_MENU;
}

GAME_STATE (handle_timer_pause_menu)() {
    if (draw_pause_menu() == OK) {
        vg_swap_buffers();
    }
    return PAUSE_MENU;
}

GAME_STATE (handle_keyboard_pause_menu)(uint16_t scancode) {
    if (scancode == KBC_ESQ_BC) {
        return IN_GAME;
    }
    return PAUSE_MENU;
}

GAME_STATE (handle_mouse_pause_menu)(struct packet pp) {
    struct sprite * mouse = get_mouse_sprite_pause_menu();
    sprite_set_pos_delta(mouse, pp.delta_x, pp.delta_y);
    if (pp.lb && !pp.rb && !pp.mb) {
        if (check_mouse_in_button(mouse, get_back_sprite_pause_menu())) {
            sprite_set_pos(get_mouse_sprite_main_menu(), mouse->x, mouse->y);
            return MAIN_MENU;
        }
        if (check_mouse_in_button(mouse,get_continue_sprite_pause_menu())) {
            return IN_GAME;
        }
    }
    return PAUSE_MENU;
}

GAME_STATE (handle_timer_game_end_menu)() {
    if (draw_game_end_menu() == OK) {
        vg_swap_buffers();
    }
    return END_GAME;
}

GAME_STATE (handle_keyboard_game_end_menu)(uint16_t scancode) {
    if (scancode == KBC_ESQ_BC) {
        return MAIN_MENU;
    }
    return END_GAME;
}

GAME_STATE (handle_mouse_game_end_menu)(struct packet pp) {
    struct sprite * mouse = get_mouse_sprite_game_end_menu();
    sprite_set_pos_delta(mouse, pp.delta_x, pp.delta_y);
    if (pp.lb && !pp.rb && !pp.mb) {
        if (check_mouse_in_button(mouse, get_back_sprite_game_end_menu())) {
            sprite_set_pos(get_mouse_sprite_main_menu(), mouse->x, mouse->y);
            return MAIN_MENU;
        }
    }
    return END_GAME;
}

GAME_STATE (handle_timer_game)() {
    if (draw_game() == OK) {
        vg_swap_buffers();
    }
    return IN_GAME;
}

GAME_STATE (handle_keyboard_game)(uint16_t scancode) {
    if (scancode == KBC_ESQ_BC) {
            return PAUSE_MENU;
    }
    if (get_turn_game() == PLAYER1) {
        if (scancode == KBC_LEFT_ARROW_MC) {
            game_set_column_left();
        } else if (scancode == KBC_RIGHT_ARROW_MC) {
            game_set_column_right();
        } else if (scancode == KBC_SPACE_BC) {
            if (game_move() != OK) { return IN_GAME; }
            return ANIMATION_GAME;
        }
    }
    return IN_GAME;
}

GAME_STATE (handle_mouse_game)(struct packet pp) {
    if (get_turn_game() == PLAYER2) {
        struct sprite * mouse = get_mouse_sprite_game();
        sprite_set_pos_delta(mouse, pp.delta_x, pp.delta_y);
        game_set_column_mouse();
        if (pp.lb && !pp.rb && !pp.mb) {
            if (game_move() != OK) { return IN_GAME; }
            return ANIMATION_GAME;
        }
    }
    return IN_GAME;
}

GAME_STATE (handle_timer_animation_game)() {
    if (draw_animation_game() == OK) {
        vg_swap_buffers();
    }
    if (check_animation_game_end()) {
        int result = check_game_end();
        if (result != 0) {
            set_result_game_end_menu(result);
            sprite_set_pos(get_mouse_sprite_game_end_menu(), get_mouse_sprite_game()->x, get_mouse_sprite_game()->y);
            return END_GAME;
        }
        next_turn();
        return IN_GAME;
    }
    return ANIMATION_GAME;
}
