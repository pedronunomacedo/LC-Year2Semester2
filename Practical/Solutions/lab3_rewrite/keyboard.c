#include "keyboard.h"

int kbd_hook_id;

int (kbd_subscribe_int)(uint8_t * bit_no) {
  *bit_no = kbd_hook_id = KBD_IRQ;
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &kbd_hook_id) != OK) {
    printf("kbd_subscribe_int: sys_irqsetpolicy failed");
    return !OK;
  }
  return OK;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
    printf("kbd_subscribe_int: sys_irqsetpolicy failed");
    return !OK;
  }
  return OK;
}

int (get_status_register)(uint8_t * st) {
  if (util_sys_inb(KBD_ST_REG, st) != OK) {
    printf("get_status_register: util_sys_inb failed");
    return !OK;
  }
  return OK;
}

uint8_t data = 0x0;

uint8_t (kbd_get_data)() {
  return data;
}

int (kbd_ih)() {
  uint8_t st;
  if (get_status_register(&st) != OK) {
    printf("kbd_ih: get_status_register failed");
    return !OK;
  }

  if (st & KBD_OBF) {
    if (util_sys_inb(KBD_OUT_BUF, &data) != OK) {
      printf("kbd_ih: util_sys_inb failed");
      return !OK;
    }
    if (st & KBD_TIMEOUT_ERROR || st & KBD_PARITY_ERROR) {
      return !OK;
    }
    return OK;
  }
  return !OK;
}

int (kbd_enable_interrupts)() {
  uint8_t cmd = 0x0;
  if (sys_outb(KBD_CMD_REG, KBD_RCB) != OK) {
    printf("kbd_enable_interrupts: util_sys_inb failed");
    return !OK;
  }

  if (util_sys_inb(KBD_OUT_BUF, &cmd) != OK) {
    printf("kbd_enable_interrupts: util_sys_inb failed");
    return !OK;
  }

  cmd |= KBD_ENABLE_INT;

  if (sys_outb(KBD_CMD_REG, KBD_WCB) != OK) {
    printf("kbd_enable_interrupts: util_sys_inb failed");
    return !OK;
  }

  if (sys_outb(KBD_ARG_REG, cmd) != OK) {
    printf("kbd_enable_interrupts: util_sys_inb failed");
    return !OK;
  }
  return OK;  
}
