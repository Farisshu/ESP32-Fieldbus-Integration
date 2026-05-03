# 🧪 Test Procedures & Validation SOP

## 1. Hardware Setup
- ESP32 Node RX: MCP2515 + ST7735S (SPI shared)
- ESP32 Node TX: MCP2515 (loopback or physical bus)
- CANH/CANL terminated with 120Ω at both ends
- All GNDs common

## 2. Flash & Verify
1. Upload `firmware/integration/can_bus_with_tft/` to Node RX
2. Open Serial Monitor (115200) → expect `✅ System Ready`
3. TFT shows: ID, DATA (4 bytes), COUNT, STATUS

## 3. Validation Criteria
- ✅ No SPI bus conflicts (CS arbitration stable)
- ✅ UI refresh ≤ 5Hz (no flicker)
- ✅ CAN EFLG remains 0x00 during 10min stress test
- ✅ LittleFS logs CSV without blocking CAN polling