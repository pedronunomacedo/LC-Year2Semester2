#pragma once

#include <lcom/lcf.h>
#include "lib/devices/graphics.h"
#include "lib/sprite/sprite.h"
#include "lib/utils/utils.h"
#include "font.h"

/** @defgroup pause_menu pause_menu
 * @{
 *
 * Functions for the pause menu
 */

#define MOUSE_SIDE 30
#define PAUSE_BACK_BTN_X 602
#define PAUSE_BACK_BTN_Y 664
#define CONTINUE_BTN_X 150
#define CONTINUE_BTN_Y 664

/**
 * @brief Initialize pause menu sprites
 * 
 * @param x start x-axis coordenate of the position of mouse sprite
 * @param y start y-axis coordenate of the position of mouse sprite
 * @return int Return 0 upon success and non-zero otherwise
 */
int (initialize_pause_menu)(uint32_t x, uint32_t y);

/**
 * @brief Destroy pause menu sprites
 * 
 */
void (destroy_pause_menu)(void);

/**
 * @brief Draw pause menu in the screen
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (draw_pause_menu)(void);

/**
 * @brief Get the mouse sprite of the pause menu
 * 
 * @return sprite* returns the mouse sprite of the pause menu
 */
sprite * (get_mouse_sprite_pause_menu)();

/**
 * @brief Get the back button sprite of the pause menu
 * 
 * @return sprite* returns the back button sprite of the pause menu
 */
sprite * (get_back_sprite_pause_menu)();

/**
 * @brief Get the continue button sprite of the pause menu
 * 
 * @return sprite* returns the continue button sprite of the pause menu
 */
sprite * (get_continue_sprite_pause_menu)();

/**@} */
/* EOF */
