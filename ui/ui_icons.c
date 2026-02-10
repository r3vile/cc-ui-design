#include "ui_icons.h"
#include <string.h>
#include <math.h>

/* ── Pixel buffers (32 × 32, 1 byte per pixel alpha) ── */
static uint8_t buf_coffee[UI_ICON_SIZE * UI_ICON_SIZE];
static uint8_t buf_steam[UI_ICON_SIZE * UI_ICON_SIZE];
static uint8_t buf_water[UI_ICON_SIZE * UI_ICON_SIZE];

/* ── Image descriptors ── */
static lv_img_dsc_t dsc_coffee;
static lv_img_dsc_t dsc_steam;
static lv_img_dsc_t dsc_water;

/* ── Drawing helpers ── */

static inline void px(uint8_t *buf, int x, int y, uint8_t a)
{
    if (x >= 0 && x < UI_ICON_SIZE && y >= 0 && y < UI_ICON_SIZE) {
        /* Blend: keep the brighter value (simple max compositing) */
        int idx = y * UI_ICON_SIZE + x;
        if (a > buf[idx]) buf[idx] = a;
    }
}

static void fill_circle(uint8_t *buf, int cx, int cy, int r, uint8_t a)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy <= r * r)
                px(buf, cx + dx, cy + dy, a);
        }
    }
}

static void fill_rect(uint8_t *buf, int x1, int y1, int x2, int y2, uint8_t a)
{
    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++)
            px(buf, x, y, a);
}

static void draw_ring(uint8_t *buf, int cx, int cy, int r_outer, int r_inner, uint8_t a)
{
    for (int dy = -r_outer; dy <= r_outer; dy++) {
        for (int dx = -r_outer; dx <= r_outer; dx++) {
            int d2 = dx * dx + dy * dy;
            if (d2 <= r_outer * r_outer && d2 >= r_inner * r_inner)
                px(buf, cx + dx, cy + dy, a);
        }
    }
}

static void draw_line_h(uint8_t *buf, int x1, int x2, int y, uint8_t a)
{
    for (int x = x1; x <= x2; x++)
        px(buf, x, y, a);
}

static void draw_line_v(uint8_t *buf, int x, int y1, int y2, uint8_t a)
{
    for (int y = y1; y <= y2; y++)
        px(buf, x, y, a);
}

static void init_dsc(lv_img_dsc_t *dsc, const uint8_t *data)
{
    dsc->header.always_zero = 0;
    dsc->header.w = UI_ICON_SIZE;
    dsc->header.h = UI_ICON_SIZE;
    dsc->header.cf = LV_IMG_CF_ALPHA_8BIT;
    dsc->data_size = UI_ICON_SIZE * UI_ICON_SIZE;
    dsc->data = data;
}

/* ── Icon: Coffee cup ──
 *
 *    ║  ║          steam lines
 *    ║  ║
 *  ┌────────┐
 *  │        │──╮    cup body + handle
 *  │        │──╯
 *  └────────┘
 */
static void draw_coffee(uint8_t *buf)
{
    memset(buf, 0, UI_ICON_SIZE * UI_ICON_SIZE);

    /* Cup body */
    fill_rect(buf, 5, 13, 21, 26, 0xE0);
    /* Rounded bottom corners */
    fill_circle(buf, 8, 24, 3, 0xE0);
    fill_circle(buf, 18, 24, 3, 0xE0);
    fill_rect(buf, 8, 24, 18, 27, 0xE0);
    /* Clean top edge */
    fill_rect(buf, 5, 13, 21, 14, 0xE0);

    /* Handle */
    draw_ring(buf, 22, 19, 5, 3, 0xC0);
    /* Erase left half of handle ring (inside cup) */
    fill_rect(buf, 5, 14, 21, 25, 0xE0);

    /* Steam lines */
    draw_line_v(buf, 10, 3, 10, 0x90);
    draw_line_v(buf, 11, 4, 9, 0x60);
    draw_line_v(buf, 16, 4, 11, 0x90);
    draw_line_v(buf, 17, 5, 10, 0x60);
}

/* ── Icon: Steam / vapor ──
 *
 *     ~  ~  ~      three wavy lines
 *     ~  ~  ~      rising up
 *     ~  ~  ~
 *    ──────────    base
 */
static void draw_steam(uint8_t *buf)
{
    memset(buf, 0, UI_ICON_SIZE * UI_ICON_SIZE);

    /* Base / surface */
    fill_rect(buf, 5, 25, 27, 27, 0xD0);

    /* Three wavy steam lines */
    int cols[] = {10, 16, 22};
    for (int c = 0; c < 3; c++) {
        int cx = cols[c];
        for (int y = 4; y <= 22; y++) {
            /* Sine wave offset for waviness */
            float phase = (float)(y - 4) * 0.45f + (float)c * 1.0f;
            int dx = (int)(sinf(phase) * 2.0f);
            uint8_t a = (uint8_t)(0xC0 - (22 - y) * 4);
            px(buf, cx + dx, y, a);
            px(buf, cx + dx + 1, y, a > 0x40 ? a - 0x40 : 0);
        }
    }
}

/* ── Icon: Water droplet ──
 *
 *       ▲
 *      / \       pointed top
 *     /   \
 *    (     )     circular bottom
 *     \   /
 *      \_/
 */
static void draw_water(uint8_t *buf)
{
    memset(buf, 0, UI_ICON_SIZE * UI_ICON_SIZE);

    int cx = 15, bot_cy = 20, r = 9;

    /* Bottom circle */
    fill_circle(buf, cx, bot_cy, r, 0xE0);

    /* Top triangle (from point down to circle) */
    int tip_y = 3;
    for (int y = tip_y; y <= bot_cy; y++) {
        float t = (float)(y - tip_y) / (float)(bot_cy - tip_y);
        int half_w = (int)(t * (float)r);
        for (int x = cx - half_w; x <= cx + half_w; x++) {
            px(buf, x, y, 0xE0);
        }
    }
}

/* ── Public API ── */

void ui_icons_init(void)
{
    draw_coffee(buf_coffee);
    draw_steam(buf_steam);
    draw_water(buf_water);

    init_dsc(&dsc_coffee, buf_coffee);
    init_dsc(&dsc_steam, buf_steam);
    init_dsc(&dsc_water, buf_water);
}

const lv_img_dsc_t *ui_icon_coffee(void) { return &dsc_coffee; }
const lv_img_dsc_t *ui_icon_steam(void)  { return &dsc_steam; }
const lv_img_dsc_t *ui_icon_water(void)  { return &dsc_water; }
