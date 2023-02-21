#pragma once
#include <lcom/lcf.h>

#include "i8042.h"

int (kbd_subscribe_int)(uint8_t * bit_no);

int (kbd_unsubscribe_int)();

int (get_status_register)(uint8_t * st);

int (kbd_ih)();

uint8_t (kbd_get_data)();

int (kbd_enable_interrupts)();
