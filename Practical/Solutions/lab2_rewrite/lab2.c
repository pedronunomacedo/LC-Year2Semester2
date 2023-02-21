#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t st;
  if (timer_get_conf(timer, &st) != OK) {
    printf("timer_test_read_config: timer_get_conf failed");
    return !OK;
  }

  if (timer_display_conf(timer, st, field) != OK) {
    printf("timer_test_read_config: timer_display_conf failed");
    return !OK;
  } 

  return OK;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  return timer_set_frequency(timer, freq);
}

int(timer_test_int)(uint8_t time) {
  int ipc_status, r;
  message msg;

  uint8_t timer_bit_no;
  if (timer_subscribe_int(&timer_bit_no) != OK) {
    printf("timer_test_int: timer_subscribe_int failed");
    return !OK;
  }
  uint8_t timer_irq_set = BIT(timer_bit_no);
  extern int counter;

  while( (unsigned)counter < time * sys_hz() ) {
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:			
                if (msg.m_notify.interrupts & timer_irq_set) { 
                  timer_int_handler();

                  if (counter % sys_hz() == 0) {
                    timer_print_elapsed_time();
                  }    
                }
                break;
            default:
                break;
        }
    } 
  }
  if (timer_unsubscribe_int() != OK) {
    printf("timer_test_int: timer_unsubscribe_int failed");
    return !OK;
  }
  return OK;
}
