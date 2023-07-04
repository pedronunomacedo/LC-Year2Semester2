// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "kbc.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  if (set_mouse_data_reporting(true) != OK) {
    printf("mouse_test_packet: set_mouse_data_reporting failed\n");
    return !OK;
  }

  int r, ipc_status;
  message msg;

  uint8_t mouse_bit_no;
  if (mouse_subscribe_int(&mouse_bit_no) != OK) {
    printf("mouse_test_packet: mouse_subscribe_int failed\n");
    return !OK;
  }
  uint32_t mouse_irq_set = BIT(mouse_bit_no);

  uint8_t mouse_packet[3];
  uint8_t counter = 0;
  uint8_t data;
  bool is_sync = false;
  struct packet pp;

  while( cnt > 0 ) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: 
              if (msg.m_notify.interrupts & mouse_irq_set) { 
                mouse_ih();

                if (check_mouse_error()) { continue; }
                data = get_mouse_data();
                
                if (counter == 0 && (data & MOUSE_FLAG)) { is_sync = true; }

                if (is_sync) {
                  mouse_packet[counter] = data;

                  if (counter == 2) {
                    is_sync = false;
                    counter = 0;
                    cnt--;
                    pp = get_mouse_packet(mouse_packet);
                    mouse_print_packet(&pp);
                  } else { counter++; }
                }
              }
              break;
          default:
              break; 
      }
    }
  }
  if (mouse_unsubscribe_int() != OK) {
    printf("mouse_test_packet: mouse_subscribe_int failed\n");
    return !OK;
  }
  return set_mouse_data_reporting(false);
}

int (mouse_test_async)(uint8_t idle_time) {
    if (set_mouse_data_reporting(true) != OK) {
    printf("mouse_test_packet: set_mouse_data_reporting failed\n");
    return !OK;
  }

  int r, ipc_status;
  message msg;

  uint8_t mouse_bit_no, timer_bit_no;
  if (mouse_subscribe_int(&mouse_bit_no) != OK || timer_subscribe_int(&timer_bit_no) != OK) {
    printf("mouse_test_packet: mouse_subscribe_int  failed\n");
    return !OK;
  }
  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);

  uint8_t mouse_packet[3];
  uint8_t byte_counter = 0;
  uint8_t data;
  bool is_sync = false;
  struct packet pp;

  extern unsigned counter;

  while( counter < sys_hz() * idle_time ) {
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
              if (msg.m_notify.interrupts & mouse_irq_set) { 
                mouse_ih();

                if (check_mouse_error()) { continue; }
                data = get_mouse_data();
                
                if (byte_counter == 0 && (data & MOUSE_FLAG)) { is_sync = true; }

                if (is_sync) {
                  mouse_packet[byte_counter] = data;

                  if (byte_counter == 2) {
                    is_sync = false;
                    byte_counter = 0;
                    pp = get_mouse_packet(mouse_packet);
                    mouse_print_packet(&pp);
                    counter = 0;
                  } else { byte_counter++; }
                }
              }
              break;
          default:
              break; 
      }
    }
  }
  if (timer_unsubscribe_int() != OK || mouse_unsubscribe_int() != OK) {
    printf("mouse_test_packet: mouse_subscribe_int failed\n");
    return !OK;
  }
  return set_mouse_data_reporting(false);
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  if (set_mouse_data_reporting(true) != OK) {
    printf("mouse_test_packet: set_mouse_data_reporting failed\n");
    return !OK;
  }

  int r, ipc_status;
  message msg;

  uint8_t mouse_bit_no;
  if (mouse_subscribe_int(&mouse_bit_no) != OK) {
    printf("mouse_test_packet: mouse_subscribe_int failed\n");
    return !OK;
  }
  uint32_t mouse_irq_set = BIT(mouse_bit_no);

  uint8_t mouse_packet[3];
  uint8_t counter = 0;
  uint8_t data;
  bool is_sync = false;
  struct packet pp;

  draw_state state = START;

  while( state != END ) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: 
              if (msg.m_notify.interrupts & mouse_irq_set) { 
                mouse_ih();

                if (check_mouse_error()) { continue; }
                data = get_mouse_data();
                
                if (counter == 0 && (data & MOUSE_FLAG)) { is_sync = true; }

                if (is_sync) {
                  mouse_packet[counter] = data;

                  if (counter == 2) {
                    is_sync = false;
                    counter = 0;
                    pp = get_mouse_packet(mouse_packet);
                    mouse_print_packet(&pp);

                    update_state(pp, &state, x_len, tolerance);
                  } else { counter++; }
                }
              }
              break;
          default:
              break; 
      }
    }
  }
  if (mouse_unsubscribe_int() != OK) {
    printf("mouse_test_packet: mouse_subscribe_int failed\n");
    return !OK;
  }
  return set_mouse_data_reporting(false);
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
