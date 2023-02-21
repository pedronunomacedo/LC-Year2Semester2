#include "game_utils.h"

uint32_t (game_get_X_pos)(int column) {
    switch(column) {
        case 0: return POS_0_X;
        case 1: return POS_1_X;
        case 2: return POS_2_X;
        case 3: return POS_3_X;
        case 4: return POS_4_X;
        case 5: return POS_5_X;
        case 6: return POS_6_X;
        default: break;
    }
    return !OK;
}

uint32_t (game_get_Y_pos)(int row) {
    switch(row) {
        case -1: return POS_INIT_Y;
        case 0: return POS_0_Y;
        case 1: return POS_1_Y;
        case 2: return POS_2_Y;
        case 3: return POS_3_Y;
        case 4: return POS_4_Y;
        case 5: return POS_5_Y;
        default: break;
    }
    return !OK;
}
