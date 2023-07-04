#pragma once

#include <lcom/lcf.h>
#include "lib/sprite/sprite.h"
#include "lib/devices/graphics.h"
#include "game/game_utils.h"

/** @defgroup game game
 * @{
 *
 * Constants and functions for the game
 */

#define COLUMN_NUM 7
#define COLUMN_CENTER 3
#define ROW_NUM 6
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2
#define DRAW 3
#define ANIMATION_DELTA 15

/**
 * @brief Struct with game infos like the board, a display_buffer to save the save of the board and
 * be more efficient to draw the game, the turn of the player and the current column
 * 
 */
typedef struct game
{
    uint8_t board[6][7];
    char * display_buffer;
    int turn;
    int column;
} game;

/**
 * @brief Struct with animation information like the sprite the x-axis start coordenate,
 * y-axis start coordenate and y-axis final coordenate
 * 
 */
typedef struct anim
{
    sprite * sp;
    int start_x;
    int start_y;
    int final_y;
} anim;

/**
 * @brief Initialize game sprites and initialize the game and animation object
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (initialize_game)(uint32_t x, uint32_t y);

/**
 * @brief Destroy game sprites, the game and the animation object
 * 
 */
void (destroy_game)(void);

/**
 * @brief Reset the game to begin to play
 * 
 */
void (start_game)(void);

/**
 * @brief Get the number that correspond to the current turn of the game
 * 
 * @return game turn
 */
int (get_turn_game)(void);

/**
 * @brief Draw the game in the screen
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (draw_game)(void);

/**
 * @brief Update sprite position to create the game animation and draw the game
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (draw_animation_game)(void);

/**
 * @brief set current column to right
 * 
 */
void (game_set_column_right)(void);

/**
 * @brief set current column to left
 * 
 */
void (game_set_column_left)(void);

/**
 * @brief set current column according to mouse position
 * 
 */
void (game_set_column_mouse)(void);

/**
 * @brief update sprite with the new column
 * 
 */
void (update_sprite_column)(void); 

/**
 * @brief Make a game move by updating the board and all informations needed for the game
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (game_move)();

/**
 * @brief Initialization of one animation of the game
 * 
 * @param sp Sprite that will be used for the animation
 * @param row Represents the final row of the board which the piece will stop
 */
void (game_start_animation)(sprite * sp, int row);

/**
 * @brief Change the turn and update all the information needed for the game
 * 
 */
void (next_turn)(void);

/**
 * @brief 
 * 
 * @return 0 The game must continue to the next player
 * @return 1 The game ends and the player 1 won the game
 * @return 2 The game ends and the player 2 won the game
 * @return 3 The game ends in a draw
 */
int (check_game_end)();

/**
 * @brief Checks if the game animation is finished
 * 
 * @return true Return true if the game animation is finished
 * @return false Return false if the game animation is not finished
 */
bool (check_animation_game_end)(void);

/**
 * @brief Get the mouse sprite of the game
 * 
 * @return sprite* returns the mouse sprite of the game
 */
sprite * (get_mouse_sprite_game)();

/**@} */
/* EOF */
