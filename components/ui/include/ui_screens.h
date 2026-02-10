#ifndef UI_SCREENS_H
#define UI_SCREENS_H

#include "lvgl.h"

typedef enum {
    UI_SCREEN_MAIN,
    UI_SCREEN_COFFEE,
    UI_SCREEN_STEAM,
    UI_SCREEN_HOTWATER,
    UI_SCREEN_SETTINGS,
    UI_SCREEN_COUNT
} ui_screen_id_t;

/**
 * Initialize the screen manager.
 */
void ui_screens_init(void);

/**
 * Navigate to the specified screen with a fade transition.
 */
void ui_screens_navigate(ui_screen_id_t target);

/**
 * Create a screen base with circular clipping for the 480x480 round display.
 * Returns the clipped root container — add all children to this object.
 * The parent screen object is stored in *out_screen if not NULL.
 */
lv_obj_t *ui_create_screen_base(lv_obj_t **out_screen);

#endif /* UI_SCREENS_H */
