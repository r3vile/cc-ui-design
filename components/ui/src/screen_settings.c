#include "screen_settings.h"
#include "ui_screens.h"
#include "ui_theme.h"
#include "ui_icons.h"

static void back_cb(lv_event_t *e)
{
    (void)e;
    ui_screens_navigate(UI_SCREEN_MAIN);
}

lv_obj_t *screen_settings_create(void)
{
    lv_obj_t *scr;
    lv_obj_t *root = ui_create_screen_base(&scr);

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

    /*--- Gear icon ---*/
    lv_obj_t *gear = lv_image_create(root);
    lv_image_set_src(gear, &icon_settings);
    lv_obj_align(gear, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_image_recolor(gear, UI_COLOR_TEXT_SEC, 0);
    lv_obj_set_style_image_recolor_opa(gear, LV_OPA_80, 0);

    /*--- Label ---*/
    lv_obj_t *lbl = lv_label_create(root);
    lv_label_set_text(lbl, "Settings");
    lv_obj_add_style(lbl, &style_text_medium, 0);
    lv_obj_set_style_text_color(lbl, UI_COLOR_TEXT_SEC, 0);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 40);

    return scr;
}
