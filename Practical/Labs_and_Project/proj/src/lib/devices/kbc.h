#pragma once

#include <lcom/lcf.h>
#include "i8042.h"

/** @defgroup kbc kbc
 * @{
 *
 * Functions for using the i8042 kbc
 */

/**
 * @brief Keyboard interrupt handler
 * 
 * Check errors and read new scancode
 */
void (kbd_ih)(void);

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Get the keyboard scancode;
 * 
 * @return uint16_t Return keyboard scancode
 */
uint16_t (get_keyboard_scancode)(void);

/**
 * @brief checks if the keyboard scancode is ready to use
 * 
 * @return true If keyboard scancode is ready to use
 * @return false If keyboard scancode is not ready to use
 */
bool (check_keyboard_ready)(void);

/**
 * @brief Mouse interrupt handler
 * 
 * Check errors and read new mouse_data
 */
void (mouse_ih)(void);

/**
 * @brief checks if the mouse data is ready to use
 * 
 * @return true If mouse data is ready to use
 * @return false If kmouse data is not ready to use
 */
bool (check_mouse_ready)(void);

/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t * bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)(void);

/**
 * @brief Enable or disable the mouse data reporting
 * 
 * @param enable true for enable and false for disable
 * @return int Return 0 upon success and non-zero otherwise
 */
int (set_mouse_data_reporting)(bool enable);

/**
 * @brief Get the mouse packet
 * 
 * @return struct packet Packet with the mouse information
 */
struct packet (get_mouse_packet)();

/**
 * @brief Get the status register object
 * 
 * @param st byte to set the status register
 * @return int Return 0 upon success and non-zero otherwise
 */
int (get_status_register)(uint8_t * st);

/**
 * @brief wait for input buffer is clear to write a command to the buffer
 * 
 */
void (wait_for_IB_is_clear)();

/**@} */
/* EOF */
