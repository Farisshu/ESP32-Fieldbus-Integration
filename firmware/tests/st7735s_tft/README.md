# ST7735S TFT Display Test 🖥️

This directory contains a test project for validating the **ST7735S TFT LCD display** (128x128 pixels) with ESP32 using SPI communication.

## Purpose

This test verifies:
- SPI communication between ESP32 and ST7735S
- Display initialization and configuration
- Basic graphics operations (fill screen, draw text, colors)
- Screen rotation functionality

## Hardware Requirements

| Component | Quantity | Specification |
|-----------|----------|---------------|
| ESP32 Dev Board | 1 | DOIT ESP32 DEVKIT V1 or equivalent |
| ST7735S TFT Module | 1 | 1.44" 128x128 IPS Display |
| Jumper Wires | - | For SPI connections |

### Pinout Configuration

| TFT Pin | ESP32 GPIO | Function |
|---------|------------|----------|
| **VCC** | 5V (or 3.3V) | Power |
| **GND** | GND | Ground |
| **SCK/CLK** | GPIO 18 | SPI Clock |
| **SDA/MOSI** | GPIO 23 | Data Input |
| **CS** | GPIO 17 | Chip Select |
| **DC/RS** | GPIO 16 | Data/Command |
| **RST** | GPIO 4 | Reset |
| **BLK** | 5V (or PWM) | Backlight |

## Building & Uploading

```bash
cd firmware/tests/st7735s_tft
pio run --target upload
pio device monitor
```

## Expected Output

### Serial Monitor:
```text
🔍 ST7735S TFT Test Started
✅ Display Initialized
✅ All tests passed!
```

### Display Test Sequence:
1. Red screen (500ms)
2. Green screen (500ms)
3. Blue screen (500ms)
4. Black screen with text:
   - "ST7735S OK" (white)
   - "SPI Shared" (yellow)

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Blank screen | Check power and backlight connection |
| Garbled display | Verify correct INIT tab (INITR_144GREENTAB) |
| No response | Check CS and DC pin connections |
| Wrong colors | Try different INIT tab variant |

---

*Part of the `esp32-logic-analyzer-automation` firmware tests*
