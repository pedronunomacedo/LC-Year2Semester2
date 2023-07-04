#pragma once
#include <lcom/lcf.h>

/** @defgroup game_utils game_utils
 * @{
 *
 * Constants and functions for the game utils
 */

#define POS_0_X 196
#define POS_1_X 306
#define POS_2_X 416
#define POS_3_X 526
#define POS_4_X 636
#define POS_5_X 746
#define POS_6_X 856
#define POS_INIT_Y 104
#define POS_0_Y 209
#define POS_1_Y 319
#define POS_2_Y 429
#define POS_3_Y 539
#define POS_4_Y 649
#define POS_5_Y 759

/**
 * @brief get the x-axis coordenate of the board
 * 
 * @param column number corresponding to the board column
 * @return uint32_t Return the x-axis coordenate
 */
uint32_t game_get_X_pos(int column);

/**
 * @brief get the y-axis coordenate of the board
 * 
 * @param row number corresponding to the board row
 * @return uint32_t Return the y-axis coordenate
 */
uint32_t game_get_Y_pos(int row);

/**@} */
/* EOF */
