#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"

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
  if (kbd_subscribe_int(&kbd_bit_no) != OK) {
    printf("kbd_test_scan: kbd_subscribe_int failed");
    return !OK;
  }
  uint8_t kbd_irq_set = BIT(kbd_bit_no);

  uint8_t scancode[2], size = 0;

  while( kbd_get_data() != ESQ_BREAK_CODE ) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:			
          if (msg.m_notify.interrupts & kbd_irq_set) { 
            if (kbd_ih() != OK) { continue; }
            scancode[size] = kbd_get_data();

            if (scancode[size] == TWO_BYTE_CODE) { size++; continue; }
            size++;

            kbd_print_scancode(!(scancode[size - 1] & SCANCODE_MSB),size,scancode);
            size = 0;
          }
          break;
        default:
          break;
        }
    }
  }

  if (kbd_unsubscribe_int() != OK) {
    printf("kbd_test_scan: kbd_unsubscribe_int failed");
    return !OK;
  }
  return OK;
}

int(kbd_test_poll)() {

  uint8_t scancode[2], size = 0;

  while (kbd_get_data() != ESQ_BREAK_CODE) {
    if (kbd_ih() != OK) { continue; }
    scancode[size] = kbd_get_data();

    if (scancode[size] == TWO_BYTE_CODE) { size++; continue; }
    size++;

    kbd_print_scancode(!(scancode[size - 1] & SCANCODE_MSB),size,scancode);
    size = 0;

    tickdelay(micros_to_ticks(DELAY));
  }

  return kbd_enable_interrupts();
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r;
  message msg;

  uint8_t kbd_bit_no, timer_bit_no;
  if (kbd_subscribe_int(&kbd_bit_no) != OK || timer_subscribe_int(&timer_bit_no) != OK) {
    printf("kbd_test_scan: kbd_subscribe_int failed");
    return !OK;
  }
  uint8_t kbd_irq_set = BIT(kbd_bit_no);
  uint8_t timer_irq_set = BIT(timer_bit_no);

  uint8_t scancode[2], size = 0;

  extern int counter;

  while( (unsigned)counter <= n * sys_hz() && kbd_get_data() != ESQ_BREAK_CODE ) {
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
            if (kbd_ih() != OK) { continue; }
            scancode[size] = kbd_get_data();

            if (scancode[size] == TWO_BYTE_CODE) { size++; continue; }
            size++;

            kbd_print_scancode(!(scancode[size - 1] & SCANCODE_MSB),size,scancode);
            size = 0;
            counter = 0;
          }
          break;
        default:
          break;
        }
    }
  }

  if (kbd_unsubscribe_int() != OK || timer_unsubscribe_int() != OK) {
    printf("kbd_test_scan: kbd_unsubscribe_int failed");
    return !OK;
  }
  return OK;
}
