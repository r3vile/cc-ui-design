#include "ui_screens.h"
#include "ui_theme.h"
#include "screen_main.h"
#include "screen_coffee.h"
#include "screen_steam.h"
#include "screen_hotwater.h"
#include "screen_settings.h"

#define SCREEN_W 480
#define SCREEN_H 480
#define TRANSITION_MS 300

typedef lv_obj_t *(*screen_create_fn)(void);

static const screen_create_fn create_fns[UI_SCREEN_COUNT] = {
    [UI_SCREEN_MAIN]     = screen_main_create,
    [UI_SCREEN_COFFEE]   = screen_coffee_create,
    [UI_SCREEN_STEAM]    = screen_steam_create,
    [UI_SCREEN_HOTWATER] = screen_hotwater_create,
    [UI_SCREEN_SETTINGS] = screen_settings_create,
};

static ui_screen_id_t current_screen = UI_SCREEN_MAIN;

void ui_screens_init(void)
{
    current_screen = UI_SCREEN_MAIN;
}

void ui_screens_navigate(ui_screen_id_t target)
{
    if (target >= UI_SCREEN_COUNT) return;

    lv_obj_t *scr = create_fns[target]();
    if (!scr) return;

    /* Going to main = fade out, going to sub-screen = fade in */
    lv_screen_load_anim_t anim = (target == UI_SCREEN_MAIN)
        ? LV_SCR_LOAD_ANIM_FADE_OUT
        : LV_SCR_LOAD_ANIM_FADE_IN;

    lv_screen_load_anim(scr, anim, TRANSITION_MS, 0, true);
    current_screen = target;
}

lv_obj_t *ui_create_screen_base(lv_obj_t **out_screen)
{
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, SCREEN_W, SCREEN_H);
    lv_obj_set_style_bg_color(scr, UI_COLOR_BG, 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    if (out_screen) *out_screen = scr;

    /* Circular clipping container */
    lv_obj_t *root = lv_obj_create(scr);
    lv_obj_set_size(root, SCREEN_W, SCREEN_H);
    lv_obj_center(root);
    lv_obj_set_style_radius(root, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(root, true, 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(root, 0, 0);
    lv_obj_set_style_pad_all(root, 0, 0);
    lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);

    return root;
}
