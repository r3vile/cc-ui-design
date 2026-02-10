#include "ui_screen_settings.h"
#include "../ui.h"
#include "../ui_theme.h"

static lv_obj_t *screen;

static void on_back_click(lv_event_t *e);

/* ── Public ── */

void ui_screen_settings_create(void)
{
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, UI_COLOR_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* ── Back button (top center) ── */
    lv_obj_t *back_btn = lv_obj_create(screen);
    lv_obj_remove_style_all(back_btn);
    lv_obj_set_size(back_btn, UI_NAV_BTN_SIZE, UI_NAV_BTN_SIZE);
    lv_obj_set_style_radius(back_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(back_btn, UI_COLOR_WHITE, 0);
    lv_obj_set_style_bg_opa(back_btn, UI_OPA_BTN_BG, 0);
    lv_obj_set_style_border_width(back_btn, 1, 0);
    lv_obj_set_style_border_color(back_btn, UI_COLOR_WHITE, 0);
    lv_obj_set_style_border_opa(back_btn, UI_OPA_BTN_BORDER, 0);
    lv_obj_set_style_bg_opa(back_btn, LV_OPA_COVER * 10 / 100, LV_STATE_PRESSED);
    lv_obj_align(back_btn, LV_ALIGN_TOP_MID, 0, UI_NAV_BTN_Y);
    lv_obj_add_flag(back_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(back_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(back_btn, on_back_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t *back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(back_lbl, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(back_lbl, LV_OPA_COVER * 50 / 100, 0);
    lv_obj_center(back_lbl);

    /* ── Placeholder content ── */
    lv_obj_t *container = lv_obj_create(screen);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(container);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(container, 16, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    /* Gear icon */
    lv_obj_t *gear = lv_label_create(container);
    lv_label_set_text(gear, LV_SYMBOL_SETTINGS);
    lv_obj_set_style_text_font(gear, UI_FONT_TEMP_LARGE, 0);
    lv_obj_set_style_text_color(gear, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(gear, UI_OPA_HINT, 0);

    /* Title */
    lv_obj_t *title = lv_label_create(container);
    lv_label_set_text(title, "SETTINGS");
    lv_obj_set_style_text_font(title, UI_FONT_SMALL, 0);
    lv_obj_set_style_text_color(title, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(title, LV_OPA_COVER * 20 / 100, 0);
    lv_obj_set_style_text_letter_space(title, 3, 0);

    /* Subtitle */
    lv_obj_t *sub = lv_label_create(container);
    lv_label_set_text(sub, "Coming soon");
    lv_obj_set_style_text_font(sub, UI_FONT_TINY, 0);
    lv_obj_set_style_text_color(sub, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(sub, LV_OPA_COVER * 10 / 100, 0);
    lv_obj_set_style_text_letter_space(sub, 1, 0);
}

lv_obj_t *ui_screen_settings_get(void)
{
    return screen;
}

/* ── Internal ── */

static void on_back_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_MAIN);
}
