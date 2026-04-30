# ESP32 Logic Analyzer Automation 🛠️📊

> Workflow lengkap: ESP32 → Logic Analyzer (FX2LP/PulseView) → Python Auto-Analysis → Professional Report

![Python](https://img.shields.io/badge/Python-3.8+-blue?logo=python)
![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)
![PulseView](https://img.shields.io/badge/PulseView-Sigrok-green?logo=linux)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## 🎯 Tujuan
Repo ini berisi toolkit untuk:
- ✅ Debug sinyal digital ESP32 (UART, PWM, SPI, I2C, CAN)
- ✅ Capture waveform dengan Logic Analyzer murah (FX2LP clone 24MHz 8CH)
- ✅ Auto-generate report dengan Python (grafik, statistik, dokumentasi)
- ✅ Dokumentasi profesional untuk proyek embedded/R&D

## 🚀 Quick Start

### 1. Persyaratan Hardware
- ESP32 Dev Board (ESP32-WROOM / ESP32-S3)
- Logic Analyzer FX2LP (24MHz 8CH clone)
- Kabel jumper female-to-female

### 2. Persyaratan Software
- [VS Code](https://code.visualstudio.com/) + [PlatformIO](https://platformio.org/)
- [PulseView](https://sigrok.org/wiki/Downloads) (sigrok + fx2lafw driver)
- Python 3.8+ dengan library: `pandas`, `matplotlib`, `numpy`

### 3. Instalasi
```bash
# Clone repo
git clone https://github.com/Farisshu/esp32-logic-analyzer-automation.git
cd esp32-logic-analyzer-automation

# Buat & aktifkan virtual environment (disarankan)
python -m venv .venv
# Windows:
.venv\Scripts\activate
# Linux/Mac:
source .venv/bin/activate

# Install dependency Python
pip install -r requirements.txt
```

### 4. Jalankan Analisis
```bash
# Setelah export CSV dari PulseView
python software/analyze_la_archive.py software/examples/sample_capture.csv

# Atau dengan file CSV custom:
python software/analyze_la_archive.py path/to/your_capture.csv
```

Hasil akan tersimpan otomatis di folder `Archive_YYYYMMDD_HHMMSS/` 📁

## 📁 Struktur Proyek
```
esp32-logic-analyzer-automation/
├── firmware/              ← Kode ESP32 (PlatformIO) untuk berbagai test case
│   └── test_uart_basic/   ← Contoh: UART TX sederhana
├── software/              ← Tool Python untuk analisis & report
│   ├── analyze_la_archive.py  ← Script utama (auto-report generator)
│   └── examples/          ← Contoh data CSV untuk testing
├── docs/                  ← Dokumentasi: wiring diagram, screenshot PulseView
├── archives/              ← [IGNORED] Hasil generate script (jangan di-commit)
├── .gitignore             ← Konfigurasi ignore file/folder
├── requirements.txt       ← Dependency Python
└── README.md              ← Dokumentasi ini
```

## 🔧 Fitur Script Python (`analyze_la_archive.py`)
- ✅ **Auto-detect signal type**: PWM, UART, Clock, Pulse, Digital Signal
- ✅ **Statistik presisi**: Frekuensi (mean ± std), Duty Cycle, Period, Jitter
- ✅ **Visualisasi otomatis**: Generate waveform plot (PNG) untuk semua channel aktif
- ✅ **Multi-format export**: 
  - `analysis_report.txt` → Laporan teks profesional
  - `summary.csv` → Ringkasan untuk Excel/Google Sheets
  - `metadata.json` → Metadata terstruktur untuk dokumentasi
- ✅ **Auto-organize**: Folder archive dengan timestamp, siap untuk versioning

## 📸 Contoh Output
```
LOGIC.2
  Signal Type      : PWM/Clock
  Frequency        : 1000.00 Hz ± 0.05 Hz
  Duty Cycle       : 50.00%
  Period (avg)     : 1000.000 µs ± 0.050 µs
```
*Output: Grafik waveform + analisis otomatis + laporan teks siap dokumentasi*

## 🧪 Contoh Firmware yang Tersedia
| Folder | Deskripsi | Protocol |
|--------|-----------|----------|
| `firmware/test_uart_basic/` | UART TX 9600 baud, kirim timestamp | UART |
| *(akan ditambah)* | PWM timing validation | PWM |
| *(akan ditambah)* | SPI communication test | SPI |
| *(akan ditambah)* | I2C sensor read test | I2C |

## 🤝 Kontribusi
Pull request welcome! Untuk bug report atau feature request, silakan buka [Issue](https://github.com/Farisshu/esp32-logic-analyzer-automation/issues).

## 📄 Lisensi
Distributed under the MIT License. See `LICENSE` for more information.

---
*Developed for embedded system debugging & professional documentation workflow.*  
*Target: R&D, industrial automation, and pre-production validation.*
```