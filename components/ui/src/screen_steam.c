#include "screen_steam.h"
#include "ui_screens.h"
#include "ui_theme.h"
#include "ui_icons.h"

#define TARGET_TEMP   125
#define READY_THRESH  120

/* Keep references for dynamic updates */
static lv_obj_t *s_arc;
static lv_obj_t *s_temp_label;
static lv_obj_t *s_unit_label;
static lv_obj_t *s_state_label;
static lv_obj_t *s_target_label;

/* Demo timer for simulator */
static lv_timer_t *s_demo_timer;
static int s_demo_temp = 24;

static void back_cb(lv_event_t *e)
{
    (void)e;
    if (s_demo_timer) {
        lv_timer_delete(s_demo_timer);
        s_demo_timer = NULL;
    }
    s_demo_temp = 24;
    ui_screens_navigate(UI_SCREEN_MAIN);
}

static void demo_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    if (s_demo_temp < TARGET_TEMP) {
        s_demo_temp += 1;
    }
    const char *state = (s_demo_temp >= READY_THRESH) ? "Ready" : "Heating";
    screen_steam_set_temp(s_demo_temp, TARGET_TEMP, state);
}

void screen_steam_set_temp(int current_temp, int target_temp, const char *state)
{
    if (!s_arc) return;

    lv_arc_set_value(s_arc, current_temp);
    lv_label_set_text_fmt(s_temp_label, "%d", current_temp);
    lv_label_set_text(s_state_label, state);
    lv_label_set_text_fmt(s_target_label, "/ %d\u00B0C", target_temp);

    /* Green when near target */
    if (current_temp >= READY_THRESH) {
        lv_obj_set_style_arc_color(s_arc, UI_COLOR_SUCCESS, LV_PART_INDICATOR);
        lv_obj_set_style_text_color(s_state_label, UI_COLOR_SUCCESS, 0);
    } else {
        lv_obj_set_style_arc_color(s_arc, UI_COLOR_ACCENT, LV_PART_INDICATOR);
        lv_obj_set_style_text_color(s_state_label, UI_COLOR_TEXT_SEC, 0);
    }
}

lv_obj_t *screen_steam_create(void)
{
    lv_obj_t *scr;
    lv_obj_t *root = ui_create_screen_base(&scr);

    /*--- Arc gauge ---*/
    s_arc = lv_arc_create(root);
    lv_obj_set_size(s_arc, 420, 420);
    lv_obj_center(s_arc);

    lv_obj_remove_style(s_arc, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(s_arc, LV_OBJ_FLAG_CLICKABLE);

    lv_arc_set_rotation(s_arc, 150);
    lv_arc_set_bg_angles(s_arc, 0, 240);
    lv_arc_set_range(s_arc, 0, TARGET_TEMP);
    lv_arc_set_value(s_arc, 0);

    /* Background track */
    lv_obj_set_style_arc_color(s_arc, UI_COLOR_ARC_BG, LV_PART_MAIN);
    lv_obj_set_style_arc_width(s_arc, 10, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(s_arc, true, LV_PART_MAIN);

    /* Indicator */
    lv_obj_set_style_arc_color(s_arc, UI_COLOR_ACCENT, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(s_arc, 10, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(s_arc, true, LV_PART_INDICATOR);

    /*--- Temperature display ---*/
    s_temp_label = lv_label_create(root);
    lv_label_set_text(s_temp_label, "24");
    lv_obj_add_style(s_temp_label, &style_text_large, 0);
    lv_obj_align(s_temp_label, LV_ALIGN_CENTER, -10, -20);

    s_unit_label = lv_label_create(root);
    lv_label_set_text(s_unit_label, "\u00B0C");
    lv_obj_add_style(s_unit_label, &style_text_medium, 0);
    lv_obj_set_style_text_color(s_unit_label, UI_COLOR_TEXT_SEC, 0);
    lv_obj_align_to(s_unit_label, s_temp_label, LV_ALIGN_OUT_RIGHT_TOP, 4, 8);

    /*--- Target temp ---*/
    s_target_label = lv_label_create(root);
    lv_label_set_text_fmt(s_target_label, "/ %d\u00B0C", TARGET_TEMP);
    lv_obj_add_style(s_target_label, &style_text_secondary, 0);
    lv_obj_align(s_target_label, LV_ALIGN_CENTER, 0, 25);

    /*--- State label ---*/
    s_state_label = lv_label_create(root);
    lv_label_set_text(s_state_label, "Heating");
    lv_obj_add_style(s_state_label, &style_text_secondary, 0);
    lv_obj_align(s_state_label, LV_ALIGN_CENTER, 0, 55);

    /*--- Back button ---*/
    lv_obj_t *back_btn = lv_obj_create(root);
    lv_obj_set_size(back_btn, 56, 56);
    lv_obj_align(back_btn, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_add_style(back_btn, &style_nav_btn, 0);
    lv_obj_add_style(back_btn, &style_nav_btn_pressed, LV_STATE_PRESSED);
    lv_obj_add_flag(back_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(back_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(back_btn, back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *back_img = lv_image_create(back_btn);
    lv_image_set_src(back_img, &icon_back);
    lv_obj_center(back_img);
    lv_obj_set_style_image_recolor(back_img, UI_COLOR_TEXT, 0);
    lv_obj_set_style_image_recolor_opa(back_img, LV_OPA_COVER, 0);

    /* Start demo timer for simulator */
    s_demo_temp = 24;
    s_demo_timer = lv_timer_create(demo_timer_cb, 200, NULL);

    return scr;
}
