# Combined TFT + MCP2515 SPI Test 🔬

This directory contains a test project for validating **shared SPI bus** operation between the ST7735S TFT display and MCP2515 CAN controller on ESP32.

## Purpose

This test verifies:
- Multiple devices sharing the same SPI bus (SCK, MOSI, MISO)
- Independent Chip Select (CS) control for each device
- Sequential communication without interference
- Real-world scenario: Display showing CAN bus status

## Hardware Requirements

| Component | Quantity | Specification |
|-----------|----------|---------------|
| ESP32 Dev Board | 1 | DOIT ESP32 DEVKIT V1 or equivalent |
| ST7735S TFT Module | 1 | 1.44" 128x128 IPS Display |
| MCP2515 Module | 1 | TJA1050 transceiver |
| Jumper Wires | - | For SPI connections |

### Pinout Configuration (Shared SPI)

| Signal | ESP32 GPIO | MCP2515 | ST7735S TFT |
|--------|------------|---------|-------------|
| **SCK** | GPIO 18 | SCK | SCK/CLK |
| **MOSI** | GPIO 23 | SDI | SDA/MOSI |
| **MISO** | GPIO 19 | SDO | - |
| **CS** | GPIO 5 | CS | - |
| **CS** | GPIO 17 | - | CS |
| **DC** | GPIO 16 | - | DC/RS |
| **RST** | GPIO 4 | - | RST |
| **VCC** | 5V | VCC | VCC |
| **GND** | GND | GND | GND |

> ⚠️ **Important**: Both devices share SCK, MOSI, and MISO lines. Each device must have its own dedicated CS pin.

## Building & Uploading

```bash
cd firmware/tests/tft_mcp2515_combined
pio run --target upload
pio device monitor
```

## Expected Output

### Serial Monitor:
```text
🔍 Combined Unit Test: TFT + MCP2515 (SPI Shared)
===============================================
1. Initialize TFT... ✅ OK
2. Init Shared SPI (SCK:18, MISO:19, MOSI:23)... ✅ OK
3. Read MCP2515 CANSTAT... 0x80
✅ SPI Bus Sharing Test Passed!
```

### Display Output:
The TFT screen will show:
- Test status messages
- MCP2515 register values
- Confirmation of successful SPI sharing

## How It Works

1. **TFT Initialization**: Display is initialized first using its dedicated CS pin
2. **SPI Bus Setup**: Hardware SPI is configured for shared operation
3. **MCP2515 Read**: MCP2515 CANSTAT register is read using its CS pin
4. **Result Display**: Results are shown on the TFT screen

## Key Considerations

### SPI Bus Sharing Rules:
1. **One Master**: ESP32 is the SPI master
2. **Multiple Slaves**: Each slave device has its own CS pin
3. **CS Management**: Only one CS should be LOW at a time
4. **Idle State**: All CS pins should be HIGH when not in use

### Code Best Practices:
```cpp
// Before SPI transaction
digitalWrite(MCP_CS, LOW);   // Select MCP2515
digitalWrite(TFT_CS, HIGH);  // Deselect TFT

// Perform SPI transfer
uint8_t data = SPI.transfer(command);

// After SPI transaction
digitalWrite(MCP_CS, HIGH);  // Deselect MCP2515
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| One device not responding | Check individual CS pin wiring |
| Data corruption | Ensure only one CS is active at a time |
| Display flickering | Add small delay after CS switching |
| MCP2515 read fails | Verify SPI frequency (try 250kHz) |

---

*Part of the `esp32-logic-analyzer-automation` firmware tests*
