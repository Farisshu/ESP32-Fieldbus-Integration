# UART Basic Test 📡

Basic UART communication test with SPI initialization for ESP32.

## Overview

This project demonstrates:
- UART serial communication at 115200 baud
- SPI bus initialization for MCP2515 CAN controller
- Diagnostic routines for SPI device detection
- Built-in LED status indication

## Hardware Requirements

- ESP32 Development Board
- MCP2515 CAN Controller Module (optional, for SPI testing)
- USB Cable for programming and serial monitor

## Pin Configuration

| Signal | ESP32 Pin | MCP2515 Pin |
|--------|-----------|-------------|
| CS | GPIO 5 | CS |
| SCK | GPIO 18 | SCK |
| MOSI | GPIO 23 | SI |
| MISO | GPIO 19 | SO |
| LED | GPIO 2 | - |

## Features

1. **SPI Initialization**: Configures SPI at 500kHz for reliable communication
2. **MCP2515 Reset**: Sends soft reset command (0xC0)
3. **Register Verification**: Reads CANSTAT register to verify device presence
4. **Loopback Test**: Write/read verification for SPI communication
5. **Serial Output**: Detailed diagnostic messages via UART

## Usage

### Build and Upload

```bash
cd firmware/test_uart_basic
pio run --target upload
pio device monitor
```

### Expected Output

```
 MCP2515 SPI DIAGNOSTIC V2
────────────────────────────
1. SPI Init (500kHz)... OK
2. Send Reset (0xC0)... Done
3. Read CANSTAT... 0x80 (Config Mode) ✅
4. Write/Read Loopback... OK ✅
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No serial output | Wrong baud rate | Ensure monitor uses 115200 |
| CANSTAT reads 0xFF | MCP2515 not connected | Check wiring and power |
| SPI init fails | Pin conflict | Verify pin assignments |

## Source Files

- `src/main.cpp` - Main application code

## PlatformIO Configuration

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 9600
```

---

*Part of the ESP32 Logic Analyzer Automation project*
*Last updated: 2026-05-03 (All Issues Resolved)*
