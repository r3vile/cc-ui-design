#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16

/*====================
   MEMORY SETTINGS
 *====================*/
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (128U * 1024U)

/*====================
   DISPLAY SETTINGS
 *====================*/
#define LV_DEF_REFR_PERIOD 16

/*====================
   FONT SETTINGS
 *====================*/
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_MONTSERRAT_16  1
#define LV_FONT_MONTSERRAT_20  1
#define LV_FONT_MONTSERRAT_24  1
#define LV_FONT_MONTSERRAT_28  1
#define LV_FONT_MONTSERRAT_36  1
#define LV_FONT_MONTSERRAT_48  1

#define LV_FONT_DEFAULT &lv_font_montserrat_14

/* Enable the built-in symbol font */
#define LV_USE_FONT_PLACEHOLDER 1

/*====================
   THEME
 *====================*/
#define LV_USE_THEME_DEFAULT 1

/*====================
   WIDGETS (all enabled by default in LVGL, keep it that way)
 *====================*/

/*====================
   DRAW ENGINE
 *====================*/
#define LV_USE_DRAW_SW_ASM      LV_DRAW_SW_ASM_NONE
#define LV_USE_NATIVE_HELIUM_ASM 0

/*====================
   ANIMATION
 *====================*/
#define LV_USE_ANIM 1

/*====================
   SDL (simulator)
 *====================*/
#ifdef SIMULATOR
    #define LV_USE_SDL 1
    #define LV_SDL_INCLUDE_PATH     <SDL2/SDL.h>
    #define LV_SDL_RENDER_MODE      LV_DISPLAY_RENDER_MODE_DIRECT
    #define LV_SDL_BUF_COUNT        1
    #define LV_SDL_FULLSCREEN       0
    #define LV_SDL_DIRECT_EXIT      1
#else
    #define LV_USE_SDL 0
#endif

/*====================
   OS ABSTRACTION
 *====================*/
#ifdef ESP_PLATFORM
    #define LV_USE_OS LV_OS_FREERTOS
#else
    #define LV_USE_OS LV_OS_NONE
#endif

/*====================
   LOGGING
 *====================*/
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

#endif /* LV_CONF_H */
