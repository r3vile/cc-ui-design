#ifndef UI_SCREEN_COFFEE_H
#define UI_SCREEN_COFFEE_H

#include "lvgl.h"
#include <stdbool.h>

void        ui_screen_coffee_create(void);
lv_obj_t   *ui_screen_coffee_get(void);

void        ui_screen_coffee_set_brew_cb(void (*cb)(const char *type));
const char *ui_screen_coffee_get_selected(void);

#endif /* UI_SCREEN_COFFEE_H */
