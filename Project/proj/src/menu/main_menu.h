#pragma once

#include <lcom/lcf.h>
#include "lib/devices/graphics.h"
#include "lib/sprite/sprite.h"
#include "lib/utils/utils.h"
#include "font.h"

/** @defgroup main_menu main_menu
 * @{
 *
 * Functions for the main menu
 */

#define MOUSE_SIDE 30
#define BUTTONS_X 376
#define PLAY_BTN_Y 400
#define RULES_BTN_Y 550
#define QUIT_BTN_Y 700

/**
 * @brief Initialize main menu sprites
 * 
 * @param x start x-axis coordenate of the position of mouse sprite
 * @param y start y-axis coordenate of the position of mouse sprite
 * @return int Return 0 upon success and non-zero otherwise
 */
int (initialize_main_menu)(uint32_t x, uint32_t y);

/**
 * @brief Destroy main menu sprites
 * 
 */
void (destroy_main_menu)(void);

/**
 * @brief Draw main menu in the screen
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (draw_main_menu)(void);

/**
 * @brief Get the mouse sprite of the main menu
 * 
 * @return sprite* returns the mouse sprite of the main menu
 */
sprite * (get_mouse_sprite_main_menu)();

/**
 * @brief Get the play sprite of the main menu
 * 
 * @return sprite* returns the play button sprite of the main menu
 */
sprite * (get_play_sprite_main_menu)();

/**
 * @brief Get the rules sprite of the main menu
 * 
 * @return sprite* returns the rules button sprite of the main menu
 */
sprite * (get_rules_sprite_main_menu)();

/**
 * @brief Get the quit sprite of the main menu
 * 
 * @return sprite* returns the quit button sprite of the main menu
 */
sprite * (get_quit_sprite_main_menu)();

/**@} */
/* EOF */
