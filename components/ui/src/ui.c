#include "ui.h"
#include "ui_theme.h"
#include "ui_icons.h"
#include "ui_screens.h"

void ui_init(lv_display_t *disp)
{
    ui_theme_init(disp);
    ui_icons_init();
    ui_screens_init();
    ui_screens_navigate(UI_SCREEN_MAIN);
}
