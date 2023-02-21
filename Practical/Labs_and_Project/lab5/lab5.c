#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <lcom/timer.h>
#include <keyboard.h>
#include <graphics.h>
#include <my_utils.h>

#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  if ( set_video_mode(mode) != OK ) {
    printf("video_test_init(): vg_init() failed\n");
    return !OK;
  }

  delay_seconds(delay);
  return vg_exit();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
uint16_t width, uint16_t height, uint32_t color) {
  
  vg_init(mode);

  vg_draw_rectangle(x,y,width,height,color);

  wait_for_ESQ();
  
  return vg_exit();
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vg_init(mode);

  vg_draw_matrix(mode == MODE_RES_1024x768_BITS_8, no_rectangles, first, step);

  wait_for_ESQ();

  return vg_exit();
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  vg_init(MODE_RES_1024x768_BITS_8);

  xpm_image_t img;
  uint8_t * sprite = xpm_load(xpm, XPM_INDEXED, &img);

  vg_draw_sprite(sprite, img, x, y);

  wait_for_ESQ();

  return vg_exit();
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  
  vg_init(MODE_RES_1024x768_BITS_8);

  xpm_image_t img;
  uint8_t * sprite = xpm_load(xpm, XPM_INDEXED, &img);

  vg_draw_sprite(sprite,img,xi,yi);

  extern int counter;
  extern uint8_t data;

  uint8_t timer_bit_no, kbd_bit_no;
  if (timer_subscribe_int(&timer_bit_no) != OK) { return !OK; }
  if (kbd_subscribe_int(&kbd_bit_no) != OK) { return !OK; }
  uint8_t timer_irq_set = BIT(timer_bit_no);
  uint8_t kbd_irq_set = BIT(kbd_bit_no);

  int r, ipc_status;
  message msg;

  int x_translate = (xi <= xf) ? 1 : -1;
  int y_translate = (yi <= yf) ? 1 : -1;  
  int frame_counter = -1 * speed;

  while( data != KBD_ESQ_BC ) {
    
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: 
              if (msg.m_notify.interrupts & timer_irq_set) { 
                if ( !(xi == xf && yi == yf) ) {
                  timer_int_handler();

                  if (counter % (60 / fr_rate) == 0) { 
                    if (speed >= 0) { //displacement in pixels between consecutive frames
                      if (xi == xf) { 
                        yi += y_translate * speed;
                      } else { 
                        xi += x_translate * speed;
                      }
                    } else { //number of frames required for a displacement of one pixel
                      frame_counter--;
                      if (frame_counter == 0) {
                        if (xi == xf) { 
                          yi += y_translate;
                        } else { 
                          xi += x_translate;
                        }
                        frame_counter = -1 * speed;
                      }
                    }
                    vg_draw_sprite(sprite, img, xi, yi);
                    //vg_swap_buffers();
                  }
                }
              }
              if (msg.m_notify.interrupts & kbd_irq_set) { 
                kbc_ih();
              }
              break;
          default:
              break; 
      }
    }
  }

  vg_exit();
  kbd_unsubscribe_int();
  timer_unsubscribe_int();

  return OK;
}

int(video_test_controller)() {

  return 1;
}

