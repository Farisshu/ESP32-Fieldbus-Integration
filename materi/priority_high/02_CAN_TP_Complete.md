# 📘 CAN TP (CAN Transport Protocol) - ISO 15765-2

> **Panduan Lengkap**: Implementasi Transport Layer untuk komunikasi multi-frame di CAN Bus  
> **完全ガイド**: CAN バスでのマルチフレーム通信用トランスポート層の実装  
> **Complete Guide**: Transport Layer implementation for multi-frame communication on CAN Bus

---

## 📋 Daftar Isi

1. [Pendahuluan](#1-pendahuluan)
2. [Apa itu CAN TP?](#2-apa-itu-can-tp)
3. [Struktur Frame CAN TP](#3-struktur-frame-can-tp)
4. [Flow Control Mechanism](#4-flow-control-mechanism)
5. [Timing Parameters](#5-timing-parameters)
6. [Implementasi Praktis](#6-implementasi-praktis)
7. [Testing & Validation](#7-testing--validation)
8. [Troubleshooting](#8-troubleshooting)
9. [Referensi](#9-referensi)

---

## 1. Pendahuluan

### 1.1 Latar Belakang

**CAN Transport Protocol (CAN TP)** atau **ISO 15765-2** adalah protokol layer transport yang memungkinkan pengiriman data lebih besar dari 8 bytes melalui CAN Bus. 

**Mengapa CAN TP diperlukan?**
- CAN frame standar hanya mendukung **maksimal 8 bytes** payload
- Banyak pesan diagnostik (UDS) memerlukan **puluhan hingga ratusan bytes**
- CAN TP memecah pesan panjang menjadi beberapa frame

### 1.2 Aplikasi Utama

| Aplikasi | Deskripsi | Contoh Data Size |
|----------|-----------|------------------|
| **UDS Diagnostics** | Diagnostic services (ISO 14229) | 10-4095 bytes |
| **ECU Programming** | Flash download/upload | 1024-65535 bytes |
| **Parameter Transfer** | Calibration data transfer | 100-1000 bytes |
| **Log Data Upload** | Vehicle data logging | 500-4095 bytes |

### 1.3 Prasyarat

| Topik | Status | Reference |
|-------|--------|-----------|
| CAN Bus 2.0A | ✅ Wajib | [CAN Bus Guide](../README_MATERI_LENGKAP.md#3-apa-itu-can-bus) |
| Hexadecimal System | ✅ Wajib | Number conversion |
| C/C++ Programming | ✅ Wajib | Embedded programming |

---

## 2. Apa itu CAN TP?

### 2.1 Definisi

**CAN TP (ISO 15765-2)** adalah protokol layer transport dalam OSI model yang berada di antara:
- **Layer Atas**: Application layer (UDS, OBD-II)
- **Layer Bawah**: Network layer (CAN Bus ISO 11898)

### 2.2 Arsitektur Protocol Stack

```
┌─────────────────────────────────────────┐
│  Application Layer (UDS - ISO 14229)    │
├─────────────────────────────────────────┤
│  Transport Layer (CAN TP - ISO 15765-2) │  ← Fokus materi ini
├─────────────────────────────────────────┤
│  Network Layer (CAN - ISO 11898)        │
├─────────────────────────────────────────┤
│  Physical Layer (ISO 11898-2/5)         │
└─────────────────────────────────────────┘
```

### 2.3 Kapabilitas CAN TP

| Parameter | Value |
|-----------|-------|
| **Max Data Length** | 4095 bytes (standard), 65535 bytes (extended) |
| **Addressing Format** | Normal (11-bit), Extended (29-bit) |
| **Supported Frames** | Single, First, Consecutive, Flow Control |
| **Block Size** | 0-255 frames per block |
| **Separation Time (STmin)** | 0-127 ms |

---

## 3. Struktur Frame CAN TP

### 3.1 Jenis-Jenis Frame

CAN TP mendefinisikan **4 jenis frame**:

| Frame Type | Abbreviation | PCI Range | Deskripsi |
|------------|--------------|-----------|-----------|
| **Single Frame** | SF | 0x00-0x07 | Data ≤ 7 bytes (langsung dikirim) |
| **First Frame** | FF | 0x10-0x1F | Frame pertama dari multi-frame |
| **Consecutive Frame** | CF | 0x20-0x2F | Frame lanjutan (ke-2 dst) |
| **Flow Control** | FC | 0x30-0x3F | Acknowledgement dari receiver |

### 3.2 Single Frame (SF)

Digunakan untuk data **≤ 7 bytes**.

**Format:**
```
┌──────────────┬──────────────────────────────┐
│  PCI (1 byte)│      Data (0-7 bytes)        │
│  0x0X        │      Payload                 │
└──────────────┴──────────────────────────────┘
     │
     └─ X = Data length (1-7)
```

**Contoh:**
```
Data to send: [0x10, 0x03]  (2 bytes - UDS Session Control request)

CAN Frame:
ID: 0x7E0
Data: 02 10 03 00 00 00 00 00
      │  └─────┘
      │    Data
      └─ PCI: 0x02 (Single Frame, 2 bytes)
```

### 3.3 First Frame (FF)

Frame **pertama** dari pesan multi-frame (> 7 bytes).

**Format:**
```
┌──────────────┬──────────────┬──────────────────────────────┐
│  PCI (1 byte)│  Data Len   │       Data (6 bytes)         │
│  0x1X        │  (2 bytes)  │                              │
└──────────────┴──────────────┴──────────────────────────────┘
     │              │
     │              └─ Total message length (0-4095 bytes)
     └─ X = akan diisi dengan high nibble dari data length
```

**Contoh:**
```
Data to send: 20 bytes total
[0x22, 0xF1, 90, ... 17 more bytes ...]

First Frame:
ID: 0x7E0
Data: 10 14 22 F1 90 00 00 00
      │  │  └─────────┘
      │  │    First 6 bytes of data
      │  └─ Total length: 0x0014 = 20 bytes
      └─ PCI: 0x10 (First Frame indicator)
```

### 3.4 Consecutive Frame (CF)

Frame **lanjutan** setelah First Frame.

**Format:**
```
┌──────────────┬──────────────────────────────┐
│  PCI (1 byte)│       Data (7 bytes)         │
│  0x2X        │                              │
└──────────────┴──────────────────────────────┘
     │
     └─ X = Sequence number (1-15, wraps around)
```

**Contoh:**
```
Consecutive Frame #1:
ID: 0x7DF (Response)
Data: 21 01 57 42 31 32 33 34
      │  └─────────────────┘
      │    7 bytes of data
      └─ PCI: 0x21 (Consecutive Frame, seq#1)

Consecutive Frame #2:
ID: 0x7DF
Data: 22 02 35 36 37 38 39 30
      │  └─────────────────┘
      │    7 bytes of data
      └─ PCI: 0x22 (Consecutive Frame, seq#2)
```

### 3.5 Flow Control Frame (FC)

Frame **acknowledgement** dari receiver ke sender.

**Format:**
```
┌──────────────┬──────────────┬──────────────┐
│  PCI (1 byte)│  Block Size  │   STmin      │
│  0x3X        │  (1 byte)    │  (1 byte)    │
└──────────────┴──────────────┴──────────────┘
     │
     └─ X = Flow Status (0=Continue, 1=Wait, 3=Overflow)
```

**Flow Status:**
| Value | Name | Deskripsi |
|-------|------|-----------|
| 0x0 | Continue To Send (CTS) | Sender boleh lanjut kirim |
| 0x1 | Wait (WT) | Sender harus tunggu |
| 0x3 | Overflow | Receiver buffer penuh, abort |

**Block Size (BS):**
- `0x00`: Tidak ada limit (continuous)
- `0x01-0xFF`: Jumlah CF sebelum FC berikutnya diperlukan

**STmin (Separation Time Minimum):**
- `0x00-0x7F`: 0-127 ms
- `0x80-0xF9`: Reserved
- `0xFA-0xFF`: 100-900 μs (microseconds)

**Contoh:**
```
Flow Control - Continue, no limit, 3ms delay:
ID: 0x7DF
Data: 30 00 03 00 00 00 00 00
      │  │  │
      │  │  └─ STmin: 3ms
      │  └─ Block Size: 0 (unlimited)
      └─ PCI: 0x30 (Flow Control, CTS)
```

---

## 4. Flow Control Mechanism

### 4.1 Communication Sequence

**Contoh lengkap transfer 20 bytes:**

```
Sender (Tester)                    Receiver (ECU)
     │                                   │
     │  First Frame (FF)                 │
     │  ─────────────────────────────►   │  10 14 22 F1 90 ...
     │                                   │  (Total: 20 bytes)
     │                                   │
     │  Flow Control (FC)                │
     │  ◄─────────────────────────────   │  30 00 03 ...
     │                                   │  (CTS, BS=0, STmin=3ms)
     │                                   │
     │  Consecutive Frame #1 (CF)        │
     │  ─────────────────────────────►   │  21 01 57 42 31 32 33 34
     │                                   │
     │  Consecutive Frame #2 (CF)        │
     │  ─────────────────────────────►   │  22 02 35 36 37 38 39 30
     │                                   │
     │  Transfer Complete                │
     │                                   │
```

### 4.2 Block Size Handling

Jika **Block Size > 0**, sender harus menunggu FC setelah setiap blok:

```
Sender                            Receiver
  │                                  │
  │  FF                             │
  │  ───────────────────────────►   │
  │                                  │
  │  FC (BS=5, STmin=10ms)          │
  │  ◄───────────────────────────   │
  │                                  │
  │  CF #1                          │
  │  ───────────────────────────►   │
  │  CF #2                          │
  │  ───────────────────────────►   │
  │  CF #3                          │
  │  ───────────────────────────►   │
  │  CF #4                          │
  │  ───────────────────────────►   │
  │  CF #5                          │
  │  ───────────────────────────►   │
  │                                  │
  │  FC (BS=5, STmin=10ms)          │  ← FC berikutnya
  │  ◄───────────────────────────   │
  │                                  │
  │  CF #6                          │
  │  ───────────────────────────►   │
  │  ...                            │
```

### 4.3 State Machine

```
┌─────────────────────────────────────────────────────────────┐
│                    CAN TP Sender State Machine               │
└─────────────────────────────────────────────────────────────┘

        ┌──────────────┐
        │    IDLE      │
        └──────┬───────┘
               │ N_WFTmax exceeded or new request
               ▼
        ┌──────────────┐
        │  WAIT_FC     │ ──────┐
        └──────┬───────┘       │ Timeout (P2)
               │ Receive FC    ▼
         ┌─────┴─────┐   ┌──────────────┐
         │  FC.CTS   │   │   ERROR      │
         └─────┬─────┘   └──────────────┘
               │
         ┌─────┴─────┐
         │  SEND_CF  │ ───► Send consecutive frames
         └─────┬─────┘
               │ All sent or block complete
               ▼
        ┌──────────────┐
        │    IDLE      │
        └──────────────┘
```

---

## 5. Timing Parameters

### 5.1 Timer Definitions

| Timer | Name | Value | Deskripsi |
|-------|------|-------|-----------|
| **P2** | Server Response Time | 50ms (default) | Max time untuk ECU response |
| **P2*** | P2 extended | 5000ms | Extended response time (untuk operasi lama) |
| **S3** | Session Timer | 5000ms | Session timeout jika tidak ada komunikasi |
| **BS** | Block Size | 0-255 | Frames per flow control block |
| **STmin** | Separation Time Min | 0-127ms | Min delay antar consecutive frames |
| **N_WFTmax** | Max Wait Frames | 10 | Max FC.WT sebelum abort |

### 5.2 Timing Diagram

```
Sender                        Receiver
  │                              │
  │  FF @ T0                     │
  │  ─────────────────────────►  │
  │                              │
  │  ◄──────── P2 max ───────►   │  (Receiver must respond within P2)
  │                              │
  │  FC @ T1 (T1 < T0+P2)        │
  │  ◄─────────────────────────  │
  │                              │
  │  CF #1 @ T1+STmin            │
  │  ─────────────────────────►  │
  │                              │
  │  CF #2 @ T1+2×STmin          │
  │  ─────────────────────────►  │
  │                              │
  │  ...                         │
  │                              │
  │  If no FC within P2* → Abort │
```

### 5.3 Timeout Handling

| Scenario | Timeout | Action |
|----------|---------|--------|
| No FC after FF | P2 (50ms) | Retry or abort |
| No CF expected | P2* (5s) | Wait longer |
| Session inactive | S3 (5s) | Return to default session |
| Too many FC.WT | N_WFTmax (10) | Abort transfer |

---

## 6. Implementasi Praktis

### 6.1 Struktur Data

```cpp
// can_tp_types.h
#ifndef CAN_TP_TYPES_H
#define CAN_TP_TYPES_H

#include <stdint.h>
#include <stddef.h>

// Frame types
#define CAN_TP_SF  0x00  // Single Frame
#define CAN_TP_FF  0x10  // First Frame
#define CAN_TP_CF  0x20  // Consecutive Frame
#define CAN_TP_FC  0x30  // Flow Control

// Flow status
#define CAN_TP_FC_CTS   0x00  // Continue To Send
#define CAN_TP_FC_WAIT  0x01  // Wait
#define CAN_TP_FC_OVERFLOW 0x03  // Overflow

// Result codes
typedef enum {
    CAN_TP_OK = 0,
    CAN_TP_ERR_TIMEOUT,
    CAN_TP_ERR_OVERFLOW,
    CAN_TP_ERR_INVALID_PARAM,
    CAN_TP_ERR_BUFFER_FULL,
    CAN_TP_ERR_WRONG_SN,
    CAN_TP_ERR_FS,
    CAN_TP_ERR_LENGTH
} CAN_TPResult;

// Configuration structure
typedef struct {
    uint32_t tx_id;        // Transmit CAN ID
    uint32_t rx_id;        // Receive CAN ID
    uint16_t bs;           // Block Size (0 = unlimited)
    uint8_t stmin;         // Separation Time Minimum (ms)
    uint8_t n_wft_max;     // Max Wait Frames
    uint32_t p2_timeout;   // P2 timer (ms)
    uint32_t p2_star_timeout; // P2* timer (ms)
    uint32_t s3_timeout;   // S3 timer (ms)
} CAN_TPConfig;

// State enumeration
typedef enum {
    CAN_TP_STATE_IDLE,
    CAN_TP_STATE_WAIT_FC,
    CAN_TP_STATE_SENDING_CF,
    CAN_TP_STATE_RECEIVING,
    CAN_TP_STATE_ERROR
} CAN_TPState;

// Context structure for state management
typedef struct {
    CAN_TPConfig config;
    CAN_TPState state;
    
    // TX buffer
    const uint8_t* tx_data;
    size_t tx_total_len;
    size_t tx_sent_len;
    uint8_t tx_sn;  // Sequence number
    
    // RX buffer
    uint8_t* rx_data;
    size_t rx_total_len;
    size_t rx_received_len;
    uint8_t rx_expected_sn;
    
    // Timers
    uint32_t timer_p2;
    uint32_t timer_s3;
    uint8_t wft_count;  // Wait frame counter
    
    // Callbacks
    void (*on_send)(const uint8_t* data, size_t len);
    bool (*on_available)(void);
    size_t (*on_receive)(uint8_t* buffer, size_t max_len);
    
} CAN_TPContext;

#endif // CAN_TP_TYPES_H
```

### 6.2 CAN TP Implementation

```cpp
// can_tp.cpp
#include "can_tp.h"
#include <string.h>
#include <Arduino.h>

class CANTransportProtocol {
private:
    CAN_TPContext ctx;
    
public:
    CANTransportProtocol(CAN_TPConfig config) {
        ctx.config = config;
        ctx.state = CAN_TP_STATE_IDLE;
        ctx.tx_data = nullptr;
        ctx.rx_data = nullptr;
        ctx.on_send = nullptr;
        ctx.on_available = nullptr;
        ctx.on_receive = nullptr;
    }
    
    /**
     * @brief Set callback functions
     */
    void setCallbacks(
        void (*send_func)(const uint8_t*, size_t),
        bool (*avail_func)(void),
        size_t (*recv_func)(uint8_t*, size_t)
    ) {
        ctx.on_send = send_func;
        ctx.on_available = avail_func;
        ctx.on_receive = recv_func;
    }
    
    /**
     * @brief Send data using CAN TP
     */
    CAN_TPResult send(const uint8_t* data, size_t len) {
        if (len == 0 || data == nullptr) {
            return CAN_TP_ERR_INVALID_PARAM;
        }
        
        if (ctx.state != CAN_TP_STATE_IDLE) {
            return CAN_TP_ERR_BUFFER_FULL;
        }
        
        ctx.tx_data = data;
        ctx.tx_total_len = len;
        ctx.tx_sent_len = 0;
        ctx.tx_sn = 1;
        
        if (len <= 7) {
            // Single Frame
            uint8_t sf_buffer[8];
            sf_buffer[0] = len;  // PCI
            memcpy(&sf_buffer[1], data, len);
            
            ctx.on_send(sf_buffer, 8);
            ctx.state = CAN_TP_STATE_IDLE;
            return CAN_TP_OK;
            
        } else if (len <= 4095) {
            // First Frame
            uint8_t ff_buffer[8];
            ff_buffer[0] = 0x10 | ((len >> 8) & 0x0F);  // PCI with high nibble of length
            ff_buffer[1] = len & 0xFF;  // Low byte of length
            memcpy(&ff_buffer[2], data, 6);  // First 6 bytes of data
            
            ctx.on_send(ff_buffer, 8);
            ctx.tx_sent_len = 6;
            ctx.state = CAN_TP_STATE_WAIT_FC;
            ctx.timer_p2 = millis();
            
            return CAN_TP_OK;
        } else {
            return CAN_TP_ERR_LENGTH;
        }
    }
    
    /**
     * @brief Process received CAN frames
     */
    CAN_TPResult process(void) {
        if (!ctx.on_available || !ctx.on_available()) {
            return CAN_TP_OK;  // No data
        }
        
        uint8_t rx_buffer[8];
        size_t len = ctx.on_receive(rx_buffer, 8);
        
        if (len != 8) {
            return CAN_TP_ERR_INVALID_PARAM;
        }
        
        uint8_t pci = rx_buffer[0] & 0xF0;
        
        switch (pci) {
            case CAN_TP_SF:
                return handleSingleFrame(rx_buffer, len);
                
            case CAN_TP_FF:
                return handleFirstFrame(rx_buffer, len);
                
            case CAN_TP_CF:
                return handleConsecutiveFrame(rx_buffer, len);
                
            case CAN_TP_FC:
                return handleFlowControl(rx_buffer, len);
                
            default:
                return CAN_TP_ERR_INVALID_PARAM;
        }
    }
    
    /**
     * @brief Check if receive is complete
     */
    bool isReceiveComplete(void) {
        return (ctx.state == CAN_TP_STATE_IDLE && 
                ctx.rx_received_len == ctx.rx_total_len &&
                ctx.rx_total_len > 0);
    }
    
    /**
     * @brief Get received data
     */
    size_t getReceivedData(uint8_t* buffer, size_t max_len) {
        if (!isReceiveComplete()) {
            return 0;
        }
        
        size_t copy_len = min(ctx.rx_total_len, max_len);
        memcpy(buffer, ctx.rx_data, copy_len);
        
        // Reset state
        ctx.rx_total_len = 0;
        ctx.rx_received_len = 0;
        ctx.state = CAN_TP_STATE_IDLE;
        
        return copy_len;
    }
    
private:
    CAN_TPResult handleSingleFrame(uint8_t* frame, size_t len) {
        uint8_t data_len = frame[0] & 0x0F;
        
        if (data_len > 7 || data_len > len - 1) {
            return CAN_TP_ERR_LENGTH;
        }
        
        if (ctx.on_receive) {
            // Directly pass to application (UDS layer)
            ctx.on_receive(&frame[1], data_len);
        }
        
        return CAN_TP_OK;
    }
    
    CAN_TPResult handleFirstFrame(uint8_t* frame, size_t len) {
        if (len != 8) {
            return CAN_TP_ERR_LENGTH;
        }
        
        // Extract total length
        uint16_t total_len = ((frame[0] & 0x0F) << 8) | frame[1];
        
        if (total_len <= 7 || total_len > 4095) {
            return CAN_TP_ERR_LENGTH;
        }
        
        // Allocate buffer (in real implementation, use pre-allocated buffer)
        ctx.rx_data = new uint8_t[total_len];
        ctx.rx_total_len = total_len;
        ctx.rx_received_len = 6;  // 6 bytes in FF
        ctx.rx_expected_sn = 1;
        
        // Copy first 6 bytes
        memcpy(ctx.rx_data, &frame[2], 6);
        
        // Send Flow Control
        uint8_t fc_buffer[8] = {0};
        fc_buffer[0] = 0x30;  // CTS
        fc_buffer[1] = ctx.config.bs;  // Block Size
        fc_buffer[2] = ctx.config.stmin;  // STmin
        
        ctx.on_send(fc_buffer, 8);
        
        ctx.state = CAN_TP_STATE_RECEIVING;
        ctx.timer_p2 = millis();
        
        return CAN_TP_OK;
    }
    
    CAN_TPResult handleConsecutiveFrame(uint8_t* frame, size_t len) {
        if (ctx.state != CAN_TP_STATE_RECEIVING) {
            return CAN_TP_ERR_WRONG_SN;
        }
        
        uint8_t sn = frame[0] & 0x0F;
        
        if (sn != ctx.rx_expected_sn) {
            return CAN_TP_ERR_WRONG_SN;
        }
        
        // Calculate how many bytes to copy
        size_t remaining = ctx.rx_total_len - ctx.rx_received_len;
        size_t to_copy = min((size_t)7, remaining);
        
        // Copy data
        memcpy(&ctx.rx_data[ctx.rx_received_len], &frame[1], to_copy);
        ctx.rx_received_len += to_copy;
        
        // Update sequence number (wrap around 0-F)
        ctx.rx_expected_sn = (sn + 1) & 0x0F;
        
        // Check if complete
        if (ctx.rx_received_len >= ctx.rx_total_len) {
            ctx.state = CAN_TP_STATE_IDLE;
            return CAN_TP_OK;
        }
        
        // Reset P2 timer
        ctx.timer_p2 = millis();
        
        return CAN_TP_OK;
    }
    
    CAN_TPResult handleFlowControl(uint8_t* frame, size_t len) {
        if (ctx.state != CAN_TP_STATE_WAIT_FC && 
            ctx.state != CAN_TP_STATE_SENDING_CF) {
            return CAN_TP_ERR_FS;
        }
        
        uint8_t fs = frame[0] & 0x0F;
        uint8_t bs = frame[1];
        uint8_t stmin = frame[2];
        
        switch (fs) {
            case CAN_TP_FC_CTS:
                ctx.wft_count = 0;
                
                // Send consecutive frames
                while (ctx.tx_sent_len < ctx.tx_total_len) {
                    uint8_t cf_buffer[8];
                    cf_buffer[0] = 0x20 | ctx.tx_sn;
                    
                    size_t remaining = ctx.tx_total_len - ctx.tx_sent_len;
                    size_t to_send = min((size_t)7, remaining);
                    
                    memcpy(&cf_buffer[1], &ctx.tx_data[ctx.tx_sent_len], to_send);
                    ctx.on_send(cf_buffer, 8);
                    
                    ctx.tx_sent_len += to_send;
                    ctx.tx_sn = (ctx.tx_sn + 1) & 0x0F;
                    
                    // Check block size limit
                    if (bs > 0 && (ctx.tx_sn % bs) == 0) {
                        ctx.state = CAN_TP_STATE_WAIT_FC;
                        ctx.timer_p2 = millis();
                        break;
                    }
                    
                    // STmin delay
                    if (stmin > 0 && ctx.tx_sent_len < ctx.tx_total_len) {
                        delay(stmin);
                    }
                }
                
                if (ctx.tx_sent_len >= ctx.tx_total_len) {
                    ctx.state = CAN_TP_STATE_IDLE;
                }
                
                return CAN_TP_OK;
                
            case CAN_TP_FC_WAIT:
                ctx.wft_count++;
                if (ctx.wft_count >= ctx.config.n_wft_max) {
                    ctx.state = CAN_TP_STATE_ERROR;
                    return CAN_TP_ERR_TIMEOUT;
                }
                
                // Reset P2 timer and wait
                ctx.timer_p2 = millis();
                return CAN_TP_OK;
                
            case CAN_TP_FC_OVERFLOW:
                ctx.state = CAN_TP_STATE_ERROR;
                return CAN_TP_ERR_OVERFLOW;
                
            default:
                return CAN_TP_ERR_FS;
        }
    }
};
```

### 6.3 Usage Example

```cpp
// main.cpp
#include "can_tp.h"
#include <mcp2515_can.h>

MCP2515 can;
CANTransportProtocol* can_tp;

// Callback: Send CAN frame
void can_send_callback(const uint8_t* data, size_t len) {
    can_frame frame;
    frame.can_id = 0x7E0;
    frame.can_dlc = len;
    memcpy(frame.data, data, len);
    can.sendMsgBuf(&frame);
}

// Callback: Check if CAN frame available
bool can_available_callback(void) {
    return can.checkReceive();
}

// Callback: Receive CAN frame
size_t can_receive_callback(uint8_t* buffer, size_t max_len) {
    can_frame frame;
    if (can.receiveMsgBuf(&frame) == CAN_OK) {
        size_t len = min((size_t)frame.can_dlc, max_len);
        memcpy(buffer, frame.data, len);
        return len;
    }
    return 0;
}

void setup() {
    Serial.begin(115200);
    
    // Initialize CAN
    if (can.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN initialized");
    } else {
        Serial.println("CAN initialization failed");
        while(1);
    }
    
    // Configure CAN TP
    CAN_TPConfig config;
    config.tx_id = 0x7E0;
    config.rx_id = 0x7DF;
    config.bs = 0;  // Unlimited
    config.stmin = 3;  // 3ms
    config.n_wft_max = 10;
    config.p2_timeout = 50;
    config.p2_star_timeout = 5000;
    config.s3_timeout = 5000;
    
    can_tp = new CANTransportProtocol(config);
    can_tp->setCallbacks(can_send_callback, can_available_callback, can_receive_callback);
    
    Serial.println("CAN TP initialized");
}

void loop() {
    // Process CAN TP
    can_tp->process();
    
    // Example: Send UDS request (20 bytes)
    static bool sent = false;
    if (!sent) {
        uint8_t uds_request[20] = {
            0x22, 0xF1, 90,  // Read VIN
            // ... 17 more bytes ...
        };
        
        CAN_TPResult result = can_tp->send(uds_request, 20);
        if (result == CAN_TP_OK) {
            Serial.println("UDS request sent via CAN TP");
            sent = true;
        } else {
            Serial.printf("Send failed: %d\n", result);
        }
    }
    
    // Check if response received
    if (can_tp->isReceiveComplete()) {
        uint8_t response[256];
        size_t len = can_tp->getReceivedData(response, sizeof(response));
        
        Serial.printf("Received %d bytes:\n", len);
        for (size_t i = 0; i < len; i++) {
            Serial.printf("%02X ", response[i]);
        }
        Serial.println();
    }
    
    delay(10);
}
```

---

## 7. Testing & Validation

### 7.1 Test Cases

| Test Case | Description | Expected Result |
|-----------|-------------|-----------------|
| **TC-01** | Single Frame (≤7 bytes) | Direct transmission, no FC |
| **TC-02** | First Frame + CF (20 bytes) | FF → FC → CF×2 |
| **TC-03** | Block Size handling (BS=5) | FC after every 5 CF |
| **TC-04** | Wait Frame (FC.WT) | Sender waits, continues after CTS |
| **TC-05** | Overflow (FC.OVFLW) | Transfer aborted |
| **TC-06** | Timeout (no FC) | Error after P2 timeout |
| **TC-07** | Wrong Sequence Number | Error, discard frame |
| **TC-08** | Maximum length (4095 bytes) | Complete transfer |

### 7.2 Python Test Script

```python
#!/usr/bin/env python3
"""
CAN TP Tester
Script untuk testing CAN Transport Protocol implementation
"""

import can
import time

class CANTPTester:
    def __init__(self, channel='can0', bitrate=500000):
        self.bus = can.interface.Bus(channel=channel, bitrate=bitrate)
        
    def send_single_frame(self, data):
        """Test Single Frame transmission"""
        if len(data) > 7:
            raise ValueError("Single Frame max 7 bytes")
        
        frame_data = [len(data)] + list(data)
        frame_data += [0] * (8 - len(frame_data))
        
        msg = can.Message(arbitration_id=0x7E0, data=frame_data)
        self.bus.send(msg)
        print(f"[SF] Sent: {frame_data}")
        
    def send_multi_frame(self, data):
        """Test Multi-Frame transmission"""
        if len(data) <= 7 or len(data) > 4095:
            raise ValueError("Multi-Frame requires 8-4095 bytes")
        
        # First Frame
        ff_pci = 0x10 | ((len(data) >> 8) & 0x0F)
        ff_data = [ff_pci, len(data) & 0xFF] + list(data[:6])
        ff_data += [0] * (8 - len(ff_data))
        
        msg = can.Message(arbitration_id=0x7E0, data=ff_data)
        self.bus.send(msg)
        print(f"[FF] Sent: {ff_data}")
        
        # Wait for Flow Control
        start_time = time.time()
        while time.time() - start_time < 0.1:
            msg = self.bus.recv(timeout=0.05)
            if msg and (msg.data[0] & 0xF0) == 0x30:
                print(f"[FC] Received: {list(msg.data)}")
                
                fs = msg.data[0] & 0x0F
                bs = msg.data[1]
                stmin = msg.data[2]
                
                if fs == 0:  # CTS
                    # Send Consecutive Frames
                    sn = 1
                    offset = 6
                    while offset < len(data):
                        cf_pci = 0x20 | sn
                        chunk = data[offset:offset+7]
                        cf_data = [cf_pci] + list(chunk)
                        cf_data += [0] * (8 - len(cf_data))
                        
                        msg = can.Message(arbitration_id=0x7E0, data=cf_data)
                        self.bus.send(msg)
                        print(f"[CF#{sn}] Sent: {cf_data}")
                        
                        offset += 7
                        sn = (sn + 1) & 0x0F
                        
                        if stmin > 0:
                            time.sleep(stmin / 1000.0)
                    
                    print("[COMPLETE] Multi-frame transfer done")
                    return True
                    
                elif fs == 1:  # WAIT
                    print("[WAIT] Waiting...")
                    time.sleep(0.1)
                    
                elif fs == 3:  # OVERFLOW
                    print("[ERROR] Overflow!")
                    return False
        
        print("[TIMEOUT] No Flow Control received")
        return False
    
    def run_tests(self):
        """Run all test cases"""
        print("=" * 50)
        print("CAN TP Test Suite")
        print("=" * 50)
        
        # Test 1: Single Frame
        print("\n[Test 1] Single Frame")
        self.send_single_frame([0x10, 0x03])
        time.sleep(0.5)
        
        # Test 2: Multi-Frame (20 bytes)
        print("\n[Test 2] Multi-Frame (20 bytes)")
        data = [0x22, 0xF1, 0x90] + [0x00] * 17
        self.send_multi_frame(data)
        time.sleep(0.5)
        
        # Test 3: Multi-Frame (50 bytes)
        print("\n[Test 3] Multi-Frame (50 bytes)")
        data = [0x22, 0xF1, 0x90] + [i for i in range(47)]
        self.send_multi_frame(data)
        
        print("\n" + "=" * 50)
        print("Test Suite Complete")
        print("=" * 50)

if __name__ == '__main__':
    tester = CANTPTester()
    tester.run_tests()
```

---

## 8. Troubleshooting

### 8.1 Common Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No Flow Control response | Receiver not implemented, wrong CAN ID | Verify addressing, check receiver code |
| Wrong Sequence Number | Frame loss, timing issue | Check bus quality, adjust STmin |
| Timeout during transfer | ECU busy, P2 too short | Increase P2 timeout, check ECU status |
| Buffer overflow | RX buffer too small | Increase buffer size, implement flow control |
| Incomplete reception | Missed CF, early termination | Check state machine, verify SN tracking |

### 8.2 Debugging Tips

1. **Use CAN Logger**
   - Capture all frames with timestamps
   - Verify PCI values and sequence numbers
   - Check timing between frames

2. **Monitor State Machine**
   - Log state transitions
   - Track timer expirations
   - Verify FC handling

3. **Check Bus Quality**
   - Monitor error counters
   - Verify termination resistors
   - Check signal integrity with oscilloscope

---

## 9. Referensi

### 9.1 Standards

| Standard | Title | Source |
|----------|-------|--------|
| ISO 15765-2 | Road vehicles - Diagnostic communication over CAN - Part 2 | ISO Store |
| ISO 14229-1 | UDS on CAN | ISO Store |

### 9.2 Tools

| Tool | Type | Link |
|------|------|------|
| PCAN-View | CAN Viewer | [PEAK-System](https://peak-system.com) |
| SavvyCAN | Open Source CAN Tool | [GitHub](https://github.com/rivieragc/savvycan) |
| CANalyzer | Professional | [Vector](https://vector.com) |

### 9.3 Japanese Terminology

| English | 日本語 | Romaji |
|---------|--------|--------|
| Transport Protocol | トランスポートプロトコル | toransupōto purotokoru |
| Flow Control | フロー制御 | furō seigyo |
| Sequence Number | シーケンス番号 | shīkensu bangō |
| Timeout | タイムアウト | taimuauto |
| Buffer | バッファ | baffa |

---

## 📝 Next Steps

Setelah menguasai CAN TP, lanjutkan ke:

1. **[UDS Implementation](./01_UDS_Complete.md)** - Application layer menggunakan CAN TP
2. **[Bootloader Development](./03_Bootloader.md)** - Firmware update dengan CAN TP
3. **[AUTOSAR TP](./02_AUTOSAR_Classic.md)** - CAN TP dalam AUTOSAR stack

---

**Document Version:** 1.0  
**Last Updated:** 2025  
**Author:** Embedded Systems Team  
**License:** MIT
