# Day 02 - Embedded C Programming

> **"C is the language of embedded systems. Master it, and you master the machine."**

---

## Tujuan Pembelajaran

Setelah mempelajari materi ini, Anda akan mampu:

- Memahami konsep pointer dan memory management dalam embedded context
- Menggunakan struct, enum, dan type system dengan efektif
- Menerapkan storage class specifiers (const, volatile, static, extern)
- Memahami memory layout embedded systems
- Mengimplementasikan interrupt callback dengan benar
- Menulis kode yang MISRA C compliant
- Mengikuti coding style industri

---

## Konsep Dasar

### Mengapa Embedded C Berbeda?

Embedded C bukan sekadar "C yang jalan di microcontroller". Ada constraints dan requirements khusus:

```
┌─────────────────────────────────────────────────────────┐
│           EMBEDDED C CONSTRAINTS                        │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  • Limited Memory (KB, bukan GB)                        │
│  • No OS / RTOS (atau minimal)                          │
│  • Real-time requirements                               │
│  • Direct hardware access                               │
│  • Power constraints                                    │
│  • Safety & Reliability critical                        │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## Penjelasan Detail

### 1. Pointer dalam Embedded Context

#### Apa Itu Pointer?

Pointer adalah variabel yang menyimpan **alamat memory**, bukan nilai.

```c
int value = 42;
int *ptr = &value;  // ptr menyimpan alamat dari value

// Akses value melalui pointer
*ptr = 100;  // value sekarang = 100
```

#### Visualisasi Memory

```
┌─────────────────────────────────────────────────────────┐
│                    MEMORY LAYOUT                        │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Address    Variable    Value                           │
│  0x20000000  value      42                              │
│  0x20000004  ptr        0x20000000  ──────────────┐    │
│                                            │      │    │
│                                            └──────┘    │
│                                             Points to  │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Pointer untuk Hardware Access

Ini adalah penggunaan pointer paling penting di embedded:

```c
// Define register address (contoh: GPIO port A output data)
#define GPIOA_ODR    (*(volatile uint32_t*)0x48000014)

// Set pin 5 high
GPIOA_ODR |= (1 << 5);

// Kenapa volatile? Karena hardware bisa ubah nilai ini kapan saja
```

#### Pointer Arithmetic untuk DMA Buffer

```c
uint8_t dma_buffer[256];
uint8_t *buffer_ptr = dma_buffer;

// Fill buffer
for (int i = 0; i < 256; i++) {
    *buffer_ptr++ = i;  // Increment pointer setelah assign
}

// Reset pointer
buffer_ptr = dma_buffer;
```

### 2. Struct untuk Data Organization

#### Basic Struct

```c
typedef struct {
    float temperature;
    float humidity;
    uint32_t timestamp;
} SensorData_t;

// Usage
SensorData_t sensor1;
sensor1.temperature = 25.5f;
sensor1.humidity = 60.0f;
sensor1.timestamp = HAL_GetTick();
```

#### Struct dengan Bit Fields (untuk Register Mapping)

```c
typedef struct {
    uint32_t EN     : 1;   // Bit 0: Enable
    uint32_t MODE   : 2;   // Bit 1-2: Mode selection
    uint32_t SPEED  : 2;   // Bit 3-4: Speed setting
    uint32_t PUPD   : 2;   // Bit 5-6: Pull-up/pull-down
    uint32_t OTYPE  : 1;   // Bit 7: Output type
    uint32_t RESERVED : 24; // Padding
} GPIO_Config_Register_t;

// Access individual bits
GPIO_Config_Register_t *gpio_reg = 
    (GPIO_Config_Register_t*)0x48000000;

gpio_reg->EN = 1;      // Set enable bit
gpio_reg->MODE = 0x01; // Set mode
```

⚠️ **Warning:** Bit fields implementation-dependent. Untuk production code, gunakan bit manipulation eksplisit.

#### Struct untuk Protocol Messages

```c
typedef struct __attribute__((packed)) {
    uint8_t  header;
    uint16_t length;
    uint8_t  command;
    uint8_t  data[32];
    uint16_t checksum;
} CommunicationPacket_t;

// __attribute__((packed)) mencegah padding
// Penting untuk protocol binary
```

### 3. Enum untuk Type Safety

#### Basic Enum

```c
typedef enum {
    STATE_OFF,
    STATE_ON,
    STATE_STANDBY,
    STATE_ERROR
} DeviceState_t;

DeviceState_t currentState = STATE_OFF;

// Lebih baik daripada magic numbers!
if (currentState == STATE_ERROR) {
    // Handle error
}
```

#### Enum dengan Explicit Values

```c
typedef enum {
    UART_BAUD_9600   = 9600,
    UART_BAUD_19200  = 19200,
    UART_BAUD_115200 = 115200
} UART_BaudRate_t;

typedef enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1 = 1,
    ADC_CHANNEL_2 = 2,
    ADC_CHANNEL_VREFINT = 17,
    ADC_CHANNEL_TEMPSENSOR = 18
} ADC_Channel_t;
```

### 4. Storage Class Specifiers

#### const - Data yang Tidak Berubah

```c
// Compile-time constant
const int MAX_RETRIES = 5;

// Constant lookup table
const uint16_t sine_table[256] = {
    2048, 2099, 2150, /* ... */
};

// Pointer to constant data (data tidak bisa diubah via pointer)
const char *error_messages[] = {
    "OK",
    "ERROR_TIMEOUT",
    "ERROR_INVALID_PARAM"
};

// Constant pointer (pointer tidak bisa diubah)
int value = 10;
int *const ptr = &value;  // ptr selalu point ke value
*ptr = 20;  // OK
// ptr = &other;  // ERROR!

// Constant pointer to constant data
const int *const cptr = &value;  // Keduanya const
```

#### volatile - Data yang Bisa Berubah Kapan Saja

```c
// Hardware register
volatile uint32_t *status_reg = (uint32_t*)0x40000000;

// Interrupt-shared variable
volatile bool data_ready = false;

// Why volatile matters:
// Tanpa volatile, compiler mungkin optimize:

// ❌ WITHOUT VOLATILE
bool flag = shared_flag;
if (flag) {
    // Compiler assume flag tidak berubah
    // Bisa cache di register
}

// ✅ WITH VOLATILE
volatile bool flag = shared_flag;
if (flag) {
    // Compiler MUST read from memory setiap kali
    // Tidak boleh cache
}
```

**Kapan pakai volatile?**
- Memory-mapped hardware registers
- Variables shared antara main code dan ISR
- Variables shared antara tasks (RTOS)
- Global variables yang di-modify oleh DMA

#### static - Scope dan Lifetime Control

```c
// Static global (file scope only)
static int internal_counter = 0;  // Hanya accessible di file ini

// Static local (persist across function calls)
void increment_counter(void) {
    static int count = 0;  // Initialized once, persists
    count++;
    printf("Count: %d\n", count);
}

// Static function (internal linkage)
static void helper_function(void) {
    // Hanya callable dari file ini
}

// Why use static?
// 1. Encapsulation (hide internal implementation)
// 2. Prevent naming conflicts
// 3. Reduce memory (single instance)
```

#### extern - Cross-File Variables

```c
// file1.c
int global_config = 0;  // Definition

// file2.c
extern int global_config;  // Declaration (not definition!)

void use_config(void) {
    global_config = 5;  // OK, refers to variable in file1.c
}
```

**Best Practice:**
```c
// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

extern int global_config;  // Declaration only
extern void System_Init(void);

#endif

// globals.c
#include "globals.h"

int global_config = 0;  // Single definition

void System_Init(void) {
    // Implementation
}
```

### 5. Function Pointer untuk Callbacks

#### Basic Function Pointer

```c
// Function pointer type definition
typedef void (*CallbackFunc_t)(int result);

// Function that matches signature
void OnComplete(int result) {
    printf("Completed with result: %d\n", result);
}

// Usage
CallbackFunc_t callback = OnComplete;
callback(42);  // Calls OnComplete(42)
```

#### Callback Table untuk State Machine

```c
typedef void (*StateHandler_t)(void);

void State_Idle(void) { /* ... */ }
void State_Running(void) { /* ... */ }
void State_Error(void) { /* ... */ }

// Lookup table
const StateHandler_t state_handlers[] = {
    [STATE_IDLE] = State_Idle,
    [STATE_RUNNING] = State_Running,
    [STATE_ERROR] = State_Error
};

// Execute current state handler
state_handlers[currentState]();
```

#### Interrupt Callback Pattern

```c
// Define callback type
typedef void (*GPIO_Callback_t)(void);

// Store callbacks (array for multiple pins)
static GPIO_Callback_t gpio_callbacks[16] = {NULL};

// Register callback
void GPIO_RegisterCallback(uint8_t pin, GPIO_Callback_t callback) {
    if (pin < 16) {
        gpio_callbacks[pin] = callback;
    }
}

// ISR calls callback
void EXTI_IRQHandler(void) {
    for (int i = 0; i < 16; i++) {
        if (EXTI_GetITStatus(i)) {
            if (gpio_callbacks[i] != NULL) {
                gpio_callbacks[i]();  // Call user callback
            }
            EXTI_ClearITPendingBit(i);
        }
    }
}
```

### 6. Memory Layout Embedded Systems

```
┌─────────────────────────────────────────────────────────┐
│          STM32 MEMORY MAP                               │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  0x08000000  ┌─────────────────┐                       │
│              │   FLASH         │ ← Code, const data    │
│              │   (512KB)       │                       │
│              ├─────────────────┤                       │
│  0x20000000  │   SRAM          │ ← Variables, stack    │
│              │   (64KB)        │    heap               │
│              │                 │                       │
│              │  ┌──────────┐   │                       │
│              │  │ .bss     │   │ ← Uninitialized vars  │
│              │  ├──────────┤   │                       │
│              │  │ .data    │   │ ← Initialized globals │
│              │  ├──────────┤   │                       │
│              │  │ Heap ↑   │   │ ← Dynamic allocation  │
│              │  ├──────────┤   │                       │
│              │  │   ↓ Stack│   │ ← Local vars, calls   │
│              │  └──────────┘   │                       │
│              └─────────────────┘                       │
│                                                         │
│  0x40000000  ┌─────────────────┐                       │
│              │   Peripherals   │ ← Registers           │
│              │   (MMIO)        │                       │
│              └─────────────────┘                       │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Memory Sections

```c
// .text (Flash): Code
void myFunction(void) { /* ... */ }

// .rodata (Flash): Read-only data
const int max_value = 100;

// .data (SRAM, initialized from Flash)
int initialized_var = 42;

// .bss (SRAM, zero-initialized)
int zero_initialized;  // or = 0

// Stack: Local variables
void func(void) {
    int local_var;  // On stack
}
```

### 7. Coding Style & MISRA C Overview

#### MISRA C Principles

MISRA (Motor Industry Software Reliability Association) guidelines untuk safety-critical code:

**Key Rules:**

1. **No dynamic memory allocation**
   ```c
   // ❌ MISRA violation
   int *ptr = malloc(sizeof(int));
   
   // ✅ MISRA compliant
   static int buffer[100];
   ```

2. **Fixed-width types**
   ```c
   // ❌ Ambiguous size
   int counter;
   long value;
   
   // ✅ Clear size
   int32_t counter;
   int64_t value;
   ```

3. **No implicit type conversions**
   ```c
   // ❌ Implicit conversion
   int16_t a = 1000;
   int8_t b = a;  // Potential overflow
   
   // ✅ Explicit cast
   int8_t b = (int8_t)a;  // At least documented
   ```

4. **Single return per function**
   ```c
   // ❌ Multiple returns
   int check(int x) {
       if (x < 0) return -1;
       if (x > 100) return -2;
       return 0;
   }
   
   // ✅ Single return
   int check(int x) {
       int result = 0;
       if (x < 0) {
           result = -1;
       } else if (x > 100) {
           result = -2;
       }
       return result;
   }
   ```

5. **All switch cases have break/default**
   ```c
   switch(state) {
       case STATE_A:
           // ...
           break;
       case STATE_B:
           // ...
           break;
       default:
           Error_Handler();
           break;
   }
   ```

#### Naming Conventions

```c
// Types end with _t
typedef struct { /* ... */ } SensorData_t;

// Constants in UPPER_SNAKE_CASE
#define MAX_BUFFER_SIZE 256
const int TIMEOUT_MS = 1000;

// Functions in PascalCase or snake_case
void System_Init(void);
void process_data(void);

// Variables in snake_case
int sensor_value;
uint32_t tick_count;

// Private/static functions with prefix
static void prv_ProcessData(void);
```

---

## Contoh Kode Lengkap

```c
/**
 * @file sensor_manager.c
 * @brief Example demonstrating Embedded C best practices
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Type Definitions */
typedef enum {
    SENSOR_OK = 0,
    SENSOR_ERROR_TIMEOUT,
    SENSOR_ERROR_INVALID,
    SENSOR_ERROR_NOT_READY
} SensorStatus_t;

typedef struct {
    float temperature;
    float humidity;
    uint32_t timestamp;
    bool is_valid;
} SensorReading_t;

/* Constants */
#define SENSOR_MAX_READINGS    10U
#define SENSOR_TIMEOUT_MS      1000U
#define SENSOR_MIN_TEMP       -40.0f
#define SENSOR_MAX_TEMP        85.0f

/* Private Variables */
static SensorReading_t readings_buffer[SENSOR_MAX_READINGS];
static uint8_t read_index = 0U;
static volatile bool conversion_complete = false;

/* Private Function Prototypes */
static SensorStatus_t prv_ValidateReading(const SensorReading_t *reading);
static void prv_StoreReading(const SensorReading_t *reading);

/**
 * @brief Callback from ADC interrupt
 * @note Called from ISR context - keep it short!
 */
void ADC_ConversionComplete_Callback(void) {
    conversion_complete = true;
}

/**
 * @brief Read sensor with timeout
 * @param reading Pointer to store result
 * @param timeout_ms Maximum wait time
 * @return SensorStatus_t
 */
SensorStatus_t Sensor_Read(SensorReading_t *reading, uint32_t timeout_ms) {
    SensorStatus_t status = SENSOR_OK;
    uint32_t start_time;
    
    /* Parameter validation */
    if (reading == NULL) {
        status = SENSOR_ERROR_INVALID;
        goto cleanup;
    }
    
    /* Reset flag */
    conversion_complete = false;
    
    /* Start ADC conversion (hardware-specific) */
    ADC_StartConversion();
    
    /* Wait for completion with timeout */
    start_time = HAL_GetTick();
    
    while (!conversion_complete) {
        if ((HAL_GetTick() - start_time) > timeout_ms) {
            status = SENSOR_ERROR_TIMEOUT;
            goto cleanup;
        }
        /* Could add low-power sleep here */
    }
    
    /* Read raw values and convert */
    reading->temperature = ADC_GetTemperature();
    reading->humidity = ADC_GetHumidity();
    reading->timestamp = HAL_GetTick();
    
    /* Validate */
    status = prv_ValidateReading(reading);
    
    if (status == SENSOR_OK) {
        reading->is_valid = true;
        prv_StoreReading(reading);
    } else {
        reading->is_valid = false;
    }
    
cleanup:
    return status;
}

/**
 * @brief Get historical readings
 * @param count Number of readings to retrieve
 * @param buffer Output buffer (must be at least count elements)
 * @return Number of readings returned
 */
uint8_t Sensor_GetHistory(uint8_t count, SensorReading_t *buffer) {
    uint8_t actual_count;
    
    if (buffer == NULL) {
        return 0U;
    }
    
    /* Limit to available data */
    actual_count = (count < read_index) ? count : read_index;
    
    /* Copy data (thread-safe if needed) */
    memcpy(buffer, readings_buffer, actual_count * sizeof(SensorReading_t));
    
    return actual_count;
}

/* Private Functions */

static SensorStatus_t prv_ValidateReading(const SensorReading_t *reading) {
    SensorStatus_t status = SENSOR_OK;
    
    if (reading->temperature < SENSOR_MIN_TEMP ||
        reading->temperature > SENSOR_MAX_TEMP) {
        status = SENSOR_ERROR_INVALID;
    }
    
    /* Add more validation as needed */
    
    return status;
}

static void prv_StoreReading(const SensorReading_t *reading) {
    readings_buffer[read_index] = *reading;
    read_index++;
    
    /* Circular buffer wrap-around */
    if (read_index >= SENSOR_MAX_READINGS) {
        read_index = 0U;
    }
}
```

---

## Kesalahan yang Sering Terjadi

### 1. Pointer tanpa Initialization

```c
// ❌ DANGEROUS!
int *ptr;
*ptr = 42;  // Undefined behavior!

// ✅ Safe
int *ptr = NULL;
if (ptr != NULL) {
    *ptr = 42;
}
```

### 2. Missing volatile untuk Shared Variables

```c
// ❌ Bug waiting to happen
bool data_ready = false;

void ISR(void) {
    data_ready = true;
}

void main_loop(void) {
    while (!data_ready) {  // Compiler might optimize this away!
        // Wait
    }
}

// ✅ Correct
volatile bool data_ready = false;
```

### 3. Buffer Overflow

```c
// ❌ No bounds checking
char buffer[10];
strcpy(buffer, user_input);  // What if input > 9 chars?

// ✅ With bounds checking
char buffer[10];
strncpy(buffer, user_input, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination
```

### 4. Integer Overflow

```c
// ❌ Silent overflow
uint16_t a = 60000;
uint16_t b = 10000;
uint16_t c = a + b;  // Wraps around!

// ✅ Check before operation
if ((UINT16_MAX - a) < b) {
    // Handle overflow
} else {
    uint16_t c = a + b;
}
```

### 5. Returning Pointer to Local Variable

```c
// ❌ Returns dangling pointer
int* get_value(void) {
    int local = 42;
    return &local;  // local dies when function returns!
}

// ✅ Return by value or use static
int get_value(void) {
    return 42;
}

// Or
static int persistent = 42;
int* get_value_ptr(void) {
    return &persistent;
}
```

---

## Tips Engineer Profesional

### 💡 Tip 1: Use Static Analysis Tools

Tools seperti PC-lint, Cppcheck, atau clang-tidy bisa catch bugs sebelum compile:

```bash
# Example with cppcheck
cppcheck --enable=all --std=c99 src/
```

### 💡 Tip 2: Initialize Everything

```c
// Make initialization a habit
int value = 0;
int *ptr = NULL;
MyStruct_t obj = {0};  // Zero-initialize struct
```

### 💡 Tip 3: Document Assumptions

```c
/**
 * @pre buffer must be at least 256 bytes
 * @pre interrupts must be enabled
 * @post data_ready flag will be set
 */
void Process_Data(uint8_t *buffer);
```

### 💡 Tip 4: Use Fixed-Width Types

```c
// Always clear about sizes
#include <stdint.h>

uint8_t byte_value;    // Exactly 8 bits
uint16_t word_value;   // Exactly 16 bits
uint32_t dword_value;  // Exactly 32 bits
```

### 💡 Tip 5: Avoid Magic Numbers

```c
// ❌ Unclear
delay(100);
if (status == 3) { /* ... */ }

// ✅ Self-documenting
#define BOOT_DELAY_MS 100
#define STATUS_ERROR_TIMEOUT 3

delay(BOOT_DELAY_MS);
if (status == STATUS_ERROR_TIMEOUT) { /* ... */ }
```

---

## Ringkasan

### Key Takeaways Day 02

1. **Pointer** adalah fundamental untuk hardware access
2. **volatile** wajib untuk hardware registers dan ISR-shared variables
3. **static** untuk encapsulation dan single instances
4. **const** untuk compile-time constants dan read-only data
5. **Function pointers** enable callbacks dan state machines
6. **Memory layout** understanding crucial for optimization
7. **MISRA C** guidelines improve safety and maintainability

### Checklist Pemahaman

- [ ] Saya paham perbedaan pointer types
- [ ] Saya tahu kapan pakai volatile
- [ ] Saya bisa implement callback dengan function pointer
- [ ] Saya mengerti memory sections (.text, .data, .bss)
- [ ] Saya familiar dengan MISRA C basic rules
- [ ] Saya bisa debug pointer-related bugs

---

## Referensi Belajar

### Books
1. **"Effective C"** - Robert C. Seacord
2. **"C Programming: A Modern Approach"** - K.N. King
3. **"Embedded C Coding Standard"** - Barr Group
4. **"MISRA C Guidelines"** - MISRA Consortium

### Online Resources
1. [Barr Group Embedded C Coding Standard](https://barrgroup.com/embedded-systems/books/embedded-c-coding-standard)
2. [MISRA C Guidelines](https://www.misra.org.uk/)
3. [CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard)

### Tools
1. Cppcheck - Static analysis
2. clang-tidy - Code quality
3. Valgrind - Memory debugging (host testing)

---

> **"In God we trust. All others must bring data."** — Brian Kernighan

*Lanjutkan ke Day 03: STM32 Fundamentals*
