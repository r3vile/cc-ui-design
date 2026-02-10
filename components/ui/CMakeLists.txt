file(GLOB_RECURSE UI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${UI_SOURCES}
        INCLUDE_DIRS "include" "."
        REQUIRES lvgl
    )
else()
    add_library(ui STATIC ${UI_SOURCES})
    target_include_directories(ui PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    target_link_libraries(ui PUBLIC lvgl)
endif()
