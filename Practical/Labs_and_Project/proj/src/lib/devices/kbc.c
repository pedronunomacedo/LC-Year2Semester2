#include "kbc.h"

//keyboard
static int kbd_hook_id;
static uint8_t keyboard_data = 0x0;
static uint8_t scancode[2], size = 0;
static bool keyboard_scancode_ready = false;

void (kbd_ih)(void) {
	keyboard_scancode_ready = false;
	if (size == 0) { bzero(scancode,2); }
	uint8_t st;
	if (get_status_register(&st) != OK) {
		printf("mouse_ih: util_sys_inb failed\n");
		return;
	}

	if (st & KBC_OBF && !(st & KBC_AUX)) {
		if (util_sys_inb(KBC_OUT_BUF,&keyboard_data) != OK) {
			printf("util_sys_inb: Reading Status Register Failed");
			return;
		}

		if (!(st & KBC_PARITY_ERR || st & KBC_TIMEOUT_ERR)) {
			scancode[size] = keyboard_data;
			size++;

			if (keyboard_data == KBC_TWOBYTE_CODE) return;

			size = 0;
			keyboard_scancode_ready = true;
		}
	}
}

int (kbd_subscribe_int)(uint8_t *bit_no) {
	kbd_hook_id = *bit_no = KBD_IRQ;

	if (sys_irqsetpolicy(KBD_IRQ, (IRQ_ENABLE | IRQ_EXCLUSIVE), &kbd_hook_id) != OK) {
		printf("kbd_subscribe_int: Keyboard sys_irqsetpolicy failed\n");
		return !OK;
	}
	return OK;
}

int (kbd_unsubscribe_int)() {
	if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
		printf("kbd_unsubscribe_int: Keyboard sys_irqsetpolicy failed\n");
		return !OK;
	}

	return OK;
}

uint16_t (get_keyboard_scancode)(void) { return scancode[0] + (scancode[1] << 8); }

bool (check_keyboard_ready)(void) { return keyboard_scancode_ready; }

//mouse
int mouse_hook_id;
static uint8_t mouse_data = 0x0, mouse_packet[3], mouse_packet_counter = 0;
static bool mouse_error = false, mouse_is_sync = false, mouse_packet_ready = false;

void (mouse_ih)(void) {
	uint8_t st;
	mouse_error = false;
	if (get_status_register(&st) != OK) {
		printf("mouse_ih: util_sys_inb failed\n");
	}

	if (st & KBC_OBF && st & KBC_AUX) {
		if (util_sys_inb(KBC_OUT_BUF, &mouse_data) != OK) {
			printf("mouse_ih: util_sys_inb failed\n");
		}

		if (!(st & KBC_PARITY_ERR || st & KBC_TIMEOUT_ERR)) {
			if (mouse_packet_counter == 0 && (mouse_data & MOUSE_FLAG)) { 
				mouse_packet_ready = false; 
				mouse_is_sync = true; 
			}

			if (mouse_is_sync) {
				mouse_packet[mouse_packet_counter] = mouse_data;

				if (mouse_packet_counter == 2) {
					mouse_is_sync = false;
					mouse_packet_counter = 0;
					mouse_packet_ready = true;
				} else { mouse_packet_counter++; }
			}
		}
	}
}

bool (check_mouse_ready)(void) { return mouse_packet_ready; }

int (mouse_subscribe_int)(uint8_t * bit_no) {
	*bit_no = mouse_hook_id = MOUSE_IRQ;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) {
		printf("mouse_subscribe_int: sys_irqsetpolicy failed\n");
		return !OK;
	}
	return OK;
}

int (mouse_unsubscribe_int)() {
	if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
		printf("mouse_subscribe_int: sys_irqrmpolicy failed\n");
		return !OK;
	}
	return OK;
}

int (set_mouse_data_reporting)(bool enable) {
	uint8_t cmd = enable ? MOUSE_ENABLE_DATA_REPORTING : MOUSE_DISABLE_DATA_REPORTING;
	while (1) {
		wait_for_IB_is_clear();
		if (sys_outb(KBC_CMD_REG, KBC_WR_BYTE_MOUSE) != OK) {
			printf("set_mouse_data_reporting: sys_outb failed\n");
			return !OK;
		}

		wait_for_IB_is_clear();
		if (sys_outb(KBC_ARG_REG, cmd) != OK) {
			printf("set_mouse_data_reporting: sys_outb failed\n");
			return !OK;
		}

		uint8_t res = 0;

		if (util_sys_inb(MOUSE_READ_RES, &res) != OK) {
			printf("set_mouse_data_reporting: sys_outb failed\n");
			return !OK;
		}

		if (res == MOUSE_ACK) { break; }
	}
	return OK;
}

struct packet (get_mouse_packet)() {
	struct packet pp;

	for (int i = 0; i < 3; i++) { pp.bytes[i] = mouse_packet[i]; }
	pp.lb = (mouse_packet[0] & MOUSE_LEFT_BUTTON);
	pp.rb = (mouse_packet[0] & MOUSE_RIGHT_BUTTON);
	pp.mb = (mouse_packet[0] & MOUSE_MIDDLE_BUTTON);
	pp.delta_x = (mouse_packet[0] & MOUSE_MSB_X_DELTA) ? 0xff00 | mouse_packet[1] : mouse_packet[1];
	pp.delta_y = (mouse_packet[0] & MOUSE_MSB_Y_DELTA) ? 0xff00 | mouse_packet[2] : mouse_packet[2];
	pp.x_ov = (mouse_packet[0] & MOUSE_X_OVFL);
	pp.y_ov = (mouse_packet[0] & MOUSE_Y_OVFL);

	return pp;
}

// kbc
int (get_status_register)(uint8_t * st) {
	if (util_sys_inb(KBC_ST_REG, st) != OK) {
		printf("get_status_register: util_sys_inb failed\n");
		return !OK;
	}
	return OK;
}

void (wait_for_IB_is_clear)() {
	uint8_t st;
	if (get_status_register(&st) != OK) {
		printf("wait_for_IB_is_clear: get_status_register failed\n");
		return;
	}
	while (st & KBC_IBF) {
		tickdelay(micros_to_ticks(DELAY));
		if (get_status_register(&st) != OK) {
			printf("wait_for_IB_is_clear: get_status_register failed\n");
			return;
		}
	}
	return;
}
