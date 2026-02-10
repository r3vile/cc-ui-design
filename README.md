# Espresso UI — LVGL Pro XML

This repository uses an **LVGL Pro XML project layout** for a 480x480 round ESP32 display UI.

## Files

- `lvgl_pro_xml/project.xml`: project definition (display metadata, source includes, entry screen).
- `lvgl_pro_xml/screens.xml`: aggregator/include file used by XML tooling.
- `lvgl_pro_xml/espresso_ui.xml`: full screen UI definition.

## Included screens

- Main (radial icon actions)
- Coffee (swipe-like selector + back + play)
- Steam (circular temperature/status)
- Hot water (circular temperature/status + start/stop)
- Settings placeholder

## Runtime IDs for binding

- `coffee_name`
- `steam_arc`, `steam_temp`, `steam_state`
- `water_arc`, `water_temp`, `water_state`, `water_start_stop`

## Notes

- `project.xml` is now the XML project entry point.
- `screens.xml` and `project.xml` reference `espresso_ui.xml` so the UI can be consumed as a project instead of a standalone file.
- The design remains LVGL Pro XML-only.
