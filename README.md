# ESP32 Logic Analyzer Automation & CAN Bus Monitor 🛠️📊📡

> **Professional Embedded System Prototype**: Capture → Auto-Detect → Professional Report Generation  
> **Industrial-Grade CAN 2.0A Communication** with TFT Visualization, FreeRTOS Multitasking, and Automated Testing  
> Standardized for R&D, Industrial Protocols (SPI, UART, I2C, CAN), and Internship Documentation.
> 
> **Learning Goals**: Embedded Systems, Automotive Networks, Monozukuri (Manufacturing Excellence)

![Python](https://img.shields.io/badge/Python-3.8+-blue?logo=python)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![ESP32](https://img.shields.io/badge/Platform-ESP32-blue)
![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green)
![CAN Bus](https://img.shields.io/badge/Protocol-CAN%202.0A-red)

---

## 🎯 Project Goal

Automate logic analyzer data processing to eliminate manual screenshot/reporting. Transform raw CSV exports into **structured, auditable test reports** with protocol decoding, timing validation, and auto-generated conclusions.

**Key Engineering Highlights:**
- ✅ **Modular Architecture**: Driver/UI/App separation (R&D standard)
- ✅ **FreeRTOS Integration**: Multi-task architecture with queue-based synchronization
- ✅ **SPI Bus Sharing**: MCP2515 + ST7735S on same bus with CS arbitration
- ✅ **Defensive Programming**: ID filtering, DLC validation, error monitoring
- ✅ **Professional Reporting**: Auto-generated test reports via `analyze_la_pro.py`

---

## 🚀 Quick Start

### 1. Setup Environment

```bash
git clone https://github.com/Farisshu/esp32-logic-analyzer-automation.git
cd esp32-logic-analyzer-automation
python -m venv .venv
# Windows:
.venv\Scripts\activate
# Linux/Mac:
source .venv/bin/activate
pip install -r requirements.txt
```

### 2. Run Analysis

```bash
# Basic (auto-detect protocols)
python software/analyze_la_pro.py capture.csv

# With metadata (recommended for reports)
python software/analyze_la_pro.py capture.csv \
  --operator "Your Name" \
  --dut "ESP32 + MCP2515" \
  --purpose "CAN Bus SPI Verification" \
  --sample-rate 8
```

### 3. Output Structure

```text
Archive_YYYYMMDD_HHMMSS/
├── professional_report.txt   # Full metadata, results, validation & conclusions
├── waveform_annotated.png    # Annotated plot with edge markers & timing labels
├── metadata.json             # Machine-readable results for CI/CD or databases
└── la_analysis.log           # Execution trace & debugging info
```

---

## 📁 Repository Structure

```text
esp32-logic-analyzer-automation/
├── firmware/                 # ESP32 test codes (PlatformIO)
│   ├── tests/               # Unit tests for individual modules
│   │   ├── mcp2515_can/     # MCP2515 CAN controller tests
│   │   ├── st7735s_tft/     # ST7735S TFT display tests
│   │   └── tft_mcp2515_combined/ # Combined SPI device tests
│   ├── integration/         # Multi-node communication tests
│   │   ├── can_two_nodes/   # Two-node CAN bus communication
│   │   └── can_bus_with_tft/ # CAN monitor with TFT visualization
│   ├── test_uart_basic/     # UART communication test
│   └── test_can_spi_test/   # CAN/MCP2515 SPI test
├── software/                 # Python analysis tools
│   ├── analyze_la_pro.py     # Main auto-report generator
│   ├── analyze_la_archive.py # Archive analysis utility
│   ├── generate_samples.py   # Synthetic CSV generator for testing
│   └── examples/             # Sample captures & test data
├── include/                  # Project header files
├── lib/                      # Private libraries
├── docs/                     # Wiring diagrams, PulseView settings, SOP
├── archives/                 # [IGNORED] Generated reports
├── requirements.txt          # Python dependencies
├── LICENSE                   # MIT License
└── README.md                 # This file
```

---

## 🔧 How It Works

### Software Workflow (Logic Analyzer)

1. **Capture**: Export CSV from PulseView / Saleae / Generic LA
2. **Auto-Detect**: Script identifies UART, SPI, or I2C patterns from waveform timing
3. **Decode**: Extracts protocol frames, register accesses, or byte streams
4. **Validate**: Checks against spec thresholds (jitter, frequency, CRC, timing margins)
5. **Report**: Generates standardized TXT/PNG/JSON output ready for documentation

### Firmware Workflow (CAN Bus Monitor)

1. **Initialize**: MCP2515 CAN controller + ST7735S TFT on shared SPI bus
2. **FreeRTOS Tasks**:
   - `canPollTask`: Monitor CAN bus for incoming messages (10ms interval)
   - `uiRefreshTask`: Update TFT display (200ms interval, 5Hz refresh)
   - `loggerTask`: Log data to LittleFS CSV (non-blocking)
3. **Message Handling**: Filter by ID, validate DLC, extract payload
4. **Error Monitoring**: Track EFLG register for bus health diagnostics
5. **Visualization**: Display ID, Data (4 bytes), Count, Status on TFT

---

## 📊 Example Report Snippet

```text
══════════════════════════════════════════════════════════════════════
LOGIC ANALYZER PROFESSIONAL TEST REPORT
Generated by: analyze_la_pro.py v1.0.1
══════════════════════════════════════════════════════════════════════

📋 METADATA
  Date/Time        : 2026-05-03 13:09:47
  Operator         : Test User
  Device Under Test: ESP32 + MCP2515
  Test Purpose     : SPI CAN Controller Test
  Report Version   : 1.0.1

⚙️  MEASUREMENT SETUP
  Sample Rate      : 2.5 MHz
  Capture Duration : 0.079 ms
  Voltage Threshold: 1.65 V (TTL)
  Channels Active  : 8
  Protocols Detected: SPI

🔍 PROTOCOL ANALYSIS RESULTS
----------------------------------------------------------------------
  SPI
  Channel(s)       : ch0-ch1
  Status           : ACTIVE
  Total Transactions: 1

✅ CONCLUSIONS & RECOMMENDATIONS
----------------------------------------------------------------------
  ✅ PASS: All protocols decoded successfully without errors.
  ✅ Signal integrity appears stable.

  Next Steps:
    • Cross-verify with oscilloscope for analog characteristics
    • Capture longer duration for statistical analysis
    • Export raw CSV + this report to version control
```

---

## 🧪 Testing & Development Workflow

This repository follows a **modular R&D workflow** aligned with industrial automation standards:

| Stage | Folder | Purpose | Status |
|-------|--------|---------|--------|
| **Unit Test** | `firmware/tests/` | Verify individual modules (SPI, UART, TFT init) | ✅ MCP2515, ST7735S Verified |
| **Integration** | `firmware/integration/` | Multi-node communication (CAN bus, RS485 Modbus) | ✅ CAN Two Nodes Complete |
| **Project** | `firmware/projects/` | Final integrated applications | ⏳ Planned |

---

## 📦 Firmware Projects

| Project | Description | Status | Key Features |
|---------|-------------|--------|--------------|
| `test_uart_basic` | Basic UART communication test with SPI initialization | ✅ Complete | UART TX/RX, SPI init |
| `test_can_spi_test` | MCP2515 CAN controller SPI interface diagnostic | ✅ Complete | Register access, loopback |
| `integration/can_two_nodes` | Two-node CAN bus communication system | ✅ Complete | 500 kbps, TX/RX roles |
| `integration/can_bus_with_tft` | CAN monitor with TFT visualization & FreeRTOS | ✅ Complete | Multi-task, SPI sharing, LittleFS logging |

---

## 🔌 Hardware Setup

### Required Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 Dev Board | 2 | Main microcontroller (TX & RX nodes) |
| MCP2515 CAN Module | 2 | CAN 2.0A controller |
| TJA1050 CAN Transceiver | 2 | CAN bus physical layer |
| ST7735S TFT (128x128) | 1 | Display module (optional) |
| 120Ω Resistor | 2 | CAN bus termination |
| Jumper Wires | - | Connections |

### Wiring Diagram (SPI Shared Bus)

| ESP32 | MCP2515 | ST7735S | Note |
|-------|---------|---------|------|
| GPIO 18 (SCK) | SCK | SCL | Shared Clock |
| GPIO 23 (MOSI) | SI | SDA | Shared Data |
| GPIO 19 (MISO) | SO | - | MCP2515 only |
| GPIO 5 | CS | - | MCP2515 Chip Select |
| GPIO 17 | - | CS | TFT Chip Select |
| GPIO 16 | - | DC | TFT Data/Command |
| GPIO 4 | - | RST | TFT Reset |
| GND | GND | GND | **Common Ground Required** |
| 3.3V | VCC | VCC | Power supply |

### CAN Bus Wiring

```text
Node TX (MCP2515)          Node RX (MCP2515)
     CANH ───────────────────── CANH
     CANL ───────────────────── CANL
              ┌─────────┐
              │ 120Ω    │  (Termination at BOTH ends)
              └─────────┘
```

---

## 🛠️ Build & Upload - ビルドとアップロード

### Firmware (PlatformIO) - ファームウェア

```bash
# Navigate to project directory - プロジェクトディレクトリへ移動
cd firmware/integration/can_bus_with_tft

# Clean build files - ビルドファイルをクリーン
pio run --target clean

# Build project - プロジェクトをビルド
pio run

# Upload to device - デバイスにアップロード
pio run --target upload

# Open serial monitor - シリアルモニターを開く
pio device monitor --baud 115200
```

### Software (Python Analysis) - ソフトウェア

```bash
# Activate virtual environment - 仮想環境をアクティブ
source .venv/bin/activate  # Linux/Mac
# or
.venv\Scripts\activate     # Windows

# Run analysis - 分析を実行
python software/analyze_la_pro.py software/examples/sample_uart_9600.csv \
  --operator "Your Name" \
  --dut "ESP32 Test Board" \
  --purpose "UART Protocol Verification"
```

---

## 📈 Test Results & Validation - テスト結果と検証

### CAN Bus Communication Test - CAN バス通信テスト

| Metric | Expected | Actual | Status | 項目 |
|--------|----------|--------|--------|
| SPI Bus Sharing | No conflicts | ✅ Stable | PASS |
| CAN Frame Transmission | TXREQ clear | ✅ 0x00 | PASS |
| CAN Frame Reception | Payload intact | ✅ 100% | PASS |
| Error Flag (EFLG) | < 0x80 | ✅ 0x05 (minor warning) | PASS* |
| UI Refresh Rate | ≤ 5Hz | ✅ 5Hz | PASS |
| FreeRTOS Task Sync | No deadlock | ✅ Stable | PASS |

*Note: EFLG 0x05 indicates minor transmit warning (normal for prototyping with jumper wires). See [Troubleshooting](#-troubleshooting) for details.

### Logic Analyzer Detection - ロジックアナライザー検出

| Protocol | Auto-Detection | Decoding | Validation |
|----------|---------------|----------|------------|
| UART | ✅ Yes | ✅ Baud, Data bits | ✅ Bit timing |
| SPI | ✅ Yes | ✅ Clock, Data | ✅ Frequency |
| I2C | ✅ Yes | ✅ Address, Data | ✅ Timing |
| CAN | 🔜 Planned | 🔜 Planned | 🔜 Planned |

---

## 🤝 Contribution & Internship Readiness - 貢献とインターンシップ準備

This toolkit is designed to be **modular and extensible**:

- Add new protocol decoders in `software/decoders/`
- Integrate with CI/CD for automated test validation
- Export reports directly to lab management systems
- Extend firmware tests for additional protocols

Built for embedded engineers who value **traceability, automation, and professional documentation**.

### Portfolio Skills Demonstrated - ポートフォリオで示したスキル

| Skill | Evidence | Industry Value | スキル |
|-------|----------|----------------|
| Embedded C/C++ | Modular driver, register-level MCP2515 access | ⭐⭐⭐⭐⭐ |
| RTOS (FreeRTOS) | 3-task architecture with queue sync | ⭐⭐⭐⭐⭐ |
| SPI/I2C/UART | Shared bus management, level shifting | ⭐⭐⭐⭐⭐ |
| Debugging | Logic Analyzer integration, EFLG monitoring | ⭐⭐⭐⭐⭐ |
| Documentation | Professional report generator, SOP, README | ⭐⭐⭐⭐⭐ |
| Version Control | Clean commit history, modular structure | ⭐⭐⭐⭐⭐ |

---

## 📄 License

MIT License. Free for educational, R&D, and commercial use.

---

## 🔧 Troubleshooting - トラブルシューティング

### Common Issues - 一般的な問題

| Issue | Cause | Solution | 問題 |
|-------|-------|----------|
| No protocol detected | CSV format mismatch | Check column names (time, ch0-ch7) |
| Poor waveform quality | Low sample rate | Increase to 8+ MS/s for SPI |
| CAN ID mismatch (0x123 vs 0x421) | ✅ FIXED: Added SPI stabilization delay | See `config.h` - `SPI_STABILIZATION_DELAY_US` |
| EFLG warning (0x05) | Minor bus noise | Use twisted pair cables, verify termination |
| TFT flicker | Refresh rate too high | Reduce to ≤5Hz in `uiRefreshTask` |
| Build fails | Missing PlatformIO | Run `pip install platformio` |
| Queue overflow | High message rate | ✅ FIXED: Batch logging + drain limiter implemented |

### EFLG Register Analysis

If you see `EFLG: 0x05` in serial output:

```text
Binary: 0000 0101
Bit 0 (EWARN)  = 1 ⚠️ Error Warning Limit reached (counter ≥ 96)
Bit 2 (TXWAR)  = 1 ⚠️ Transmit Error Warning
Bit 3-7        = 0 ✅ No Bus-Off, No Passive Error, No Overflow
```

**This is NORMAL for prototyping.** Causes:
- Long jumper wires (>15cm) → capacitance noise
- Missing/improper 120Ω termination
- Crystal tolerance on MCP2515 clone modules

**Solution for production:** Use shielded twisted pair cables, proper termination, and measure with oscilloscope.

---

## 📚 Additional Documentation - 追加資料

- [Test Procedures & SOP](docs/test_procedures.md) - Hardware setup & validation steps
- [Firmware README](firmware/README.md) - Detailed firmware project guide
- [Software README](software/README.md) - Python tool documentation
- [Integration Tests](firmware/integration/README.md) - Multi-node testing guide

---

## 🎓 Project Status & Roadmap - プロジェクト状況とロードマップ

### Current Status: **Production-Ready for R&D Demonstration** ✅ - 現状： **研究開発デモ準備完了**

### 📊 Interactive Progress Dashboard

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'primaryColor': '#4CAF50', 'edgeLabelBackground':'#ffffff', 'tertiaryColor': '#fff'}}}%%
gantt
    title ESP32 Logic Analyzer - Development Timeline
    dateFormat  YYYY-MM-DD
    axisFormat  %b %Y
    
    section Core Features
    SPI Bus Sharing (MCP2515 + TFT)     :done, spi, 2025-04-01, 2025-04-15
    CAN 2.0A Communication              :done, can, 2025-04-10, 2025-04-25
    FreeRTOS Multitasking               :done, rtos, 2025-04-15, 2025-04-28
    Logic Analyzer Auto-Report          :done, la, 2025-04-20, 2025-05-01
    
    section Stability Improvements
    LittleFS CSV Logging                :done, log, 2025-04-25, 2025-04-30
    ID Decoding Fix                     :done, idfix, 2025-04-28, 2025-05-01
    SPI Stabilization Delay             :done, spidelay, 2025-04-29, 2025-05-02
    Queue Management                    :done, queue, 2025-04-30, 2025-05-03
    Error Handling                      :done, err, 2025-05-01, 2025-05-03
    
    section Future Enhancements
    CAN TP (ISO 15765-2)                :active, cantp, 2025-05-10, 30d
    WiFi/Bluetooth Monitoring           :         wifi, after cantp, 20d
    Web Dashboard                       :         web, after wifi, 15d
    CI/CD Pipeline                      :         cicd, after web, 10d
```

### Feature Completion Overview - 機能完了概要

```mermaid
pie title Feature Completion Status
    "Completed (High Priority)" : 9
    "Completed (Medium Priority)" : 2
    "In Progress" : 1
    "Planned" : 4
```

### Detailed Status Table - 詳細ステータステーブル

| Feature | Status | Priority | Completion Date | 機能 |
|---------|--------|----------|-----------------|
| **Core Architecture** | | | |
| SPI Bus Sharing (MCP2515 + TFT) | ✅ Complete | High | 2025-04-15 |
| CAN 2.0A Communication | ✅ Complete | High | 2025-04-25 |
| FreeRTOS Multitasking (3 tasks) | ✅ Complete | High | 2025-04-28 |
| Logic Analyzer Auto-Report | ✅ Complete | High | 2025-05-01 |
| **Stability & Reliability** | | | |
| LittleFS CSV Logging | ✅ Complete | Medium | 2025-04-30 |
| ID Decoding Fix | ✅ Complete | High | 2025-05-01 |
| SPI Stabilization Delay | ✅ Complete | High | 2025-05-02 |
| Queue Management Optimization | ✅ Complete | High | 2025-05-03 |
| Error Handling Enhancement | ✅ Complete | Medium | 2025-05-03 |
| Configuration Centralization | ✅ Complete | Medium | 2025-05-03 |
| **Future Enhancements** | | | |
| CAN TP (ISO 15765-2) | ⏳ In Progress | Low | Planned Q2 2025 |
| WiFi/Bluetooth Monitoring | 🔜 Planned | Low | After CAN TP |
| Web-based Dashboard | 🔜 Planned | Low | After WiFi |
| Automated CI/CD Pipeline | 🔜 Planned | Medium | Final Phase |

---

*Developed for industrial automation workflows & internship preparation (Automotive R&D)*  
*Author: M. Faris A. G. | Version: 1.0.2 | Last Updated: 2026-05-03 (All Issues Resolved)*

---

## 🇯🇵 Japanese Learning Resources

> 📚 **Comprehensive Japanese learning materials are available in the `materi/` directory:**
> - [日本語技術学習ガイド](materi/JAPANESE_LEARNING_GUIDE.md) - Japanese Technical Learning Guide (with furigana)
> - [Kamus Teknis Jepang](materi/KAMUS_TEKNIS_JEPANG.md) - Japanese Technical Dictionary (Indonesian)
> - [README Materi Lengkap](materi/README_MATERI_LENGKAP.md) - Complete Learning Materials (Indonesian)

### Quick Reference: Key Technical Terms

| English | 日本語 | Romaji |
|---------|--------|--------|
| Embedded System | 組込システム | Komikomi Shisutemu |
| Microcontroller | マイコン | Maikon |
| Communication Protocol | 通信プロトコル | Tsūshin Purotokoru |
| Bus Termination | 終端抵抗 | Shūtan Teikō |
| Debugging | デバッグ | Debaggugu |
| Documentation | 資料 | Shiryō |

### Workplace Phrases (HORENSO)

| Japanese | Romaji | English |
|----------|--------|---------|
| 報告します | Hōkoku shimasu | I will report |
| 連絡します | Renraku shimasu | I will contact |
| 相談します | Sōdan shimasu | I will consult |

### Cultural Concepts

1. **ものづくり (Monozukuri)** - Manufacturing excellence, craftsmanship spirit
2. **カイゼン (Kaizen)** - 改善 - Continuous improvement  
3. **報連相 (HORENSO)** - 報告・連絡・相談 - Report, Contact, Consult
4. **現地現物 (Genchi Genbutsu)** - Go and see for yourself

---

