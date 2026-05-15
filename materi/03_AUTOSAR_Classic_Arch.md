# AUTOSAR Classic Platform - Arsitektur Lengkap

## 📋 Informasi Modul

| Item | Deskripsi |
|------|-----------|
| **Standar** | AUTOSAR Classic Platform R20-11 |
| **Level** | Intermediate → Advanced |
| **Prasyarat** | C Basics, Microcontroller Architecture, CAN Bus |
| **Estimasi Waktu** | 8-12 jam |
| **Bahasa** | Indonesia - English - 日本語 |

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, Anda akan mampu:

1. ✅ Memahami filosofi dan tujuan AUTOSAR dalam industri otomotif
2. ✅ Menjelaskan arsitektur layer AUTOSAR Classic Platform
3. ✅ Mengidentifikasi komponen Basic Software (BSW) dan fungsinya
4. ✅ Merancang Software Component (SWC) dengan interface standar
5. ✅ Memahami peran Runtime Environment (RTE)
6. ✅ Mengkonfigurasi module BSW menggunakan tools
7. ✅ Membaca dan memahami AUTOSAR XML (ARXML) files
8. ✅ Menerapkan konsep Virtual Function Bus (VFB)

---

## 📚 Daftar Isi

1. [Pengantar AUTOSAR](#1-pengantar-autosar)
2. [Sejarah dan Evolusi](#2-sejarah-dan-evolusi)
3. [Arsitektur Layer AUTOSAR](#3-arsitektur-layer-autosar)
4. [Basic Software (BSW)](#4-basic-software-bsw)
5. [Runtime Environment (RTE)](#5-runtime-environment-rte)
6. [Application Layer](#6-application-layer)
7. [Methodology dan Tools](#7-methodology-dan-tools)
8. [Studi Kasus Implementasi](#8-studi-kasus-implementasi)
9. [Best Practices](#9-best-practices)
10. [Latihan dan Tantangan](#10-latihan-dan-tantangan)

---

## 1. Pengantar AUTOSAR

### 1.1 Apa itu AUTOSAR?

**AUTOSAR** (AUTomotive Open System ARchitecture) adalah partnership global produsen otomotif, supplier, dan perusahaan teknologi yang mengembangkan standar terbuka untuk arsitektur software ECU (Electronic Control Unit).

```
┌─────────────────────────────────────────────────────────────┐
│                    AUTOSAR Consortium                        │
├─────────────────────────────────────────────────────────────┤
│  Premium Car Manufacturers:                                  │
│  • BMW • Mercedes-Benz • Volkswagen • Audi • Porsche        │
│                                                              │
│  Volume Car Manufacturers:                                   │
│  • Toyota • Ford • GM • Stellantis • Renault                │
│                                                              │
│  Suppliers:                                                  │
│  • Bosch • Continental • Denso • Delphi • ZF                │
│                                                              │
│  Technology Partners:                                        │
│  • Vector • ETAS • Elektrobit • dSPACE                      │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Mengapa AUTOSAR Diperlukan?

#### Masalah Sebelum AUTOSAR

Sebelum AUTOSAR, setiap supplier mengembangkan software ECU dengan cara mereka sendiri:

```
┌──────────────────────────────────────────────────────────────┐
│           Situasi Sebelum AUTOSAR (Pre-2003)                 │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ECU A (Supplier X):    ECU B (Supplier Y):                  │
│  ┌──────────────┐       ┌──────────────┐                     │
│  │ Application  │       │ Application  │                     │
│  ├──────────────┤       ├──────────────┤                     │
│  │   OS Proprietary     │   OS Lain    │                     │
│  ├──────────────┤       ├──────────────┤                     │
│  │ Driver X1    │       │ Driver Y1    │                     │
│  │ Driver X2    │       │ Driver Y2    │                     │
│  └──────────────┘       └──────────────┘                     │
│       ↑                       ↑                               │
│  Tidak kompatibel      Tidak reusable                         │
│                                                              │
│  Dampak:                                                     │
│  ❌ Sulit migrasi ke hardware baru                           │
│  ❌ Biaya development tinggi                                 │
│  ❌ Waktu time-to-market lama                                │
│  ❌ Sulit integrasi multi-supplier                           │
└──────────────────────────────────────────────────────────────┘
```

#### Solusi AUTOSAR

AUTOSAR menyediakan standarisasi melalui:

```
┌──────────────────────────────────────────────────────────────┐
│              Solusi AUTOSAR (Post-2003)                      │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              Standardized Architecture                │   │
│  ├──────────────────────────────────────────────────────┤   │
│  │  Application Layer (Portable across ECUs)            │   │
│  ├──────────────────────────────────────────────────────┤   │
│  │  RTE (Standardized Communication Interface)          │   │
│  ├──────────────────────────────────────────────────────┤   │
│  │  BSW (Configurable, not recompiled)                  │   │
│  ├──────────────────────────────────────────────────────┤   │
│  │  MCAL (Hardware Abstraction)                         │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
│  Keuntungan:                                                 │
│  ✅ Portability - Code reuse across platforms               │
│  ✅ Scalability - Easy to add/remove features               │
│  ✅ Maintainability - Clear separation of concerns          │
│  ✅ Interoperability - Multi-supplier integration           │
│  ✅ Cost Reduction - Reduced development effort             │
└──────────────────────────────────────────────────────────────┘
```

### 1.3 Prinsip Dasar AUTOSAR

#### 1. Separation of Concerns

```
┌─────────────────────────────────────────────────────────────┐
│              Separation of Application & Infrastructure      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Application Software                                       │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  "WHAT" the system does                             │   │
│  │  • Engine control logic                             │   │
│  │  • Transmission strategy                            │   │
│  │  • Brake control algorithm                          │   │
│  └─────────────────────────────────────────────────────┘   │
│                          ↕                                  │
│  Runtime Environment (RTE)                                  │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  Communication & Data Exchange                      │   │
│  └─────────────────────────────────────────────────────┘   │
│                          ↕                                  │
│  Basic Software (BSW)                                       │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  "HOW" the system works                             │   │
│  │  • Device drivers                                   │   │
│  │  • Operating system                                 │   │
│  │  • Communication protocols                          │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 2. Configuration over Programming

```cpp
// ❌ SEBELUM AUTOSAR - Hardcoded
void CanInit(void) {
    CAN_CTRL1 = 0x0A;  // Magic number!
    CAN_CTRL2 = 0x1F;  // Tidak jelas artinya
    CAN_BTR = 0x3C;    // Bit timing hardcoded
}

// ✅ DENGAN AUTOSAR - Configured
// File: Can_Cfg.h (di-generate oleh tools)
#define CAN_CONTROLLER_0_BAUDRATE    500000UL
#define CAN_CONTROLLER_0_TSEG1       13U
#define CAN_CONTROLLER_0_TSEG2       2U
#define CAN_CONTROLLER_0_SJW         1U

void Can_Init(const Can_ConfigType* config) {
    // Konfigurasi dari parameter yang jelas
    Can_ControllerSetBaudrate(CAN_CONTROLLER_0, 
                              config->CanBaudrate);
}
```

#### 3. Component-Based Development

```
┌─────────────────────────────────────────────────────────────┐
│              Component-Based Architecture                   │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │  Engine SWC │───▶│  Trans SWC  │───▶│  Brake SWC  │     │
│  │  (Component)│    │ (Component) │    │ (Component) │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│        ↕                  ↕                  ↕              │
│  ┌─────────────────────────────────────────────────────┐   │
│  │           Runtime Environment (RTE)                  │   │
│  │     Handles all inter-component communication        │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
│  Keuntungan:                                                │
│  • Reusable components                                      │
│  • Independent development                                  │
│  • Easy testing & validation                                │
└─────────────────────────────────────────────────────────────┘
```

---

## 2. Sejarah dan Evolusi

### 2.1 Timeline Perkembangan AUTOSAR

```
┌──────────────────────────────────────────────────────────────┐
│                    AUTOSAR Development Timeline              │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  2003    2004    2006    2008    2010    2014    2018   2023 │
│   │       │       │       │       │       │       │      │   │
│   ▼       ▼       ▼       ▼       ▼       ▼       ▼      ▼   │
│  Founding R1.0   R2.0    R2.1    R2.2    R3.0    R4.x   R20- │
│  Partner Release Release Release Release Release Release 11   │
│                                                              │
│  Key Milestones:                                             │
│  2003: Partnership established by BMW, Bosch, Continental,   │
│        Ford, GM, PSA, Toyota, VW                             │
│                                                              │
│  2004: First release (R1.0) - Basic architecture defined    │
│                                                              │
│  2006: R2.0 - First production-ready version                │
│                                                              │
│  2008: R2.1 - Enhanced diagnostics support                  │
│                                                              │
│  2010: R2.2 - Improved scalability                          │
│                                                              │
│  2014: R3.0 - Major update with enhanced features           │
│                                                              │
│  2018: R4.2.2 - Adaptive Platform introduced                │
│                                                              │
│  2023: R20-11 - Latest Classic Platform release             │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 2.2 Classic vs Adaptive Platform

```
┌──────────────────────────────────────────────────────────────┐
│              AUTOSAR Platform Comparison                     │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌─────────────────────────┐  ┌─────────────────────────┐   │
│  │   Classic Platform      │  │  Adaptive Platform      │   │
│  ├─────────────────────────┤  ├─────────────────────────┤   │
│  │ Purpose:                │  │ Purpose:                │   │
│  │ • Safety-critical apps  │  │ • High-performance apps │   │
│  │ • Real-time control     │  │ • Connectivity          │   │
│  │ • Powertrain, Chassis   │  │ • ADAS, Infotainment    │   │
│  │                         │  │                         │   │
│  │ OS: OSEK/VDX based      │  │ OS: POSIX-based (Linux) │   │
│  │                         │  │                         │   │
│  │ Language: C             │  │ Language: C++           │   │
│  │                         │  │                         │   │
│  │ Communication:          │  │ Communication:          │   │
│  │ • CAN, LIN, FlexRay     │  │ • Ethernet (SOME/IP)    │   │
│  │                         │  │ • WiFi, Bluetooth       │   │
│  │                         │  │                         │   │
│  │ Memory: Static          │  │ Memory: Dynamic         │   │
│  │ allocation              │  │ allocation              │   │
│  │                         │  │                         │   │
│  │ ASIL: Up to ASIL D      │  │ ASIL: QM to ASIL B      │   │
│  └─────────────────────────┘  └─────────────────────────┘   │
│                                                              │
│  Fokus Modul Ini: Classic Platform                           │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 2.3 Versi Release dan Kompatibilitas

| Versi | Tahun | Fitur Utama | Status |
|-------|-------|-------------|--------|
| R1.x | 2004-2005 | Initial architecture | Deprecated |
| R2.0 | 2006 | First production version | Legacy |
| R2.1 | 2008 | Diagnostics enhancement | Legacy |
| R2.2 | 2010 | Scalability improvements | Legacy |
| R3.0 | 2014 | Major feature update | Still used |
| R4.0 | 2017 | Adaptive Platform intro | Active |
| R4.2 | 2018 | Ethernet support | Active |
| R20-11 | 2023 | Latest Classic | **Recommended** |

---

## 3. Arsitektur Layer AUTOSAR

### 3.1 Overview Arsitektur

```
┌──────────────────────────────────────────────────────────────┐
│              AUTOSAR Classic Platform Architecture           │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌────────────────────────────────────────────────────────┐ │
│  │              Application Layer                         │ │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐ │ │
│  │  │  Sensor SWC  │  │ Control SWC  │  │  Actuator SWC│ │ │
│  │  └──────────────┘  └──────────────┘  └──────────────┘ │ │
│  └────────────────────────────────────────────────────────┘ │
│                              ↕                               │
│  ┌────────────────────────────────────────────────────────┐ │
│  │           Runtime Environment (RTE)                    │ │
│  │  • Inter-SWC Communication                             │ │
│  │  • Client-Server Communication                         │ │
│  │  • Mode Management                                     │ │
│  └────────────────────────────────────────────────────────┘ │
│                              ↕                               │
│  ┌────────────────────────────────────────────────────────┐ │
│  │           Basic Software (BSW)                         │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │           Service Layer                          │ │ │
│  │  │  • OS • COM • DCM • DEM • NVM • WDGM            │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │        ECU Abstraction Layer                     │ │ │
│  │  │  • IOHWAB • MEMIF • FEE • PDU Router            │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │    Microcontroller Abstraction Layer (MCAL)      │ │ │
│  │  │  • DIO • PORT • ADC • PWM • CAN • LIN • SPI     │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  └────────────────────────────────────────────────────────┘ │
│                              ↕                               │
│  ┌────────────────────────────────────────────────────────┐ │
│  │              Microcontroller Hardware                  │ │
│  │  • CPU Core • Memory • Peripherals • Interrupts       │ │
│  └────────────────────────────────────────────────────────┘ │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 3.2 Application Layer

Application Layer adalah lapisan tertinggi yang berisi logika bisnis aplikasi.

#### Karakteristik Application Layer:

```
┌──────────────────────────────────────────────────────────────┐
│              Application Layer Characteristics               │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ✓ Hardware-independent                                      │
│  ✓ Portable across different ECUs                            │
│  ✓ Contains application-specific algorithms                  │
│  ✓ Implemented as Software Components (SWC)                  │
│  ✓ Communicates via RTE only                                 │
│  ✓ No direct hardware access                                 │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### Jenis-jenis Software Component (SWC):

```
┌──────────────────────────────────────────────────────────────┐
│              Software Component Types                        │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  1. Atomic Software Component                                │
│     ┌────────────────────────────────────────────────────┐  │
│     │  ┌──────────────────────────────────────────────┐  │  │
│     │  │           Runnable Entities                  │  │  │
│     │  │  ┌──────────┐  ┌──────────┐  ┌──────────┐   │  │  │
│     │  │  │ Runnable │  │ Runnable │  │ Runnable │   │  │  │
│     │  │  │    A     │  │    B     │  │    C     │   │  │  │
│     │  │  └──────────┘  └──────────┘  └──────────┘   │  │  │
│     │  └──────────────────────────────────────────────┘  │  │
│     │  Ports: Provider / Receiver / Client / Server       │  │
│     └────────────────────────────────────────────────────┘  │
│                                                              │
│  2. Composition SwComponent                                  │
│     ┌────────────────────────────────────────────────────┐  │
│     │  Contains multiple Atomic SwComponents             │  │
│     │  ┌────────┐  ┌────────┐  ┌────────┐               │  │
│     │  │ Atomic │  │ Atomic │  │ Atomic │               │  │
│     │  │  SWC   │  │  SWC   │  │  SWC   │               │  │
│     │  └────────┘  └────────┘  └────────┘               │  │
│     │  Internal connections between atomic SWCs          │  │
│     └────────────────────────────────────────────────────┘  │
│                                                              │
│  3. Sensor/Sensor Actuator SwComponent                       │
│     • Direct connection to ECU Abstraction Layer            │
│     • For simple I/O components                              │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 3.3 Runtime Environment (RTE)

RTE adalah middleware yang menghubungkan Application Layer dengan BSW.

#### Fungsi Utama RTE:

```
┌──────────────────────────────────────────────────────────────┐
│                    RTE Functions                             │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  1. Inter-SWC Communication                                  │
│     ┌─────────────┐         ┌─────────────┐                 │
│     │    SWC A    │────────▶│    SWC B    │                 │
│     │  (Sender)   │   RTE   │ (Receiver)  │                 │
│     └─────────────┘         └─────────────┘                 │
│                                                              │
│  2. Intra-SWC Communication                                  │
│     Between Runnable Entities within same SWC                │
│                                                              │
│  3. Client-Server Communication                              │
│     RPC-like mechanism for service calls                     │
│                                                              │
│  4. Mode Management                                          │
│     Handling different operation modes (e.g., sleep, run)    │
│                                                              │
│  5. Timing Protection                                        │
│     Monitoring execution time of runnables                   │
│                                                              │
│  6. Data Consistency                                         │
│     Ensuring atomic read/write operations                    │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### RTE Generation Process:

```
┌──────────────────────────────────────────────────────────────┐
│                  RTE Generation Flow                         │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  System Description (ARXML)                                  │
│         │                                                    │
│         ▼                                                    │
│  ┌─────────────────┐                                         │
│  │  RTE Generator  │  (Vector DaVinci, ETAS ISOLAR, etc.)   │
│  └─────────────────┘                                         │
│         │                                                    │
│         ▼                                                    │
│  Generated Files:                                            │
│  • Rte_Type.h           - Data type definitions             │
│  • Rte_Cfg.h            - Configuration parameters          │
│  • Rte_Internal.h       - Internal RTE structures           │
│  • Rte_<SwcName>.h      - SWC-specific headers              │
│  • Rte_<SwcName>.c      - SWC implementation stubs          │
│  • Rte_Main.c           - RTE main functions                │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 3.4 Basic Software (BSW)

BSW menyediakan layanan infrastruktur untuk aplikasi.

#### Struktur BSW Modules:

```
┌──────────────────────────────────────────────────────────────┐
│              Basic Software Module Structure                 │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌────────────────────────────────────────────────────────┐ │
│  │                 Service Layer                          │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  Operating System (OS)                           │ │ │
│  │  │  • Task scheduling                               │ │ │
│  │  │  • Interrupt management                          │ │ │
│  │  │  • Resource protection                           │ │ │
│  │  │  • Alarm handling                                │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  Communication (COM)                             │ │ │
│  │  │  • Signal packing/unpacking                      │ │ │
│  │  │  • I-PDU management                              │ │ │
│  │  │  • Gateway functionality                         │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  Diagnostics (DCM/DEM)                           │ │ │
│  │  │  • UDS services                                  │ │ │
│  │  │  • DTC management                                │ │ │
│  │  │  • Freeze frame storage                          │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  Memory Services (NVM/FEE)                       │ │ │
│  │  │  • Non-volatile data management                  │ │ │
│  │  │  • Flash EEPROM emulation                        │ │ │
│  │  │  • Wear leveling                                 │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │              ECU Abstraction Layer                     │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  I/O Hardware Abstraction (IOHWAB)               │ │ │
│  │  │  • Abstract I/O access                           │ │ │
│  │  │  • Pin configuration                             │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  Memory Abstraction (MEMIF)                      │ │ │
│  │  │  • Unified memory interface                      │ │ │
│  │  │  • Multi-driver support                          │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  PDU Router (PDUR)                               │ │ │
│  │  │  • Routing between interfaces                    │ │ │
│  │  │  • Multiplexing/demultiplexing                   │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  └────────────────────────────────────────────────────────┘ │
│  ┌────────────────────────────────────────────────────────┐ │
│  │           Microcontroller Abstraction Layer            │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  MCU Driver                                      │ │ │
│  │  │  • Clock setup                                   │ │ │
│  │  │  • Power mode management                         │ │ │
│  │  │  • Reset control                                 │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  PORT Driver                                     │ │ │
│  │  │  • Pin direction configuration                   │ │ │
│  │  │  • Pull-up/pull-down settings                    │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  DIO Driver                                      │ │ │
│  │  │  • Digital I/O read/write                        │ │ │
│  │  │  • Channel/group access                          │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  Communication Drivers                           │ │ │
│  │  │  • CAN Driver • LIN Driver • FlexRay Driver      │ │ │
│  │  │  • SPI Driver • I2C Driver • UART Driver         │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  │  ┌──────────────────────────────────────────────────┐ │ │
│  │  │  ADC Driver                                      │ │ │
│  │  │  • Analog-to-digital conversion                  │ │ │
│  │  │  • Channel groups                                │ │ │
│  │  │  • Result buffers                                │ │ │
│  │  └──────────────────────────────────────────────────┘ │ │
│  └────────────────────────────────────────────────────────┘ │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 3.5 Microcontroller Abstraction Layer (MCAL)

MCAL adalah lapisan terendah BSW yang berinteraksi langsung dengan hardware.

#### MCAL Driver Categories:

```
┌──────────────────────────────────────────────────────────────┐
│                    MCAL Driver Categories                    │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  1. Microcontroller Drivers                                  │
│     • Mcu      - Microcontroller unit driver                │
│     • Port     - Port pin configuration                     │
│     • Dio      - Digital I/O                                │
│     • Gpt      - General purpose timer                      │
│     • Pwm      - Pulse width modulation                     │
│     • Adc      - Analog-to-digital converter                │
│                                                              │
│  2. Memory Drivers                                           │
│     • Fls      - Flash driver                               │
│     • Eep      - EEPROM driver                              │
│     • RamTst   - RAM test driver                            │
│                                                              │
│  3. Communication Drivers                                    │
│     • Can      - Controller Area Network                    │
│     • Lin      - Local Interconnect Network                 │
│     • FlexRay  - FlexRay bus                                │
│     • Spi      - Serial Peripheral Interface                │
│     • I2c      - Inter-Integrated Circuit                   │
│     • Uart     - Universal Asynchronous Receiver/Transmitter│
│                                                              │
│  4. Safety Drivers                                           │
│     • Wdg      - Watchdog driver                            │
│     • SeIp     - Security IP driver                         │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

---

## 4. Basic Software (BSW) - Detail Implementation

### 4.1 Operating System (OS)

AUTOSAR OS berdasarkan standar OSEK/VDX.

#### OS Configuration Example:

```c
/* File: Os_Cfg.h */
#ifndef OS_CFG_H
#define OS_CFG_H

#include "Os_Types.h"

/* Task Definitions */
#define OS_TASK_SENSOR_READ     1U
#define OS_TASK_CONTROL_ALG     2U
#define OS_TASK_ACTUATOR_WRITE  3U

/* Alarm Definitions */
#define OS_ALARM_SENSOR_PERIOD  1U
#define OS_ALARM_CONTROL_PERIOD 2U

/* Counter Definitions */
#define OS_COUNTER_SYSTEM       1U

/* Resource Definitions */
#define OS_RESOURCE_SHARED_DATA 1U

/* Task Configuration */
extern const TaskConstType TaskConstConfig[];

/* Alarm Configuration */
extern const AlarmConstType AlarmConstConfig[];

/* Counter Configuration */
extern const CounterConstType CounterConstConfig[];

/* Resource Configuration */
extern const ResourceConstType ResourceConstConfig[];

#endif /* OS_CFG_H */
```

#### Task Implementation:

```c
/* File: ApplicationTasks.c */
#include "Os.h"
#include "Rte_Type.h"
#include "Rte_Main.h"

/* Task: Sensor Read */
TASK(Task_SensorRead) {
    Rte_Runnable_SensorComponent_ReadSensor();
    TerminateTask();
}

/* Task: Control Algorithm */
TASK(Task_ControlAlg) {
    Rte_Runnable_ControlComponent_CalculateControl();
    TerminateTask();
}

/* Task: Actuator Write */
TASK(Task_ActuatorWrite) {
    Rte_Runnable_ActuatorComponent_WriteActuator();
    TerminateTask();
}

/* Main Function */
int main(void) {
    /* Initialize BSW */
    Bsw_Init();
    
    /* Initialize RTE */
    Rte_Init();
    
    /* Start OS */
    StartOS(OSDEFAULTAPPMODE);
    
    /* Should never reach here */
    return 0;
}
```

### 4.2 Communication Stack (COM)

Communication stack menangani pertukaran data antar ECU.

#### COM Stack Architecture:

```
┌──────────────────────────────────────────────────────────────┐
│              AUTOSAR Communication Stack                     │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Application Layer                                           │
│  ┌─────────────┐                                            │
│  │  SWC Ports  │◀── Sender/Receiver Interfaces              │
│  └─────────────┘                                            │
│        ↕                                                     │
│  RTE                                                       │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              COM Module                              │   │
│  │  • Signal packing/unpacking                          │   │
│  │  • Signal group handling                             │   │
│  │  • Update bit detection                              │   │
│  │  • Deadline monitoring                               │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              PDU Router (PDUR)                       │   │
│  │  • Routing between COM and PDU Providers             │   │
│  │  • Gateway functionality                             │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │           PDU Providers                              │   │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐              │   │
│  │  │ CanIf   │  │ LinIf   │  │ FlexRay │              │   │
│  │  └─────────┘  └─────────┘  └─────────┘              │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │           Hardware Drivers                           │   │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐              │   │
│  │  │   CAN   │  │   LIN   │  │FlexRay  │              │   │
│  │  └─────────┘  └─────────┘  └─────────┘              │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### Signal Definition Example:

```c
/* File: Com_Cfg.h */
#ifndef COM_CFG_H
#define COM_CFG_H

#include "Com_Types.h"

/* Signal Definitions */
#define COM_SIGNAL_ENGINE_SPEED         1U
#define COM_SIGNAL_VEHICLE_SPEED        2U
#define COM_SIGNAL_BRAKE_STATUS         3U
#define COM_SIGNAL_GEAR_POSITION        4U

/* Signal Group Definitions */
#define COM_SIGNALGROUP_POWERTRAIN      1U

/* PDU Definitions */
#define COM_PDU_POWERTRAIN_TX           1U
#define COM_PDU_CHASSIS_RX              2U

/* IPDU Definitions */
#define COM_IPDU_POWERTRAIN             1U
#define COM_IPDU_CHASSIS                2U

/* Signal Configuration */
extern const ComSignalConfiguration ComSignalConfig[];

/* PDU Configuration */
extern const ComPduConfiguration ComPduConfig[];

#endif /* COM_CFG_H */
```

### 4.3 Diagnostic Stack (DCM/DEM)

Diagnostic stack menyediakan layanan diagnostik sesuai ISO 14229 (UDS).

#### DCM Architecture:

```
┌──────────────────────────────────────────────────────────────┐
│              AUTOSAR Diagnostic Stack                        │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  External Tester                                             │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │           Diagnostic Communication Manager           │   │
│  │                    (DCM)                             │   │
│  │  • UDS Service Handler                               │   │
│  │  • Session Control                                   │   │
│  │  • Security Access                                   │   │
│  │  • Routine Control                                   │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         Diagnostic Event Manager (DEM)               │   │
│  │  • DTC Storage                                       │   │
│  │  • Freeze Frame                                      │   │
│  │  • Occurrence Counter                                │   │
│  │  • Aging Mechanism                                   │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │           Non-Volatile Memory (NVM)                  │   │
│  │  • DTC Storage                                       │   │
│  │  • Configuration Data                                │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### DTC Event Configuration:

```c
/* File: Dem_Cfg.h */
#ifndef DEM_CFG_H
#define DEM_CFG_H

#include "Dem_Types.h"

/* DTC Definitions */
#define DEM_DTC_ENGINE_MISFIRE          0x000001U
#define DEM_DTC_FUEL_SYSTEM_LEAN        0x000002U
#define DEM_DTC_FUEL_SYSTEM_RICH        0x000003U
#define DEM_DTC_O2_SENSOR_FAULT         0x000004U

/* Event Configuration */
extern const DemEventParameterType DemEventConfig[];

/* DTC Configuration */
extern const DemDtcType DemDtcConfig[];

/* Freeze Frame Configuration */
#define DEM_FREEZE_FRAME_RECORD_COUNT   3U
#define DEM_FF_DATA_SIZE               64U

#endif /* DEM_CFG_H */
```

### 4.4 Memory Stack (NVM/FEE)

Memory stack mengelola penyimpanan data non-volatile.

#### Memory Stack Architecture:

```
┌──────────────────────────────────────────────────────────────┐
│              AUTOSAR Memory Stack                            │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Application Layer                                           │
│  ┌─────────────┐                                            │
│  │  NvM Blocks │  ← Data blocks definition                  │
│  └─────────────┘                                            │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         Non-Volatile Memory Manager (NvM)            │   │
│  │  • Block management                                  │   │
│  │  • Redundancy handling                               │   │
│  │  • CRC checking                                      │   │
│  │  • RAM mirror                                        │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │      Memory Abstraction Interface (MemIf)            │   │
│  │  • Unified memory interface                          │   │
│  │  • Multi-driver support                              │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │     Flash EEPROM Emulation (Fee)                     │   │
│  │  • Virtual EEPROM on Flash                           │   │
│  │  • Wear leveling                                     │   │
│  │  • Logical sector management                         │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              Flash Driver (Fls)                      │   │
│  │  • Flash sector erase                                │   │
│  │  • Flash page write                                  │   │
│  │  • Flash read                                        │   │
│  └──────────────────────────────────────────────────────┘   │
│        ↕                                                     │
│  Flash Memory Hardware                                       │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### NvM Block Configuration:

```c
/* File: NvM_Cfg.h */
#ifndef NVM_CFG_H
#define NVM_CFG_H

#include "NvM_Types.h"

/* Block IDs */
#define NVM_BLOCK_VEHICLE_CONFIG        1U
#define NVM_BLOCK_CALIBRATION_DATA      2U
#define NVM_BLOCK_LEARNED_VALUES        3U
#define NVM_BLOCK_DTC_SNAPSHOT          4U

/* Block Configuration */
extern const NvMBlockDescriptorType NvMBlockConfig[];

/* Block Types */
typedef struct {
    uint8_t vehicle_model;
    uint8_t engine_type;
    uint16_t max_rpm_limit;
    uint8_t transmission_type;
    uint8_t reserved[4];
    uint16_t crc;
} VehicleConfigType;

typedef struct {
    float fuel_injection_map[16];
    float ignition_timing_map[16];
    uint8_t adaptation_values[32];
    uint16_t crc;
} CalibrationDataType;

#endif /* NVM_CFG_H */
```

---

## 5. Runtime Environment (RTE) - Deep Dive

### 5.1 RTE Communication Mechanisms

#### Sender-Receiver Communication:

```c
/* File: Rte_SensorComponent.h */
#ifndef RTE_SENSOR_COMPONENT_H
#define RTE_SENSOR_COMPONENT_H

#include "Rte_Type.h"

/* Port: EngineSpeedPort (Sender) */
Std_ReturnType Rte_Write_EngineSpeedPort_EngineSpeed(
    uint16 engineSpeed
);

/* Port: VehicleSpeedPort (Sender) */
Std_ReturnType Rte_Write_VehicleSpeedPort_VehicleSpeed(
    uint8 vehicleSpeed
);

/* Port: BrakeStatusPort (Receiver) */
Std_ReturnType Rte_Read_BrakeStatusPort_BrakeStatus(
    boolean* brakeStatus
);

#endif /* RTE_SENSOR_COMPONENT_H */
```

#### Client-Server Communication:

```c
/* File: Rte_DiagnosticService.h */
#ifndef RTE_DIAGNOSTIC_SERVICE_H
#define RTE_DIAGNOSTIC_SERVICE_H

#include "Rte_Type.h"

/* Server Operation: ReadDTCInformation */
Std_ReturnType Rte_Call_DiagnosticService_ReadDTCInformation(
    uint32 dtcFormatIdentifier,
    DtcInfoType* dtcInfo
);

/* Server Operation: ClearDTC */
Std_ReturnType Rte_Call_DiagnosticService_ClearDTC(
    uint32 dtcGroup,
    uint8* numberOfClearedDTCs
);

/* Callback for server implementation */
void Rte_Srv_DiagnosticService_ReadDTCInformation(
    uint32 dtcFormatIdentifier,
    DtcInfoType* dtcInfo,
    Std_ReturnType* result
);

#endif /* RTE_DIAGNOSTIC_SERVICE_H */
```

### 5.2 Runnable Entity Scheduling

```c
/* File: Rte_Main.c */
#include "Rte_Main.h"
#include "Os.h"

/* Runnable: SensorComponent_ReadSensor */
FUNC(void, RTE_CODE) Rte_Runnable_SensorComponent_ReadSensor(void) {
    uint16 engine_speed;
    uint8 vehicle_speed;
    
    /* Read sensors from MCAL */
    engine_speed = Adc_ReadChannel(ADC_CHANNEL_ENGINE_SPEED);
    vehicle_speed = Adc_ReadChannel(ADC_CHANNEL_VEHICLE_SPEED);
    
    /* Write to ports */
    Rte_Write_EngineSpeedPort_EngineSpeed(engine_speed);
    Rte_Write_VehicleSpeedPort_VehicleSpeed(vehicle_speed);
}

/* Runnable: ControlComponent_CalculateControl */
FUNC(void, RTE_CODE) Rte_Runnable_ControlComponent_CalculateControl(void) {
    uint16 engine_speed;
    uint8 vehicle_speed;
    boolean brake_status;
    uint16 target_throttle;
    
    /* Read input ports */
    Rte_Read_EngineSpeedPort_EngineSpeed(&engine_speed);
    Rte_Read_VehicleSpeedPort_VehicleSpeed(&vehicle_speed);
    Rte_Read_BrakeStatusPort_BrakeStatus(&brake_status);
    
    /* Control algorithm */
    if (brake_status == TRUE) {
        target_throttle = 0;
    } else {
        target_throttle = CalculateThrottle(engine_speed, vehicle_speed);
    }
    
    /* Write output port */
    Rte_Write_ThrottleCommandPort_ThrottleCommand(target_throttle);
}

/* Runnable: ActuatorComponent_WriteActuator */
FUNC(void, RTE_CODE) Rte_Runnable_ActuatorComponent_WriteActuator(void) {
    uint16 throttle_command;
    
    /* Read command */
    Rte_Read_ThrottleCommandPort_ThrottleCommand(&throttle_command);
    
    /* Write to actuator */
    Pwm_SetChannelValue(PWM_CHANNEL_THROTTLE, throttle_command);
}
```

### 5.3 Mode Management

```c
/* File: Rte_ModeManagement.h */
#ifndef RTE_MODE_MANAGEMENT_H
#define RTE_MODE_MANAGEMENT_H

#include "Rte_Type.h"

/* Mode Declaration Groups */
#define RTE_MODEGROUP_ENGINEMODE         1U
#define RTE_MODEGROUP_TRANSMISSIONMODE   2U

/* Engine Modes */
#define RTE_MODE_ENGINEMODE_STOPPED      0U
#define RTE_MODE_ENGINEMODE_CRANKING     1U
#define RTE_MODE_ENGINEMODE_RUNNING      2U
#define RTE_MODE_ENGINEMODE_SHUTDOWN     3U

/* Transmission Modes */
#define RTE_MODE_TRANSMISSIONMODE_PARK   0U
#define RTE_MODE_TRANSMISSIONMODE_REVERSE 1U
#define RTE_MODE_TRANSMISSIONMODE_NEUTRAL 2U
#define RTE_MODE_TRANSMISSIONMODE_DRIVE  3U

/* Mode Switch Interface */
Std_ReturnType Rte_SwitchEngineMode(uint8 mode);
Std_ReturnType Rte_SwitchTransmissionMode(uint8 mode);

/* Current Mode Query */
uint8 Rte_GetCurrentEngineMode(void);
uint8 Rte_GetCurrentTransmissionMode(void);

#endif /* RTE_MODE_MANAGEMENT_H */
```

---

## 6. Application Layer - Implementation Guide

### 6.1 Creating Software Components

#### Step 1: Define Data Types

```c
/* File: Rte_Type.h */
#ifndef RTE_TYPE_H
#define RTE_TYPE_H

#include "Std_Types.h"

/* Application Data Types */
typedef uint16 EngineSpeedType;        /* Range: 0-8000 RPM */
typedef uint8  VehicleSpeedType;       /* Range: 0-250 km/h */
typedef uint16 ThrottleCommandType;    /* Range: 0-100% */
typedef boolean BrakeStatusType;       /* TRUE/FALSE */

/* Complex Data Types */
typedef struct {
    EngineSpeedType engineSpeed;
    VehicleSpeedType vehicleSpeed;
    BrakeStatusType brakeStatus;
    uint8 gearPosition;
    int16 engineTemp;
} PowertrainDataType;

typedef struct {
    uint16 injectionTime;
    uint16 ignitionAdvance;
    uint8 idleControl;
} ControlOutputType;

#endif /* RTE_TYPE_H */
```

#### Step 2: Define Port Interfaces

```xml
<!-- File: PowertrainInterfaces.arxml -->
<?xml version="1.0" encoding="UTF-8"?>
<AUTOSAR xmlns="http://autosar.org/schema/r4.0">
  <AR-PACKAGES>
    <AR-PACKAGE>
      <SHORT-NAME>PowertrainInterfaces</SHORT-NAME>
      
      <!-- Sender-Receiver Interface -->
      <CAN-CLUSTER>
        <SHORT-NAME>EngineSpeedInterface</SHORT-NAME>
        <TRANSMISSION-RECEPTION-INTERFACES>
          <SENDER-RECEIVER-INTERFACE>
            <SHORT-NAME>EngineSpeedPort</SHORT-NAME>
            <DATA-ELEMENTS>
              <VARIABLE-DATA-PROTOTYPE>
                <SHORT-NAME>EngineSpeed</SHORT-NAME>
                <TYPE-TREF DEST="IMPLEMENTATION-DATA-TYPE">
                  /PowertrainTypes/EngineSpeedType
                </TYPE-TREF>
              </VARIABLE-DATA-PROTOTYPE>
            </DATA-ELEMENTS>
          </SENDER-RECEIVER-INTERFACE>
        </TRANSMISSION-RECEPTION-INTERFACES>
      </CAN-CLUSTER>
      
      <!-- Client-Server Interface -->
      <CLIENT-SERVER-INTERFACE>
        <SHORT-NAME>DiagnosticService</SHORT-NAME>
        <OPERATIONS>
          <CLIENT-SERVER-OPERATION>
            <SHORT-NAME>ReadDTCInformation</SHORT-NAME>
            <ARGUMENTS>
              <ARGUMENT>
                <SHORT-NAME>DtcFormatIdentifier</SHORT-NAME>
                <DIRECTION>IN</DIRECTION>
                <TYPE-TREF DEST="IMPLEMENTATION-DATA-TYPE">
                  /Types/UInt32
                </TYPE-TREF>
              </ARGUMENT>
              <ARGUMENT>
                <SHORT-NAME>DtcInfo</SHORT-NAME>
                <DIRECTION>OUT</DIRECTION>
                <TYPE-TREF DEST="IMPLEMENTATION-DATA-TYPE">
                  /Types/DtcInfoType
                </TYPE-TREF>
              </ARGUMENT>
            </ARGUMENTS>
          </CLIENT-SERVER-OPERATION>
        </OPERATIONS>
      </CLIENT-SERVER-INTERFACE>
      
    </AR-PACKAGE>
  </AR-PACKAGES>
</AUTOSAR>
```

#### Step 3: Implement Software Component

```c
/* File: EngineControlSwC.c */
#include "Rte_EngineControlSwC.h"
#include "EngineControl_Algorithm.h"

/* Runnable: CalculateEngineControl */
FUNC(void, ENGINECONTROL_CODE) 
Rte_Runnable_EngineControlSwC_CalculateEngineControl(void) {
    
    /* Local variables */
    EngineSpeedType engine_speed;
    VehicleSpeedType vehicle_speed;
    BrakeStatusType brake_status;
    ControlOutputType control_output;
    
    /* Read input ports */
    if (Rte_Read_EngineControlSwC_EngineSpeedPort_EngineSpeed(
            &engine_speed) != E_OK) {
        /* Handle read error */
        return;
    }
    
    if (Rte_Read_EngineControlSwC_VehicleSpeedPort_VehicleSpeed(
            &vehicle_speed) != E_OK) {
        /* Handle read error */
        return;
    }
    
    if (Rte_Read_EngineControlSwC_BrakeStatusPort_BrakeStatus(
            &brake_status) != E_OK) {
        /* Handle read error */
        return;
    }
    
    /* Execute control algorithm */
    control_output = CalculateEngineControl(
        engine_speed,
        vehicle_speed,
        brake_status
    );
    
    /* Write output ports */
    Rte_Write_EngineControlSwC_ThrottleCommandPort_ThrottleCommand(
        control_output.injectionTime
    );
    
    Rte_Write_EngineControlSwC_IgnitionCommandPort_IgnitionAdvance(
        control_output.ignitionAdvance
    );
}

/* Helper function: Control algorithm */
STATIC(ControlOutputType, ENGINECONTROL_CODE) CalculateEngineControl(
    EngineSpeedType engine_speed,
    VehicleSpeedType vehicle_speed,
    BrakeStatusType brake_status
) {
    ControlOutputType output;
    
    /* Safety check */
    if (brake_status == TRUE) {
        output.injectionTime = 0;
        output.ignitionAdvance = 0;
        output.idleControl = 0;
        return output;
    }
    
    /* Normal control logic */
    output.injectionTime = LookupInjectionMap(engine_speed, vehicle_speed);
    output.ignitionAdvance = LookupIgnitionMap(engine_speed, vehicle_speed);
    output.idleControl = CalculateIdleControl(engine_speed);
    
    return output;
}
```

### 6.2 Composition SwComponent

```c
/* File: PowertrainComposition.c */
#include "Rte_PowertrainComposition.h"

/* Internal connections within composition */
/* EngineControlSwC -> TransmissionControlSwC */

FUNC(void, POWERTRAIN_CODE) 
Rte_Runnable_PowertrainComposition_SyncControl(void) {
    
    EngineSpeedType engine_speed;
    uint8 target_gear;
    
    /* Get engine speed from EngineControlSwC */
    Rte_Read_PowertrainComposition_EngineSpeed(
        &engine_speed
    );
    
    /* Calculate target gear */
    target_gear = CalculateTargetGear(engine_speed);
    
    /* Send to TransmissionControlSwC */
    Rte_Write_PowertrainComposition_TargetGear(
        target_gear
    );
}
```

---

## 7. Methodology dan Tools

### 7.1 AUTOSAR Development Workflow

```
┌──────────────────────────────────────────────────────────────┐
│              AUTOSAR Development Workflow                    │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Phase 1: System Design                                      │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ • Define system requirements                           │ │
│  │ • Create system architecture                           │ │
│  │ • Define SWC and interfaces                            │ │
│  │ • Generate System Description (ARXML)                  │ │
│  └────────────────────────────────────────────────────────┘ │
│                          ↓                                   │
│  Phase 2: ECU Configuration                                  │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ • Extract ECU-specific information                     │ │
│  │ • Configure BSW modules                                │ │
│  │ • Configure RTE                                        │ │
│  │ • Generate ECU Configuration (ARXML)                   │ │
│  └────────────────────────────────────────────────────────┘ │
│                          ↓                                   │
│  Phase 3: Code Generation                                    │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ • Generate BSW code from configuration                 │ │
│  │ • Generate RTE code from SWC description               │ │
│  │ • Generate OS configuration                            │ │
│  │ • Generate memory mapping                              │ │
│  └────────────────────────────────────────────────────────┘ │
│                          ↓                                   │
│  Phase 4: Application Development                            │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ • Implement SWC runnables                              │ │
│  │ • Integrate with generated code                        │ │
│  │ • Add application-specific logic                       │ │
│  └────────────────────────────────────────────────────────┘ │
│                          ↓                                   │
│  Phase 5: Build & Integration                                │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ • Compile all sources                                  │ │
│  │ • Link with libraries                                  │ │
│  │ • Generate executable                                  │ │
│  │ • Flash to ECU                                         │ │
│  └────────────────────────────────────────────────────────┘ │
│                          ↓                                   │
│  Phase 6: Testing & Validation                               │
│  ┌────────────────────────────────────────────────────────┐ │
│  │ • Unit testing                                         │ │
│  │ • Integration testing                                  │ │
│  │ • HIL testing                                          │ │
│  │ • Vehicle testing                                      │ │
│  └────────────────────────────────────────────────────────┘ │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 7.2 Popular AUTOSAR Tools

| Tool Vendor | Product Name | Features |
|-------------|--------------|----------|
| Vector | DaVinci Developer | System design, SWC modeling |
| Vector | DaVinci Configurator | BSW configuration |
| ETAS | ISOLAR-A | System design & configuration |
| ETAS | ISOLAR-B | ECU configuration |
| Elektrobit | Tresos Studio | BSW configuration |
| dSPACE | SystemDesk | System architecture design |
| IBM | Rational Rhapsody | Model-based design |

### 7.3 ARXML File Structure

```xml
<!-- Example: SystemDescription.arxml -->
<?xml version="1.0" encoding="UTF-8"?>
<AUTOSAR xmlns="http://autosar.org/schema/r4.0">
  <AR-PACKAGES>
    
    <!-- Package: DataTypes -->
    <AR-PACKAGE>
      <SHORT-NAME>DataTypes</SHORT-NAME>
      <ELEMENTS>
        <IMPLEMENTATION-DATA-TYPE>
          <SHORT-NAME>EngineSpeedType</SHORT-NAME>
          <CATEGORY>VALUE</CATEGORY>
          <SW-DATA-DEF-PROPS>
            <SW-DATA-DEF-PROPS-VARIANT-CONDITIONAL>
              <SW-DATA-TYPE>
                <BASE-TYPE>
                  <SHORT-NAME>uint16</SHORT-NAME>
                </BASE-TYPE>
              </SW-DATA-TYPE>
            </SW-DATA-DEF-PROPS-VARIANT-CONDITIONAL>
          </SW-DATA-DEF-PROPS>
        </IMPLEMENTATION-DATA-TYPE>
      </ELEMENTS>
    </AR-PACKAGE>
    
    <!-- Package: Interfaces -->
    <AR-PACKAGE>
      <SHORT-NAME>Interfaces</SHORT-NAME>
      <ELEMENTS>
        <SENDER-RECEIVER-INTERFACE>
          <SHORT-NAME>EngineSpeedInterface</SHORT-NAME>
          <DATA-ELEMENTS>
            <VARIABLE-DATA-PROTOTYPE>
              <SHORT-NAME>EngineSpeed</SHORT-NAME>
              <TYPE-TREF DEST="IMPLEMENTATION-DATA-TYPE">
                /DataTypes/EngineSpeedType
              </TYPE-TREF>
            </VARIABLE-DATA-PROTOTYPE>
          </DATA-ELEMENTS>
        </SENDER-RECEIVER-INTERFACE>
      </ELEMENTS>
    </AR-PACKAGE>
    
    <!-- Package: SwComponents -->
    <AR-PACKAGE>
      <SHORT-NAME>SwComponents</SHORT-NAME>
      <ELEMENTS>
        <APPLICATION-SW-COMPONENT-TYPE>
          <SHORT-NAME>EngineControlSwC</SHORT-NAME>
          <PORTS>
            <P-PORT-PROTOTYPE>
              <SHORT-NAME>EngineSpeedPort</SHORT-NAME>
              <PROVIDED-INTERFACE-TREF DEST="SENDER-RECEIVER-INTERFACE">
                /Interfaces/EngineSpeedInterface
              </PROVIDED-INTERFACE-TREF>
            </P-PORT-PROTOTYPE>
            <R-PORT-PROTOTYPE>
              <SHORT-NAME>VehicleSpeedPort</SHORT-NAME>
              <REQUIRED-INTERFACE-TREF DEST="SENDER-RECEIVER-INTERFACE">
                /Interfaces/VehicleSpeedInterface
              </REQUIRED-INTERFACE-TREF>
            </R-PORT-PROTOTYPE>
          </PORTS>
          <RUNNABLE-ENTITIES>
            <RUNNABLE-ENTITY>
              <SHORT-NAME>CalculateEngineControl</SHORT-NAME>
              <MINIMUM-STARTING-PERIOD>10.0</MINIMUM-STARTING-PERIOD>
              <SYMBOL-NAME>Rte_Runnable_EngineControlSwC_CalculateEngineControl</SYMBOL-NAME>
            </RUNNABLE-ENTITY>
          </RUNNABLE-ENTITIES>
        </APPLICATION-SW-COMPONENT-TYPE>
      </ELEMENTS>
    </AR-PACKAGE>
    
  </AR-PACKAGES>
</AUTOSAR>
```

---

## 8. Studi Kasus Implementasi

### 8.1 Case Study: Engine Control Unit (ECU)

#### System Requirements:

```
┌──────────────────────────────────────────────────────────────┐
│              Engine Control Unit Requirements                │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Functional Requirements:                                    │
│  FR-001: Read engine speed sensor (0-8000 RPM)              │
│  FR-002: Read vehicle speed sensor (0-250 km/h)             │
│  FR-003: Read brake pedal status                            │
│  FR-004: Calculate fuel injection timing                    │
│  FR-005: Calculate ignition advance                         │
│  FR-006: Control idle air valve                             │
│  FR-007: Support UDS diagnostic services                    │
│  FR-008: Store DTCs in non-volatile memory                  │
│                                                              │
│  Non-Functional Requirements:                                │
│  NFR-001: Response time < 10ms for control loop             │
│  NFR-002: Support CAN communication at 500 kbps             │
│  NFR-003: ASIL B compliance                                 │
│  NFR-004: MISRA C:2012 compliance                           │
│  NFR-005: Memory usage < 128KB ROM, < 16KB RAM              │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### Architecture Design:

```
┌──────────────────────────────────────────────────────────────┐
│              ECU Architecture Overview                       │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Application Layer:                                          │
│  ┌────────────────────────────────────────────────────────┐ │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐ │ │
│  │  │ SensorSwC    │  │ ControlSwC   │  │ ActuatorSwC  │ │ │
│  │  │ - Read RPM   │  │ - Calculate  │  │ - Write PWM  │ │ │
│  │  │ - Read Speed │  │ - Fuel Map   │  │ - Injectors  │ │ │
│  │  │ - Read Brake │  │ - Ignition   │  │ - Idle Valve │ │ │
│  │  └──────────────┘  └──────────────┘  └──────────────┘ │ │
│  └────────────────────────────────────────────────────────┘ │
│                                                              │
│  RTE:                                                        │
│  • Inter-SWC: SensorSwC → ControlSwC → ActuatorSwC          │
│  • Client-Server: ControlSwC ↔ DiagnosticSwC                │
│                                                              │
│  BSW:                                                        │
│  • OS: OSEK-compliant with 3 tasks                          │
│  • COM: CAN signal processing                               │
│  • DCM: UDS services (0x10, 0x11, 0x22, 0x2E, 0x3E)         │
│  • DEM: DTC storage with freeze frame                       │
│  • NVM: Calibration data storage                            │
│  • MCAL: DIO, ADC, PWM, CAN drivers                         │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

#### Implementation Example:

```c
/* File: Main.c */
#include "Os.h"
#include "Rte_Main.h"
#include "SchM_EcuM.h"
#include "SchM_BswM.h"

/* BSW Initialization */
STATIC(void, MAIN_CODE) Bsw_Init(void) {
    /* Initialize Microcontroller */
    Mcu_Init(&Mcu_Config);
    Mcu_InitClock(McuConfClockSetting);
    Mcu_DistributePllClock();
    
    /* Initialize Ports */
    Port_Init(&Port_Config);
    
    /* Initialize DIO */
    Dio_Init(&Dio_Config);
    
    /* Initialize ADC */
    Adc_Init(&Adc_Config);
    
    /* Initialize PWM */
    Pwm_Init(&Pwm_Config);
    
    /* Initialize CAN */
    Can_Init(&Can_Config);
    CanIf_Init(&CanIf_Config);
    PduR_Init(&PduR_Config);
    Com_Init(&Com_Config);
    
    /* Initialize OS */
    Os_Init();
    
    /* Initialize NVM */
    NvM_Init();
    
    /* Initialize DEM */
    Dem_Init();
    
    /* Initialize DCM */
    Dcm_Init();
}

/* Main Entry Point */
int main(void) {
    /* Disable global interrupts */
    DisableInterrupts();
    
    /* Initialize BSW modules */
    Bsw_Init();
    
    /* Initialize RTE */
    Rte_Init();
    
    /* Enable global interrupts */
    EnableInterrupts();
    
    /* Start OS scheduler */
    StartOS(OSDEFAULTAPPMODE);
    
    /* Should never reach here */
    while(1) {
        /* Error handler */
    }
    
    return 0;
}
```

---

## 9. Best Practices

### 9.1 Coding Guidelines

```c
/* ✅ DO: Use AUTOSAR naming conventions */
#define ENGINE_CONTROL_SWC    1U
#define TASK_SENSOR_READ      2U

/* ❌ DON'T: Use arbitrary names */
#define ENG_CTRL              1
#define SENSOR_TASK           2

/* ✅ DO: Use Std_Types */
#include "Std_Types.h"
uint8  value_u8;
uint16 value_u16;
boolean flag_bool;

/* ❌ DON'T: Use standard C types */
unsigned char value;
short value2;
int flag;

/* ✅ DO: Use proper return type checks */
Std_ReturnType result = Rte_Read_Port_Data(&data);
if (result == E_OK) {
    /* Process data */
} else {
    /* Handle error */
}

/* ❌ DON'T: Ignore return values */
Rte_Read_Port_Data(&data);  /* No error check! */
```

### 9.2 Memory Optimization

```c
/* ✅ DO: Use appropriate data types */
typedef uint8  GearPositionType;    /* 0-6 gears */
typedef uint16 EngineSpeedType;     /* 0-8000 RPM */
typedef uint32 OdometerType;        /* 0-999999 km */

/* ❌ DON'T: Use oversized types */
typedef uint32 GearPositionType;    /* Wasteful! */
typedef uint32 EngineSpeedType;     /* Unnecessary */

/* ✅ DO: Use const for read-only data */
CONST(float, ROM_CONST) FuelMap[16][16] = {
    /* Lookup table data */
};

/* ✅ DO: Pack structures efficiently */
typedef struct {
    uint8  status;      /* 1 byte */
    uint16 value;       /* 2 bytes */
    uint8  flags;       /* 1 byte */
} /* Total: 4 bytes */ EfficientStructType;

/* ❌ DON'T: Create inefficient structures */
typedef struct {
    uint8  status;      /* 1 byte + 3 padding */
    uint32 value;       /* 4 bytes */
    uint8  flags;       /* 1 byte + 3 padding */
} /* Total: 12 bytes */ InefficientStructType;
```

### 9.3 Error Handling

```c
/* ✅ DO: Comprehensive error handling */
FUNC(Std_ReturnType, CONTROL_CODE) ControlModule_Execute(void) {
    Std_ReturnType result;
    SensorDataType sensor_data;
    
    /* Read sensor with error check */
    result = Rte_Read_SensorPort_Data(&sensor_data);
    if (result != E_OK) {
        /* Report error to DEM */
        Dem_SetEventStatus(DEM_EVENT_SENSOR_READ_FAILURE, 
                          DEM_EVENT_STATUS_FAILED);
        return result;
    }
    
    /* Validate data */
    if (!ValidateSensorData(sensor_data)) {
        Dem_SetEventStatus(DEM_EVENT_INVALID_SENSOR_DATA,
                          DEM_EVENT_STATUS_FAILED);
        return E_NOT_OK;
    }
    
    /* Process data */
    result = ProcessSensorData(sensor_data);
    if (result != E_OK) {
        return result;
    }
    
    return E_OK;
}

/* ❌ DON'T: Skip error handling */
FUNC(void, CONTROL_CODE) ControlModule_Execute(void) {
    SensorDataType sensor_data;
    
    Rte_Read_SensorPort_Data(&sensor_data);  /* No check! */
    ProcessSensorData(sensor_data);          /* Dangerous! */
}
```

---

## 10. Latihan dan Tantangan

### 10.1 Latihan Dasar

#### Exercise 1: Define Data Types
Buat definisi tipe data untuk sistem transmisi otomatis:
- Gear position (P, R, N, D, S)
- Transmission fluid temperature (-40°C to 150°C)
- Torque converter slip ratio (0% to 100%)
- Shift solenoid states (8 solenoids on/off)

#### Exercise 2: Create Port Interfaces
Definisikan port interfaces untuk komunikasi antara:
- EngineControlSwC → TransmissionControlSwC
- TransmissionControlSwC → GearboxActuatorSwC

#### Exercise 3: Implement Runnable
Implementasikan runnable entity untuk:
- Membaca sensor kecepatan kendaraan
- Menghitung target gear berdasarkan kecepatan dan throttle
- Menulis command ke aktuator shift solenoid

### 10.2 Tantangan Lanjutan

#### Challenge 1: Mode Management
Implementasikan mode management untuk:
- Normal mode
- Sport mode
- Eco mode
- Winter mode

Setiap mode harus memiliki karakteristik shift pattern yang berbeda.

#### Challenge 2: Diagnostic Services
Implementasikan UDS services:
- Read DTC (0x19)
- Clear DTC (0x14)
- Read/Write DID (0x22/0x2E)
- Routine control untuk adaptive learning (0x31)

#### Challenge 3: Safety Mechanisms
Tambahkan safety mechanisms:
- Plausibility checks untuk sensor input
- Timeout monitoring untuk communication
- Limp-home mode activation
- DTC setting untuk fault detection

### 10.3 Project Akhir

**Tujuan:** Membuat complete AUTOSAR-based Transmission Control System

**Deliverables:**
1. System description (ARXML)
2. SWC implementation code
3. BSW configuration
4. Test cases dan validation report

**Timeline:** 2-3 minggu

---

## 📖 Referensi dan Bacaan Lebih Lanjut

### Dokumentasi Resmi
- [AUTOSAR Official Website](https://www.autosar.org)
- [AUTOSAR Classic Platform Documentation](https://www.autosar.org/standards/classic-platform/)
- [AUTOSAR Methodology](https://www.autosar.org/fileadmin/user_upload/standards/classic/4-4/AUTOSAR_TR_Methodology.pdf)

### Buku dan Tutorial
- "AUTOSAR Explained" by Robert Bosch GmbH
- "Introduction to AUTOSAR" by Vector Informatik
- "AUTOSAR Compliance Guidelines" by Elektrobit

### Tools dan Simulator
- [Vector DaVinci Demo Version](https://vector.com)
- [ETAS ISOLAR Trial](https://etas.com)
- [OpenAUTOSAR](https://github.com/OpenAUTOSAR)

### Forum dan Komunitas
- [AUTOSAR Community Forum](https://community.autosar.org)
- [Stack Overflow - AUTOSAR Tag](https://stackoverflow.com/questions/tagged/autosar)
- [LinkedIn AUTOSAR Groups](https://linkedin.com/groups/autosar)

---

## 🎓 Glosarium Istilah Teknis Jepang-Inggris

| Japanese | Romaji | English | Indonesian |
|----------|--------|---------|------------|
| 自動車 | Jidōsha | Automobile | Mobil |
| 制御 | Seigyo | Control | Kontrol |
| ソフトウェア | Sofutowea | Software | Perangkat Lunak |
| アーキテクチャ | Ākitekucha | Architecture | Arsitektur |
| 標準 | Hyōjun | Standard | Standar |
| 通信 | Tsūshin | Communication | Komunikasi |
| 診断 | Shindan | Diagnostic | Diagnostik |
| 安全 | Anzen | Safety | Keselamatan |
| 部品 | Buhin | Component | Komponen |
| 設定 | Settei | Configuration | Konfigurasi |

---

## ✅ Checklist Pemahaman

Setelah mempelajari modul ini, pastikan Anda dapat:

- [ ] Menjelaskan perbedaan Classic vs Adaptive Platform
- [ ] Menggambar arsitektur layer AUTOSAR
- [ ] Menjelaskan fungsi masing-masing layer BSW
- [ ] Membuat definisi Software Component sederhana
- [ ] Memahami konsep Sender-Receiver dan Client-Server
- [ ] Menjelaskan proses code generation dari ARXML
- [ ] Mengimplementasikan runnable entity
- [ ] Memahami mode management dalam AUTOSAR
- [ ] Mengkonfigurasi basic communication stack
- [ ] Menerapkan best practices coding AUTOSAR

---

**Next Module:** [Bootloader Development Guide](./04_Bootloader_Dev_Guide.md)

**Previous Module:** [CAN TP Deep Dive](./09_CAN_TP_DeepDive.md)

**Back to:** [Learning Path Index](./README.md)
