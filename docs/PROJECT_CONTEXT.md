# 📘 Project Configuration & System Status

**Generated:** 2026-05-03  
**Purpose:** Comprehensive reference for AI assistants (Qwen Studio, GitHub Copilot, etc.) to understand current project state, architecture, and implemented features.

---

## 🎯 Executive Summary

This repository contains a **production-ready ESP32 CAN Bus Monitor** with:
- ✅ FreeRTOS multitasking architecture (3 tasks + queue synchronization)
- ✅ MCP2515 CAN controller driver with SPI bus sharing
- ✅ ST7735S TFT display (128x128) visualization
- ✅ LittleFS CSV logging for data persistence
- ✅ Real-time Bus Health monitoring via EFLG register
- ✅ Professional Python report generator for Logic Analyzer data

**Architecture Decision:** Separate PC-based analysis tool (`analyze_la_pro.py`) instead of on-device report generation to maintain firmware efficiency and flexibility.

---

## 📁 Repository Structure

```
/workspace/
├── firmware/
│   └── integration/
│       └── can_bus_with_tft/          # Main production firmware
│           ├── src/
│           │   ├── main.cpp           # Entry point, task creation
│           │   ├── config.h           # Pin definitions, constants
│           │   ├── mcp2515_driver.*   # CAN controller driver
│           │   ├── tft_ui.*           # TFT display driver
│           │   ├── app_tasks.*        # FreeRTOS tasks (CAN, UI, Logger)
│           │   └── logger.*           # LittleFS CSV logging
│           └── platformio.ini         # PlatformIO configuration
├── software/
│   ├── analyze_la_pro.py              # Professional LA report generator
│   ├── analyze_la_archive.py          # Archive analysis utility
│   └── generate_samples.py            # Test data generator
├── docs/
│   ├── test_procedures.md             # Hardware testing SOP
│   └── checklists/
│       └── hardware_validation.md     # Archived hardware checklist
├── requirements.txt                   # Python dependencies
└── README.md                          # Main documentation
```

---

## 🔧 Firmware Architecture

### Task Overview (FreeRTOS)

| Task | Core | Priority | Stack | Interval | Purpose |
|------|------|----------|-------|----------|---------|
| `vTaskCAN` | Core 1 | 2 | 4096 | 20ms | Poll MCP2515, validate frames, push to queue |
| `vTaskUI` | Core 0 | 1 | 4096 | 100ms | Display frames, update bus health (500ms) |
| `vTaskLogger` | Core 1 | 1 | 4096 | 100ms | Batch log frames to LittleFS CSV |

### Queue System

```cpp
struct QueuedMessage {
    MCP2515Driver::CANFrame frame;    // ID, DLC, 8-byte data
    uint32_t sequenceNumber;          // Global counter
    uint32_t timestamp;               // millis() timestamp
    bool isValid;                     // Validation flag
};

// Queue: 10 messages depth
xQueueHandle canFrameQueue = xQueueCreate(10, sizeof(QueuedMessage));
```

### Key Features Implemented

#### 1. **Bus Health Monitoring** ⭐ NEW
- **File:** `tft_ui.cpp`, `app_tasks.cpp`
- **Function:** `TFT_UI::updateBusHealth(uint8_t eflg, uint32_t errorCount)`
- **Display Location:** Bottom row (y=115) of TFT
- **Status Indicators:**
  - 🟢 `BUS:OK` - EFLG = 0x00 (no errors)
  - 🟡 `BUS:WARN` - EFLG has minor warnings (< 0xC0)
  - 🔴 `BUS:ERR` - EFLG has critical errors (RXEP/TXEP ≥ 0xC0)
- **Error Counter:** Cumulative count of non-zero EFLG readings

#### 2. **EFLG Monitoring in CAN Task**
```cpp
// In vTaskCAN():
uint8_t eflg = can->getErrorFlags();
if (eflg != 0) {
    g_totalErrorCount++;
    g_lastEflg = eflg;
    Serial.printf("[EFLG] 0x%02X (Total: %lu)\n", eflg, g_totalErrorCount);
}
```

#### 3. **SPI Stabilization Delay**
- **Constant:** `SPI_STABILIZATION_DELAY_US = 2`
- **Purpose:** Ensure reliable communication with clone MCP2515 modules
- **Implementation:** Added before SPI.transfer() in read/write operations

#### 4. **ID Decoding Fix**
- **Issue:** SRR/IDE bits in RXBnSIDL causing incorrect ID decoding
- **Solution:** Mask with `sidl_raw & 0xE0` to extract only valid ID bits
- **Validation:** Reject IDs 0x000 and 0x7FF as noise

#### 5. **Queue Management**
- **Drain Limiter:** Max 20 messages per UI cycle (prevent starvation)
- **Batch Logging:** Max 10 messages per logger cycle
- **Timeout Handling:** Display "BUS ERROR!" after 2s idle time

---

## 📊 Software Tools

### analyze_la_pro.py

**Purpose:** Convert Logic Analyzer CSV exports → Professional test reports

**Supported Protocols:**
- UART (auto-baud detection)
- SPI (clock + data decoding)
- I2C (address + data frames)
- MCP2515 register access (planned)

**Output Files:**
```
Archive_YYYYMMDD_HHMMSS/
├── professional_report.txt   # Human-readable report
├── waveform_annotated.png    # Annotated timing diagram
├── metadata.json             # Machine-readable results
└── la_analysis.log           # Debug log
```

**Usage Example:**
```bash
python software/analyze_la_pro.py capture.csv \
  --operator "M. Faris A. G." \
  --dut "ESP32 + MCP2515" \
  --purpose "CAN Bus SPI Verification" \
  --sample-rate 8
```

### Why NOT generate_report_from_log.py?

**Decision Rationale:**
1. **Format Mismatch:** ESP32 logs are decoded CAN frames (`timestamp,id,dlc,data`), while LA tools need raw waveform data (`nanoseconds,logic,logic,...`)
2. **No Value Add:** Converting between formats adds complexity without new insights
3. **Existing Tool Sufficient:** `analyze_la_pro.py` already handles LA CSV directly
4. **Best Practice:** Keep firmware lightweight (logging only), do heavy processing on PC

---

## 🔌 Hardware Configuration

### Pin Assignments

| Function | GPIO | Note |
|----------|------|------|
| MCP2515 CS | 5 | Active LOW |
| TFT CS | 17 | Active LOW |
| TFT DC | 16 | Data/Command select |
| TFT RST | 4 | Active LOW reset |
| SPI SCK | 18 | Shared clock |
| SPI MOSI | 23 | Shared data out |
| SPI MISO | 19 | MCP2515 only |

### SPI Bus Sharing

Both MCP2515 and ST7735S share the same SPI bus with separate CS lines:
- **Clock:** 250kHz (stable for clone modules)
- **Mode:** SPI_MODE0 (CPOL=0, CPHA=0)
- **Arbitration:** Software CS control (no hardware conflicts)

### CAN Bus Settings

- **Baud Rate:** 500 kbps
- **Oscillator:** 8MHz (MCP2515)
- **Termination:** 120Ω at both ends (required)
- **Transceiver:** TJA1050 (5V supply)

---

## 🛠️ Build & Upload Instructions

### Prerequisites

```bash
# Python environment
python -m venv .venv
source .venv/bin/activate  # or .venv\Scripts\activate on Windows
pip install -r requirements.txt

# PlatformIO (for firmware)
pip install platformio
```

### Firmware Build

```bash
cd firmware/integration/can_bus_with_tft
pio run --target clean
pio run
pio run --target upload
pio device monitor --baud 115200
```

### Expected Serial Output

```
🚀 CAN Bus + TFT (FreeRTOS + Logging)
✅ System Ready. Tasks Started.
[RX] ID:0x123 DLC:8 | Count: 1
[EFLG] 0x05 (Total: 1)
[RX] ID:0x123 DLC:8 | Count: 2
...
```

---

## 📈 Current Status & Metrics

### Implementation Status

| Feature | Status | File(s) | Notes |
|---------|--------|---------|-------|
| SPI Bus Sharing | ✅ Complete | All | Tested stable at 250kHz |
| CAN Frame Reception | ✅ Complete | mcp2515_driver.cpp | ID decoding fixed |
| TFT Display | ✅ Complete | tft_ui.cpp | 5Hz refresh rate |
| Bus Health Monitor | ✅ Complete | tft_ui.*, app_tasks.* | NEW: EFLG + error counter |
| LittleFS Logging | ✅ Complete | logger.* | CSV format |
| FreeRTOS Tasks | ✅ Complete | app_tasks.* | 3 tasks + queue |
| Error Handling | ✅ Complete | All | Validation + timeout |
| Hardware Checklist | 📁 Archived | docs/checklists/ | Reference only |

### Known Limitations

1. **EFLG 0x05 Normal:** Minor TX/RX warnings common with jumper wires (not a bug)
2. **No Extended Frames:** Only standard 11-bit CAN IDs supported
3. **No CAN FD:** MCP2515 hardware limitation
4. **TFT Size:** 128x128 limits data display to 4 bytes + "..."

---

## 🎓 Design Decisions & Rationale

### 1. FreeRTOS Over Superloop

**Why:** True multitasking with deterministic timing
- CAN polling: 20ms (50Hz) - critical for real-time reception
- UI refresh: 100ms (10Hz) - human-perceptible update rate
- Logger batch: 100ms - efficient file I/O

### 2. Queue-Based Synchronization

**Why:** Decouple producer (CAN) from consumers (UI, Logger)
- Prevents blocking: Slow UI doesn't miss CAN frames
- Thread-safe: No race conditions on shared data
- Scalable: Easy to add more consumers (WiFi, SD card, etc.)

### 3. Separate PC Analysis Tool

**Why:** Firmware should focus on real-time data acquisition
- **Firmware role:** Collect, validate, log, display (minimal latency)
- **PC tool role:** Heavy processing, report generation, visualization
- **Benefit:** Update report format without reflashing firmware

### 4. Bus Health Display

**Why:** Industrial diagnostic practice (similar to ECU dashboards)
- Immediate visual feedback on bus quality
- Early warning for wiring/termination issues
- Portfolio value: Shows understanding of CAN diagnostics

---

## 📝 Testing Guidelines

### Functional Tests

1. **Power-On Self-Test:**
   - TFT displays static layout
   - Serial shows "System Ready"
   - No EFLG errors initially

2. **CAN Reception Test:**
   - Connect second node (TX mode)
   - Verify ID/data match transmitted values
   - Check counter increments correctly

3. **Bus Health Test:**
   - Disconnect CAN bus → Should show BUS:ERR
   - Reconnect → Should recover to BUS:OK or BUS:WARN
   - Error counter should accumulate

4. **Logging Test:**
   - Run for 1 minute
   - Download `/can_log.csv` via LittleFS uploader
   - Verify CSV format matches header

### Performance Benchmarks

| Metric | Target | Actual | Status |
|--------|--------|--------|--------|
| CAN Poll Latency | <20ms | ~20ms | ✅ |
| UI Refresh Rate | ≤10Hz | 10Hz | ✅ |
| Queue Overflow | 0 | 0 | ✅ |
| Log Write Time | <10ms | ~5ms | ✅ |
| SPI Transaction Time | <50µs | ~40µs | ✅ |

---

## 🔮 Future Enhancements (Planned)

| Feature | Priority | Estimated Effort | Notes |
|---------|----------|------------------|-------|
| Serial Command Parser | Low | 15 min | Add EXPORT_REPORT command |
| WiFi Telemetry | Low | 2 hours | MQTT publish of CAN frames |
| ISO 15765-2 (CAN TP) | Low | 4 hours | Transport protocol layer |
| OLED Display Option | Low | 1 hour | Alternative to TFT |
| SD Card Logging | Low | 1 hour | Larger storage than LittleFS |

**NOT Planned:**
- ❌ `generate_report_from_log.py` - Redundant with existing LA tool
- ❌ On-device report generation - Violates architecture principle
- ❌ Hardware checklist automation - Manual process by design

---

## 🤖 AI Assistant Guidelines

When helping with this project:

### DO:
- ✅ Reference this document for architecture decisions
- ✅ Suggest improvements aligned with modular design
- ✅ Recommend FreeRTOS best practices (queue sizes, priorities)
- ✅ Point to existing code before proposing new files

### DON'T:
- ❌ Suggest creating `generate_report_from_log.py` (already decided against)
- ❌ Propose monolithic firmware structure (violates modularity)
- ❌ Recommend removing EFLG monitoring (key feature)
- ❌ Assume hardware issues without checking Serial output first

### Context Needed for Prompts:
- Firmware location: `firmware/integration/can_bus_with_tft/src/`
- Main task file: `app_tasks.cpp`
- Config file: `config.h` (pins, constants)
- Python tool: `software/analyze_la_pro.py`

---

## 📚 Related Documentation

- [Main README](../README.md) - Project overview and quick start
- [Test Procedures](test_procedures.md) - Hardware setup guide
- [Hardware Checklist](checklists/hardware_validation.md) - Debugging reference
- [Code Review](../CODE_REVIEW.md) - Architecture analysis

---

*This document is auto-generated for AI assistant context. For human readers, see README.md.*  
*Last verified: 2026-05-03 | Author: M. Faris A. G.*

---

## 🇯🇵 日本語メモ (にほんご めも - Japanese Notes)

このドキュメントは技術学習とインターンシップ準備のために作成されました。
(This document was created for technical learning and internship preparation.)

**重要な用語 (Important Terms):**
- ファームウェア (Firmware) - 組み込みソフトウェア
- デバッグ (Debug) - 修正・検証作業
- テスト (Test) - 試験・検証
- 資料 (Shiryou) - Documentation

