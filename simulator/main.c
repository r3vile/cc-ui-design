#define SDL_MAIN_HANDLED
#include "lvgl.h"
#include "ui.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    lv_init();

    /* Create 480x480 SDL window */
    lv_display_t *disp = lv_sdl_window_create(480, 480);

    /* Create input devices */
    lv_sdl_mouse_create();
    lv_sdl_mousewheel_create();

    /* Initialize the espresso UI */
    ui_init(disp);

    /* Main loop */
    while (1) {
        uint32_t ms = lv_timer_handler();
        usleep((ms < 5 ? 5 : ms) * 1000);
    }

    return 0;
}
