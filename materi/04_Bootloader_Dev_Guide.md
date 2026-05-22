# Bootloader Development Guide - Automotive Firmware Update

## 📋 Informasi Modul

| **Item** | **Detail** |
|----------|------------|
| **Standar** | ISO 14229-1, SAE J2534, AUTOSAR SWS BSW |
| **Level** | Advanced |
| **Prasyarat** | [UDS Protocol Master](./10_UDS_Protocol_Master.md), [C Basics](./01_C_Basics_for_Embedded.md), Microcontroller Architecture |
| **Durasi Belajar** | 10-15 jam teori + 20-30 jam praktik |
| **Bahasa** | Inggris + 日本語 (Technical Terms) |

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, Anda akan mampu:

1. ✅ Memahami arsitektur dan filosofi bootloader dalam sistem otomotif
2. ✅ Merancang memory layout untuk aplikasi dan bootloader
3. ✅ Mengimplementasikan bootloader dengan fitur keamanan lengkap
4. ✅ Membuat mekanisme flash programming yang reliable
5. ✅ Mengembangkan protokol update firmware via UDS
6. ✅ Implementasi rollback dan recovery mechanism
7. ✅ Validasi integritas firmware dengan checksum dan CRC
8. ✅ Membangun secure boot dengan cryptographic verification

---

## 📚 Daftar Isi

1. [Pendahuluan Bootloader](#1-pendahuluan-bootloader)
2. [Arsitektur Bootloader](#2-arsitektur-bootloader)
3. [Memory Layout Design](#3-memory-layout-design)
4. [Boot Process Flow](#4-boot-process-flow)
5. [Flash Memory Programming](#5-flash-memory-programming)
6. [Communication Protocol](#6-communication-protocol)
7. [Security Implementation](#7-security-implementation)
8. [Integrity Verification](#8-integrity-verification)
9. [Rollback & Recovery](#9-rollback--recovery)
10. [AUTOSAR Bootloader](#10-autosar-bootloader)
11. [Implementasi Lengkap](#11-implementasi-lengkap)
12. [Testing & Validation](#12-testing--validation)
13. [Troubleshooting](#13-troubleshooting)
14. [Latihan & Studi Kasus](#14-latihan--studi-kasus)
15. [Glosarium Jepang-Inggris](#15-glosarium-jepang-inggris)

---

## 1. Pendahuluan Bootloader

### 1.1 Apa itu Bootloader?

**Bootloader** adalah program khusus yang dieksekusi pertama kali saat mikrokontroler dinyalakan atau di-reset. Bootloader bertanggung jawab untuk:
- Inisialisasi hardware dasar
- Validitas firmware aplikasi
- Memuat dan menjalankan aplikasi
- Menyediakan mekanisme update firmware

```
┌─────────────────────────────────────────┐
│         Power-On / Reset                │
└─────────────────┬───────────────────────┘
                  │
                  ▼
        ┌─────────────────┐
        │   Bootloader    │
        │  (First Stage)  │
        └────────┬────────┘
                 │
        ┌────────┴────────┐
        │                 │
        ▼                 ▼
┌──────────────┐  ┌──────────────┐
│   Update     │  │   Normal     │
│   Mode       │  │   Boot       │
└──────────────┘  └──────────────┘
```

### 1.2 Mengapa Bootloader Penting di Otomotif?

| **Fungsi** | **Deskripsi** | **Contoh Penggunaan** |
|------------|---------------|----------------------|
| **Field Updates** | Update firmware tanpa mengganti ECU | Recall campaigns, feature updates |
| **Calibration** | Update parameter kalibrasi | Engine tuning, emission adjustments |
| **Bug Fixes** | Perbaikan software bugs | Safety patches, performance improvements |
| **Feature Activation** | Aktivasi fitur baru | OTA feature enablement |
| **Recovery** | Recovery dari corrupted firmware | Brick prevention, fail-safe |

### 1.3 Jenis-Jenis Bootloader

**Berdasarkan Lokasi:**
```
Primary Bootloader (ROM Code):
├─ Embedded dalam mask ROM
├─ Tidak dapat diupdate
└─ Load secondary bootloader

Secondary Bootloader (Flash):
├─ Dapat diupdate
├─ Lebih kompleks
└─ Menjalankan aplikasi

Application Bootloader:
├─ Bagian dari aplikasi
├─ Untuk update diri sendiri
└─ Dual-bank implementation
```

**Berdasarkan Fungsi:**
```c
/*
Simple Bootloader:
- Basic validation
- Direct jump to application
- No communication interface

Advanced Bootloader:
- Cryptographic verification
- Communication via CAN/ETH
- Rollback capability
- Secure boot support

Production Bootloader:
- AUTOSAR compliant
- Multi-ECU flashing
- Diagnostic services
- Security HSM integration
*/
```

### 1.4 Bootloader vs Application

| **Aspek** | **Bootloader** | **Application** |
|-----------|---------------|-----------------|
| **Ukuran** | Kecil (4-16 KB) | Besar (64-512 KB) |
| **Kompleksitas** | Rendah | Tinggi |
| **Update Frequency** | Jarang | Sering |
| **Critical Level** | Sangat Tinggi | Tinggi |
| **Initialization** | Minimal | Lengkap |
| **Stack Size** | Kecil | Besar |

---

## 2. Arsitektur Bootloader

### 2.1 Arsitektur Umum

```
┌─────────────────────────────────────────────────────────┐
│                    Vehicle Gateway                       │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐      │
│  │  Engine ECU │  │  Brake ECU  │  │  Body ECU   │      │
│  │  + BL       │  │  + BL       │  │  + BL       │      │
│  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘      │
│         │                │                │              │
│         └────────────────┼────────────────┘              │
│                          │                               │
│                 ┌────────▼────────┐                      │
│                 │  Diagnostic     │                      │
│                 │  Interface      │                      │
│                 └────────┬────────┘                      │
└──────────────────────────┼───────────────────────────────┘
                           │
                  ┌────────▼────────┐
                  │  External Tool  │
                  │  (CANoe/Flash)  │
                  └─────────────────┘
```

### 2.2 Komponen Bootloader

```c
typedef struct {
    // Core Components
    void (*init)(void);              // Hardware initialization
    bool (*validate_app)(void);      // Application validation
    void (*jump_to_app)(void);       // Jump to application
    
    // Communication
    int (*receive_data)(uint8_t*, uint16_t);  // Receive flash data
    int (*send_response)(uint8_t*, uint16_t); // Send ACK/NACK
    
    // Flash Operations
    int (*erase_flash)(uint32_t addr, uint32_t size);
    int (*write_flash)(uint32_t addr, const uint8_t* data, uint32_t size);
    int (*verify_flash)(uint32_t addr, const uint8_t* data, uint32_t size);
    
    // Security
    bool (*verify_signature)(const uint8_t* fw, uint32_t size);
    uint32_t (*calculate_checksum)(const uint8_t* data, uint32_t size);
    
    // State Management
    uint8_t current_state;
    uint32_t app_start_address;
    uint32_t app_size;
} BootloaderConfigType;
```

### 2.3 State Machine

```
┌─────────────────┐
│  POWER_ON_RESET │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  INITIALIZATION │◄──────────────────┐
└────────┬────────┘                   │
         │                            │
         ▼                            │
┌─────────────────┐     Timeout       │
│  WAIT_COMMAND   │───────────────────┘
└────────┬────────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
┌───────┐ ┌──────────┐
│ FLASH │ │  BOOT    │
│ MODE  │ │  NORMAL  │
└───┬───┘ └────┬─────┘
    │          │
    ▼          ▼
┌───────┐ ┌──────────┐
│PROGRAM│ │RUN APP   │
└───────┘ └──────────┘
```

---

## 3. Memory Layout Design

### 3.1 Standard Memory Map

```
Memory Address Space (Example: 512KB Flash)

┌─────────────────────────────────────┐ ← 0x00000000
│     Bootloader Code (16 KB)         │
│     - Vector Table                  │
│     - Boot Logic                    │
│     - Communication Drivers         │
├─────────────────────────────────────┤ ← 0x00004000
│     Bootloader Data (4 KB)          │
│     - Configuration                 │
│     - Flags                         │
│     - Backup Data                   │
├─────────────────────────────────────┤ ← 0x00005000
│     Reserved / Empty                │
├─────────────────────────────────────┤ ← 0x00008000
│     Application Code (448 KB)       │
│     - Application Vector Table      │
│     - Program Code                  │
│     - Constants                     │
├─────────────────────────────────────┤ ← 0x00078000
│     Application Data (32 KB)        │
│     - Variables                     │
│     - Stack                         │
│     - Heap                          │
├─────────────────────────────────────┤ ← 0x00080000
│     EEPROM Simulation (4 KB)        │
│     - Calibration Data              │
│     - DTC Storage                   │
├─────────────────────────────────────┤ ← 0x00081000
│     Flash Parameters (4 KB)         │
│     - Boot Counter                  │
│     - Version Info                  │
│     - Security Keys                 │
└─────────────────────────────────────┘ ← 0x00082000 (512KB)
```

### 3.2 Linker Script Example (GCC ARM)

```ld
/* linker_script.ld */

MEMORY
{
    FLASH_BL (rx) : ORIGIN = 0x00000000, LENGTH = 16K
    FLASH_DATA (r) : ORIGIN = 0x00004000, LENGTH = 4K
    FLASH_APP (rx) : ORIGIN = 0x00008000, LENGTH = 448K
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
    /* Bootloader Section */
    .bootloader :
    {
        _sbl = .;
        *(.isr_vector)
        *(.text.bl)
        *(.rodata.bl)
        _ebl = .;
    } > FLASH_BL

    /* Bootloader Data */
    .bl_data :
    {
        *(.bl_config)
        *(.bl_flags)
    } > FLASH_DATA

    /* Application Section */
    .app :
    {
        _sapp = .;
        *(.text.app)
        *(.rodata.app)
        _eapp = .;
    } > FLASH_APP

    /* RAM for both BL and App */
    .ram :
    {
        _sram = .;
        *(.data)
        *(.bss)
        _eram = .;
    } > RAM
}

/* Symbols for runtime use */
__bootloader_end = 0x00005000;
__app_start = 0x00008000;
__app_end = 0x00080000;
```

### 3.3 Dual-Bank Architecture

Untuk update yang lebih aman, gunakan dual-bank:

```
Dual-Bank Flash Layout

Bank A (Active)          Bank B (Inactive)
┌─────────────────┐     ┌─────────────────┐
│  Application A  │     │  Application B  │
│  (Running)      │     │  (Update Target)│
└─────────────────┘     └─────────────────┘
        ▲                       │
        │                       ▼
        │              Write new firmware
        │                       │
        │                       ▼
        │              Verify & Validate
        │                       │
        └────────────── Swap Banks
               (Update Complete)
```

**Keuntungan Dual-Bank:**
- ✅ Rollback instant jika update gagal
- ✅ Tidak ada downtime
- ✅ Always have working firmware
- ✅ Safe update process

**Kekurangan:**
- ❌ Butuh 2x flash memory
- ❌ Lebih kompleks
- ❌ Biaya lebih tinggi

---

## 4. Boot Process Flow

### 4.1 Startup Sequence

```c
#include "bootloader.h"
#include "hardware_init.h"
#include "flash_driver.h"

// Entry point - called after reset
void Reset_Handler(void) {
    // 1. Disable interrupts
    __disable_irq();
    
    // 2. Initialize clock system
    Clock_Init();
    
    // 3. Initialize basic peripherals
    GPIO_Init();
    UART_Init();  // For debug output
    
    // 4. Check boot mode
    BootModeType boot_mode = Determine_Boot_Mode();
    
    // 5. Route to appropriate handler
    switch (boot_mode) {
        case BOOT_MODE_APPLICATION:
            BootLoader_JumpToApplication();
            break;
            
        case BOOT_MODE_BOOTLOADER:
            BootLoader_MainLoop();
            break;
            
        case BOOT_MODE_RECOVERY:
            BootLoader_RecoveryMode();
            break;
            
        default:
            // Default to bootloader
            BootLoader_MainLoop();
            break;
    }
    
    // Should never reach here
    while (1) {
        __WFI();
    }
}

/**
 * @brief Determine boot mode based on flags and conditions
 */
BootModeType Determine_Boot_Mode(void) {
    // Check hardware pin (e.g., BOOT0 pin)
    if (GPIO_ReadPin(BOOT_PIN) == BOOT_ACTIVE_LOW) {
        return BOOT_MODE_BOOTLOADER;
    }
    
    // Check bootloader flag in backup register
    uint32_t boot_flag = Backup_Read(BOOT_FLAG_REG);
    if (boot_flag == BOOT_FLAG_FORCE_BL) {
        Backup_Write(BOOT_FLAG_REG, 0);  // Clear flag
        return BOOT_MODE_BOOTLOADER;
    }
    
    // Check application validity
    if (!BootLoader_ValidateApplication()) {
        // Invalid application - stay in bootloader
        UART_Printf("Invalid application detected!\n");
        return BOOT_MODE_BOOTLOADER;
    }
    
    // Check for update request
    if (BootLoader_CheckUpdateRequest()) {
        return BOOT_MODE_BOOTLOADER;
    }
    
    // Default: boot application
    return BOOT_MODE_APPLICATION;
}
```

### 4.2 Application Validation

```c
/**
 * @brief Validate application before jumping
 * @return true if valid, false otherwise
 */
bool BootLoader_ValidateApplication(void) {
    // 1. Check stack pointer is valid
    uint32_t app_sp = *((uint32_t*)__app_start);
    if ((app_sp & 0xFFFFFC00) != 0x20000000) {
        UART_Printf("Invalid stack pointer: 0x%08X\n", app_sp);
        return false;
    }
    
    // 2. Check reset vector is valid
    uint32_t app_reset = *((uint32_t*)(__app_start + 4));
    if ((app_reset & 0x00000001) == 0) {
        UART_Printf("Invalid reset vector: 0x%08X\n", app_reset);
        return false;
    }
    
    // 3. Verify checksum
    uint32_t stored_checksum = *((uint32_t*)(__app_end - 4));
    uint32_t calculated_checksum = Calculate_CRC32(
        (uint8_t*)__app_start, 
        __app_end - __app_start - 4
    );
    
    if (stored_checksum != calculated_checksum) {
        UART_Printf("Checksum mismatch! Stored: 0x%08X, Calc: 0x%08X\n",
                   stored_checksum, calculated_checksum);
        return false;
    }
    
    // 4. Verify signature (if security enabled)
    #if ENABLE_SECURE_BOOT
    if (!BootLoader_VerifySignature((uint8_t*)__app_start, 
                                    __app_end - __app_start)) {
        UART_Printf("Signature verification failed!\n");
        return false;
    }
    #endif
    
    UART_Printf("Application validation successful!\n");
    return true;
}

/**
 * @brief Jump to application
 */
void BootLoader_JumpToApplication(void) {
    UART_Printf("Jumping to application at 0x%08X...\n", __app_start);
    
    // 1. Deinitialize peripherals used by bootloader
    UART_DeInit();
    Can_DeInit();
    
    // 2. Disable interrupts
    __disable_irq();
    
    // 3. Set stack pointer
    __set_MSP(*((uint32_t*)__app_start));
    
    // 4. Set vector table offset
    SCB->VTOR = __app_start;
    
    // 5. Get application entry point
    uint32_t app_entry = *((uint32_t*)(__app_start + 4));
    void (*app_reset_handler)(void) = (void(*)(void))app_entry;
    
    // 6. Jump to application
    app_reset_handler();
    
    // Should never return
    while (1) {
        __WFI();
    }
}
```

### 4.3 Bootloader Main Loop

```c
/**
 * @brief Main bootloader loop
 */
void BootLoader_MainLoop(void) {
    UART_Printf("\n=== Bootloader Started ===\n");
    UART_Printf("Version: %s\n", BOOTLOADER_VERSION);
    UART_Printf("App Start: 0x%08X\n", __app_start);
    
    // Initialize communication
    Can_Init(CAN_CONFIG_500KBPS);
    Uds_Init();
    
    // Main state machine
    BootStateType state = BOOT_STATE_WAIT;
    uint32_t timeout_counter = 0;
    const uint32_t timeout_max = BOOT_TIMEOUT_MS / SYSTEM_TICK_MS;
    
    while (1) {
        // Service communication
        Uds_MainFunction();
        
        // Check for commands
        if (BootLoader_HasPendingCommand()) {
            BootCommandType cmd = BootLoader_GetCommand();
            state = BootLoader_ProcessCommand(cmd);
            timeout_counter = 0;  // Reset timeout on activity
        }
        
        // Check timeout
        timeout_counter++;
        if (timeout_counter >= timeout_max) {
            UART_Printf("Boot timeout - attempting to boot application\n");
            
            if (BootLoader_ValidateApplication()) {
                BootLoader_JumpToApplication();
            }
            timeout_counter = 0;
        }
        
        // Watchdog feed
        IWDG_Reload();
    }
}
```

---

## 5. Flash Memory Programming

### 5.1 Flash Characteristics

```c
typedef struct {
    uint32_t base_address;
    uint32_t total_size;
    uint32_t sector_count;
    uint32_t sector_size;
    uint32_t page_size;
    uint32_t write_alignment;
    uint32_t erase_time_ms;
    uint32_t write_time_ms;
    uint32_t endurance_cycles;
} FlashPropertiesType;

// Example: STM32F4 Flash
static const FlashPropertiesType flash_props = {
    .base_address = 0x08000000,
    .total_size = 512 * 1024,  // 512 KB
    .sector_count = 12,
    .sector_size = 16 * 1024,  // 16 KB sectors
    .page_size = 4,             // Word write (4 bytes)
    .write_alignment = 4,
    .erase_time_ms = 1000,
    .write_time_ms = 50,
    .endurance_cycles = 10000
};
```

### 5.2 Flash Driver Implementation

```c
#include "stm32f4xx.h"

typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR_BUSY,
    FLASH_ERROR_PROGRAM,
    FLASH_ERROR_WRITE_PROTECTION,
    FLASH_ERROR_ALIGNMENT,
    FLASH_ERROR_TIMEOUT
} FlashStatusType;

/**
 * @brief Erase flash sectors
 */
FlashStatusType Flash_EraseSectors(uint32_t start_sector, uint32_t sector_count) {
    // Wait for busy
    if (FLASH_WaitForOperation(FLASH_TIMEOUT_ERASE) != FLASH_OK) {
        return FLASH_ERROR_TIMEOUT;
    }
    
    // Unlock flash
    FLASH_Unlock();
    
    // Clear status flags
    FLASH->SR = FLASH_SR_EOP | FLASH_SR_OPERR | FLASH_SR_WRPERR;
    
    // Erase sectors
    for (uint32_t i = 0; i < sector_count; i++) {
        uint32_t sector = start_sector + i;
        
        // Set sector erase bit
        FLASH->CR &= ~FLASH_CR_SNB;
        FLASH->CR |= FLASH_CR_SER;
        FLASH->CR |= (sector << FLASH_CR_SNB_Pos);
        
        // Start erase
        FLASH->CR |= FLASH_CR_STRT;
        
        // Wait for completion
        if (FLASH_WaitForOperation(FLASH_TIMEOUT_ERASE) != FLASH_OK) {
            FLASH_Lock();
            return FLASH_ERROR_TIMEOUT;
        }
    }
    
    // Lock flash
    FLASH_Lock();
    
    return FLASH_OK;
}

/**
 * @brief Program flash memory
 */
FlashStatusType Flash_ProgramWord(uint32_t address, uint32_t data) {
    // Check alignment
    if (address % 4 != 0) {
        return FLASH_ERROR_ALIGNMENT;
    }
    
    // Wait for busy
    if (FLASH_WaitForOperation(FLASH_TIMEOUT_WRITE) != FLASH_OK) {
        return FLASH_ERROR_TIMEOUT;
    }
    
    // Unlock flash
    FLASH_Unlock();
    
    // Clear status flags
    FLASH->SR = FLASH_SR_EOP | FLASH_SR_OPERR | FLASH_SR_WRPERR;
    
    // Set programming bit
    FLASH->CR |= FLASH_CR_PG;
    
    // Write data
    *(__IO uint32_t*)address = data;
    
    // Wait for completion
    FlashStatusType status = FLASH_WaitForOperation(FLASH_TIMEOUT_WRITE);
    
    // Lock flash
    FLASH_Lock();
    
    return status;
}

/**
 * @brief Wait for flash operation to complete
 */
FlashStatusType FLASH_WaitForOperation(uint32_t timeout_ms) {
    uint32_t start_time = GetTickCount();
    
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET) {
        if (GetTickCount() - start_time > timeout_ms) {
            return FLASH_ERROR_TIMEOUT;
        }
    }
    
    // Check error flags
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPERR) != RESET) {
        return FLASH_ERROR_PROGRAM;
    }
    
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) != RESET) {
        return FLASH_ERROR_WRITE_PROTECTION;
    }
    
    return FLASH_OK;
}

/**
 * @brief Verify flash content
 */
FlashStatusType Flash_Verify(uint32_t address, const uint8_t* expected_data, 
                            uint32_t length) {
    for (uint32_t i = 0; i < length; i += 4) {
        uint32_t expected_word;
        memcpy(&expected_word, &expected_data[i], 4);
        
        uint32_t actual_word = *((uint32_t*)(address + i));
        
        if (expected_word != actual_word) {
            UART_Printf("Verify failed at 0x%08X: Expected 0x%08X, Got 0x%08X\n",
                       address + i, expected_word, actual_word);
            return FLASH_ERROR_PROGRAM;
        }
    }
    
    return FLASH_OK;
}
```

### 5.3 Block Transfer Protocol

```c
#define BLOCK_SIZE 256
#define MAX_BLOCKS 2048

typedef struct {
    uint16_t block_number;
    uint16_t total_blocks;
    uint8_t data[BLOCK_SIZE];
    uint32_t checksum;
} FlashBlockType;

/**
 * @brief Handle flash block write
 */
int BootLoader_HandleBlockWrite(const FlashBlockType* block) {
    static uint16_t expected_block = 0;
    static uint32_t flash_address = __app_start;
    
    // Check sequence
    if (block->block_number != expected_block) {
        UART_Printf("Block sequence error: Expected %d, Got %d\n",
                   expected_block, block->block_number);
        return -1;
    }
    
    // Verify block checksum
    uint32_t calc_checksum = Calculate_CRC32(block->data, BLOCK_SIZE);
    if (calc_checksum != block->checksum) {
        UART_Printf("Block checksum error!\n");
        return -1;
    }
    
    // Erase sector if needed
    if (block->block_number % (SECTOR_SIZE / BLOCK_SIZE) == 0) {
        uint32_t sector = block->block_number / (SECTOR_SIZE / BLOCK_SIZE);
        Flash_EraseSectors(APP_START_SECTOR + sector, 1);
    }
    
    // Write block
    FlashStatusType status = Flash_ProgramData(
        flash_address, 
        block->data, 
        BLOCK_SIZE
    );
    
    if (status != FLASH_OK) {
        UART_Printf("Flash write failed!\n");
        return -1;
    }
    
    // Verify block
    status = Flash_Verify(flash_address, block->data, BLOCK_SIZE);
    if (status != FLASH_OK) {
        UART_Printf("Flash verify failed!\n");
        return -1;
    }
    
    // Update state
    flash_address += BLOCK_SIZE;
    expected_block++;
    
    UART_Printf("Block %d/%d written successfully\n",
               block->block_number, block->total_blocks);
    
    return 0;
}
```

---

## 6. Communication Protocol

### 6.1 UDS-Based Flashing

Menggunakan UDS services untuk flashing:

```c
/**
 * @brief Handle UDS Request Download (0x34)
 */
int Uds_HandleRequestDownload(const uint8_t* request, uint8_t len, uint8_t* response) {
    if (len < 3) {
        return Uds_SendNegativeResponse(0x34, 0x13);
    }
    
    uint8_t data_format = request[1];
    uint32_t address = ((uint32_t)request[2] << 16) | 
                      ((uint32_t)request[3] << 8) | 
                      request[4];
    uint32_t size = ((uint32_t)request[5] << 16) | 
                   ((uint32_t)request[6] << 8) | 
                   request[7];
    
    // Validate address
    if (address < __app_start || address + size > __app_end) {
        return Uds_SendNegativeResponse(0x34, 0x31);
    }
    
    // Check security
    if (g_uds_state.security_level < SECURITY_LEVEL_2) {
        return Uds_SendNegativeResponse(0x34, 0x33);
    }
    
    // Prepare for download
    g_flash_context.address = address;
    g_flash_context.size = size;
    g_flash_context.blocks_received = 0;
    g_flash_context.total_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    // Erase target area
    Flash_EraseSectors(APP_START_SECTOR, APP_SECTOR_COUNT);
    
    // Positive response
    response[0] = 0x74;  // 0x34 + 0x40
    response[1] = 0x20;  // Length format identifier
    response[2] = 0x00;  // Address format
    response[3] = 0x00;
    response[4] = 0x00;
    response[5] = 0x04;  // Max block length
    
    return 6;
}

/**
 * @brief Handle UDS Transfer Data (0x36)
 */
int Uds_HandleTransferData(const uint8_t* request, uint8_t len, uint8_t* response) {
    if (len < 2) {
        return Uds_SendNegativeResponse(0x36, 0x13);
    }
    
    uint8_t block_sequence = request[1];
    const uint8_t* data = &request[2];
    uint8_t data_len = len - 2;
    
    // Check sequence
    if (block_sequence != g_flash_context.expected_sequence) {
        return Uds_SendNegativeResponse(0x36, 0x73);
    }
    
    // Write data
    Flash_ProgramData(g_flash_context.address, data, data_len);
    
    // Update context
    g_flash_context.address += data_len;
    g_flash_context.expected_sequence++;
    g_flash_context.blocks_received++;
    
    // Positive response
    response[0] = 0x76;  // 0x36 + 0x40
    response[1] = block_sequence;
    
    return 2;
}
```

### 6.2 CAN-Based Custom Protocol

```c
#define CAN_ID_FLASH_REQUEST  0x7DF
#define CAN_ID_FLASH_RESPONSE 0x7E8

typedef enum {
    FLASH_CMD_INIT = 0x01,
    FLASH_CMD_ERASE = 0x02,
    FLASH_CMD_WRITE = 0x03,
    FLASH_CMD_VERIFY = 0x04,
    FLASH_CMD_COMPLETE = 0x05,
    FLASH_CMD_ABORT = 0x06
} FlashCommandType;

typedef struct {
    uint8_t command;
    uint8_t sequence;
    uint16_t length;
    uint32_t address;
    uint8_t data[8];
} FlashMessageTx;

typedef struct {
    uint8_t response;
    uint8_t sequence;
    uint16_t status;
    uint32_t progress;
} FlashMessageRx;

/**
 * @brief Process flash command via CAN
 */
void Flash_ProcessCanCommand(const FlashMessageTx* cmd) {
    FlashMessageRx response;
    response.sequence = cmd->sequence;
    
    switch (cmd->command) {
        case FLASH_CMD_INIT:
            response.response = FLASH_CMD_INIT;
            response.status = BootLoader_InitFlash();
            response.progress = 0;
            break;
            
        case FLASH_CMD_ERASE:
            response.response = FLASH_CMD_ERASE;
            response.status = Flash_EraseApplication();
            response.progress = 10;  // 10% complete
            break;
            
        case FLASH_CMD_WRITE:
            response.response = FLASH_CMD_WRITE;
            response.status = Flash_WriteBlock(cmd->address, cmd->data, cmd->length);
            response.progress = 50 + (g_flash_context.blocks_received * 40 / 
                                     g_flash_context.total_blocks);
            break;
            
        case FLASH_CMD_VERIFY:
            response.response = FLASH_CMD_VERIFY;
            response.status = Flash_VerifyApplication();
            response.progress = 95;
            break;
            
        case FLASH_CMD_COMPLETE:
            response.response = FLASH_CMD_COMPLETE;
            response.status = BootLoader_FinalizeUpdate();
            response.progress = 100;
            break;
            
        default:
            response.response = 0xFF;
            response.status = 1;  // Error
            break;
    }
    
    // Send response
    Can_Send(CAN_ID_FLASH_RESPONSE, (uint8_t*)&response, sizeof(response));
}
```

---

## 7. Security Implementation

### 7.1 Secure Boot

```c
#if ENABLE_SECURE_BOOT

#include "crypto_lib.h"

#define PUBLIC_KEY_STORAGE_ADDR  0x0807F000
#define SIGNATURE_STORAGE_ADDR   0x0807F200

/**
 * @brief Verify firmware signature using RSA/ECC
 */
bool BootLoader_VerifySignature(const uint8_t* firmware, uint32_t size) {
    // Read public key from protected area
    uint8_t public_key[64];
    Flash_Read(PUBLIC_KEY_STORAGE_ADDR, public_key, 64);
    
    // Read stored signature
    uint8_t signature[64];
    Flash_Read(SIGNATURE_STORAGE_ADDR, signature, 64);
    
    // Calculate hash of firmware
    uint8_t hash[32];
    Crypto_SHA256(firmware, size, hash);
    
    // Verify signature
    int result = Crypto_VerifyRSA(public_key, hash, 32, signature, 64);
    
    if (result == 0) {
        UART_Printf("Signature verified successfully\n");
        return true;
    } else {
        UART_Printf("Signature verification FAILED!\n");
        return false;
    }
}

/**
 * @brief Sign firmware during build process (host-side example)
 */
void SignFirmware(const char* firmware_file, const char* private_key_file,
                 const char* output_file) {
    // Read firmware
    uint8_t* firmware = ReadFile(firmware_file);
    uint32_t size = GetFileSize(firmware_file);
    
    // Calculate hash
    uint8_t hash[32];
    SHA256(firmware, size, hash);
    
    // Sign hash with private key
    uint8_t signature[64];
    SignRSA(private_key_file, hash, 32, signature, 64);
    
    // Append signature to firmware
    WriteFile(output_file, firmware, size);
    AppendFile(output_file, signature, 64);
    
    printf("Firmware signed successfully\n");
}

#endif
```

### 7.2 Boot Counter & Anti-Rollback

```c
#define BOOT_COUNTER_ADDR  0x0807E000
#define VERSION_ADDR       0x0807E004

typedef struct {
    uint32_t boot_counter;
    uint32_t current_version;
    uint32_t minimum_version;  // For anti-rollback
    uint32_t reserved[5];
    uint32_t checksum;
} BootInfoType;

/**
 * @brief Check for rollback attempt
 */
bool BootLoader_CheckAntiRollback(void) {
    BootInfoType boot_info;
    Flash_Read(BOOT_COUNTER_ADDR, &boot_info, sizeof(boot_info));
    
    uint32_t new_version = GetFirmwareVersion();
    
    if (new_version < boot_info.minimum_version) {
        UART_Printf("ROLLBACK DETECTED! New version %d < Minimum version %d\n",
                   new_version, boot_info.minimum_version);
        return false;  // Reject older version
    }
    
    return true;
}

/**
 * @brief Update boot information after successful update
 */
void BootLoader_UpdateBootInfo(uint32_t new_version) {
    BootInfoType boot_info;
    
    boot_info.boot_counter++;
    boot_info.current_version = new_version;
    
    // Only increase minimum version
    if (new_version > boot_info.minimum_version) {
        boot_info.minimum_version = new_version;
    }
    
    boot_info.checksum = Calculate_CRC32(&boot_info, sizeof(boot_info) - 4);
    
    // Write to flash
    Flash_EraseSector(BOOT_INFO_SECTOR);
    Flash_ProgramData(BOOT_COUNTER_ADDR, &boot_info, sizeof(boot_info));
}
```

---

## 8. Integrity Verification

### 8.1 Checksum Algorithms

```c
/**
 * @brief Simple checksum (XOR)
 */
uint8_t Calculate_Checksum_XOR(const uint8_t* data, uint32_t length) {
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

/**
 * @brief Sum checksum
 */
uint8_t Calculate_Checksum_Sum(const uint8_t* data, uint32_t length) {
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}

/**
 * @brief CRC-16 CCITT
 */
uint16_t Calculate_CRC16(const uint8_t* data, uint32_t length) {
    uint16_t crc = 0xFFFF;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

/**
 * @brief CRC-32 (Ethernet polynomial)
 */
uint32_t Calculate_CRC32(const uint8_t* data, uint32_t length) {
    uint32_t crc = 0xFFFFFFFF;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc ^ 0xFFFFFFFF;
}
```

### 8.2 Integrity Check on Boot

```c
/**
 * @brief Comprehensive integrity check
 */
bool BootLoader_FullIntegrityCheck(void) {
    uint32_t app_size = __app_end - __app_start;
    
    UART_Printf("Performing integrity check...\n");
    
    // 1. CRC check
    uint32_t stored_crc = *((uint32_t*)(__app_end - 4));
    uint32_t calculated_crc = Calculate_CRC32((uint8_t*)__app_start, app_size - 4);
    
    UART_Printf("CRC Check: Stored=0x%08X, Calculated=0x%08X\n",
               stored_crc, calculated_crc);
    
    if (stored_crc != calculated_crc) {
        return false;
    }
    
    // 2. Signature check (if enabled)
    #if ENABLE_SECURE_BOOT
    if (!BootLoader_VerifySignature((uint8_t*)__app_start, app_size)) {
        return false;
    }
    #endif
    
    // 3. Vector table sanity check
    uint32_t msp = *((uint32_t*)__app_start);
    uint32_t reset = *((uint32_t*)(__app_start + 4));
    
    if ((msp & 0xFF000000) != 0x20000000) {
        UART_Printf("Invalid MSP: 0x%08X\n", msp);
        return false;
    }
    
    if ((reset & 1) == 0) {
        UART_Printf("Invalid reset vector: 0x%08X\n", reset);
        return false;
    }
    
    UART_Printf("Integrity check PASSED\n");
    return true;
}
```

---

## 9. Rollback & Recovery

### 9.1 Rollback Mechanism

```c
typedef enum {
    UPDATE_STATUS_NONE = 0,
    UPDATE_STATUS_IN_PROGRESS,
    UPDATE_STATUS_COMPLETED,
    UPDATE_STATUS_FAILED,
    UPDATE_STATUS_ROLLBACK
} UpdateStatusType;

#define UPDATE_FLAG_ADDR  0x0807D000

/**
 * @brief Initiate rollback to previous version
 */
void BootLoader_Rollback(void) {
    UART_Printf("Initiating rollback...\n");
    
    // Read update status
    UpdateStatusType status;
    Flash_Read(UPDATE_FLAG_ADDR, &status, sizeof(status));
    
    if (status == UPDATE_STATUS_IN_PROGRESS) {
        // Update was interrupted - mark as failed
        status = UPDATE_STATUS_FAILED;
        Flash_ProgramWord(UPDATE_FLAG_ADDR, status);
    }
    
    // In dual-bank system, switch to other bank
    #if DUAL_BANK_ENABLED
    uint32_t active_bank = Read_Active_Bank();
    Switch_Active_Bank(!active_bank);
    UART_Printf("Switched to bank %d\n", !active_bank);
    #endif
    
    // Attempt to boot
    if (BootLoader_ValidateApplication()) {
        BootLoader_JumpToApplication();
    } else {
        UART_Printf("Rollback FAILED - no valid application\n");
        // Stay in bootloader for recovery
    }
}

/**
 * @brief Recovery mode - accept firmware via alternative interface
 */
void BootLoader_RecoveryMode(void) {
    UART_Printf("=== RECOVERY MODE ===\n");
    
    // Initialize UART for XMODEM download
    UART_Init_Recovery(115200);
    
    UART_Printf("Waiting for firmware upload via XMODEM...\n");
    
    // XMODEM receive
    uint8_t buffer[128];
    uint32_t total_received = 0;
    
    while (1) {
        int packet_num = XMODEM_ReceivePacket(buffer);
        if (packet_num < 0) {
            UART_Printf("XMODEM error\n");
            continue;
        }
        
        if (packet_num == 0) {
            // End of transmission
            break;
        }
        
        // Write to flash
        Flash_ProgramData(__app_start + total_received, buffer, 128);
        total_received += 128;
        
        UART_Printf("Received %d bytes\n", total_received);
    }
    
    UART_Printf("Upload complete. Validating...\n");
    
    if (BootLoader_ValidateApplication()) {
        UART_Printf("Validation successful. Booting...\n");
        BootLoader_JumpToApplication();
    } else {
        UART_Printf("Validation failed. Staying in recovery.\n");
    }
}
```

### 9.2 Watchdog Integration

```c
/**
 * @brief Independent watchdog configuration
 */
void BootLoader_Watchdog_Init(void) {
    // Enable LSI oscillator
    RCC->CSR |= RCC_CSR_LSION;
    while (!(RCC->CSR & RCC_CSR_LSIRDY)) {
        // Wait for LSI ready
    }
    
    // Enable IWDG
    IWDG->KR = 0x5555;  // Enable register access
    IWDG->PR = 0x06;    // Prescaler /256
    IWDG->RLR = 0xFFF;  // Reload value (~2 seconds)
    IWDG->KR = 0xAAAA;  // Reload
    IWDG->KR = 0xCCCC;  // Start IWDG
}

/**
 * @brief Feed watchdog
 */
void BootLoader_Watchdog_Feed(void) {
    IWDG->KR = 0xAAAA;
}

// Usage in main loop
void BootLoader_MainLoop(void) {
    BootLoader_Watchdog_Init();
    
    while (1) {
        // Process commands
        BootLoader_ProcessCommands();
        
        // Feed watchdog
        BootLoader_Watchdog_Feed();
        
        // Check for timeout
        if (Watchdog_Timer_Expired()) {
            // System will reset automatically
        }
    }
}
```

---

## 10. AUTOSAR Bootloader

### 10.1 AUTOSAR BSW Architecture

```
AUTOSAR Bootloader Stack

+----------------------------------+
│     Application Layer            │
│  - Fim (Flash Image Management)  │
+----------------------------------+
│     RTE                          │
+----------------------------------+
│     BSW Module Layer             │
│  - Fls (Flash Driver)            │
│  - Fee (Flash EEPROM Emulation)  │
│  - Ea (EEPROM Abstraction)       │
│  - Bl (Bootloader Handler)       │
+----------------------------------+
│     MCAL                         │
│  - Mcu, Port, Dio                │
+----------------------------------+
│     Microcontroller              │
+----------------------------------+
```

### 10.2 AUTOSAR Bl State Machine

```c
typedef enum {
    BL_ST_IDLE = 0,
    BL_ST_RECEIVE,
    BL_ST_ERASE,
    BL_ST_WRITE,
    BL_ST_VERIFY,
    BL_ST_COMPLETE,
    BL_ST_ERROR
} Bl_StateType;

// AUTOSAR-style interface
Std_ReturnType Bl_Init(const Bl_ConfigType* config);
Std_ReturnType Bl_MainFunction(void);
Std_ReturnType Bl_RequestDownload(uint32_t address, uint32_t size);
Std_ReturnType Bl_TransferData(uint8_t sequence, const uint8_t* data, uint16_t length);
Std_ReturnType Bl_RequestTransferExit(void);
```

---

## 11. Implementasi Lengkap

### 11.1 Full Bootloader Structure

```c
/* bootloader.h */
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include <stdbool.h>

// Configuration
#define BOOTLOADER_VERSION "1.0.0"
#define ENABLE_SECURE_BOOT 1
#define DUAL_BANK_ENABLED 0
#define BOOT_TIMEOUT_MS 5000

// Memory addresses
#define __app_start     0x08008000
#define __app_end       0x08080000
#define FLASH_SIZE      0x80000

// Boot modes
typedef enum {
    BOOT_MODE_APPLICATION,
    BOOT_MODE_BOOTLOADER,
    BOOT_MODE_RECOVERY
} BootModeType;

// Flash context
typedef struct {
    uint32_t address;
    uint32_t size;
    uint16_t blocks_received;
    uint16_t total_blocks;
    uint8_t expected_sequence;
    bool in_progress;
} FlashContextType;

// API Functions
void BootLoader_Init(void);
void BootLoader_MainLoop(void);
bool BootLoader_ValidateApplication(void);
void BootLoader_JumpToApplication(void);

#endif
```

### 11.2 Main Implementation

```c
/* bootloader.c */
#include "bootloader.h"
#include "hardware.h"
#include "flash.h"
#include "can.h"
#include "uds.h"

static FlashContextType g_flash_context = {0};
static bool g_update_in_progress = false;

void BootLoader_Init(void) {
    // Hardware initialization
    SystemClock_Init();
    GPIO_Init();
    UART_Init(115200);
    
    UART_Printf("\n================================\n");
    UART_Printf("  Bootloader v%s\n", BOOTLOADER_VERSION);
    UART_Printf("================================\n");
    
    // Initialize watchdog
    Watchdog_Init();
    
    // Initialize communication
    Can_Init(CAN_500KBPS);
    Uds_Init();
}

void BootLoader_MainLoop(void) {
    uint32_t last_activity = GetTickCount();
    
    while (1) {
        // Service communication stack
        Can_MainFunction();
        Uds_MainFunction();
        
        // Check for update completion
        if (g_update_in_progress && !Flash_IsBusy()) {
            g_update_in_progress = false;
            UART_Printf("Update complete!\n");
        }
        
        // Check timeout
        if (GetTickCount() - last_activity > BOOT_TIMEOUT_MS) {
            if (BootLoader_ValidateApplication()) {
                UART_Printf("Timeout - booting application\n");
                BootLoader_JumpToApplication();
            }
            last_activity = GetTickCount();
        }
        
        // Feed watchdog
        Watchdog_Feed();
    }
}
```

---

## 12. Testing & Validation

### 12.1 Test Cases

```python
# test_bootloader.py
import can
import time

def test_bootloader_sequence():
    bus = can.interface.Bus(channel='can0', bustype='socketcan')
    
    print("Test 1: Enter bootloader mode")
    send_command(bus, 0x11, 0x01)  # ECU Reset
    time.sleep(1)
    
    print("Test 2: Request download")
    send_uds_request(bus, [0x34, 0x00, 0x20, 
                           0x08, 0x00, 0x80,  # Address
                           0x00, 0x01, 0x00]) # Size
    
    print("Test 3: Transfer data blocks")
    for i in range(100):
        data = [0x36, i] + [0xAA] * 7
        send_can(bus, 0x7DF, data)
        time.sleep(0.01)
    
    print("Test 4: Exit transfer")
    send_uds_request(bus, [0x37])
    
    print("Test 5: ECU reset")
    send_command(bus, 0x11, 0x01)
    
    print("All tests completed!")
```

### 12.2 Validation Checklist

- [ ] Boot time < 100ms
- [ ] Flash write speed > 1 KB/s
- [ ] Checksum verification correct
- [ ] Rollback works on corrupted image
- [ ] Security authentication required
- [ ] Watchdog resets on hang
- [ ] Communication timeout handled
- [ ] All error cases tested

---

## 13. Troubleshooting

### 13.1 Common Issues

| **Issue** | **Symptom** | **Solution** |
|-----------|-------------|--------------|
| Boot loop | Constant resetting | Check watchdog, power supply |
| Flash write fail | Error during programming | Verify voltage, erase first |
| Checksum mismatch | Validation fails | Check endianness, algorithm |
| Cannot enter BL | Always boots app | Check BOOT pin, flags |
| Communication fail | No response | Verify baud rate, wiring |

### 13.2 Debug Tips

```c
// Enable debug output
#define BL_DEBUG 1

#if BL_DEBUG
    #define BL_LOG(fmt, ...) UART_Printf("[BL] " fmt, ##__VA_ARGS__)
#else
    #define BL_LOG(fmt, ...)
#endif

// Log all flash operations
void Flash_LogOperation(const char* op, uint32_t addr, int result) {
    BL_LOG("Flash %s @ 0x%08X -> %d\n", op, addr, result);
}
```

---

## 14. Latihan & Studi Kasus

### 14.1 Latihan 1: Simple Bootloader

Buat bootloader dasar dengan:
- Validasi checksum sederhana
- Jump ke aplikasi
- UART debug output

### 14.2 Latihan 2: UDS Flashing

Implementasi:
- Request Download (0x34)
- Transfer Data (0x36)
- Transfer Exit (0x37)

### 14.3 Studi Kasus: Production Bootloader

Requirements:
- Secure boot dengan RSA
- Dual-bank untuk rollback
- AUTOSAR compliant
- Support multi-ECU flashing

---

## 15. Glosarium Jepang-Inggris

| **日本語** | **Romaji** | **English** | **Indonesian** |
|------------|------------|-------------|----------------|
| ブートローダ | Būtorōda | Bootloader | Bootloader |
| ファームウェア | Fāmuwea | Firmware | Firmware |
| フラッシュメモリ | Furasshu Memori | Flash Memory | Memori Flash |
| 書き込み | Kakikomi | Write | Tulis |
| 消去 | Shōkyo | Erase | Hapus |
| 検証 | Kenshō | Verification | Verifikasi |
| 更新 | Kōshin | Update | Pembaruan |
| 復旧 | Fukkyū | Recovery | Pemulihan |
| 安全性 | Anzen-sei | Safety | Keamanan |
| 認証 | Ninshō | Authentication | Autentikasi |

---

## 🔗 Referensi Silang

- **Prasyarat:** [UDS Protocol](./10_UDS_Protocol_Master.md), [C Basics](./01_C_Basics_for_Embedded.md)
- **Lanjutan:** [AUTOSAR Classic](./03_AUTOSAR_Classic_Arch.md), [ISO 26262](./06_ISO26262_Safety_Concepts.md)
- **Standar:** ISO 14229-1, SAE J2534, AUTOSAR SWS BSW

---

**File Size:** ~1500 baris  
**Last Updated:** 2024  
**Author:** Embedded C Automotive Learning Path  
**License:** Educational Use Only
