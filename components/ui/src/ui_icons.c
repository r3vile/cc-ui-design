#include "ui_icons.h"
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*--- Icon buffer sizes ---*/
#define ICON_LARGE  64
#define ICON_SMALL  48

/*--- Static buffers (retained in memory for LVGL image sources) ---*/
static uint8_t buf_coffee[ICON_LARGE * ICON_LARGE];
static uint8_t buf_steam[ICON_LARGE * ICON_LARGE];
static uint8_t buf_hotwater[ICON_LARGE * ICON_LARGE];
static uint8_t buf_settings[ICON_LARGE * ICON_LARGE];
static uint8_t buf_back[ICON_SMALL * ICON_SMALL];
static uint8_t buf_play[ICON_SMALL * ICON_SMALL];
static uint8_t buf_stop[ICON_SMALL * ICON_SMALL];
static uint8_t buf_espresso[ICON_LARGE * ICON_LARGE];
static uint8_t buf_cappuccino[ICON_LARGE * ICON_LARGE];
static uint8_t buf_americano[ICON_LARGE * ICON_LARGE];
static uint8_t buf_latte[ICON_LARGE * ICON_LARGE];

/*--- Image descriptors ---*/
lv_image_dsc_t icon_coffee;
lv_image_dsc_t icon_steam;
lv_image_dsc_t icon_hotwater;
lv_image_dsc_t icon_settings;
lv_image_dsc_t icon_back;
lv_image_dsc_t icon_play;
lv_image_dsc_t icon_stop;
lv_image_dsc_t icon_espresso;
lv_image_dsc_t icon_cappuccino;
lv_image_dsc_t icon_americano;
lv_image_dsc_t icon_latte;

/*=======================================================
 * Drawing helpers — all operate on A8 buffers
 *=======================================================*/

static inline void px(uint8_t *buf, int stride, int x, int y, uint8_t a)
{
    if (x >= 0 && x < stride && y >= 0 && y < stride) {
        /* Blend: max of existing and new alpha */
        uint8_t cur = buf[y * stride + x];
        if (a > cur) buf[y * stride + x] = a;
    }
}

/* Anti-aliased pixel with sub-pixel coverage */
static inline void px_aa(uint8_t *buf, int stride, int x, int y, float coverage)
{
    if (coverage <= 0.0f) return;
    if (coverage > 1.0f) coverage = 1.0f;
    px(buf, stride, x, y, (uint8_t)(coverage * 255.0f));
}

/* Filled circle */
static void draw_filled_circle(uint8_t *buf, int stride, float cx, float cy, float r)
{
    int x0 = (int)(cx - r - 1);
    int x1 = (int)(cx + r + 2);
    int y0 = (int)(cy - r - 1);
    int y1 = (int)(cy + r + 2);
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            float dx = (float)x + 0.5f - cx;
            float dy = (float)y + 0.5f - cy;
            float d = sqrtf(dx * dx + dy * dy);
            float coverage = r - d + 0.5f;
            px_aa(buf, stride, x, y, coverage);
        }
    }
}

/* Circle ring (outline) */
static void draw_circle_ring(uint8_t *buf, int stride, float cx, float cy,
                              float r, float thickness)
{
    float r_outer = r + thickness * 0.5f;
    float r_inner = r - thickness * 0.5f;
    int x0 = (int)(cx - r_outer - 1);
    int x1 = (int)(cx + r_outer + 2);
    int y0 = (int)(cy - r_outer - 1);
    int y1 = (int)(cy + r_outer + 2);
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            float dx = (float)x + 0.5f - cx;
            float dy = (float)y + 0.5f - cy;
            float d = sqrtf(dx * dx + dy * dy);
            float outer_cov = r_outer - d + 0.5f;
            float inner_cov = d - r_inner + 0.5f;
            float coverage = outer_cov < inner_cov ? outer_cov : inner_cov;
            px_aa(buf, stride, x, y, coverage);
        }
    }
}

/* Filled rounded rectangle */
static void draw_rounded_rect(uint8_t *buf, int stride,
                               float x, float y, float w, float h, float r)
{
    int px0 = (int)(x - 1);
    int px1 = (int)(x + w + 2);
    int py0 = (int)(y - 1);
    int py1 = (int)(y + h + 2);
    for (int py = py0; py <= py1; py++) {
        for (int ppx = px0; ppx <= px1; ppx++) {
            float fx = (float)ppx + 0.5f;
            float fy = (float)py + 0.5f;
            /* Distance to nearest point inside rounded rect */
            float dx = 0, dy = 0;
            if (fx < x + r) dx = x + r - fx;
            else if (fx > x + w - r) dx = fx - (x + w - r);
            if (fy < y + r) dy = y + r - fy;
            else if (fy > y + h - r) dy = fy - (y + h - r);

            float coverage;
            if (dx > 0 && dy > 0) {
                float d = sqrtf(dx * dx + dy * dy);
                coverage = r - d + 0.5f;
            } else {
                /* Inside the straight edges */
                float ex = 0, ey = 0;
                if (fx < x) ex = x - fx;
                else if (fx > x + w) ex = fx - (x + w);
                if (fy < y) ey = y - fy;
                else if (fy > y + h) ey = fy - (y + h);
                float edge = ex > ey ? ex : ey;
                coverage = 0.5f - edge + 0.5f;
            }
            px_aa(buf, stride, ppx, py, coverage);
        }
    }
}

/* Thick line segment with anti-aliasing */
static void draw_thick_line(uint8_t *buf, int stride,
                             float x1, float y1, float x2, float y2, float thickness)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float len = sqrtf(dx * dx + dy * dy);
    if (len < 0.001f) return;
    float nx = -dy / len;
    float ny = dx / len;
    float half_t = thickness * 0.5f;

    int min_x = (int)(fminf(x1, x2) - half_t - 1);
    int max_x = (int)(fmaxf(x1, x2) + half_t + 2);
    int min_y = (int)(fminf(y1, y2) - half_t - 1);
    int max_y = (int)(fmaxf(y1, y2) + half_t + 2);

    for (int py = min_y; py <= max_y; py++) {
        for (int ppx = min_x; ppx <= max_x; ppx++) {
            float fx = (float)ppx + 0.5f - x1;
            float fy = (float)py + 0.5f - y1;
            /* Project onto line direction */
            float along = (fx * dx + fy * dy) / len;
            float perp = fabsf(fx * nx + fy * ny);
            /* Clamp to segment */
            float end_dist = 0;
            if (along < 0) end_dist = -along;
            else if (along > len) end_dist = along - len;
            float d = sqrtf(perp * perp + end_dist * end_dist);
            float coverage = half_t - d + 0.5f;
            px_aa(buf, stride, ppx, py, coverage);
        }
    }
}

/* Wavy line (sine wave, vertical direction) */
static void draw_wavy_line(uint8_t *buf, int stride,
                            float cx, float y_start, float y_end,
                            float amplitude, float wavelength, float thickness,
                            float phase)
{
    int py0 = (int)(y_start - 1);
    int py1 = (int)(y_end + 2);
    float half_t = thickness * 0.5f;

    for (int py = py0; py <= py1; py++) {
        float fy = (float)py + 0.5f;
        float x_center = cx + amplitude * sinf(2.0f * (float)M_PI * (fy - y_start) / wavelength + phase);
        int px0 = (int)(x_center - amplitude - half_t - 2);
        int px1 = (int)(x_center + amplitude + half_t + 2);
        for (int ppx = px0; ppx <= px1; ppx++) {
            float fx = (float)ppx + 0.5f;
            float d = fabsf(fx - x_center);
            float coverage = half_t - d + 0.5f;
            /* Also fade at vertical ends */
            float end_fade = 1.0f;
            if (fy < y_start + 2) end_fade = (fy - y_start) / 2.0f;
            if (fy > y_end - 2) end_fade = (y_end - fy) / 2.0f;
            if (end_fade < 0) end_fade = 0;
            if (end_fade > 1) end_fade = 1;
            px_aa(buf, stride, ppx, py, coverage * end_fade);
        }
    }
}

/*=======================================================
 * Setup image descriptor
 *=======================================================*/
static void setup_dsc(lv_image_dsc_t *dsc, uint8_t *data, int size)
{
    memset(&dsc->header, 0, sizeof(dsc->header));
    dsc->header.magic = LV_IMAGE_HEADER_MAGIC;
    dsc->header.cf    = LV_COLOR_FORMAT_A8;
    dsc->header.w     = size;
    dsc->header.h     = size;
    dsc->header.stride = size;
    dsc->data_size    = size * size;
    dsc->data         = data;
}

/*=======================================================
 * Icon generation functions
 *=======================================================*/

static void gen_coffee(void)
{
    memset(buf_coffee, 0, sizeof(buf_coffee));
    int s = ICON_LARGE;
    /* Cup body */
    draw_rounded_rect(buf_coffee, s, 14, 24, 28, 28, 4);
    /* Handle */
    draw_circle_ring(buf_coffee, s, 42, 36, 8, 3.0f);
    /* Mask out inside of handle that overlaps cup */
    for (int y = 24; y < 52; y++)
        for (int x = 14; x < 43; x++)
            ; /* handle ring already blends correctly */
    /* Steam lines */
    draw_wavy_line(buf_coffee, s, 22, 8, 22, 3, 14, 2.0f, 0);
    draw_wavy_line(buf_coffee, s, 32, 6, 20, 3, 14, 2.0f, 1.5f);
    /* Saucer */
    draw_rounded_rect(buf_coffee, s, 10, 52, 36, 4, 2);
    setup_dsc(&icon_coffee, buf_coffee, s);
}

static void gen_steam(void)
{
    memset(buf_steam, 0, sizeof(buf_steam));
    int s = ICON_LARGE;
    /* Three wavy steam lines */
    draw_wavy_line(buf_steam, s, 22, 10, 54, 4, 20, 2.5f, 0);
    draw_wavy_line(buf_steam, s, 32, 8,  56, 4, 20, 2.5f, 2.0f);
    draw_wavy_line(buf_steam, s, 42, 10, 54, 4, 20, 2.5f, 4.0f);
    setup_dsc(&icon_steam, buf_steam, s);
}

static void gen_hotwater(void)
{
    memset(buf_hotwater, 0, sizeof(buf_hotwater));
    int s = ICON_LARGE;
    /* Water droplet: circle bottom + pointed top */
    draw_filled_circle(buf_hotwater, s, 32, 40, 14);
    /* Triangle point at top */
    for (int y = 14; y < 40; y++) {
        float progress = (float)(y - 14) / 26.0f;
        float half_w = progress * 14.0f;
        for (int x = (int)(32 - half_w - 1); x <= (int)(32 + half_w + 1); x++) {
            float d = fabsf((float)x + 0.5f - 32.0f) - half_w;
            float coverage = 0.5f - d;
            px_aa(buf_hotwater, s, x, y, coverage);
        }
    }
    setup_dsc(&icon_hotwater, buf_hotwater, s);
}

static void gen_settings(void)
{
    memset(buf_settings, 0, sizeof(buf_settings));
    int s = ICON_LARGE;
    float cx = 32, cy = 32;
    /* Inner circle */
    draw_circle_ring(buf_settings, s, cx, cy, 10, 3.5f);
    /* Outer gear teeth (8 teeth) */
    for (int i = 0; i < 8; i++) {
        float angle = (float)i * (float)M_PI / 4.0f;
        float x1 = cx + 15.0f * cosf(angle);
        float y1 = cy + 15.0f * sinf(angle);
        float x2 = cx + 23.0f * cosf(angle);
        float y2 = cy + 23.0f * sinf(angle);
        draw_thick_line(buf_settings, s, x1, y1, x2, y2, 5.0f);
    }
    /* Outer ring connecting teeth */
    draw_circle_ring(buf_settings, s, cx, cy, 19, 4.0f);
    /* Center dot */
    draw_filled_circle(buf_settings, s, cx, cy, 4);
    setup_dsc(&icon_settings, buf_settings, s);
}

static void gen_back(void)
{
    memset(buf_back, 0, sizeof(buf_back));
    int s = ICON_SMALL;
    float cx = 24, cy = 24;
    /* Left-pointing chevron */
    draw_thick_line(buf_back, s, cx + 6, cy - 12, cx - 6, cy, 3.0f);
    draw_thick_line(buf_back, s, cx - 6, cy, cx + 6, cy + 12, 3.0f);
    setup_dsc(&icon_back, buf_back, s);
}

static void gen_play(void)
{
    memset(buf_play, 0, sizeof(buf_play));
    int s = ICON_SMALL;
    /* Right-pointing triangle */
    float x_left = 16, x_right = 36;
    float y_top = 10, y_bot = 38;
    float cy = (y_top + y_bot) / 2.0f;
    for (int y = (int)y_top; y <= (int)y_bot; y++) {
        float fy = (float)y + 0.5f;
        float progress;
        if (fy < cy) {
            progress = (fy - y_top) / (cy - y_top);
        } else {
            progress = (y_bot - fy) / (y_bot - cy);
        }
        float x_edge = x_left + progress * (x_right - x_left);
        for (int x = (int)x_left; x <= (int)(x_edge + 1); x++) {
            float coverage = 1.0f;
            float d_right = x_edge - ((float)x + 0.5f);
            if (d_right < 0.5f) coverage = d_right + 0.5f;
            float d_left = (float)x + 0.5f - x_left;
            if (d_left < 0.5f && d_left < coverage) coverage = d_left + 0.5f;
            px_aa(buf_play, s, x, y, coverage);
        }
    }
    setup_dsc(&icon_play, buf_play, s);
}

static void gen_stop(void)
{
    memset(buf_stop, 0, sizeof(buf_stop));
    int s = ICON_SMALL;
    /* Rounded square */
    draw_rounded_rect(buf_stop, s, 12, 12, 24, 24, 4);
    setup_dsc(&icon_stop, buf_stop, s);
}

static void gen_espresso(void)
{
    memset(buf_espresso, 0, sizeof(buf_espresso));
    int s = ICON_LARGE;
    /* Small demitasse cup */
    draw_rounded_rect(buf_espresso, s, 16, 28, 24, 20, 3);
    /* Handle */
    draw_circle_ring(buf_espresso, s, 40, 37, 6, 2.5f);
    /* Saucer */
    draw_rounded_rect(buf_espresso, s, 12, 48, 32, 4, 2);
    /* Steam */
    draw_wavy_line(buf_espresso, s, 24, 12, 26, 2.5f, 12, 1.8f, 0);
    draw_wavy_line(buf_espresso, s, 32, 10, 24, 2.5f, 12, 1.8f, 1.5f);
    setup_dsc(&icon_espresso, buf_espresso, s);
}

static void gen_cappuccino(void)
{
    memset(buf_cappuccino, 0, sizeof(buf_cappuccino));
    int s = ICON_LARGE;
    /* Cup body */
    draw_rounded_rect(buf_cappuccino, s, 14, 28, 28, 24, 4);
    /* Foam dome on top */
    draw_filled_circle(buf_cappuccino, s, 28, 28, 14);
    /* Handle */
    draw_circle_ring(buf_cappuccino, s, 42, 38, 7, 2.5f);
    /* Saucer */
    draw_rounded_rect(buf_cappuccino, s, 10, 52, 36, 4, 2);
    setup_dsc(&icon_cappuccino, buf_cappuccino, s);
}

static void gen_americano(void)
{
    memset(buf_americano, 0, sizeof(buf_americano));
    int s = ICON_LARGE;
    /* Taller cup (mug shape) */
    draw_rounded_rect(buf_americano, s, 16, 18, 24, 32, 4);
    /* Handle */
    draw_circle_ring(buf_americano, s, 40, 32, 8, 2.5f);
    /* Saucer */
    draw_rounded_rect(buf_americano, s, 12, 50, 32, 4, 2);
    /* Liquid line inside cup */
    draw_thick_line(buf_americano, s, 20, 26, 36, 26, 2.0f);
    setup_dsc(&icon_americano, buf_americano, s);
}

static void gen_latte(void)
{
    memset(buf_latte, 0, sizeof(buf_latte));
    int s = ICON_LARGE;
    /* Tall glass shape (slightly tapered) */
    for (int y = 16; y < 52; y++) {
        float progress = (float)(y - 16) / 36.0f;
        float half_w = 10.0f + progress * 4.0f;
        float cx = 28.0f;
        /* Left edge */
        draw_thick_line(buf_latte, s, cx - half_w, (float)y, cx - half_w, (float)(y + 1), 2.5f);
        /* Right edge */
        draw_thick_line(buf_latte, s, cx + half_w, (float)y, cx + half_w, (float)(y + 1), 2.5f);
    }
    /* Bottom */
    draw_thick_line(buf_latte, s, 18, 52, 42, 52, 2.5f);
    /* Top rim */
    draw_thick_line(buf_latte, s, 18, 16, 38, 16, 2.5f);
    /* Latte art: simple leaf */
    draw_wavy_line(buf_latte, s, 28, 24, 40, 3, 16, 1.5f, 0);
    /* Handle */
    draw_circle_ring(buf_latte, s, 44, 34, 7, 2.5f);
    setup_dsc(&icon_latte, buf_latte, s);
}

/*=======================================================
 * Public init
 *=======================================================*/
void ui_icons_init(void)
{
    gen_coffee();
    gen_steam();
    gen_hotwater();
    gen_settings();
    gen_back();
    gen_play();
    gen_stop();
    gen_espresso();
    gen_cappuccino();
    gen_americano();
    gen_latte();
}
