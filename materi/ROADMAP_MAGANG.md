# 🚀 Roadmap Pengembangan Karir & Pembelajaran Magang

> **Panduan Strategis**: Persiapan magang di industri embedded systems, rekomendasi pembelajaran, dan wawasan produk Horiba ADS EVO  
> **戦略ガイド**: 組込システム業界でのインターンシップ準備、学習推奨事項、Horiba ADS EVO 製品の洞察

---

## 📋 Daftar Isi

1. [Persiapan Magang](#1-persiapan-magang)
2. [Rekomendasi Pembelajaran](#2-rekomendasi-pembelajaran)
3. [Skill Gap Analysis](#3-skill-gap-analysis)
4. [Wawasan Produk: Horiba ADS EVO](#4-wawasan-produk-horiba-ads-evo)
5. [Timeline Pembelajaran](#5-timeline-pembelajaran)
6. [Portfolio & Evidence](#6-portfolio--evidence)
7. [Tips Sukses Magang](#7-tips-sukses-magang)

---

## 1. Persiapan Magang

### 🔍 Apa yang Harus Dipersiapkan Sebelum Magang

#### Hard Skills (Wajib)

| Kategori | Skill | Status Saat Ini | Target | Prioritas |
|----------|-------|-----------------|--------|-----------|
| **Microcontroller** | ARM Cortex-M (STM32) | ⭐⭐⭐ Intermediate | ⭐⭐⭐⭐ Advanced | 🔴 High |
| | ESP32 | ⭐⭐⭐⭐ Advanced | ⭐⭐⭐⭐⭐ Expert | 🟢 Medium |
| | Renesas RA/RX | ⭐ Beginner | ⭐⭐⭐ Intermediate | 🔴 High |
| **Protocols** | CAN Bus 2.0A | ⭐⭐⭐⭐ Advanced | ⭐⭐⭐⭐⭐ Expert | 🟢 Done |
| | CAN TP (ISO 15765-2) | ⭐⭐ Basic | ⭐⭐⭐⭐ Advanced | 🔴 High |
| | UDS (ISO 14229) | ❌ Not Started | ⭐⭐⭐ Intermediate | 🔴 High |
| | Ethernet/IP | ❌ Not Started | ⭐⭐ Basic | 🟡 Medium |
| | FlexRay | ❌ Not Started | ⭐ Basic | 🟢 Low |
| **RTOS** | FreeRTOS | ⭐⭐⭐ Intermediate | ⭐⭐⭐⭐ Advanced | 🟡 Medium |
| | OSEK/AUTOSAR OS | ❌ Not Started | ⭐⭐ Basic | 🔴 High |
| **Tools** | STM32CubeIDE | ⭐⭐⭐ Intermediate | ⭐⭐⭐⭐ Advanced | 🟡 Medium |
| | Vector CANoe/CANalyzer | ❌ Not Started | ⭐⭐⭐ Intermediate | 🔴 High |
| | PCAN-View | ❌ Not Started | ⭐⭐ Basic | 🟡 Medium |
| | Oscilloscope | ⭐⭐ Basic | ⭐⭐⭐⭐ Advanced | 🟡 Medium |
| **Standards** | MISRA C:2012 | ⭐⭐ Awareness | ⭐⭐⭐ Compliance | 🔴 High |
| | AUTOSAR Classic | ❌ Not Started | ⭐ Basic | 🟡 Medium |
| | ISO 26262 (Functional Safety) | ❌ Not Started | ⭐ Awareness | 🟡 Medium |

#### Soft Skills (Penting)

| Skill | Deskripsi | Cara Latihan |
|-------|-----------|--------------|
| **Technical Documentation** | Menulis laporan teknis dalam Bhs Inggris & Jepang | Latih dokumentasi setiap project |
| **Problem Solving** | Debugging sistematis dengan logic analyzer | Buat SOP troubleshooting |
| **Time Management** | Estimasi task dan deadline tracking | Gunakan Gantt chart sederhana |
| **Team Collaboration** | Code review, Git workflow | Biasakan PR/MR dengan deskripsi jelas |
| **Presentation** | Presentasi hasil testing ke supervisor | Rekam demo video setiap milestone |

---

## 2. Rekomendasi Pembelajaran

### 🎯 Top 10 Skill yang Paling Berharga di Industri Otomotif

Berdasarkan riset lowongan kerja embedded automotive engineer:

| Rank | Skill | Alasan | Sumber Belajar |
|------|-------|--------|----------------|
| 1 | **CAN Bus + UDS** | Standar wajib semua ECU otomotif | [Vector Academy](https://vector.com/academy), BOSCH specs |
| 2 | **AUTOSAR Classic** | Arsitektur software standar industri | [AUTOSAR.org](https://www.autosar.org), Udemy courses |
| 3 | **MISRA C Compliance** | Safety-critical coding standard | MISRA C:2012 guideline book, PC-lint |
| 4 | **ISO 26262 (ASIL)** | Functional safety requirement | ISO 26262 overview, TÜV certification |
| 5 | **Bootloader Development** | Firmware update mechanism | AN from NXP/ST, custom implementation |
| 6 | **Diagnostics (OBD-II)** | Vehicle diagnostics protocol | SAE J1979, ISO 15031 |
| 7 | **Model-Based Design** | Simulink/Stateflow for control | MATLAB Academy, MathWorks training |
| 8 | **Ethernet Automotive** | Next-gen vehicle networking | OPEN Alliance, IEEE 802.3bw |
| 9 | **Cybersecurity (ISO/SAE 21434)** | Vehicle security standards | ISO/SAE 21434 overview, SecOC |
| 10 | **Japanese Technical Reading** | Datasheet & spec reading | JLPT N3+, technical documentation |

### 📚 Resource Pembelajaran Gratis vs Berbayar

#### Gratis (Recommended untuk Mulai)

| Resource | Topik | Link | Kualitas |
|----------|-------|------|----------|
| **STMicroelectronics University** | STM32, CAN, RTOS | [st.com](https://www.st.com) | ⭐⭐⭐⭐⭐ |
| **ESP-IDF Programming Guide** | ESP32 deep dive | [docs.espressif.com](https://docs.espressif.com) | ⭐⭐⭐⭐⭐ |
| **CAN in Automation (CiA)** | CAN protocol basics | [can-cia.org](https://www.can-cia.org) | ⭐⭐⭐⭐ |
| **FreeRTOS Documentation** | RTOS concepts | [freertos.org](https://www.freertos.org) | ⭐⭐⭐⭐⭐ |
| **MISRA C Guidelines Summary** | Coding standards | [misra.org.uk](https://www.misra.org.uk) | ⭐⭐⭐⭐ |
| **YouTube: Fastbit Embedded Brain** | ARM, RTOS, Embedded C | [YouTube](https://youtube.com/c/FastbitEmbeddedBrain) | ⭐⭐⭐⭐⭐ |
| **YouTube: Phil's Lab** | PCB design, STM32 | [YouTube](https://youtube.com/c/PhilsLab) | ⭐⭐⭐⭐⭐ |

#### Berbayar (Investasi Worth It)

| Course | Platform | Harga (IDR) | ROI |
|--------|----------|-------------|-----|
| **Mastering Microcontroller with Embedded Driver Development** | Udemy | ~150rb | ⭐⭐⭐⭐⭐ |
| **AUTOSAR Classic Platform Fundamentals** | Udemy/LinkedIn | ~200rb | ⭐⭐⭐⭐⭐ |
| **CAN Bus using MCP2515 + TJA1050** | Udemy | ~100rb | ⭐⭐⭐⭐ |
| **MATLAB/Simulink for Engineers** | Coursera | ~500rb/bln | ⭐⭐⭐⭐ |
| **JLPT Preparation Course** | Various | ~300rb | ⭐⭐⭐⭐ |

---

## 3. Skill Gap Analysis

### 📊 Kondisi Saat Ini vs Target Magang

```
Current State (Mei 2025):
├── ✅ CAN Bus 2.0A Implementation (2-node)
├── ✅ FreeRTOS Multitasking (3 tasks)
├── ✅ SPI Bus Sharing (MCP2515 + TFT)
├── ✅ UART/RS485 Communication
├── ✅ Logic Analyzer Integration
├── ✅ Basic Documentation (Bilingual)
├── ⚠️  CAN TP (In Progress)
└── ❌  UDS, AUTOSAR, ISO 26262

Target (3 Bulan Menuju Magang):
├── ✅ CAN Bus + CAN TP + UDS Basic
├── ✅ FreeRTOS + Queue/Semaphore Mastery
├── ✅ MISRA C Compliant Code
├── ✅ Basic AUTOSAR Architecture Understanding
├── ✅ Bootloader Implementation (Simple)
├── ✅ Japanese Technical Reading (JLPT N4-N3)
└── ✅ Portfolio Project (Complete System)
```

### 🎯 Action Plan untuk Menutup Gap

| Gap | Action | Timeline | Evidence Target |
|-----|--------|----------|-----------------|
| **CAN TP** | Implement transport protocol layer | 2 minggu | Multi-frame TX/RX demo |
| **UDS** | Study ISO 14229 services (0x10, 0x11, 0x22, 0x2E) | 3 minggu | UDS service simulator |
| **MISRA C** | Refactor existing code dengan PC-lint | 2 minggu | Report compliance % |
| **Bootloader** | Buat bootloader sederhana via UART | 3 minggu | OTA update demo |
| **AUTOSAR** | Pelajari ARXML, SWC, RTE concept | 4 minggu | Architecture diagram |
| **Japanese** | Fokus technical reading (datasheet) | Ongoing | Translate 5 datasheet pages |

---

## 4. Wawasan Produk: Horiba ADS EVO

### 🏢 Tentang Horiba Ltd.

**Horiba, Ltd.** (株式会社堀場製作所 - Kabushiki-gaisha Horiba Seisakusho) adalah perusahaan instrumen pengukuran dan analitik asal Jepang yang didirikan pada 1945 oleh Masao Horiba.

- **Headquarters**: Kyoto, Japan
- **Employees**: ~6,000+ worldwide
- **Revenue**: ~230 miliar JPY (2023)
- **Focus Areas**: Automotive, Medical, Environmental, Semiconductor

### 🔧 Produk ADS EVO (Advanced Diagnostic System EVO)

#### Apa itu ADS EVO?

**ADS EVO** adalah sistem diagnostik kendaraan generasi terbaru dari Horiba yang digunakan untuk:
- **Powertrain Testing**: Engine, transmission, hybrid/EV system validation
- **Emission Measurement**: Gas analysis, particulate matter measurement
- **Vehicle Dynamics**: Chassis dynamometer integration
- **ADAS Calibration**: Sensor calibration for autonomous driving

#### Spesifikasi Teknis (Berdasarkan Public Datasheet)

| Parameter | Spesifikasi |
|-----------|-------------|
| **Communication Protocols** | CAN FD, FlexRay, Ethernet (DoIP), LIN |
| **Supported Standards** | UDS (ISO 14229), OBD-II, J1939, GMLAN |
| **Sampling Rate** | Hingga 1 MHz per channel |
| **Channels** | Modular, hingga 64 channel analog/digital |
| **Integration** | MATLAB/Simulink, LabVIEW, Python API |
| **Safety Rating** | ISO 26262 ASIL-B compliant |
| **Operating System** | Real-time Linux dengan Xenomai patch |

#### Arsitektur Sistem ADS EVO

```
                    ADS EVO Main Unit
┌─────────────────────────────────────────────────────────────┐
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │   CAN FD     │  │   FlexRay    │  │   Ethernet   │      │
│  │   Module     │  │   Module     │  │   (DoIP)     │      │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘      │
│         │                 │                 │               │
│         └─────────────────┼─────────────────┘               │
│                           │                                 │
│                  ┌────────▼────────┐                        │
│                  │  Real-time CPU  │                        │
│                  │  (Intel Xeon)   │                        │
│                  └────────┬────────┘                        │
│                           │                                 │
│         ┌─────────────────┼─────────────────┐               │
│         │                 │                 │               │
│  ┌──────▼───────┐  ┌──────▼───────┐  ┌──────▼───────┐      │
│  │ Analog Input │  │ Digital I/O  │  │  Encoder     │      │
│  │ (±10V, 24bit)│  │  (TTL/LVTTL) │  │  Interface   │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
              ┌─────────────────────────┐
              │   Host PC (Windows)     │
              │  - STARS II Software    │
              │  - Data Visualization   │
              │  - Report Generation    │
              └─────────────────────────┘
```

#### Software Stack: STARS II

**STARS II** adalah software suite untuk ADS EVO yang menyediakan:
- **Test Sequence Editor**: Drag-and-drop test automation
- **Real-time Monitoring**: Live data plotting dengan resolusi tinggi
- **Data Logging**: Sinkronisasi multi-channel dengan timestamp presisi
- **Report Generator**: Auto-generate PDF/Excel report
- **API Integration**: Python, MATLAB, C++ SDK

#### Aplikasi di Industri

| Perusahaan | Penggunaan |
|------------|------------|
| **Toyota** | Powertrain calibration, emission testing |
| **Honda** | Hybrid system validation, battery testing |
| **Nissan** | EV motor characterization, inverter testing |
| **Denso** | ECU production line testing |
| **Bosch** | Sensor calibration, actuator testing |
| **Continental** | ADAS camera/radar integration testing |

#### Hubungan dengan Project Anda

Project CAN Bus + TFT Display yang Anda buat adalah **miniatur konsep** dari sistem yang ada di ADS EVO:

| Fitur di Project Anda | Equivalent di ADS EVO |
|-----------------------|----------------------|
| MCP2515 CAN Controller | Vector VN1630/VN8900 interface |
| ESP32 as Node | ECU simulator / Test bench controller |
| TFT Display | STARS II HMI dashboard |
| Logic Analyzer logging | High-speed oscilloscope integration |
| CSV data logging | STARS II database logging |
| FreeRTOS multitasking | Real-time Linux with Xenomai |

### 💡 Peluang Kontribusi di Horiba

Jika Anda magang di Horiba (atau perusahaan sejenis), berikut area dimana skill Anda bisa berkontribusi:

1. **Driver Development**: Membuat device driver untuk modul I/O baru
2. **Protocol Stack**: Implementasi CAN FD, DoIP, atau SOME/IP
3. **Test Automation**: Script Python untuk automated testing
4. **HMI Development**: Dashboard UI untuk monitoring real-time
5. **Data Analysis**: Algoritma untuk anomaly detection di logged data
6. **Documentation**: Technical writing dalam bilingual format

---

## 5. Timeline Pembelajaran

### 📅 Roadmap 6 Bulan Menuju Magang

```
Mei 2025          Juni 2025         Juli 2025
├── CAN TP        ├── UDS Basics    ├── AUTOSAR Intro
├── MISRA C       ├── Bootloader    ├── Model-Based Design
└── Documentation └── Japanese N4   └── Portfolio Polish

Agustus 2025      September 2025    Oktober 2025
├── Mock Interview ├── Company       ├── Internship
├── Final Project  │   Research      │   Preparation
└── Resume Build   └── Application   └── Onboarding
```

#### Detail Per Bulan

**Mei 2025 - Foundation Strengthening**
- [ ] Selesaikan CAN TP implementation
- [ ] Refactor code dengan MISRA C guidelines
- [ ] Dokumentasikan semua project dengan format profesional
- [ ] Mulai belajar Japanese JLPT N4 (fokus technical)

**Juni 2025 - Advanced Topics**
- [ ] Pelajari UDS services dasar (0x10, 0x11, 0x22, 0x2E, 0x85)
- [ ] Buat simple bootloader via UART/CAN
- [ ] Experiment dengan CAN FD (jika hardware tersedia)
- [ ] Latihan translate datasheet Jepang → Inggris

**Juli 2025 - Industry Standards**
- [ ] Pelajari AUTOSAR architecture (Classic platform)
- [ ] Pahami konsep ISO 26262 (Functional Safety)
- [ ] Buat simulasi ECU dengan Stateflow/MATLAB (jika ada akses)
- [ ] Update LinkedIn & portfolio website

**Agustus 2025 - Portfolio & Application**
- [ ] Consolidate semua project ke GitHub portfolio
- [ ] Buat demo video (3-5 menit) untuk each major project
- [ ] Tulis technical blog post (minimal 2 artikel)
- [ ] Mulai apply internship positions

**September 2025 - Interview Prep**
- [ ] Latihan technical interview (C, RTOS, CAN)
- [ ] Prepare STAR method untuk behavioral questions
- [ ] Riset target company (produk, teknologi, culture)
- [ ] Mock interview dengan mentor/teman

**Oktober 2025 - Onboarding**
- [ ] Selesaikan administrasi magang
- [ ] Pelajari tools spesifik yang akan dipakai di kantor
- [ ] Network dengan senior/mantan intern
- [ ] Set goals untuk masa magang

---

## 6. Portfolio & Evidence

### 📁 Struktur Portfolio Ideal

```
portfolio/
├── projects/
│   ├── can-bus-system/          # Project utama ini
│   │   ├── README.md            # Professional documentation
│   │   ├── firmware/            # Source code
│   │   ├── docs/                # Schematics, wiring diagrams
│   │   ├── videos/              # Demo recordings
│   │   └── reports/             # Test reports (bilingual)
│   ├── bootloader-uart/         # Side project
│   ├── uds-simulator/           # Learning project
│   └── japanese-tech-dict/      # Unique value proposition
├── certifications/
│   ├── jlpt-n4.pdf
│   ├── online-courses.pdf
│   └── workshop-attendance.pdf
├── publications/
│   ├── blog-post-can-bus.md
│   └── technical-article-rtos.md
└── resume/
    ├── resume_en.pdf
    ├── resume_jp.pdf
    └── cover_letter_template.docx
```

### ✅ Checklist Evidence untuk Magang

| Kategori | Item | Status | Link |
|----------|------|--------|------|
| **Technical Projects** | CAN Bus 2-node communication | ✅ Complete | [Link](../README.md) |
| | FreeRTOS multitasking demo | ✅ Complete | [Link](../firmware/) |
| | SPI bus sharing (MCP2515+TFT) | ✅ Complete | [Link](../firmware/integration/) |
| | Logic analyzer auto-report | ✅ Complete | [Link](../software/) |
| | **CAN TP implementation** | ⏳ In Progress | - |
| | **UDS service simulator** | 🔜 Planned | - |
| | **Bootloader demo** | 🔜 Planned | - |
| **Documentation** | Bilingual test reports | ✅ Complete | [Link](../docs/) |
| | Japanese technical dictionary | ✅ Complete | [Link](./JAPANESE_EMBEDDED_LEARNING.md) |
| | SOP & wiring diagrams | ✅ Complete | [Link](../docs/test_procedures.md) |
| **Certifications** | JLPT N4/N3 | 🔜 Target 2025 | - |
| | Online course certificates | ⏳ In Progress | - |
| **Publications** | Technical blog posts | 🔜 Planned | - |
| | GitHub contribution graph | ✅ Active | [Profile](https://github.com/) |

---

## 7. Tips Sukses Magang

### 💼 Do's (Lakukan)

| # | Tips | Contoh Implementasi |
|---|------|---------------------|
| 1 | **Document Everything** | Setiap experiment, tulis procedure & result |
| 2 | **Ask Questions Early** | Jangan tunggu 1 minggu stuck, tanya setelah 1 hari |
| 3 | **Version Control Rigorously** | Commit message jelas, branch naming convention |
| 4 | **Learn Company Tools Fast** | Weekend pertama: explore semua tool internal |
| 5 | **Build Relationships** | Makan siang dengan team, tanya career path mereka |
| 6 | **Show Initiative** | Kalau ada waktu luang, offer help atau learn new skill |
| 7 | **Present Progress Weekly** | Buat slide 3-5 halaman untuk weekly meeting |
| 8 | **Keep Learning Log** | Catat hal baru setiap hari (technical & non-technical) |

### 🚫 Don'ts (Hindari)

| # | Mistake | Consequence | Prevention |
|---|---------|-------------|------------|
| 1 | Working in isolation | Missed deadlines, wrong direction | Daily sync dengan supervisor |
| 2 | Not asking for help | Wasted time, frustration | Set 2-hour rule before asking |
| 3 | Poor commit messages | Can't track changes, hard to revert | Use conventional commits |
| 4 | Ignoring coding standards | Code rejected in review | Run linter before commit |
| 5 | Overpromising | Lost trust, stress | Under-promise, over-deliver |
| 6 | Not documenting code | Others can't maintain | Comment why, not what |
| 7 | Skipping breaks | Burnout, decreased productivity | Pomodoro technique, 5 min/hour |
| 8 | Not networking | Missed opportunities | Attend company events, lunch talks |

### 🎯 Goal Setting untuk Masa Magang

Gunakan framework **SMART**:

| Goal | Specific | Measurable | Achievable | Relevant | Time-bound |
|------|----------|------------|------------|----------|------------|
| **Technical** | Master CAN FD implementation | Complete 2 working demos | Yes, with mentor guidance | Core job requirement | Month 2 |
| **Language** | Improve Japanese reading | Translate 10 datasheets | Yes, 1/week | Work with JP docs | End of internship |
| **Soft Skill** | Presentation skills | Present 3x in team meeting | Yes, start small | Career growth | Monthly |
| **Network** | Build professional connections | Connect with 10 engineers | Yes, 1/week | Future references | End of internship |
| **Portfolio** | Add industry project | 1 major feature shipped | Yes, with team | Show real impact | End of internship |

---

## 📞 Resources & Contact

### Komunitas Online

- **Reddit**: r/embedded, r/ECE, r/EngineeringStudents
- **Discord**: Embedded Systems Community, EEVblog
- **LinkedIn Groups**: Embedded Professionals, Automotive Electronics
- **Forum**: STM32 Community, ESP32 Forum, CAN in Automation

### Mentor & Networking

- Cari mentor di LinkedIn dengan filter: "Embedded Engineer" + "Alumni [Kampus Anda]"
- Join meetup lokal: IoT Jakarta, Embedded Systems Indonesia
- Attend conference: Embedded Systems Conference (ESC), Electronica

### Recommended Books

| Title | Author | Level | Topic |
|-------|--------|-------|-------|
| **Making Embedded Systems** | Elecia White | Beginner | General embedded |
| **Test Driven Development for Embedded C** | James Grenning | Intermediate | Testing |
| **Patterns for Time-Triggered Embedded Systems** | Michael Pont | Advanced | Architecture |
| **Automotive Ethernet** | Kirsten Matheus | Intermediate | Networking |
| **The Automotive Chassis** | Reimpell | Advanced | Vehicle dynamics |

---

**Last Updated**: 2025-05-XX  
**Version**: 1.0.0  
**Maintained by**: Your Name - Embedded Systems Enthusiast

---

## 🔗 Related Documents

- [Main Learning Path](README_MATERI_LENGKAP.md) - Technical roadmap
- [Japanese Learning Guide](JAPANESE_EMBEDDED_LEARNING.md) - Language study plan
- [Project README](../README.md) - Main project documentation
- [Archive Index](../archive/ARCHIVE_INDEX.md) - Historical documentation
