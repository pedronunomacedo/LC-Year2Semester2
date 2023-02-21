#pragma once
#include <lcom/lcf.h>

#include "game/game.h"
#include "game/game_end_menu.h"
#include "menu/main_menu.h"
#include "menu/rules_menu.h"
#include "menu/pause_menu.h"
#include "lib/sprite/sprite.h"
#include "lib/devices/i8042.h"

/** @defgroup events events
 * @{
 *
 * Functions for the events
 */

/**
 * @brief Enum wich saves the state of the program
 * 
 */
typedef enum {
    IN_GAME,
    ANIMATION_GAME,
    END_GAME,
    MAIN_MENU,
    PAUSE_MENU,
    RULES_MENU,
    QUIT,
} GAME_STATE;

/**
 * @brief handle timer interrupt when the game state correspond to the main menu
 * 
 * @return GAME_STATE 
 */
GAME_STATE (handle_timer_main_menu)();

/**
 * @brief handle keyboard interrupt when the game state correspond to the main menu
 * 
 * @param scancode Keyboard scancode of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_keyboard_main_menu)(uint16_t scancode);

/**
 * @brief handle mouse interrupt when the game state correspond to the main menu
 * 
 * @param pp Mouse packet of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_mouse_main_menu)(struct packet pp);

/**
 * @brief handle timer interrupt when the game state correspond to the rules menu
 * 
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_timer_rules_menu)();

/**
 * @brief handle keyboard interrupt when the game state correspond to the rules menu
 * 
 * @param scancode Keyboard scancode of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_keyboard_rules_menu)(uint16_t scancode);

/**
 * @brief handle moouse interrupt when the game state correspond to the rules menu
 * 
 * @param pp Mouse packet of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_mouse_rules_menu)(struct packet pp);

/**
 * @brief handle timer interrupt when the game state correspond to the pause menu
 * 
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_timer_pause_menu)();

/**
 * @brief handle keyboard interrupt when the game state correspond to the pause menu
 * 
 * @param scancode Keyboard scancode of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_keyboard_pause_menu)(uint16_t scancode);

/**
 * @brief handle mouse interrupt when the game state correspond to the pause menu
 * 
 * @param pp Mouse packet of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_mouse_pause_menu)(struct packet pp);

/**
 * @brief handle timer interrupt when the game state correspond to the game end menu
 * 
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_timer_game_end_menu)();

/**
 * @brief handle keyboard interrupt when the game state correspond to the game end menu
 * 
 * @param scancode Keyboard scancode of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_keyboard_game_end_menu)(uint16_t scancode);

/**
 * @brief handle mouse interrupt when the game state correspond to the game end menu
 * 
 * @param pp Mouse packet of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_mouse_game_end_menu)(struct packet pp);

/**
 * @brief handle timer interrupt when the game state correspond to the game itself
 * 
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_timer_game)();

/**
 * @brief handle keyboard interrupt when the game state correspond to the game itself
 * 
 * @param scancode Keyboard scancode of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_keyboard_game)(uint16_t scancode);

/**
 * @brief handle mouse interrupt when the game state correspond to the game itself
 * 
 * @param pp Mouse packet of the last interrupt
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_mouse_game)(struct packet pp);

/**
 * @brief handle timer interrupt when the game state correspond to the animation
 * 
 * @return GAME_STATE Next state of the program after handling the interrupt
 */
GAME_STATE (handle_timer_animation_game)();

/**@} */
/* EOF */
