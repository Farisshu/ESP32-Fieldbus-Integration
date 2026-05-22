# 🔧 UDS Protocol Master (ISO 14229)
## Complete Implementation Guide for Automotive Diagnostics

---

### 📋 Quick Reference

| Attribute | Value |
|-----------|-------|
| **Standard** | ISO 14229-1 / ISO 14229-2 |
| **Layer** | Application Layer (Layer 7) |
| **Transport** | ISO 15765-2 (CAN TP) |
| **Max Services** | 256 Service IDs |
| **Prerequisites** | CAN TP, Basic C/C++ |
| **Difficulty** | ⭐⭐⭐⭐ Advanced |
| **Japanese** | 統一診断サービス (Tōitsu Shindan Sābisu) |

---

## 🎯 Learning Objectives

Setelah mempelajari materi ini, Anda akan mampu:
1. ✅ Memahami arsitektur UDS dan hubungannya dengan CAN TP
2. ✅ Menguasai semua Service ID (SID) utama
3. ✅ Mengimplementasikan Security Access (0x27)
4. ✅ Membuat UDS server untuk ECU simulasi
5. ✅ Menangani positive dan negative responses
6. ✅ Debug komunikasi UDS dengan tool diagnostik

---

## 1. UDS Architecture Overview

### 1.1 Protocol Stack

```
┌─────────────────────────────────────┐
│     Application Layer (UDS)         │  ← ISO 14229
├─────────────────────────────────────┤
│   Transport Layer (CAN TP)          │  ← ISO 15765-2
├─────────────────────────────────────┤
│      Data Link Layer (CAN)          │  ← ISO 11898
├─────────────────────────────────────┤
│       Physical Layer                │
└─────────────────────────────────────┘
```

### 1.2 Client-Server Model

```
┌──────────────┐                    ┌──────────────┐
│   Client     │                    │    Server    │
│  (Tester)    │                    │    (ECU)     │
│              │                    │              │
│  Send Request│ ─────────────────→ │ Receive      │
│              │                    │ Process      │
│  Receive Resp│ ←───────────────── │ Send Response│
└──────────────┘                    └──────────────┘
```

### 1.3 Message Structure

**Request Format:**
```
[SID] [Sub-function] [Data...]
```

**Positive Response Format:**
```
[SID + 0x40] [Sub-function] [Data...]
```

**Negative Response Format:**
```
[0x7F] [SID] [ResponseCode]
```

---

## 2. Service ID (SID) Reference

### 2.1 Complete SID Table

| SID | Service Name | Description | Sub-function |
|-----|-------------|-------------|--------------|
| **0x10** | DiagnosticSessionControl | Mengubah mode sesi | 0x01-0x03 |
| **0x11** | ECUReset | Reset ECU | 0x01-0x04 |
| **0x22** | ReadDataByIdentifier | Baca data DID | - |
| **0x23** | ReadMemoryByAddress | Baca dari alamat memori | - |
| **0x27** | SecurityAccess | Akses keamanan (unlock) | 0x01-0x42 |
| **0x28** | CommunicationControl | Kontrol komunikasi | 0x00-0x03 |
| **0x2E** | WriteDataByIdentifier | Tulis data DID | - |
| **0x31** | RoutineControl | Jalankan routine | 0x01-0x03 |
| **0x34** | RequestDownload | Persiapan download | - |
| **0x36** | TransferData | Transfer data blok | - |
| **0x37** | RequestTransferExit | Akhiri transfer | - |
| **0x3E** | TesterPresent | Keep session alive | 0x00 |
| **0x85** | ControlDTCSetting | Aktifkan/nonaktifkan DTC | 0x00-0x01 |

### 2.2 Response Code (NRC) Reference

| NRC | Name | Description |
|-----|------|-------------|
| **0x10** | generalReject | Rejection umum |
| **0x11** | serviceNotSupported | Service tidak didukung |
| **0x12** | subFunctionNotSupported | Sub-function tidak didukung |
| **0x13** | incorrectMessageLengthOrInvalidFormat | Panjang pesan salah |
| **0x22** | conditionsNotCorrect | Kondisi tidak terpenuhi |
| **0x24** | requestSequenceError | Urutan request salah |
| **0x25** | requestOutOfRange | Request di luar range |
| **0x27** | securityAccessDenied | Akses keamanan ditolak |
| **0x31** | requestInvalid | Request tidak valid |
| **0x33** | securityAccessDeniedExceeded | Kesalahan security terlalu banyak |
| **0x78** | responsePending | Response masih diproses |
| **0x83** | serviceNotSupportedInActiveSession | Service tidak aktif di sesi ini |

---

## 3. Detailed Service Implementation

### 3.1 Diagnostic Session Control (0x10)

**Purpose**: Mengubah mode diagnostik ECU.

**Sessions:**
| Sub-function | Name | Description |
|-------------|------|-------------|
| 0x01 | Default Session | Mode normal |
| 0x02 | Programming Session | Mode flashing |
| 0x03 | Extended Diagnostic Session | Mode diagnostik lengkap |
| 0x40+ | Safety System Session | Mode safety critical |

**Request Example:**
```
[10 03] → Enter Extended Diagnostic Session
```

**Positive Response:**
```
[50 03 00 32 01 F4] 
         │  │  │
         │  │  └─ P2 Server Max (500ms)
         │  └──── P2 Server Min (50ms)
         └─────── Session ID (0x03)
```

**Implementation:**
```c
typedef enum {
    SESSION_DEFAULT      = 0x01,
    SESSION_PROGRAMMING  = 0x02,
    SESSION_EXTENDED     = 0x03,
    SESSION_SAFETY       = 0x40
} uds_session_t;

typedef struct {
    uds_session_t current_session;
    uint32_t session_timeout_ms;
    uint32_t last_activity_time;
    bool is_session_active;
} uds_session_context_t;

/**
 * @brief Handle Diagnostic Session Control
 */
int uds_handle_diagnostic_session_control(uds_session_context_t *ctx,
                                          uint8_t sub_function) {
    // Validate sub-function
    if (sub_function != SESSION_DEFAULT &&
        sub_function != SESSION_PROGRAMMING &&
        sub_function != SESSION_EXTENDED &&
        sub_function != SESSION_SAFETY) {
        return uds_send_negative_response(0x10, 0x12); // subFunctionNotSupported
    }
    
    // Check security requirements
    if (sub_function == SESSION_PROGRAMMING || 
        sub_function == SESSION_SAFETY) {
        if (!ctx->is_security_unlocked) {
            return uds_send_negative_response(0x10, 0x27); // securityAccessDenied
        }
    }
    
    // Change session
    ctx->current_session = (uds_session_t)sub_function;
    ctx->is_session_active = true;
    ctx->last_activity_time = get_current_time_ms();
    
    // Set timeout based on session
    switch (sub_function) {
        case SESSION_DEFAULT:
            ctx->session_timeout_ms = 5000;
            break;
        case SESSION_PROGRAMMING:
            ctx->session_timeout_ms = 60000;
            break;
        case SESSION_EXTENDED:
            ctx->session_timeout_ms = 10000;
            break;
    }
    
    // Send positive response
    uint8_t response[6] = {0x50, sub_function, 0x00, 0x32, 0x01, 0xF4};
    return uds_send_response(response, sizeof(response));
}
```

### 3.2 Security Access (0x27) - CRITICAL

**Purpose**: Membuka akses ke fungsi proteksi (flashing, calibration).

**Algorithm**: Seed-Key Challenge-Response

**Flow:**
```
Client                          Server
  │                              │
  │── [27 01] Request Seed ────▶│
  │                              │ Calculate seed
  │◀── [67 01] [Seed] ──────────│ Send seed
  │                              │
  │ Calculate key from seed      │
  │                              │
  │── [27 02] [Key] ───────────▶│ Verify key
  │                              │
  │◀── [67 02] Success ─────────│ Unlock granted
```

**Sub-functions:**
| Request | Response | Description |
|---------|----------|-------------|
| 0x01 | 0x02 | Request Seed Level 1 |
| 0x03 | 0x04 | Request Seed Level 2 |
| 0x05 | 0x06 | Request Seed Level 3 |
| 0x41 | 0x42 | Send Key Level 1 |
| 0x43 | 0x44 | Send Key Level 2 |
| 0x45 | 0x46 | Send Key Level 3 |

**Implementation:**
```c
#define MAX_SECURITY_ATTEMPTS 3
#define SECURITY_LOCKOUT_TIME_MS 600000  // 10 minutes

typedef struct {
    uint8_t seed_level;
    bool is_unlocked;
    uint8_t failed_attempts;
    uint32_t lockout_time;
    uint32_t (*seed_generator)(void);
    bool (*key_validator)(uint8_t level, uint32_t seed, uint32_t key);
} uds_security_context_t;

/**
 * @brief Generate pseudo-random seed
 */
static uint32_t generate_seed(void) {
    // Simple LFSR-based PRNG (replace with crypto-safe in production)
    static uint32_t lfsr = 0xACE1;
    uint32_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
    lfsr = (lfsr >> 1) | (bit << 31);
    return lfsr;
}

/**
 * @brief Validate key (server-side)
 * In production, this should use a secure algorithm
 */
static bool validate_key(uint8_t level, uint32_t seed, uint32_t key) {
    // Simple XOR-based algorithm (NOT SECURE - for demonstration only)
    // In real implementation, use AES or proprietary algorithm
    uint32_t expected_key = seed ^ 0xBEEFCAFE;
    
    // Add level-specific transformation
    expected_key += (level * 0x12345678);
    
    return (key == expected_key);
}

/**
 * @brief Handle Security Access - Request Seed
 */
int uds_handle_security_access_request_seed(uds_security_context_t *sec,
                                            uint8_t sub_function) {
    // Check lockout
    if (sec->failed_attempts >= MAX_SECURITY_ATTEMPTS) {
        uint32_t elapsed = get_current_time_ms() - sec->lockout_time;
        if (elapsed < SECURITY_LOCKOUT_TIME_MS) {
            return uds_send_negative_response(0x27, 0x33); // exceededAttempts
        } else {
            // Reset after lockout period
            sec->failed_attempts = 0;
        }
    }
    
    // Extract level from sub-function (odd numbers: 0x01, 0x03, 0x05...)
    uint8_t level = (sub_function + 1) / 2;
    
    if (level > 3) {
        return uds_send_negative_response(0x27, 0x12); // subFunctionNotSupported
    }
    
    // Generate and store seed
    uint32_t seed = generate_seed();
    sec->seed_level = level;
    sec->current_seed = seed;
    
    // Send seed in response
    uint8_t response[5];
    response[0] = 0x67;  // SID + 0x40
    response[1] = sub_function;
    response[2] = (seed >> 24) & 0xFF;
    response[3] = (seed >> 16) & 0xFF;
    response[4] = (seed >> 8) & 0xFF;
    // Note: Some implementations send only 2 bytes of seed
    
    return uds_send_response(response, 5);
}

/**
 * @brief Handle Security Access - Send Key
 */
int uds_handle_security_access_send_key(uds_security_context_t *sec,
                                        uint8_t sub_function,
                                        uint32_t client_key) {
    // Validate sub-function (even numbers: 0x02, 0x04, 0x06...)
    if (sub_function < 0x02 || sub_function > 0x42 || (sub_function % 2) != 0) {
        return uds_send_negative_response(0x27, 0x12); // subFunctionNotSupported
    }
    
    uint8_t level = sub_function / 2;
    
    // Check if seed was requested first
    if (level != sec->seed_level) {
        return uds_send_negative_response(0x27, 0x24); // requestSequenceError
    }
    
    // Validate key
    if (validate_key(level, sec->current_seed, client_key)) {
        // Success
        sec->is_unlocked = true;
        sec->failed_attempts = 0;
        
        uint8_t response[2] = {0x67, sub_function};
        return uds_send_response(response, 2);
    } else {
        // Failed attempt
        sec->failed_attempts++;
        
        if (sec->failed_attempts >= MAX_SECURITY_ATTEMPTS) {
            sec->lockout_time = get_current_time_ms();
            return uds_send_negative_response(0x27, 0x33); // exceededAttempts
        }
        
        return uds_send_negative_response(0x27, 0x35); // invalidKey
    }
}
```

### 3.3 Read Data By Identifier (0x22)

**Purpose**: Membaca data dari ECU menggunakan Data Identifier (DID).

**Common DID:**
| DID | Hex | Description | Length |
|-----|-----|-------------|--------|
| VIN | 0xF190 | Vehicle Identification Number | 17 bytes |
| System Supplier | 0xF191 | Nama supplier | Variable |
| Calibration Date | 0xF19C | Tanggal kalibrasi | 10 bytes |
| Software Version | 0xF1A0 | Versi software | Variable |
| Hardware Version | 0xF1A1 | Versi hardware | Variable |

**Request Example:**
```
[22 F1 90] → Read VIN
```

**Positive Response:**
```
[62 F1 90 57 4F 4C 4B 53 57 38 39 30 31 32 33 34 35 36 37 38 39]
 │  │  │  └────────────────────────────────────────────────────┘
 │  │  │                   VIN: WOLKSW890123456789
 │  │  └─ DID
 │  └─ Response SID (0x22 + 0x40)
 └─ Service ID
```

**Implementation:**
```c
typedef struct {
    uint16_t did;
    uint8_t *data_ptr;
    uint16_t data_length;
    bool read_only;
} uds_did_entry_t;

// Define supported DIDs
static const uds_did_entry_t g_did_table[] = {
    {0xF190, g_vin_number, 17, true},
    {0xF191, g_supplier_name, 20, true},
    {0xF19C, g_calibration_date, 10, true},
    {0xF1A0, g_sw_version, 15, true},
    {0xF1A1, g_hw_version, 10, true},
    {0xF18E, g_ecu_serial, 12, true},
    {0xF1DB, g_boot_checksum, 4, true},
};

#define DID_TABLE_SIZE (sizeof(g_did_table) / sizeof(g_did_table[0]))

/**
 * @brief Handle Read Data By Identifier
 */
int uds_handle_read_data_by_identifier(const uint8_t *request, uint8_t length) {
    if (length < 3) {
        return uds_send_negative_response(0x22, 0x13); // incorrectMessageLength
    }
    
    // Extract DID (big-endian)
    uint16_t did = (request[1] << 8) | request[2];
    
    // Search DID table
    for (size_t i = 0; i < DID_TABLE_SIZE; i++) {
        if (g_did_table[i].did == did) {
            // Found DID
            const uds_did_entry_t *entry = &g_did_table[i];
            
            // Build response
            uint8_t response[2 + entry->data_length];
            response[0] = 0x62;  // 0x22 + 0x40
            response[1] = (did >> 8) & 0xFF;
            response[2] = did & 0xFF;
            memcpy(&response[3], entry->data_ptr, entry->data_length);
            
            return uds_send_response(response, sizeof(response));
        }
    }
    
    // DID not found
    return uds_send_negative_response(0x22, 0x15); // requestOutOfRange
}
```

### 3.4 ECU Reset (0x11)

**Purpose**: Melakukan reset ECU dengan berbagai mode.

**Reset Types:**
| Sub-function | Name | Description |
|-------------|------|-------------|
| 0x01 | Hard Reset | Reset penuh seperti power cycle |
| 0x02 | Key Off-On Reset | Reset setelah kunci off-on |
| 0x03 | Soft Reset | Reset software tanpa power cycle |
| 0x04 | Enable Rapid Power Shutdown | Matikan power cepat |

**Request:**
```
[11 01] → Hard Reset
```

**Implementation:**
```c
typedef enum {
    RESET_HARD           = 0x01,
    RESET_KEY_OFF_ON     = 0x02,
    RESET_SOFT           = 0x03,
    RESET_RAPID_SHUTDOWN = 0x04
} uds_reset_type_t;

/**
 * @brief Handle ECU Reset
 */
int uds_handle_ecu_reset(uint8_t sub_function) {
    // Check session permission
    if (g_uds_ctx.current_session == SESSION_DEFAULT) {
        return uds_send_negative_response(0x11, 0x83); // serviceNotSupportedInActiveSession
    }
    
    // Validate reset type
    if (sub_function < RESET_HARD || sub_function > RESET_RAPID_SHUTDOWN) {
        return uds_send_negative_response(0x11, 0x12); // subFunctionNotSupported
    }
    
    // Send positive response BEFORE reset (important!)
    uint8_t response[2] = {0x51, sub_function};
    uds_send_response(response, 2);
    
    // Schedule reset after delay (to allow response transmission)
    switch (sub_function) {
        case RESET_HARD:
            schedule_hardware_reset(100);  // 100ms delay
            break;
        case RESET_SOFT:
            schedule_software_reset(100);
            break;
        case RESET_KEY_OFF_ON:
            set_reset_flag(RESET_PENDING_KEY_CYCLE);
            break;
    }
    
    return 0;
}
```

### 3.5 Tester Present (0x3E)

**Purpose**: Menjaga sesi diagnostik tetap aktif (keep-alive).

**Request:**
```
[3E 00] → Tester Present (suppress response)
[3E 80] → Tester Present (expect response)
```

**Implementation:**
```c
/**
 * @brief Handle Tester Present
 */
int uds_handle_tester_present(uint8_t sub_function) {
    // Update session activity timestamp
    g_uds_ctx.last_activity_time = get_current_time_ms();
    
    // Check suppressPosRspMsgIndicationBit (bit 7)
    bool suppress_response = (sub_function & 0x80) != 0;
    
    if (suppress_response) {
        // Don't send response
        return 0;
    }
    
    // Send positive response
    uint8_t response[2] = {0x7E, sub_function & 0x7F};
    return uds_send_response(response, 2);
}

/**
 * @brief Check session timeout
 */
void uds_check_session_timeout(void) {
    uint32_t elapsed = get_current_time_ms() - g_uds_ctx.last_activity_time;
    
    if (elapsed > g_uds_ctx.session_timeout_ms) {
        // Session timeout - revert to default session
        g_uds_ctx.current_session = SESSION_DEFAULT;
        g_uds_ctx.is_session_active = false;
        g_uds_ctx.is_security_unlocked = false;
        
        printf("UDS Session timeout - reverted to default\n");
    }
}
```

### 3.6 Write Data By Identifier (0x2E)

**Purpose**: Menulis data ke DID tertentu (biasanya memerlukan security access).

**Request:**
```
[2E F1 8E 12 34 56 78 9A BC DE F0 12 34 56]
         │  └──────────────────────────────┘
         │         New serial number
         └─ DID (0xF18E = ECU Serial)
```

**Implementation:**
```c
/**
 * @brief Handle Write Data By Identifier
 */
int uds_handle_write_data_by_identifier(const uint8_t *request, uint8_t length) {
    if (length < 3) {
        return uds_send_negative_response(0x2E, 0x13); // incorrectMessageLength
    }
    
    // Extract DID
    uint16_t did = (request[1] << 8) | request[2];
    
    // Check security access for writable DIDs
    if (!g_uds_ctx.is_security_unlocked) {
        return uds_send_negative_response(0x2E, 0x27); // securityAccessDenied
    }
    
    // Find DID in table
    for (size_t i = 0; i < DID_TABLE_SIZE; i++) {
        if (g_did_table[i].did == did) {
            const uds_did_entry_t *entry = &g_did_table[i];
            
            // Check if writable
            if (entry->read_only) {
                return uds_send_negative_response(0x2E, 0x27); // securityAccessDenied
            }
            
            // Validate length
            uint16_t data_length = length - 3;  // Subtract SID and DID
            if (data_length != entry->data_length) {
                return uds_send_negative_response(0x2E, 0x13); // incorrectMessageLength
            }
            
            // Write data
            memcpy(entry->data_ptr, &request[3], data_length);
            
            // Send positive response
            uint8_t response[3] = {0x6E, (did >> 8) & 0xFF, did & 0xFF};
            return uds_send_response(response, 3);
        }
    }
    
    // DID not found
    return uds_send_negative_response(0x2E, 0x15); // requestOutOfRange
}
```

---

## 4. Complete UDS Server Implementation

### 4.1 Main Dispatcher

```c
typedef struct {
    uds_session_context_t session;
    uds_security_context_t security;
    uint8_t rx_buffer[4096];
    uint16_t rx_length;
    bool is_processing;
} uds_server_context_t;

static uds_server_context_t g_uds_server;

/**
 * @brief Main UDS request dispatcher
 */
int uds_process_request(const uint8_t *request, uint16_t length) {
    if (length < 1) {
        return -1;
    }
    
    uint8_t sid = request[0];
    
    // Dispatch based on SID
    switch (sid) {
        case 0x10:
            return uds_handle_diagnostic_session_control(
                &g_uds_server.session, 
                request[1]);
                
        case 0x11:
            return uds_handle_ecu_reset(request[1]);
            
        case 0x22:
            return uds_handle_read_data_by_identifier(request, length);
            
        case 0x27:
            if (request[1] % 2 == 1) {
                // Odd = Request Seed
                return uds_handle_security_access_request_seed(
                    &g_uds_server.security, 
                    request[1]);
            } else {
                // Even = Send Key
                uint32_t key = (request[2] << 24) | (request[3] << 16) | 
                              (request[4] << 8) | request[5];
                return uds_handle_security_access_send_key(
                    &g_uds_server.security, 
                    request[1], 
                    key);
            }
            
        case 0x2E:
            return uds_handle_write_data_by_identifier(request, length);
            
        case 0x3E:
            return uds_handle_tester_present(request[1]);
            
        case 0x85:
            return uds_handle_control_dtc_setting(request[1]);
            
        default:
            return uds_send_negative_response(sid, 0x11); // serviceNotSupported
    }
}
```

### 4.2 Response Functions

```c
/**
 * @brief Send positive response
 */
int uds_send_response(const uint8_t *data, uint16_t length) {
    // Pass to CAN TP layer
    return tp_send(&g_tp_session, data, length);
}

/**
 * @brief Send negative response
 */
int uds_send_negative_response(uint8_t request_sid, uint8_t nrc) {
    uint8_t response[3] = {0x7F, request_sid, nrc};
    return uds_send_response(response, 3);
}
```

---

## 5. Testing with Diagnostic Tools

### 5.1 Using Vector CANoe

```
Test Setup:
1. Configure CAN channel
2. Set baudrate to 500 kbps
3. Load UDS configuration
4. Send diagnostic requests

Example CAPL Script:
message CAN txMsg;

on start {
  txMsg.id = 0x7DF;
  txMsg.dlc = 8;
  
  // Read VIN
  txMsg.byte(0) = 0x02;
  txMsg.byte(1) = 0x22;
  txMsg.byte(2) = 0xF1;
  txMsg.byte(3) = 0x90;
  txMsg.byte(4..7) = 0x00;
  
  output(txMsg);
}
```

### 5.2 Using Python (python-can)

```python
import can
import time

# Initialize CAN bus
bus = can.interface.Bus(channel='can0', bustype='socketcan', bitrate=500000)

def send_uds_request(data):
    """Send UDS request via CAN TP"""
    msg = can.Message(arbitration_id=0x7DF, data=data, is_extended_id=False)
    bus.send(msg)
    print(f"Sent: {data.hex()}")
    
    # Wait for response
    response = bus.recv(timeout=2.0)
    if response:
        print(f"Received: {response.data.hex()}")
        return response.data
    return None

# Test: Read VIN
vin_request = [0x02, 0x22, 0xF1, 0x90, 0x00, 0x00, 0x00, 0x00]
response = send_uds_request(vin_request)

# Test: Security Access
seed_request = [0x02, 0x27, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00]
seed_response = send_uds_request(seed_request)

if seed_response and seed_response[0] == 0x67:
    seed = int.from_bytes(seed_response[2:6], 'big')
    print(f"Seed: 0x{seed:08X}")
    
    # Calculate key (simple XOR for demo)
    key = seed ^ 0xBEEFCAFE
    
    key_request = [0x06, 0x27, 0x02] + list(key.to_bytes(4, 'big'))
    key_response = send_uds_request(key_request)
```

---

## 6. Troubleshooting Guide

### Common Issues

| Issue | Symptoms | Solution |
|-------|----------|----------|
| **Service Not Supported** | NRC 0x11 | Verify SID is implemented, check session permissions |
| **Security Access Denied** | NRC 0x27 | Complete security access sequence first |
| **Wrong Sequence** | NRC 0x24 | Ensure proper order (seed before key) |
| **Session Timeout** | No response | Send Tester Present periodically |
| **Incorrect Length** | NRC 0x13 | Verify message length matches specification |
| **Conditions Not Correct** | NRC 0x22 | Check preconditions (ignition state, gear position) |

### Debug Checklist

```c
// Enable UDS debug logging
#define UDS_DEBUG_ENABLE 1

#if UDS_DEBUG_ENABLE
    #define UDS_LOG(fmt, ...) printf("[UDS] " fmt "\n", ##__VA_ARGS__)
#else
    #define UDS_LOG(fmt, ...)
#endif

// Log all requests and responses
int uds_process_request(const uint8_t *request, uint16_t length) {
    UDS_LOG("Request: %.*s", length, format_hex(request, length));
    
    int result = process_internal(request, length);
    
    UDS_LOG("Result: %d", result);
    return result;
}
```

---

## 7. Japanese Technical Vocabulary

| English | Japanese | Romaji | Indonesian |
|---------|----------|--------|------------|
| Diagnostic | 診断 | Shindan | Diagnosa |
| Session | セッション | Sesshon | Sesi |
| Security | セキュリティ | Sekyuriti | Keamanan |
| Identifier | 識別子 | Shikibetsushi | Pengenal |
| Request | リクエスト | Rikuesuto | Permintaan |
| Response | レスポンス | Resuponsu | Tanggapan |
| Reset | リセット | Risetto | Reset |
| Download | ダウンロード | Daunrōdo | Unduh |

---

## 8. Next Steps

### After Mastering UDS:
1. ✅ **Next Module**: [AUTOSAR Classic Architecture](./03_AUTOSAR_Classic_Arch.md)
2. 📚 **Related**: [CAN TP Deep Dive](./01_CAN_TP_DeepDive.md)
3. 🔧 **Practice**: Implement UDS server on your ECU

### Practice Exercises:
1. Implement all major UDS services (0x10, 0x11, 0x22, 0x27, 0x2E, 0x3E)
2. Create a custom seed-key algorithm
3. Add support for DTC reading (0x18 service)
4. Implement flash bootloader with UDS (0x34, 0x36, 0x37)

---

## 9. References

- **ISO 14229-1**: Road vehicles — Unified diagnostic services (UDS) — Part 1: Specification
- **ISO 14229-2**: UDS — Session layer services
- **ISO 14229-3**: UDS — Implementation on CAN
- **SAE J1979**: E/E Diagnostic Test Modes
- **Vector UDS Guide**: Vector Informatik GmbH

---

<div align="center">

**Previous**: [CAN TP Deep Dive](./01_CAN_TP_DeepDive.md) | **Next**: [AUTOSAR Classic Architecture](./03_AUTOSAR_Classic_Arch.md)

[Back to Top](#-uds-protocol-master-iso-14229)

</div>
