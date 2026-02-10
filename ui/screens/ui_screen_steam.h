#ifndef UI_SCREEN_STEAM_H
#define UI_SCREEN_STEAM_H

#include "lvgl.h"

void        ui_screen_steam_create(void);
lv_obj_t   *ui_screen_steam_get(void);

void        ui_screen_steam_set_temp(int temp_c);
void        ui_screen_steam_set_target(int target_c);
void        ui_screen_steam_set_state(const char *state);

#endif /* UI_SCREEN_STEAM_H */
