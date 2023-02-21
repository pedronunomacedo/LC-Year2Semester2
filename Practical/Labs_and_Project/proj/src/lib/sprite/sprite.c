#include "sprite.h"

sprite * (sprite_new)(xpm_map_t xpm, uint32_t x, uint32_t y) {
    sprite * s = (sprite *)malloc(sizeof(sprite));
    if (s == NULL) { return NULL; }
    s->map = xpm_load(xpm, XPM_8_8_8_8, &(s->img));
    s->x = x;
    s->y = y;
    return s;
}

void (sprite_destroy)(sprite * sprite) {
    free(sprite->map);
    free(sprite);
}

void (sprite_set_pos)(sprite * sprite, uint32_t x, uint32_t y) {
    sprite->x = x;
    sprite->y = y;
}

void (sprite_set_pos_delta)(sprite * sprite, int x_delta, int y_delta) {
    if ((int)sprite->x + x_delta + sprite->img.width > WINDOW_WIDTH) {
        sprite->x = WINDOW_WIDTH - sprite->img.width;
    } else if ((int)sprite->x + x_delta < 0) {
        sprite->x = 0;
    } else { sprite->x += x_delta; }

    if ((int)sprite->y - y_delta + sprite->img.height > WINDOW_HEIGHT) {
        sprite->y = WINDOW_HEIGHT - sprite->img.height;
    } else if ((int)sprite->y - y_delta < 0) {
        sprite->y = 0;
    } else { sprite->y -= y_delta; }
}

