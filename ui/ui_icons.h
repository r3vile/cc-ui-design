#ifndef UI_ICONS_H
#define UI_ICONS_H

#include "lvgl.h"

/*
 * Icon system for the espresso machine UI.
 *
 * Icons are generated at runtime as 32×32 ALPHA_8BIT images using simple
 * geometric primitives. Call ui_icons_init() once before creating screens.
 *
 * For production, replace these with properly designed icons:
 *   1. Design 32×32 (or larger) icons in Figma / Illustrator
 *   2. Export as PNG with transparency
 *   3. Convert using https://lvgl.io/tools/imageconverter
 *      Format: CF_ALPHA_8BIT (for single-color icons)
 *   4. Replace the lv_img_dsc_t definitions in ui_icons.c
 */

#define UI_ICON_SIZE 32

void ui_icons_init(void);

const lv_img_dsc_t *ui_icon_coffee(void);
const lv_img_dsc_t *ui_icon_steam(void);
const lv_img_dsc_t *ui_icon_water(void);

#endif /* UI_ICONS_H */
