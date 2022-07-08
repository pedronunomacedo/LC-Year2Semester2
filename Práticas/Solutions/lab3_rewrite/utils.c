#include <lcom/lcf.h>
#include <stdint.h>

int (util_sys_inb)(int port, uint8_t * val) {
  uint32_t input;
  if (sys_inb(port, &input) != OK) {
    printf("util_sys_inb: sys_inb failed");
    return !OK;
  }

  *val = (uint8_t)input;
  return OK;
}
