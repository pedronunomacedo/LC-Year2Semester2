#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <keyboard.h>

#include <stdbool.h>
#include <stdint.h>

extern uint8_t data;
extern bool error;

int main(int argc, char *argv[]) {
	// sets the language of LCF messages (can be either EN-US or PT-PT)
	lcf_set_language("EN-US");

	// enables to log function invocations that are being "wrapped" by LCF
	// [comment this out if you don't want/need it]
	lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

	// enables to save the output of printf function calls on a file
	// [comment this out if you don't want/need it]
	lcf_log_output("/home/lcom/labs/lab3/output.txt");

	// handles control over to LCF
	// [LCF handles command line arguments and invokes the right function]
	if (lcf_start(argc, argv))
		return 1;

	// LCF clean up tasks
	// [must be the last statement before return]
	lcf_cleanup();

	return 0;
}

int(kbd_test_scan)() {

	int ipc_status, r;
	message msg;

	uint8_t kbd_bit_no;
	kbd_subscribe_int(&kbd_bit_no);
	uint8_t kbd_irq_set = BIT(kbd_bit_no);

	uint8_t scancode[2], size = 0;

	while( data != KBD_ESQ_BC ) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
			printf("driver_receive failed with: %d", r);
			continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      	switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE: 
						if (msg.m_notify.interrupts & kbd_irq_set) {
							kbc_ih();
							scancode[size] = data;
							size++;

							if (error) {
								error = false;
								break;
							}

							if (data == KBD_TWOBYTE_CODE) { continue; }

							kbd_print_scancode(!(scancode[size - 1] & KBD_MSB),size,scancode);

							size = 0;
						}
						break;
					default:
						break; 
			}
		}
	}

	kbd_unsubscribe_int();

	return 0;
}

int(kbd_test_poll)() {
	
	uint8_t scancode[2], size = 0, data = 0x0;

	while ( data != KBD_ESQ_BC ) {
		if (kbd_read_code(&data) != OK) {
			size = 0;
			continue;
		}
		scancode[size] = data;
		size ++;

		if (data == KBD_TWOBYTE_CODE) { 
			tickdelay(micros_to_ticks(DELAY));
			continue;
		}

		kbd_print_scancode(!(scancode[size - 1] & KBD_MSB),size,scancode);
		size = 0;

		tickdelay(micros_to_ticks(DELAY));
	}

	if (kbd_restore_interrupts() != OK) {
		printf("interrupts restore failed");
	}

	return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
	extern int counter;
	int ipc_status, r;
	message msg;

	uint8_t kbd_bit_no, timer_bit_no;

	timer_subscribe_int(&timer_bit_no);
	uint8_t timer_irq_set = BIT(timer_bit_no);

	kbd_subscribe_int(&kbd_bit_no);
	uint8_t kbd_irq_set = BIT(kbd_bit_no);

	uint8_t scancode[2], size = 0;

	int max_ticks = n * 60;

	while( data != KBD_ESQ_BC && counter < max_ticks) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
			printf("driver_receive failed with: %d", r);
			continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      	switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE: 

						if (msg.m_notify.interrupts & timer_irq_set) {
							timer_int_handler();
						}

						if (msg.m_notify.interrupts & kbd_irq_set) {
							counter = 0;
							kbc_ih();
							scancode[size] = data;
							size++;

							if (error) {
								error = false;
								break;
							}

							if (data == KBD_TWOBYTE_CODE) { continue; }

							kbd_print_scancode(!(scancode[size - 1] & KBD_MSB),size,scancode);

							size = 0;
						}
						
						break;
					default:
						break; 
			}
		}
	}

	kbd_unsubscribe_int();

	timer_unsubscribe_int();

	return 0;
}
