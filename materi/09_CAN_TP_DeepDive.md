# CAN Transport Protocol (ISO 15765-2) - Deep Dive

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan bab ini, Anda akan mampu:
- Memahami konsep CAN Transport Protocol (CAN TP) sesuai ISO 15765-2
- Mengimplementasikan Single Frame, First Frame, Consecutive Frame, dan Flow Control
- Mengelola buffer dan reassembly pesan UDS yang panjang
- Menangani timing dan flow control dalam komunikasi multi-frame

## 📋 Daftar Isi

1. [Pengantar CAN TP](#1-pengantar-can-tp)
2. [Frame Types dan Struktur](#2-frame-types-dan-struktur)
3. [Protocol Data Unit (PDU)](#3-protocol-data-unit-pdu)
4. [Flow Control Mechanism](#4-flow-control-mechanism)
5. [Implementasi CAN TP Layer](#5-implementasi-can-tp-layer)
6. [Timing Parameters](#6-timing-parameters)
7. [Buffer Management](#7-buffer-management)
8. [Error Handling](#8-error-handling)
9. [Studi Kasus](#9-studi-kasus)
10. [Latihan](#10-latihan)

---

## 1. Pengantar CAN TP

### Mengapa CAN TP Diperlukan?

CAN bus standar memiliki limitasi **8 byte per frame**. Namun, pesan diagnostik (UDS) sering kali lebih panjang dari 8 byte. CAN TP mengatasi limitasi ini dengan:

- **Segmentasi**: Membagi pesan panjang menjadi beberapa frame
- **Reassembly**: Menyusun kembali frame-frame menjadi pesan utuh
- **Flow Control**: Mengatur laju transmisi untuk mencegah overflow

### ISO 15765-2 Standard

```
Layer Stack:
┌─────────────────────┐
│   Application       │  ← UDS (ISO 14229)
├─────────────────────┤
│   Network Layer     │  ← CAN TP (ISO 15765-2)
├─────────────────────┤
│   Data Link Layer   │  ← CAN 2.0 (ISO 11898)
├─────────────────────┤
│   Physical Layer    │  ← CAN Transceiver
└─────────────────────┘
```

### Addressing Formats

**Normal Addressing:**
```
CAN ID = Source Address + Target Address + Protocol Info
```

**Extended Addressing:**
```
First byte = Extended Address
Remaining bytes = TP Data
```

---

## 2. Frame Types dan Struktur

### 2.1 Single Frame (SF)

Untuk pesan ≤ 7 byte.

```
Byte 0: PCI (Protocol Control Information)
Byte 1-7: Data

PCI Format:
Bits 7-4: Frame Type (0 = Single Frame)
Bits 3-0: Data Length (0-7)
```

**Contoh:**
```c
// UDS Request: Read Data by Identifier (0x22 F1 90)
// Panjang: 4 bytes (termasuk SID)
TX: [0x04] [0x22] [0xF1] [0x90] [0x00] [0x00] [0x00] [0x00]
     ↑      ↑      ↑      ↑
     |      |      |      └─ Data byte 3
     |      |      └─ Data byte 2
     |      └─ Service ID (Read Data by Identifier)
     └─ PCI: Type=0 (SF), Length=4
```

### 2.2 First Frame (FF)

Frame pertama dari pesan multi-frame (> 7 bytes).

```
Byte 0-1: PCI (16 bits)
Byte 2-7: Data (6 bytes pertama)

PCI Format:
Bits 15-12: Frame Type (1 = First Frame)
Bits 11-0:  Total Message Length (0-4095 bytes)
```

**Contoh:**
```c
// UDS Response dengan 20 bytes data
// FF: Panjang total = 20 bytes, 6 bytes data pertama
TX: [0x10 0x14] [Data0] [Data1] [Data2] [Data3] [Data4] [Data5]
       ↑            ↑      ↑      ↑      ↑      ↑      ↑
       |            |      |      |      |      |      └─ Data byte 5
       |            |      |      |      |      └─ Data byte 4
       |            |      |      |      └─ Data byte 3
       |            |      |      └─ Data byte 2
       |            |      └─ Data byte 1
       |            └─ Data byte 0
       └─ PCI: Type=1 (FF), Length=20 (0x014)
```

### 2.3 Consecutive Frame (CF)

Frame lanjutan setelah First Frame.

```
Byte 0: PCI
Byte 1-7: Data

PCI Format:
Bits 7-4: Frame Type (2 = Consecutive Frame)
Bits 3-0: Sequence Number (1-15, wrap around)
```

**Contoh:**
```c
// CF #1: Sequence number = 1, 7 bytes data
TX: [0x21] [Data6] [Data7] [Data8] [Data9] [Data10] [Data11] [Data12]
       ↑      ↑      ↑      ↑      ↑       ↑       ↑
       |      |      |      |      |       |       └─ Data byte 12
       |      |      |      |      |       └─ Data byte 11
       |      |      |      |      └─ Data byte 10
       |      |      |      └─ Data byte 9
       |      |      └─ Data byte 8
       |      └─ Data byte 7
       └─ PCI: Type=2 (CF), Sequence=1

// CF #2: Sequence number = 2
TX: [0x22] [Data13] [Data14] [Data15] [Data16] [Data17] [Data18] [Data19]
```

### 2.4 Flow Control Frame (FC)

Mengatur laju transmisi dari pengirim.

```
Byte 0: PCI
Byte 1: Flow Status (FS)
Byte 2: Block Size (BS)
Byte 3: Separation Time (STmin)

PCI Format:
Bits 7-4: Frame Type (3 = Flow Control)
Bits 3-0: Reserved
```

**Flow Status Values:**
- `0x00` = Continue to Send (CTS)
- `0x01` = Wait (WFT)
- `0x02` = Overflow (OVFLW)

**Contoh:**
```c
// FC: Continue to Send, Block Size = 0 (unlimited), STmin = 0ms
TX: [0x30] [0x00] [0x00] [0x00] [0x00] [0x00] [0x00] [0x00]
       ↑      ↑      ↑      ↑
       |      |      |      └─ Padding
       |      |      └─ STmin = 0ms
       |      └─ BS = 0 (no limit)
       └─ PCI: Type=3 (FC), FS=0 (CTS)

// FC: Wait, penerima belum siap
TX: [0x30] [0x01] [0x00] [0x00] [0x00] [0x00] [0x00] [0x00]
                    ↑
                    └─ FS = 1 (Wait)
```

---

## 3. Protocol Data Unit (PDU)

### N-PDU (Network Protocol Data Unit)

Struktur lengkap CAN TP message:

```c
typedef struct {
    uint8_t pci;              // Protocol Control Information
    uint8_t data[7];          // Payload (max 7 bytes per frame)
} CanTpSingleFrameType;

typedef struct {
    uint16_t ff_pci_length;   // First Frame: 12-bit length
    uint8_t data[6];          // First 6 bytes of data
} CanTpFirstFrameType;

typedef struct {
    uint8_t cf_pci_sequence;  // CF: 4-bit sequence number
    uint8_t data[7];          // Next 7 bytes of data
} CanTpConsecutiveFrameType;

typedef struct {
    uint8_t fc_pci;           // FC: Flow Control type
    uint8_t fs;               // Flow Status
    uint8_t bs;               // Block Size
    uint8_t stmin;            // Separation Time Minimum
} CanTpFlowControlType;
```

### PDU Assembly Example

```c
// Contoh assembly 20-byte UDS response
uint8_t uds_response[20] = {
    0x62, 0xF1, 0x90,                    // Positive response + DID
    'V', 'W', '1', 'Z', 'Z', 'Z',        // VIN characters
    'A', 'B', 'C', 'D', 'E', 'F',        // More VIN
    '1', '2', '3', '4', '5', '6'         // Final VIN chars
};

// Frame 1: First Frame
CanFrameType ff_frame;
ff_frame.id = 0x7E8;                     // Response ID
ff_frame.dlc = 8;
ff_frame.data[0] = 0x10;                 // FF PCI type
ff_frame.data[1] = 0x14;                 // Length = 20 bytes
memcpy(&ff_frame.data[2], &uds_response[0], 6);

// Frame 2: Consecutive Frame #1
CanFrameType cf1_frame;
cf1_frame.id = 0x7E8;
cf1_frame.dlc = 8;
cf1_frame.data[0] = 0x21;                // CF PCI, sequence = 1
memcpy(&cf1_frame.data[1], &uds_response[6], 7);

// Frame 3: Consecutive Frame #2
CanFrameType cf2_frame;
cf2_frame.id = 0x7E8;
cf2_frame.dlc = 8;
cf2_frame.data[0] = 0x22;                // CF PCI, sequence = 2
memcpy(&cf2_frame.data[1], &uds_response[13], 7);
```

---

## 4. Flow Control Mechanism

### Flow Control Parameters

| Parameter | Deskripsi | Range |
|-----------|-----------|-------|
| **FS** (Flow Status) | Status aliran data | 0=CTS, 1=WFT, 2=OVFLW |
| **BS** (Block Size) | Jumlah CF sebelum FC berikutnya | 0-255 (0=unlimited) |
| **STmin** (Separation Time) | Delay minimum antar CF | 0-127 ms, 0xF0-0xF9 = μs |

### Flow Control Timing Diagram

```
Transmitter                          Receiver
    |                                   |
    |--- First Frame ------------------>|
    |                                   | Process FF
    |                                   | Send FC
    |<-- Flow Control (CTS) ------------|
    |                                   |
    |--- Consecutive Frame #1 --------->|
    |--- Consecutive Frame #2 --------->|
    |--- ...                           |
    |--- Consecutive Frame #BS ------->|  (jika BS > 0)
    |                                   |
    |                                   | Send FC untuk block berikutnya
    |<-- Flow Control (CTS) ------------|
    |                                   |
    |--- Remaining CFs ---------------->|
    |                                   |
```

### Implementasi Flow Control

```c
typedef enum {
    FC_CONTINUE = 0,
    FC_WAIT = 1,
    FC_OVERFLOW = 2
} FlowControlStatusType;

typedef struct {
    FlowControlStatusType fs;
    uint8_t block_size;
    uint8_t stmin_ms;
    uint8_t frames_remaining;
    uint32_t last_frame_time;
} FlowControlConfigType;

Std_ReturnType CanTp_SendFlowControl(FlowControlConfigType *fc_config) {
    CanFrameType fc_frame;
    
    fc_frame.id = 0x7E8;  // Target address
    fc_frame.dlc = 8;
    fc_frame.data[0] = 0x30;  // FC PCI type
    fc_frame.data[1] = fc_config->fs;
    fc_frame.data[2] = fc_config->block_size;
    fc_frame.data[3] = fc_config->stmin_ms;
    
    // Padding dengan 0x00
    for(uint8_t i = 4; i < 8; i++) {
        fc_frame.data[i] = 0x00;
    }
    
    return Can_Transmit(&fc_frame);
}

// Handler untuk menerima FC
void CanTp_HandleFlowControl(CanFrameType *rx_frame) {
    FlowControlStatusType fs = (FlowControlStatusType)rx_frame->data[1];
    uint8_t bs = rx_frame->data[2];
    uint8_t stmin = rx_frame->data[3];
    
    switch(fs) {
        case FC_CONTINUE:
            // Lanjutkan pengiriman CF
            g_can_tp_state = CANTP_STATE_SEND_CF;
            g_can_tp_bs_counter = bs;
            g_can_tp_stmin_delay = stmin;
            break;
            
        case FC_WAIT:
            // Tunggu FC berikutnya
            g_can_tp_state = CANTP_STATE_WAIT_FC;
            StartTimer(P2_CAN_TIMER, P2_CAN_MAX_MS);
            break;
            
        case FC_OVERFLOW:
            // Receiver overflow, abort transmission
            g_can_tp_state = CANTP_STATE_IDLE;
            CanTp_AbortTransmission();
            break;
    }
}
```

---

## 5. Implementasi CAN TP Layer

### State Machine

```c
typedef enum {
    CANTP_STATE_IDLE = 0,
    CANTP_STATE_WAIT_FF,
    CANTP_STATE_WAIT_CF,
    CANTP_STATE_SEND_SF,
    CANTP_STATE_SEND_FF,
    CANTP_STATE_SEND_CF,
    CANTP_STATE_WAIT_FC,
    CANTP_STATE_RECEIVE_COMPLETE,
    CANTP_STATE_ERROR
} CanTpStateType;

// Global state
static CanTpStateType g_can_tp_state = CANTP_STATE_IDLE;
static uint8_t g_can_tp_rx_buffer[4096];  // Max ISO-TP message size
static uint16_t g_can_tp_rx_length = 0;
static uint16_t g_can_tp_rx_bytes_received = 0;
static uint8_t g_can_tp_next_sequence_number = 0;
```

### Receive Function Implementation

```c
Std_ReturnType CanTp_Receive(CanFrameType *rx_frame) {
    uint8_t pci_type = (rx_frame->data[0] >> 4) & 0x0F;
    
    switch(pci_type) {
        case 0:  // Single Frame
            return CanTp_ReceiveSingleFrame(rx_frame);
            
        case 1:  // First Frame
            return CanTp_ReceiveFirstFrame(rx_frame);
            
        case 2:  // Consecutive Frame
            return CanTp_ReceiveConsecutiveFrame(rx_frame);
            
        case 3:  // Flow Control
            CanTp_HandleFlowControl(rx_frame);
            return E_OK;
            
        default:
            return E_NOT_OK;
    }
}

Std_ReturnType CanTp_ReceiveSingleFrame(CanFrameType *rx_frame) {
    uint8_t length = rx_frame->data[0] & 0x0F;
    
    if(length > 7) {
        return E_NOT_OK;  // Invalid SF length
    }
    
    // Copy data
    g_can_tp_rx_length = length;
    memcpy(g_can_tp_rx_buffer, &rx_frame->data[1], length);
    
    // Notify upper layer (UDS)
    CanTp_RxIndication(E_OK, g_can_tp_rx_length);
    
    g_can_tp_state = CANTP_STATE_IDLE;
    return E_OK;
}

Std_ReturnType CanTp_ReceiveFirstFrame(CanFrameType *rx_frame) {
    // Extract length dari 12-bit field
    uint16_t total_length = ((rx_frame->data[0] & 0x0F) << 8) | rx_frame->data[1];
    
    // Check buffer capacity
    if(total_length > sizeof(g_can_tp_rx_buffer)) {
        // Send FC with OVFLW
        FlowControlConfigType fc = {FC_OVERFLOW, 0, 0, 0};
        CanTp_SendFlowControl(&fc);
        return E_NOT_OK;
    }
    
    g_can_tp_rx_length = total_length;
    g_can_tp_rx_bytes_received = 6;  // 6 bytes data di FF
    g_can_tp_next_sequence_number = 1;
    
    // Copy first 6 bytes
    memcpy(g_can_tp_rx_buffer, &rx_frame->data[2], 6);
    
    // Send FC (CTS)
    FlowControlConfigType fc = {
        .fs = FC_CONTINUE,
        .block_size = 0,      // Unlimited
        .stmin_ms = 0         // No delay
    };
    CanTp_SendFlowControl(&fc);
    
    g_can_tp_state = CANTP_STATE_WAIT_CF;
    StartTimer(P2_CAN_TIMER, P2_CAN_MAX_MS);
    
    return E_OK;
}

Std_ReturnType CanTp_ReceiveConsecutiveFrame(CanFrameType *rx_frame) {
    uint8_t sequence_number = rx_frame->data[0] & 0x0F;
    
    // Check sequence number
    if(sequence_number != g_can_tp_next_sequence_number) {
        // Wrong sequence, abort
        g_can_tp_state = CANTP_STATE_ERROR;
        return E_NOT_OK;
    }
    
    // Calculate bytes to copy
    uint16_t remaining = g_can_tp_rx_length - g_can_tp_rx_bytes_received;
    uint8_t bytes_to_copy = (remaining > 7) ? 7 : remaining;
    
    // Copy data
    memcpy(&g_can_tp_rx_buffer[g_can_tp_rx_bytes_received],
           &rx_frame->data[1],
           bytes_to_copy);
    
    g_can_tp_rx_bytes_received += bytes_to_copy;
    g_can_tp_next_sequence_number++;
    if(g_can_tp_next_sequence_number > 15) {
        g_can_tp_next_sequence_number = 0;  // Wrap around
    }
    
    // Check if receive complete
    if(g_can_tp_rx_bytes_received >= g_can_tp_rx_length) {
        StopTimer(P2_CAN_TIMER);
        CanTp_RxIndication(E_OK, g_can_tp_rx_length);
        g_can_tp_state = CANTP_STATE_IDLE;
    } else {
        // Restart timer for next CF
        StartTimer(P2_CAN_TIMER, P2_CAN_MAX_MS);
    }
    
    return E_OK;
}
```

### Transmit Function Implementation

```c
typedef struct {
    uint8_t *data;
    uint16_t length;
    uint16_t bytes_sent;
    uint8_t next_sequence;
    uint8_t block_size;
    uint8_t stmin;
    uint32_t last_send_time;
} CanTpTxContextType;

static CanTpTxContextType g_tx_context;

Std_ReturnType CanTp_Transmit(const uint8_t *data, uint16_t length) {
    if(length == 0 || length > 4095) {
        return E_NOT_OK;
    }
    
    g_tx_context.data = (uint8_t*)data;
    g_tx_context.length = length;
    g_tx_context.bytes_sent = 0;
    
    if(length <= 7) {
        // Single Frame
        return CanTp_SendSingleFrame(data, length);
    } else {
        // Multi-frame: Send First Frame
        return CanTp_SendFirstFrame(data, length);
    }
}

Std_ReturnType CanTp_SendSingleFrame(const uint8_t *data, uint16_t length) {
    CanFrameType sf_frame;
    
    sf_frame.id = 0x7DF;  // Broadcast atau target address
    sf_frame.dlc = 8;
    sf_frame.data[0] = length & 0x0F;  // SF PCI
    
    memcpy(&sf_frame.data[1], data, length);
    
    // Padding dengan 0x00
    for(uint8_t i = length + 1; i < 8; i++) {
        sf_frame.data[i] = 0x00;
    }
    
    Std_ReturnType result = Can_Transmit(&sf_frame);
    if(result == E_OK) {
        CanTp_TxConfirmation(E_OK);
    }
    
    return result;
}

Std_ReturnType CanTp_SendFirstFrame(const uint8_t *data, uint16_t length) {
    CanFrameType ff_frame;
    
    ff_frame.id = 0x7DF;
    ff_frame.dlc = 8;
    ff_frame.data[0] = 0x10 | ((length >> 8) & 0x0F);  // FF PCI high nibble
    ff_frame.data[1] = length & 0xFF;                   // FF PCI low byte
    
    memcpy(&ff_frame.data[2], data, 6);  // First 6 bytes
    
    g_tx_context.bytes_sent = 6;
    g_tx_context.next_sequence = 1;
    g_can_tp_state = CANTP_STATE_WAIT_FC;
    
    StartTimer(P2_CAN_TIMER, P2_CAN_MAX_MS);
    
    return Can_Transmit(&ff_frame);
}

void CanTp_MainFunction(void) {
    switch(g_can_tp_state) {
        case CANTP_STATE_SEND_CF:
            // Check STmin delay
            if(GetTimeSince(g_tx_context.last_send_time) >= g_tx_context.stmin) {
                CanTp_SendNextConsecutiveFrame();
            }
            break;
            
        case CANTP_STATE_WAIT_FC:
            // Check timeout
            if(IsTimerExpired(P2_CAN_TIMER)) {
                CanTp_AbortTransmission();
            }
            break;
            
        default:
            break;
    }
}

Std_ReturnType CanTp_SendNextConsecutiveFrame(void) {
    if(g_tx_context.bytes_sent >= g_tx_context.length) {
        g_can_tp_state = CANTP_STATE_IDLE;
        CanTp_TxConfirmation(E_OK);
        return E_OK;
    }
    
    CanFrameType cf_frame;
    cf_frame.id = 0x7DF;
    cf_frame.dlc = 8;
    cf_frame.data[0] = 0x20 | g_tx_context.next_sequence;  // CF PCI
    
    uint16_t remaining = g_tx_context.length - g_tx_context.bytes_sent;
    uint8_t bytes_to_send = (remaining > 7) ? 7 : remaining;
    
    memcpy(&cf_frame.data[1], 
           &g_tx_context.data[g_tx_context.bytes_sent], 
           bytes_to_send);
    
    g_tx_context.bytes_sent += bytes_to_send;
    g_tx_context.next_sequence++;
    if(g_tx_context.next_sequence > 15) {
        g_tx_context.next_sequence = 0;
    }
    
    g_tx_context.last_send_time = GetTickCount();
    
    // Decrement block size counter
    if(g_tx_context.block_size > 0) {
        g_tx_context.block_size--;
        if(g_tx_context.block_size == 0) {
            // Wait for next FC
            g_can_tp_state = CANTP_STATE_WAIT_FC;
            StartTimer(P2_CAN_TIMER, P2_CAN_MAX_MS);
        }
    }
    
    return Can_Transmit(&cf_frame);
}
```

---

## 6. Timing Parameters

### ISO 15765-2 Timer Definitions

| Timer | Deskripsi | Default | Min | Max |
|-------|-----------|---------|-----|-----|
| **P2_CAN** | Response time untuk SF/FF/FC | 50 ms | - | 5000 ms |
| **P2_CAN\*** | Response time untuk CF (dengan STmin) | - | - | - |
| **P2_RX** | Maximum wait time untuk CF | - | P2_CAN | - |
| **P2_TX** | Minimum separation time antar CF (STmin) | 0 ms | 0 | 127 ms |

### Timer Implementation

```c
#define P2_CAN_DEFAULT_MS     50U
#define P2_CAN_MAX_MS         5000U
#define P2_RX_MAX_MS          2000U

typedef enum {
    TIMER_P2_CAN = 0,
    TIMER_P2_RX,
    TIMER_COUNT
} CanTpTimerType;

static uint32_t g_timer_start_time[TIMER_COUNT];
static uint32_t g_timer_timeout[TIMER_COUNT];
static uint8_t g_timer_active[TIMER_COUNT];

void StartTimer(CanTpTimerType timer, uint32_t timeout_ms) {
    g_timer_start_time[timer] = GetTickCount();
    g_timer_timeout[timer] = timeout_ms;
    g_timer_active[timer] = 1;
}

uint8_t IsTimerExpired(CanTpTimerType timer) {
    if(!g_timer_active[timer]) {
        return 0;
    }
    
    uint32_t elapsed = GetTickCount() - g_timer_start_time[timer];
    if(elapsed >= g_timer_timeout[timer]) {
        g_timer_active[timer] = 0;
        return 1;
    }
    
    return 0;
}

void StopTimer(CanTpTimerType timer) {
    g_timer_active[timer] = 0;
}

// Main function polling timers
void CanTp_TimerHandler(void) {
    if(IsTimerExpired(TIMER_P2_CAN)) {
        // Timeout waiting for response
        CanTp_TimeoutIndication();
    }
    
    if(IsTimerExpired(TIMER_P2_RX)) {
        // Timeout waiting for consecutive frame
        CanTp_ReceiveTimeout();
    }
}
```

---

## 7. Buffer Management

### Ring Buffer untuk RX

```c
#define CAN_TP_RX_BUFFER_SIZE   4096U

typedef struct {
    uint8_t buffer[CAN_TP_RX_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
    uint16_t total_length;
    uint8_t is_receiving;
} CanTpRxBufferType;

static CanTpRxBufferType g_rx_buffer;

void CanTp_BufferInit(void) {
    g_rx_buffer.head = 0;
    g_rx_buffer.tail = 0;
    g_rx_buffer.count = 0;
    g_rx_buffer.is_receiving = 0;
}

Std_ReturnType CanTp_BufferWrite(const uint8_t *data, uint16_t length) {
    if((g_rx_buffer.count + length) > CAN_TP_RX_BUFFER_SIZE) {
        return E_NOT_OK;  // Buffer overflow
    }
    
    for(uint16_t i = 0; i < length; i++) {
        g_rx_buffer.buffer[g_rx_buffer.head] = data[i];
        g_rx_buffer.head = (g_rx_buffer.head + 1) % CAN_TP_RX_BUFFER_SIZE;
        g_rx_buffer.count++;
    }
    
    return E_OK;
}

Std_ReturnType CanTp_BufferRead(uint8_t *data, uint16_t *length) {
    if(*length > g_rx_buffer.count) {
        *length = g_rx_buffer.count;
    }
    
    for(uint16_t i = 0; i < *length; i++) {
        data[i] = g_rx_buffer.buffer[g_rx_buffer.tail];
        g_rx_buffer.tail = (g_rx_buffer.tail + 1) % CAN_TP_RX_BUFFER_SIZE;
        g_rx_buffer.count--;
    }
    
    return E_OK;
}
```

---

## 8. Error Handling

### Error Codes

```c
typedef enum {
    CANTP_E_OK = 0,
    CANTP_E_WRONG_SN = 1,       // Wrong sequence number
    CANTP_E_INVALID_FS = 2,     // Invalid flow status
    CANTP_E_UNEXP_PDU = 3,      // Unexpected PDU
    CANTP_E_WFT_OVRN = 4,       // Wait frame overrun
    CANTP_E_BC_MISMATCH = 5,    // Block count mismatch
    CANTP_E_STMIN_OVRN = 6,     // STmin overrun
    CANTP_E_BUFFER_OVFLW = 7,   // Buffer overflow
    CANTP_E_TIMEOUT = 8,        // P2/P2* timeout
    CANTP_E_LENGTH_MISMATCH = 9 // Length field mismatch
} CanTpErrorCodeType;
```

### Error Recovery

```c
void CanTp_HandleError(CanTpErrorCodeType error_code) {
    switch(error_code) {
        case CANTP_E_WRONG_SN:
            // Reset reception, send negative FC
            CanTp_ResetReception();
            SendNegativeFC(FC_OVERFLOW);
            break;
            
        case CANTP_E_TIMEOUT:
            // Abort current transaction
            CanTp_AbortTransaction();
            CanTp_NotifyUpperLayer(CANTP_EVENT_TIMEOUT);
            break;
            
        case CANTP_E_BUFFER_OVFLW:
            // Clear buffer and reset
            CanTp_BufferClear();
            CanTp_ResetState();
            break;
            
        default:
            // Generic error handling
            CanTp_ResetState();
            break;
    }
}
```

---

## 9. Studi Kasus

### Case Study: Diagnostic Session Change

**Scenario**: Tester mengirim "Diagnostic Session Control" request dengan security access data (15 bytes).

```
Request: 10 03 27 01 [12 bytes seed/key data]
Total length: 17 bytes
```

**CAN TP Exchange:**

```
Tester → ECU:
[10 11] [10 03 27 01 xx xx]  (FF, length=17)
ECU → Tester:
[30 00 00 00 00 00 00 00]    (FC, CTS, unlimited)
Tester → ECU:
[21 xx xx xx xx xx xx xx]    (CF #1)
[22 xx xx xx xx xx xx xx]    (CF #2)

ECU → Tester:
[10 07] [62 03 00 00 00 00]  (FF, positive response preview)
Tester → ECU:
[30 00 00 00 00 00 00 00]    (FC)
ECU → Tester:
[21 00 00 00 00 00 00 00]    (CF #1, remaining padding)
```

---

## 10. Latihan

### Exercise 1: Single Frame Parser
Buat fungsi untuk parse Single Frame dan validasi length field.

### Exercise 2: Multi-Frame Reassembly
Implementasikan complete reassembly untuk pesan 50-byte dengan 8 CF frames.

### Exercise 3: Flow Control Simulator
Simulasikan scenario dengan BS=5 dan STmin=10ms.

### Exercise 4: Error Injection
Test error handling dengan:
- Wrong sequence number
- Buffer overflow
- Timeout scenarios

---

## 🔗 Next Steps

Setelah menguasai CAN TP, lanjutkan ke:
- [UDS Protocol Master](./10_UDS_Protocol_Master.md) - Layer aplikasi di atas CAN TP
- [Communication Protocols](./08_Communication_Protocols.md) - Review protokol lain
- [Diagnostic Systems](./11_Diagnostic_Systems.md) - Implementasi lengkap diagnostik

## 📚 Referensi

- ISO 15765-2: Road vehicles - Diagnostic communication over CAN
- SAE J1576: Transportation - CAN Transport Protocol
- Vector CAN TP Implementation Guide
- BOSCH Automotive Handbook

---

**Last Updated**: 2024  
**Difficulty**: Advanced  
**Estimated Time**: 2-3 weeks  
**Prerequisites**: [Communication Protocols](./08_Communication_Protocols.md)
