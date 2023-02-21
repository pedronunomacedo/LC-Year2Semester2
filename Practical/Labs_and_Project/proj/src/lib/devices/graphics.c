#include "graphics.h"

static char * video_mem[3] = { NULL, NULL, NULL };
static vbe_mode_info_t vmi;
static uint8_t current_buffer = 0;
static uint8_t bytes_per_pixel;
static unsigned int vram_size;

int (vg_set_video_mode)(uint16_t mode) {
	reg86_t reg86;

	memset(&reg86, 0, sizeof(reg86));

	reg86.intno = VBE_INTERRUPT_VECTOR;
	reg86.ah = VBE_AH;
	reg86.al = SET_VBE_MODE;
	reg86.bx = mode | LINEAR_FRAME_BUFFER;

	if (sys_int86(&reg86) != OK) {
		printf("vg_init(): sys_int86() failed\n");
		return !OK;
	}
	
	return OK;
}

int (vg_init_graphic)(uint16_t mode) {
	int r;	
	memset(&vmi, 0, sizeof(vmi));

	if (vbe_get_mode_info(mode, &vmi) != OK) {
		printf("vg_init(): vbe_get_mode_info() failed\n");
		return !OK;
	}

	struct minix_mem_range mr;
	unsigned int vram_base = vmi.PhysBasePtr;
	vram_size = vmi.YResolution * vmi.BytesPerScanLine;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;	
	mr.mr_limit = mr.mr_base + 3 * vram_size;  

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	for (int i = 0; i < 3; i++) {
		video_mem[i] = (char *) vm_map_phys(SELF, (void *)(mr.mr_base + i*vram_size), vram_size);

		if(video_mem[i] == MAP_FAILED)
			panic("couldn't map video memory");
		
		bzero(video_mem[i], vram_size);
	}

	if (vg_set_video_mode(mode) != OK) {
		printf("vg_init(): vg_set_video_mode() failed\n");
		return !OK;
	}

	bytes_per_pixel = vmi.BytesPerScanLine / vmi.XResolution;

	return OK;
}

int (vg_swap_buffers)() {
	reg86_t reg86;

	memset(&reg86, 0, sizeof(reg86));

	reg86.intno = VBE_INTERRUPT_VECTOR;
	reg86.ah = VBE_AH;
	reg86.al = AL_SET_VBE_START;
	reg86.dx = (current_buffer) * vmi.YResolution;

	if (sys_int86(&reg86) != OK) {
		printf("vg_swap_buffers: sys_int86() failed");
		return !OK;
	}
	
	current_buffer = (current_buffer + 1) % 3;
	return OK;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
	if (color == TRANSPARENCY_COLOR_8_8_8_8) { return OK; }

	memcpy(video_mem[current_buffer] + bytes_per_pixel * (x + y * vmi.XResolution),
				 &color, bytes_per_pixel);

	return OK;
}

int (vg_draw_pixel_in_buffer)(char * buffer, uint16_t x, uint16_t y, uint32_t color) {
	if (color == TRANSPARENCY_COLOR_8_8_8_8) { return OK; }

	memcpy(buffer + bytes_per_pixel * (x + y * vmi.XResolution),
				 &color, bytes_per_pixel);

	return OK;
}

int (vg_draw_sprite)(struct sprite * sprite) {
	uint32_t color;
	
	for (int row = 0; row < sprite->img.height; row++) {
		for (int col = 0; col < sprite->img.width; col++) {
			memcpy(&color, sprite->map + bytes_per_pixel*(col + row * sprite->img.width), bytes_per_pixel);

			if (vg_draw_pixel(sprite->x + col, sprite->y + row, color) != OK) {
				return !OK;
			}
		}
	}
	return OK;
}

int (vg_draw_sprite_in_buffer)(char * buffer, struct sprite * sprite) {
	uint32_t color;
	
	for (int row = 0; row < sprite->img.height; row++) {
		for (int col = 0; col < sprite->img.width; col++) {
			memcpy(&color, sprite->map + bytes_per_pixel*(col + row * sprite->img.width), bytes_per_pixel);

			if (vg_draw_pixel_in_buffer(buffer, sprite->x + col, sprite->y + row, color) != OK) {
				return !OK;
			}
		}
	}
	return OK;
}

int (vg_draw_block_sprite_without_checks)(struct sprite * sprite) {
	if (sprite->img.height == vmi.YResolution && sprite->img.width == vmi.XResolution) {
		memcpy(video_mem[current_buffer], sprite->map, vram_size);
	} else {
		uint32_t bytes_per_sprite_line;
		for (int row = 0; row < sprite->img.height; row++) {
			bytes_per_sprite_line = bytes_per_pixel * sprite->img.width;
			memcpy(video_mem[current_buffer] + 
					bytes_per_pixel * (sprite->x + vmi.XResolution * (row + sprite->y)), 
					sprite->map + bytes_per_sprite_line * row,
					bytes_per_sprite_line);
		}
	}
	return OK;
}

unsigned int (vg_get_vram_size)() {
	return vram_size;
}

void (vg_set_current_buffer)(char * buffer) {
	memcpy(video_mem[current_buffer], buffer, vram_size);
}
