# 📚 Materi Prioritas Tinggi - High Priority Learning Materials

> **Embedded Systems & Automotive Protocols**  
> 組込システムと自動車プロトコル学習教材  
> Panduan lengkap untuk persiapan magang di industri otomotif

---

## 📋 Daftar Materi

### ✅ Topik Prioritas Tinggi (High Priority)

| No | Topik | File | Status | Halaman |
|----|-------|------|--------|---------|
| 1 | **UDS (ISO 14229)** | [01_UDS_Complete.md](./01_UDS_Complete.md) | ✅ Complete | 1225 lines |
| 2 | **CAN TP (ISO 15765-2)** | [02_CAN_TP_Complete.md](./02_CAN_TP_Complete.md) | ✅ Complete | 1064 lines |
| 3 | **AUTOSAR Classic** | [03_AUTOSAR_Classic.md](./03_AUTOSAR_Classic.md) | 🔜 Coming Soon | - |
| 4 | **Bootloader Development** | [04_Bootloader.md](./04_Bootloader.md) | 🔜 Coming Soon | - |

---

## 🎯 Learning Path yang Direkomendasikan

```
Step 1: CAN Bus Fundamentals
         ↓
Step 2: CAN TP (Transport Protocol) ← Start here if you know CAN basics
         ↓
Step 3: UDS (Diagnostic Services)
         ↓
Step 4: AUTOSAR Classic Architecture
         ↓
Step 5: Bootloader Implementation
         ↓
Step 6: MISRA C Compliance
```

---

## 📖 Ringkasan Materi

### 1. UDS (Unified Diagnostic Services) - ISO 14229

**Apa yang akan Anda pelajari:**
- Diagnostic communication antara tester dan ECU
- Service Identifiers (SID) utama: 0x10, 0x11, 0x22, 0x27, 0x2E, 0x19, 0x14
- Positive dan Negative Response format
- Security Access mechanism (seed-key)
- DTC (Diagnostic Trouble Code) handling
- Implementasi UDS Client dan Server

**Prerequisites:** CAN Bus, Hexadecimal system

**Duration:** 2-3 minggu

---

### 2. CAN TP (CAN Transport Protocol) - ISO 15765-2

**Apa yang akan Anda pelajari:**
- Single Frame, First Frame, Consecutive Frame, Flow Control
- Flow Control mechanism (CTS, Wait, Overflow)
- Timing parameters (P2, P2*, S3, STmin, Block Size)
- State machine implementation
- Multi-frame data transfer (> 8 bytes)

**Prerequisites:** CAN Bus fundamentals

**Duration:** 1-2 minggu

---

### 3. AUTOSAR Classic (Coming Soon)

**Yang akan dibahas:**
- AUTOSAR architecture overview
- Software Components (SWC)
- Runtime Environment (RTE)
- Basic Software (BSW) layers
- Configuration dengan ARXML

---

### 4. Bootloader Development (Coming Soon)

**Yang akan dibahas:**
- Bootloader architecture
- Memory layout (Flash, EEPROM)
- Communication via UDS (0x34, 0x36, 0x37)
- Safety mechanisms (checksum, rollback)
- Dual-bank firmware update

---

## 🛠️ Tools yang Diperlukan

| Tool | Purpose | Link |
|------|---------|------|
| **PCAN-View** | CAN bus monitoring | [PEAK-System](https://peak-system.com) |
| **SavvyCAN** | Open-source CAN tool | [GitHub](https://github.com/rivieragc/savvycan) |
| **Python + python-can** | Scripting & testing | [python-can](https://python-can.readthedocs.io) |
| **Logic Analyzer** | Hardware debugging | Saleae, DSView |
| **MCP2515 + TJA1050** | CAN hardware | Module murah untuk praktik |

---

## 📝 Cara Menggunakan Materi Ini

1. **Baca teori** dari markdown file
2. **Pelajari code example** (C++ dan Python)
3. **Practice dengan hardware** (MCP2515 module)
4. **Run test scripts** untuk validasi
5. **Document hasil** dalam portfolio

---

## 🔗 Link ke Materi Lain

- **[Materi Lengkap](../README_MATERI_LENGKAP.md)** - Overview semua topik
- **[Roadmap Magang](../ROADMAP_MAGANG.md)** - Persiapan magang
- **[Japanese Learning](../JAPANESE_EMBEDDED_LEARNING.md)** - Bahasa Jepang teknis

---

## 📊 Progress Tracking

| Minggu | Target | Status |
|--------|--------|--------|
| 1-2 | CAN TP mastery | ⬜ |
| 3-4 | UDS services implementation | ⬜ |
| 5-6 | AUTOSAR basics | ⬜ |
| 7-8 | Bootloader prototype | ⬜ |

---

## 💡 Tips Belajar Efektif

1. **Hands-on Practice**: Teori saja tidak cukup, wajib praktik dengan hardware
2. **Debug Systematically**: Gunakan logic analyzer untuk setiap masalah
3. **Document Everything**: Catat setiap percobaan dan hasilnya
4. **Join Community**: Forum seperti EEVblog, Reddit r/embedded
5. **Build Portfolio**: Setiap milestone, upload ke GitHub

---

## 📧 Contact & Support

Jika ada pertanyaan atau menemukan error dalam materi:
- Buat issue di repository ini
- Diskusi di forum embedded systems
- Referensi ke standar ISO resmi untuk detail lengkap

---

**Last Updated:** 2025  
**Version:** 1.0  
**License:** MIT
