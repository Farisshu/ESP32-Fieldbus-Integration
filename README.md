# ESP32 Logic Analyzer Automation 🛠️📊

> **All-in-One Logic Analyzer Workflow**: Capture → Auto-Detect → Professional Report Generation  
> Standardized for R&D, Industrial Protocols (SPI, UART, I2C, CAN), and Internship Documentation.

![Python](https://img.shields.io/badge/Python-3.8+-blue?logo=python)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Build](https://github.com/Farisshu/esp32-logic-analyzer-automation/actions/workflows/pio-build.yml/badge.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![ESP32](https://img.shields.io/badge/Platform-ESP32-blue)
## 🎯 Project Goal

Automate logic analyzer data processing to eliminate manual screenshot/reporting. Transform raw CSV exports into **structured, auditable test reports** with protocol decoding, timing validation, and auto-generated conclusions.

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

## 📁 Repository Structure

```
esp32-logic-analyzer-automation/
├── firmware/                 # ESP32 test codes (PlatformIO)
│   ├── tests/               # Unit tests for individual modules
│   ├── integration/         # Multi-node communication tests
│   ├── test_uart_basic/     # UART communication test
│   └── test_can_spi_test/   # CAN/MCP2515 SPI test
├── software/
│   ├── analyze_la_pro.py     # Main auto-report generator
│   ├── analyze_la_archive.py # Archive analysis utility
│   ├── generate_samples.py   # Synthetic CSV generator for testing
│   └── examples/             # Sample captures & test data
├── include/                  # Project header files
├── lib/                      # Private libraries
├── docs/                     # Wiring diagrams, PulseView settings (planned)
├── archives/                 # [IGNORED] Generated reports
├── requirements.txt          # Python dependencies
├── LICENSE                   # MIT License
└── README.md                 # This file
```

## 🔧 How It Works

1. **Capture**: Export CSV from PulseView / Saleae / Generic LA
2. **Auto-Detect**: Script identifies UART, SPI, or I2C patterns from waveform timing
3. **Decode**: Extracts protocol frames, register accesses, or byte streams
4. **Validate**: Checks against spec thresholds (jitter, frequency, CRC, timing margins)
5. **Report**: Generates standardized TXT/PNG/JSON output ready for documentation

## 📊 Example Report Snippet

```
🔍 PROTOCOL ANALYSIS RESULTS
  SPI
  Channel(s)       : ch0-ch1
  Status           : ACTIVE
  Total Transactions: 1

  ✅ CONCLUSIONS & RECOMMENDATIONS
  ✅ PASS: All protocols decoded successfully without errors.
  ✅ Signal integrity appears stable.
```

## 🧪 Testing & Development Workflow

This repository follows a **modular R&D workflow** aligned with industrial automation standards:

| Stage | Folder | Purpose | Status |
|-------|--------|---------|--------|
| **Unit Test** | `firmware/tests/` | Verify individual modules (SPI, UART, TFT init) | ✅ MCP2515 SPI+Loopback Verified |
| **Integration** | `firmware/integration/` | Multi-node communication (CAN bus, RS485 Modbus) | 🔄 In Progress |
| **Project** | `firmware/projects/` | Final integrated applications | ⏳ Planned |

## 📦 Firmware Projects

| Project | Description | Status |
|---------|-------------|--------|
| `test_uart_basic` | Basic UART communication test with SPI initialization | ✅ Complete |
| `test_can_spi_test` | MCP2515 CAN controller SPI interface diagnostic | ✅ Complete |
| `integration/can_two_nodes` | Two-node CAN bus communication system | 🔄 In Progress |

## 🤝 Contribution & Internship Readiness

This toolkit is designed to be **modular and extensible**:

- Add new protocol decoders in `software/decoders/`
- Integrate with CI/CD for automated test validation
- Export reports directly to lab management systems
- Extend firmware tests for additional protocols

Built for embedded engineers who value **traceability, automation, and professional documentation**.

## 📄 License

MIT License. Free for educational, R&D, and commercial use.

---

*Developed for industrial automation workflows & internship preparation (Automotive R&D)*

### 📐 Wiring & Test SOP
See [docs/test_procedures.md](docs/test_procedures.md) for hardware setup & validation steps.

### 📊 Screenshots
![CAN Monitor UI](docs/screenshots/tft_can_monitor.png)

