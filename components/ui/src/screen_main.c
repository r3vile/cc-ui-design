#include "screen_main.h"
#include "ui_screens.h"
#include "ui_theme.h"
#include "ui_icons.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define CENTER    240
#define ICON_R    120   /* radius of icon circle */
#define ICON_SIZE  64

typedef struct {
    lv_image_dsc_t *icon;
    ui_screen_id_t  target;
    int             angle_deg; /* 0 = right (3 o'clock), 90 = down, etc. */
} menu_item_t;

static const menu_item_t items[] = {
    { &icon_coffee,   UI_SCREEN_COFFEE,   270 },  /* top */
    { &icon_steam,    UI_SCREEN_STEAM,      0 },  /* right */
    { &icon_hotwater, UI_SCREEN_HOTWATER,  90 },  /* bottom */
    { &icon_settings, UI_SCREEN_SETTINGS, 180 },  /* left */
};

#define ITEM_COUNT (sizeof(items) / sizeof(items[0]))

static void icon_clicked_cb(lv_event_t *e)
{
    uintptr_t idx = (uintptr_t)lv_event_get_user_data(e);
    if (idx < ITEM_COUNT) {
        ui_screens_navigate(items[idx].target);
    }
}

lv_obj_t *screen_main_create(void)
{
    lv_obj_t *scr;
    lv_obj_t *root = ui_create_screen_base(&scr);

    for (size_t i = 0; i < ITEM_COUNT; i++) {
        float angle_rad = (float)items[i].angle_deg * (float)M_PI / 180.0f;
        int x = CENTER + (int)(ICON_R * cosf(angle_rad)) - ICON_SIZE / 2;
        int y = CENTER + (int)(ICON_R * sinf(angle_rad)) - ICON_SIZE / 2;

        /* Clickable container for larger hit area */
        lv_obj_t *btn = lv_obj_create(root);
        lv_obj_set_size(btn, ICON_SIZE + 24, ICON_SIZE + 24);
        lv_obj_set_pos(btn, x - 12, y - 12);
        lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_color(btn, UI_COLOR_ACCENT_DIM, LV_STATE_PRESSED);
        lv_obj_set_style_bg_opa(btn, LV_OPA_30, LV_STATE_PRESSED);
        lv_obj_set_style_border_width(btn, 0, 0);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_pad_all(btn, 0, 0);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(btn, icon_clicked_cb, LV_EVENT_CLICKED,
                            (void *)(uintptr_t)i);

        /* Icon image */
        lv_obj_t *img = lv_image_create(btn);
        lv_image_set_src(img, items[i].icon);
        lv_obj_center(img);
        lv_obj_add_style(img, &style_icon_default, 0);
        lv_obj_add_style(img, &style_icon_pressed, LV_STATE_PRESSED);
    }

    return scr;
}
