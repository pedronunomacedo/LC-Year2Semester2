#include <lcom/lcf.h>
#include <lcom/proj.h>

#include "lib/devices/timer.h"
#include "lib/devices/kbc.h"
#include "lib/devices/graphics.h"
#include "lib/devices/vbe.h"
#include "lib/devices/rtc.h"
#include "events/events.h"
#include "menu/main_menu.h"
#include "menu/rules_menu.h"
#include "menu/pause_menu.h"
#include "menu/font.h"
#include "game/game_end_menu.h"
#include "game/game.h"
#include "lib/sprite/sprite.h"

#include "assets/game/initializing.xpm"

/** @defgroup proj proj
 * @{
 *
 * Functions for the proj
 */


#define FPS 60

/**
 * @brief Main function
 * 
 * @param argc Number of arguments
 * @param argv List of arguments
 * @return int Return 0 upon success and non-zero otherwise
 */
int main(int argc, char *argv[]) {
	lcf_set_language("EN-US");
	lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
	lcf_log_output("/home/lcom/labs/proj/output.txt");
	if (lcf_start(argc, argv)) 
		return !OK;
	lcf_cleanup();
	return OK;
}

/**
 * @brief Program main loop
 * 
 * @param argc Number of arguments
 * @param argv List of arguments
 * @return int Return 0 upon success and non-zero otherwise
 */
int (proj_main_loop)(int argc, char *argv[]) {
	if (vg_init_graphic(MODE_RES_1152x864_BITS_32) != OK) { return !OK; }

	sprite * init = sprite_new(initializing_xpm,0,0);
	if (vg_draw_block_sprite_without_checks(init) != OK) { return !OK; }

	if (timer_set_frequency(TIMER_SEL0, FPS) != OK) { return !OK; }
	if (set_mouse_data_reporting(true) != OK) { return !OK; }

	uint8_t timer_bit_no, keyboard_bit_no, mouse_bit_no, rtc_bit_no;
	if (timer_subscribe_int(&timer_bit_no) != OK) { return !OK; }
	if (kbd_subscribe_int(&keyboard_bit_no) != OK) { return !OK; }
	if (mouse_subscribe_int(&mouse_bit_no) != OK) { return !OK; }
	if (rtc_subscribe_int(&rtc_bit_no) != OK) { return !OK; }
	uint32_t 	timer_irq_set = BIT(timer_bit_no), 
				keyboard_irq_set = BIT(keyboard_bit_no),
				mouse_irq_set = BIT(mouse_bit_no),
				rtc_irq_set = BIT(rtc_bit_no);
	rtc_disable_int();
  	rtc_enable_update_int();

	int r, ipc_status;
	message msg;
	GAME_STATE game_state = MAIN_MENU;

	uint32_t init_middle_width = WINDOW_WIDTH / 2;
	uint32_t init_middle_height = WINDOW_HEIGHT / 3;
	
	if (initialize_main_menu(init_middle_width, init_middle_height) != OK) { return !OK; }
	if (initialize_rules_menu(init_middle_width, init_middle_height) != OK) { return !OK; }
	if (initialize_pause_menu(init_middle_width, init_middle_height) != OK) { return !OK; }
	if (initialize_font() != OK) { return !OK; }
	if (initialize_game_end_menu() != OK) { return !OK; }
	if (initialize_game(init_middle_width, init_middle_height) != OK) { return !OK; }

	while( game_state != QUIT ) {
    	if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
			printf("driver_receive failed with: %d", r);
			continue;
    	}

		if (is_ipc_notify(ipc_status)) { 
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: 

					if (msg.m_notify.interrupts & timer_irq_set) {
						timer_int_handler();
						switch (game_state)	{
							case MAIN_MENU:
								game_state=handle_timer_main_menu();
								break;
							case RULES_MENU:
								game_state=handle_timer_rules_menu();
								break;
							case IN_GAME:
								game_state=handle_timer_game();
								break;
							case PAUSE_MENU:
								game_state=handle_timer_pause_menu();
								break;
							case END_GAME:
								game_state=handle_timer_game_end_menu();
								break;
							case ANIMATION_GAME:
								game_state=handle_timer_animation_game();
								break;
							default:
								break;
						}
					}

					if (msg.m_notify.interrupts & keyboard_irq_set) {
						kbd_ih();
						if (check_keyboard_ready()) {
							switch (game_state) {
								case MAIN_MENU:
									game_state=handle_keyboard_main_menu(get_keyboard_scancode());
									break;
								case RULES_MENU:
									game_state=handle_keyboard_rules_menu(get_keyboard_scancode());
									break;
								case IN_GAME:
									game_state=handle_keyboard_game(get_keyboard_scancode());
									break;
								case PAUSE_MENU:
									game_state=handle_keyboard_pause_menu(get_keyboard_scancode());
									break;
								case END_GAME:
									game_state=handle_keyboard_game_end_menu(
										get_keyboard_scancode());
									break;
								default:
									break;
							}
						}
					}

					if (msg.m_notify.interrupts & mouse_irq_set) {
						mouse_ih();
						if (check_mouse_ready()) {
							switch (game_state) {
								case MAIN_MENU:
									game_state=handle_mouse_main_menu(get_mouse_packet());
									break;
								case RULES_MENU:
									game_state=handle_mouse_rules_menu(get_mouse_packet());
									break;
								case PAUSE_MENU:
									game_state=handle_mouse_pause_menu(get_mouse_packet());
									break;
								case IN_GAME:
									game_state=handle_mouse_game(get_mouse_packet());
									break;
								case END_GAME:
									game_state=handle_mouse_game_end_menu(
										get_mouse_packet());
									break;
								default:
									break;
							}
						}
					}

					if (msg.m_notify.interrupts & rtc_irq_set) {
						rtc_ih();
					}
					break;
				default:
					break; 
			}
		}
	}

	destroy_game();
	destroy_game_end_menu();
	destroy_font();
	destroy_pause_menu();
	destroy_rules_menu();
	destroy_main_menu();
	sprite_destroy(init);

	if (rtc_unsubscribe_int() != OK) { return !OK; }
	if (mouse_unsubscribe_int() != OK) { return !OK; }
	if (kbd_unsubscribe_int() != OK) { return !OK; }
	if (timer_unsubscribe_int() != OK) { return !OK; }
	if (set_mouse_data_reporting(false) != OK) { return !OK; }
	
	return vg_exit();
}

/**@} */
/* EOF */
