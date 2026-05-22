# UDS Protocol Master - ISO 14229

## 📋 Informasi Modul

| Item | Deskripsi |
|------|-----------|
| **Standar** | ISO 14229-1:2020 |
| **Level** | Advanced |
| **Prasyarat** | CAN TP Deep Dive, C Basics for Embedded |
| **Durasi Belajar** | 8-12 jam |
| **Implementasi** | C/C++ Production Code |

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, Anda akan mampu:
1. Memahami arsitektur dan filosofi UDS (Unified Diagnostic Services)
2. Mengimplementasikan semua layanan UDS utama dalam C
3. Membangun stack diagnostik yang compliant dengan ISO 14229
4. Melakukan troubleshooting dan testing sistem diagnostik
5. Mengintegrasikan UDS dengan CAN TP untuk komunikasi multi-frame

---

## 📚 Daftar Isi

1. [Pengenalan UDS](#1-pengenalan-uds)
2. [Arsitektur Protokol UDS](#2-arsitektur-protokol-uds)
3. [Service Identifiers (SID) Lengkap](#3-service-identifiers-sid-lengkap)
4. [Implementasi Core Services](#4-implementasi-core-services)
5. [Security Access Mechanism](#5-security-access-mechanism)
6. [Data Identifier Management](#6-data-identifier-management)
7. [Routine Control Implementation](#7-routine-control-implementation)
8. [DTC Management System](#8-dtc-management-system)
9. [Session Management](#9-session-management)
10. [Testing dan Validation](#10-testing-dan-validation)
11. [Troubleshooting Guide](#11-troubleshooting-guide)
12. [Glosarium Jepang-Inggris](#12-glosarium-jepang-inggris)

---

## 1. Pengenalan UDS

### 1.1 Apa itu UDS?

**UDS (Unified Diagnostic Services)** adalah protokol diagnostik standar internasional yang didefinisikan dalam **ISO 14229**. Protokol ini menyediakan layanan standar untuk komunikasi antara alat diagnosa (tester) dan ECU (Electronic Control Unit) dalam kendaraan.

### 1.2 Mengapa UDS Penting?

| Aspek | Penjelasan |
|-------|------------|
| **Standardisasi** | Semua OEM menggunakan UDS sebagai basis diagnostik |
| **Interoperability** | Tool dari vendor berbeda dapat berkomunikasi dengan ECU apapun |
| **Scalability** | Dapat digunakan dari motor hingga truk berat |
| **Future-proof** | Mendukung update firmware, coding, dan calibrasi |

### 1.3 Aplikasi UDS dalam Industri Otomotif

```
┌─────────────────────────────────────────────────────────────┐
│                    UDS Application Areas                     │
├─────────────────────────────────────────────────────────────┤
│  🔧 Workshop Diagnostics    → Read DTC, Clear DTC, I/O Ctrl │
│  🏭 Production Line         → ECU Programming, Coding       │
│  🚗 Field Updates           → Flash Update, Calibration     │
│  🔬 Development & Testing   → Data Logging, Parameter Read │
│  🛡️ Security                → Secure Boot, Authenticated FW │
└─────────────────────────────────────────────────────────────┘
```

### 1.4 Relationship dengan Protokol Lain

```
┌──────────────────────────────────────────────────────────────┐
│              Automotive Diagnostic Stack                      │
├──────────────────────────────────────────────────────────────┤
│  Application Layer:    UDS (ISO 14229)                       │
│                        OBD-II (SAE J1979)                    │
├──────────────────────────────────────────────────────────────┤
│  Transport Layer:      CAN TP (ISO 15765-2)                  │
│                        DoIP (ISO 13400)                      │
│                        LIN TP (ISO 17987)                    │
├──────────────────────────────────────────────────────────────┤
│  Network Layer:        CAN (ISO 11898)                       │
│                        Ethernet (IEEE 802.3)                 │
│                        LIN (ISO 17987)                       │
└──────────────────────────────────────────────────────────────┘
```

---

## 2. Arsitektur Protokol UDS

### 2.1 Struktur Pesan UDS

Setiap pesan UDS terdiri dari tiga komponen utama:

```
┌─────────┬──────────────┬─────────────────────────────────┐
│   SID   │ Sub-function │          Data Payload           │
│ (1 byte)│  (1 byte)    │        (0-254 bytes)            │
└─────────┴──────────────┴─────────────────────────────────┘
```

**Komponen:**
- **SID (Service Identifier)**: Menentukan jenis layanan yang diminta
- **Sub-function**: Parameter tambahan untuk memodifikasi perilaku SID
- **Data Payload**: Data spesifik yang diperlukan untuk layanan tersebut

### 2.2 Format Request dan Response

#### Request Message Structure
```
[SID] [Sub-function] [Data...]
```

#### Positive Response Structure
```
[SID + 0x40] [Sub-function] [Data...]
```

#### Negative Response Structure
```
[0x7F] [SID] [Response Code]
```

### 2.3 Response Codes (NRC - Negative Response Code)

| NRC | Hex | Deskripsi |
|-----|-----|-----------|
| generalReject | 0x10 | Permintaan ditolak tanpa alasan spesifik |
| serviceNotSupported | 0x11 | SID tidak didukung |
| subFunctionNotSupported | 0x12 | Sub-function tidak didukung |
| incorrectMessageLength | 0x13 | Panjang pesan tidak valid |
| conditionsNotCorrect | 0x22 | Kondisi saat ini tidak memungkinkan |
| requestOutOfRange | 0x31 | Parameter di luar range yang valid |
| securityAccessDenied | 0x33 | Akses keamanan ditolak |
| invalidKey | 0x35 | Key keamanan tidak valid |
| exceededNumberOfAttempts | 0x36 | Terlalu banyak percobaan |
| requiredTimeDelayNotExpired | 0x37 | Waktu tunda belum habis |

### 2.4 State Machine UDS

```c
typedef enum {
    UDS_STATE_IDLE = 0,           // Menunggu request
    UDS_STATE_RECEIVING,          // Menerima data (multi-frame)
    UDS_STATE_PROCESSING,         // Memproses request
    UDS_STATE_SENDING_RESPONSE,   // Mengirim response
    UDS_STATE_WAIT_DELAY,         // Menunggu time delay
    UDS_STATE_SECURITY_LOCKED     // Locked setelah failed attempts
} UdsStateType;

typedef struct {
    UdsStateType state;
    uint32_t session_timeout;
    uint8_t current_session;
    uint8_t security_level;
    uint8_t failed_attempts;
    uint32_t lock_timer;
} UdsContextType;

static UdsContextType uds_context = {
    .state = UDS_STATE_IDLE,
    .session_timeout = 5000,  // 5 detik default
    .current_session = 0x01,  // Default session
    .security_level = 0,
    .failed_attempts = 0,
    .lock_timer = 0
};
```

---

## 3. Service Identifiers (SID) Lengkap

### 3.1 Diagnostic and Communication Management (0x10-0x3E)

| SID | Service Name | Deskripsi |
|-----|--------------|-----------|
| 0x10 | DiagnosticSessionControl | Mengubah sesi diagnostik |
| 0x11 | ECUReset | Reset ECU (hard/soft/power cycle) |
| 0x14 | ClearDiagnosticInformation | Menghapus DTC |
| 0x19 | ReadDTCInformation | Membaca informasi DTC |
| 0x22 | ReadDataByIdentifier | Membaca data berdasarkan DID |
| 0x23 | ReadMemoryByAddress | Membaca memori langsung |
| 0x24 | ReadScalingDataByIdentifier | Membaca scaling data |
| 0x27 | SecurityAccess | Autentikasi keamanan |
| 0x28 | CommunicationControl | Enable/disable komunikasi |
| 0x2A | Authentication | Autentikasi tingkat lanjut |
| 0x2C | DynamicallyDefineDataIdentifier | Define DID dinamis |
| 0x2E | WriteDataByIdentifier | Menulis data berdasarkan DID |
| 0x2F | InputOutputControlByIdentifier | Kontrol I/O langsung |
| 0x31 | RoutineControl | Eksekusi routine |
| 0x34 | RequestDownload | Persiapan download firmware |
| 0x35 | RequestUpload | Persiapan upload data |
| 0x36 | TransferData | Transfer data block |
| 0x37 | RequestTransferExit | Selesai transfer data |
| 0x38 | RequestFileTransfer | Transfer file |
| 0x3E | TesterPresent | Menjaga sesi tetap aktif |

### 3.2 System Management (0x85-0xB5)

| SID | Service Name | Deskripsi |
|-----|--------------|-----------|
| 0x85 | ControlDTCSetting | Enable/disable DTC storage |
| 0x86 | ResponseOnEvent | Konfigurasi response on event |
| 0x87 | LinkControl | Kontrol link komunikasi |

---

## 4. Implementasi Core Services

### 4.1 Diagnostic Session Control (0x10)

#### Fungsi
Mengubah mode operasi ECU untuk mengaktifkan fungsi diagnostik tertentu.

#### Session Types
```c
#define SESSION_DEFAULT         0x01
#define SESSION_PROGRAMMING     0x02
#define SESSION_EXTENDED        0x03
#define SESSION_SAFETY          0x04
#define SESSION_SUPPLIER        0x80  // Vendor-specific

// Session parameter records (contoh timing)
typedef struct {
    uint16_t p2_server_max;      // Max response time (ms)
    uint16_t p2_star_server_max; // Enhanced response time (ms)
    uint16_t s3_server;          // Session timeout (ms)
} SessionTimingType;

static const SessionTimingType session_timings[] = {
    {50, 5000, 5000},   // Default session
    {50, 5000, 30000},  // Programming session
    {50, 5000, 5000},   // Extended session
    {50, 5000, 2000}    // Safety session
};
```

#### Implementasi
```c
Std_ReturnType Uds_DiagnosticSessionControl(const uint8_t* request, 
                                            uint8_t* response,
                                            uint16_t* response_length) {
    uint8_t sub_function = request[1];
    Std_ReturnType result = E_NOT_OK;
    
    // Validasi panjang pesan
    if (request[0] != 2) {
        response[0] = 0x7F;
        response[1] = 0x10;
        response[2] = 0x13;  // incorrectMessageLength
        *response_length = 3;
        return E_OK;
    }
    
    // Cek apakah sub-function didukung
    if (sub_function > 0x04 && sub_function < 0x80) {
        response[0] = 0x7F;
        response[1] = 0x10;
        response[2] = 0x12;  // subFunctionNotSupported
        *response_length = 3;
        return E_OK;
    }
    
    // Cek kondisi transisi session
    if (!Uds_CanTransitionToSession(uds_context.current_session, sub_function)) {
        response[0] = 0x7F;
        response[1] = 0x10;
        response[2] = 0x22;  // conditionsNotCorrect
        *response_length = 3;
        return E_OK;
    }
    
    // Transisi session berhasil
    uds_context.current_session = sub_function;
    uds_context.session_timeout = session_timings[sub_function - 1].s3_server;
    
    // Build positive response
    response[0] = 0x50;  // 0x10 + 0x40
    response[1] = sub_function;
    
    // Copy session parameter record (optional)
    uint8_t param_offset = 2;
    response[param_offset++] = (session_timings[sub_function - 1].p2_server_max >> 8) & 0xFF;
    response[param_offset++] = session_timings[sub_function - 1].p2_server_max & 0xFF;
    response[param_offset++] = (session_timings[sub_function - 1].p2_star_server_max >> 8) & 0xFF;
    response[param_offset++] = session_timings[sub_function - 1].p2_star_server_max & 0xFF;
    
    *response_length = param_offset;
    
    // Log session change
    Uds_LogEvent(UDS_EVENT_SESSION_CHANGED, sub_function);
    
    return E_OK;
}

static bool Uds_CanTransitionToSession(uint8_t from_session, uint8_t to_session) {
    // Dari default session bisa ke semua session
    if (from_session == SESSION_DEFAULT) {
        return true;
    }
    
    // Dari programming session hanya bisa ke default atau supplier
    if (from_session == SESSION_PROGRAMMING) {
        return (to_session == SESSION_DEFAULT || to_session >= 0x80);
    }
    
    // Dari extended/safety bisa ke default atau programming
    if (from_session == SESSION_EXTENDED || from_session == SESSION_SAFETY) {
        return (to_session == SESSION_DEFAULT || to_session == SESSION_PROGRAMMING);
    }
    
    return false;
}
```

### 4.2 ECU Reset (0x11)

#### Reset Types
```c
#define RESET_HARD_RESET          0x01
#define RESET_KEY_OFF_ON_RESET    0x02
#define RESET_SOFT_RESET          0x03
#define RESET_ENABLE_RAPID_POWER  0x04
#define RESET_DISABLE_RAPID_POWER 0x05
```

#### Implementasi
```c
Std_ReturnType Uds_EcuReset(const uint8_t* request,
                            uint8_t* response,
                            uint16_t* response_length) {
    uint8_t reset_type = request[1];
    
    // Validasi reset type
    if (reset_type > RESET_DISABLE_RAPID_POWER) {
        response[0] = 0x7F;
        response[1] = 0x11;
        response[2] = 0x12;  // subFunctionNotSupported
        *response_length = 3;
        return E_OK;
    }
    
    // Cek kondisi untuk reset
    if (!Uds_IsResetAllowed(reset_type)) {
        response[0] = 0x7F;
        response[1] = 0x11;
        response[2] = 0x22;  // conditionsNotCorrect
        *response_length = 3;
        return E_OK;
    }
    
    // Simpan reset type untuk dieksekusi setelah response
    Uds_PendingReset(reset_type);
    
    // Build positive response
    response[0] = 0x51;  // 0x11 + 0x40
    response[1] = reset_type;
    response[2] = 0x00;  // Power down time (optional)
    *response_length = 3;
    
    return E_OK;
}

static bool Uds_IsResetAllowed(uint8_t reset_type) {
    // Tidak boleh reset jika sedang flashing
    if (Uds_IsFlashingInProgress()) {
        return false;
    }
    
    // Soft reset selalu allowed
    if (reset_type == RESET_SOFT_RESET) {
        return true;
    }
    
    // Hard reset memerlukan security access level tertentu
    if (reset_type == RESET_HARD_RESET) {
        return (uds_context.security_level >= SECURITY_LEVEL_2);
    }
    
    return true;
}

static void Uds_PendingReset(uint8_t reset_type) {
    // Set flag untuk reset setelah mengirim response
    volatile uint8_t* reset_flag_reg = (volatile uint8_t*)RESET_FLAG_ADDRESS;
    *reset_flag_reg = reset_type;
}
```

### 4.3 Read Data By Identifier (0x22)

#### Data Identifier Categories
```c
// Vehicle Manufacturer Specific (0xF400-0xF7FF)
#define DID_VIN                     0xF190
#define DID_SYSTEM_SUPPLIER_ID      0xF187
#define DID_CALIBRATION_ID          0xF186
#define DID_SOFTWARE_VERSION        0xF185
#define DID_PART_NUMBER             0xF184

// System Specific (0xF000-0xF1FF)
#define DID_ENGINE_SPEED            0xF200
#define DID_VEHICLE_SPEED           0xF201
#define DID_COOLANT_TEMP            0xF202
#define DID_FUEL_LEVEL              0xF203

// Component Specific (0xF100-0xF1FF)
#define DID_SENSOR_DATA             0xF150
#define DID_ACTUATOR_STATUS         0xF151
```

#### Implementasi
```c
Std_ReturnType Uds_ReadDataByIdentifier(const uint8_t* request,
                                        uint8_t* response,
                                        uint16_t* response_length) {
    uint16_t did = ((uint16_t)request[1] << 8) | request[2];
    uint16_t data_offset = 3;  // Offset untuk data response
    
    // Validasi panjang request
    if (request[0] != 3) {
        response[0] = 0x7F;
        response[1] = 0x22;
        response[2] = 0x13;  // incorrectMessageLength
        *response_length = 3;
        return E_OK;
    }
    
    // Set header response
    response[0] = 0x62;  // 0x22 + 0x40
    response[1] = request[1];  // High byte DID
    response[2] = request[2];  // Low byte DID
    
    // Dispatch ke handler berdasarkan DID
    switch (did) {
        case DID_VIN:
            data_offset += Uds_ReadVin(&response[data_offset]);
            break;
            
        case DID_SYSTEM_SUPPLIER_ID:
            data_offset += Uds_ReadSystemSupplierId(&response[data_offset]);
            break;
            
        case DID_CALIBRATION_ID:
            data_offset += Uds_ReadCalibrationId(&response[data_offset]);
            break;
            
        case DID_SOFTWARE_VERSION:
            data_offset += Uds_ReadSoftwareVersion(&response[data_offset]);
            break;
            
        case DID_ENGINE_SPEED:
            data_offset += Uds_ReadEngineSpeed(&response[data_offset]);
            break;
            
        case DID_VEHICLE_SPEED:
            data_offset += Uds_ReadVehicleSpeed(&response[data_offset]);
            break;
            
        default:
            // DID tidak didukung
            response[0] = 0x7F;
            response[1] = 0x22;
            response[2] = 0x31;  // requestOutOfRange
            *response_length = 3;
            return E_OK;
    }
    
    *response_length = data_offset;
    return E_OK;
}

// Helper functions untuk membaca berbagai DID
static uint16_t Uds_ReadVin(uint8_t* buffer) {
    // VIN adalah 17 karakter ASCII
    const char* vin = "WVWZZZ1JZ3W386752";  // Contoh VIN
    memcpy(buffer, vin, 17);
    return 17;
}

static uint16_t Uds_ReadSystemSupplierId(uint8_t* buffer) {
    const char* supplier_id = "BOSCH";
    uint16_t len = strlen(supplier_id);
    memcpy(buffer, supplier_id, len);
    return len;
}

static uint16_t Uds_ReadCalibrationId(uint8_t* buffer) {
    // Calibration ID biasanya 16 byte
    const uint8_t cal_id[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    memcpy(buffer, cal_id, 16);
    return 16;
}

static uint16_t Uds_ReadSoftwareVersion(uint8_t* buffer) {
    const char* sw_version = "V1.2.3";
    uint16_t len = strlen(sw_version);
    memcpy(buffer, sw_version, len);
    return len;
}

static uint16_t Uds_ReadEngineSpeed(uint8_t* buffer) {
    // Engine speed dalam RPM (2 byte, resolution 0.25 RPM/bit)
    uint16_t rpm = GetEngineRpm();  // Fungsi untuk membaca RPM aktual
    uint16_t encoded_rpm = rpm * 4;  // Encode sesuai spesifikasi
    buffer[0] = (encoded_rpm >> 8) & 0xFF;
    buffer[1] = encoded_rpm & 0xFF;
    return 2;
}

static uint16_t Uds_ReadVehicleSpeed(uint8_t* buffer) {
    // Vehicle speed dalam km/h (1 byte, resolution 1 km/h)
    uint8_t speed = GetVehicleSpeed();  // Fungsi untuk membaca speed aktual
    buffer[0] = speed;
    return 1;
}
```

### 4.4 Write Data By Identifier (0x2E)

#### Implementasi
```c
Std_ReturnType Uds_WriteDataByIdentifier(const uint8_t* request,
                                         uint8_t* response,
                                         uint16_t* response_length) {
    uint16_t did = ((uint16_t)request[1] << 8) | request[2];
    const uint8_t* data = &request[3];
    uint16_t data_length = request[0] - 3;
    
    // Validasi panjang minimum request
    if (request[0] < 4) {
        response[0] = 0x7F;
        response[1] = 0x2E;
        response[2] = 0x13;  // incorrectMessageLength
        *response_length = 3;
        return E_OK;
    }
    
    // Cek security access untuk DID yang sensitif
    if (Uds_RequiresSecurityAccess(did) && 
        uds_context.security_level < Uds_GetRequiredSecurityLevel(did)) {
        response[0] = 0x7F;
        response[1] = 0x2E;
        response[2] = 0x33;  // securityAccessDenied
        *response_length = 3;
        return E_OK;
    }
    
    // Dispatch ke handler berdasarkan DID
    Std_ReturnType write_result = E_NOT_OK;
    switch (did) {
        case 0xF186:  // Calibration Identification
            write_result = Uds_WriteCalibrationId(data, data_length);
            break;
            
        case 0xF185:  // Software Version
            write_result = Uds_WriteSoftwareVersion(data, data_length);
            break;
            
        case 0xF187:  // System Supplier Identification
            write_result = Uds_WriteSystemSupplierId(data, data_length);
            break;
            
        case 0xF18C:  // Configuration Data
            write_result = Uds_WriteConfigurationData(data, data_length);
            break;
            
        default:
            // DID tidak didukung untuk writing
            response[0] = 0x7F;
            response[1] = 0x2E;
            response[2] = 0x31;  // requestOutOfRange
            *response_length = 3;
            return E_OK;
    }
    
    if (write_result != E_OK) {
        response[0] = 0x7F;
        response[1] = 0x2E;
        response[2] = 0x72;  // generalProgrammingFailure
        *response_length = 3;
        return E_OK;
    }
    
    // Build positive response
    response[0] = 0x6E;  // 0x2E + 0x40
    response[1] = request[1];
    response[2] = request[2];
    *response_length = 3;
    
    return E_OK;
}

static Std_ReturnType Uds_WriteCalibrationId(const uint8_t* data, uint16_t length) {
    // Calibration ID harus 16 byte
    if (length != 16) {
        return E_NOT_OK;
    }
    
    // Tulis ke EEPROM/Flash
    return Eeprom_Write(CAL_ID_ADDRESS, data, 16);
}

static Std_ReturnType Uds_WriteSoftwareVersion(const uint8_t* data, uint16_t length) {
    // Software version max 20 karakter
    if (length > 20) {
        return E_NOT_OK;
    }
    
    // Tulis ke EEPROM
    return Eeprom_Write(SW_VERSION_ADDRESS, data, length);
}

static Std_ReturnType Uds_WriteSystemSupplierId(const uint8_t* data, uint16_t length) {
    // Supplier ID max 10 karakter
    if (length > 10) {
        return E_NOT_OK;
    }
    
    return Eeprom_Write(SUPPLIER_ID_ADDRESS, data, length);
}

static Std_ReturnType Uds_WriteConfigurationData(const uint8_t* data, uint16_t length) {
    // Configuration data max 64 byte
    if (length > 64) {
        return E_NOT_OK;
    }
    
    // Validasi checksum configuration
    if (!Uds_ValidateConfigurationChecksum(data, length)) {
        return E_NOT_OK;
    }
    
    return Eeprom_Write(CONFIG_DATA_ADDRESS, data, length);
}
```

---

## 5. Security Access Mechanism

### 5.1 Konsep Security Access

Security Access (SID 0x27) adalah mekanisme autentikasi untuk melindungi fungsi-fungsi sensitif seperti:
- Writing calibration data
- ECU reset
- Flash programming
- I/O control
- DTC clearing

### 5.2 Seed-Key Algorithm

```
Tester                          ECU
  |                              |
  |-- Request Seed (odd SF) ---> |
  |                              | Generate Seed
  |<-- Send Seed ----------------|
  |                              |
  | Calculate Key                |
  |                              |
  |-- Send Key (even SF) ------> |
  |                              | Verify Key
  |<-- Access Granted -----------| or Denied
```

### 5.3 Implementasi Security Access

```c
#define SECURITY_LEVEL_1        0x01  // Base level
#define SECURITY_LEVEL_2        0x02  // Programming
#define SECURITY_LEVEL_3        0x03  // Engineering
#define MAX_FAILED_ATTEMPTS     3
#define LOCKOUT_TIME_MS         10000  // 10 detik

// Seed generation function (contoh sederhana)
static uint32_t Uds_GenerateSeed(void) {
    // Gunakan hardware RNG atau timer-based seed
    uint32_t seed = 0;
    
    #ifdef HAVE_HW_RNG
        seed = HwRng_GetRandom();
    #else
        // Fallback: gunakan timer value sebagai entropy
        seed = (uint32_t)GetTimerValue() ^ (uint32_t)GetTickCount();
    #endif
    
    // Pastikan seed tidak 0
    if (seed == 0) {
        seed = 0x12345678;
    }
    
    return seed;
}

// Key calculation algorithm (contoh sederhana)
static uint32_t Uds_CalculateKey(uint32_t seed, uint8_t security_level) {
    uint32_t key = seed;
    
    // Simple XOR-based algorithm (ganti dengan algoritma proprietary)
    switch (security_level) {
        case SECURITY_LEVEL_1:
            key ^= 0x12345678;
            key = ((key << 5) | (key >> 27)) ^ 0x87654321;
            break;
            
        case SECURITY_LEVEL_2:
            key ^= 0xABCDEF01;
            key = ((key << 7) | (key >> 25)) ^ 0x10FEDCBA;
            break;
            
        case SECURITY_LEVEL_3:
            key ^= 0xDEADBEEF;
            key = ((key << 11) | (key >> 21)) ^ 0xEFBEADDE;
            break;
            
        default:
            key = 0;
            break;
    }
    
    return key;
}

Std_ReturnType Uds_SecurityAccess(const uint8_t* request,
                                  uint8_t* response,
                                  uint16_t* response_length) {
    uint8_t sub_function = request[1];
    
    // Validasi panjang request
    if (request[0] < 2) {
        response[0] = 0x7F;
        response[1] = 0x27;
        response[2] = 0x13;  // incorrectMessageLength
        *response_length = 3;
        return E_OK;
    }
    
    // Cek apakah ECU dalam kondisi locked
    if (uds_context.state == UDS_STATE_SECURITY_LOCKED) {
        if (GetTickCount() < uds_context.lock_timer) {
            response[0] = 0x7F;
            response[1] = 0x27;
            response[2] = 0x37;  // requiredTimeDelayNotExpired
            *response_length = 3;
            return E_OK;
        } else {
            // Reset lock status
            uds_context.state = UDS_STATE_IDLE;
            uds_context.failed_attempts = 0;
        }
    }
    
    if (sub_function % 2 == 1) {
        // Odd sub-function: Request Seed
        uint8_t security_level = (sub_function + 1) / 2;
        
        // Cek apakah security level didukung
        if (security_level > SECURITY_LEVEL_3) {
            response[0] = 0x7F;
            response[1] = 0x27;
            response[2] = 0x12;  // subFunctionNotSupported
            *response_length = 3;
            return E_OK;
        }
        
        // Generate dan simpan seed
        uint32_t seed = Uds_GenerateSeed();
        uds_context.current_seed = seed;
        uds_context.pending_security_level = security_level;
        
        // Build response
        response[0] = 0x67;  // 0x27 + 0x40
        response[1] = sub_function;
        response[2] = (seed >> 24) & 0xFF;
        response[3] = (seed >> 16) & 0xFF;
        response[4] = (seed >> 8) & 0xFF;
        response[5] = seed & 0xFF;
        *response_length = 6;
        
    } else {
        // Even sub-function: Send Key
        uint8_t security_level = sub_function / 2;
        
        // Validasi bahwa request seed sudah dilakukan
        if (uds_context.pending_security_level != security_level) {
            response[0] = 0x7F;
            response[1] = 0x27;
            response[2] = 0x22;  // conditionsNotCorrect
            *response_length = 3;
            return E_OK;
        }
        
        // Validasi panjang request untuk key
        if (request[0] != 6) {
            response[0] = 0x7F;
            response[1] = 0x27;
            response[2] = 0x13;  // incorrectMessageLength
            *response_length = 3;
            return E_OK;
        }
        
        // Extract key dari request
        uint32_t received_key = ((uint32_t)request[2] << 24) |
                               ((uint32_t)request[3] << 16) |
                               ((uint32_t)request[4] << 8) |
                               request[5];
        
        // Calculate expected key
        uint32_t expected_key = Uds_CalculateKey(uds_context.current_seed, security_level);
        
        if (received_key == expected_key) {
            // Key valid - grant access
            uds_context.security_level = security_level;
            uds_context.pending_security_level = 0;
            uds_context.current_seed = 0;
            uds_context.failed_attempts = 0;
            
            response[0] = 0x67;  // 0x27 + 0x40
            response[1] = sub_function;
            *response_length = 2;
            
            Uds_LogEvent(UDS_EVENT_SECURITY_GRANTED, security_level);
            
        } else {
            // Key invalid
            uds_context.failed_attempts++;
            
            if (uds_context.failed_attempts >= MAX_FAILED_ATTEMPTS) {
                // Lock ECU
                uds_context.state = UDS_STATE_SECURITY_LOCKED;
                uds_context.lock_timer = GetTickCount() + LOCKOUT_TIME_MS;
                
                response[0] = 0x7F;
                response[1] = 0x27;
                response[2] = 0x36;  // exceededNumberOfAttempts
                *response_length = 3;
                
                Uds_LogEvent(UDS_EVENT_SECURITY_LOCKED, 0);
                
            } else {
                response[0] = 0x7F;
                response[1] = 0x27;
                response[2] = 0x35;  // invalidKey
                *response_length = 3;
                
                Uds_LogEvent(UDS_EVENT_SECURITY_DENIED, uds_context.failed_attempts);
            }
            
            uds_context.pending_security_level = 0;
            uds_context.current_seed = 0;
        }
    }
    
    return E_OK;
}
```

---

## 6. Data Identifier Management

### 6.1 DID Structure dan Organization

```c
typedef enum {
    DID_CATEGORY_VEHICLE = 0,      // 0xF000-0xF0FF
    DID_CATEGORY_SYSTEM = 1,       // 0xF100-0xF1FF
    DID_CATEGORY_COMPONENT = 2,    // 0xF200-0xF2FF
    DID_CATEGORY_MANUFACTURER = 3, // 0xF400-0xF7FF
    DID_CATEGORY_CUSTOM = 4        // 0xF800-0xFEFF
} DidCategoryType;

typedef struct {
    uint16_t did;
    DidCategoryType category;
    uint16_t data_length;
    uint8_t read_access_level;
    uint8_t write_access_level;
    bool is_dynamic;
    void (*read_handler)(uint8_t* buffer, uint16_t* length);
    Std_ReturnType (*write_handler)(const uint8_t* data, uint16_t length);
} DidEntryType;

// Tabel DID definitions
static const DidEntryType did_table[] = {
    {DID_VIN, DID_CATEGORY_VEHICLE, 17, 0, 0, false, Uds_ReadVin, NULL},
    {DID_SYSTEM_SUPPLIER_ID, DID_CATEGORY_SYSTEM, 10, 0, 2, false, Uds_ReadSystemSupplierId, Uds_WriteSystemSupplierId},
    {DID_CALIBRATION_ID, DID_CATEGORY_SYSTEM, 16, 0, 2, false, Uds_ReadCalibrationId, Uds_WriteCalibrationId},
    {DID_SOFTWARE_VERSION, DID_CATEGORY_SYSTEM, 20, 0, 2, false, Uds_ReadSoftwareVersion, Uds_WriteSoftwareVersion},
    {DID_ENGINE_SPEED, DID_CATEGORY_COMPONENT, 2, 0, 0, false, Uds_ReadEngineSpeed, NULL},
    {DID_VEHICLE_SPEED, DID_CATEGORY_COMPONENT, 1, 0, 0, false, Uds_ReadVehicleSpeed, NULL},
    // ... tambahkan DID lainnya
};

#define DID_TABLE_SIZE (sizeof(did_table) / sizeof(did_table[0]))
```

### 6.2 Dynamic DID Definition

```c
Std_ReturnType Uds_DynamicallyDefineDataIdentifier(const uint8_t* request,
                                                   uint8_t* response,
                                                   uint16_t* response_length) {
    uint8_t sub_function = request[1];
    
    switch (sub_function) {
        case 0x01:  // defineByIdentifier
            return Uds_DefineDidByIdentifier(request, response, response_length);
            
        case 0x02:  // defineByMemoryAddress
            return Uds_DefineDidByMemoryAddress(request, response, response_length);
            
        case 0x03:  // clearDynamicIdentifiers
            return Uds_ClearDynamicIdentifiers(request, response, response_length);
            
        default:
            response[0] = 0x7F;
            response[1] = 0x2C;
            response[2] = 0x12;  // subFunctionNotSupported
            *response_length = 3;
            return E_OK;
    }
}

static Std_ReturnType Uds_DefineDidByIdentifier(const uint8_t* request,
                                                uint8_t* response,
                                                uint16_t* response_length) {
    // Validasi security access
    if (uds_context.security_level < SECURITY_LEVEL_2) {
        response[0] = 0x7F;
        response[1] = 0x2C;
        response[2] = 0x33;  // securityAccessDenied
        *response_length = 3;
        return E_OK;
    }
    
    // Extract parameters dari request
    uint16_t new_did = ((uint16_t)request[2] << 8) | request[3];
    uint8_t source_count = request[4];
    
    // Validasi range DID untuk dynamic definition
    if (new_did < 0xF800 || new_did > 0xFEFF) {
        response[0] = 0x7F;
        response[1] = 0x2C;
        response[2] = 0x31;  // requestOutOfRange
        *response_length = 3;
        return E_OK;
    }
    
    // Parse source identifiers
    uint16_t offset = 5;
    for (uint8_t i = 0; i < source_count; i++) {
        uint16_t source_did = ((uint16_t)request[offset] << 8) | request[offset + 1];
        uint8_t position_in_new_did = request[offset + 2];
        uint8_t memory_size = request[offset + 3];
        
        // Validasi source DID exists
        if (!Uds_DidExists(source_did)) {
            response[0] = 0x7F;
            response[1] = 0x2C;
            response[2] = 0x31;  // requestOutOfRange
            *response_length = 3;
            return E_OK;
        }
        
        offset += 4;
    }
    
    // Register dynamic DID (implementasi tergantung sistem)
    Uds_RegisterDynamicDid(new_did, &request[5], request[0] - 5);
    
    // Build positive response
    response[0] = 0x6C;  // 0x2C + 0x40
    response[1] = sub_function;
    response[2] = request[2];
    response[3] = request[3];
    *response_length = 4;
    
    return E_OK;
}
```

---

## 7. Routine Control Implementation

### 7.1 Routine Types

```c
#define ROUTINE_SUB_START           0x01
#define ROUTINE_SUB_STOP            0x02
#define ROUTINE_SUB_REQUEST_RESULT  0x03

// Routine IDs contoh
#define ROUTINE_ERASE_MEMORY        0xFF00
#define ROUTINE_CHECKSUM_CALC       0xFF01
#define ROUTINE_IO_CONTROL_TEST     0xFF02
#define ROUTINE_SENSOR_CALIBRATION  0xFF03
#define ROUTINE_ACTUATOR_TEST       0xFF04
```

### 7.2 Implementasi Routine Control

```c
typedef struct {
    uint16_t routine_id;
    uint8_t required_security_level;
    bool (*start_handler)(const uint8_t* option_record, uint8_t length);
    bool (*stop_handler)(void);
    bool (*result_handler)(uint8_t* result_data, uint16_t* length);
    bool is_running;
    uint32_t start_time;
    uint32_t timeout_ms;
} RoutineEntryType;

// Tabel routine definitions
static RoutineEntryType routine_table[] = {
    {ROUTINE_ERASE_MEMORY, SECURITY_LEVEL_2, Uds_StartEraseMemory, NULL, Uds_GetEraseResult, false, 0, 30000},
    {ROUTINE_CHECKSUM_CALC, SECURITY_LEVEL_1, Uds_StartChecksumCalc, NULL, Uds_GetChecksumResult, false, 0, 5000},
    {ROUTINE_IO_CONTROL_TEST, SECURITY_LEVEL_1, Uds_StartIoTest, Uds_StopIoTest, Uds_GetIoTestResult, false, 0, 10000},
    {ROUTINE_SENSOR_CALIBRATION, SECURITY_LEVEL_2, Uds_StartSensorCalib, NULL, Uds_GetCalibResult, false, 0, 60000},
    {ROUTINE_ACTUATOR_TEST, SECURITY_LEVEL_1, Uds_StartActuatorTest, Uds_StopActuatorTest, Uds_GetActuatorResult, false, 0, 15000}
};

#define ROUTINE_TABLE_SIZE (sizeof(routine_table) / sizeof(routine_table[0]))

Std_ReturnType Uds_RoutineControl(const uint8_t* request,
                                  uint8_t* response,
                                  uint16_t* response_length) {
    uint8_t sub_function = request[1];
    uint16_t routine_id = ((uint16_t)request[2] << 8) | request[3];
    
    // Cari routine dalam tabel
    RoutineEntryType* routine = Uds_FindRoutine(routine_id);
    if (routine == NULL) {
        response[0] = 0x7F;
        response[1] = 0x31;
        response[2] = 0x12;  // subFunctionNotSupported (routine not found)
        *response_length = 3;
        return E_OK;
    }
    
    // Cek security access
    if (uds_context.security_level < routine->required_security_level) {
        response[0] = 0x7F;
        response[1] = 0x31;
        response[2] = 0x33;  // securityAccessDenied
        *response_length = 3;
        return E_OK;
    }
    
    switch (sub_function) {
        case ROUTINE_SUB_START:
            return Uds_StartRoutine(routine, request, response, response_length);
            
        case ROUTINE_SUB_STOP:
            return Uds_StopRoutine(routine, request, response, response_length);
            
        case ROUTINE_SUB_REQUEST_RESULT:
            return Uds_RequestRoutineResult(routine, request, response, response_length);
            
        default:
            response[0] = 0x7F;
            response[1] = 0x31;
            response[2] = 0x12;  // subFunctionNotSupported
            *response_length = 3;
            return E_OK;
    }
}

static Std_ReturnType Uds_StartRoutine(RoutineEntryType* routine,
                                       const uint8_t* request,
                                       uint8_t* response,
                                       uint16_t* response_length) {
    // Cek apakah routine sudah running
    if (routine->is_running) {
        response[0] = 0x7F;
        response[1] = 0x31;
        response[2] = 0x72;  // generalProgrammingFailure (already running)
        *response_length = 3;
        return E_OK;
    }
    
    // Cek apakah ada routine lain yang sedang running
    if (Uds_IsAnyRoutineRunning()) {
        response[0] = 0x7F;
        response[1] = 0x31;
        response[2] = 0x22;  // conditionsNotCorrect
        *response_length = 3;
        return E_OK;
    }
    
    // Extract option record (jika ada)
    uint8_t option_length = request[0] - 4;
    const uint8_t* option_record = (option_length > 0) ? &request[4] : NULL;
    
    // Panggil start handler
    if (routine->start_handler != NULL) {
        if (!routine->start_handler(option_record, option_length)) {
            response[0] = 0x7F;
            response[1] = 0x31;
            response[2] = 0x72;  // generalProgrammingFailure
            *response_length = 3;
            return E_OK;
        }
    }
    
    // Update status routine
    routine->is_running = true;
    routine->start_time = GetTickCount();
    
    // Build positive response
    response[0] = 0x71;  // 0x31 + 0x40
    response[1] = ROUTINE_SUB_START;
    response[2] = request[2];
    response[3] = request[3];
    *response_length = 4;
    
    return E_OK;
}

static Std_ReturnType Uds_StopRoutine(RoutineEntryType* routine,
                                      const uint8_t* request,
                                      uint8_t* response,
                                      uint16_t* response_length) {
    // Cek apakah routine sedang running
    if (!routine->is_running) {
        response[0] = 0x7F;
        response[1] = 0x31;
        response[2] = 0x72;  // generalProgrammingFailure (not running)
        *response_length = 3;
        return E_OK;
    }
    
    // Panggil stop handler (jika ada)
    if (routine->stop_handler != NULL) {
        if (!routine->stop_handler()) {
            response[0] = 0x7F;
            response[1] = 0x31;
            response[2] = 0x72;  // generalProgrammingFailure
            *response_length = 3;
            return E_OK;
        }
    }
    
    // Update status routine
    routine->is_running = false;
    
    // Build positive response
    response[0] = 0x71;  // 0x31 + 0x40
    response[1] = ROUTINE_SUB_STOP;
    response[2] = request[2];
    response[3] = request[3];
    *response_length = 4;
    
    return E_OK;
}

static Std_ReturnType Uds_RequestRoutineResult(RoutineEntryType* routine,
                                               const uint8_t* request,
                                               uint8_t* response,
                                               uint16_t* response_length) {
    // Cek apakah routine sedang running atau sudah selesai
    if (routine->is_running) {
        // Cek timeout
        if (GetTickCount() - routine->start_time > routine->timeout_ms) {
            // Timeout - stop routine
            routine->is_running = false;
            if (routine->stop_handler) {
                routine->stop_handler();
            }
            
            response[0] = 0x7F;
            response[1] = 0x31;
            response[2] = 0x72;  // generalProgrammingFailure (timeout)
            *response_length = 3;
            return E_OK;
        }
        
        // Masih running - kembalikan status
        response[0] = 0x71;  // 0x31 + 0x40
        response[1] = ROUTINE_SUB_REQUEST_RESULT;
        response[2] = request[2];
        response[3] = request[3];
        response[4] = 0x01;  // Status: running
        *response_length = 5;
        return E_OK;
    }
    
    // Routine sudah selesai - ambil result
    uint16_t result_length = 0;
    uint8_t result_data[64];
    
    if (routine->result_handler != NULL) {
        if (!routine->result_handler(result_data, &result_length)) {
            response[0] = 0x7F;
            response[1] = 0x31;
            response[2] = 0x72;  // generalProgrammingFailure
            *response_length = 3;
            return E_OK;
        }
    }
    
    // Build positive response dengan result data
    response[0] = 0x71;  // 0x31 + 0x40
    response[1] = ROUTINE_SUB_REQUEST_RESULT;
    response[2] = request[2];
    response[3] = request[3];
    response[4] = 0x02;  // Status: completed
    
    if (result_length > 0) {
        memcpy(&response[5], result_data, result_length);
        *response_length = 5 + result_length;
    } else {
        *response_length = 5;
    }
    
    return E_OK;
}

// Contoh implementasi routine handlers
static bool Uds_StartEraseMemory(const uint8_t* option_record, uint8_t length) {
    // Mulai proses erase memory
    return Flash_EraseAll();
}

static bool Uds_GetEraseResult(uint8_t* result_data, uint16_t* length) {
    // Kembalikan hasil erase
    result_data[0] = Flash_GetEraseStatus();
    *length = 1;
    return (result_data[0] == FLASH_ERASE_SUCCESS);
}

static bool Uds_StartChecksumCalc(const uint8_t* option_record, uint8_t length) {
    // Hitung checksum seluruh firmware
    uint32_t checksum = CalculateFirmwareChecksum();
    Uds_StoreChecksum(checksum);
    return true;
}

static bool Uds_GetChecksumResult(uint8_t* result_data, uint16_t* length) {
    uint32_t checksum = Uds_GetStoredChecksum();
    result_data[0] = (checksum >> 24) & 0xFF;
    result_data[1] = (checksum >> 16) & 0xFF;
    result_data[2] = (checksum >> 8) & 0xFF;
    result_data[3] = checksum & 0xFF;
    *length = 4;
    return true;
}
```

---

## 8. DTC Management System

### 8.1 DTC Structure

```c
#define DTC_FORMAT_SAE_J2012      0x00
#define DTC_FORMAT_ISO_14229      0x01

typedef struct {
    uint32_t dtc_code;           // 3-byte DTC + 1-byte format
    uint8_t dtc_status;
    uint8_t dtc_severity;
    uint8_t functional_unit;
    uint16_t occurrence_counter;
    uint32_t first_failure_time;
    uint32_t last_failure_time;
    uint8_t environment_data[32];
} DtcEntryType;

// DTC Status Bits
#define DTC_STATUS_TEST_FAILED               0x01
#define DTC_STATUS_TEST_FAILED_THIS_OP_CYCLE 0x02
#define DTC_STATUS_PENDING_DTC               0x04
#define DTC_STATUS_CONFIRMED_DTC             0x08
#define DTC_STATUS_WARNING_INDICATOR_ACTIVE  0x10
#define DTC_STATUS_TEST_NOT_COMPLETED        0x20
```

### 8.2 Implementasi DTC Management

```c
#define MAX_DTC_ENTRIES 256
static DtcEntryType dtc_storage[MAX_DTC_ENTRIES];
static uint16_t dtc_count = 0;

Std_ReturnType Uds_ReadDTCInformation(const uint8_t* request,
                                      uint8_t* response,
                                      uint16_t* response_length) {
    uint8_t sub_function = request[1];
    
    switch (sub_function) {
        case 0x02:  // reportNumberOfDTCByStatusMask
            return Uds_ReportNumberOfDtcByStatusMask(request, response, response_length);
            
        case 0x04:  // reportDTCByStatusMask
            return Uds_ReportDtcByStatusMask(request, response, response_length);
            
        case 0x06:  // reportDTCSnapshotIdentification
            return Uds_ReportDtcSnapshotIdentification(request, response, response_length);
            
        case 0x07:  // reportDTCSnapshotRecordByDTCNumber
            return Uds_ReportDtcSnapshotRecordByDtcNumber(request, response, response_length);
            
        case 0x08:  // reportDTCStoredDataByRecordNumber
            return Uds_ReportDtcStoredDataByRecordNumber(request, response, response_length);
            
        case 0x0A:  // reportNumberOfDTCBySeverityMask
            return Uds_ReportNumberOfDtcBySeverityMask(request, response, response_length);
            
        case 0x0B:  // reportDTCBySeverityMask
            return Uds_ReportDtcBySeverityMask(request, response, response_length);
            
        case 0x0F:  // reportFirstTestFailedDTC
            return Uds_ReportFirstTestFailedDtc(request, response, response_length);
            
        case 0x10:  // reportFirstConfirmedDTC
            return Uds_ReportFirstConfirmedDtc(request, response, response_length);
            
        case 0x12:  // reportMostRecentTestFailedDTC
            return Uds_ReportMostRecentTestFailedDtc(request, response, response_length);
            
        case 0x13:  // reportMostRecentConfirmedDTC
            return Uds_ReportMostRecentConfirmedDtc(request, response, response_length);
            
        case 0x1A:  // reportSupportedDTC
            return Uds_ReportSupportedDtc(request, response, response_length);
            
        default:
            response[0] = 0x7F;
            response[1] = 0x19;
            response[2] = 0x12;  // subFunctionNotSupported
            *response_length = 3;
            return E_OK;
    }
}

static Std_ReturnType Uds_ReportDtcByStatusMask(const uint8_t* request,
                                                uint8_t* response,
                                                uint16_t* response_length) {
    uint8_t status_mask = request[2];
    uint16_t response_idx = 3;
    
    // Header response
    response[0] = 0x59;  // 0x19 + 0x40
    response[1] = 0x04;  // sub-function
    response[2] = status_mask;
    
    // Iterasi melalui semua DTC
    for (uint16_t i = 0; i < dtc_count && response_idx < 250; i++) {
        // Filter berdasarkan status mask
        if (dtc_storage[i].dtc_status & status_mask) {
            // Tambahkan DTC ke response
            response[response_idx++] = (dtc_storage[i].dtc_code >> 16) & 0xFF;
            response[response_idx++] = (dtc_storage[i].dtc_code >> 8) & 0xFF;
            response[response_idx++] = dtc_storage[i].dtc_code & 0xFF;
            response[response_idx++] = dtc_storage[i].dtc_status;
        }
    }
    
    *response_length = response_idx;
    return E_OK;
}

Std_ReturnType Uds_ClearDiagnosticInformation(const uint8_t* request,
                                              uint8_t* response,
                                              uint16_t* response_length) {
    uint8_t group_mask = request[1];
    
    // Validasi security access
    if (uds_context.security_level < SECURITY_LEVEL_1) {
        response[0] = 0x7F;
        response[1] = 0x14;
        response[2] = 0x33;  // securityAccessDenied
        *response_length = 3;
        return E_OK;
    }
    
    // Clear DTC berdasarkan group mask
    if (group_mask == 0xFF) {
        // Clear semua DTC
        dtc_count = 0;
        memset(dtc_storage, 0, sizeof(dtc_storage));
    } else {
        // Clear DTC berdasarkan group (implementasi spesifik)
        Uds_ClearDtcByGroup(group_mask);
    }
    
    // Build positive response
    response[0] = 0x54;  // 0x14 + 0x40
    *response_length = 1;
    
    return E_OK;
}

Std_ReturnType Uds_ControlDTCSetting(const uint8_t* request,
                                     uint8_t* response,
                                     uint16_t* response_length) {
    uint8_t sub_function = request[1];
    
    switch (sub_function) {
        case 0x01:  // on
            Uds_EnableDtcStorage();
            response[0] = 0x85;  // 0x85 + 0x40
            response[1] = 0x01;
            *response_length = 2;
            return E_OK;
            
        case 0x02:  // off
            Uds_DisableDtcStorage();
            response[0] = 0x85;  // 0x85 + 0x40
            response[1] = 0x02;
            *response_length = 2;
            return E_OK;
            
        default:
            response[0] = 0x7F;
            response[1] = 0x85;
            response[2] = 0x12;  // subFunctionNotSupported
            *response_length = 3;
            return E_OK;
    }
}

// Helper functions untuk DTC management
static void Uds_AddDTC(uint32_t dtc_code, uint8_t severity) {
    if (dtc_count >= MAX_DTC_ENTRIES) {
        return;  // Storage penuh
    }
    
    // Cari apakah DTC sudah ada
    for (uint16_t i = 0; i < dtc_count; i++) {
        if (dtc_storage[i].dtc_code == dtc_code) {
            // Update existing DTC
            dtc_storage[i].dtc_status |= DTC_STATUS_TEST_FAILED;
            dtc_storage[i].dtc_status |= DTC_STATUS_TEST_FAILED_THIS_OP_CYCLE;
            dtc_storage[i].occurrence_counter++;
            dtc_storage[i].last_failure_time = GetTickCount();
            return;
        }
    }
    
    // Tambahkan DTC baru
    dtc_storage[dtc_count].dtc_code = dtc_code;
    dtc_storage[dtc_count].dtc_status = DTC_STATUS_TEST_FAILED | 
                                        DTC_STATUS_TEST_FAILED_THIS_OP_CYCLE |
                                        DTC_STATUS_PENDING_DTC;
    dtc_storage[dtc_count].dtc_severity = severity;
    dtc_storage[dtc_count].occurrence_counter = 1;
    dtc_storage[dtc_count].first_failure_time = GetTickCount();
    dtc_storage[dtc_count].last_failure_time = GetTickCount();
    
    dtc_count++;
}

static void Uds_ConfirmDTC(uint32_t dtc_code) {
    for (uint16_t i = 0; i < dtc_count; i++) {
        if (dtc_storage[i].dtc_code == dtc_code) {
            dtc_storage[i].dtc_status |= DTC_STATUS_CONFIRMED_DTC;
            return;
        }
    }
}
```

---

## 9. Session Management

### 9.1 Session Timeout Handling

```c
void Uds_MainFunction(void) {
    static uint32_t last_activity_time = 0;
    
    // Update activity timer
    if (uds_context.tester_present_active) {
        last_activity_time = GetTickCount();
    }
    
    // Cek session timeout
    uint32_t current_time = GetTickCount();
    uint32_t elapsed_time = current_time - last_activity_time;
    
    if (elapsed_time > uds_context.session_timeout) {
        // Session timeout - kembali ke default session
        if (uds_context.current_session != SESSION_DEFAULT) {
            uds_context.current_session = SESSION_DEFAULT;
            uds_context.security_level = 0;
            uds_context.session_timeout = 5000;  // Default timeout
            
            Uds_LogEvent(UDS_EVENT_SESSION_TIMEOUT, 0);
        }
    }
    
    // Proses pending operations
    Uds_ProcessPendingOperations();
    
    // Handle routine timeouts
    Uds_CheckRoutineTimeouts();
    
    // Handle security lockout expiry
    if (uds_context.state == UDS_STATE_SECURITY_LOCKED) {
        if (current_time >= uds_context.lock_timer) {
            uds_context.state = UDS_STATE_IDLE;
            uds_context.failed_attempts = 0;
        }
    }
}
```

### 9.2 Tester Present Monitoring

```c
Std_ReturnType Uds_TesterPresent(const uint8_t* request,
                                 uint8_t* response,
                                 uint16_t* response_length) {
    uint8_t sub_function = request[1];
    
    // Validasi sub-function (harus 0x00)
    if (sub_function != 0x00) {
        response[0] = 0x7F;
        response[1] = 0x3E;
        response[2] = 0x12;  // subFunctionNotSupported
        *response_length = 3;
        return E_OK;
    }
    
    // Reset session timer
    uds_context.tester_present_active = 1;
    uds_context.last_tester_present = GetTickCount();
    
    // Build positive response
    response[0] = 0x7E;  // 0x3E + 0x40
    response[1] = 0x00;
    *response_length = 2;
    
    return E_OK;
}
```

---

## 10. Testing dan Validation

### 10.1 Test Cases untuk UDS Services

```c
// Unit test untuk Diagnostic Session Control
void test_DiagnosticSessionControl_DefaultToProgramming(void) {
    uint8_t request[] = {0x02, 0x10, 0x02};  // Session Control, Programming
    uint8_t response[256];
    uint16_t response_length;
    
    // Setup: pastikan dalam default session
    uds_context.current_session = SESSION_DEFAULT;
    
    // Execute
    Std_ReturnType result = Uds_DiagnosticSessionControl(request, response, &response_length);
    
    // Assert
    TEST_ASSERT_EQUAL(E_OK, result);
    TEST_ASSERT_EQUAL(0x50, response[0]);  // Positive response
    TEST_ASSERT_EQUAL(0x02, response[1]);  // Programming session
    TEST_ASSERT_EQUAL(SESSION_PROGRAMMING, uds_context.current_session);
}

void test_DiagnosticSessionControl_InvalidTransition(void) {
    uint8_t request[] = {0x02, 0x10, 0x02};  // Session Control, Programming
    uint8_t response[256];
    uint16_t response_length;
    
    // Setup: dalam programming session, coba ke extended (invalid)
    uds_context.current_session = SESSION_PROGRAMMING;
    
    // Execute
    Std_ReturnType result = Uds_DiagnosticSessionControl(request, response, &response_length);
    
    // Assert
    TEST_ASSERT_EQUAL(E_OK, result);
    TEST_ASSERT_EQUAL(0x7F, response[0]);  // Negative response
    TEST_ASSERT_EQUAL(0x10, response[1]);  // Original SID
    TEST_ASSERT_EQUAL(0x22, response[2]);  // conditionsNotCorrect
}

// Unit test untuk Security Access
void test_SecurityAccess_ValidKey(void) {
    uint8_t request_seed[] = {0x02, 0x27, 0x01};  // Security Access, Level 1
    uint8_t response[256];
    uint16_t response_length;
    
    // Request seed
    Uds_SecurityAccess(request_seed, response, &response_length);
    
    // Extract seed dari response
    uint32_t seed = ((uint32_t)response[2] << 24) |
                   ((uint32_t)response[3] << 16) |
                   ((uint32_t)response[4] << 8) |
                   response[5];
    
    // Calculate key
    uint32_t key = Uds_CalculateKey(seed, SECURITY_LEVEL_1);
    
    // Send key
    uint8_t request_key[] = {
        0x06, 0x27, 0x02,
        (key >> 24) & 0xFF,
        (key >> 16) & 0xFF,
        (key >> 8) & 0xFF,
        key & 0xFF
    };
    
    // Execute
    Std_ReturnType result = Uds_SecurityAccess(request_key, response, &response_length);
    
    // Assert
    TEST_ASSERT_EQUAL(E_OK, result);
    TEST_ASSERT_EQUAL(0x67, response[0]);  // Positive response
    TEST_ASSERT_EQUAL(SECURITY_LEVEL_1, uds_context.security_level);
}

// Unit test untuk Read Data By Identifier
void test_ReadDataByIdentifier_VIN(void) {
    uint8_t request[] = {0x03, 0x22, 0xF1, 0x90};  // Read DID, VIN
    uint8_t response[256];
    uint16_t response_length;
    
    // Execute
    Std_ReturnType result = Uds_ReadDataByIdentifier(request, response, &response_length);
    
    // Assert
    TEST_ASSERT_EQUAL(E_OK, result);
    TEST_ASSERT_EQUAL(0x62, response[0]);  // Positive response
    TEST_ASSERT_EQUAL(0xF1, response[1]);  // DID high byte
    TEST_ASSERT_EQUAL(0x90, response[2]);  // DID low byte
    TEST_ASSERT_EQUAL(20, response_length);  // 3 header + 17 VIN
}
```

### 10.2 Integration Testing dengan CAN TP

```c
void test_UdsOverCanTp_MultiFrameRead(void) {
    // Setup: Request data yang besar (> 7 byte)
    uint8_t request[] = {0x03, 0x22, 0xF1, 0x86};  // Read Calibration ID (16 byte)
    
    // Simulasi CAN TP layer
    CanTp_Request request_tp;
    request_tp.data = request;
    request_tp.length = 4;
    request_tp.address = 0x7DF;  // Functional address
    
    // Execute melalui CAN TP
    CanTp_SendRequest(&request_tp);
    
    // Wait untuk response
    CanTp_Response response_tp;
    CanTp_WaitForResponse(&response_tp, 5000);
    
    // Assert
    TEST_ASSERT_GREATER_THAN(0, response_tp.length);
    TEST_ASSERT_EQUAL(0x62, response_tp.data[0]);  // Positive response
    TEST_ASSERT_EQUAL(16, response_tp.length - 3);  // 16 byte data
}
```

---

## 11. Troubleshooting Guide

### 11.1 Common Issues dan Solutions

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| Negative Response 0x12 | Sub-function tidak didukung | Periksa spesifikasi SID dan sub-function |
| Negative Response 0x13 | Panjang pesan salah | Validasi panjang request sesuai standar |
| Negative Response 0x22 | Kondisi tidak tepat | Cek session, security level, atau state ECU |
| Negative Response 0x33 | Security access ditolak | Lakukan security access dengan benar |
| Negative Response 0x35 | Invalid key | Periksa algoritma seed-key |
| Negative Response 0x36 | Too many attempts | Tunggu lockout time expired |
| Negative Response 0x72 | General programming failure | Cek resource availability dan validasi parameter |

### 11.2 Debugging Tips

```c
// Enable debug logging
#define UDS_DEBUG_ENABLED 1

#if UDS_DEBUG_ENABLED
    #define UDS_LOG(level, message, ...) \
        printf("[UDS-%s] " message "\n", level, ##__VA_ARGS__)
#else
    #define UDS_LOG(level, message, ...)
#endif

// Contoh penggunaan dalam code
Std_ReturnType Uds_SecurityAccess(...) {
    UDS_LOG("DEBUG", "Security Access request: sub_function=%d", sub_function);
    
    if (received_key != expected_key) {
        UDS_LOG("ERROR", "Invalid key received. Expected: 0x%08X, Got: 0x%08X", 
                expected_key, received_key);
    }
    
    // ... rest of implementation
}
```

---

## 12. Glosarium Jepang-Inggris

| Japanese | Romaji | English | Indonesia |
|----------|--------|---------|-----------|
| 診断サービス | Shindan sābisu | Diagnostic Services | Layanan Diagnostik |
| セキュリティアクセス | Sekyuriti akusesu | Security Access | Akses Keamanan |
| データ識別子 | Dēta shikibetsu | Data Identifier | Pengenal Data |
| ルーチン制御 | Rūchin seigyo | Routine Control | Kontrol Rutin |
| エラーコード | Erā kōdo | Error Code | Kode Kesalahan |
| 認証 | Ninshō | Authentication | Autentikasi |
| セッション管理 | Sesshon kanri | Session Management | Manajemen Sesi |
| アップデート | Appudēto | Update | Pembaruan |
| 書き込み | Kakikomi | Write | Tulis |
| 読み取り | Yomitori | Read | Baca |
| リセット | Risetto | Reset | Atur Ulang |
| 確認 | Kakunin | Confirmation | Konfirmasi |
| 無効化 | Mukōka | Disable | Nonaktifkan |
| 有効化 | Yūkōka | Enable | Aktifkan |
| タイムアウト | Taimuauto | Timeout | Waktu Habis |

---

## 📝 Latihan dan Tantangan

### Latihan 1: Implementasi Custom DID
Buat DID custom untuk membaca sensor temperature dengan format:
- DID: 0xF250
- Data: 2 byte (resolution 0.1°C/bit, offset -40°C)
- Access: Read-only, no security required

### Latihan 2: Security Algorithm
Implementasikan algoritma seed-key yang lebih kompleks menggunakan:
- Linear Feedback Shift Register (LFSR)
- Lookup table based transformation
- Time-based seed variation

### Latihan 3: Routine Development
Buat routine untuk kalibrasi sensor dengan fitur:
- Auto-calibration sequence
- Progress reporting
- Result validation
- Rollback capability

---

## 🔗 Referensi dan Materi Terkait

- **Prasyarat**: [CAN TP Deep Dive](./09_CAN_TP_DeepDive.md)
- **Lanjutan**: [Bootloader Development](./04_Bootloader_Dev_Guide.md)
- **Standar**: ISO 14229-1:2020, ISO 15765-2
- **Tools**: Vector CANoe, PCAN-View, SocketCAN

---

*Dokumentasi ini merupakan bagian dari Embedded C Automotive Learning Path. Untuk materi lengkap, lihat [README](./README.md).*
