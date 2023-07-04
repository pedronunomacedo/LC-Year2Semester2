#pragma once

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <sys/mman.h>

#include "vbe.h"
#include "../sprite/sprite.h"

/** @defgroup graphics graphics
 * @{
 *
 * Functions for using the vbe
 */

/**
 * @brief Set the graphic mode
 * 
 * @param mode graphic mode 
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_set_video_mode)(uint16_t mode);

/**
 * @brief Initializes the video module in graphics mode.
 * 
 * @param mode graphic mode 
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_init_graphic)(uint16_t mode);

/**
 * @brief swap video buffers calling the VBE function to flip to current buffer
 * 
 * The buffer is switched to the next one (one of the three - triple buffering)
 * 
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_swap_buffers)();

/**
 * @brief Draws a pixel by changing the video RAM with the specified color
 * 
 * @param x x-axis coordenate of the pixel
 * @param y y-axis coordenate of the pixel
 * @param color Color to set the pixel
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a pixel in a specified buffer by changing the video RAM with the specified color
 * 
 * @param buffer Buffer to draw the pixel
 * @param x x-axis coordenate of the pixel
 * @param y y-axis coordenate of the pixel
 * @param color Color to set the pixel
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_draw_pixel_in_buffer)(char * buffer, uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a sprite by changing the video RAM with the specified color
 * 
 * @param sprite Sprite to draw
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_draw_sprite)(struct sprite * sprite);

/**
 * @brief Draws a sprite in a specified buffer by changing the video RAM
 * 
 * @param buffer Buffer to draw the sprite
 * @param sprite Sprite to draw
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_draw_sprite_in_buffer)(char * buffer, struct sprite * sprite);

/**
 * @brief Draws a block sprite (rectangular) by changing the video RAM with the specified color.
 * 
 * This function does not do error detection
 * 
 * @param sp Sprite to draw
 * @return int Return 0 on success, non-zero otherwise
 */
int (vg_draw_block_sprite_without_checks)(struct sprite * sp);

/**
 * @brief Set the current buffer
 * 
 * @param buffer New video buffer
 */
void (vg_set_current_buffer)(char * buffer);

/**
 * @brief get the size of the vram (video buffer size)
 * 
 * @return unsigned int Returns the size of the vram
 */
unsigned int (vg_get_vram_size)();

/**@} */
/* EOF */
