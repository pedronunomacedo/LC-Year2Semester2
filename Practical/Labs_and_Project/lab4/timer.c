#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int timer_hook_id = 0;
unsigned counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st;

  int port = TIMER_0 + timer;

  uint16_t set_freq = TIMER_FREQ / freq;

  uint8_t msb, lsb;
  util_get_MSB(set_freq,&msb);
  util_get_LSB(set_freq,&lsb);

  if (timer_get_conf(timer, &st) != OK) {
    return !OK;
  }

  uint8_t cmd = 0x0;

  cmd = (st & 0xf) | TIMER_LSB_MSB | BIT(4) | (timer << 6);

  if (sys_outb(TIMER_CTRL, cmd) != OK) { return !OK; }

  if (sys_outb(port,lsb) != OK) { return !OK; }
  if (sys_outb(port,msb) != OK) { return !OK; }

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  timer_hook_id = *bit_no = TIMER0_IRQ;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) {
    printf("Timer sys_irqsetpolicy failed");
    return !OK;
  }

  return OK;
}

int (timer_unsubscribe_int)() {

  if (sys_irqrmpolicy(&timer_hook_id) != OK) {
    printf("Timer sys_irqrmpolicy failed");
    return !OK;
  }

  return OK;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t cmd = 0x0;

  cmd = cmd | TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL ,cmd) != OK) {
    return !OK;
  }

  return util_sys_inb(TIMER_0 + timer,st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val u;

  uint8_t buf;

  switch(field) {
    case tsf_all:
      u.byte = st;
      break;
    case tsf_initial:
      buf = (st & (BIT(4) | BIT(5))) >> 4;

      if (buf == 0x1) {
        u.in_mode = LSB_only;
      } else if (buf == 0x2) {
        u.in_mode = MSB_only;
      } else if (buf == 0x3) {
        u.in_mode = MSB_after_LSB;
      } else {
        u.in_mode = INVAL_val;
      }

      break;
    case tsf_mode:
      buf =  (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;

      if (buf == 0x0) {
        u.count_mode = 0;
      } else if (buf == 0x1) {
        u.count_mode = 1;
      } else if (buf == 0x2 || buf == 0x6) {
        u.count_mode = 2;
      } else if (buf == 0x3 || buf == 0x7) {
        u.count_mode = 3;
      } else if (buf == 0x4) {
        u.count_mode = 4;
      } else if (buf == 0x5) {
        u.count_mode = 5;
      }

      break;
    case tsf_base:
      u.bcd = (st & BIT(0));
      break;
  }
  
  return timer_print_config(timer,field,u);
}
