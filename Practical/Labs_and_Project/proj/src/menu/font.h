#pragma once

#include <lcom/lcf.h>
#include "lib/devices/graphics.h"
#include "lib/sprite/sprite.h"
#include "lib/utils/utils.h"
#include "lib/devices/rtc.h"

#define FIRST_X 306
#define FIRST_Y 25
#define NUM_WIDTH 30
#define DATE_SEP_WIDTH 30
#define TIME_SEP_WIDTH 15

/** @defgroup font font
 * @{
 *
 * Functions for the font
 */

/**
 * @brief Initialize font sprites
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (initialize_font)(void);

/**
 * @brief Destroy font sprites
 * 
 */
void (destroy_font)(void);

/**
 * @brief Draw date in the screen
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int (draw_date_font)(void);

/**
 * @brief Get the sprite font object
 * 
 * @param c Character of the wanted sprite
 * @return sprite* Character sprite of the correspondent char c
 */
sprite * (get_sprite_font)(char c);

/**@} */
/* EOF */
