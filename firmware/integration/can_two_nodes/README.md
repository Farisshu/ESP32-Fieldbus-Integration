# CAN Two Nodes 🔗

Dual-node CAN bus communication system using ESP32 and MCP2515 controllers.

## Overview

This project implements a complete CAN bus communication system with two nodes:
- **TX Node**: Transmits CAN messages
- **RX Node**: Receives and displays CAN messages

## Hardware Requirements

| Component | Quantity | Notes |
|-----------|----------|-------|
| ESP32 Dev Board | 2 | Any ESP32 variant |
| MCP2515 Module | 2 | With TJA1050 transceiver |
| Jumper Wires | ~16 | For SPI and CAN connections |
| 120Ω Resistor | 1 | CAN bus termination |

## Wiring Diagram

### ESP32 to MCP2515 (Both Nodes)

| ESP32 Pin | MCP2515 Pin | Function |
|-----------|-------------|----------|
| GPIO 5 | CS | Chip Select |
| GPIO 18 | SCK | SPI Clock |
| GPIO 23 | SI | MOSI |
| GPIO 19 | SO | MISO |
| 5V | VCC | Power |
| GND | GND | Ground |

### CAN Bus Connection

```
Node 1 (TX)                    Node 2 (RX)
MCP2515                        MCP2515
┌─────────┐                   ┌─────────┐
│ CAN_H   ├──────┬────────────┤ CAN_H   │
│ CAN_L   ├──────┼────────────┤ CAN_L   │
└─────────┘      │            └─────────┘
                 │
           ┌─────┴─────┐
           │  120Ω     │  Termination Resistor
           └───────────┘
```

## Configuration

### Set Node Role

Edit `src/main.cpp` on each node:

**TX Node:**
```cpp
#define NODE_ROLE TX
```

**RX Node:**
```cpp
#define NODE_ROLE RX
```

### CAN Settings

Default configuration in `src/main.cpp`:
```cpp
#define CAN_BAUDRATE_KBPS 500    // 500 kbps
#define CRYSTAL_FREQ_HZ 8000000  // 8 MHz crystal
```

## Usage

### Build and Upload

```bash
cd firmware/integration/can_two_nodes

# Upload to TX node
pio run --target upload

# Upload to RX node (on second ESP32)
pio run --target upload

# Monitor both nodes
pio device monitor -p /dev/ttyUSB0  # TX node
pio device monitor -p /dev/ttyUSB1  # RX node
```

### Expected Output

**TX Node:**
```
CAN TX Node Initialized
Sending message every 1000ms...
Message sent: ID=0x123, Data=01 02 03 04 05 06 07 08
```

**RX Node:**
```
CAN RX Node Initialized
Waiting for messages...
Message received: ID=0x123, Data=01 02 03 04 05 06 07 08
```

## Features

- **Configurable Baud Rate**: Support for standard CAN speeds (125k, 250k, 500k, 1M)
- **Error Handling**: Detects and reports CAN bus errors
- **LED Indicators**: Visual status feedback
- **Serial Debugging**: Detailed output via UART

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No messages received | Missing termination | Add 120Ω resistor |
| CRC errors | Incorrect baud rate | Match settings on both nodes |
| Bus off error | Wiring fault | Check CAN_H/L connections |
| SPI errors | Wrong pin wiring | Verify SPI connections |

## Captures Directory

The `captures/` directory stores logic analyzer captures and test data:
- CSV exports from PulseView
- Test logs and debug information
- Reference waveforms

## Testing Checklist

- [ ] Both nodes power up correctly
- [ ] SPI communication verified on both nodes
- [ ] CAN bus properly terminated
- [ ] TX node sends messages at expected interval
- [ ] RX node receives all messages without errors
- [ ] No CRC or bus errors in serial output

---

*Part of the ESP32 Logic Analyzer Automation project*
