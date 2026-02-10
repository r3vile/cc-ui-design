#include "ui.h"
#include "ui_theme.h"
#include "ui_icons.h"
#include "screens/ui_screen_main.h"
#include "screens/ui_screen_coffee.h"
#include "screens/ui_screen_steam.h"
#include "screens/ui_screen_water.h"
#include "screens/ui_screen_settings.h"

/* ── Screen lookup ── */
typedef lv_obj_t *(*screen_getter_t)(void);

static const screen_getter_t screen_getters[UI_SCREEN_COUNT] = {
    [UI_SCREEN_MAIN]     = ui_screen_main_get,
    [UI_SCREEN_COFFEE]   = ui_screen_coffee_get,
    [UI_SCREEN_STEAM]    = ui_screen_steam_get,
    [UI_SCREEN_WATER]    = ui_screen_water_get,
    [UI_SCREEN_SETTINGS] = ui_screen_settings_get,
};

static ui_screen_t current_screen = UI_SCREEN_MAIN;

/* ── Initialization ── */

void ui_init(void)
{
    /* Generate programmatic icons */
    ui_icons_init();

    /* Create all screens (they remain in memory for fast switching) */
    ui_screen_main_create();
    ui_screen_coffee_create();
    ui_screen_steam_create();
    ui_screen_water_create();
    ui_screen_settings_create();

    /* Show main screen without animation */
    lv_disp_load_scr(ui_screen_main_get());
    current_screen = UI_SCREEN_MAIN;
}

/* ── Navigation ── */

void ui_load_screen(ui_screen_t screen)
{
    if (screen >= UI_SCREEN_COUNT) return;
    if (screen == current_screen) return;

    lv_obj_t *target = screen_getters[screen]();
    if (!target) return;

    /*
     * Choose animation direction based on navigation hierarchy:
     *   Main → sub-screen  = slide left (move over)
     *   sub-screen → Main  = slide right (move back)
     */
    lv_scr_load_anim_t anim;
    if (screen == UI_SCREEN_MAIN) {
        anim = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
    } else if (current_screen == UI_SCREEN_MAIN) {
        anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;
    } else {
        anim = LV_SCR_LOAD_ANIM_FADE_ON;
    }

    lv_scr_load_anim(target, anim, UI_ANIM_SCREEN_MS, 0, false);
    current_screen = screen;
}

/* ── Steam screen pass-through ── */

void ui_steam_set_temp(int temp_c)
{
    ui_screen_steam_set_temp(temp_c);
}

void ui_steam_set_target(int target_c)
{
    ui_screen_steam_set_target(target_c);
}

void ui_steam_set_state(const char *state)
{
    ui_screen_steam_set_state(state);
}

/* ── Hot water screen pass-through ── */

void ui_water_set_temp(int temp_c)
{
    ui_screen_water_set_temp(temp_c);
}

void ui_water_set_target(int target_c)
{
    ui_screen_water_set_target(target_c);
}

void ui_water_set_state(const char *state)
{
    ui_screen_water_set_state(state);
}

void ui_water_set_running(bool running)
{
    ui_screen_water_set_running(running);
}

/* ── Coffee screen pass-through ── */

void ui_coffee_set_brew_cb(ui_brew_cb_t cb)
{
    ui_screen_coffee_set_brew_cb(cb);
}

const char *ui_coffee_get_selected(void)
{
    return ui_screen_coffee_get_selected();
}

/* ── Water screen callback pass-through ── */

void ui_water_set_toggle_cb(ui_water_toggle_cb_t cb)
{
    ui_screen_water_set_toggle_cb(cb);
}
