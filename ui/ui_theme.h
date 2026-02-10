#ifndef UI_THEME_H
#define UI_THEME_H

#include "lvgl.h"

/* ── Display ── */
#define UI_HOR_RES              480
#define UI_VER_RES              480
#define UI_CENTER_X             (UI_HOR_RES / 2)
#define UI_CENTER_Y             (UI_VER_RES / 2)

/* ── Color palette ── */
#define UI_COLOR_BG             lv_color_hex(0x0A0A0A)
#define UI_COLOR_BG_LIGHT       lv_color_hex(0x151515)
#define UI_COLOR_GOLD           lv_color_hex(0xD4A054)
#define UI_COLOR_GOLD_DARK      lv_color_hex(0xB8863E)
#define UI_COLOR_ORANGE         lv_color_hex(0xE8934A)
#define UI_COLOR_BLUE           lv_color_hex(0x5BA0D4)
#define UI_COLOR_BLUE_LIGHT     lv_color_hex(0x7BBCE8)
#define UI_COLOR_BLUE_DARK      lv_color_hex(0x4080B8)
#define UI_COLOR_RED            lv_color_hex(0xD45B5B)
#define UI_COLOR_RED_DARK       lv_color_hex(0xB84040)
#define UI_COLOR_WHITE          lv_color_hex(0xFFFFFF)

/* ── Opacity presets ── */
#define UI_OPA_ICON_NORMAL      (LV_OPA_COVER * 55 / 100)
#define UI_OPA_BTN_BG           (LV_OPA_COVER * 4 / 100)
#define UI_OPA_BTN_BORDER       (LV_OPA_COVER * 8 / 100)
#define UI_OPA_RING_TRACK       (LV_OPA_COVER * 5 / 100)
#define UI_OPA_TEXT_PRIMARY     LV_OPA_COVER
#define UI_OPA_TEXT_DIM         (LV_OPA_COVER * 30 / 100)
#define UI_OPA_TEXT_MUTED       (LV_OPA_COVER * 18 / 100)
#define UI_OPA_HINT             (LV_OPA_COVER * 12 / 100)

/* ── Layout sizes ── */
#define UI_MAIN_ICON_BTN_SIZE   72
#define UI_MAIN_ORBIT_RADIUS    148
#define UI_MAIN_RING_DIAMETER   320
#define UI_NAV_BTN_SIZE         44
#define UI_ACTION_BTN_SIZE      56
#define UI_COFFEE_ICON_SIZE     96
#define UI_ARC_DIAMETER         420
#define UI_ARC_WIDTH            6
#define UI_DOT_SIZE             6
#define UI_DOT_ACTIVE_WIDTH     18

/* ── Vertical positions (from top of 480px screen) ── */
#define UI_NAV_BTN_Y            48
#define UI_ACTION_BTN_Y         (UI_VER_RES - 52 - UI_ACTION_BTN_SIZE)
#define UI_HINT_Y               (UI_VER_RES - 68)

/* ── Fonts ─────────────────────────────────────────────
 *  Adjust these to match your lv_conf.h font settings.
 *  For the premium look, use a custom light/thin weight
 *  font for the temperature display (e.g. Inter Light).
 *  Convert with: https://lvgl.io/tools/fontconverter
 * ──────────────────────────────────────────────────── */
#define UI_FONT_TEMP_LARGE      &lv_font_montserrat_48
#define UI_FONT_TEMP_UNIT       &lv_font_montserrat_20
#define UI_FONT_TITLE           &lv_font_montserrat_18
#define UI_FONT_BODY            &lv_font_montserrat_14
#define UI_FONT_SMALL           &lv_font_montserrat_12
#define UI_FONT_TINY            &lv_font_montserrat_10
#define UI_FONT_ICON_NAV        &lv_font_montserrat_18
#define UI_FONT_ICON_ACTION     &lv_font_montserrat_22
#define UI_FONT_LOGO            &lv_font_montserrat_12

/* ── Animation timing (ms) ── */
#define UI_ANIM_SCREEN_MS       350
#define UI_ANIM_TEMP_MS         800
#define UI_ANIM_PULSE_MS        1500

#endif /* UI_THEME_H */
