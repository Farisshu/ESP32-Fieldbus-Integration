# 📚 Materi Pembelajaran - Embedded Systems & Communication Protocols

> **Panduan Lengkap**: Memahami konsep dasar hingga implementasi praktis protokol komunikasi embedded (CAN Bus, UART, SPI, I2C, RS485) berdasarkan repository ini.

---

## 📋 Daftar Isi

1. [Pendahuluan](#1-pendahuluan)
2. [Learning Path Terstruktur](#2-learning-path-terstruktur)
3. [Apa itu CAN Bus?](#3-apa-itu-can-bus)
4. [Protokol Komunikasi Serial](#4-protokol-komunikasi-serial)
5. [Perbandingan Protokol](#5-perbandingan-protokol)
6. [Implementasi dalam Repository Ini](#6-implementasi-dalam-repository-ini)
7. [Cara Kerja Sistem](#7-cara-kerja-sistem)
8. [Referensi Kode](#8-referensi-kode)

---

## 1. Pendahuluan

Repository ini berisi implementasi praktis sistem embedded menggunakan **ESP32** dengan berbagai protokol komunikasi:

- **CAN Bus** - Controller Area Network untuk otomotif/industrial
- **UART** - Serial communication dasar
- **SPI** - High-speed synchronous communication
- **I2C** - Two-wire interface untuk sensor/peripheral
- **RS485** - Differential signaling untuk industrial

### Komponen Hardware yang Digunakan

| Komponen | Fungsi | Protokol |
|----------|--------|----------|
| ESP32 Dev Board | Mikrokontroler utama | Semua protokol |
| MCP2515 | CAN controller | SPI |
| TJA1050 | CAN transceiver | CAN Bus |
| ST7735S | TFT Display 128x128 | SPI |
| XY-017 | RS485 to TTL module | UART (RS485) |

---

## 2. Learning Path Terstruktur

### 🎯 Roadmap Pembelajaran

Berikut adalah struktur pembelajaran yang direkomendasikan untuk menguasai embedded systems:

#### Level 1: Fundamental (Wajib)

| Topik | Materi | Status | Prioritas |
|-------|--------|--------|-----------|
| C/C++ Programming | Dasar pemrograman embedded | ✅ Available | ⭐⭐⭐⭐⭐ |
| ARM Cortex-M Architecture | STM32, Renesas basics | 📚 Reference | ⭐⭐⭐⭐⭐ |
| Bare-metal vs RTOS | Understanding execution models | ✅ Available | ⭐⭐⭐⭐⭐ |
| Git & Version Control | Source code management | ✅ Available | ⭐⭐⭐⭐⭐ |

#### Level 2: Communication Protocols (Core)

| Topik | Materi | Status | Prioritas |
|-------|--------|--------|-----------|
| **CAN Bus** | CAN 2.0A, MCP2515, TJA1050 | ✅ Complete | ⭐⭐⭐⭐⭐ |
| UART | Basic serial communication | ✅ Complete | ⭐⭐⭐⭐⭐ |
| SPI | Shared bus, device arbitration | ✅ Complete | ⭐⭐⭐⭐⭐ |
| I2C | Sensor interfacing | 🔜 Planned | ⭐⭐⭐⭐ |
| RS485 | Industrial differential signaling | ✅ Complete | ⭐⭐⭐⭐ |
| Logic Level Shifting | 3.3V ↔ 5V conversion | ✅ Available | ⭐⭐⭐⭐ |

#### Level 3: Advanced Topics

| Topik | Materi | Status | Prioritas |
|-------|--------|--------|-----------|
| FreeRTOS Multitasking | Task scheduling, queues | ✅ Complete | ⭐⭐⭐⭐⭐ |
| Finite State Machine (FSM) | State-based programming | 📚 Reference | ⭐⭐⭐⭐ |
| MISRA C Standard | Coding standards for safety | 📚 Reference | ⭐⭐⭐⭐ |
| Watchdog Timer (WDT) | System reliability | ✅ Available | ⭐⭐⭐⭐ |
| ADC & DAC | Analog interfacing | 🔜 Planned | ⭐⭐⭐ |
| Motor Control & Encoder | PWM, quadrature decoding | 🔜 Planned | ⭐⭐⭐ |

#### Level 4: Industry Standards & Tools

| Topik | Materi | Status | Prioritas |
|-------|--------|--------|-----------|
| CAN TP (ISO 15765-2) | Transport protocol | ⏳ In Progress | ⭐⭐⭐ |
| EtherCAT | Real-time industrial Ethernet | 📚 Reference | ⭐⭐ |
| ESP-NOW & Telemetry | Wireless communication | 🔜 Planned | ⭐⭐ |
| STM32CubeIDE & PlatformIO | Development environments | ✅ Available | ⭐⭐⭐⭐⭐ |
| MATLAB & Simulink | Model-based design | 📚 Reference | ⭐⭐⭐ |
| LTspice | Circuit simulation | 📚 Reference | ⭐⭐⭐ |
| KiCad | PCB design | 📚 Reference | ⭐⭐⭐ |
| Logic Analyzer & Teleplot | Debugging tools | ✅ Available | ⭐⭐⭐⭐⭐ |

#### Level 5: Product Research

| Topik | Materi | Status |
|-------|--------|--------|
| Riset Produk Horiba (ADS EVO) | Automotive diagnostic tools | 📚 Reference |

---

### 📊 Skill Matrix

| Kategori | Skill | Level | Evidence |
|----------|-------|-------|----------|
| **Microcontroller** | ARM Cortex-M (STM32) | Intermediate | Firmware projects |
| | ESP32 | Advanced | Main platform |
| | Renesas | Beginner | Reference only |
| **Protocols** | CAN Bus | Advanced | Two-node implementation |
| | SPI | Advanced | Shared bus with TFT+MCP2515 |
| | UART | Advanced | RS485 Modbus |
| | I2C | Beginner | Planned |
| **Software** | C/C++ | Intermediate | MISRA-compliant code |
| | FreeRTOS | Intermediate | 3-task architecture |
| | FSM | Intermediate | State machines in firmware |
| **Tools** | PlatformIO | Advanced | Primary IDE |
| | Logic Analyzer | Advanced | Automated reporting |
| | Git | Intermediate | Version control |

---

### 🇯🇵 Japanese Technical Terms (Ringkasan)

Untuk materi lengkap bahasa Jepang teknis, lihat: [`/archive/japanese_learning/`](../archive/japanese_learning/)

| English | 日本語 | Romaji | Indonesia |
|---------|--------|--------|-----------|
| Embedded System | 組込システム | komikomi shisutemu | Sistem Tertanam |
| Microcontroller | マイコン | maikon | Mikrokontroler |
| Communication Protocol | 通信プロトコル | tsūshin purotokoru | Protokol Komunikasi |
| Bus Termination | 終端抵抗 | shūtan teikō | Resistor Terminasi |
| Report | 報告書 | hōkokusho | Laporan |
| Testing | テスト | tesuto | Pengujian |

### 3. Apa itu CAN Bus?

### 3.1 Definisi

**CAN (Controller Area Network)** adalah protokol komunikasi serial yang dikembangkan oleh Bosch pada tahun 1983, awalnya untuk industri otomotif. Sekarang digunakan luas di:
- Otomotif (ECU communication)
- Industrial automation
- Medical equipment
- Aerospace

### 3.2 Karakteristik Utama

| Fitur | Deskripsi |
|-------|-----------|
| **Topologi** | Multi-master, broadcast bus |
| **Jarak** | Hingga 40m @ 1 Mbps, hingga 1km @ 50 kbps |
| **Kecepatan** | 5 kbps - 1 Mbps |
| **Node Maksimum** | Hingga 110 node (tergantung transceiver) |
| **Error Detection** | CRC, ACK, Frame Check |
| **Priority** | Message-based arbitration (ID lebih kecil = prioritas lebih tinggi) |

### 3.3 Arsitektur CAN Bus

```
┌──────────┐     ┌──────────┐     ┌──────────┐
│  Node 1  │─────│  Node 2  │─────│  Node 3  │
│ (ESP32+  │     │ (ESP32+  │     │ (ESP32+  │
│ MCP2515) │     │ MCP2515) │     │ MCP2515) │
└────┬─────┘     └────┬─────┘     └────┬─────┘
     │                │                │
     ├────────────────┼────────────────┤
     │      CAN H     │                │
     │      CAN L     │                │
     └────────────────┴────────────────┘
              │
         ┌────┴────┐
         │ 120Ω    │  ← Termination resistor
         └─────────┘
```

### 3.4 Frame Structure (CAN 2.0A - Standard Frame)

```
┌─────────┬──────┬──────┬──────────┬───────────┬─────────┐
│  SOF    │  ID  │ RTR  │   DLC    │   DATA    │   CRC   │
│  1 bit  │ 11bit│ 1 bit│ 4 bits   │ 0-8 bytes │ 15 bits │
└─────────┴──────┴──────┴──────────┴───────────┴─────────┘
```

**Keterangan:**
- **SOF (Start of Frame)**: Menandakan awal transmisi
- **ID**: Identifier pesan (11-bit untuk standard frame, 29-bit untuk extended)
- **RTR (Remote Transmission Request)**: Menunjukkan apakah ini data frame atau remote frame
- **DLC (Data Length Code)**: Panjang data (0-8 bytes)
- **DATA**: Payload data (0-8 bytes)
- **CRC**: Cyclic Redundancy Check untuk error detection

### 3.5 Implementasi dalam Repository

File referensi: `firmware/integration/can_bus_with_tft/src/mcp2515_driver.cpp`

```cpp
// Struktur CAN Frame dalam kode
struct CANFrame {
    uint16_t id;      // 11-bit identifier
    uint8_t dlc;      // Data length (0-8)
    uint8_t data[8];  // Payload data
};

// Contoh penerimaan frame
bool receiveFrame(CANFrame &frame) {
    // Baca status register
    uint8_t status = readRegister(MCP2515_CANINTF);
    
    // Cek apakah ada pesan masuk
    if (status & 0x01) {  // RX0IF flag
        // Baca ID dari register RXBnSIDH dan RXBnSIDL
        uint8_t sidh = readRegister(MCP2515_RXB0SIDH);
        uint8_t sidl = readRegister(MCP2515_RXB0SIDL);
        frame.id = ((sidh << 3) | (sidl >> 5)) & 0x7FF;
        
        // Baca DLC
        frame.dlc = readRegister(MCP2515_RXB0DLC) & 0x0F;
        
        // Baca data
        for (int i = 0; i < frame.dlc; i++) {
            frame.data[i] = readRegister(MCP2515_RXB0D0 + i);
        }
        return true;
    }
    return false;
}
```

### 3.6 Error Handling pada CAN

MCP2515 memiliki register **EFLG (Error Flag)** yang menunjukkan status bus:

```
Bit 7: RXEP - Receive Error Passive
Bit 6: TXEP - Transmit Error Passive  
Bit 5: EWARN - Error Warning Limit reached
Bit 4: - 
Bit 3: TXBO - Bus-Off error
Bit 2: TXWAR - Transmit Error Warning
Bit 1: RXWAR - Receive Error Warning
Bit 0: EWARN (duplicate)
```

**Implementasi monitoring:**
```cpp
// Dari tft_ui.cpp
void updateBusHealth(uint8_t eflg, uint32_t errorCount) {
    if (eflg == 0x00) {
        display.drawString(0, 115, "BUS:OK");
    } else if (eflg < 0xC0) {
        display.drawString(0, 115, "BUS:WARN");
    } else {
        display.drawString(0, 115, "BUS:ERR");
    }
}
```

---

## 4. Protokol Komunikasi Serial

### 3.1 UART (Universal Asynchronous Receiver-Transmitter)

#### Pengertian
UART adalah protokol komunikasi **asynchronous** yang tidak memerlukan clock signal. Data dikirim secara serial bit-by-bit.

#### Karakteristik
| Parameter | Nilai |
|-----------|-------|
| **Wire Count** | 2 (TX, RX) + GND |
| **Mode** | Full-duplex |
| **Speed** | 300 bps - 921600 bps |
| **Distance** | < 15m (TTL), hingga 1.2km (RS485) |
| **Nodes** | Point-to-point (1 TX, 1 RX) |

#### Frame Structure
```
┌───────┬─────────┬───────────┬─────────┬───────┐
│ START │  DATA   │ PARITY    │  STOP   │  IDLE │
│ 1 bit │ 5-9 bits│ 0-1 bit   │ 1-2 bits│ HIGH  │
└───────┴─────────┴───────────┴─────────┴───────┘
```

#### Implementasi dalam Repository

**File:** `firmware/validation/uart_loopback_verification/src/main.cpp`

```cpp
// Konfigurasi UART2 pada ESP32
HardwareSerial rs485Serial(2);

void setup() {
    // Inisialisasi UART dengan baud rate 115200
    rs485Serial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop() {
    // Kirim data
    rs485Serial.println("LOOPBACK_TEST_12345");
    
    // Terima data
    if (rs485Serial.available()) {
        String received = rs485Serial.readStringUntil('\n');
        Serial.println("Received: " + received);
    }
}
```

#### Timing Diagram UART
```
Idle ─┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌────
      │   │ D0│   │ D1│   │ D2│   │ D3│   │ D4│   │ D5│   │ D6│
      └───┘   └───┘   └───┘   └───┘   └───┘   └───┘   └───┘
      START                               STOP    PARITY
```

---

### 3.2 SPI (Serial Peripheral Interface)

#### Pengertian
SPI adalah protokol komunikasi **synchronous** yang dikembangkan oleh Motorola. Menggunakan arsitektur master-slave dengan clock signal.

#### Karakteristik
| Parameter | Nilai |
|-----------|-------|
| **Wire Count** | 4 (SCK, MOSI, MISO, CS) + GND |
| **Mode** | Full-duplex |
| **Speed** | Hingga 50+ MHz |
| **Distance** | < 1m (high speed) |
| **Nodes** | Multi-slave (dengan CS terpisah) |

#### Pin Configuration
| Pin | Nama | Fungsi |
|-----|------|--------|
| SCK | Serial Clock | Clock signal dari master |
| MOSI | Master Out Slave In | Data dari master ke slave |
| MISO | Master In Slave Out | Data dari slave ke master |
| CS/SS | Chip Select / Slave Select | Aktifkan slave tertentu |

#### SPI Modes (Clock Polarity & Phase)
| Mode | CPOL | CPHA | Clock Idle | Data Capture |
|------|------|------|------------|--------------|
| 0 | 0 | 0 | LOW | Rising edge |
| 1 | 0 | 1 | LOW | Falling edge |
| 2 | 1 | 0 | HIGH | Falling edge |
| 3 | 1 | 1 | HIGH | Rising edge |

#### Implementasi dalam Repository

**File:** `firmware/integration/can_bus_with_tft/src/mcp2515_driver.cpp`

```cpp
// Inisialisasi SPI untuk MCP2515
SPIClass* spi = new SPIClass(HSPI);
spi->begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

// Membaca register MCP2515
uint8_t readRegister(uint8_t addr) {
    spi->beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);  // Select MCP2515
    
    // Stabilization delay untuk clone modules
    delayMicroseconds(2);
    
    spi->transfer(MCP2515_READ);
    spi->transfer(addr);
    uint8_t data = spi->transfer(0x00);
    
    digitalWrite(_csPin, HIGH);  // Deselect
    spi->endTransaction();
    
    return data;
}

// Menulis register MCP2515
void writeRegister(uint8_t addr, uint8_t val) {
    spi->beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));
    digitalWrite(_csPin, LOW);
    
    delayMicroseconds(2);
    
    spi->transfer(MCP2515_WRITE);
    spi->transfer(addr);
    spi->transfer(val);
    
    digitalWrite(_csPin, HIGH);
    spi->endTransaction();
}
```

#### SPI Bus Sharing (MCP2515 + TFT)

Repository ini mengimplementasikan **SPI bus sharing** dimana dua device (MCP2515 dan ST7735S TFT) berbagi bus SPI yang sama:

```cpp
// Dari config.h
#define MCP2515_CS  5   // Chip Select untuk MCP2515
#define TFT_CS      17  // Chip Select untuk TFT
#define TFT_DC      16  // Data/Command untuk TFT

// Prinsip kerja:
// - SCK, MOSI, MISO dibagi bersama
// - Setiap device punya CS sendiri
// - Hanya satu device aktif pada satu waktu
```

#### Timing Diagram SPI (Mode 0)
```
SCK  ─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
      └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
      ↑   ↑   ↑   ↑   ↑   ↑   ↑   ↑
      D7  D6  D5  D4  D3  D2  D1  D0
      
CS   ────────┐                       ┌────
             └───────────────────────┘
             Device Active
```

---

### 3.3 I2C (Inter-Integrated Circuit)

#### Pengertian
I2C adalah protokol komunikasi **synchronous** yang dikembangkan oleh Philips. Menggunakan hanya 2 wire untuk komunikasi multi-device.

#### Karakteristik
| Parameter | Nilai |
|-----------|-------|
| **Wire Count** | 2 (SDA, SCL) + GND |
| **Mode** | Half-duplex |
| **Speed** | 100 kbps (Standard), 400 kbps (Fast), 3.4 Mbps (High-speed) |
| **Distance** | < 1m |
| **Nodes** | Hingga 127 devices (7-bit address) |

#### Pin Configuration
| Pin | Nama | Fungsi |
|-----|------|--------|
| SDA | Serial Data | Data bidirectional |
| SCL | Serial Clock | Clock dari master |

#### Protocol Overview
```
START → ADDRESS + R/W → ACK → DATA → ACK → ... → STOP
```

#### Implementasi dalam Repository

Meskipun repository ini fokus pada SPI dan CAN, I2C tersedia di ESP32 untuk sensor tambahan:

```cpp
#include <Wire.h>

void setup() {
    Wire.begin(SDA_PIN, SCL_PIN);
    
    // Scan I2C devices
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.printf("Device found at 0x%02X\n", addr);
        }
    }
}
```

#### Timing Diagram I2C
```
SDA  ────┐   ┌───────┐   ┌───────┐   ┌───────┐   ┌────
         │   │ DATA  │   │ DATA  │   │ DATA  │   │
         └───┘       └───┘       └───┘       └───┘
         
SCL  ─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
      └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
      
      START           ACK           ACK          STOP
```

---

### 3.4 RS485

#### Pengertian
RS485 adalah standar komunikasi **differential signaling** untuk industrial applications. Mendukung multi-drop network hingga 32 nodes (bisa lebih dengan repeater).

#### Karakteristik
| Parameter | Nilai |
|-----------|-------|
| **Wire Count** | 2 (A, B) + GND |
| **Mode** | Half-duplex (standar) atau Full-duplex |
| **Speed** | Hingga 10 Mbps |
| **Distance** | Hingga 1.2 km |
| **Nodes** | Hingga 32 (tanpa repeater) |

#### Differential Signaling
```
Logic 1: A > B (V_A - V_B > +200mV)
Logic 0: A < B (V_A - V_B < -200mV)
```

#### Pin Configuration (XY-017 Module)
| Pin | Fungsi | Keterangan |
|-----|--------|------------|
| VCC | Power | 5V atau 3.3V (sesuai jumper) |
| GND | Ground | Common ground wajib |
| RO | Receiver Output | Connected to ESP32 RX |
| DI | Driver Input | Connected to ESP32 TX |
| DE/RE | Direction Enable | HIGH=Transmit, LOW=Receive |
| A | Non-inverting | Differential pair (+) |
| B | Inverting | Differential pair (-) |

#### Implementasi dalam Repository

**File:** `firmware/projects/rs485_master_slave/src/master.cpp`

```cpp
#define RS485_DE_RE 4   // Direction control pin
#define RS485_TX    17  // TX2
#define RS485_RX    16  // RX2

HardwareSerial rs485Serial(2);

void setup() {
    pinMode(RS485_DE_RE, OUTPUT);
    digitalWrite(RS485_DE_RE, LOW);  // Start in receive mode
    
    rs485Serial.begin(115200, SERIAL_8N1, RS485_RX, RS485_TX);
}

void transmitData(String data) {
    // Switch to transmit mode
    digitalWrite(RS485_DE_RE, HIGH);
    delayMicroseconds(100);  // Wait for mode switch
    
    // Send data
    rs485Serial.println(data);
    rs485Serial.flush();  // Wait for transmission complete
    
    // Switch back to receive mode
    digitalWrite(RS485_DE_RE, LOW);
    delayMicroseconds(100);
}

void loop() {
    // Transmit command every 3 seconds
    transmitData("CMD:READ_DATA|TS:" + String(millis()));
    
    // Listen for response
    if (rs485Serial.available()) {
        String response = rs485Serial.readStringUntil('\n');
        Serial.println("Response: " + response);
    }
    
    delay(3000);
}
```

#### RS485 Network Topology
```
┌──────────┐     ┌──────────┐     ┌──────────┐
│  Master  │─────│  Slave 1 │─────│  Slave 2 │
│  (ESP32) │     │  (ESP32) │     │  (ESP32) │
└────┬─────┘     └────┬─────┘     └────┬─────┘
     │                │                │
     ├──── A ─────────┼────────────────┤
     ├──── B ─────────┼────────────────┤
     └──── GND ───────┴────────────────┘
     
     ┌─────────┐
     │ 120Ω    │  ← Termination resistor (at ends only)
     └─────────┘
```

---

## 5. Perbandingan Protokol

### Tabel Perbandingan

| Fitur | UART | SPI | I2C | CAN Bus | RS485 |
|-------|------|-----|-----|---------|-------|
| **Wire Count** | 2-3 | 4+ | 2 | 2+ | 2+ |
| **Sync/Async** | Async | Sync | Sync | Sync | Async |
| **Duplex** | Full | Full | Half | Broadcast | Half |
| **Max Speed** | ~1 Mbps | 50+ MHz | 3.4 Mbps | 1 Mbps | 10 Mbps |
| **Max Distance** | 15m (TTL) | <1m | <1m | 1km (@50kbps) | 1.2km |
| **Max Nodes** | 2 | Multi-slave | 127 | 110 | 32+ |
| **Addressing** | None | CS lines | Address | Message ID | Manual |
| **Error Detection** | Parity (opt) | None | ACK/CRC | CRC/ACK | Manual |
| **Complexity** | Low | Medium | Medium | High | Medium |
| **Cost** | Low | Low | Low | Medium-High | Low |
| **Use Case** | Debug, GPS | Display, SD card | Sensors | Automotive | Industrial |

### Kapan Menggunakan Setiap Protokol?

#### Gunakan UART jika:
- ✅ Komunikasi point-to-point sederhana
- ✅ Debugging via serial monitor
- ✅ Interface dengan GPS, Bluetooth, WiFi module
- ✅ Tidak butuh kecepatan tinggi

#### Gunakan SPI jika:
- ✅ Butuh kecepatan tinggi
- ✅ Komunikasi dengan display, SD card, flash memory
- ✅ Full-duplex communication diperlukan
- ✅ Jarak pendek (<1m)

#### Gunakan I2C jika:
- ✅ Banyak device dengan wire minimal
- ✅ Komunikasi dengan sensor (temperature, humidity, IMU)
- ✅ Kecepatan tidak kritis
- ✅ Jarak pendek

#### Gunakan CAN Bus jika:
- ✅ Aplikasi otomotif atau industrial
- ✅ Butuh reliability tinggi dengan error detection
- ✅ Multi-master network
- ✅ Noise immunity penting
- ✅ Jarak menengah (hingga 1km)

#### Gunakan RS485 jika:
- ✅ Industrial environment dengan noise tinggi
- ✅ Jarak jauh (hingga 1.2km)
- ✅ Multi-drop network (Modbus RTU)
- ✅ Biaya rendah untuk long-distance

---

## 6. Implementasi dalam Repository Ini

### 5.1 Struktur Folder

```
/workspace/
├── firmware/
│   ├── tests/                    # Unit tests
│   │   ├── mcp2515_can/          # Test CAN controller via SPI
│   │   ├── st7735s_tft/          # Test TFT display via SPI
│   │   └── tft_mcp2515_combined/ # Test SPI bus sharing
│   ├── validation/               # Validation tests
│   │   ├── uart_loopback_verification/  # UART test
│   │   └── mcp2515_spi_validation/      # SPI validation
│   ├── projects/                 # Production projects
│   │   ├── rs485_master_slave/   # RS485 communication
│   │   └── rs485_loopback_test/  # RS485 hardware test
│   └── integration/              # Integrated systems
│       ├── can_two_nodes/        # CAN communication 2 nodes
│       └── can_bus_with_tft/     # CAN monitor with display
├── software/                     # Python tools
│   ├── analyze_la_pro.py         # Logic analyzer report generator
│   └── examples/                 # Sample data
└── docs/                         # Documentation
    ├── test_procedures.md        # Testing SOP
    └── PROJECT_CONTEXT.md        # Project overview
```

### 5.2 FreeRTOS Architecture

Repository ini menggunakan **FreeRTOS** untuk multitasking:

```cpp
// Dari app_tasks.cpp
void vTaskCAN(void *pvParameters) {
    // Poll CAN bus setiap 20ms (50Hz)
    while (1) {
        if (can->receiveFrame(frame)) {
            xQueueSend(canFrameQueue, &frame, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void vTaskUI(void *pvParameters) {
    // Update display setiap 100ms (10Hz)
    while (1) {
        if (xQueueReceive(canFrameQueue, &frame, 0)) {
            ui.displayFrame(frame);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vTaskLogger(void *pvParameters) {
    // Log ke LittleFS setiap 100ms
    while (1) {
        logger.batchWrite(frames);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

### 5.3 SPI Bus Sharing Implementation

```cpp
// Dari main.cpp
SPIClass* hspi = nullptr;
MCP2515Driver* can = nullptr;
TFT_UI* ui = nullptr;

void setup() {
    // Initialize shared SPI bus
    hspi = new SPIClass(HSPI);
    hspi->begin(SCK_PIN, MISO_PIN, MOSI_PIN);
    
    // Initialize devices with separate CS pins
    can = new MCP2515Driver(MCP2515_CS);
    can->begin();
    
    ui = new TFT_UI(TFT_CS, TFT_DC, TFT_RST, hspi);
    ui->begin();
}
```

---

## 7. Cara Kerja Sistem

### 6.1 Flow Diagram - CAN Bus Monitor

```
┌─────────────────────────────────────────────────────────────┐
│                      POWER ON                                │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  INITIALIZATION                                             │
│  - Initialize SPI bus (250kHz, MODE0)                       │
│  - Initialize MCP2515 (500kbps CAN)                         │
│  - Initialize TFT display                                   │
│  - Initialize LittleFS                                      │
│  - Create FreeRTOS tasks                                    │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  TASK: vTaskCAN (Core 1, Priority 2, 20ms interval)         │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ 1. Poll MCP2515 for incoming frames                 │   │
│  │ 2. Validate frame (ID, DLC, CRC)                    │   │
│  │ 3. Read EFLG register for bus health                │   │
│  │ 4. Push valid frame to queue                        │   │
│  │ 5. Log error if EFLG != 0                           │   │
│  └─────────────────────────────────────────────────────┘   │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  TASK: vTaskUI (Core 0, Priority 1, 100ms interval)         │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ 1. Receive frame from queue (max 20 per cycle)      │   │
│  │ 2. Update display: ID, Data, Counter                │   │
│  │ 3. Update bus health indicator (OK/WARN/ERR)        │   │
│  │ 4. Refresh at 5Hz to prevent flicker                │   │
│  └─────────────────────────────────────────────────────┘   │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  TASK: vTaskLogger (Core 1, Priority 1, 100ms interval)     │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ 1. Batch receive frames from queue (max 10)         │   │
│  │ 2. Append to CSV file in LittleFS                   │   │
│  │ 3. Format: timestamp,id,dlc,data0,data1,...,data7   │   │
│  └─────────────────────────────────────────────────────┘   │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│  CONTINUOUS OPERATION                                       │
│  - All tasks run concurrently                               │
│  - Queue prevents blocking                                  │
│  - Error monitoring active                                  │
└─────────────────────────────────────────────────────────────┘
```

### 6.2 Data Flow

```
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│   CAN Bus    │────▶│   MCP2515    │────▶│    ESP32     │
│  (Physical)  │     │  (SPI Ctrl)  │     │  (via SPI)   │
└──────────────┘     └──────────────┘     └───────┬──────┘
                                                   │
                                                   ▼
                                          ┌────────────────┐
                                          │  canFrameQueue │
                                          │  (FreeRTOS)    │
                                          └───────┬────────┘
                                                  │
                          ┌───────────────────────┼───────────────────────┐
                          │                       │                       │
                          ▼                       ▼                       ▼
                   ┌─────────────┐        ┌─────────────┐        ┌─────────────┐
                   │  vTaskUI    │        │ vTaskLogger │        │  Serial     │
                   │  (Display)  │        │  (LittleFS) │        │  (Debug)    │
                   └─────────────┘        └─────────────┘        └─────────────┘
```

---

## 8. Referensi Kode

### 7.1 File-file Penting

#### Firmware Files

| File | Lokasi | Deskripsi |
|------|--------|-----------|
| `main.cpp` | `firmware/integration/can_bus_with_tft/src/` | Entry point, task creation |
| `config.h` | `firmware/integration/can_bus_with_tft/src/` | Pin definitions, constants |
| `mcp2515_driver.cpp/h` | `firmware/integration/can_bus_with_tft/src/` | CAN controller driver |
| `tft_ui.cpp/h` | `firmware/integration/can_bus_with_tft/src/` | TFT display driver |
| `app_tasks.cpp/h` | `firmware/integration/can_bus_with_tft/src/` | FreeRTOS tasks |
| `logger.cpp/h` | `firmware/integration/can_bus_with_tft/src/` | LittleFS logging |

#### Test Files

| File | Lokasi | Deskripsi |
|------|--------|-----------|
| `master.cpp` | `firmware/projects/rs485_master_slave/src/` | RS485 master node |
| `slave.cpp` | `firmware/projects/rs485_master_slave/src/` | RS485 slave node |
| `main.cpp` | `firmware/tests/mcp2515_can/src/` | MCP2515 unit test |
| `main.cpp` | `firmware/validation/uart_loopback_verification/src/` | UART loopback test |

#### Software Files

| File | Lokasi | Deskripsi |
|------|--------|-----------|
| `analyze_la_pro.py` | `software/` | Logic analyzer report generator |
| `generate_samples.py` | `software/` | Synthetic test data generator |

### 7.2 Key Functions Reference

#### MCP2515 Driver Functions

```cpp
// Initialization
bool MCP2515Driver::begin();

// Register access
uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t val);
void readRegisters(uint8_t startAddr, uint8_t* buffer, uint8_t count);

// CAN operations
bool receiveFrame(CANFrame &frame);
uint8_t getErrorFlags();
```

#### UI Functions

```cpp
// Display initialization
void TFT_UI::begin();

// Frame display
void TFT_UI::displayFrame(const CANFrame& frame);

// Bus health monitoring
void TFT_UI::updateBusHealth(uint8_t eflg, uint32_t errorCount);
```

#### Logger Functions

```cpp
// Logging initialization
bool Logger::begin();

// Batch write
void Logger::batchWrite(const QueuedMessage* frames, size_t count);

// File management
void Logger::rotateFile();
```

### 7.3 Python Analysis Tool

```python
# Usage example
python software/analyze_la_pro.py capture.csv \
  --operator "Your Name" \
  --dut "ESP32 + MCP2515" \
  --purpose "CAN Bus SPI Verification" \
  --sample-rate 8

# Output files
Archive_YYYYMMDD_HHMMSS/
├── professional_report.txt   # Human-readable report
├── waveform_annotated.png    # Annotated timing diagram
├── metadata.json             # Machine-readable results
└── la_analysis.log           # Debug log
```

---

## 📖 Referensi Tambahan

### Dokumentasi Resmi
- [MCP2515 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/21801e.pdf)
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [CAN Bus Specification 2.0](https://www.bosch-semiconductors.de/media/pdf_1/canliteratur/can2spec.pdf)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/api.html)

### Standar Industri
- **ISO 11898** - CAN Bus standard
- **ISO 15765-2** - CAN Transport Protocol (planned)
- **Modbus RTU** - RS485 protocol standard
- **RS-485 Standard** - TIA/EIA-485

### Tutorial Online
- [CAN Bus Explained](https://www.csselectronics.com/pages/can-bus-simple-intro)
- [SPI vs I2C vs UART](https://www.protoexpress.com/blog/differences-between-spi-i2c-and-uart-protocols/)
- [ESP32 FreeRTOS Tutorial](https://randomnerdtutorials.com/guide-for-freertos-with-esp32-arduino-ide/)

---

## 🎯 Kesimpulan

Repository ini memberikan implementasi lengkap dari berbagai protokol komunikasi embedded:

1. **CAN Bus** - Untuk aplikasi otomotif/industrial dengan reliability tinggi
2. **SPI** - Untuk high-speed communication dengan peripheral
3. **UART** - Untuk debugging dan simple serial communication
4. **RS485** - Untuk industrial long-distance communication
5. **I2C** - Tersedia untuk sensor integration

Setiap protokol memiliki karakteristik unik dan use case spesifik. Pemilihan protokol tergantung pada:
- Kebutuhan kecepatan
- Jarak komunikasi
- Jumlah node
- Environment (noise level)
- Budget dan kompleksitas

Semua implementasi dalam repository ini mengikuti best practices:
- ✅ Modular architecture
- ✅ Error handling
- ✅ Professional documentation
- ✅ Testing & validation
- ✅ FreeRTOS integration untuk real-time operation

---

*Dokumentasi ini dibuat sebagai referensi pembelajaran berdasarkan repository `esp32-logic-analyzer-automation`.*  
*Author: M. Faris A. G. | Last Updated: 2026*
