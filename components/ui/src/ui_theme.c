#include "ui_theme.h"

/*--- Shared style objects ---*/
lv_style_t style_icon_default;
lv_style_t style_icon_pressed;
lv_style_t style_card;
lv_style_t style_text_large;
lv_style_t style_text_medium;
lv_style_t style_text_secondary;
lv_style_t style_nav_btn;
lv_style_t style_nav_btn_pressed;

static lv_style_transition_dsc_t icon_transition;
static lv_style_prop_t icon_trans_props[] = {
    LV_STYLE_TRANSFORM_SCALE_X,
    LV_STYLE_TRANSFORM_SCALE_Y,
    LV_STYLE_IMAGE_RECOLOR,
    0
};

static lv_style_transition_dsc_t btn_transition;
static lv_style_prop_t btn_trans_props[] = {
    LV_STYLE_BG_COLOR,
    LV_STYLE_TRANSFORM_SCALE_X,
    LV_STYLE_TRANSFORM_SCALE_Y,
    0
};

static void styles_init(void)
{
    /* Transition descriptors */
    lv_style_transition_dsc_init(&icon_transition, icon_trans_props,
                                  lv_anim_path_ease_out, 150, 0, NULL);
    lv_style_transition_dsc_init(&btn_transition, btn_trans_props,
                                  lv_anim_path_ease_out, 150, 0, NULL);

    /* Icon default state */
    lv_style_init(&style_icon_default);
    lv_style_set_image_recolor(&style_icon_default, UI_COLOR_ACCENT);
    lv_style_set_image_recolor_opa(&style_icon_default, LV_OPA_COVER);

    /* Icon pressed state */
    lv_style_init(&style_icon_pressed);
    lv_style_set_image_recolor(&style_icon_pressed, UI_COLOR_TEXT);
    lv_style_set_image_recolor_opa(&style_icon_pressed, LV_OPA_COVER);
    lv_style_set_transform_scale(&style_icon_pressed, 276); /* ~108% of 256 */
    lv_style_set_transition(&style_icon_pressed, &icon_transition);

    /* Card style (coffee carousel cards) */
    lv_style_init(&style_card);
    lv_style_set_bg_color(&style_card, UI_COLOR_SURFACE);
    lv_style_set_bg_opa(&style_card, LV_OPA_COVER);
    lv_style_set_radius(&style_card, 20);
    lv_style_set_pad_all(&style_card, 16);
    lv_style_set_border_width(&style_card, 0);

    /* Large text (temperature) */
    lv_style_init(&style_text_large);
    lv_style_set_text_font(&style_text_large, &lv_font_montserrat_48);
    lv_style_set_text_color(&style_text_large, UI_COLOR_TEXT);

    /* Medium text (labels, names) */
    lv_style_init(&style_text_medium);
    lv_style_set_text_font(&style_text_medium, &lv_font_montserrat_24);
    lv_style_set_text_color(&style_text_medium, UI_COLOR_TEXT);

    /* Secondary text (state, hints) */
    lv_style_init(&style_text_secondary);
    lv_style_set_text_font(&style_text_secondary, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_text_secondary, UI_COLOR_TEXT_SEC);

    /* Navigation button (back, play, stop) */
    lv_style_init(&style_nav_btn);
    lv_style_set_bg_opa(&style_nav_btn, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_nav_btn, 0);
    lv_style_set_shadow_width(&style_nav_btn, 0);
    lv_style_set_pad_all(&style_nav_btn, 12);
    lv_style_set_radius(&style_nav_btn, LV_RADIUS_CIRCLE);
    lv_style_set_image_recolor(&style_nav_btn, UI_COLOR_TEXT);
    lv_style_set_image_recolor_opa(&style_nav_btn, LV_OPA_COVER);

    /* Navigation button pressed */
    lv_style_init(&style_nav_btn_pressed);
    lv_style_set_bg_opa(&style_nav_btn_pressed, LV_OPA_20);
    lv_style_set_bg_color(&style_nav_btn_pressed, UI_COLOR_ACCENT);
    lv_style_set_transform_scale(&style_nav_btn_pressed, 270);
    lv_style_set_transition(&style_nav_btn_pressed, &btn_transition);
}

void ui_theme_init(lv_display_t *disp)
{
    /* Apply LVGL default dark theme */
    lv_theme_t *th = lv_theme_default_init(
        disp,
        UI_COLOR_ACCENT,
        lv_color_hex(0x7B5B3A),
        true,
        &lv_font_montserrat_14
    );
    lv_display_set_theme(disp, th);

    /* Initialize our shared styles */
    styles_init();
}
