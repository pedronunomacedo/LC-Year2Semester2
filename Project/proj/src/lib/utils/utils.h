#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "lib/sprite/sprite.h"

/** @defgroup utils utils
 * @{
 *
 * Utils functions
 */

/**
 * @brief Returns the LSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param lsb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Returns the MSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 *
 * @param port the input port that is to be read
 * @param value a pointer to the variable that is to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Checks if the position of mouse sprite is above of the button sprite
 * 
 * @param mouse 
 * @param button 
 * @return true if the position of mouse sprite is above of the button sprite
 * @return false if the position of mouse sprite is not above of the button sprite
 */
bool (check_mouse_in_button)(struct sprite * mouse, struct sprite * button);

/**@} */
/* EOF */
