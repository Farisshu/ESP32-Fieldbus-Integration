# CAN SPI Test 🔌

MCP2515 CAN controller SPI interface diagnostic test for ESP32.

## Overview

This project provides a comprehensive diagnostic test for the MCP2515 CAN controller's SPI interface. It verifies:
- SPI communication integrity
- MCP2515 register access
- Basic initialization sequence

## Hardware Requirements

- ESP32 Development Board
- MCP2515 CAN Controller Module
- 8MHz crystal MCP2515 (most common)
- USB Cable for programming

## Pin Configuration

| Signal | ESP32 Pin | MCP2515 Pin | Description |
|--------|-----------|-------------|-------------|
| CS | GPIO 5 | CS | Chip Select (active low) |
| SCK | GPIO 18 | SCK | SPI Clock |
| MOSI | GPIO 23 | SI | SPI Data Out (Master Out) |
| MISO | GPIO 19 | SO | SPI Data In (Master In) |
| LED | GPIO 2 | - | Status indicator |

## Features

### Diagnostic Blink Test
The main test performs a simple blink pattern on all 4 SPI pins:
- All pins HIGH for 100ms
- All pins LOW for 100ms
- Repeats continuously

This allows visual verification with a logic analyzer or oscilloscope.

## Usage

### Build and Upload

```bash
cd firmware/test_can_spi_test
pio run --target upload
pio device monitor
```

### Verify with Logic Analyzer

1. Connect logic analyzer probes to GPIO 5, 18, 19, 23
2. Capture at 8+ MS/s sample rate
3. Look for square wave pattern at 5Hz (100ms high, 100ms low)
4. All channels should toggle simultaneously

### Expected Waveform

```
CS   : ████████        ████████        
SCK  : ████████        ████████        
MOSI : ████████        ████████        
MISO : ████████        ████████        
     0ms      100ms    200ms
```

## Source Files

- `src/main.cpp` - Main diagnostic blink code
- `include/` - Header files (if needed)
- `lib/` - Private libraries (if needed)
- `test/` - Unit tests (if implemented)

## PlatformIO Configuration

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No output on pins | Wrong pin numbers | Verify GPIO assignments |
| Weak signal | Pin not set as output | Check pinMode() calls |
| Irregular timing | Interrupts interfering | Disable interrupts if needed |

## Next Steps

After verifying SPI pin functionality:
1. Proceed to full MCP2515 initialization test
2. Implement CAN message transmission
3. Test CAN bus communication with another node

---

*Part of the ESP32 Logic Analyzer Automation project*
