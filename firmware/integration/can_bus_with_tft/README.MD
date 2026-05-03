
```markdown
##  CAN Bus + TFT Monitor (Modular Integration)

Real-time CAN 2.0A monitor using ESP32, MCP2515, and ST7735S (128x128) on a **shared SPI bus**. Built with clean architecture for maintainability & industrial prototyping.

### ️ Architecture
```
src/
├── config.h              # Pin mapping & system constants
├── mcp2515_driver.h/cpp  # Low-level SPI register access, frame parsing
├── tft_ui.h/cpp          # Adafruit GFX rendering, layout management
└── main.cpp              # Application orchestration (setup/loop, rate limiting)
```

### 🔌 Wiring (SPI Shared)
| ESP32 | MCP2515 | ST7735S TFT | Note |
|-------|---------|-------------|------|
| GPIO 18 (SCK) | SCK | SCL | Shared Clock |
| GPIO 23 (MOSI) | SI | SDA | Shared Data |
| GPIO 5 | CS | - | MCP Select |
| GPIO 17 | - | CS | TFT Select |
| GPIO 16 | - | DC | TFT Data/Command |
| GPIO 4 | - | RST | TFT Reset |
| 3.3V/5V | VCC | VCC | Power via LLC if needed |
| GND | GND | GND | **Common Ground Required** |

### 🛠️ How to Run
1. Connect Node TX (ESP32+MCP2515) to Node RX via CANH/CANL + 120Ω termination
2. Flash `firmware/integration/can_bus_with_tft/` to Node RX
3. Open Serial Monitor (115200) → verify `✅ System Ready`
4. TFT displays: CAN ID, Data (4 bytes), RX Count, Bus Status

### 📊 Output Example
```
[RX] ID:0x021 DLC:8 | Count:2
[RX] ID:0x123 DLC:8 | Count:3
⚠️ CAN Bus Error! EFLG: 0xC0
```
```

---