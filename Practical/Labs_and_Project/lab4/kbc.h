#pragma once

#include <lcom/lcf.h>
#include "i8042.h"

typedef enum {START, LEFT, LEFT_COMPLETE, RIGHT_START, RIGHT, RIGHT_COMPLETE, END} draw_state;

int (mouse_subscribe_int)(uint8_t * bit_no);

int (mouse_unsubscribe_int)(void);

int (set_mouse_data_reporting)(bool enable);

void (mouse_ih)(void);

void (wait_for_IB_is_clear)();

int (get_status_register)(uint8_t * st);

uint8_t (get_mouse_data)(void);

bool (check_mouse_error)(void);

struct packet (get_mouse_packet)(uint8_t * mouse_packet);

void (update_state)(struct packet pp, draw_state * state, uint8_t x_len, uint8_t tolerance);

bool (check_only_lb)(struct packet pp);

bool (check_none_pressed)(struct packet pp);

bool (check_only_rb)(struct packet pp);
