#include "kbc.h"

int mouse_hook_id = MOUSE_IRQ;
static uint8_t data = 0x0;
static bool error = false;

int (mouse_subscribe_int)(uint8_t * bit_no) {
  *bit_no = MOUSE_IRQ;
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

void (mouse_ih)(void) {
  uint8_t st;
  error = false;
  if (get_status_register(&st) != OK) {
    printf("mouse_ih: util_sys_inb failed\n");
  }

  if (st & KBC_OBF && st & KBC_AUX) {
    if (util_sys_inb(KBC_OUT_BUF, &data) != OK) {
      printf("mouse_ih: util_sys_inb failed\n");
    }

    if (st & KBC_PARITY_ERR || st & KBC_TIMEOUT_ERR) {
      error = true;
    }
  }
}

int (get_status_register)(uint8_t * st) {
  if (util_sys_inb(KBC_ST_REG, st) != OK) {
    printf("get_status_register: util_sys_inb failed\n");
    return !OK;
  }
  return OK;
}

uint8_t (get_mouse_data)() { return data; }

bool (check_mouse_error)() { return error; }

struct packet (get_mouse_packet)(uint8_t * mouse_packet) {
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

static int x_delta;
void (update_state)(struct packet pp, draw_state * state, uint8_t x_len, uint8_t tolerance) {
  switch (*state)
  {
    case START:
      if (check_only_lb(pp)) {
        *state = LEFT;
        x_delta = 0;
      }
      break;
    
    case LEFT:
      if (pp.delta_x < -tolerance || pp.delta_y < -tolerance || !check_only_lb(pp)) {
        *state = START;
      } else {
        x_delta += pp.delta_x;
        if (x_delta >= x_len) {
          *state = LEFT_COMPLETE;
        }
      }
      break;
    
    case LEFT_COMPLETE:
      if (pp.delta_x < -tolerance || pp.delta_y < -tolerance) {
        *state = START;
      } else if (check_none_pressed(pp)) {
        *state = RIGHT_START;
      } else if (check_only_lb(pp)) {
        break;
      } else {
        *state = START;
      }
      break;

    case RIGHT_START:
      if (abs(pp.delta_x) > tolerance || abs(pp.delta_y) > tolerance) {
        printf("1 - %d - %d\n", abs(pp.delta_x) > tolerance, abs(pp.delta_y) > tolerance);
        *state = START;
      } else if (check_only_rb(pp)) {
        *state = RIGHT;
        x_delta = 0;
      } else if (check_none_pressed(pp)) {
        break;
      } else {
        *state = START;
      }
      break;

    case RIGHT:
      if (pp.delta_x < -tolerance || pp.delta_y > tolerance || !check_only_rb(pp)) {
        *state = START;
      } else {
        x_delta += pp.delta_x;
        if (x_delta >= x_len) {
          *state = RIGHT_COMPLETE;
        }
      }
      break;
    
    case RIGHT_COMPLETE:
      if (pp.delta_x < -tolerance || pp.delta_y > tolerance) {
        *state = START;
      } else if (check_none_pressed(pp)) {
        *state = END;
      } else if (check_only_rb(pp)) {
        break;
      } else {
        *state = START;
      }
      break;

    default:
      break;
  }
}

bool (check_only_lb)(struct packet pp) { 
  return pp.lb && !pp.rb && !pp.mb;
}

bool (check_none_pressed)(struct packet pp) { 
  return !pp.lb && !pp.rb && !pp.mb;
}

bool (check_only_rb)(struct packet pp) { 
  return !pp.lb && pp.rb && !pp.mb;
}
