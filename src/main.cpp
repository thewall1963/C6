#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>

/* ── Pin config ──────────────────────────────────────────────────── */
#define LCD_DC_PIN    15
#define LCD_CS_PIN    14
#define LCD_SCK_PIN   7
#define LCD_MOSI_PIN  6
#define LCD_RST_PIN   21
#define LCD_BL_PIN    22

/* ── Display resolution ──────────────────────────────────────────── */
#define LCD_W           172
#define LCD_H           320
/*
 * ST7789 controller is 240-wide; 172-wide panel sits centred.
 * Offset = (240 - 172) / 2 = 34.
 */
#define LCD_COL_OFFSET  34

/* ── Globals (pointers only; objects created in setup after HAL init) */
static Arduino_DataBus *bus = nullptr;
static Arduino_GFX     *gfx = nullptr;

/* ── LVGL draw buffer (50 lines ≈ 17 KB) ────────────────────────── */
static lv_disp_draw_buf_t draw_buf;
static lv_color_t         buf1[LCD_W * 50];

/* ── LVGL flush callback ─────────────────────────────────────────── */
static void disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)color_p, w, h);
    lv_disp_flush_ready(drv);
}

/* ── Demo UI ─────────────────────────────────────────────────────── */
static void build_ui(void)
{
    lv_obj_t *scr = lv_scr_act();

    /* Header bar */
    lv_obj_t *hdr = lv_obj_create(scr);
    lv_obj_set_size(hdr, LCD_W, 40);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(hdr, 0, 0);
    lv_obj_set_style_bg_color(hdr, lv_color_hex(0x1565C0), 0);
    lv_obj_set_style_border_width(hdr, 0, 0);

    lv_obj_t *title = lv_label_create(hdr);
    lv_label_set_text(title, "ESP32-C6  LVGL");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_center(title);

    /* Info card */
    lv_obj_t *card = lv_obj_create(scr);
    lv_obj_set_size(card, LCD_W - 8, 70);
    lv_obj_align(card, LV_ALIGN_TOP_MID, 0, 48);
    lv_obj_set_style_pad_all(card, 6, 0);

    lv_obj_t *info = lv_label_create(card);
    lv_obj_set_style_text_font(info, &lv_font_montserrat_12, 0);
    lv_label_set_text(info,
        "Board:  Waveshare ESP32-C6\n"
        "LCD:    ST7789  172x320\n"
        "MCU:    160 MHz  4 MB Flash");
    lv_obj_align(info, LV_ALIGN_TOP_LEFT, 0, 0);

    /* Spinner */
    lv_obj_t *spin = lv_spinner_create(scr, 1000, 60);
    lv_obj_set_size(spin, 56, 56);
    lv_obj_align(spin, LV_ALIGN_TOP_MID, 0, 128);

    /* Slider */
    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_width(slider, LCD_W - 24);
    lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 200);
    lv_slider_set_value(slider, 60, LV_ANIM_OFF);

    lv_obj_t *sl_lbl = lv_label_create(scr);
    lv_label_set_text(sl_lbl, "Brightness: 60%");
    lv_obj_set_style_text_font(sl_lbl, &lv_font_montserrat_12, 0);
    lv_obj_align(sl_lbl, LV_ALIGN_TOP_MID, 0, 228);

    /* Button */
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_size(btn, LCD_W - 24, 42);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -12);

    lv_obj_t *btn_lbl = lv_label_create(btn);
    lv_label_set_text(btn_lbl, "Hello, LVGL!");
    lv_obj_center(btn_lbl);
}

/* ── Arduino entry points ────────────────────────────────────────── */
void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("[boot] starting setup");


    /* Backlight on */
    pinMode(LCD_BL_PIN, OUTPUT);
    digitalWrite(LCD_BL_PIN, HIGH);
    Serial.println("[boot] backlight on");

    /* Build display objects here (after Arduino HAL is fully ready) */
    bus = new Arduino_ESP32SPI(
        LCD_DC_PIN, LCD_CS_PIN, LCD_SCK_PIN, LCD_MOSI_PIN, GFX_NOT_DEFINED
    );
    gfx = new Arduino_ST7789(
        bus,
        LCD_RST_PIN,
        0,             /* rotation 0 = portrait */
        false,         /* not IPS */
        LCD_W, LCD_H,
        LCD_COL_OFFSET, 0,
        LCD_COL_OFFSET, 0
    );

    /* Display init */
    Serial.println("[boot] calling gfx->begin()");
    if (!gfx->begin(20000000)) {   /* 20 MHz SPI — half of 40, easier on power */
        Serial.println("[boot] ERROR: display init failed");
    } else {
        Serial.println("[boot] display OK");
    }
    gfx->fillScreen(0x0000);

    /* LVGL init */
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LCD_W * 50);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = LCD_W;
    disp_drv.ver_res  = LCD_H;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    build_ui();

    Serial.println("[boot] Ready.");
}

void loop()
{
    lv_timer_handler();
    delay(33);   /* ~30 fps is enough; reduces CPU and SPI activity */
}
