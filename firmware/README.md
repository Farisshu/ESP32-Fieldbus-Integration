# Firmware Directory 📦

This directory contains all ESP32 firmware projects built with PlatformIO.

## Structure

```
firmware/
├── tests/                  # Unit tests for individual modules
├── integration/            # Multi-node communication tests
├── test_uart_basic/        # UART communication test
└── test_can_spi_test/      # CAN/MCP2515 SPI interface test
```

## Projects Overview

| Project | Description | Status | Key Features |
|---------|-------------|--------|--------------|
| `test_uart_basic` | Basic UART with SPI initialization | ✅ Complete | UART TX/RX, SPI init, LED status |
| `test_can_spi_test` | MCP2515 CAN controller SPI diagnostic | ✅ Complete | Pin toggle test, wiring verification |
| `tests/mcp2515_can` | Comprehensive MCP2515 loopback test | ✅ Complete | Loopback mode, register verification |
| `tests/st7735s_tft` | ST7735S TFT display test | ✅ Complete | Color test, text rendering |
| `tests/tft_mcp2515_combined` | Shared SPI bus test (TFT + MCP2515) | ✅ Complete | CS arbitration, no bus conflict |
| `integration/can_two_nodes` | Two-node CAN bus communication | ✅ Complete | 500 kbps, TX/RX roles, error mon. |
| `integration/can_bus_with_tft` | CAN monitor with TFT & FreeRTOS | ✅ Complete | Multi-task, SPI sharing, LittleFS |

## Getting Started

### Prerequisites

- [PlatformIO Core](https://platformio.org/install/cli) or [PlatformIO IDE](https://platformio.org/platformio-ide)
- ESP32 development board
- USB cable
- Serial monitor (115200 baud)

### Build & Upload

```bash
cd firmware/<project_name>
pio run --target upload
pio device monitor
```

### Common Commands

```bash
# Build project
pio run

# Upload to device
pio run --target upload

# Open serial monitor
pio device monitor

# Clean build files
pio run --target clean

# Run tests (if available)
pio test
```

## Testing Workflow

1. **Hardware Verification**: Start with `test_can_spi_test/` for basic pin testing
2. **Unit Tests**: Verify individual modules in `tests/`
3. **Integration Tests**: Test multi-component systems in `integration/`
4. **Final Application**: Deploy complete applications

## Adding New Projects

1. Create a new directory: `mkdir firmware/my_new_project`
2. Initialize PlatformIO: `pio init --board esp32dev --directory firmware/my_new_project`
3. Add your source code to `src/`
4. Configure `platformio.ini` as needed
5. Update this README with project details

## Pin Configuration Reference

### SPI Shared Bus (MCP2515 + ST7735S)

| Module | Pin | Function | Notes |
|--------|-----|----------|-------|
| MCP2515 | GPIO 5 | CS (Chip Select) | Active LOW |
| MCP2515 | GPIO 18 | SCK (SPI Clock) | Shared with TFT |
| MCP2515 | GPIO 23 | MOSI (SPI Data Out) | Shared with TFT |
| MCP2515 | GPIO 19 | MISO (SPI Data In) | MCP2515 only |
| ST7735S | GPIO 17 | CS (Chip Select) | Active LOW |
| ST7735S | GPIO 16 | DC (Data/Command) | Control pin |
| ST7735S | GPIO 4 | RST (Reset) | Active LOW |
| Built-in LED | GPIO 2 | Status Indicator | For debug |

### CAN Bus Wiring

```
Node TX (MCP2515)          Node RX (MCP2515)
     CANH ───────────────────── CANH
     CANL ───────────────────── CANL
              ┌─────────┐
              │ 120Ω    │  (Termination at BOTH ends)
              └─────────┘
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| Build fails | Missing PlatformIO | Run `pip install platformio` |
| Upload fails | Wrong COM port | Check `pio device list` |
| MCP2515 not responding | SPI wiring issue | Verify CS, SCK, MOSI, MISO connections |
| TFT shows white screen | Wrong init sequence | Try different `initR()` parameter |
| CAN errors (EFLG > 0) | Bus noise/termination | Add 120Ω resistor, check wiring |

## Best Practices

1. **Always test SPI devices individually** before sharing the bus
2. **Use common ground** between all modules
3. **Add termination resistors** (120Ω) for CAN bus
4. **Keep SPI cables short** (<15cm) to reduce noise
5. **Use `SPI.beginTransaction()`** for shared bus arbitration
6. **Monitor EFLG register** for CAN bus health
7. **Use configuration constants** from `config.h` instead of magic numbers
8. **Implement queue drain limiters** to prevent task starvation
9. **Add SPI stabilization delay** (2µs) after CS LOW for MCP2515 clone modules

---

*For detailed documentation, see the main [README.md](../README.md)*
*Last updated: 2026-05-03 (All Issues Resolved)*

---

## 🇯🇵 日本語メモ (にほんご めも - Japanese Notes)

このドキュメントは技術学習とインターンシップ準備のために作成されました。
(This document was created for technical learning and internship preparation.)

**重要な用語 (Important Terms):**
- ファームウェア (Firmware) - 組み込みソフトウェア
- デバッグ (Debug) - 修正・検証作業
- テスト (Test) - 試験・検証
- 資料 (Shiryou) - Documentation

