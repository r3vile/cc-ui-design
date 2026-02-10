# Espresso UI — LVGL Pro XML

This repository now uses **LVGL Pro XML** only.

## Files

- `lvgl_pro_xml/espresso_ui.xml`: full screen definition for the espresso-machine UI.

## Included screens

- Main (radial icon actions)
- Coffee (swipe-like selector + back + play)
- Steam (circular temperature/status)
- Hot water (circular temperature/status + start/stop)
- Settings placeholder

## How to preview

Use the LVGL Pro XML workflow from the LVGL docs to load this XML and preview it in the LVGL environment (simulator/editor pipeline for XML projects).

At runtime you can bind your controller logic to IDs such as:

- `coffee_name`
- `steam_arc`, `steam_temp`, `steam_state`
- `water_arc`, `water_temp`, `water_state`, `water_start_stop`

## Note

All previous non-XML preview and C scaffolding files were intentionally removed per request.
