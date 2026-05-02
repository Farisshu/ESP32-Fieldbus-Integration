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

| Project | Description | Status |
|---------|-------------|--------|
| `tests/` | Unit testing framework and test cases | ✅ Ready |
| `integration/can_two_nodes/` | Two-node CAN bus communication | 🔄 In Progress |
| `test_uart_basic/` | Basic UART with SPI initialization | ✅ Complete |
| `test_can_spi_test/` | MCP2515 CAN controller SPI diagnostic | ✅ Complete |

## Getting Started

### Prerequisites

- [PlatformIO Core](https://platformio.org/install/cli) or [PlatformIO IDE](https://platformio.org/platformio-ide)
- ESP32 development board
- USB cable

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
```

## Testing Workflow

1. **Unit Tests**: Verify individual modules in `tests/`
2. **Integration Tests**: Test multi-component systems in `integration/`
3. **Projects**: Deploy complete applications

## Adding New Projects

1. Create a new directory: `mkdir firmware/my_new_project`
2. Initialize PlatformIO: `pio init --board esp32dev --directory firmware/my_new_project`
3. Add your source code to `src/`
4. Configure `platformio.ini` as needed

## Pin Configuration Reference

| Module | Pin | Function |
|--------|-----|----------|
| MCP2515 | GPIO 5 | CS (Chip Select) |
| MCP2515 | GPIO 18 | SCK (SPI Clock) |
| MCP2515 | GPIO 23 | MOSI (SPI Data Out) |
| MCP2515 | GPIO 19 | MISO (SPI Data In) |
| Built-in LED | GPIO 2 | Status Indicator |

---

*For detailed documentation, see the main [README.md](../README.md)*
