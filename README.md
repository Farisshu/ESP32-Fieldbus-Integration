# ESP32 Logic Analyzer Automation & CAN Bus Monitor 🛠️📊📡

> **Professional Embedded System Prototype**: Capture → Auto-Detect → Professional Report Generation  
> **Industrial-Grade CAN 2.0A Communication** with TFT Visualization, FreeRTOS Multitasking, and Automated Testing  
> Standardized for R&D, Industrial Protocols (SPI, UART, I2C, CAN), and Internship Documentation.

![Python](https://img.shields.io/badge/Python-3.8+-blue?logo=python)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Build](https://github.com/Farisshu/esp32-logic-analyzer-automation/actions/workflows/pio-build.yml/badge.svg)
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

```
Archive_YYYYMMDD_HHMMSS/
├── professional_report.txt   # Full metadata, results, validation & conclusions
├── waveform_annotated.png    # Annotated plot with edge markers & timing labels
├── metadata.json             # Machine-readable results for CI/CD or databases
└── la_analysis.log           # Execution trace & debugging info
```

---

## 📁 Repository Structure

```
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
├── software/
│   ├── analyze_la_pro.py     # Main auto-report generator
│   ├── analyze_la_archive.py # Archive analysis utility
│   ├── generate_samples.py   # Synthetic CSV generator for testing
│   └── examples/             # Sample captures & test data
├── include/                  # Project header files
├── lib/                      # Private libraries
├── docs/                     # Wiring diagrams, PulseView settings, SOP
├── Archive_*/                # [IGNORED] Generated reports
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

```
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

```
Node TX (MCP2515)          Node RX (MCP2515)
     CANH ───────────────────── CANH
     CANL ───────────────────── CANL
              ┌─────────┐
              │ 120Ω    │  (Termination at BOTH ends)
              └─────────┘
```

---

## 🛠️ Build & Upload

### Firmware (PlatformIO)

```bash
# Navigate to project directory
cd firmware/integration/can_bus_with_tft

# Clean build files
pio run --target clean

# Build project
pio run

# Upload to device
pio run --target upload

# Open serial monitor
pio device monitor --baud 115200
```

### Software (Python Analysis)

```bash
# Activate virtual environment
source .venv/bin/activate  # Linux/Mac
# or
.venv\Scripts\activate     # Windows

# Run analysis
python software/analyze_la_pro.py software/examples/sample_uart_9600.csv \
  --operator "Your Name" \
  --dut "ESP32 Test Board" \
  --purpose "UART Protocol Verification"
```

---

## 📈 Test Results & Validation

### CAN Bus Communication Test

| Metric | Expected | Actual | Status |
|--------|----------|--------|--------|
| SPI Bus Sharing | No conflicts | ✅ Stable | PASS |
| CAN Frame Transmission | TXREQ clear | ✅ 0x00 | PASS |
| CAN Frame Reception | Payload intact | ✅ 100% | PASS |
| Error Flag (EFLG) | < 0x80 | ✅ 0x05 (minor warning) | PASS* |
| UI Refresh Rate | ≤ 5Hz | ✅ 5Hz | PASS |
| FreeRTOS Task Sync | No deadlock | ✅ Stable | PASS |

*Note: EFLG 0x05 indicates minor transmit warning (normal for prototyping with jumper wires). See [Troubleshooting](#-troubleshooting) for details.

### Logic Analyzer Detection

| Protocol | Auto-Detection | Decoding | Validation |
|----------|---------------|----------|------------|
| UART | ✅ Yes | ✅ Baud, Data bits | ✅ Bit timing |
| SPI | ✅ Yes | ✅ Clock, Data | ✅ Frequency |
| I2C | ✅ Yes | ✅ Address, Data | ✅ Timing |
| CAN | 🔜 Planned | 🔜 Planned | 🔜 Planned |

---

## 🤝 Contribution & Internship Readiness

This toolkit is designed to be **modular and extensible**:

- Add new protocol decoders in `software/decoders/`
- Integrate with CI/CD for automated test validation
- Export reports directly to lab management systems
- Extend firmware tests for additional protocols

Built for embedded engineers who value **traceability, automation, and professional documentation**.

### Portfolio Skills Demonstrated

| Skill | Evidence | Industry Value |
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

## 🔧 Troubleshooting

### Common Issues

| Issue | Cause | Solution |
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

```
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

## 📚 Additional Documentation

- [Test Procedures & SOP](docs/test_procedures.md) - Hardware setup & validation steps
- [Firmware README](firmware/README.md) - Detailed firmware project guide
- [Software README](software/README.md) - Python tool documentation
- [Integration Tests](firmware/integration/README.md) - Multi-node testing guide

---

## 🎓 Project Status & Roadmap

### Current Status: **Production-Ready for R&D Demonstration** ✅

| Feature | Status | Priority |
|---------|--------|----------|
| SPI Bus Sharing (MCP2515 + TFT) | ✅ Complete | High |
| CAN 2.0A Communication | ✅ Complete | High |
| FreeRTOS Multitasking | ✅ Complete | High |
| Logic Analyzer Auto-Report | ✅ Complete | High |
| LittleFS CSV Logging | ✅ Complete | Medium |
| ID Decoding Fix | ✅ Complete | High |
| SPI Stabilization Delay | ✅ Complete | High |
| Queue Management | ✅ Complete | High |
| Error Handling | ✅ Improved | Medium |
| Configuration Centralization | ✅ Complete | Medium |
| CAN TP (ISO 15765-2) | ⏳ Planned | Low |
| WiFi/Bluetooth Monitoring | ⏳ Planned | Low |

### Future Improvements

- [ ] Auto-baud detection for crystal tolerance compensation
- [ ] Extended frame (29-bit ID) support
- [ ] WiFi/Bluetooth remote monitoring via MQTT
- [ ] ISO 15765-2 (CAN Transport Protocol) layer
- [ ] Web-based dashboard for real-time monitoring
- [ ] Automated CI/CD pipeline for firmware testing

---

*Developed for industrial automation workflows & internship preparation (Automotive R&D)*  
*Author: M. Faris A. G. | Version: 1.0.2 | Last Updated: 2026-05-03 (All Issues Resolved)*
