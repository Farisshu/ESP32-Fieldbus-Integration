# 📡 RS485 Industrial Communication Module

This project implements industrial-grade serial communication using the **RS485 half-duplex standard** with the **XY-017 RS485 to TTL module**. It demonstrates robust point-to-point and multi-drop network capabilities suitable for industrial automation and IoT applications.

---

## 🎯 Project Objectives

1.  **Hardware Validation**: Verify XY-017 module integrity via loopback testing.
2.  **Point-to-Point Communication**: Establish reliable Master-Slave data exchange between two ESP32 nodes.
3.  **Industrial Protocol Implementation**: Demonstrate proper DE/RE pin control, error handling, and data framing.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Specification |
|-----------|----------|---------------|
| ESP32 Dev Board | 2 | DOIT ESP32 DEVKIT V1 or equivalent |
| RS485 Module | 2 | XY-017 RS485 to TTL (Maxim MAX485 chip) |
| Jumper Wires | - | For GPIO connections |
| Terminal Block Wire | - | For RS485 A/B differential pair |

### 🔌 Pinout Configuration

| XY-017 Pin | ESP32 GPIO | Function | Description |
|------------|------------|----------|-------------|
| **VCC** | 5V (or 3.3V*) | Power | *Check module jumper setting |
| **GND** | GND | Ground | **CRITICAL: Common ground required** |
| **RO** | GPIO 16 | RX2 | Receiver Output |
| **DI** | GPIO 17 | TX2 | Driver Input |
| **DE/RE** | GPIO 4 | Control | Direction Enable (High=Tx, Low=Rx) |
| **A** | Terminal A | Non-inverting | Differential Pair (+) |
| **B** | Terminal B | Inverting | Differential Pair (-) |

> ⚠️ **Wiring Note**: For multi-node networks, connect all **A** terminals together and all **B** terminals together. Always ensure a common **GND** connection between all nodes.

---

## 📂 Project Structure

```text
firmware/projects/
├── rs485_loopback_test/       # Test 1: Hardware Self-Validation
│   ├── src/
│   │   └── main.cpp           # Loopback firmware
│   └── platformio.ini
│
├── rs485_master_slave/        # Test 2: Two-Node Communication
│   ├── src/
│   │   ├── master.cpp         # Master node firmware
│   │   └── slave.cpp          # Slave node firmware
│   └── platformio.ini
│
└── README_RS485.md            # This documentation
```

---

## 🧪 Testing Procedures

### Test 1: Loopback Hardware Validation

**Objective**: Confirm the RS485 module can transmit and receive data correctly.

**Setup**:
1.  Connect a short jumper wire between terminal **A** and terminal **B** on the XY-017 module.
2.  Upload `rs485_loopback_test` firmware to a single ESP32.

**Expected Output**:
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

**Troubleshooting**:
-   If no data is received, check the A-B jumper connection.
-   Garbage data indicates baud rate mismatch or unstable power supply.

---

### Test 2: Master-Slave Point-to-Point Communication

**Objective**: Validate bidirectional communication between two independent ESP32 nodes.

**Setup**:
1.  Remove the A-B jumper from both modules.
2.  Connect **A ↔ A**, **B ↔ B**, and **GND ↔ GND** between the two modules.
3.  Upload `master.cpp` to ESP32 #1 (select environment `rs485_master`).
4.  Upload `slave.cpp` to ESP32 #2 (select environment `rs485_slave`).

**Expected Output (Master Side)**:
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

**Expected Output (Slave Side)**:
```text
========================================
👤 RS485 SLAVE Node Started
========================================
✅ Slave Ready - Listening for commands...
========================================

📥 Slave Received: CMD:READ_DATA|TS:54321
📤 Slave Responding: ACK|TEMP:25.5|HUM:62.0|STAT:OK
```

---

## 🔍 Troubleshooting Guide

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| **No Response** | Missing GND connection | Ensure common ground between nodes |
| **Garbage Characters** | Baud rate mismatch | Verify `BAUD_RATE` matches in all nodes |
| **Partial Data** | DE/RE timing too fast | Increase `delayMicroseconds(100)` after mode switch |
| **Intermittent Errors** | Long cable without termination | Add 120Ω resistor across A-B at the furthest node |
| **Constant Collision** | Both nodes transmitting | Verify only Master initiates communication |

---

## 🏗️ Architecture Highlights

### Half-Duplex Control
The XY-017 module uses a single GPIO (`DE/RE`) to toggle between Transmit and Receive modes:
-   **HIGH**: Driver Enabled (Transmit Mode)
-   **LOW**: Receiver Enabled (Receive Mode)

Proper timing (100µs delay) is implemented to prevent data truncation during switching.

### Command Protocol
A simple text-based protocol is used for demonstration:
-   **Request**: `CMD:<COMMAND>|<PARAMETERS>`
-   **Success Response**: `ACK|<DATA>|STAT:OK`
-   **Error Response**: `NACK|ERR:<ERROR_CODE>`

This structure is easily extensible for binary protocols like Modbus RTU.

---

## 🚀 Future Enhancements

-   [ ] **Modbus RTU Implementation**: Industry-standard register mapping.
-   [ ] **Multi-Drop Network**: Support for 1 Master + up to 32 Slaves with address filtering.
-   [ ] **CRC Validation**: Add Cyclic Redundancy Check for data integrity.
-   [ ] **Non-Blocking Driver**: Refactor to use interrupts or FreeRTOS tasks for scalable design.

---

## 📄 License

Part of the `esp32-logic-analyzer-automation` portfolio project.
