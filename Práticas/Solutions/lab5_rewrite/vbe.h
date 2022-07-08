#pragma once

#include <lcom/lcf.h>
#include <machine/int86.h>

#define VBE_INTNO 0x10
#define VBE_AH_INVOKE_FUNCTION 0x4f
#define VBE_AL_SET_MODE 0x02

int (set_video_mode)(uint16_t mode);

void* (vg_init)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_pattern)(uint16_t mode,uint8_t no_rectangles, uint32_t first, uint8_t step);

int (vg_draw_sprite)(char* xpm, xpm_image_t img, uint16_t x, uint16_t y);





