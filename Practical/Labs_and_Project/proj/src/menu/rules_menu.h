#pragma once

#include <lcom/lcf.h>
#include "lib/devices/graphics.h"
#include "lib/sprite/sprite.h"
#include "lib/utils/utils.h"
#include "font.h"

/** @defgroup rules_menu rules_menu
 * @{
 *
 * Functions for the rules menu
 */

#define MOUSE_SIDE 30
#define BACK_BTN_X 570
#define BACK_BTN_Y 630

/**
 * @brief Initialize rules menu sprites
 * 
 * @param x start x-axis coordenate of the position of mouse sprite
 * @param y start y-axis coordenate of the position of mouse sprite
 * @return int Return 0 upon success and non-zero otherwise
 */
int (initialize_rules_menu)(uint32_t x, uint32_t y);

/**
 * @brief Destroy rules menu sprites
 * 
 */
void (destroy_rules_menu)(void);

/**
 * @brief Draw rules menu in the screen
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (draw_rules_menu)(void);

/**
 * @brief Get the mouse sprite of the rules menu
 * 
 * @return sprite* returns the mouse sprite of the rules menu
 */
sprite * (get_mouse_sprite_rules_menu)();

/**
 * @brief Get the back button sprite of the rules menu
 * 
 * @return sprite* returns the back button sprite of the rules menu
 */
sprite * (get_back_sprite_rules_menu)();

/**@} */
/* EOF */
