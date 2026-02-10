#include "ui_screen_water.h"
#include "../ui.h"
#include "../ui_theme.h"

/* ── State ── */
static lv_obj_t *screen;
static lv_obj_t *arc;
static lv_obj_t *temp_label;
static lv_obj_t *unit_label;
static lv_obj_t *target_label;
static lv_obj_t *state_label;
static lv_obj_t *state_dot;
static lv_obj_t *action_btn;
static lv_obj_t *action_icon;

static int  current_temp = 0;
static int  target_temp  = 95;
static bool is_running   = false;
static void (*toggle_cb)(bool start) = NULL;

/* ── Forward declarations ── */
static void on_back_click(lv_event_t *e);
static void on_action_click(lv_event_t *e);
static void pulse_anim_cb(void *obj, int32_t v);
static void update_arc(void);
static void update_action_btn(void);

/* ── Public ── */

void ui_screen_water_create(void)
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

    /* ── Progress arc ── */
    arc = lv_arc_create(screen);
    lv_obj_set_size(arc, UI_ARC_DIAMETER, UI_ARC_DIAMETER);
    lv_obj_center(arc);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_range(arc, 0, target_temp);
    lv_arc_set_value(arc, current_temp);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    /* Track */
    lv_obj_set_style_arc_width(arc, UI_ARC_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, UI_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, UI_OPA_RING_TRACK, LV_PART_MAIN);

    /* Indicator */
    lv_obj_set_style_arc_width(arc, UI_ARC_WIDTH, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, UI_COLOR_BLUE, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_INDICATOR);

    /* Hide knob */
    lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_set_style_pad_all(arc, 0, LV_PART_KNOB);

    /* ── Temperature display ── */
    lv_obj_t *temp_row = lv_obj_create(screen);
    lv_obj_remove_style_all(temp_row);
    lv_obj_set_size(temp_row, 240, 48);
    lv_obj_align(temp_row, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_flex_flow(temp_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(temp_row, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(temp_row, 2, 0);
    lv_obj_clear_flag(temp_row, LV_OBJ_FLAG_SCROLLABLE);

    temp_label = lv_label_create(temp_row);
    lv_label_set_text(temp_label, "0");
    lv_obj_set_style_text_font(temp_label, UI_FONT_TEMP_LARGE, 0);
    lv_obj_set_style_text_color(temp_label, UI_COLOR_WHITE, 0);

    unit_label = lv_label_create(temp_row);
    lv_label_set_text(unit_label, "\u00B0C");
    lv_obj_set_style_text_font(unit_label, UI_FONT_TEMP_UNIT, 0);
    lv_obj_set_style_text_color(unit_label, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(unit_label, LV_OPA_COVER * 40 / 100, 0);

    /* ── Target label ── */
    target_label = lv_label_create(screen);
    lv_obj_set_style_text_font(target_label, UI_FONT_SMALL, 0);
    lv_obj_set_style_text_color(target_label, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(target_label, UI_OPA_TEXT_DIM, 0);
    lv_obj_set_style_text_letter_space(target_label, 2, 0);
    lv_obj_align(target_label, LV_ALIGN_CENTER, 0, 16);

    char buf[32];
    lv_snprintf(buf, sizeof(buf), "TARGET %d\u00B0C", target_temp);
    lv_label_set_text(target_label, buf);

    /* ── State indicator ── */
    lv_obj_t *state_row = lv_obj_create(screen);
    lv_obj_remove_style_all(state_row);
    lv_obj_set_size(state_row, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(state_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(state_row, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(state_row, 8, 0);
    lv_obj_align(state_row, LV_ALIGN_CENTER, 0, 40);
    lv_obj_clear_flag(state_row, LV_OBJ_FLAG_SCROLLABLE);

    state_dot = lv_obj_create(state_row);
    lv_obj_remove_style_all(state_dot);
    lv_obj_set_size(state_dot, 6, 6);
    lv_obj_set_style_radius(state_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(state_dot, UI_COLOR_BLUE, 0);
    lv_obj_set_style_bg_opa(state_dot, LV_OPA_COVER, 0);
    lv_obj_clear_flag(state_dot, LV_OBJ_FLAG_SCROLLABLE);

    state_label = lv_label_create(state_row);
    lv_label_set_text(state_label, "HEATING");
    lv_obj_set_style_text_font(state_label, UI_FONT_TINY, 0);
    lv_obj_set_style_text_color(state_label, UI_COLOR_BLUE, 0);
    lv_obj_set_style_text_letter_space(state_label, 3, 0);

    /* Pulsing dot animation */
    lv_anim_t pulse;
    lv_anim_init(&pulse);
    lv_anim_set_var(&pulse, state_dot);
    lv_anim_set_values(&pulse, LV_OPA_COVER, LV_OPA_COVER * 30 / 100);
    lv_anim_set_time(&pulse, UI_ANIM_PULSE_MS);
    lv_anim_set_playback_time(&pulse, UI_ANIM_PULSE_MS);
    lv_anim_set_repeat_count(&pulse, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&pulse, pulse_anim_cb);
    lv_anim_start(&pulse);

    /* ── Start / Stop button (bottom center) ── */
    action_btn = lv_obj_create(screen);
    lv_obj_remove_style_all(action_btn);
    lv_obj_set_size(action_btn, UI_ACTION_BTN_SIZE, UI_ACTION_BTN_SIZE);
    lv_obj_set_style_radius(action_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_shadow_width(action_btn, 20, 0);
    lv_obj_set_style_shadow_spread(action_btn, 0, 0);
    lv_obj_align(action_btn, LV_ALIGN_BOTTOM_MID, 0, -52);
    lv_obj_add_flag(action_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(action_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(action_btn, on_action_click, LV_EVENT_CLICKED, NULL);

    action_icon = lv_label_create(action_btn);
    lv_obj_set_style_text_font(action_icon, UI_FONT_ICON_ACTION, 0);
    lv_obj_set_style_text_color(action_icon, UI_COLOR_BG, 0);
    lv_obj_center(action_icon);

    update_action_btn();
}

lv_obj_t *ui_screen_water_get(void)
{
    return screen;
}

void ui_screen_water_set_temp(int temp_c)
{
    current_temp = temp_c;
    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%d", temp_c);
    lv_label_set_text(temp_label, buf);
    update_arc();
}

void ui_screen_water_set_target(int target_c)
{
    target_temp = target_c;
    lv_arc_set_range(arc, 0, target_c);

    char buf[32];
    lv_snprintf(buf, sizeof(buf), "TARGET %d\u00B0C", target_c);
    lv_label_set_text(target_label, buf);
    lv_obj_align(target_label, LV_ALIGN_CENTER, 0, 16);
    update_arc();
}

void ui_screen_water_set_state(const char *state)
{
    lv_label_set_text(state_label, state);
}

void ui_screen_water_set_running(bool running)
{
    is_running = running;
    update_action_btn();
}

void ui_screen_water_set_toggle_cb(void (*cb)(bool start))
{
    toggle_cb = cb;
}

/* ── Internal ── */

static void update_arc(void)
{
    lv_arc_set_value(arc, current_temp);
}

static void update_action_btn(void)
{
    if (is_running) {
        /* Stop state — red button with pause icon */
        lv_obj_set_style_bg_color(action_btn, UI_COLOR_RED, 0);
        lv_obj_set_style_bg_opa(action_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_shadow_color(action_btn, UI_COLOR_RED, 0);
        lv_obj_set_style_shadow_opa(action_btn, LV_OPA_COVER * 30 / 100, 0);
        lv_obj_set_style_bg_color(action_btn, UI_COLOR_RED_DARK, LV_STATE_PRESSED);
        lv_label_set_text(action_icon, LV_SYMBOL_PAUSE);
    } else {
        /* Start state — blue button with play icon */
        lv_obj_set_style_bg_color(action_btn, UI_COLOR_BLUE, 0);
        lv_obj_set_style_bg_opa(action_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_shadow_color(action_btn, UI_COLOR_BLUE, 0);
        lv_obj_set_style_shadow_opa(action_btn, LV_OPA_COVER * 30 / 100, 0);
        lv_obj_set_style_bg_color(action_btn, UI_COLOR_BLUE_DARK, LV_STATE_PRESSED);
        lv_label_set_text(action_icon, LV_SYMBOL_PLAY);
    }
    lv_obj_center(action_icon);
}

static void on_back_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_MAIN);
}

static void on_action_click(lv_event_t *e)
{
    (void)e;
    is_running = !is_running;
    update_action_btn();
    if (toggle_cb) {
        toggle_cb(is_running);
    }
}

static void pulse_anim_cb(void *obj, int32_t v)
{
    lv_obj_set_style_bg_opa((lv_obj_t *)obj, (lv_opa_t)v, 0);
}
