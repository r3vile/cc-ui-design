#ifndef SCREEN_HOTWATER_H
#define SCREEN_HOTWATER_H

#include "lvgl.h"

lv_obj_t *screen_hotwater_create(void);
void screen_hotwater_set_temp(int current_temp, int target_temp, const char *state);

#endif /* SCREEN_HOTWATER_H */
