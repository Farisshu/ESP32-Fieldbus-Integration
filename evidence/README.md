# Logic Analyzer Evidence & Test Data

This directory contains all logic analyzer captures, reports, and waveform documentation organized by test scenario.

## Structure

```
evidence/
├── uart_basic_test/           # UART loopback verification data
├── mcp2515_init_test/         # MCP2515 SPI initialization captures
├── spi_validation_test/       # SPI pin toggling validation
├── can_bus_monitor/           # CAN bus integration test data
├── rs485_test/                # RS485 communication captures
└── README.md                  # This file
```

## Contents per Test Folder

Each test folder contains:

| File | Description |
|------|-------------|
| `*.csv` | Raw logic analyzer capture data (Saleae/DSView format) |
| `waveform_annotated.png` | Visualized signals with protocol decoding overlays |
| `metadata.json` | Test configuration, parameters, and auto-detected protocols |
| `professional_report.txt` | Human-readable analysis summary |
| `analysis_report.txt` | Detailed timing analysis (legacy format) |
| `summary.csv` | Protocol detection summary table |
| `all_channels_waveform.png` | Multi-channel overview (legacy format) |

## How to Generate Reports

Use the Python analysis tool to process raw CSV captures:

```bash
cd software/

# Analyze a specific capture
python analyze_la_pro.py ../evidence/can_bus_monitor/capture.csv \
  --operator "Your Name" \
  --dut "ESP32+MCP2515" \
  --purpose "CAN Bus Integration Test"

# Output files will be generated in the same folder:
# - waveform_annotated.png
# - professional_report.txt
# - metadata.json
```

## Evidence Summary

### 1. UART Basic Test (`uart_basic_test/`)
- **Source**: `firmware/validation/uart_loopback_verification/`
- **Capture Date**: 2026-04-30
- **Sample Rate**: 8 MS/s
- **Channels**: logic.2, logic.3
- **Findings**: PWM/Clock signal at 2.67 MHz, 80% duty cycle
- **Status**: ✅ Hardware verified

### 2. MCP2515 Init Test (`mcp2515_init_test/`)
- **Source**: `firmware/validation/mcp2515_spi_validation/`
- **Capture Date**: 2026-05-02
- **Sample Rate**: 2.5 MS/s
- **Protocol Detected**: SPI at 2.5 MHz
- **Transactions**: 1 successful SPI transaction
- **Status**: ✅ SPI communication verified

### 3. SPI Validation Test (`spi_validation_test/`)
- **Source**: `firmware/validation/mcp2515_spi_validation/`
- **Capture Date**: 2026-05-02
- **Sample Rate**: 8 MS/s
- **Protocols Detected**: SPI, I2C (cross-talk analysis)
- **Purpose**: Multi-protocol interference check
- **Status**: ✅ No interference detected

### 4. CAN Bus Monitor (`can_bus_monitor/`)
- **Source**: `firmware/integration/can_two_nodes/`
- **Capture Date**: 2026-05-03
- **Sample Rate**: 8 MS/s
- **DUT**: ESP32-A (TX) ↔ ESP32-B (RX)
- **Test Purpose**: CAN Bus Integration Test
- **Status**: ✅ Two-node communication verified

### 5. RS485 Test (`rs485_test/`)
- **Source**: `firmware/projects/rs485_loopback_test/`
- **Capture Date**: 2026-05-03
- **Sample Rate**: 8 MS/s
- **Module**: XY-017 RS485 to TTL
- **Test Purpose**: RS485 half-duplex validation
- **Status**: ✅ Loopback test successful

## Using Evidence for Portfolio

When presenting this portfolio:

1. **Show the workflow**: 
   Code → Upload → Capture → Analyze → Report

2. **Highlight key metrics**:
   - Sample rates used (2.5 - 8 MS/s)
   - Protocol detection accuracy
   - Timing verification results

3. **Demonstrate debugging skills**:
   - Reference specific waveform anomalies found
   - Show how issues were resolved
   - Compare before/after captures

## Sample Data for Offline Analysis

The `software/examples/` folder contains sample CSV files for testing the analysis tool without hardware:

- `sample_capture.csv` - Generic digital signals
- `sample_uart_9600.csv` - UART at 9600 baud
- `sample_spi_mcp2515.csv` - SPI communication with MCP2515

## Best Practices

1. **Naming Convention**: Use descriptive folder names matching test scenarios
2. **Metadata**: Always include operator name, DUT description, and test purpose
3. **Version Control**: Only commit small sample files; large captures stay local
4. **Documentation**: Update this README when adding new test categories

## Git Configuration

Large binary files (PNG, CSV >1MB) are excluded from Git via `.gitignore`. 
To share specific evidence:

```bash
# Compress a test folder
zip -r evidence_can_bus.zip evidence/can_bus_monitor/

# Or use Git LFS for large files
git lfs track "evidence/**/*.csv"
```

---

*Part of ESP32 Logic Analyzer Automation Project*
*Evidence Repository Version: 1.0*
*Last Updated: 2026-05-06*
