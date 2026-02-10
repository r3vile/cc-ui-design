#ifndef UI_H
#define UI_H

#include "lvgl.h"
#include <stdbool.h>

/* ── Screens ── */
typedef enum {
    UI_SCREEN_MAIN,
    UI_SCREEN_COFFEE,
    UI_SCREEN_STEAM,
    UI_SCREEN_WATER,
    UI_SCREEN_SETTINGS,
    UI_SCREEN_COUNT
} ui_screen_t;

/* ── Callbacks for application integration ── */
typedef void (*ui_brew_cb_t)(const char *coffee_type);
typedef void (*ui_water_toggle_cb_t)(bool start);

/* ── Initialization ── */
void ui_init(void);

/* ── Navigation ── */
void ui_load_screen(ui_screen_t screen);

/* ── Steam screen API ── */
void ui_steam_set_temp(int temp_c);
void ui_steam_set_target(int target_c);
void ui_steam_set_state(const char *state);

/* ── Hot water screen API ── */
void ui_water_set_temp(int temp_c);
void ui_water_set_target(int target_c);
void ui_water_set_state(const char *state);
void ui_water_set_running(bool running);

/* ── Coffee screen API ── */
void ui_coffee_set_brew_cb(ui_brew_cb_t cb);
const char *ui_coffee_get_selected(void);

/* ── Hot water screen callback ── */
void ui_water_set_toggle_cb(ui_water_toggle_cb_t cb);

#endif /* UI_H */
