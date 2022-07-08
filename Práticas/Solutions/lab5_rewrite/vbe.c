#include "vbe.h"

static char *video_mem; 
static vbe_mode_info_t vmi;
static uint8_t bytes_per_pixel;

int (set_video_mode)(uint16_t mode){
  reg86_t reg;
  bzero(&reg, sizeof(reg86_t));

  //Invoking VBE function
  reg.intno = VBE_INTNO;
  reg.ah = VBE_AH_INVOKE_FUNCTION;
  reg.al = VBE_AL_SET_MODE;

  //Set mode
  reg.bx = BIT(14) | mode;

  if (sys_int86(&reg) != OK) {
    return !OK;
  }
  return OK;
}

void* (vg_init)(uint16_t mode) {
  bzero(&vmi, sizeof(vbe_mode_info_t));		    
  if (vbe_get_mode_info(mode,&vmi) != OK) { return NULL; }

  struct minix_mem_range mr;
  unsigned int vram_base = vmi.PhysBasePtr;  
  unsigned int vram_size = vmi.BytesPerScanLine * vmi.YResolution;

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  int r;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = (char *)vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  bzero(video_mem,vram_size);
  set_video_mode(mode);

  bytes_per_pixel = vmi.BytesPerScanLine / vmi.XResolution;

  return NULL;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x > vmi.XResolution || y > vmi.YResolution) { return !OK; }
  uint32_t pos = bytes_per_pixel * (x + y * vmi.XResolution);
  memcpy(video_mem + pos, &color, bytes_per_pixel);
  return OK;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != OK) { return !OK; }
  }
  return OK;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    if (vg_draw_hline(x,y+i,width,color) != OK) { return !OK; }
  }
  return OK;
}

uint32_t set_bits(uint8_t size) {
  uint32_t res = 0x0;
  for (uint8_t i = 0; i < size; i++) {
    res |= BIT(i);
  }
  return res;
}

int (vg_draw_pattern)(uint16_t mode,uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint32_t rect_width = vmi.XResolution / no_rectangles;
  uint32_t rect_height = vmi.YResolution / no_rectangles;

  if (mode == 0x105) {
    for (int row = 0; row < no_rectangles; row++) {
      for (int col = 0; col < no_rectangles;  col++) {
        uint32_t color = (first + (row * no_rectangles + col) * step) % (1 << vmi.BitsPerPixel);
        vg_draw_rectangle(col * rect_width, row * rect_height, rect_width, rect_height, color);
      }
    }
  } else {
    uint32_t first_red = (first >> vmi.RedFieldPosition) & set_bits(vmi.RedMaskSize);
    uint32_t first_green = (first >> vmi.GreenFieldPosition) & set_bits(vmi.GreenMaskSize);
    uint32_t first_blue = (first >> vmi.BlueFieldPosition) & set_bits(vmi.RedMaskSize);

    for (int row = 0; row < no_rectangles; row++) {
      for (int col = 0; col < no_rectangles;  col++) {
        uint32_t red = (first_red + col * step) % (1 << vmi.RedMaskSize);
        uint32_t green = (first_green + row * step) % (1 << vmi.GreenMaskSize);
        uint32_t blue = (first_blue + (col + row) * step) % (1 << vmi.BlueMaskSize);

        uint32_t color = (red << vmi.RedFieldPosition) | (green << vmi.GreenFieldPosition) | (blue << vmi.BlueFieldPosition);
        vg_draw_rectangle(col * rect_width, row * rect_height, rect_width, rect_height, color);
      }
    }
  }

  return OK;
}

int (vg_draw_sprite)(char* xpm, xpm_image_t img, uint16_t x, uint16_t y) {
  for (int i = 0; i < img.height; i++) {
    memcpy(video_mem + bytes_per_pixel * (x + (y + i) * vmi.XResolution),
            xpm + bytes_per_pixel * i * img.width,
            bytes_per_pixel * img.width);
  }
  return OK;
}
