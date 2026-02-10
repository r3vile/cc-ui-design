#include "ui_screen_main.h"
#include "../ui.h"
#include "../ui_theme.h"
#include "../ui_icons.h"

static lv_obj_t *screen;

/* ── Forward declarations ── */
static void on_coffee_click(lv_event_t *e);
static void on_steam_click(lv_event_t *e);
static void on_water_click(lv_event_t *e);
static void on_settings_click(lv_event_t *e);
static lv_obj_t *create_icon_btn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
                                  const lv_img_dsc_t *icon, const char *symbol,
                                  lv_event_cb_t click_cb);

/* ── Public ── */

void ui_screen_main_create(void)
{
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, UI_COLOR_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* ── Subtle decorative ring ── */
    lv_obj_t *ring = lv_obj_create(screen);
    lv_obj_remove_style_all(ring);
    lv_obj_set_size(ring, UI_MAIN_RING_DIAMETER, UI_MAIN_RING_DIAMETER);
    lv_obj_center(ring);
    lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(ring, 1, 0);
    lv_obj_set_style_border_color(ring, UI_COLOR_WHITE, 0);
    lv_obj_set_style_border_opa(ring, LV_OPA_COVER * 3 / 100, 0);
    lv_obj_set_style_bg_opa(ring, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(ring, LV_OBJ_FLAG_CLICKABLE);

    /* ── Center logo ── */
    lv_obj_t *logo = lv_label_create(screen);
    lv_label_set_text(logo, "ESPRESSO");
    lv_obj_set_style_text_font(logo, UI_FONT_LOGO, 0);
    lv_obj_set_style_text_color(logo, UI_COLOR_GOLD, 0);
    lv_obj_set_style_text_opa(logo, LV_OPA_COVER * 70 / 100, 0);
    lv_obj_set_style_text_letter_space(logo, 5, 0);
    lv_obj_center(logo);

    /* ── Icon buttons arranged in a circle ──
     *  Positions calculated from center (240,240) with orbit radius 148.
     *  Top, Right, Bottom, Left — like a compass.
     */
    lv_coord_t cx = UI_CENTER_X - UI_MAIN_ICON_BTN_SIZE / 2;
    lv_coord_t cy = UI_CENTER_Y - UI_MAIN_ICON_BTN_SIZE / 2;
    lv_coord_t r  = UI_MAIN_ORBIT_RADIUS;

    /* Coffee — top */
    create_icon_btn(screen, cx, cy - r,
                    ui_icon_coffee(), NULL, on_coffee_click);

    /* Steam — right */
    create_icon_btn(screen, cx + r, cy,
                    ui_icon_steam(), NULL, on_steam_click);

    /* Hot Water — bottom */
    create_icon_btn(screen, cx, cy + r,
                    ui_icon_water(), NULL, on_water_click);

    /* Settings — left (uses built-in symbol) */
    create_icon_btn(screen, cx - r, cy,
                    NULL, LV_SYMBOL_SETTINGS, on_settings_click);
}

lv_obj_t *ui_screen_main_get(void)
{
    return screen;
}

/* ── Helpers ── */

static lv_obj_t *create_icon_btn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
                                  const lv_img_dsc_t *icon, const char *symbol,
                                  lv_event_cb_t click_cb)
{
    lv_obj_t *btn = lv_obj_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, UI_MAIN_ICON_BTN_SIZE, UI_MAIN_ICON_BTN_SIZE);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);

    /* Glass-like background */
    lv_obj_set_style_bg_color(btn, UI_COLOR_WHITE, 0);
    lv_obj_set_style_bg_opa(btn, UI_OPA_BTN_BG, 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_color(btn, UI_COLOR_WHITE, 0);
    lv_obj_set_style_border_opa(btn, UI_OPA_BTN_BORDER, 0);

    /* Pressed state */
    lv_obj_set_style_bg_color(btn, UI_COLOR_GOLD, LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER * 15 / 100, LV_STATE_PRESSED);
    lv_obj_set_style_border_color(btn, UI_COLOR_GOLD, LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(btn, LV_OPA_COVER * 30 / 100, LV_STATE_PRESSED);

    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);

    /* Icon (image or symbol label) */
    if (icon) {
        lv_obj_t *img = lv_img_create(btn);
        lv_img_set_src(img, icon);
        lv_obj_center(img);
        lv_obj_set_style_img_recolor(img, UI_COLOR_WHITE, 0);
        lv_obj_set_style_img_recolor_opa(img, UI_OPA_ICON_NORMAL, 0);
        /* Gold tint on press */
        lv_obj_set_style_img_recolor(img, UI_COLOR_GOLD, LV_STATE_PRESSED);
        lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_STATE_PRESSED);
    } else if (symbol) {
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, symbol);
        lv_obj_set_style_text_font(lbl, UI_FONT_ICON_ACTION, 0);
        lv_obj_set_style_text_color(lbl, UI_COLOR_WHITE, 0);
        lv_obj_set_style_text_opa(lbl, UI_OPA_ICON_NORMAL, 0);
        lv_obj_center(lbl);
    }

    lv_obj_add_event_cb(btn, click_cb, LV_EVENT_CLICKED, NULL);

    return btn;
}

/* ── Navigation callbacks ── */

static void on_coffee_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_COFFEE);
}

static void on_steam_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_STEAM);
}

static void on_water_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_WATER);
}

static void on_settings_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_SETTINGS);
}
