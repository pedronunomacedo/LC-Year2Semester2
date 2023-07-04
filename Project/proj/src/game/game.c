#include "game.h"

#include "assets/game/board.xpm"
#include "assets/game/turn_player1.xpm"
#include "assets/game/turn_player2.xpm"
#include "assets/game/pieces/blue_piece.xpm"
#include "assets/game/pieces/red_piece.xpm"
#include "assets/mouse/mouse.xpm"

static game * state;
static sprite * board;
static sprite * mouse;
static sprite * turn_player1;
static sprite * turn_player2;
static sprite * blue_piece;
static sprite * red_piece;
static anim * animation;
static uint32_t init_x_pos;
static uint32_t init_y_pos;
static bool in_animation;

int (initialize_game)(uint32_t x, uint32_t y) {
    state = (game *)malloc(sizeof(game));
    if (state == NULL) { return !OK; }

    animation = (anim *)malloc(sizeof(anim));

    board = sprite_new(board_xpm,0,0);
    state->display_buffer = (char *)malloc(vg_get_vram_size());
    if (state->display_buffer == NULL) {
        return !OK;
    }

    mouse = sprite_new(mouse_xpm, x, y);
    turn_player1 = sprite_new(turn_player1_xpm,862,15);
    turn_player2 = sprite_new(turn_player2_xpm,862,15);
    blue_piece = sprite_new(blue_piece_xpm,0,0);
    red_piece = sprite_new(red_piece_xpm,0,0);
    if (board == NULL ||
        mouse == NULL ||
        turn_player1 == NULL ||
        turn_player2 == NULL ||
        blue_piece == NULL || 
        red_piece == NULL) {
        return !OK;
    }

    init_x_pos = x;
    init_y_pos = y;
    return OK;
}

void (destroy_game)(void) {
    sprite_destroy(board);
    sprite_destroy(mouse);
    sprite_destroy(turn_player1);
    sprite_destroy(turn_player2);
    sprite_destroy(blue_piece);
    sprite_destroy(red_piece);
    free(state->display_buffer);
    free(state);
    free(animation);
}

void (start_game)(void) {
    memcpy(state->display_buffer, board->map, vg_get_vram_size());
    sprite_set_pos(mouse, init_x_pos, init_y_pos);
    state->turn = PLAYER1;
    state->column = COLUMN_CENTER;
    for (int i = 0; i < ROW_NUM; i++) 
        for (int j = 0; j < COLUMN_NUM; j++) { state->board[i][j] = 0; }
    update_sprite_column();
}

int (get_turn_game)(void) {
    return state->turn;
}

int (draw_game)(void) {
    vg_set_current_buffer(state->display_buffer);
    if (state->turn == PLAYER1) {
        if (vg_draw_block_sprite_without_checks(turn_player1) != OK) { return !OK; }
        if (vg_draw_sprite(blue_piece) != OK) { return !OK; }
    } else if (state->turn == PLAYER2) {
        if (vg_draw_block_sprite_without_checks(turn_player2) != OK) { return !OK; }
        if (vg_draw_sprite(red_piece) != OK) { return !OK; }
        if (!in_animation) {
            if (vg_draw_sprite(mouse) != OK) { return !OK; }
        }
    }
    return OK;
}

int (draw_animation_game)(void) {
    if (animation->start_y + ANIMATION_DELTA >= animation->final_y) {
        animation->start_y = animation->final_y;
        sprite_set_pos(animation->sp, animation->start_x, animation->start_y);
        in_animation = false;
    } else {
        animation->start_y += ANIMATION_DELTA;
        sprite_set_pos(animation->sp, animation->start_x, animation->start_y);
    }
    if (draw_game() != OK) { return !OK; }
    return OK;
}

void (game_set_column_right)(void) {
    if (state->column + 1 == COLUMN_NUM) return;
    state->column += 1;
    update_sprite_column();
}

void (game_set_column_left)(void) {
    if (state->column == 0) return;
    state->column -= 1;
    update_sprite_column();
}

void (game_set_column_mouse)(void) {
    uint32_t x = mouse->x;
    if (x < POS_1_X) {
        state->column = 0;
    } else if (x < POS_2_X) {
        state->column = 1;
    } else if (x < POS_3_X) {
        state->column = 2;
    } else if (x < POS_4_X) {
        state->column = 3;
    } else if (x < POS_5_X) {
        state->column = 4;
    } else if (x < POS_6_X) {
        state->column = 5;
    } else {
        state->column = 6;
    }
    update_sprite_column();
}

void (update_sprite_column)(void) {
    if (state->turn == PLAYER1) {
        sprite_set_pos(blue_piece,game_get_X_pos(state->column),game_get_Y_pos(-1));
    } else if (state->turn == PLAYER2) {
        sprite_set_pos(red_piece,game_get_X_pos(state->column),game_get_Y_pos(-1));
    }
}

int (game_move)() {
    if (state->board[0][state->column] != EMPTY) { return !OK; }
    int row = 0;
    for (int i = 0; i < ROW_NUM; i++) {
        if (state->board[i][state->column] != 0) {
            state->board[i - 1][state->column] = state->turn;
            row = i - 1;
            break;
        } else if (i == ROW_NUM - 1) {
            row = ROW_NUM - 1;
            state->board[ROW_NUM - 1][state->column] = state->turn;
        }
    }

    if (state->turn == PLAYER1) {
        game_start_animation(blue_piece,row);
    } else if (state->turn == PLAYER2) {
        game_start_animation(red_piece,row);
    }

    return OK;
}

void (game_start_animation)(sprite * sp, int row) {
    animation->sp = sp;
    animation->start_x = game_get_X_pos(state->column);
    animation->start_y = game_get_Y_pos(-1);
    animation->final_y = game_get_Y_pos(row);
    in_animation = true;
}

void (next_turn)(void) {
    if (state->turn == PLAYER1) {
        state->turn = PLAYER2;
        sprite_set_pos(mouse, init_x_pos, init_y_pos);
        vg_draw_sprite_in_buffer(state->display_buffer, blue_piece);
    } else if (state->turn == PLAYER2) {
        state->turn = PLAYER1;
        vg_draw_sprite_in_buffer(state->display_buffer, red_piece);
    }
    state->column = COLUMN_CENTER;
    update_sprite_column();
}

int (check_game_end)() {
    int counter = 0;
    for (int i = 0; i < ROW_NUM; i++) {
        counter = 0;
        for (int j = 0; j < COLUMN_NUM; j++) {
            if (state->board[i][j] == state->turn) {
                counter++;
                if (counter == 4) { return state->turn; }
            } else { counter = 0; }
        }
    }

    for (int j = 0; j < COLUMN_NUM; j++) {
        counter = 0;
        for (int i = 0; i < ROW_NUM; i++) {
            if (state->board[i][j] == state->turn) {
                counter++;
                if (counter == 4) { return state->turn; }
            } else { counter = 0; }
        }
    }
    //only first and last row (it is enough)
    for (int i = 0; i < ROW_NUM; i+=ROW_NUM-1) {
        for (int j = 0; j < COLUMN_NUM; j++) {
            counter = 0;
            if (i == 0) {
                //right-down
                for (int ii = i, jj = j; 
                        ii < ROW_NUM && jj < COLUMN_NUM;
                        ii++,jj++) {
                    if (state->board[ii][jj] == state->turn) {
                        counter++;
                        if (counter == 4) { return state->turn; }
                    } else { counter = 0; }
                }
                counter = 0;
                //left-down
                for (int ii = i, jj = j; 
                        ii < ROW_NUM && jj >= 0;
                        ii++,jj--) {
                    if (state->board[ii][jj] == state->turn) {
                        counter++;
                        if (counter == 4) { return state->turn; }
                    } else { counter = 0; }
                }
            } else {
                //right-up
                for (int ii = i, jj = j; 
                        ii >= 0 && jj < COLUMN_NUM;
                        ii--,jj++) {
                    if (state->board[ii][jj] == state->turn) {
                        counter++;
                        if (counter == 4) { return state->turn; }
                    } else { counter = 0; }
                }
                counter = 0;
                //left-up
                for (int ii = i, jj = j; 
                        ii >= 0 && jj >= 0;
                        ii--,jj--) {
                    if (state->board[ii][jj] == state->turn) {
                        counter++;
                        if (counter == 4) { return state->turn; }
                    } else { counter = 0; }
                }
            }
        }
    }

    bool check_next_move_possible = false;
    for (int j = 0; j < COLUMN_NUM; j++) {
        if (state->board[0][j] == 0) {
            check_next_move_possible = true;
            break;
        }
    }
    if (!check_next_move_possible) { return DRAW; }

    return 0;
}

bool (check_animation_game_end)(void) {
    return animation->start_y == animation->final_y;
}

sprite * (get_mouse_sprite_game)() {
    return mouse;
}
