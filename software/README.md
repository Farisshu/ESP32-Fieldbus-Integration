# Software Tools 🖥️

Python-based logic analyzer automation tools for capture analysis and report generation.

## Overview

This directory contains the core software tools that automate logic analyzer data processing, protocol decoding, and professional report generation.

## Structure

```
software/
├── analyze_la_pro.py       # Main auto-report generator
├── analyze_la_archive.py   # Archive analysis utility
├── generate_samples.py     # Synthetic CSV generator for testing
├── examples/               # Sample captures & test data
└── README.md               # This file
```

## Tools

### 1. analyze_la_pro.py

**Purpose**: Main analysis tool with auto-detection and professional report generation.

**Features:**
- Auto-detects UART, SPI, I2C protocols from waveform timing
- Decodes protocol frames and byte streams
- Validates against specification thresholds
- Generates comprehensive reports (TXT, PNG, JSON)

**Usage:**
```bash
# Basic analysis (auto-detect)
python analyze_la_pro.py capture.csv

# With metadata
python analyze_la_pro.py capture.csv \
  --operator "Your Name" \
  --dut "ESP32 + MCP2515" \
  --purpose "CAN Bus Verification" \
  --sample-rate 8
```

### 2. analyze_la_archive.py

**Purpose**: Analyze archived captures and batch process multiple files.

**Features:**
- Batch processing of multiple captures
- Historical data analysis
- Trend reporting

**Usage:**
```bash
python analyze_la_archive.py --input archives/ --output summary.txt
```

### 3. generate_samples.py

**Purpose**: Generate synthetic CSV files for testing and development.

**Features:**
- Create realistic UART, SPI, I2C waveforms
- Configurable parameters (baud rate, frequency, etc.)
- Useful for algorithm testing without hardware

**Usage:**
```bash
# Generate sample UART capture
python generate_samples.py --protocol uart --baud 9600 --output test_uart.csv

# Generate sample SPI capture
python generate_samples.py --protocol spi --frequency 1000000 --output test_spi.csv
```

## Examples Directory

The `examples/` folder contains sample captures for testing:

┌─────────────────────────────┬──────────────────────────┬──────────────┐
│ File                        │ Description              │ Protocol     │
├─────────────────────────────┼──────────────────────────┼──────────────┤
│ `sample_capture.csv`        │ Basic test capture       │ Generic      │
│ `sample_spi_mcp2515.csv`    │ MCP2515 SPI communication│ SPI          │
│ `sample_uart_9600.csv`      │ UART at 9600 baud        │ UART         │
└─────────────────────────────┴──────────────────────────┴──────────────┘

## Installation

```bash
# Create virtual environment
python -m venv .venv
source .venv/bin/activate  # Linux/Mac
# or
.venv\Scripts\activate     # Windows

# Install dependencies
pip install -r ../requirements.txt
```

## Dependencies

- **pandas** >= 1.5.0 - Data manipulation and analysis
- **matplotlib** >= 3.6.0 - Waveform visualization
- **numpy** >= 1.24.0 - Numerical operations

## Output Format

Analysis generates an archive directory with:

```
Archive_YYYYMMDD_HHMMSS/
├── professional_report.txt   # Full analysis report
├── waveform_annotated.png    # Annotated waveform plot
├── metadata.json             # Machine-readable results
└── la_analysis.log           # Execution log
```

## Supported Protocols

┌──────────────┬───────────────┬────────────────────┬────────────────┐
│ Protocol     │ Detection     │ Decoding           │ Validation     │
├──────────────┼───────────────┼────────────────────┼────────────────┤
│ UART         │ ✅ Auto       │ ✅ Baud, Data bits │ ✅ Bit timing  │
│ SPI          │ ✅ Auto       │ ✅ Clock, Data     │ ✅ Frequency   │
│ I2C          │ ✅ Auto       │ ✅ Address, Data   │ ✅ Timing      │
│ CAN          │ 🔜 Planned    │ 🔜 Planned         │ 🔜 Planned     │
└──────────────┴───────────────┴────────────────────┴────────────────┘

## Workflow Integration

### CI/CD Pipeline

```yaml
# Example GitHub Actions step
- name: Analyze Logic Analyzer Capture
  run: |
    python software/analyze_la_pro.py captures/latest.csv \
      --operator "CI Bot" \
      --dut "${{ matrix.device }}"
```

### Lab Automation

Integrate with lab management systems via JSON output:
```python
import json
with open('metadata.json') as f:
    results = json.load(f)
    if results['status'] == 'PASS':
        deploy_to_production()
```

## Troubleshooting

┌─────────────────────────────┬─────────────────────────────────────────────────┐
│ Issue                       │ Solution                                        │
├─────────────────────────────┼─────────────────────────────────────────────────┤
│ No protocol detected        │ Check CSV format and column names               │
│ Poor waveform quality       │ Increase sample rate in capture                 │
│ Missing dependencies        │ Run `pip install -r requirements.txt`           │
│ UART detection too strict   │ ✅ FIXED: Lowered threshold to 50 edges (v1.0.2)│
│ SPI requires 4 channels     │ ✅ FIXED: Now supports 3-wire SPI (CS, SCK, MOSI)│
└─────────────────────────────┴─────────────────────────────────────────────────┘

---

*For more information, see the main [README.md](../README.md)*
*Last updated: 2026-05-03 (All Issues Resolved)*

---

## 🇯🇵 日本語メモ (にほんご めも - Japanese Notes)

このドキュメントは技術学習とインターンシップ準備のために作成されました。
(This document was created for technical learning and internship preparation.)

**重要な用語 (Important Terms):**
- ファームウェア (Firmware) - 組み込みソフトウェア
- デバッグ (Debug) - 修正・検証作業
- テスト (Test) - 試験・検証
- 資料 (Shiryou) - Documentation

