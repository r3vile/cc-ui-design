#include "ui_screen_coffee.h"
#include "../ui.h"
#include "../ui_theme.h"
#include "../ui_icons.h"

/* ── Coffee types ── */
typedef struct {
    const char *name;
} coffee_entry_t;

static const coffee_entry_t coffee_types[] = {
    { "Espresso" },
    { "Doppio" },
    { "Americano" },
    { "Cappuccino" },
    { "Flat White" },
    { "Latte" },
};
#define COFFEE_TYPE_COUNT (sizeof(coffee_types) / sizeof(coffee_types[0]))

/* ── State ── */
static lv_obj_t *screen;
static lv_obj_t *icon_container;
static lv_obj_t *name_label;
static lv_obj_t *dots_container;
static int       current_index = 0;
static void    (*brew_cb)(const char *type) = NULL;

/* ── Forward declarations ── */
static void update_display(void);
static void on_back_click(lv_event_t *e);
static void on_play_click(lv_event_t *e);
static void on_gesture(lv_event_t *e);

/* ── Public ── */

void ui_screen_coffee_create(void)
{
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, UI_COLOR_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /* Detect swipe gestures */
    lv_obj_add_event_cb(screen, on_gesture, LV_EVENT_GESTURE, NULL);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_GESTURE_BUBBLE);

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

    lv_obj_t *back_icon = lv_label_create(back_btn);
    lv_label_set_text(back_icon, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(back_icon, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(back_icon, LV_OPA_COVER * 50 / 100, 0);
    lv_obj_center(back_icon);

    /* ── Coffee icon circle (centered, offset upward) ── */
    icon_container = lv_obj_create(screen);
    lv_obj_remove_style_all(icon_container);
    lv_obj_set_size(icon_container, UI_COFFEE_ICON_SIZE, UI_COFFEE_ICON_SIZE);
    lv_obj_set_style_radius(icon_container, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(icon_container, UI_COLOR_GOLD, 0);
    lv_obj_set_style_bg_opa(icon_container, LV_OPA_COVER * 6 / 100, 0);
    lv_obj_set_style_border_width(icon_container, 1, 0);
    lv_obj_set_style_border_color(icon_container, UI_COLOR_GOLD, 0);
    lv_obj_set_style_border_opa(icon_container, LV_OPA_COVER * 12 / 100, 0);
    lv_obj_align(icon_container, LV_ALIGN_CENTER, 0, -50);
    lv_obj_clear_flag(icon_container, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon_img = lv_img_create(icon_container);
    lv_img_set_src(icon_img, ui_icon_coffee());
    lv_obj_center(icon_img);
    lv_obj_set_style_img_recolor(icon_img, UI_COLOR_GOLD, 0);
    lv_obj_set_style_img_recolor_opa(icon_img, LV_OPA_COVER, 0);

    /* ── Coffee type name ── */
    name_label = lv_label_create(screen);
    lv_obj_set_style_text_font(name_label, UI_FONT_TITLE, 0);
    lv_obj_set_style_text_color(name_label, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(name_label, LV_OPA_COVER * 85 / 100, 0);
    lv_obj_set_style_text_letter_space(name_label, 3, 0);
    lv_obj_align(name_label, LV_ALIGN_CENTER, 0, 22);

    /* ── Pagination dots ── */
    dots_container = lv_obj_create(screen);
    lv_obj_remove_style_all(dots_container);
    lv_obj_set_size(dots_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_column(dots_container, 8, 0);
    lv_obj_set_flex_flow(dots_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(dots_container, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(dots_container, LV_ALIGN_CENTER, 0, 50);
    lv_obj_clear_flag(dots_container, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < (int)COFFEE_TYPE_COUNT; i++) {
        lv_obj_t *dot = lv_obj_create(dots_container);
        lv_obj_remove_style_all(dot);
        lv_obj_set_size(dot, UI_DOT_SIZE, UI_DOT_SIZE);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dot, UI_COLOR_WHITE, 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER * 15 / 100, 0);
        lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    }

    /* ── Swipe hint ── */
    lv_obj_t *hint = lv_label_create(screen);
    lv_label_set_text(hint, "SWIPE TO CHANGE");
    lv_obj_set_style_text_font(hint, UI_FONT_TINY, 0);
    lv_obj_set_style_text_color(hint, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(hint, LV_OPA_COVER * 20 / 100, 0);
    lv_obj_set_style_text_letter_space(hint, 2, 0);
    lv_obj_align(hint, LV_ALIGN_CENTER, 0, 70);

    /* ── Play / Brew button (bottom center) ── */
    lv_obj_t *play_btn = lv_obj_create(screen);
    lv_obj_remove_style_all(play_btn);
    lv_obj_set_size(play_btn, UI_ACTION_BTN_SIZE, UI_ACTION_BTN_SIZE);
    lv_obj_set_style_radius(play_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(play_btn, UI_COLOR_GOLD, 0);
    lv_obj_set_style_bg_opa(play_btn, LV_OPA_COVER, 0);
    lv_obj_set_style_shadow_width(play_btn, 20, 0);
    lv_obj_set_style_shadow_color(play_btn, UI_COLOR_GOLD, 0);
    lv_obj_set_style_shadow_opa(play_btn, LV_OPA_COVER * 30 / 100, 0);
    lv_obj_set_style_shadow_spread(play_btn, 0, 0);
    /* Pressed state */
    lv_obj_set_style_bg_color(play_btn, UI_COLOR_GOLD_DARK, LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(play_btn, LV_OPA_COVER * 50 / 100, LV_STATE_PRESSED);

    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, -(int)52);
    lv_obj_add_flag(play_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(play_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(play_btn, on_play_click, LV_EVENT_CLICKED, NULL);

    lv_obj_t *play_icon = lv_label_create(play_btn);
    lv_label_set_text(play_icon, LV_SYMBOL_PLAY);
    lv_obj_set_style_text_font(play_icon, UI_FONT_ICON_ACTION, 0);
    lv_obj_set_style_text_color(play_icon, UI_COLOR_BG, 0);
    lv_obj_center(play_icon);

    /* Initial display */
    update_display();
}

lv_obj_t *ui_screen_coffee_get(void)
{
    return screen;
}

void ui_screen_coffee_set_brew_cb(void (*cb)(const char *type))
{
    brew_cb = cb;
}

const char *ui_screen_coffee_get_selected(void)
{
    return coffee_types[current_index].name;
}

/* ── Internal ── */

static void update_display(void)
{
    /* Update name */
    lv_label_set_text(name_label, coffee_types[current_index].name);
    lv_obj_align(name_label, LV_ALIGN_CENTER, 0, 22);

    /* Update dots */
    uint32_t cnt = lv_obj_get_child_cnt(dots_container);
    for (uint32_t i = 0; i < cnt; i++) {
        lv_obj_t *dot = lv_obj_get_child(dots_container, (int32_t)i);
        if ((int)i == current_index) {
            lv_obj_set_size(dot, UI_DOT_ACTIVE_WIDTH, UI_DOT_SIZE);
            lv_obj_set_style_radius(dot, UI_DOT_SIZE / 2, 0);
            lv_obj_set_style_bg_color(dot, UI_COLOR_GOLD, 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        } else {
            lv_obj_set_size(dot, UI_DOT_SIZE, UI_DOT_SIZE);
            lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_color(dot, UI_COLOR_WHITE, 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_COVER * 15 / 100, 0);
        }
    }
}

static void on_back_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_MAIN);
}

static void on_play_click(lv_event_t *e)
{
    (void)e;
    if (brew_cb) {
        brew_cb(coffee_types[current_index].name);
    }
}

static void on_gesture(lv_event_t *e)
{
    (void)e;
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

    if (dir == LV_DIR_LEFT) {
        current_index = (current_index + 1) % (int)COFFEE_TYPE_COUNT;
        update_display();
    } else if (dir == LV_DIR_RIGHT) {
        current_index = (current_index - 1 + (int)COFFEE_TYPE_COUNT) % (int)COFFEE_TYPE_COUNT;
        update_display();
    }
}
