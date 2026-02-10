#ifndef UI_THEME_H
#define UI_THEME_H

#include "lvgl.h"

/*--- Color palette ---*/
#define UI_COLOR_BG          lv_color_hex(0x1A1A1A)
#define UI_COLOR_SURFACE     lv_color_hex(0x2A2A2A)
#define UI_COLOR_ACCENT      lv_color_hex(0xD4893F)
#define UI_COLOR_ACCENT_DIM  lv_color_hex(0x8B5A2B)
#define UI_COLOR_TEXT        lv_color_hex(0xF0E6D8)
#define UI_COLOR_TEXT_SEC    lv_color_hex(0x9A9A9A)
#define UI_COLOR_SUCCESS     lv_color_hex(0x4CAF50)
#define UI_COLOR_DANGER      lv_color_hex(0xC62828)
#define UI_COLOR_ARC_BG      lv_color_hex(0x3A3A3A)

/*--- Shared styles ---*/
extern lv_style_t style_icon_default;
extern lv_style_t style_icon_pressed;
extern lv_style_t style_card;
extern lv_style_t style_text_large;
extern lv_style_t style_text_medium;
extern lv_style_t style_text_secondary;
extern lv_style_t style_nav_btn;
extern lv_style_t style_nav_btn_pressed;

/**
 * Initialize the custom dark theme and shared styles.
 */
void ui_theme_init(lv_display_t *disp);

#endif /* UI_THEME_H */
