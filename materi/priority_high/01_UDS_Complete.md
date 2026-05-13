# 📘 UDS (Unified Diagnostic Services) - ISO 14229

> **Panduan Lengkap**: Memahami protokol diagnostik kendaraan modern untuk embedded engineers  
> **完全ガイド**: 現代車両診断プロトコルの理解 - エンジニア向け  
> **Complete Guide**: Understanding modern vehicle diagnostic protocols for embedded engineers

---

## 📋 Daftar Isi

1. [Pendahuluan](#1-pendahuluan)
2. [Apa itu UDS?](#2-apa-itu-uds)
3. [Arsitektur Diagnostik Kendaraan](#3-arsitektur-diagnostik-kendaraan)
4. [Layanan UDS Utama](#4-layanan-uds-utama)
5. [Struktur Pesan UDS](#5-struktur-pesan-uds)
6. [Implementasi Praktis](#6-implementasi-praktis)
7. [Testing & Simulation](#7-testing--simulation)
8. [Troubleshooting](#8-troubleshooting)
9. [Referensi & Resources](#9-referensi--resources)

---

## 1. Pendahuluan

### 1.1 Latar Belakang

**Unified Diagnostic Services (UDS)** adalah protokol diagnostik standar yang didefinisikan dalam **ISO 14229**. Protokol ini digunakan secara luas di industri otomotif untuk:

- **Diagnostic Communication** antara tester dan ECU
- **Fault Code Reading** (DTC - Diagnostic Trouble Codes)
- **Software Updates** (Flashing/Reprogramming)
- **Parameter Configuration** (Read/Write Data by Identifier)
- **Session Control** (Access different operational modes)

### 1.2 Hubungan dengan Protokol Lain

```
┌─────────────────────────────────────────────────────────────┐
│                    Diagnostic Stack                          │
├─────────────────────────────────────────────────────────────┤
│  Application Layer    →  UDS (ISO 14229)                    │
│  Network Layer        →  CAN TP (ISO 15765-2)               │
│  Data Link Layer      →  CAN Bus (ISO 11898)                │
│  Physical Layer       →  ISO 11898-2/5 (High-Speed/Low-Speed)│
└─────────────────────────────────────────────────────────────┘
```

### 1.3 Prasyarat Pembelajaran

Sebelum mempelajari UDS, pastikan Anda memahami:

| Topik | Status | Reference |
|-------|--------|-----------|
| CAN Bus 2.0A | ✅ Wajib | [CAN Bus Guide](../README_MATERI_LENGKAP.md#3-apa-itu-can-bus) |
| CAN TP (ISO 15765-2) | ⚠️ Disarankan | [CAN TP Guide](./01_CAN_TP.md) |
| C/C++ Programming | ✅ Wajib | Basic embedded programming |
| Hexadecimal System | ✅ Wajib | Number conversion |

---

## 2. Apa itu UDS?

### 2.1 Definisi

**UDS (Unified Diagnostic Services)** adalah protokol layer aplikasi (OSI Layer 7) yang menyediakan layanan diagnostik terstandarisasi untuk sistem elektronik kendaraan.

### 2.2 Sejarah Perkembangan

| Tahun | Standar | Deskripsi |
|-------|---------|-----------|
| 1990-an | OBD-II | On-Board Diagnostics (US market) |
| 2000-an | EOBD | European OBD (EN 14229) |
| 2006 | ISO 14229-1 | UDS on CAN (First edition) |
| 2013 | ISO 14229-1:2013 | Second edition (Current widely used) |
| 2020 | ISO 14229-1:2020 | Third edition (Latest) |

### 2.3 Aplikasi di Industri

| Manufacturer | Implementation | Tools |
|--------------|----------------|-------|
| Toyota | Global OBD + UDS | Techstream |
| Volkswagen | VAS-PC + UDS | VCDS, ODIS |
| BMW | ISTA/D + UDS | INPA, Tool32 |
| Mercedes-Benz | XENTRY + UDS | Star Diagnosis |
| Honda | HDS + UDS | i-HDS |

---

## 3. Arsitektur Diagnostik Kendaraan

### 3.1 Komponen Sistem Diagnostik

```
┌─────────────────┐         ┌─────────────────┐
│   Diagnostic    │         │      Vehicle    │
│     Tester      │◄───────►│       ECU       │
│   (Client)      │  UDS    │    (Server)     │
│                 │  over   │                 │
│  - Laptop/PC    │  CAN    │  - Engine Ctrl  │
│  - Scan Tool    │         │  - Transmission │
│  - OEM Tool     │         │  - ABS/ESP      │
└─────────────────┘         └─────────────────┘
```

### 3.2 Client-Server Relationship

| Role | Description | Example |
|------|-------------|---------|
| **Client** | Initiates diagnostic requests | Diagnostic tester, scan tool |
| **Server** | Responds to client requests | ECU (Engine, TCU, BCM, etc.) |

### 3.3 Communication Flow

```
Client                    Server
  │                         │
  │  ─── Request ─────────► │  (SID + Sub-function + Data)
  │                         │
  │  ◄─── Response ──────── │  (Positive: SID+0x40 or Negative: 0x7F)
  │                         │
```

---

## 4. Layanan UDS Utama

### 4.1 Overview Layanan

UDS mendefinisikan **Service Identifiers (SID)** untuk setiap jenis layanan. Berikut adalah layanan utama yang wajib diketahui:

| SID (Hex) | Service Name | Deskripsi | Prioritas |
|-----------|--------------|-----------|-----------|
| **0x10** | Diagnostic Session Control | Mengubah mode operasi ECU | 🔴 Tinggi |
| **0x11** | ECU Reset | Reset ECU (hard/soft/power cycle) | 🔴 Tinggi |
| **0x27** | Security Access | Unlock protected functions | 🔴 Tinggi |
| **0x2E** | Write Data By Identifier | Menulis parameter konfigurasi | 🟡 Sedang |
| **0x22** | Read Data By Identifier | Membaca data sensor/parameter | 🔴 Tinggi |
| **0x85** | Control DTC Setting | Enable/disable DTC storage | 🟡 Sedang |
| **0x31** | Routine Control | Execute predefined routines | 🟡 Sedang |
| **0x34/36/37** | Request Download/Transfer/Exit | Flash programming sequence | 🔴 Tinggi |
| **0x19** | Read DTC Information | Read fault codes | 🔴 Tinggi |
| **0x14** | Clear DTC Information | Clear fault codes | 🟡 Sedang |

### 4.2 Detail Layanan

#### 4.2.1 0x10 - Diagnostic Session Control

Mengubah session diagnostik ECU ke mode operasi berbeda:

| Sub-function | Session Type | Deskripsi |
|--------------|--------------|-----------|
| 0x01 | Default Session | Mode normal, semua fungsi basic tersedia |
| 0x02 | Programming Session | Untuk flashing/reprogramming |
| 0x03 | Extended Diagnostic Session | Akses fungsi diagnostik lengkap |
| 0x40-0x5F | Safety System Session | Untuk airbag, ABS, dll |

**Request Example:**
```
Request:  10 03
Response: 50 03 00 32 01 F4
           │  │  │  │  └─── Min timing between messages (ms)
           │  │  │  └────── Max timing between messages (ms)  
           │  │  └───────── Reserved by OEM
           │  └──────────── Session type (echo)
           └─────────────── Positive response (0x50 = 0x10 + 0x40)
```

#### 4.2.2 0x11 - ECU Reset

Melakukan reset pada ECU:

| Sub-function | Reset Type | Deskripsi |
|--------------|------------|-----------|
| 0x01 | Hard Reset | Reset penuh seperti power cycle |
| 0x02 | Key Off-On Reset | Reset setelah ignition cycle |
| 0x03 | Soft Reset | Reset software tanpa power cycle |
| 0x04 | Enable Rapid Power Shutdown | Persiapan sleep mode |
| 0x05 | Disable Rapid Power Shutdown | Wake up dari sleep |

**Request Example:**
```
Request:  11 01
Response: 51 01
```

#### 4.2.3 0x27 - Security Access

Proses autentikasi untuk mengakses fungsi terbatas:

**Step 1: Request Seed**
```
Request:  27 01          (Send key level 1)
Response: 67 01 A1 B2 C3 D4  (Return 4-byte seed)
```

**Step 2: Send Key**
```
Calculation: key = f(seed) using OEM algorithm
Request:  27 02 K1 K2 K3 K4  (Send calculated key)
Response: 67 02              (Security unlocked)
```

**Negative Response jika gagal:**
```
Response: 7F 27 35  (Invalid key)
```

#### 4.2.4 0x22 - Read Data By Identifier

Membaca data berdasarkan DID (Data Identifier):

**Standard DID Range:**
| DID Range | Type | Example |
|-----------|------|---------|
| 0xF180-0xF1FF | Vehicle Info | VIN (0xF190) |
| 0xF170-0xF17F | System Info | Software version |
| 0x2200-0x22FF | Sensor Data | Engine RPM, temperature |
| 0x3100-0x31FF | Calibration | Fuel trim, ignition timing |

**Request Example (Read VIN):**
```
Request:  22 F1 90
Response: 62 F1 90 57 42 31 32 33 34 35 36 37 38 39 30
           │  │  │  └───────────────────────────────── VIN: WB1234567890
           │  │  └──────────────────────────────────── DID echo
           │  └─────────────────────────────────────── Positive response
```

#### 4.2.5 0x2E - Write Data By Identifier

Menulis parameter konfigurasi:

**Request Example:**
```
Request:  2E F1 91 4D 59 43 41 52  (Write "MYCAR" to DID 0xF191)
Response: 6E F1 91
```

#### 4.2.6 0x19 - Read DTC Information

Membaca kode kesalahan (Diagnostic Trouble Codes):

**Sub-functions:**
| Sub-function | Deskripsi |
|--------------|-----------|
| 0x02 | Read DTC by Status Mask |
| 0x04 | Read Snapshot Record by DTCNumber |
| 0x0A | Read Supported DTCs |
| 0x12 | Read DTC Fault Counter |
| 0x18 | Read DTC with Permanent Status |

**Request Example:**
```
Request:  19 02 FF  (Read all DTCs with any status)
Response: 19 02 0A 0C 01 05 12 34 56 78 9A BC DE F0 11 22
           │  │  │  └──────────────────────────────────── DTC data
           │  │  └─────────────────────────────────────── DTC count
           │  └────────────────────────────────────────── Sub-function echo
           └───────────────────────────────────────────── Positive response
```

#### 4.2.7 0x34/0x36/0x37 - Download Sequence

Sequence untuk flashing ECU:

**Step 1: Request Download (0x34)**
```
Request:  34 00 44 00 00 10 00  (Prepare download, 0x1000 bytes)
Response: 36 20 10 40  (Max block length info)
```

**Step 2: Transfer Data (0x36)**
```
Request:  36 01 AA BB CC DD ...  (Block 1 data)
Response: 37 01
Request:  36 02 EE FF 00 11 ...  (Block 2 data)
Response: 37 02
...
```

**Step 3: Transfer Exit (0x37)**
```
Request:  37 01
Response: 77 01
```

---

## 5. Struktur Pesan UDS

### 5.1 Format Pesan Single Frame

```
┌─────────┬───────────┬────────────────────────────────┐
│  Length │    SID    │            Data                │
│ (1 byte)│ (1 byte)  │        (0-7 bytes)             │
└─────────┴───────────┴────────────────────────────────┘
```

### 5.2 Positive Response Format

```
┌─────────┬───────────┬────────────────────────────────┐
│  Length │ SID+0x40  │            Data                │
│ (1 byte)│ (1 byte)  │        (Variable)              │
└─────────┴───────────┴────────────────────────────────┘
```

**Contoh:**
- Request SID 0x10 → Response SID 0x50 (0x10 + 0x40)
- Request SID 0x22 → Response SID 0x62 (0x22 + 0x40)

### 5.3 Negative Response Format

```
┌─────────┬───────────┬───────────┐
│  Length │  0x7F     │   NRC     │
│ (1 byte)│ (1 byte)  │ (1 byte)  │
└─────────┴───────────┴───────────┘
```

**Common Negative Response Codes (NRC):**

| NRC (Hex) | Name | Deskripsi |
|-----------|------|-----------|
| 0x10 | generalReject | Request tidak valid |
| 0x11 | serviceNotSupported | SID tidak didukung |
| 0x12 | subFunctionNotSupported | Sub-function tidak didukung |
| 0x21 | busyRepeatRequest | ECU sibuk, coba lagi |
| 0x22 | conditionsNotCorrect | Kondisi tidak terpenuhi |
| 0x24 | requestSequenceError | Urutan request salah |
| 0x25 | noResponseFromSubnetComponent | Sub-component timeout |
| 0x26 | pendingResponse | Response sedang diproses |
| 0x31 | requestOutOfRange | Parameter di luar range |
| 0x33 | securityAccessDenied | Security lock aktif |
| 0x35 | invalidKey | Key authentication gagal |
| 0x36 | exceededNumberOfAttempts | Terlalu banyak percobaan |
| 0x37 | requiredTimeDelay | Timer belum expire |
| 0x78 | responsePending | Perlu waktu lebih lama (keep-alive) |
| 0x83 | uploadDownloadNotAccepted | Transfer ditolak |

### 5.4 Multi-Frame Messages (via CAN TP)

Untuk pesan > 7 bytes, digunakan CAN Transport Protocol:

| Frame Type | PCI | Deskripsi |
|------------|-----|-----------|
| Single Frame (SF) | 0x0X | Data ≤ 7 bytes |
| First Frame (FF) | 0x1X | Frame pertama (> 7 bytes) |
| Consecutive Frame (CF) | 0x2X | Frame lanjutan |
| Flow Control (FC) | 0x3X | Acknowledgement dari receiver |

**Contoh Transfer Panjang (Read VIN via CAN TP):**

```
Tester                        ECU
  │                           │
  │  First Frame (FF)         │
  │  ───────────────────────► │  10 14 22 F1 90
  │                           │  (Total length: 0x014 = 20 bytes)
  │                           │
  │  Flow Control (FC)        │
  │  ◄─────────────────────── │  30 00 03
  │                           │  (CTS: 0, BS: 0, STmin: 3ms)
  │                           │
  │  Consecutive Frame (CF)   │
  │  ◄─────────────────────── │  21 01 57 42 31 32 33 34
  │                           │
  │  Consecutive Frame (CF)   │
  │  ◄─────────────────────── │  22 02 35 36 37 38 39 30
  │                           │
```

---

## 6. Implementasi Praktis

### 6.1 Struktur Data UDS

```cpp
// uds_types.h
#ifndef UDS_TYPES_H
#define UDS_TYPES_H

#include <stdint.h>
#include <stddef.h>

// Service Identifiers
#define UDS_SID_DIAG_SESSION_CTRL   0x10
#define UDS_SID_ECU_RESET           0x11
#define UDS_SID_SECURITY_ACCESS     0x27
#define UDS_SID_READ_DATA_BY_ID     0x22
#define UDS_SID_WRITE_DATA_BY_ID    0x2E
#define UDS_SID_CONTROL_DTC         0x85
#define UDS_SID_ROUTINE_CTRL        0x31
#define UDS_SID_READ_DTC_INFO       0x19
#define UDS_SID_CLEAR_DTC_INFO      0x14

// Negative Response Codes
#define UDS_NRC_POSITIVE_RESPONSE   0x00
#define UDS_NRC_GENERAL_REJECT      0x10
#define UDS_NRC_SERVICE_NOT_SUPPORTED 0x11
#define UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED 0x12
#define UDS_NRC_BUSY_REPEAT_REQUEST 0x21
#define UDS_NRC_CONDITIONS_NOT_CORRECT 0x22
#define UDS_NRC_REQUEST_OUT_OF_RANGE 0x31
#define UDS_NRC_SECURITY_ACCESS_DENIED 0x33
#define UDS_NRC_INVALID_KEY         0x35
#define UDS_NRC_RESPONSE_PENDING    0x78

// Session Types
#define UDS_SESSION_DEFAULT         0x01
#define UDS_SESSION_PROGRAMMING     0x02
#define UDS_SESSION_EXTENDED        0x03

// Reset Types
#define UDS_RESET_HARD              0x01
#define UDS_RESET_KEY_OFF_ON        0x02
#define UDS_RESET_SOFT              0x03

// Structure untuk UDS Request
typedef struct {
    uint8_t sid;           // Service Identifier
    uint8_t sub_function;  // Sub-function (jika ada)
    uint16_t did;          // Data Identifier (untuk 0x22/0x2E)
    uint8_t* data;         // Payload data
    size_t data_length;    // Panjang data
} UDSRequest;

// Structure untuk UDS Response
typedef struct {
    uint8_t sid;           // Service Identifier (+0x40 untuk positive)
    uint8_t nrc;           // Negative Response Code (0x00 = success)
    uint8_t* data;         // Response data
    size_t data_length;    // Panjang response
    bool is_negative;      // Flag negative response
} UDSResponse;

// Function result codes
typedef enum {
    UDS_OK = 0,
    UDS_ERR_TIMEOUT,
    UDS_ERR_NEGATIVE_RESPONSE,
    UDS_ERR_INVALID_PARAM,
    UDS_ERR_TRANSPORT,
    UDS_ERR_SECURITY_LOCKED
} UDSResult;

#endif // UDS_TYPES_H
```

### 6.2 UDS Client Implementation

```cpp
// uds_client.cpp
#include "uds_client.h"
#include "can_tp.h"  // CAN Transport Protocol layer
#include <string.h>
#include <Arduino.h>

class UDSClient {
private:
    uint32_t server_ecu_id;    // CAN ID ECU server
    uint32_t tester_id;        // CAN ID tester
    uint32_t p2_timer;         // P2 timer (max response time)
    uint32_t s3_timer;         // S3 timer (session timeout)
    uint8_t current_session;   // Current active session
    
    CAN_TP* can_tp;            // Pointer to CAN TP layer
    
public:
    UDSClient(uint32_t ecu_id, uint32_t tester_addr, CAN_TP* tp) 
        : server_ecu_id(ecu_id), tester_id(tester_addr), can_tp(tp) {
        p2_timer = 50;   // 50ms default P2
        s3_timer = 5000; // 5s default S3
        current_session = UDS_SESSION_DEFAULT;
    }
    
    /**
     * @brief Change diagnostic session
     */
    UDSResult changeSession(uint8_t session_type) {
        uint8_t request[2] = {UDS_SID_DIAG_SESSION_CTRL, session_type};
        
        UDSResponse response;
        UDSResult result = sendRequest(request, 2, &response);
        
        if (result == UDS_OK && response.sid == (UDS_SID_DIAG_SESSION_CTRL + 0x40)) {
            current_session = session_type;
            Serial.printf("[UDS] Session changed to 0x%02X\n", session_type);
            return UDS_OK;
        }
        
        return result;
    }
    
    /**
     * @brief Perform ECU reset
     */
    UDSResult resetECU(uint8_t reset_type) {
        uint8_t request[2] = {UDS_SID_ECU_RESET, reset_type};
        
        UDSResponse response;
        return sendRequest(request, 2, &response);
    }
    
    /**
     * @brief Read data by identifier (DID)
     */
    UDSResult readDataByIdentifier(uint16_t did, uint8_t* buffer, size_t max_len, size_t* actual_len) {
        uint8_t request[3] = {
            UDS_SID_READ_DATA_BY_ID,
            (did >> 8) & 0xFF,  // High byte
            did & 0xFF          // Low byte
        };
        
        UDSResponse response;
        UDSResult result = sendRequest(request, 3, &response);
        
        if (result == UDS_OK && response.sid == (UDS_SID_READ_DATA_BY_ID + 0x40)) {
            // Copy data to buffer
            size_t copy_len = min(response.data_length, max_len);
            memcpy(buffer, response.data, copy_len);
            *actual_len = copy_len;
            return UDS_OK;
        }
        
        return result;
    }
    
    /**
     * @brief Write data by identifier (DID)
     */
    UDSResult writeDataByIdentifier(uint16_t did, const uint8_t* data, size_t len) {
        uint8_t request[256];
        request[0] = UDS_SID_WRITE_DATA_BY_ID;
        request[1] = (did >> 8) & 0xFF;
        request[2] = did & 0xFF;
        memcpy(&request[3], data, len);
        
        UDSResponse response;
        return sendRequest(request, 3 + len, &response);
    }
    
    /**
     * @brief Read DTC information
     */
    UDSResult readDTCInformation(uint8_t mask, uint8_t* dtc_buffer, size_t max_len, size_t* dtc_count) {
        uint8_t request[3] = {UDS_SID_READ_DTC_INFO, 0x02, mask};
        
        UDSResponse response;
        UDSResult result = sendRequest(request, 3, &response);
        
        if (result == UDS_OK && response.sid == (UDS_SID_READ_DTC_INFO + 0x40)) {
            // Parse DTC data
            *dtc_count = response.data[0];  // DTC count
            memcpy(dtc_buffer, &response.data[1], min(response.data_length - 1, max_len));
            return UDS_OK;
        }
        
        return result;
    }
    
    /**
     * @brief Clear DTC information
     */
    UDSResult clearDTCInformation(uint8_t group) {
        uint8_t request[2] = {UDS_SID_CLEAR_DTC_INFO, group};
        
        UDSResponse response;
        return sendRequest(request, 2, &response);
    }
    
    /**
     * @brief Security access - request seed
     */
    UDSResult requestSeed(uint8_t level, uint32_t* seed) {
        uint8_t request[2] = {UDS_SID_SECURITY_ACCESS, level};
        
        UDSResponse response;
        UDSResult result = sendRequest(request, 2, &response);
        
        if (result == UDS_OK && response.sid == (UDS_SID_SECURITY_ACCESS + 0x40)) {
            // Extract 4-byte seed
            *seed = (response.data[0] << 24) | 
                    (response.data[1] << 16) | 
                    (response.data[2] << 8) | 
                    response.data[3];
            return UDS_OK;
        }
        
        return result;
    }
    
    /**
     * @brief Security access - send key
     */
    UDSResult sendKey(uint8_t level, uint32_t key) {
        uint8_t request[5] = {
            UDS_SID_SECURITY_ACCESS, 
            level + 1,  // Send key = request seed level + 1
            (key >> 24) & 0xFF,
            (key >> 16) & 0xFF,
            (key >> 8) & 0xFF,
            key & 0xFF
        };
        
        UDSResponse response;
        return sendRequest(request, 5, &response);
    }
    
private:
    /**
     * @brief Send UDS request and wait for response
     */
    UDSResult sendRequest(const uint8_t* request, size_t len, UDSResponse* response) {
        // Send via CAN TP
        can_tp->send(request, len);
        
        // Wait for response with timeout
        uint32_t start_time = millis();
        while (millis() - start_time < p2_timer) {
            if (can_tp->available()) {
                uint8_t rx_buffer[256];
                size_t rx_len = can_tp->receive(rx_buffer, sizeof(rx_buffer));
                
                // Parse response
                if (rx_len >= 2) {
                    response->sid = rx_buffer[0];
                    
                    if (rx_buffer[0] == 0x7F) {
                        // Negative response
                        response->is_negative = true;
                        response->nrc = rx_buffer[2];
                        Serial.printf("[UDS] Negative response: 0x%02X\n", rx_buffer[2]);
                        return UDS_ERR_NEGATIVE_RESPONSE;
                    } else {
                        // Positive response
                        response->is_negative = false;
                        response->nrc = 0x00;
                        response->data = &rx_buffer[1];
                        response->data_length = rx_len - 1;
                        return UDS_OK;
                    }
                }
            }
            delay(1);
        }
        
        Serial.println("[UDS] Timeout waiting for response");
        return UDS_ERR_TIMEOUT;
    }
};
```

### 6.3 UDS Server (ECU Simulator)

```cpp
// uds_server.cpp
#include "uds_server.h"
#include <string.h>

class UDSServer {
private:
    uint8_t current_session;
    bool security_unlocked;
    uint32_t security_seed;
    uint32_t s3_timer_start;
    uint32_t s3_timeout;
    
    // Simulated ECU data
    char vin[18] = "WB1234567890ABCDE";
    uint8_t software_version[4] = {0x01, 0x02, 0x03, 0x04};
    uint16_t engine_rpm = 0;
    uint8_t coolant_temp = 0;
    
    // DTC storage (simulated)
    struct DTC {
        uint32_t code;
        uint8_t status;
    } dtc_storage[10];
    uint8_t dtc_count = 0;
    
public:
    UDSServer() {
        current_session = UDS_SESSION_DEFAULT;
        security_unlocked = false;
        s3_timeout = 5000;
    }
    
    /**
     * @brief Process incoming UDS request
     */
    void processRequest(const uint8_t* request, size_t len, uint8_t* response, size_t* resp_len) {
        if (len < 1) return;
        
        uint8_t sid = request[0];
        
        switch (sid) {
            case UDS_SID_DIAG_SESSION_CTRL:
                handleSessionControl(request, len, response, resp_len);
                break;
                
            case UDS_SID_ECU_RESET:
                handleECUReset(request, len, response, resp_len);
                break;
                
            case UDS_SID_SECURITY_ACCESS:
                handleSecurityAccess(request, len, response, resp_len);
                break;
                
            case UDS_SID_READ_DATA_BY_ID:
                handleReadDataById(request, len, response, resp_len);
                break;
                
            case UDS_SID_WRITE_DATA_BY_ID:
                handleWriteDataById(request, len, response, resp_len);
                break;
                
            case UDS_SID_READ_DTC_INFO:
                handleReadDTCInfo(request, len, response, resp_len);
                break;
                
            case UDS_SID_CLEAR_DTC_INFO:
                handleClearDTCInfo(request, len, response, resp_len);
                break;
                
            default:
                sendNegativeResponse(response, resp_len, sid, UDS_NRC_SERVICE_NOT_SUPPORTED);
                break;
        }
        
        // Reset S3 timer on valid request
        s3_timer_start = millis();
    }
    
private:
    void handleSessionControl(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        if (len < 2) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
            return;
        }
        
        uint8_t session_type = req[1];
        
        // Check if transition is allowed
        if (session_type == UDS_SESSION_DEFAULT ||
            session_type == UDS_SESSION_PROGRAMMING ||
            session_type == UDS_SESSION_EXTENDED) {
            
            current_session = session_type;
            
            // Build positive response
            resp[0] = 0x50;  // 0x10 + 0x40
            resp[1] = session_type;
            resp[2] = 0x00;  // Reserved
            resp[3] = 0x32;  // P2 max = 50ms
            resp[4] = 0x01;  // P2 extended
            resp[5] = 0xF4;
            *resp_len = 6;
        } else {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
        }
    }
    
    void handleECUReset(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        if (len < 2) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
            return;
        }
        
        uint8_t reset_type = req[1];
        
        // Simulate reset action
        Serial.printf("[UDS Server] ECU Reset requested: 0x%02X\n", reset_type);
        
        if (reset_type == UDS_RESET_HARD || 
            reset_type == UDS_RESET_SOFT ||
            reset_type == UDS_RESET_KEY_OFF_ON) {
            
            resp[0] = 0x51;  // 0x11 + 0x40
            resp[1] = reset_type;
            *resp_len = 2;
            
            // In real implementation, trigger reset here
        } else {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
        }
    }
    
    void handleSecurityAccess(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        if (len < 2) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
            return;
        }
        
        uint8_t sub_function = req[1];
        
        if (sub_function % 2 == 1) {
            // Request seed (odd number)
            security_seed = random(0xFFFFFFFF);
            
            resp[0] = 0x67;  // 0x27 + 0x40
            resp[1] = sub_function;
            resp[2] = (security_seed >> 24) & 0xFF;
            resp[3] = (security_seed >> 16) & 0xFF;
            resp[4] = (security_seed >> 8) & 0xFF;
            resp[5] = security_seed & 0xFF;
            *resp_len = 6;
            
            Serial.printf("[UDS Server] Seed sent: 0x%08lX\n", security_seed);
            
        } else if (sub_function % 2 == 0) {
            // Send key (even number)
            if (len < 6) {
                sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_WRONG_LENGTH);
                return;
            }
            
            uint32_t received_key = (req[2] << 24) | (req[3] << 16) | (req[4] << 8) | req[5];
            
            // Simple algorithm: key = seed XOR 0xDEADBEEF (for demo only!)
            uint32_t expected_key = security_seed ^ 0xDEADBEEF;
            
            if (received_key == expected_key) {
                security_unlocked = true;
                resp[0] = 0x67;
                resp[1] = sub_function;
                *resp_len = 2;
                Serial.println("[UDS Server] Security unlocked!");
            } else {
                sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_INVALID_KEY);
                Serial.println("[UDS Server] Invalid key!");
            }
        } else {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
        }
    }
    
    void handleReadDataById(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        if (len < 3) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_WRONG_LENGTH);
            return;
        }
        
        uint16_t did = (req[1] << 8) | req[2];
        
        resp[0] = 0x62;  // 0x22 + 0x40
        resp[1] = req[1];  // DID high byte
        resp[2] = req[2];  // DID low byte
        
        switch (did) {
            case 0xF190:  // VIN
                memcpy(&resp[3], vin, 17);
                *resp_len = 20;
                break;
                
            case 0xF170:  // Software version
                memcpy(&resp[3], software_version, 4);
                *resp_len = 7;
                break;
                
            case 0x2200:  // Engine RPM (simulated)
                engine_rpm = random(800, 6000);
                resp[3] = (engine_rpm >> 8) & 0xFF;
                resp[4] = engine_rpm & 0xFF;
                *resp_len = 5;
                break;
                
            case 0x2201:  // Coolant temperature
                coolant_temp = random(60, 110);
                resp[3] = coolant_temp;
                *resp_len = 4;
                break;
                
            default:
                sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_REQUEST_OUT_OF_RANGE);
                return;
        }
    }
    
    void handleWriteDataById(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        if (len < 4) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_WRONG_LENGTH);
            return;
        }
        
        // Check security (for protected DIDs)
        if (!security_unlocked) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SECURITY_ACCESS_DENIED);
            return;
        }
        
        uint16_t did = (req[1] << 8) | req[2];
        
        // Simulate writing data
        Serial.printf("[UDS Server] Writing to DID 0x%04X\n", did);
        
        resp[0] = 0x6E;  // 0x2E + 0x40
        resp[1] = req[1];
        resp[2] = req[2];
        *resp_len = 3;
    }
    
    void handleReadDTCInfo(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        if (len < 3) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_WRONG_LENGTH);
            return;
        }
        
        uint8_t sub_function = req[1];
        
        if (sub_function == 0x02) {  // Read DTC by status mask
            resp[0] = 0x59;  // 0x19 + 0x40
            resp[1] = 0x02;
            resp[2] = dtc_count;  // Number of DTCs
            
            // Add DTC entries (3 bytes each: DTC + status)
            size_t idx = 3;
            for (uint8_t i = 0; i < dtc_count && idx < 250; i++) {
                resp[idx++] = (dtc_storage[i].code >> 16) & 0xFF;
                resp[idx++] = (dtc_storage[i].code >> 8) & 0xFF;
                resp[idx++] = dtc_storage[i].code & 0xFF;
                resp[idx++] = dtc_storage[i].status;
            }
            
            *resp_len = idx;
        } else {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
        }
    }
    
    void handleClearDTCInfo(const uint8_t* req, size_t len, uint8_t* resp, size_t* resp_len) {
        // Check security
        if (!security_unlocked) {
            sendNegativeResponse(resp, resp_len, req[0], UDS_NRC_SECURITY_ACCESS_DENIED);
            return;
        }
        
        // Clear all DTCs
        dtc_count = 0;
        
        resp[0] = 0x54;  // 0x14 + 0x40
        *resp_len = 1;
        
        Serial.println("[UDS Server] DTCs cleared");
    }
    
    void sendNegativeResponse(uint8_t* resp, size_t* resp_len, uint8_t req_sid, uint8_t nrc) {
        resp[0] = 0x7F;
        resp[1] = req_sid;
        resp[2] = nrc;
        *resp_len = 3;
        
        Serial.printf("[UDS Server] Negative response: 0x%02X\n", nrc);
    }
};
```

---

## 7. Testing & Simulation

### 7.1 UDS Tester Simulator (Python)

```python
#!/usr/bin/env python3
"""
UDS Tester Simulator
Simple Python script untuk testing UDS services
"""

import can
import time
from typing import Optional, Tuple

class UDSTester:
    def __init__(self, channel='can0', bitrate=500000):
        self.bus = can.interface.Bus(channel=channel, bitrate=bitrate)
        self.ecu_id = 0x7E0  # Default ECU address
        self.tester_id = 0x7DF  # Default tester address
        
    def send_uds_request(self, data: list, timeout: float = 0.1) -> Optional[list]:
        """Send UDS request and wait for response"""
        msg = can.Message(
            arbitration_id=self.ecu_id,
            data=data,
            is_extended_id=False
        )
        
        print(f"[TX] {' '.join(f'{b:02X}' for b in data)}")
        self.bus.send(msg)
        
        # Wait for response
        start_time = time.time()
        while time.time() - start_time < timeout:
            msg = self.bus.recv(timeout=0.05)
            if msg and msg.arbitration_id == self.tester_id:
                response = list(msg.data)
                print(f"[RX] {' '.join(f'{b:02X}' for b in response)}")
                return response
        
        print("[TIMEOUT] No response received")
        return None
    
    def test_session_control(self):
        """Test 0x10 - Diagnostic Session Control"""
        print("\n=== Testing Session Control ===")
        
        # Default session
        response = self.send_uds_request([0x10, 0x01])
        if response and response[0] == 0x50:
            print("✓ Default session OK")
        
        time.sleep(0.1)
        
        # Extended session
        response = self.send_uds_request([0x10, 0x03])
        if response and response[0] == 0x50:
            print("✓ Extended session OK")
    
    def test_read_did(self, did: int):
        """Test 0x22 - Read Data By Identifier"""
        print(f"\n=== Testing Read DID 0x{did:04X} ===")
        
        request = [0x22, (did >> 8) & 0xFF, did & 0xFF]
        response = self.send_uds_request(request)
        
        if response and response[0] == 0x62:
            data = response[3:]
            print(f"✓ Data: {' '.join(f'{b:02X}' for b in data)}")
            return data
        return None
    
    def test_ecu_reset(self, reset_type: int = 0x01):
        """Test 0x11 - ECU Reset"""
        print(f"\n=== Testing ECU Reset (0x{reset_type:02X}) ===")
        
        request = [0x11, reset_type]
        response = self.send_uds_request(request)
        
        if response and response[0] == 0x51:
            print("✓ Reset command accepted")
    
    def test_security_access(self):
        """Test 0x27 - Security Access"""
        print("\n=== Testing Security Access ===")
        
        # Step 1: Request seed
        request = [0x27, 0x01]
        response = self.send_uds_request(request)
        
        if response and response[0] == 0x67:
            seed = (response[2] << 24) | (response[3] << 16) | \
                   (response[4] << 8) | response[5]
            print(f"Seed received: 0x{seed:08X}")
            
            # Calculate key (simple XOR for demo)
            key = seed ^ 0xDEADBEEF
            
            # Step 2: Send key
            request = [0x27, 0x02, 
                      (key >> 24) & 0xFF, (key >> 16) & 0xFF,
                      (key >> 8) & 0xFF, key & 0xFF]
            response = self.send_uds_request(request)
            
            if response and response[0] == 0x67:
                print("✓ Security unlocked!")
                return True
        
        return False
    
    def test_read_dtc(self):
        """Test 0x19 - Read DTC Information"""
        print("\n=== Testing Read DTC ===")
        
        request = [0x19, 0x02, 0xFF]  # Read all DTCs
        response = self.send_uds_request(request)
        
        if response and response[0] == 0x59:
            dtc_count = response[2]
            print(f"✓ Found {dtc_count} DTC(s)")
            
            # Parse DTCs
            idx = 3
            for i in range(dtc_count):
                if idx + 3 < len(response):
                    dtc = (response[idx] << 16) | (response[idx+1] << 8) | response[idx+2]
                    status = response[idx+3]
                    print(f"  DTC {i+1}: 0x{dtc:06X}, Status: 0x{status:02X}")
                    idx += 4
    
    def run_all_tests(self):
        """Run complete UDS test suite"""
        print("=" * 50)
        print("UDS Test Suite Starting...")
        print("=" * 50)
        
        self.test_session_control()
        time.sleep(0.5)
        
        self.test_read_did(0xF190)  # VIN
        time.sleep(0.5)
        
        self.test_read_did(0xF170)  # Software version
        time.sleep(0.5)
        
        self.test_security_access()
        time.sleep(0.5)
        
        self.test_read_dtc()
        
        print("\n" + "=" * 50)
        print("Test Suite Complete")
        print("=" * 50)

if __name__ == '__main__':
    tester = UDSTester()
    tester.run_all_tests()
```

### 7.2 Test Checklist

| Test Case | Expected Result | Status |
|-----------|----------------|--------|
| Session Control (Default) | Response 0x50 0x01 | ⬜ |
| Session Control (Extended) | Response 0x50 0x03 | ⬜ |
| Read VIN (0xF190) | Response 0x62 + 17 bytes | ⬜ |
| Read Software Version | Response 0x62 + version data | ⬜ |
| Security Access (Seed) | Response 0x67 + 4-byte seed | ⬜ |
| Security Access (Key) | Response 0x67 (unlock) | ⬜ |
| Read DTC | Response 0x59 + DTC list | ⬜ |
| Clear DTC | Response 0x54 | ⬜ |
| ECU Reset | Response 0x51 | ⬜ |

---

## 8. Troubleshooting

### 8.1 Common Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No response from ECU | Wrong CAN ID, baud rate mismatch | Verify ECU address and bus speed |
| Negative response 0x7F 0x11 | Service not supported | Check if ECU implements this SID |
| Negative response 0x7F 0x33 | Security access denied | Perform security access first |
| Negative response 0x7F 0x35 | Invalid key | Verify seed-to-key algorithm |
| Negative response 0x7F 0x78 | Response pending | Wait longer, send flow control |
| Timeout | ECU busy, wrong session | Check session state, retry |

### 8.2 Debugging Tips

1. **Use Logic Analyzer / CAN Logger**
   - Capture raw CAN frames
   - Verify timing (P2, S3 timers)
   - Check for multi-frame sequences

2. **Check Session State**
   - Some services only available in specific sessions
   - Extended session required for most diagnostics

3. **Verify Security Access**
   - Many OEM-specific functions require unlock
   - Seed-to-key algorithm varies by manufacturer

4. **Monitor Error Counters**
   - Check EFLG register on MCP2515
   - Watch for bus-off errors

---

## 9. Referensi & Resources

### 9.1 Standards

| Standard | Title | Source |
|----------|-------|--------|
| ISO 14229-1 | UDS on CAN | ISO Store |
| ISO 15765-2 | CAN Transport Protocol | ISO Store |
| ISO 11898-1/2 | CAN Physical/Data Link | ISO Store |
| SAE J1979 | OBD-II Services | SAE International |

### 9.2 Tools

| Tool | Type | Cost |
|------|------|------|
| Vector CANoe | Professional | $$$$ |
| PCAN-View | Basic Viewer | Free |
| SavvyCAN | Open Source | Free |
| CANalyzer | Professional | $$$$ |
| SocketCAN (Linux) | Framework | Free |

### 9.3 Learning Resources

- [Vector Academy - UDS Training](https://vector.com/academy)
- [Intrepid Control Systems - UDS Guide](https://intrepidcs.com)
- [AUTOSAR Diagnostic Stack](https://www.autosar.org)

### 9.4 Japanese Terminology

| English | 日本語 | Romaji |
|---------|--------|--------|
| Diagnostic Service | 診断サービス | shindan sābisu |
| Trouble Code | 故障コード | koshō kōdo |
| Session Control | セッション制御 | sesshon seigyo |
| Security Access | セキュリティアクセス | sekyuriti akusesu |
| ECU Reset | ECU リセット | ECU risetto |
| Data Identifier | データ識別子 | dēta shikibetsushi |

---

## 📝 Next Steps

Setelah menguasai UDS, lanjutkan ke:

1. **[CAN TP Implementation](./01_CAN_TP.md)** - Transport protocol untuk multi-frame messages
2. **[Bootloader Development](./03_Bootloader.md)** - Firmware update menggunakan UDS
3. **[AUTOSAR Diagnostics](./02_AUTOSAR_Classic.md)** - Diagnostic stack dalam AUTOSAR

---

**Document Version:** 1.0  
**Last Updated:** 2025  
**Author:** Embedded Systems Team  
**License:** MIT
