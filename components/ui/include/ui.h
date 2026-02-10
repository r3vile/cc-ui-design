#ifndef UI_H
#define UI_H

#include "lvgl.h"

/**
 * Initialize the espresso machine UI.
 * Call after LVGL and display are initialized.
 */
void ui_init(lv_display_t *disp);

#endif /* UI_H */
