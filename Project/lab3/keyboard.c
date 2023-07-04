#include <keyboard.h>

static int kbd_hook_id;

int (kbd_subscribe_int)(uint8_t *bit_no) {

	kbd_hook_id = *bit_no = KBD_IRQ;

	if (sys_irqsetpolicy(KBD_IRQ, (IRQ_ENABLE | IRQ_EXCLUSIVE), &kbd_hook_id) != OK) {
		printf("Keyboard sys_irqsetpolicy failed\n");
		return !OK;
	}
	return OK;
}

int (kbd_unsubscribe_int)() {
	if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
		printf("Keyboard sys_irqsetpolicy failed\n");
		return !OK;
	}

	return OK;
}

uint8_t data = 0x0;
bool error = false;

void (kbc_ih)(void) {
	uint8_t st;
	if (util_sys_inb(KBC_ST_REG, &st) != OK) {
		printf("Reading Status Register Failed");
		return;
	}

	if (st & KBC_OBF) {
		if (util_sys_inb(KBC_OUT_BUF,&data) != OK) {
			printf("Reading Status Register Failed");
			return;
		}

		if (st & KBC_PARITY_ERR || st & KBC_TIMEOUT_ERR) {
			error = true;
		}
	}
}

int (kbd_read_code)(uint8_t * scancode) {
	uint8_t st;
	if (util_sys_inb(KBC_ST_REG, &st) != OK) {
		printf("Reading Status Register Failed");
		return !OK;
	}

	if ((st & KBC_OBF) && !(st & KBC_AUX)) {
		if (util_sys_inb(KBC_OUT_BUF,scancode) != OK) {
			printf("Reading Status Register Failed");
			return !OK;
		}

		if (st & KBC_PARITY_ERR || st & KBC_TIMEOUT_ERR) {
			return !OK;
		}
	} else { return !OK; }

	return OK;
}

int (kbd_restore_interrupts)() {
	uint8_t cmd_byte;

	if (sys_outb(KBC_CMD_REG,KBC_CMD_RCB) != OK) {
		printf("writing 0x20 to command register failed");
		return !OK;
	} 

	if (util_sys_inb(KBC_OUT_BUF, &cmd_byte) != OK) {
		printf("reading command byte failed");
		return !OK;
	}

	cmd_byte |= KBC_INT;

	if (sys_outb(KBC_CMD_REG,KBC_CMD_WCB) != OK) {
		printf("writing 0x20 to command register failed");
		return !OK;
	}

	if (sys_outb(KBC_OUT_BUF,cmd_byte) != OK) {
		printf("writing 0x20 to command register failed");
		return !OK;
	}

	return OK;
}
