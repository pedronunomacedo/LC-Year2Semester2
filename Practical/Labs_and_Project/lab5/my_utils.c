#include <my_utils.h>

void (delay_seconds)(uint8_t delay) {
  tickdelay(micros_to_ticks(delay*1000000));
}
