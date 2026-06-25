/**
 * lv_conf.h — LVGL 8.x configuration
 * Waveshare ESP32-C6 1.47" (ST7789, 172x320)
 *
 * Only settings that differ from LVGL defaults are listed here.
 * lv_conf_internal.h supplies the rest automatically.
 */

/* clang-format off */
#if 1   /* must be 1 to activate this file */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* ── Color ───────────────────────────────────────────────────────── */
#define LV_COLOR_DEPTH      16      /* RGB565 */
#define LV_COLOR_16_SWAP    1       /* Arduino_GFX sends bytes in memory order; pre-swap for correct RGB on display */

/* ── Memory ──────────────────────────────────────────────────────── */
#define LV_MEM_SIZE         (64U * 1024U)   /* 64 KB LVGL heap */

/* ── HAL / tick ──────────────────────────────────────────────────── */
#define LV_TICK_CUSTOM              1
#define LV_TICK_CUSTOM_INCLUDE      "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

#define LV_DISP_DEF_REFR_PERIOD    10   /* ms */
#define LV_INDEV_DEF_READ_PERIOD   10   /* ms */

/* ── Logging ─────────────────────────────────────────────────────── */
#define LV_USE_LOG  0

/* ── Fonts ───────────────────────────────────────────────────────── */
#define LV_FONT_MONTSERRAT_12   1
#define LV_FONT_MONTSERRAT_14   1
#define LV_FONT_MONTSERRAT_16   1
#define LV_FONT_MONTSERRAT_20   1
#define LV_FONT_DEFAULT         &lv_font_montserrat_14

/* ── Theme ───────────────────────────────────────────────────────── */
#define LV_USE_THEME_DEFAULT        1
#define LV_THEME_DEFAULT_DARK       1   /* dark theme suits the display */
#define LV_THEME_DEFAULT_GROW       1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80

/* ── Extra widgets ───────────────────────────────────────────────── */
#define LV_USE_ARC          1
#define LV_USE_BAR          1
#define LV_USE_BTN          1
#define LV_USE_CHART        1
#define LV_USE_CHECKBOX     1
#define LV_USE_DROPDOWN     1
#define LV_USE_IMG          1
#define LV_USE_LABEL        1
#define LV_USE_LINE         1
#define LV_USE_METER        1
#define LV_USE_ROLLER       1
#define LV_USE_SLIDER       1
#define LV_USE_SPINNER      1
#define LV_USE_SWITCH       1
#define LV_USE_TABLE        1
#define LV_USE_TABVIEW      1
#define LV_USE_TEXTAREA     1

/* ── Layout engines ──────────────────────────────────────────────── */
#define LV_USE_FLEX     1
#define LV_USE_GRID     1

/* ── Disable unused 3rd-party decoders ──────────────────────────── */
#define LV_USE_PNG      0
#define LV_USE_BMP      0
#define LV_USE_SJPG     0
#define LV_USE_GIF      0
#define LV_BUILD_EXAMPLES 0

#endif /* LV_CONF_H */
#endif /* Enable content */
