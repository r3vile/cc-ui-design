#ifndef SCREEN_STEAM_H
#define SCREEN_STEAM_H

#include "lvgl.h"

lv_obj_t *screen_steam_create(void);
void screen_steam_set_temp(int current_temp, int target_temp, const char *state);

#endif /* SCREEN_STEAM_H */
