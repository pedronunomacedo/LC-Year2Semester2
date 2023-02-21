#pragma once
#include <lcom/lcf.h>

#define WINDOW_WIDTH 1152
#define WINDOW_HEIGHT 864

/** @defgroup sprite sprite
 * @{
 *
 * Functions for using sprites
 */

/**
 * @brief Struct with the position, xpm_image and the map of the sprite
 * 
 */
typedef struct sprite
{
    uint8_t * map;
    xpm_image_t img;
    uint32_t x;
    uint32_t y;
} sprite;

/**
 * @brief Creates a new sprite object
 * 
 * @param xpm xpm map of the sprite
 * @param x x-axis coordenate of the top-left of the sprite
 * @param y y-axis coordenate of the top-left of the sprite
 * @return sprite* 
 */
sprite * (sprite_new)(xpm_map_t xpm, uint32_t x, uint32_t y);

/**
 * @brief Destry a sprite
 * 
 * @param sprite sprite to destroy
 */
void (sprite_destroy)(sprite * sprite);

/**
 * @brief Set the sprite coordenates
 * 
 * @param sprite sprite to set the new coordenates
 * @param x New x-axis coordenate of the top-left of the sprite
 * @param y New y-axis coordenate of the top-left of the sprite
 */
void (sprite_set_pos)(sprite * sprite, uint32_t x, uint32_t y);

/**
 * @brief Add a delta value to the sprite coordenates
 * 
 * @param sprite sprite to add the delta value to the coordenates
 * @param x_delta Delta value to add to x-axis coordenate of the top-left of the sprite
 * @param y_delta Delta value to add to y-axis coordenate of the top-left of the sprite
 */
void (sprite_set_pos_delta)(sprite * sprite, int x_delta, int y_delta);

/**@} */
/* EOF */
