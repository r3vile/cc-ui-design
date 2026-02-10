#ifndef UI_ICONS_H
#define UI_ICONS_H

#include "lvgl.h"

/* Main menu icons (64x64) */
extern lv_image_dsc_t icon_coffee;
extern lv_image_dsc_t icon_steam;
extern lv_image_dsc_t icon_hotwater;
extern lv_image_dsc_t icon_settings;

/* Navigation icons (48x48) */
extern lv_image_dsc_t icon_back;
extern lv_image_dsc_t icon_play;
extern lv_image_dsc_t icon_stop;

/* Coffee type icons (64x64) */
extern lv_image_dsc_t icon_espresso;
extern lv_image_dsc_t icon_cappuccino;
extern lv_image_dsc_t icon_americano;
extern lv_image_dsc_t icon_latte;

/**
 * Generate all icons. Call once at startup before creating any screens.
 */
void ui_icons_init(void);

#endif /* UI_ICONS_H */
