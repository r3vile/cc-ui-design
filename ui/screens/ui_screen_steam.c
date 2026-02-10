#include "ui_screen_steam.h"
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

static int current_temp  = 0;
static int target_temp   = 125;

/* ── Forward declarations ── */
static void on_back_click(lv_event_t *e);
static void pulse_anim_cb(void *obj, int32_t v);
static void update_arc(void);

/* ── Public ── */

void ui_screen_steam_create(void)
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

    lv_obj_t *back_icon = lv_label_create(back_btn);
    lv_label_set_text(back_icon, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(back_icon, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(back_icon, LV_OPA_COVER * 50 / 100, 0);
    lv_obj_center(back_icon);

    /* ── Progress arc ──
     *  A near-full-circle arc hugging the display edge.
     *  Start at 135° (bottom-left), span 270° clockwise.
     *  Range maps to 0 … target_temp °C.
     */
    arc = lv_arc_create(screen);
    lv_obj_set_size(arc, UI_ARC_DIAMETER, UI_ARC_DIAMETER);
    lv_obj_center(arc);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_range(arc, 0, target_temp);
    lv_arc_set_value(arc, current_temp);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    /* Track (background arc) */
    lv_obj_set_style_arc_width(arc, UI_ARC_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, UI_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, UI_OPA_RING_TRACK, LV_PART_MAIN);

    /* Indicator (progress arc) */
    lv_obj_set_style_arc_width(arc, UI_ARC_WIDTH, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, UI_COLOR_ORANGE, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_INDICATOR);

    /* Hide the knob */
    lv_obj_set_style_bg_opa(arc, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_set_style_pad_all(arc, 0, LV_PART_KNOB);

    /* ── Temperature display ── */
    lv_obj_t *temp_container = lv_obj_create(screen);
    lv_obj_remove_style_all(temp_container);
    lv_obj_set_size(temp_container, 240, 48);
    lv_obj_center(temp_container);
    lv_obj_set_flex_flow(temp_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(temp_container, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(temp_container, 2, 0);
    lv_obj_clear_flag(temp_container, LV_OBJ_FLAG_SCROLLABLE);

    temp_label = lv_label_create(temp_container);
    lv_label_set_text(temp_label, "0");
    lv_obj_set_style_text_font(temp_label, UI_FONT_TEMP_LARGE, 0);
    lv_obj_set_style_text_color(temp_label, UI_COLOR_WHITE, 0);

    unit_label = lv_label_create(temp_container);
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
    lv_obj_align(target_label, LV_ALIGN_CENTER, 0, 36);

    char buf[32];
    lv_snprintf(buf, sizeof(buf), "TARGET %d\u00B0C", target_temp);
    lv_label_set_text(target_label, buf);

    /* ── State indicator (dot + text) ── */
    lv_obj_t *state_row = lv_obj_create(screen);
    lv_obj_remove_style_all(state_row);
    lv_obj_set_size(state_row, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(state_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(state_row, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(state_row, 8, 0);
    lv_obj_align(state_row, LV_ALIGN_CENTER, 0, 60);
    lv_obj_clear_flag(state_row, LV_OBJ_FLAG_SCROLLABLE);

    state_dot = lv_obj_create(state_row);
    lv_obj_remove_style_all(state_dot);
    lv_obj_set_size(state_dot, 6, 6);
    lv_obj_set_style_radius(state_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(state_dot, UI_COLOR_ORANGE, 0);
    lv_obj_set_style_bg_opa(state_dot, LV_OPA_COVER, 0);
    lv_obj_clear_flag(state_dot, LV_OBJ_FLAG_SCROLLABLE);

    state_label = lv_label_create(state_row);
    lv_label_set_text(state_label, "HEATING");
    lv_obj_set_style_text_font(state_label, UI_FONT_TINY, 0);
    lv_obj_set_style_text_color(state_label, UI_COLOR_ORANGE, 0);
    lv_obj_set_style_text_letter_space(state_label, 3, 0);

    /* Pulsing animation on the state dot */
    lv_anim_t pulse;
    lv_anim_init(&pulse);
    lv_anim_set_var(&pulse, state_dot);
    lv_anim_set_values(&pulse, LV_OPA_COVER, LV_OPA_COVER * 30 / 100);
    lv_anim_set_time(&pulse, UI_ANIM_PULSE_MS);
    lv_anim_set_playback_time(&pulse, UI_ANIM_PULSE_MS);
    lv_anim_set_repeat_count(&pulse, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&pulse, pulse_anim_cb);
    lv_anim_start(&pulse);

    /* ── Valve hint (bottom) ── */
    lv_obj_t *hint = lv_label_create(screen);
    lv_label_set_text(hint, "OPEN VALVE WHEN READY");
    lv_obj_set_style_text_font(hint, UI_FONT_TINY, 0);
    lv_obj_set_style_text_color(hint, UI_COLOR_WHITE, 0);
    lv_obj_set_style_text_opa(hint, UI_OPA_TEXT_MUTED, 0);
    lv_obj_set_style_text_letter_space(hint, 2, 0);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -62);
}

lv_obj_t *ui_screen_steam_get(void)
{
    return screen;
}

void ui_screen_steam_set_temp(int temp_c)
{
    current_temp = temp_c;
    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%d", temp_c);
    lv_label_set_text(temp_label, buf);
    update_arc();
}

void ui_screen_steam_set_target(int target_c)
{
    target_temp = target_c;
    lv_arc_set_range(arc, 0, target_c);

    char buf[32];
    lv_snprintf(buf, sizeof(buf), "TARGET %d\u00B0C", target_c);
    lv_label_set_text(target_label, buf);
    lv_obj_align(target_label, LV_ALIGN_CENTER, 0, 36);
    update_arc();
}

void ui_screen_steam_set_state(const char *state)
{
    lv_label_set_text(state_label, state);
}

/* ── Internal ── */

static void update_arc(void)
{
    lv_arc_set_value(arc, current_temp);
}

static void on_back_click(lv_event_t *e)
{
    (void)e;
    ui_load_screen(UI_SCREEN_MAIN);
}

static void pulse_anim_cb(void *obj, int32_t v)
{
    lv_obj_set_style_bg_opa((lv_obj_t *)obj, (lv_opa_t)v, 0);
}
