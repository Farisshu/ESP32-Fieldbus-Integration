# RS485 Master-Slave Communication 🤖👤

This directory contains a test project for point-to-point communication between two ESP32 nodes using the **RS485 half-duplex standard**.

## Purpose

This test verifies:
- Bidirectional RS485 communication between two independent nodes
- Master-Slave protocol implementation
- DE/RE pin control for half-duplex operation
- Command-response data exchange pattern

## Hardware Requirements

| Component | Quantity | Specification |
|-----------|----------|---------------|
| ESP32 Dev Board | 2 | DOIT ESP32 DEVKIT V1 or equivalent |
| RS485 Module | 2 | XY-017 RS485 to TTL (Maxim MAX485 chip) |
| Jumper Wires | - | For GPIO and differential pair connections |

### Pinout Configuration (Both Nodes)

| XY-017 Pin | ESP32 GPIO | Function |
|------------|------------|----------|
| **VCC** | 5V | Power |
| **GND** | GND | Ground (**CRITICAL**: Common ground required) |
| **RO** | GPIO 16 | Receiver Output (RX) |
| **DI** | GPIO 17 | Driver Input (TX) |
| **DE/RE** | GPIO 4 | Direction Enable (High=Tx, Low=Rx) |
| **A** | Terminal A | Non-inverting (+) |
| **B** | Terminal B | Inverting (-) |

### Network Wiring

```
Master Node                    Slave Node
┌─────────────┐               ┌─────────────┐
│  XY-017     │               │  XY-017     │
│  [A] ───────┼───────────────┼── [A]       │
│  [B] ───────┼───────────────┼── [B]       │
│  [GND] ─────┼───────────────┼── [GND]     │
└─────────────┘               └─────────────┘
```

> ⚠️ **Important**: 
> - Remove any A-B jumpers used in loopback testing
> - Connect A↔A, B↔B, and GND↔GND between modules
> - Common ground is essential for reliable communication

## Building & Uploading

### Upload to Master Node (ESP32 #1):
```bash
cd firmware/projects/rs485_master_slave
pio run --target upload --environment rs485_master
pio device monitor
```

### Upload to Slave Node (ESP32 #2):
```bash
cd firmware/projects/rs485_master_slave
pio run --target upload --environment rs485_slave
pio device monitor
```

> 💡 **Tip**: Use two terminal windows to monitor both nodes simultaneously.

## Expected Output

### Master Side:
```text
========================================
🤖 RS485 MASTER Node Started
========================================
✅ Master Ready - Sending commands every 3 seconds
========================================

📤 Master Sending: CMD:READ_DATA|TS:54321
📥 Master Received: ACK|TEMP:25.5|HUM:62.0|STAT:OK
✅ Valid Response from Slave
```

### Slave Side:
```text
========================================
👤 RS485 SLAVE Node Started
========================================
✅ Slave Ready - Listening for commands...
========================================

📥 Slave Received: CMD:READ_DATA|TS:54321
📤 Slave Responding: ACK|TEMP:25.5|HUM:62.0|STAT:OK
```

## Communication Protocol

### Request Format (Master → Slave):
```
CMD:<COMMAND>|<PARAMETERS>
```
Example: `CMD:READ_DATA|TS:54321`

### Response Format (Slave → Master):
```
Success: ACK|<DATA>|STAT:OK
Error:   NACK|ERR:<ERROR_CODE>
```
Example: `ACK|TEMP:25.5|HUM:62.0|STAT:OK`

## How It Works

1. **Master Initiates**: Master sends command with timestamp
2. **Slave Receives**: Slave listens and parses incoming command
3. **Slave Processes**: Slave generates response (simulated sensor data)
4. **Slave Responds**: Slave sends acknowledgment with data
5. **Master Validates**: Master verifies response integrity

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| No response from slave | Missing GND connection | Ensure common ground between nodes |
| Garbage characters | Baud rate mismatch | Verify 115200 baud on both nodes |
| Partial data received | DE/RE timing too fast | Increase delay after mode switch |
| Intermittent errors | Long cable without termination | Add 120Ω resistor across A-B at furthest node |
| Both transmitting | Protocol violation | Verify only master initiates communication |

## Architecture Highlights

### Half-Duplex Control
The XY-017 module uses a single GPIO (`DE/RE`) to toggle between modes:
- **HIGH**: Driver Enabled (Transmit Mode)
- **LOW**: Receiver Enabled (Receive Mode)

A 100µs delay is implemented after switching to prevent data truncation.

### Timing Diagram
```
Master: [TX: CMD] ──────┐
                        ├── Bus Active
Slave:  [RX: CMD] ──────┘
        [Processing...]
Slave:  [TX: ACK] ──────┐
                        ├── Bus Active
Master: [RX: ACK] ──────┘
```

---

*Part of the `esp32-logic-analyzer-automation` RS485 project*
*For detailed documentation, see `../README_RS485.md`*
