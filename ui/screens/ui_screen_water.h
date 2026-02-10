#ifndef UI_SCREEN_WATER_H
#define UI_SCREEN_WATER_H

#include "lvgl.h"
#include <stdbool.h>

void        ui_screen_water_create(void);
lv_obj_t   *ui_screen_water_get(void);

void        ui_screen_water_set_temp(int temp_c);
void        ui_screen_water_set_target(int target_c);
void        ui_screen_water_set_state(const char *state);
void        ui_screen_water_set_running(bool running);
void        ui_screen_water_set_toggle_cb(void (*cb)(bool start));

#endif /* UI_SCREEN_WATER_H */
