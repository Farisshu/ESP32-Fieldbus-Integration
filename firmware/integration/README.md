# Integration Tests 🔗

Multi-node communication tests and integration scenarios for ESP32-based systems.

## Overview

This directory contains integration tests that verify communication between multiple devices or nodes. These tests go beyond unit testing to validate complete system behavior.

## Structure

```
integration/
├── can_two_nodes/         # Two-node CAN bus communication test
├── can_bus_with_tft/      # CAN monitor with TFT visualization & FreeRTOS
└── README.md              # This file
```

## Projects

### 1. CAN Two Nodes (`can_two_nodes/`)

A dual-node CAN bus communication system using MCP2515 controllers.

**Features:**
- ✅ Configurable node roles (TX/RX) via compile-time flag
- ✅ 500 kbps CAN bus communication
- ✅ Message transmission and reception verification
- ✅ Error handling and EFLG monitoring
- ✅ Periodic transmission (1 second interval)

**Hardware Required:**
- 2x ESP32 boards
- 2x MCP2515 CAN modules with TJA1050 transceivers
- CAN bus wiring (CAN_H, CAN_L, 120Ω termination resistors)

**Setup:**
```bash
cd firmware/integration/can_two_nodes
pio run --target upload  # Upload to both nodes
pio device monitor       # Monitor serial output (115200 baud)
```

**Configuration:**
Edit `src/main.cpp` to set node role:
```cpp
#define NODE_ROLE TX  // or RX for receiver node
```

**Expected Output:**

TX Node:
```
[TX] Frame Sent | TXB0CTRL: 0x00 | EFLG: 0x05
```

RX Node:
```
[RX] ID: 0x123 | DLC: 8 | Data: AA BB CC DD EE FF 00 11 | EFLG: 0x05
```

---

### 2. CAN Bus with TFT (`can_bus_with_tft/`)

Real-time CAN 2.0A monitor using ESP32, MCP2515, and ST7735S (128x128) on a **shared SPI bus**. Built with clean architecture for maintainability & industrial prototyping.

**Features:**
- ✅ **FreeRTOS Multitasking**: 3 concurrent tasks (CAN polling, UI refresh, CSV logging)
- ✅ **SPI Bus Sharing**: MCP2515 + ST7735S on same bus with CS arbitration
- ✅ **Modular Architecture**: Driver/UI/App separation (R&D standard)
- ✅ **LittleFS Logging**: Non-blocking CSV export to flash
- ✅ **Queue-Based Sync**: Thread-safe message passing between tasks
- ✅ **Error Monitoring**: EFLG register tracking for bus health diagnostics

**Architecture:**
```
src/
├── config.h              # Pin mapping & system constants
├── mcp2515_driver.h/cpp  # Low-level SPI register access, frame parsing
├── tft_ui.h/cpp          # Adafruit GFX rendering, layout management
├── logger.h/cpp          # LittleFS CSV logging
├── app_tasks.h/cpp       # FreeRTOS task definitions
└── main.cpp              # Application orchestration
```

**Task Configuration:**
| Task | Priority | Core | Interval | Purpose |
|------|----------|------|----------|---------|
| `vTaskCAN` | 2 | 1 | 20ms | Poll MCP2515 for incoming frames |
| `vTaskUI` | 1 | 0 | 100ms | Update TFT display (max 10Hz) |
| `vTaskLogger` | 1 | 1 | 500ms | Log frames to CSV (batch mode) |

**Hardware Required:**
- 1x ESP32 board
- 1x MCP2515 CAN module
- 1x ST7735S TFT (128x128, green tab)
- 1x TJA1050 CAN transceiver
- 120Ω termination resistor

**Wiring (SPI Shared Bus):**

| ESP32 | MCP2515 | ST7735S TFT | Note |
|-------|---------|-------------|------|
| GPIO 18 (SCK) | SCK | SCL | Shared Clock |
| GPIO 23 (MOSI) | SI | SDA | Shared Data |
| GPIO 19 (MISO) | SO | - | MCP2515 only |
| GPIO 5 | CS | - | MCP2515 Chip Select |
| GPIO 17 | - | CS | TFT Chip Select |
| GPIO 16 | - | DC | TFT Data/Command |
| GPIO 4 | - | RST | TFT Reset |
| GND | GND | GND | **Common Ground Required** |
| 3.3V/5V | VCC | VCC | Power (use LLC if needed) |

**Setup:**
```bash
cd firmware/integration/can_bus_with_tft
pio run --target upload
pio device monitor  # 115200 baud
```

**Expected Output:**
```
🚀 CAN Bus + TFT (FreeRTOS + Logging)
✅ System Ready. Tasks Started.
[RX] ID:0x021 DLC:8 | Count: 1
[RX] ID:0x123 DLC:8 | Count: 2
```

**TFT Display Layout:**
```
┌─────────────────────┐
│ CAN BUS MONITOR     │
├─────────────────────┤
│ ID:    0x123        │
│ DATA:  AA BB CC DD  │
│ COUNT: 47            │
│ STATUS: ACTIVE OK   │
└─────────────────────┘
```

**CSV Log Format (`/can_log.csv`):**
```csv
timestamp_ms,id,dlc,d0,d1,d2,d3,d4,d5,d6,d7
1234,0x123,8,AA,BB,CC,DD,EE,FF,00,11
```

---

## Integration Test Workflow

1. **Individual Node Testing**: Verify each node works independently
   - Run `test_can_spi_test/` for basic SPI verification
   - Run `tests/mcp2515_can/` for loopback test

2. **Pairwise Testing**: Test communication between two nodes
   - Deploy `can_two_nodes/` to TX and RX nodes
   - Verify message transmission and reception

3. **Full Integration**: Add visualization and logging
   - Deploy `can_bus_with_tft/` to RX node
   - Verify TFT display and CSV logging

4. **Stress Testing**: Validate under load and error conditions
   - Increase message frequency
   - Monitor EFLG register for errors
   - Test with different cable lengths

## Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| No CAN messages received | Missing termination | Add 120Ω resistor across CAN_H/L at BOTH ends |
| CRC errors in frames | Wiring issues / noise | Check cable connections, use twisted pair |
| Node not responding | Wrong configuration | Verify `NODE_ROLE` setting in code |
| TFT shows garbage data | SPI conflict | Ensure CS pins are correctly configured |
| High EFLG (>0x80) | Bus-off condition | Check termination, reduce cable length |
| Queue overflow | Too many messages | Reduce CAN traffic or increase queue size |

## Best Practices

1. **Always test SPI devices individually** before sharing the bus
2. **Use common ground** between all modules and nodes
3. **Add termination resistors** (120Ω) at BOTH ends of CAN bus
4. **Keep SPI cables short** (<15cm) to reduce noise and timing issues
5. **Use `SPI.beginTransaction()`** for shared bus arbitration (already implemented)
6. **Monitor EFLG register** regularly for early error detection
7. **Implement queue draining** to prevent message loss (already implemented)
8. **Use FreeRTOS priorities wisely** - higher priority for time-critical tasks

## Performance Metrics

| Metric | Value | Notes |
|--------|-------|-------|
| CAN Bus Speed | 500 kbps | Standard for automotive |
| SPI Frequency | 250 kHz | Conservative for stability |
| UI Refresh Rate | 5-10 Hz | Smooth display without flicker |
| Logger Frequency | 2 Hz | Batch logging to reduce I/O |
| Queue Size | 10 messages | Adequate for typical traffic |
| Max Supported DLC | 8 bytes | CAN 2.0A standard |

## Future Improvements

- [ ] Auto-baud detection for crystal tolerance compensation
- [ ] Extended frame (29-bit ID) support
- [ ] WiFi/Bluetooth remote monitoring via MQTT
- [ ] ISO 15765-2 (CAN Transport Protocol) layer
- [ ] Web-based dashboard for real-time monitoring
- [ ] Automated CI/CD pipeline for firmware testing
- [ ] Unit tests with Unity framework

---

*For more information, see the main [firmware README](../README.md)*
*Last updated: 2026-05-03*
