#include "screen_coffee.h"
#include "ui_screens.h"
#include "ui_theme.h"
#include "ui_icons.h"

typedef struct {
    const char       *name;
    lv_image_dsc_t   *icon;
} coffee_type_t;

static coffee_type_t coffee_types[] = {
    { "Espresso",    &icon_espresso },
    { "Cappuccino",  &icon_cappuccino },
    { "Americano",   &icon_americano },
    { "Latte",       &icon_latte },
};

#define TYPE_COUNT (sizeof(coffee_types) / sizeof(coffee_types[0]))

static int s_selected_idx = 0;

static void back_cb(lv_event_t *e)
{
    (void)e;
    ui_screens_navigate(UI_SCREEN_MAIN);
}

static void play_cb(lv_event_t *e)
{
    (void)e;
    /* Future: trigger brewing for coffee_types[s_selected_idx] */
}

static void scroll_cb(lv_event_t *e)
{
    lv_obj_t *carousel = lv_event_get_target(e);
    lv_coord_t scroll_x = lv_obj_get_scroll_x(carousel);
    /* Each card is ~200px wide with spacing, estimate index */
    int idx = (scroll_x + 100) / 200;
    if (idx < 0) idx = 0;
    if (idx >= (int)TYPE_COUNT) idx = (int)TYPE_COUNT - 1;
    s_selected_idx = idx;
}

lv_obj_t *screen_coffee_create(void)
{
    lv_obj_t *scr;
    lv_obj_t *root = ui_create_screen_base(&scr);

    /*--- Back button at top ---*/
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

    /*--- Carousel container ---*/
    lv_obj_t *carousel = lv_obj_create(root);
    lv_obj_set_size(carousel, 440, 240);
    lv_obj_align(carousel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(carousel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(carousel, LV_FLEX_ALIGN_START,
                           LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_snap_x(carousel, LV_SCROLL_SNAP_CENTER);
    lv_obj_add_flag(carousel, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_set_scrollbar_mode(carousel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(carousel, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(carousel, 0, 0);
    lv_obj_set_style_pad_column(carousel, 20, 0);
    /* Add left/right padding so first and last cards can center */
    lv_obj_set_style_pad_left(carousel, 120, 0);
    lv_obj_set_style_pad_right(carousel, 120, 0);

    lv_obj_add_event_cb(carousel, scroll_cb, LV_EVENT_SCROLL, NULL);

    /*--- Coffee type cards ---*/
    for (size_t i = 0; i < TYPE_COUNT; i++) {
        lv_obj_t *card = lv_obj_create(carousel);
        lv_obj_set_size(card, 180, 220);
        lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(card, LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_add_style(card, &style_card, 0);
        lv_obj_add_flag(card, LV_OBJ_FLAG_SNAPPABLE);
        lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

        /* Icon */
        lv_obj_t *img = lv_image_create(card);
        lv_image_set_src(img, coffee_types[i].icon);
        lv_obj_add_style(img, &style_icon_default, 0);

        /* Spacer */
        lv_obj_t *spacer = lv_obj_create(card);
        lv_obj_set_size(spacer, 1, 12);
        lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(spacer, 0, 0);

        /* Name label */
        lv_obj_t *lbl = lv_label_create(card);
        lv_label_set_text(lbl, coffee_types[i].name);
        lv_obj_add_style(lbl, &style_text_medium, 0);
    }

    /* Scroll to first card */
    lv_obj_update_snap(carousel, LV_ANIM_OFF);

    /*--- Play button at bottom ---*/
    lv_obj_t *play_btn = lv_obj_create(root);
    lv_obj_set_size(play_btn, 64, 64);
    lv_obj_set_style_radius(play_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(play_btn, UI_COLOR_ACCENT, 0);
    lv_obj_set_style_bg_opa(play_btn, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(play_btn, 0, 0);
    lv_obj_set_style_shadow_width(play_btn, 20, 0);
    lv_obj_set_style_shadow_color(play_btn, UI_COLOR_ACCENT_DIM, 0);
    lv_obj_set_style_shadow_opa(play_btn, LV_OPA_50, 0);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_add_flag(play_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(play_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(play_btn, play_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *play_img = lv_image_create(play_btn);
    lv_image_set_src(play_img, &icon_play);
    lv_obj_center(play_img);
    lv_obj_set_style_image_recolor(play_img, UI_COLOR_TEXT, 0);
    lv_obj_set_style_image_recolor_opa(play_img, LV_OPA_COVER, 0);

    return scr;
}
