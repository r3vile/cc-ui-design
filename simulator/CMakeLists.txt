cmake_minimum_required(VERSION 3.16)
project(espresso-ui-simulator C)

# Global SIMULATOR define so lv_conf.h enables SDL
add_compile_definitions(SIMULATOR=1)

# SDL2
find_package(SDL2 REQUIRED)

# LVGL configuration: point to our lv_conf.h
set(LV_CONF_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/../components/ui/lv_conf.h"
    CACHE STRING "LVGL config path" FORCE)

# Fetch LVGL v9
include(FetchContent)
FetchContent_Declare(
    lvgl
    GIT_REPOSITORY https://github.com/lvgl/lvgl.git
    GIT_TAG        v9.2.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(lvgl)

# Remove ARM-specific assembly files from LVGL target (they can't build on x86)
get_target_property(LVGL_SOURCES lvgl SOURCES)
list(FILTER LVGL_SOURCES EXCLUDE REGEX ".*\\.S$")
set_target_properties(lvgl PROPERTIES SOURCES "${LVGL_SOURCES}")

# Our shared UI component
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/ui
    ${CMAKE_CURRENT_BINARY_DIR}/ui
)

# Simulator executable
add_executable(simulator main.c)
target_link_libraries(simulator PRIVATE ui lvgl SDL2::SDL2 m)
