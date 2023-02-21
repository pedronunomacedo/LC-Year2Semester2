#include <lcom/lcf.h>
#include <stdint.h>

int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return !OK;
  *lsb = (uint8_t)val;
  return OK;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return !OK;
  *msb = (uint8_t)(val >> 8);
  return OK;
}

int util_sys_inb(int port, uint8_t * value) {
  if (value == NULL) return !OK;
  uint32_t input = 0x0;
  if (sys_inb(port, &input) != OK) { return !OK; }
  *value = (uint8_t)input;
  return OK;
}
