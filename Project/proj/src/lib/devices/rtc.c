#include "rtc.h"

static char current_date[20];
static int rtc_hook_id;

int (rtc_subscribe_int)(uint8_t * bit_no) {
  * bit_no = rtc_hook_id = RTC_IRQ;

  uint8_t reg_c = 0;
  if (rtc_read_register(RTC_REG_C, &reg_c) != OK) {
    printf("rtc_subscribe_int: rtc_read_register failed");
    return !OK;
  }

  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id) != OK) {
    printf("rtc_subscribe_int: sys_irqsetpolicy failed");
    return !OK;
  }
  return OK;
}

int (rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&rtc_hook_id) != OK) {
    printf("rtc_subscribe_int: sys_irqrmpolicy failed");
    return !OK;
  }
  return OK;
}

int (rtc_read_register)(uint8_t reg, uint8_t * input) {
  if (sys_outb(RTC_ADDR_REG, reg) != OK) {
    printf("rtc_read_register: sys_outb failed");
    return !OK;
  }

  if (util_sys_inb(RTC_DATA_REG, input) != OK) {
    printf("rtc_read_register: util_sys_inb failed");
    return !OK;
  }

  return OK;
}

int (rtc_write_register)(uint8_t reg, uint8_t output) {
  if (sys_outb(RTC_ADDR_REG, reg) != OK) {
    printf("rtc_write_register: sys_outb failed");
    return !OK;
  }

  if (sys_outb(RTC_DATA_REG, output) != OK) {
    printf("rtc_write_register: sys_outb failed");
    return !OK;
  }

  return OK;
}

void (wait_for_a_valid_rtc)(void) {
  uint8_t reg_a = 0;
  do {
    rtc_disable_int();
    rtc_read_register(RTC_REG_A, &reg_a);
    rtc_enable_update_int();
    tickdelay(micros_to_ticks(RTC_DELAY));
  } while ( reg_a & RTC_UIP);
}

void (rtc_ih)(void) {
  uint8_t reg_c;
  
  rtc_read_register(RTC_REG_C, &reg_c);

  if( reg_c & RTC_UF )
    rtc_handle_update_int();
}

int (rtc_disable_int)(void) {
  uint8_t reg_b = 0;
  if (rtc_read_register(RTC_REG_B, &reg_b) != OK) {
    printf("rtc_disable_int: rtc_read_register failed\n");
    return !OK;
  }
  reg_b &= ~RTC_UIE;
  reg_b &= ~RTC_AIE;
  reg_b &= ~RTC_PIE;
  if (rtc_write_register(RTC_REG_B, reg_b) != OK) {
    printf("rtc_disable_int: rtc_read_register failed\n");
    return !OK;
  }
  return OK;
}

int (rtc_enable_update_int)(void) {
  uint8_t reg_b = 0;
  if (rtc_read_register(RTC_REG_B, &reg_b) != OK) {
    printf("rtc_enable_update_int: rtc_read_register failed\n");
    return !OK;
  }
  reg_b |= RTC_UIE;
  reg_b |= RTC_DM;
  if (rtc_write_register(RTC_REG_B, reg_b) != OK) {
    printf("rtc_enable_update_int: rtc_read_register failed\n");
    return !OK;
  }
  return OK;
}

void (rtc_handle_update_int)(void) {
  wait_for_a_valid_rtc();
  uint8_t year, month, day, hour, minute, second;

  if (rtc_read_register(RTC_YEAR, &year) != OK ||
      rtc_read_register(RTC_MONTH, &month) != OK ||
      rtc_read_register(RTC_DAY_MONTH, &day) != OK ||
      rtc_read_register(RTC_HOURS, &hour) != OK ||
      rtc_read_register(RTC_MINUTES, &minute) != OK ||
      rtc_read_register(RTC_SECONDS, &second) != OK) {
    return;
  }

  sprintf(current_date, "%02d/%02d/%04d %02d:%02d:%02d\n", day, month, year + RTC_BASE_YEAR, hour, minute, second);
}

char * (rtc_get_current_date)(void) { return current_date; }
