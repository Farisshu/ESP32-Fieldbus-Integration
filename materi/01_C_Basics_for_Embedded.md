# Dasar-Dasar Bahasa C untuk Sistem Embedded

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan bab ini, Anda akan mampu:
- Memahami konsep dasar bahasa C untuk embedded system
- Mengetahui perbedaan antara C standar dan C untuk embedded
- Menguasai konsep memory management di lingkungan resource constrained
- Menerapkan best practices dalam penulisan kode embedded C

## 📋 Daftar Isi

1. [Variabel dan Tipe Data](#1-variabel-dan-tipe-data)
2. [Pointer dan Array](#2-pointer-dan-array)
3. [Struktur dan Union](#3-struktur-dan-union)
4. [Bit Manipulation](#4-bit-manipulation)
5. [Preprocessor Directives](#5-preprocessor-directives)
6. [Static vs Global Variables](#6-static-vs-global-variables)
7. [Interrupt Service Routines ISR](#7-interrupt-service-routines-isr)
8. [Memory Management](#8-memory-management)
9. [Best Practices](#9-best-practices)
10. [Latihan](#10-latihan)

---

## 1. Variabel dan Tipe Data

Dalam sistem embedded, ukuran data sangat penting karena keterbatasan memori. Setiap byte berharga!

### Tipe Data Standar C

| Tipe Data | Ukuran (bit) | Range | Penggunaan |
|-----------|--------------|-------|------------|
| `char` | 8 | -128 to 127 | Karakter, flag kecil |
| `unsigned char` / `uint8_t` | 8 | 0 to 255 | Sensor data, status flags |
| `short` / `int16_t` | 16 | -32,768 to 32,767 | ADC readings, PWM values |
| `unsigned short` / `uint16_t` | 16 | 0 to 65,535 | Timer counts, frequencies |
| `int` / `int32_t` | 32 | ±2×10⁹ | Calculations, counters |
| `unsigned int` / `uint32_t` | 32 | 0 to 4×10⁹ | Timestamps, large counters |
| `float` | 32 | ±3.4×10³⁸ | Analog measurements |
| `double` | 64 | ±1.7×10³⁰⁸ | High precision (jarang digunakan) |

### Contoh Penggunaan

```c
#include <stdint.h>

// Sensor data - gunakan unsigned karena tidak ada nilai negatif
uint8_t temperature_sensor;      // 0-255°C
uint16_t adc_reading;            // 0-65535 (16-bit ADC)

// Status flags - hemat memori dengan menggunakan bit field
typedef struct {
    uint8_t engine_running : 1;
    uint8_t brake_pressed : 1;
    uint8_t door_open : 1;
    uint8_t reserved : 5;        // Reserved untuk ekspansi masa depan
} VehicleStatusType;

// Fixed-width types untuk portabilitas
int32_t calculate_distance(int32_t start, int32_t end);
uint16_t read_adc_channel(uint8_t channel);
```

### ⚠️ Common Mistakes

```c
// ❌ SALAH - Tidak portable, ukuran int bisa berbeda
int sensor_value;

// ✅ BENAR - Ukuran pasti, portable ke semua platform
int16_t sensor_value;  // Selalu 16 bit
```

---

## 2. Pointer dan Array

Pointer adalah konsep paling penting (dan paling menantang) dalam embedded C.

### Konsep Dasar Pointer

```c
#include <stdint.h>

uint8_t variable = 0x55;      // Variabel biasa
uint8_t *ptr = &variable;     // Pointer ke variable

// Akses nilai melalui pointer
*ptr = 0xAA;                  // variable sekarang = 0xAA

// Pointer arithmetic
uint8_t array[5] = {1, 2, 3, 4, 5};
uint8_t *arr_ptr = array;     // arr_ptr pointing to array[0]

arr_ptr++;                    // Sekarang pointing to array[1]
*(arr_ptr + 2) = 10;          // Set array[3] = 10
```

### Pointer ke Register Hardware

```c
// Definisi register addresses (contoh untuk ARM Cortex-M)
#define GPIOA_BASE    0x40020000UL
#define GPIOA_MODER   (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t*)(GPIOA_BASE + 0x14))
#define GPIOA_IDR     (*(volatile uint32_t*)(GPIOA_BASE + 0x10))

// Fungsi untuk konfigurasi GPIO
void GPIO_Init(void) {
    // Set PA5 sebagai output (mode 01)
    GPIOA_MODER &= ~(0x03 << (5 * 2));  // Clear bits
    GPIOA_MODER |= (0x01 << (5 * 2));   // Set as output
    
    // Toggle LED
    GPIOA_ODR ^= (1 << 5);
}

// Baca input dari pin
uint8_t GPIO_ReadPin(uint8_t pin) {
    return (GPIOA_IDR & (1 << pin)) ? 1 : 0;
}
```

### Array vs Pointer

```c
// Array - alokasi statis
uint8_t buffer_static[256];     // Selalu menggunakan 256 bytes

// Pointer - bisa dinamis atau static
uint8_t *buffer_ptr;            // Hanya 4 bytes untuk pointer

// Access pattern yang sama
buffer_static[0] = 0x55;
buffer_ptr[0] = 0x55;

// Tapi array name adalah constant pointer
// buffer_static = other_array;  // ❌ ERROR!
// buffer_ptr = other_array;     // ✅ OK
```

---

## 3. Struktur dan Union

### Struktur (struct)

Digunakan untuk mengelompokkan data terkait.

```c
typedef struct {
    uint16_t rpm;              // Engine RPM (0-65535)
    uint8_t gear;              // Current gear (1-6)
    float temperature;         // Engine temperature
    uint8_t status_flags;      // Status bits
} EngineDataType;

// Penggunaan
EngineDataType engine_data;
engine_data.rpm = 3000;
engine_data.gear = 4;
engine_data.temperature = 85.5;
engine_data.status_flags = 0x01;

// Pointer ke struct
EngineDataType *engine_ptr = &engine_data;
engine_ptr->rpm = 3500;        // Arrow operator untuk pointer
```

### Union

Digunakan untuk hemat memori ketika beberapa variabel tidak digunakan bersamaan.

```c
typedef union {
    uint32_t raw_value;        // Akses sebagai 32-bit value
    struct {
        uint8_t byte0;         // Akses per byte
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    } bytes;
    struct {
        uint16_t word0;        // Akses per word
        uint16_t word1;
    } words;
    struct {
        uint32_t value : 24;   // Bit field 24-bit
        uint32_t status : 4;   // 4-bit status
        uint32_t reserved : 4; // Padding
    } fields;
} SensorDataUnion;

// Penggunaan
SensorDataUnion sensor;
sensor.raw_value = 0x12345678;

// Semua akses berikut mengacu pada memory yang sama
printf("Byte0: 0x%02X\n", sensor.bytes.byte0);  // 0x78
printf("Word0: 0x%04X\n", sensor.words.word0);  // 0x5678
```

### Struct dengan Bit Fields

```c
// CAN Message structure dengan bit fields
typedef struct {
    uint32_t id : 11;          // Standard CAN ID (11 bits)
    uint32_t ide : 1;          // Identifier Extension bit
    uint32_t rtr : 1;          // Remote Transmission Request
    uint32_t dlc : 4;          // Data Length Code (0-8)
    uint32_t reserved : 15;    // Padding
    uint8_t data[8];           // Data payload
} CanMessageStdType;

// Extended CAN Message
typedef struct {
    uint32_t id : 29;          // Extended CAN ID (29 bits)
    uint32_t ide : 1;
    uint32_t rtr : 1;
    uint32_t dlc : 4;
    uint32_t reserved : 13;
    uint8_t data[8];
} CanMessageExtType;
```

---

## 4. Bit Manipulation

Operasi bit adalah skill fundamental dalam embedded programming.

### Operasi Dasar

```c
#define SET_BIT(reg, bit)     ((reg) |= (1U << (bit)))
#define CLR_BIT(reg, bit)     ((reg) &= ~(1U << (bit)))
#define TOG_BIT(reg, bit)     ((reg) ^= (1U << (bit)))
#define GET_BIT(reg, bit)     (((reg) >> (bit)) & 1U)

#define SET_BITS(reg, mask, val)  ((reg) = (((reg) & ~(mask)) | ((val) & (mask))))
#define GET_BITS(reg, mask, pos)  (((reg) & (mask)) >> (pos))

// Contoh penggunaan
volatile uint32_t *control_reg = (volatile uint32_t*)0x40021000;

SET_BIT(*control_reg, 5);      // Set bit 5
CLR_BIT(*control_reg, 3);      // Clear bit 3
TOG_BIT(*control_reg, 7);      // Toggle bit 7

if(GET_BIT(*control_reg, 2)) { // Check if bit 2 is set
    // Bit 2 is high
}

// Set multiple bits
SET_BITS(*control_reg, 0x0F00, 0x0500);  // Set bits 8-11 to value 5
```

### Masking dan Shifting

```c
// Extract specific bits from ADC value
uint16_t adc_raw = 0xABCD;

// Extract upper 4 bits
uint8_t upper_nibble = (adc_raw >> 12) & 0x0F;  // 0xA

// Extract middle 8 bits
uint8_t middle_byte = (adc_raw >> 4) & 0xFF;    // 0xBC

// Extract lower 4 bits
uint8_t lower_nibble = adc_raw & 0x0F;          // 0xD

// Pack multiple values into single word
uint16_t packed = ((uint16_t)sensor_id << 12) | 
                  ((uint16_t)channel << 8) | 
                  (value & 0xFF);
```

### Practical Example: Register Configuration

```c
// Timer configuration register
typedef struct {
    uint32_t enable : 1;       // Bit 0: Timer enable
    uint32_t mode : 2;         // Bit 1-2: Timer mode
    uint32_t prescaler : 4;    // Bit 3-6: Prescaler value
    uint32_t interrupt : 1;    // Bit 7: Interrupt enable
    uint32_t reserved : 24;    // Bit 8-31: Reserved
} TimerConfigType;

void Timer_Init(uint8_t prescaler_value) {
    volatile uint32_t *timer_ctrl = (volatile uint32_t*)0x40001000;
    
    // Disable timer first
    CLR_BIT(*timer_ctrl, 0);
    
    // Configure prescaler (bits 3-6)
    *timer_ctrl &= ~(0x0F << 3);           // Clear prescaler bits
    *timer_ctrl |= (prescaler_value << 3); // Set new prescaler
    
    // Set mode to continuous (mode = 1)
    *timer_ctrl &= ~(0x03 << 1);           // Clear mode bits
    *timer_ctrl |= (0x01 << 1);            // Set continuous mode
    
    // Enable interrupt
    SET_BIT(*timer_ctrl, 7);
    
    // Enable timer
    SET_BIT(*timer_ctrl, 0);
}
```

---

## 5. Preprocessor Directives

Preprocessor directives diproses sebelum kompilasi.

### Macros

```c
// Simple macro
#define LED_PIN           5
#define MAX_BUFFER_SIZE   256U
#define PI                3.14159265359f

// Function-like macro
#define MIN(a, b)         (((a) < (b)) ? (a) : (b))
#define MAX(a, b)         (((a) > (b)) ? (a) : (b))
#define ABS(x)            (((x) < 0) ? -(x) : (x))

// Macro dengan side effects - HATI-HATI!
#define SQUARE(x)         ((x) * (x))

// Penggunaan
int a = 5, b = 10;
int min_val = MIN(a, b);        // Expands to: (((5) < (10)) ? (5) : (10))
int squared = SQUARE(a + 1);    // Expands to: ((a + 1) * (a + 1)) = 36

// ⚠️ Bahaya: SQUARE(a++) expands to ((a++) * (a++)) - increment twice!
```

### Conditional Compilation

```c
// Debug vs Release build
#ifdef DEBUG
    #define DBG_PRINT(fmt, ...) printf("[DBG] " fmt "\n", ##__VA_ARGS__)
    #define DBG_ASSERT(cond) if(!(cond)) { printf("ASSERT FAILED\n"); while(1); }
#else
    #define DBG_PRINT(fmt, ...) ((void)0)
    #define DBG_ASSERT(cond) ((void)0)
#endif

// Platform-specific code
#if defined(STM32F4)
    #define MCU_CLOCK_FREQ    168000000UL
    #define FLASH_SIZE        1024U
#elif defined(STM32F1)
    #define MCU_CLOCK_FREQ    72000000UL
    #define FLASH_SIZE        512U
#elif defined(AVR_ATMEGA328)
    #define MCU_CLOCK_FREQ    16000000UL
    #define FLASH_SIZE        32U
#else
    #error "Unsupported MCU"
#endif

// Feature flags
#define FEATURE_CAN_ENABLED       1
#define FEATURE_LIN_ENABLED       0
#define FEATURE_UDS_ENABLED       1

#if FEATURE_CAN_ENABLED
    void CAN_Init(void);
    void CAN_Transmit(uint32_t id, uint8_t *data, uint8_t len);
#endif
```

### Include Guards

```c
// sensor_interface.h
#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

#include <stdint.h>

// Function declarations
uint16_t Sensor_ReadTemperature(void);
uint16_t Sensor_ReadPressure(void);
void Sensor_Init(void);

#endif // SENSOR_INTERFACE_H
```

---

## 6. Static vs Global Variables

### Storage Classes

```c
// Global variable - accessible dari semua file
int global_counter = 0;

// Static global - hanya accessible di file ini
static int file_local_counter = 0;

// Static local - persistent antar pemanggilan fungsi
void increment_counter(void) {
    static int call_count = 0;  // Initialized once, persists
    call_count++;
    printf("Function called %d times\n", call_count);
}

// Auto (default) - created/destroyed setiap call
void auto_example(void) {
    int auto_var = 0;  // Created and initialized each call
    auto_var++;
    printf("Auto var: %d\n", auto_var);  // Always prints 1
}
```

### Best Practices

```c
// ❌ BAD - Global variable tanpa kontrol akses
int critical_data;

// ✅ GOOD - Encapsulation dengan getter/setter
static int protected_data = 0;  // Static, file-local

int get_protected_data(void) {
    return protected_data;
}

void set_protected_data(int value) {
    // Validation bisa ditambahkan di sini
    if(value >= 0 && value <= 100) {
        protected_data = value;
    }
}
```

---

## 7. Interrupt Service Routines (ISR)

ISR harus cepat, deterministik, dan tidak blocking.

### ISR Basics

```c
#include <stdint.h>
#include "mcu_interrupts.h"

// Volatile untuk variables yang diakses dari ISR
volatile uint8_t rx_buffer[256];
volatile uint16_t rx_index = 0;
volatile uint8_t rx_complete_flag = 0;

// UART Receive ISR
void USART1_RX_IRQHandler(void) {
    // Check if RXNE (Receive Not Empty) flag is set
    if(USART1_SR & USART_SR_RXNE) {
        // Read data register (clears RXNE flag)
        uint8_t data = USART1_DR;
        
        // Store in buffer
        if(rx_index < 256) {
            rx_buffer[rx_index++] = data;
        } else {
            // Buffer overflow - handle error
            rx_index = 0;
        }
    }
    
    // Check for overrun error
    if(USART1_SR & USART_SR_ORE) {
        // Clear error flag
        uint8_t dummy = USART1_DR;
        (void)dummy;
    }
}

// Main loop polling for complete message
int main(void) {
    USART1_Init(115200);
    NVIC_EnableIRQ(USART1_RX_IRQn);
    
    while(1) {
        if(rx_complete_flag) {
            rx_complete_flag = 0;
            ProcessReceivedData(rx_buffer, rx_index);
            rx_index = 0;
        }
    }
}
```

### ISR Best Practices

```c
// ✅ DO - Keep ISR short
volatile uint8_t button_pressed = 0;

void EXTI0_IRQHandler(void) {
    button_pressed = 1;  // Set flag
    EXTI_ClearITPendingBit(EXTI_Line0);  // Clear interrupt
}

// ❌ DON'T - Long operations in ISR
void BAD_EXTI0_IRQHandler(void) {
    button_pressed = 1;
    
    // Don't do this in ISR!
    Delay_ms(100);  // Blocking!
    printf("Button pressed\n");  // Slow I/O
    ProcessComplexAlgorithm();  // Time-consuming
}

// ✅ GOOD - Defer processing to main loop
void GOOD_EXTI0_IRQHandler(void) {
    button_pressed = 1;
    EXTI_ClearITPendingBit(EXTI_Line0);
}

int main(void) {
    while(1) {
        if(button_pressed) {
            button_pressed = 0;
            // Do complex processing here
            ProcessButtonPress();
        }
    }
}
```

---

## 8. Memory Management

### Stack vs Heap

```c
// Stack allocation - automatic, fast, limited
void stack_example(void) {
    uint8_t local_buffer[256];  // On stack
    // Automatically freed when function returns
}

// Heap allocation - manual, slower, fragmentation risk
void heap_example(void) {
    uint8_t *dynamic_buffer = malloc(256);
    if(dynamic_buffer != NULL) {
        // Use buffer
        free(dynamic_buffer);  // Must free manually!
    }
}

// Static allocation - predictable, no fragmentation
static uint8_t global_buffer[256];  // In .bss section
```

### Memory Sections

```c
// .text section - code
void my_function(void) {
    // Code here
}

// .data section - initialized globals
int initialized_var = 42;

// .bss section - uninitialized globals
int uninitialized_var;  // Automatically zero-initialized

// .rodata section - constants
const int constant_value = 100;
const char message[] = "Hello World";

// Custom sections (compiler-specific)
#pragma DATA_SECTION(slow_data, ".slow_ram")
uint8_t slow_data[256];

#pragma CODE_SECTION(fast_func, ".fast_flash")
void fast_func(void) {
    // Critical timing function
}
```

### Memory Pool Implementation

```c
// Fixed-size memory pool untuk real-time systems
#define POOL_BLOCK_SIZE   64
#define POOL_BLOCK_COUNT  32

typedef struct {
    uint8_t data[POOL_BLOCK_SIZE];
    uint8_t free;
} MemoryBlockType;

static MemoryBlockType memory_pool[POOL_BLOCK_COUNT];

void* MemoryPool_Allocate(void) {
    for(int i = 0; i < POOL_BLOCK_COUNT; i++) {
        if(memory_pool[i].free) {
            memory_pool[i].free = 0;
            return memory_pool[i].data;
        }
    }
    return NULL;  // Pool exhausted
}

void MemoryPool_Free(void *ptr) {
    if(ptr == NULL) return;
    
    // Find block and mark as free
    for(int i = 0; i < POOL_BLOCK_COUNT; i++) {
        if(memory_pool[i].data == ptr) {
            memory_pool[i].free = 1;
            return;
        }
    }
}
```

---

## 9. Best Practices

### Naming Conventions

```c
// Types - PascalCase with _t suffix
typedef struct { ... } EngineDataType;
typedef enum { ... } StateMachineType;

// Functions - PascalCase or snake_case
void Engine_Init(void);
uint16_t ADC_ReadChannel(uint8_t channel);

// Variables - snake_case
uint16_t sensor_value;
int32_t temperature_offset;

// Constants - UPPER_CASE
#define MAX_TEMPERATURE    150U
#define MIN_VOLTAGE        3.3f

// Pointers - indicate with 'Ptr' suffix
uint8_t *dataPtr;
EngineDataType *enginePtr;
```

### Error Handling

```c
typedef enum {
    E_OK = 0,
    E_NOT_OK = 1,
    E_PARAM_NULL = 2,
    E_PARAM_RANGE = 3,
    E_TIMEOUT = 4,
    E_BUSY = 5
} Std_ReturnType;

Std_ReturnType Sensor_Init(SensorConfigType *config) {
    if(config == NULL) {
        return E_PARAM_NULL;
    }
    
    if(config->sampling_rate > MAX_SAMPLING_RATE) {
        return E_PARAM_RANGE;
    }
    
    // Initialization code
    if(!Hardware_Check()) {
        return E_TIMEOUT;
    }
    
    return E_OK;
}

// Usage
Std_ReturnType result = Sensor_Init(&sensor_config);
if(result != E_OK) {
    Error_Handler(result);
}
```

### Documentation

```c
/**
 * @brief Calculate engine torque based on RPM and load
 * 
 * @param rpm Engine speed in RPM (0-8000)
 * @param load Engine load percentage (0-100)
 * @return Calculated torque in Nm
 * 
 * @note This function uses lookup table for interpolation
 * @warning Returns 0 if parameters out of range
 */
float Engine_CalculateTorque(uint16_t rpm, uint8_t load) {
    // Implementation
}
```

---

## 10. Latihan

### Exercise 1: Bit Manipulation
Buat fungsi untuk:
1. Set bit tertentu dalam register
2. Clear bit tertentu
3. Toggle bit
4. Extract bit field

### Exercise 2: Struct dan Union
Buat struktur untuk:
1. CAN message dengan ID, DLC, dan data
2. Sensor data dengan multiple access methods menggunakan union
3. Config struct dengan bit fields

### Exercise 3: Pointer Arithmetic
Implementasikan:
1. Circular buffer menggunakan pointer
2. String manipulation functions
3. Array sorting dengan pointer

### Exercise 4: ISR Design
Design ISR untuk:
1. UART receive dengan ring buffer
2. Timer interrupt dengan state machine
3. External interrupt dengan debouncing

---

## 🔗 Next Steps

Setelah menguasai dasar-dasar ini, lanjutkan ke:
- [Arsitektur Mikrokontroler](./02_Microcontroller_Arch.md)
- [MISRA C Style Guide](./05_MISRA_C_Style_Guide.md)
- [Embedded OS Basics](./07_Embedded_OS_Basics.md)

## 📚 Referensi

- "The C Programming Language" by K&R
- "Embedded C Coding Standard" by Barr Group
- MISRA C:2012 Guidelines
- ARM Cortex-M Technical Reference Manual

---

**Last Updated**: 2024  
**Difficulty**: Beginner  
**Estimated Time**: 2-3 weeks
