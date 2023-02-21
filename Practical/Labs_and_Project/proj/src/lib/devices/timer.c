#include "timer.h"

static int timer_hook_id;
static uint32_t timer_counter = 0;

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	uint8_t rbc = 0x0;
	rbc |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

	if (sys_outb(TIMER_CTRL, rbc) != OK) {
		printf("timer_get_conf: sys_outb failed");
		return !OK;
	}

	if (util_sys_inb(TIMER_0 + timer, st) != OK) {
		printf("timer_get_conf: util_sys_inb failed");
		return !OK;
	}

	return OK;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
												enum timer_status_field field) {
	union timer_status_field_val u;
	uint8_t count_mode, bits = 0x0;

	switch (field) {
		case tsf_all:
			u.byte = st;
			break;
		
		case tsf_initial:
			u.in_mode = (st & TIMER_LSB_MSB) >> 4;
			break;

		case tsf_mode:
			bits |= BIT(1) | BIT(2) | BIT(3);
			count_mode = (st & bits) >> 1;
			if (count_mode == 6) { count_mode = 2; }
			if (count_mode == 7) { count_mode = 3; }
			u.count_mode = count_mode;
			break;

		case tsf_base:
			u.bcd = st & BIT(0);
			break;

		default:
			return !OK;
	}

	return timer_print_config(timer, field, u);
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
	uint16_t divs = TIMER_FREQ / freq;
	uint8_t msb, lsb;
	if (util_get_LSB(divs, &lsb) != OK || util_get_MSB(divs, &msb) != OK) {
		printf("timer_set_frequency: util_get_LSB or util_get_MSB failed");
		return !OK;
	}

	uint8_t st = 0x0;
	if (timer_get_conf(timer, &st) != OK) { 
		printf("timer_set_frequency: timer_get_conf failed");
		return !OK;
	}

	uint8_t cw = 0x0 | (timer << 6) | TIMER_LSB_MSB | (st & 0xf);

	if (sys_outb(TIMER_CTRL, cw) != OK) {
		printf("timer_set_frequency: sys_outb failed");
		return !OK;
	}

	if (sys_outb(TIMER_0 + timer, lsb) != OK) {
		printf("timer_set_frequency: sys_outb failed");
		return !OK;
	}

	if (sys_outb(TIMER_0 + timer, msb) != OK) {
		printf("timer_set_frequency: sys_outb failed");
		return !OK;
	}

	return OK;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
	*bit_no = timer_hook_id = TIMER0_IRQ;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) {
		printf("timer_subscribe_int: sys_irqsetpolicy failed");
		return !OK;
	}
	return OK;
}

int (timer_unsubscribe_int)() {
	if (sys_irqrmpolicy(&timer_hook_id) != OK) {
		printf("timer_unsubscribe_int: sys_irqrmpolicy failed");
		return !OK;
	}
	return OK;
}

void (timer_int_handler)() {
	timer_counter++;
}
