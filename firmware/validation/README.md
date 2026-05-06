# Firmware Validation Tests

This directory contains low-level hardware validation tests used to verify individual components before integration.

## Purpose

These tests are designed for:
- **Hardware bring-up**: Verify new PCB or breadboard connections
- **Debugging**: Isolate hardware issues from software bugs
- **Documentation**: Provide reference waveforms for logic analyzer verification
- **CI/CD**: Automated hardware-in-the-loop testing (future)

## Test Categories

### 1. UART Loopback Verification (`uart_loopback_verification/`)
**Purpose**: Validate basic UART communication and SPI bus initialization.

**What it tests**:
- UART TX/RX at 115200 baud
- SPI bus initialization at 500kHz
- MCP2515 soft reset command
- Register read/write verification

**Expected Result**: 
- Serial output shows "SPI VERIFIED!" with checkmark
- LED stays steady ON (success) or fast blinks (failure)

**Logic Analyzer Capture**: See `../../evidence/uart_basic_test/`

---

### 2. MCP2515 SPI Validation (`mcp2515_spi_validation/`)
**Purpose**: Pure SPI pin toggling test for visual verification with oscilloscope or logic analyzer.

**What it tests**:
- GPIO output functionality on SPI pins (CS, SCK, MOSI, MISO)
- Signal integrity and timing
- Pin assignment correctness

**Method**: Blinks all 4 SPI pins simultaneously at 5Hz (100ms HIGH, 100ms LOW)

**Expected Waveform**:
```
CS   : ████████        ████████
SCK  : ████████        ████████
MOSI : ████████        ████████
MISO : ████████        ████████
     0ms      100ms    200ms
```

**Logic Analyzer Capture**: See `../../evidence/mcp2515_init_test/` and `../../evidence/spi_validation_test/`

---

## How to Run

### Prerequisites
- PlatformIO CLI or VS Code with PlatformIO extension
- ESP32 development board
- Logic analyzer (Saleae, DSView, etc.) - optional but recommended

### Build and Upload

```bash
# For UART Loopback Test
cd firmware/validation/uart_loopback_verification
pio run --target upload
pio device monitor

# For MCP2515 SPI Validation
cd firmware/validation/mcp2515_spi_validation
pio run --target upload
# No serial output - use logic analyzer to verify pin toggling
```

## Evidence Directory

All logic analyzer captures, reports, and waveform images are stored in:
```
/evidence/
├── uart_basic_test/          # From uart_loopback_verification
├── mcp2515_init_test/        # From mcp2515_spi_validation
├── spi_validation_test/      # Additional SPI tests
├── can_bus_monitor/          # Integration test captures
├── rs485_test/               # RS485 communication captures
└── ...
```

Each folder contains:
- `*.csv` - Raw logic analyzer data
- `waveform_annotated.png` - Visualized signals with protocol decoding
- `metadata.json` - Test configuration and parameters
- `professional_report.txt` - Auto-generated analysis report

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No serial output | Wrong baud rate | Use 115200 for monitor |
| CANSTAT reads 0xFF | MCP2515 not connected | Check wiring and power (3.3V vs 5V) |
| SPI pins not toggling | Wrong GPIO numbers | Verify pin assignments in code |
| Garbage data | Missing common ground | Connect GND between ESP32 and module |

## Next Steps

After passing validation tests:
1. Proceed to `../integration/` for multi-node communication tests
2. Move to `../projects/` for application-level implementations
3. Review evidence data with Python analysis tools in `../../software/`

---

*Part of ESP32 Logic Analyzer Automation Project*
*Documentation Version: 1.0*

---

## 🇯🇵 日本語メモ (にほんご めも - Japanese Notes)

このドキュメントは技術学習とインターンシップ準備のために作成されました。
(This document was created for technical learning and internship preparation.)

**重要な用語 (Important Terms):**
- ファームウェア (Firmware) - 組み込みソフトウェア
- デバッグ (Debug) - 修正・検証作業
- テスト (Test) - 試験・検証
- 資料 (Shiryou) - Documentation

