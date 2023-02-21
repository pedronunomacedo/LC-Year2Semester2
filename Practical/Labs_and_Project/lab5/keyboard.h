#pragma once

#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <i8042.h>

void (kbc_ih)(void);

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int (kbd_read_code)(uint8_t * scancode);

int (kbd_restore_interrupts)();

int (wait_for_ESQ)(void);
