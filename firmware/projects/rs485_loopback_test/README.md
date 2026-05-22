# RS485 Loopback Test 🔁

This directory contains a test project for hardware validation of the **XY-017 RS485 to TTL module** using loopback testing.

## Purpose

This test verifies:
- RS485 module hardware integrity
- DE/RE pin control functionality
- Data transmission and reception on the same node
- Proper wiring configuration

## Hardware Requirements

| Component | Quantity | Specification |
|-----------|----------|---------------|
| ESP32 Dev Board | 1 | DOIT ESP32 DEVKIT V1 or equivalent |
| RS485 Module | 1 | XY-017 RS485 to TTL (Maxim MAX485 chip) |
| Jumper Wires | - | For GPIO and terminal connections |

### Pinout Configuration

| XY-017 Pin | ESP32 GPIO | Function |
|------------|------------|----------|
| **VCC** | 5V | Power |
| **GND** | GND | Ground |
| **RO** | GPIO 16 | Receiver Output (RX) |
| **DI** | GPIO 17 | Driver Input (TX) |
| **DE/RE** | GPIO 4 | Direction Enable |
| **A** | Terminal A | Non-inverting (+) |
| **B** | Terminal B | Inverting (-) |

### Loopback Setup

⚠️ **Critical**: Connect a short jumper wire between terminal **A** and terminal **B** on the RS485 module.

```
XY-017 Module:
[A] ────┐
        ├──── Jumper Wire
[B] ────┘
```

## Building & Uploading

```bash
cd firmware/projects/rs485_loopback_test
pio run --target upload
pio device monitor
```

## Expected Output

```text
========================================
🚀 RS485 Loopback Test Started
========================================
✅ RS485 Module Initialized
📌 Ensure A and B terminals are jumpered together!
========================================

📤 Transmitting: LOOPBACK_TEST_12345
📥 Received:      LOOPBACK_TEST_12345
✅ SUCCESS: Data integrity verified!
```

The test will continuously send data and verify that it receives the exact same data back.

## How It Works

1. **Transmit Mode**: DE/RE pin set HIGH, data sent via DI pin
2. **Loopback**: Data travels through A-B jumper back to RO pin
3. **Receive Mode**: DE/RE pin set LOW, data received via RO pin
4. **Verification**: Transmitted and received data are compared

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| No data received | Missing A-B jumper | Connect A and B terminals together |
| Garbage characters | Baud rate mismatch | Verify `monitor_speed = 115200` |
| Intermittent success | Unstable power | Use stable 5V power supply |
| Constant failure | Wrong GPIO pins | Check pinout configuration |

---

*Part of the `esp32-logic-analyzer-automation` RS485 project*
*For detailed documentation, see `../README_RS485.md`*
