# MCP2515 CAN Module Test 🧪

This directory contains a test project for validating the **MCP2515 CAN controller** with ESP32 using direct SPI communication.

## Purpose

This test verifies:
- SPI communication between ESP32 and MCP2515
- MCP2515 register read/write operations
- CAN bus initialization and configuration
- Basic message transmission and reception

## Hardware Requirements

| Component | Quantity | Specification |
|-----------|----------|---------------|
| ESP32 Dev Board | 1 | DOIT ESP32 DEVKIT V1 or equivalent |
| MCP2515 Module | 1 | TJA1050 transceiver |
| Jumper Wires | - | For SPI connections |

### Pinout Configuration

| MCP2515 Pin | ESP32 GPIO | Function |
|-------------|------------|----------|
| **VCC** | 5V | Power |
| **GND** | GND | Ground |
| **SCK** | GPIO 18 | SPI Clock |
| **SDI** | GPIO 23 | MOSI |
| **SDO** | GPIO 19 | MISO |
| **CS** | GPIO 5 | Chip Select |
| **INT** | GPIO 34 | Interrupt (optional) |

## Building & Uploading

```bash
cd firmware/tests/mcp2515_can
pio run --target upload
pio device monitor
```

## Expected Output

```text
 MCP2515 Diagnostic v7 (CANINTF Fix)
======================================
✅ MCP2515 Found!
CANSTAT: 0x80
Entering Normal Mode...
📤 Sending: Frame ID=0x123, Data=[0x01 0x02 0x03 0x04]
✅ TX OK
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| No response from MCP2515 | Check SPI wiring and CS pin |
| Garbage data | Verify baud rate and SPI frequency |
| CAN errors | Check termination resistor (120Ω) |

---

*Part of the `esp32-logic-analyzer-automation` firmware tests*
