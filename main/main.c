#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "ui.h"

static const char *TAG = "espresso-ui";

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing LVGL port");

    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    esp_err_t err = lvgl_port_init(&lvgl_cfg);
    assert(err == ESP_OK);

    /*
     * TODO: Initialize your display driver here.
     *
     * Example for a typical SPI round display (GC9A01):
     *   1. Configure SPI bus via spi_bus_initialize()
     *   2. Create panel IO via esp_lcd_new_panel_io_spi()
     *   3. Create panel via esp_lcd_new_panel_gc9a01()
     *   4. Initialize panel via esp_lcd_panel_init()
     *
     * Example for an RGB parallel display (ST7701S):
     *   1. Configure RGB panel via esp_lcd_new_rgb_panel()
     *   2. Initialize panel via esp_lcd_panel_init()
     *
     * After creating the panel, register it with LVGL:
     */

    /* Placeholder display config — fill in after LCD driver init */
    /*
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = 480 * 48,
        .double_buffer = true,
        .hres = 480,
        .vres = 480,
    };
    lv_display_t *disp = lvgl_port_add_disp(&disp_cfg);
    */

    /* TODO: Initialize touch input driver and call lvgl_port_add_touch() */

    /*
     * Once disp is initialized, start the UI:
     *
     *   lvgl_port_lock(0);
     *   ui_init(disp);
     *   lvgl_port_unlock();
     */

    ESP_LOGI(TAG, "UI ready — configure your display driver to start");
}
