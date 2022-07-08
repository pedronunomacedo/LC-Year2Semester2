// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/pp.h>

#include <stdint.h>

/* 
1º - Configure the video card to operate in the mode specified in its
first argument
2º map its frame-buffer
3º Then draw a line (either horizontal or vertical)
4º Sleep for delay seconds
5º Switch back to Minix’s text mode

*/

// Any header files included below this line should have been created by you


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/pp/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/pp/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

#define BIT(n) (1<<n)

typedef struct {
  uint16_t x_res; // horizontal resolution
  uint16_t y_res; // vertical resolution
  uint8_t bpp; // bits per pixel
  uint8_t r_sz; // red component size
  uint8_t r_pos; // red component LSB position
  uint8_t g_sz; // green component size
  uint8_t g_pos; // green component LSB position
  uint8_t b_sz; // blue component size
  uint8_t b_pos; // blue component LSB position
  phys_addr_t phys_addr; // video ram base phys address
} lpv_mode_info_t;

enum lpv_dir_t {
  lpv_hor, // horizontal line
  lpv_vert // vertical line
};

static char *video_mem;
lpv_mode_info_t info;
uint8_t bytes_per_pixel;
static unsigned int vram_size;

static uint16_t x_resolution;
static uint16_t y_resolution;

/*
int (set_video_mode) (uint16_t mode) {
  reg86_t reg86;

  memset(reg86, 0, sizeof(reg86));

  reg86.intno = VBE_INTERRUPT_VECTOR; // 0x10
  reg86.ah = VBE_AH; // 0x4f
  reg86.al = VBE_AL; // 0x02
  reg86.bx = mode | LINEAR_FRAME_BUFFER; // Activate BIT(14) on the mode specified

  if (sys_int86(&reg86) != 0) { // Makes the BIOS call
    printf("ERROR: sys_int86() failed!\n");
    return 1;
  }

  return 0;
}
*/

int vg_exit() {
  reg86_t reg86;

  reg86.intno = 0x10;
  reg86.ah = 0x00;
  reg86.al = 0x03;

  if (sys_int86(&reg86) != 0) {
    printf("ERROR: sys_int86() in vg_exit() function failed!\n");
    return 1;
  }

  return 0;
}

int draw_pixel(int x, int y, uint32_t color) {
  color &= set_bits(0, info.bpp); // Return the hexa code of the color given, but with the restrition of the bits per pixel

  // Copies the hexa code color to the pixel specified (x,y) in the buffer (video_mem)
  memcpy(video_mem + ((y * info.x_resolution) + x) * bytes_per_pixel, &color, bytes_per_pixel);

  return 0;
}

int draw_line(uint16_t x, uint16_t y, uint16_t len, uint32_t color, lpv_dir_t dir) {
  if (dir == lpv_hor) {
    for (int i = 0; i < len; i++) {
      if (draw_pixel(x + i, y, color) != 0) {
        printf("ERROR: draw_pixel() failed!\n");
        return 1;
      }
    }
  }
  else if (dir == lpv_vert) {
    for (int i = 0; i < len; i++) {
      if (draw_pixel(x, y + i, color) != 0) {
        printf("ERROR: draw_pixel() failed!\n");
        return 1;
      }
    }
  }
}

int pp_test_line(uint_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {
  // Get mode information
  if (lpv_get_mode_info(mode, &info) != 0) {
    printf("ERROR: lpv_get_mode_info() failed!\n");
    return 1;
  }

  /* Create virtual memory (in this case, minix memory) */
  struct minix_memory_range mr;
  unsigned int vram_base = info.phys_addr;
  vram_size = info.y_res * ((info.bpp + 7) / 8);

  /* Allow memory mapping */
  mr.mr_base = (phys_addr) vram_base;
  mr.mr_limit = mr.mr_base + vram_size; // Um buffer

  // Grant a process the permission to map a given address range
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != 0) {
    printf("ERROR: sys_privctl() failed!\n");
    return 1;
  }

  /* Memory mapping */
  video_mem = video_map_phys(mr.mr_base, vram_size); // Returns the virtual address of the specified physical memory range
  if (video_mem == MAP_FAILED) {
    panic("ERROR: video_map_phys() failed!\n");
  }
  bzero(video_mem, vram_size);

  /* Set video mode */
  if (lpv_set_mode(mode) != 0) { // Set the video mode given
    printf("ERROR: lpv_set_mode() failed!\n");
    return 1;
  }

  bytes_per_pixel = ( (info.bpp + 7)/8 );

  /* Analyze te color given */
  uint32_t color2;
  uint32_t red, red_first, green, green_first, blue, blue_first;

  if (mode == 0) { // text mode
    printf("Not avaliated!\n");
    return vg_exit();
  }

  if (!(mode == 1)) { // Direct mode (not indexed_mode or text_mode)
    red = (color >> info.r_pos) && set_bits(0, info.r_sz);
    green= (color >> info.g_pos) && set_bits(0, info.g_sz);
    blue = (color >> info.b_pos) && set_bits(0, info.b_sz);

    color2 = (red << info.r_pos) | (green << info.g_pos) | (blue << info.b_pos);
  }

  if (draw_line(x, y, len, color2, dir) != 0) {
    printf("ERROR: draw_line() failed!\n");
    return 1;
  }

  wait_for_ESQ();

  return vg_exit();
}

int set_bits(uint8_t start, uint8_t end) {
  uint32_t bits = 0x0;
  for (int i = start; i < end; i++) {
    bits |= BIT(i);
  }

  return bits;
}