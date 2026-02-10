/*
 * Example: Integrating the espresso machine UI with ESP32 + LVGL.
 *
 * This file shows how to initialize the UI and feed it real data
 * from your machine's sensors and actuators. It is NOT a complete
 * ESP-IDF application — adjust to your display driver, timer setup,
 * and hardware abstraction layer.
 *
 * Prerequisites:
 *   - LVGL 8.3.x configured via lv_conf.h
 *   - Display driver initialized (e.g. GC9A01 480×480 round)
 *   - Touch driver initialized (e.g. CST816S)
 *   - lv_tick_inc() called from a timer interrupt
 *
 * Required fonts in lv_conf.h:
 *   #define LV_FONT_MONTSERRAT_10  1
 *   #define LV_FONT_MONTSERRAT_12  1
 *   #define LV_FONT_MONTSERRAT_14  1
 *   #define LV_FONT_MONTSERRAT_18  1
 *   #define LV_FONT_MONTSERRAT_20  1
 *   #define LV_FONT_MONTSERRAT_22  1
 *   #define LV_FONT_MONTSERRAT_28  1
 *   #define LV_FONT_MONTSERRAT_48  1
 */

#include "ui/ui.h"
#include <stdio.h>

/* ── Application callbacks ── */

static void on_brew(const char *coffee_type)
{
    printf("Brewing: %s\n", coffee_type);
    /* TODO: Start brew cycle for the selected coffee type.
     *       E.g. set PID target, start pump, open solenoid. */
}

static void on_water_toggle(bool start)
{
    if (start) {
        printf("Hot water: START\n");
        /* TODO: Open hot water solenoid / start pump */
    } else {
        printf("Hot water: STOP\n");
        /* TODO: Close solenoid / stop pump */
    }
}

/* ── Simulated sensor update (call this from your real sensor task) ── */

void app_update_steam_temp(int temp_c)
{
    ui_steam_set_temp(temp_c);

    if (temp_c >= 125) {
        ui_steam_set_state("READY");
    } else {
        ui_steam_set_state("HEATING");
    }
}

void app_update_water_temp(int temp_c)
{
    ui_water_set_temp(temp_c);

    if (temp_c >= 95) {
        ui_water_set_state("READY");
    } else {
        ui_water_set_state("HEATING");
    }
}

/* ── Entry point ── */

void app_ui_init(void)
{
    /*
     * Call this AFTER:
     *   - lv_init()
     *   - Display driver registration
     *   - Input device driver registration
     */

    /* Initialize the espresso UI */
    ui_init();

    /* Register application callbacks */
    ui_coffee_set_brew_cb(on_brew);
    ui_water_set_toggle_cb(on_water_toggle);

    /* Set initial targets (optional — defaults are 125°C / 95°C) */
    ui_steam_set_target(125);
    ui_water_set_target(95);
}

/*
 * In your main loop or RTOS task:
 *
 *   while (1) {
 *       lv_timer_handler();
 *       vTaskDelay(pdMS_TO_TICKS(5));
 *
 *       // Periodically read sensors and update UI
 *       int steam_temp = read_steam_thermocouple();
 *       app_update_steam_temp(steam_temp);
 *
 *       int water_temp = read_water_thermocouple();
 *       app_update_water_temp(water_temp);
 *   }
 */
