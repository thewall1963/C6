# Waveshare ESP32-C6-LCD-1.47 — LVGL Starter

PlatformIO project for the [Waveshare ESP32-C6-LCD-1.47](https://www.waveshare.com/esp32-c6-lcd-1.47.htm) development board.

| | |
|---|---|
| **MCU** | ESP32-C6 (RISC-V, 80 MHz) |
| **Display** | ST7789 · 172 × 320 · 262K colour |
| **GUI** | LVGL 8.4 |
| **Display driver** | Arduino_GFX 1.6.6 |
| **Framework** | Arduino (via pioarduino 54.03.21-2 / arduino-esp32 3.2.1) |

## Pin assignments

| Signal | GPIO |
|--------|------|
| LCD MOSI | 6 |
| LCD SCLK | 7 |
| LCD CS | 14 |
| LCD DC | **15** |
| LCD RST | 21 |
| LCD Backlight | **22** |

> **Note:** Many online sources incorrectly list DC = GPIO 4 and BL = GPIO 15.
> The correct values are DC = 15 and BL = 22.

## Requirements

### Hardware
- Waveshare ESP32-C6-LCD-1.47
- USB-C cable

### Software
- [VS Code](https://code.visualstudio.com/) + [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
- Python 3.10 or newer (required by pioarduino's esptool 5.x)

If your system Python is older than 3.10, create a virtual environment and point PlatformIO at it.
The included `use_python314.py` extra script handles this automatically for Homebrew Python 3.14 on macOS:

```
/opt/homebrew/bin/python3.14 -m venv ~/.platformio-esptool-venv
~/.platformio-esptool-venv/bin/pip install -e ~/.platformio/packages/tool-esptoolpy/
```

If you have a different Python path, update `use_python314.py` accordingly.

## Build & upload

```bash
# Build only
pio run

# Build and upload
pio run --target upload

# Upload to a specific port
pio run --target upload --upload-port /dev/cu.usbmodem1101
```

## Project structure

```
C6/
├── boards/
│   └── waveshare_esp32c6.json   # Custom board definition (adds Arduino framework support)
├── include/
│   └── lv_conf.h                # LVGL 8.x configuration
├── src/
│   └── main.cpp                 # Display init + LVGL demo UI
├── platformio.ini
└── use_python314.py             # esptool Python version shim
```

## What the demo shows

- Blue header bar with board name
- Info card (board, display, MCU details)
- Animated spinner
- Slider at 60%
- Button

## Customisation tips

| Problem | Fix |
|---|---|
| Colours inverted | Change `LV_COLOR_16_SWAP` in `include/lv_conf.h` |
| Image rotated / mirrored | Change the `0` (rotation) in `Arduino_ST7789(...)` to 1, 2, or 3 |
| Display offset wrong | Adjust `LCD_COL_OFFSET` in `src/main.cpp` (default 34) |
| Board runs hot | Already mitigated: 80 MHz CPU, 20 MHz SPI, 33 ms LVGL tick |

## Known build quirks

- **TFT_eSPI does not build for ESP32-C6** — it uses `VSPI` and register-level SPI code that only exists on the original Xtensa ESP32. Arduino_GFX is used instead.
- **The official PlatformIO espressif32 platform does not enable Arduino framework for ESP32-C6** — this project uses the [pioarduino](https://github.com/pioarduino/platform-espressif32) fork which ships arduino-esp32 3.x with full C6 support.
- **esptool 5.x requires Python ≥ 3.10** — handled via `use_python314.py`.

## License

MIT
