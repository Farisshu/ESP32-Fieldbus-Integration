# Example Captures 📊

Sample logic analyzer captures for testing and demonstration purposes.

## Overview

This directory contains CSV files exported from logic analyzers (PulseView, Saleae, etc.) that can be used to:
- Test the analysis scripts without hardware
- Learn expected file formats
- Validate protocol detection algorithms
- Demonstrate report generation capabilities

## Files

| File | Size | Protocol | Description |
|------|------|----------|-------------|
| `sample_capture.csv` | ~260 B | Generic | Basic test capture with simple waveforms |
| `sample_spi_mcp2515.csv` | ~4.3 KB | SPI | MCP2515 CAN controller SPI communication |
| `sample_uart_9600.csv` | ~215 KB | UART | UART serial data at 9600 baud |

## Usage

### Testing Analysis Scripts

```bash
# Test with SPI capture
python ../analyze_la_pro.py sample_spi_mcp2515.csv \
  --operator "Test User" \
  --dut "MCP2515 Test Board"

# Test with UART capture
python ../analyze_la_pro.py sample_uart_9600.csv \
  --purpose "UART Communication Test"
```

### Generating New Samples

Use the sample generator to create custom test data:

```bash
python ../generate_samples.py \
  --protocol uart \
  --baud 9600 \
  --duration 1.0 \
  --output my_uart_sample.csv
```

## CSV Format

Expected column structure:

```csv
time,ch0,ch1,ch2,ch3
0.000000,0,1,0,1
0.000001,0,1,0,1
...
```

| Column | Description |
|--------|-------------|
| `time` | Timestamp in seconds |
| `ch0-ch7` | Channel data (0 or 1) |

## Creating Your Own Captures

### Using PulseView

1. Connect logic analyzer to your circuit
2. Configure sample rate (recommended: 8+ MS/s for SPI, 1+ MS/s for UART)
3. Capture the waveform
4. Export as CSV: `File → Export → Comma-Separated Values (.csv)`
5. Place in this directory for testing

### Recommended Settings

| Protocol | Sample Rate | Capture Duration | Channels |
|----------|-------------|------------------|----------|
| UART 9600 | 1 MS/s | 1 second | 1-2 |
| UART 115200 | 8 MS/s | 0.5 seconds | 1-2 |
| SPI 1MHz | 8 MS/s | 0.1 seconds | 3-4 |
| I2C 100kHz | 1 MS/s | 0.5 seconds | 2 |

## Protocol Details

### SPI (sample_spi_mcp2515.csv)

Captured signals:
- **CH0**: CS (Chip Select)
- **CH1**: SCK (Clock)
- **CH2**: MOSI (Master Out Slave In)
- **CH3**: MISO (Master In Slave Out)

Contains MCP2515 register access sequences.

### UART (sample_uart_9600.csv)

Captured signals:
- **CH0**: TX (Transmit)
- **CH1**: RX (Receive) - optional

Contains ASCII text data at 9600 baud, 8N1.

## Contributing

When adding new sample captures:
1. Use descriptive filenames (e.g., `spi_device_register.csv`)
2. Include a brief description in this README
3. Ensure CSV is properly formatted
4. Remove any sensitive information

---

*These samples are provided for testing and educational purposes*
