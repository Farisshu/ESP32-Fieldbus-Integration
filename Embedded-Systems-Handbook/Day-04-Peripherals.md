# Day 04 - Embedded Architecture & Peripherals

> **"Good architecture makes the system easy to understand, modify, and test."**

---

## Tujuan Pembelajaran

Setelah mempelajari materi ini, Anda akan mampu:

- Memahami layered architecture dalam embedded systems
- Mengimplementasikan Finite State Machine (FSM) dengan benar
- Membangun driver layer yang reusable
- Memisahkan application layer dari hardware
- Memahami konsep BSP (Board Support Package)
- Mengenal middleware components
- Memahami dasar-dasar RTOS
- Menerapkan modular programming
- Menggunakan Git workflow sederhana untuk embedded development

---

## Konsep Dasar

### Mengapa Architecture Penting?

Tanpa arsitektur yang baik:
```
┌─────────────────────────────────────────────────────────┐
│           SPAGHETTI CODE ARCHITECTURE                   │
├─────────────────────────────────────────────────────────┤
│                                                         │
│   main()                                                │
│     ├── GPIO calls                                      │
│     ├── UART calls                                      │
│     ├── ADC reads                                       │
│     ├── Business logic                                  │
│     └── More GPIO calls...                              │
│                                                         │
│   Result:                                               │
│   • Hard to test                                        │
│   • Hard to modify                                      │
│   • Hard to reuse                                       │
│   • One change breaks everything                        │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

Dengan arsitektur yang baik:
```
┌─────────────────────────────────────────────────────────┐
│           LAYERED ARCHITECTURE                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│   ┌─────────────────────────────────────┐              │
│   │        Application Layer            │              │
│   │     (Business Logic, FSM)           │              │
│   └─────────────────┬───────────────────┘              │
│                     │                                   │
│   ┌─────────────────▼───────────────────┐              │
│   │         Middleware Layer            │              │
│   │    (Protocols, Filters, Buffers)    │              │
│   └─────────────────┬───────────────────┘              │
│                     │                                   │
│   ┌─────────────────▼───────────────────┐              │
│   │          Driver Layer               │              │
│   │   (HAL Abstraction, Device Drivers) │              │
│   └─────────────────┬───────────────────┘              │
│                     │                                   │
│   ┌─────────────────▼───────────────────┐              │
│   │            BSP / HAL                │              │
│   │      (Hardware Specific)            │              │
│   └─────────────────┬───────────────────┘              │
│                     │                                   │
│   ┌─────────────────▼───────────────────┐              │
│   │            Hardware                 │              │
│   │         (STM32, Sensors)            │              │
│   └─────────────────────────────────────┘              │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## Penjelasan Detail

### 1. Finite State Machine (FSM) Deep Dive

#### FSM dengan Hierarchical States

```c
typedef enum {
    /* Top-level states */
    STATE_SYSTEM_INIT,
    STATE_NORMAL_OPERATION,
    STATE_MAINTENANCE,
    STATE_ERROR,
    
    /* Sub-states for NORMAL_OPERATION */
    STATE_NORMAL_IDLE,
    STATE_NORMAL_PROCESSING,
    STATE_NORMAL_WAITING,
    
    /* Sub-states for ERROR */
    STATE_ERROR_RECOVERABLE,
    STATE_ERROR_CRITICAL
} SystemState_t;

typedef struct {
    SystemState_t currentState;
    SystemState_t previousState;
    uint32_t stateEntryTime;
    void (*entryFunc)(void);
    void (*exitFunc)(void);
    void (*updateFunc)(void);
} FSM_HandleTypeDef;
```

#### FSM dengan Event Queue

```c
typedef enum {
    EVENT_BUTTON_PRESS,
    EVENT_BUTTON_RELEASE,
    EVENT_TIMER_TIMEOUT,
    EVENT_DATA_RECEIVED,
    EVENT_ERROR_DETECTED
} Event_t;

typedef struct {
    Event_t event;
    uint32_t timestamp;
    void *data;
} EventMessage_t;

/* Event queue for decoupled communication */
#define EVENT_QUEUE_SIZE 16
static EventMessage_t eventQueue[EVENT_QUEUE_SIZE];
static uint8_t queueHead = 0;
static uint8_t queueTail = 0;

void FSM_PostEvent(Event_t event, void *data) {
    uint8_t nextHead = (queueHead + 1) % EVENT_QUEUE_SIZE;
    
    if (nextHead != queueTail) {
        eventQueue[queueHead].event = event;
        eventQueue[queueHead].timestamp = HAL_GetTick();
        eventQueue[queueHead].data = data;
        queueHead = nextHead;
    }
}

EventMessage_t* FSM_GetEvent(void) {
    if (queueHead == queueTail) {
        return NULL;  /* Queue empty */
    }
    
    EventMessage_t *event = &eventQueue[queueTail];
    queueTail = (queueTail + 1) % EVENT_QUEUE_SIZE;
    return event;
}
```

### 2. Driver Layer Design

#### Generic Sensor Driver Interface

```c
/* sensor_driver.h */
#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/* Abstract sensor interface */
typedef struct {
    int32_t (*init)(void);
    int32_t (*read)(float *value);
    int32_t (*calibrate)(void);
    bool (*isReady)(void);
    const char* (*getName)(void);
} Sensor_Driver_t;

/* Implementation for specific sensor */
int32_t BMP280_Init(void);
int32_t BMP280_Read(float *pressure);
bool BMP280_IsReady(void);

const Sensor_Driver_t BMP280_Driver = {
    .init = BMP280_Init,
    .read = BMP280_Read,
    .calibrate = NULL,
    .isReady = BMP280_IsReady,
    .getName = "BMP280"
};

#endif
```

#### Driver Implementation Template

```c
/* bmp280_driver.c */
#include "bmp280_driver.h"
#include "i2c_hal.h"

/* Private definitions */
#define BMP280_I2C_ADDR     0x76
#define BMP280_REG_TEMP     0xE7
#define BMP280_REG_PRESS    0xF7

/* Private variables */
static bool isInitialized = false;

/* Private functions */
static int32_t readRegister(uint8_t reg, uint8_t *data, uint16_t len) {
    return I2C_ReadRegisters(BMP280_I2C_ADDR, reg, data, len);
}

/* Public implementation */
int32_t BMP280_Init(void) {
    if (isInitialized) {
        return 0;  /* Already initialized */
    }
    
    /* Reset sensor */
    I2C_WriteRegister(BMP280_I2C_ADDR, 0xE0, 0xB6);
    HAL_Delay(10);
    
    /* Configure */
    I2C_WriteRegister(BMP280_I2C_ADDR, 0xF5, 0x05);  /* oversampling */
    
    isInitialized = true;
    return 0;
}

int32_t BMP280_Read(float *pressure) {
    if (!isInitialized) {
        return -1;
    }
    
    uint8_t rawData[3];
    if (readRegister(BMP280_REG_PRESS, rawData, 3) != 0) {
        return -1;
    }
    
    /* Convert raw data to pressure (hPa) */
    uint32_t raw = ((uint32_t)rawData[0] << 12) |
                   ((uint32_t)rawData[1] << 4) |
                   ((uint32_t)rawData[2] >> 4);
    
    *pressure = (float)raw / 256.0f;  /* Simplified conversion */
    
    return 0;
}

bool BMP280_IsReady(void) {
    return isInitialized;
}
```

### 3. Application Layer Pattern

#### Separation from Hardware

```c
/* application.h - No hardware dependencies! */
#ifndef APPLICATION_H
#define APPLICATION_H

#include "sensor_driver.h"
#include "communication.h"

typedef struct {
    float temperature;
    float humidity;
    uint32_t lastUpdate;
    bool isValid;
} SensorData_t;

void Application_Init(void);
void Application_Update(void);
void Application_ProcessData(const SensorData_t *data);

#endif
```

```c
/* application.c */
#include "application.h"

/* Private variables */
static SensorData_t currentData;
static uint32_t updateCounter = 0;

void Application_Init(void) {
    updateCounter = 0;
    currentData.isValid = false;
}

void Application_Update(void) {
    updateCounter++;
    
    /* Business logic here - no direct hardware calls! */
    if (currentData.isValid) {
        /* Process valid data */
        ProcessSensorData(&currentData);
    }
}

void Application_ProcessData(const SensorData_t *data) {
    /* Application-specific processing */
    /* Logging, decisions, etc. */
}
```

### 4. BSP (Board Support Package)

#### BSP Structure

```
bsp/
├── bsp.h              /* Common BSP interface */
├── bsp_stm32f4/       /* STM32F4 specific */
│   ├── bsp_gpio.c
│   ├── bsp_uart.c
│   └── bsp_config.h
├── bsp_stm32f1/       /* STM32F1 specific */
│   ├── bsp_gpio.c
│   ├── bsp_uart.c
│   └── bsp_config.h
└── bsp_custom/        /* Custom board */
    ├── bsp_gpio.c
    └── bsp_config.h
```

#### BSP Interface

```c
/* bsp.h */
#ifndef BSP_H
#define BSP_H

#include <stdint.h>

/* Board identification */
typedef enum {
    BOARD_UNKNOWN = 0,
    BOARD_STM32F4_DISCOVERY,
    BOARD_STM32F1_BLUEPILL,
    BOARD_CUSTOM_V1
} BoardType_t;

/* BSP initialization */
int32_t BSP_Init(void);
BoardType_t BSP_GetBoardType(void);
const char* BSP_GetBoardName(void);

/* BSP version for compatibility check */
#define BSP_VERSION_MAJOR 1
#define BSP_VERSION_MINOR 0

#endif
```

### 5. Middleware Components

#### Ring Buffer for Data Streaming

```c
/* ring_buffer.h */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *buffer;
    size_t size;
    size_t head;
    size_t tail;
    size_t count;
} RingBuffer_t;

void RingBuffer_Init(RingBuffer_t *rb, uint8_t *buffer, size_t size);
size_t RingBuffer_Write(RingBuffer_t *rb, const uint8_t *data, size_t len);
size_t RingBuffer_Read(RingBuffer_t *rb, uint8_t *data, size_t len);
size_t RingBuffer_GetCount(RingBuffer_t *rb);
bool RingBuffer_IsEmpty(RingBuffer_t *rb);
bool RingBuffer_IsFull(RingBuffer_t *rb);

#endif
```

```c
/* ring_buffer.c */
#include "ring_buffer.h"
#include <string.h>

void RingBuffer_Init(RingBuffer_t *rb, uint8_t *buffer, size_t size) {
    rb->buffer = buffer;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

size_t RingBuffer_Write(RingBuffer_t *rb, const uint8_t *data, size_t len) {
    size_t writable = rb->size - rb->count;
    size_t toWrite = (len < writable) ? len : writable;
    
    for (size_t i = 0; i < toWrite; i++) {
        rb->buffer[rb->head] = data[i];
        rb->head = (rb->head + 1) % rb->size;
    }
    
    rb->count += toWrite;
    return toWrite;
}

size_t RingBuffer_Read(RingBuffer_t *rb, uint8_t *data, size_t len) {
    size_t readable = rb->count;
    size_t toRead = (len < readable) ? len : readable;
    
    for (size_t i = 0; i < toRead; i++) {
        data[i] = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) % rb->size;
    }
    
    rb->count -= toRead;
    return toRead;
}
```

### 6. RTOS Overview

#### When to Use RTOS?

```
┌─────────────────────────────────────────────────────────┐
│           SUPERLOOP vs RTOS                             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  SUPERLOOP (Bare Metal):                                │
│  • Simple applications                                  │
│  • Tight timing requirements                            │
│  • Low resource usage                                   │
│  • Single developer                                     │
│                                                         │
│  RTOS:                                                  │
│  • Multiple concurrent tasks                            │
│  • Complex timing requirements                          │
│  • Team development                                     │
│  • Need task prioritization                             │
│  • Memory available (>10KB RAM)                         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Basic RTOS Concepts

```c
/* FreeRTOS example */
#include "FreeRTOS.h"
#include "task.h"

/* Task handles */
TaskHandle_t xTaskHandle_LED;
TaskHandle_t xTaskHandle_Sensor;

/* Task function */
void vLED_Task(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        vTaskDelay(pdMS_TO_TICKS(500));  /* Non-blocking delay */
    }
}

void vSensor_Task(void *pvParameters) {
    while (1) {
        float sensor_value = ReadSensor();
        ProcessSensor(sensor_value);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* Create tasks */
void StartScheduler(void) {
    xTaskCreate(vLED_Task, "LED", 128, NULL, 1, &xTaskHandle_LED);
    xTaskCreate(vSensor_Task, "Sensor", 256, NULL, 2, &xTaskHandle_Sensor);
    
    vTaskStartScheduler();  /* Start RTOS scheduler */
}
```

### 7. Modular Programming

#### Module Interface Pattern

```c
/* temperature_module.h */
#ifndef TEMPERATURE_MODULE_H
#define TEMPERATURE_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Module configuration */
#define TEMP_MODULE_ENABLED 1

/* Public types */
typedef enum {
    TEMP_UNIT_CELSIUS,
    TEMP_UNIT_FAHRENHEIT,
    TEMP_UNIT_KELVIN
} TempUnit_t;

/* Public functions */
int32_t TempModule_Init(void);
float TempModule_GetTemperature(TempUnit_t unit);
int32_t TempModule_SetCalibration(float offset);
void TempModule_EnableAlert(float threshold);
void TempModule_DisableAlert(void);

/* Module version */
#define TEMP_MODULE_VERSION "1.0.0"

#ifdef __cplusplus
}
#endif

#endif
```

#### Dependency Injection Pattern

```c
/* Instead of hardcoding dependencies */
❌ void ProcessData(void) {
    Sensor_Read();  /* Hardcoded dependency */
}

/* Use dependency injection */
✅ typedef struct {
    int32_t (*sensorRead)(float*);
    void (*loggerLog)(const char*);
} ProcessDependencies_t;

void ProcessData(ProcessDependencies_t *deps) {
    float value;
    if (deps->sensorRead(&value) == 0) {
        deps->loggerLog("Data processed");
    }
}
```

### 8. Git Workflow untuk Embedded

#### Simple Git Flow

```
main (production)
  │
  ├─── release/v1.0
  │       │
  │       └─── hotfix/bug-fix
  │
  └─── develop
          │
          ├─── feature/new-sensor
          ├─── feature/uart-driver
          └─── fix/timing-issue
```

#### Commit Message Convention

```bash
# Format: type(scope): description

feat(drivers): add BMP280 sensor driver
fix(uart): resolve buffer overflow at high baudrate
docs(readme): update installation instructions
refactor(fsm): simplify state transition logic
test(adc): add unit tests for ADC calibration
chore(build): update PlatformIO configuration
```

#### .gitignore untuk Embedded

```gitignore
# Build outputs
build/
dist/
*.elf
*.hex
*.bin
*.map

# IDE files
.vscode/
.idea/
*.launch
*.project
*.cproject

# STM32CubeMX generated code (optional)
Middlewares/
Drivers/

# Debug files
*.log
*.dump

# OS files
.DS_Store
Thumbs.db
```

---

## Kesalahan yang Sering Terjadi

### 1. Circular Dependencies

```c
/* ❌ module_a.h */
#include "module_b.h"  /* module_a needs module_b */

/* ❌ module_b.h */
#include "module_a.h"  /* module_b needs module_a */
/* Result: Compilation error! */

/* ✅ Solution: Forward declaration */
/* module_a.h */
typedef struct ModuleB ModuleB_t;  /* Forward declare */
void ModuleA_Function(ModuleB_t *b);

/* module_b.h */
typedef struct ModuleA ModuleA_t;  /* Forward declare */
void ModuleB_Function(ModuleA_t *a);
```

### 2. Global State Everywhere

```c
/* ❌ Hard to test, hard to maintain */
int global_counter;
float global_sensor_value;
bool global_flag;

/* ✅ Encapsulate in module */
typedef struct {
    int counter;
    float sensor_value;
    bool flag;
} ModuleState_t;

static ModuleState_t state = {0};

/* Provide controlled access */
void Module_SetValue(float value);
float Module_GetValue(void);
```

### 3. Blocking Code in RTOS Tasks

```c
/* ❌ Blocks entire task */
void vTask(void *pvParameters) {
    while (1) {
        HAL_Delay(1000);  /* Blocks this task only (good) */
        SomeFunction();
    }
}

/* ✅ Use RTOS delays */
void vTask(void *pvParameters) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  /* Proper RTOS delay */
        SomeFunction();
    }
}
```

### 4. No Error Propagation

```c
/* ❌ Silent failure */
void ProcessData(void) {
    ReadSensor();  /* What if it fails? */
    Calculate();   /* Using potentially invalid data */
}

/* ✅ Check and propagate errors */
int32_t ProcessData(void) {
    int32_t status;
    
    status = ReadSensor();
    if (status != 0) {
        return status;  /* Propagate error */
    }
    
    status = Calculate();
    if (status != 0) {
        return status;
    }
    
    return 0;
}
```

---

## Tips Engineer Profesional

### 💡 Tip 1: Design Before Coding

Spend 30% of time designing:
- Draw architecture diagram
- Define module interfaces
- Identify dependencies
- Plan testing strategy

### 💡 Tip 2: Keep Layers Thin

Each layer should:
- Have single responsibility
- Know only about layer below
- Be testable in isolation
- Have clear interface

### 💡 Tip 3: Document Interfaces, Not Implementation

```c
/* Good documentation */
/**
 * @brief Initialize sensor driver
 * @param config Pointer to configuration structure
 * @return 0 on success, negative error code on failure
 * 
 * @pre I2C peripheral must be initialized
 * @post Sensor is ready for reading
 * 
 * @note This function is not thread-safe
 */
int32_t Sensor_Init(SensorConfig_t *config);
```

### 💡 Tip 4: Use Static Analysis

```bash
# Add to your build process
cppcheck --enable=all src/
clang-tidy src/*.c
```

### 💡 Tip 5: Version Your Interfaces

```c
/* Interface version for compatibility */
#define DRIVER_API_VERSION 2

typedef struct {
    uint32_t api_version;
    /* ... other fields ... */
} DriverConfig_t;

int32_t Driver_Init(DriverConfig_t *config) {
    if (config->api_version != DRIVER_API_VERSION) {
        return ERROR_VERSION_MISMATCH;
    }
    /* ... */
}
```

---

## Ringkasan

### Key Takeaways Day 04

1. **Layered architecture** separates concerns and improves maintainability
2. **FSM** provides predictable, testable behavior
3. **Driver abstraction** enables hardware independence
4. **BSP** encapsulates board-specific code
5. **Middleware** provides reusable components
6. **RTOS** helps manage complex concurrent operations
7. **Modular design** enables team collaboration
8. **Git workflow** tracks changes and enables collaboration

### Checklist Pemahaman

- [ ] Saya bisa draw layered architecture diagram
- [ ] Saya bisa implement FSM dengan event handling
- [ ] Saya bisa create generic driver interface
- [ ] Saya paham separation antara application dan hardware
- [ ] Saya bisa implement ring buffer
- [ ] Saya mengerti kapan perlu RTOS
- [ ] Saya bisa use Git untuk version control

---

## Referensi Belajar

### Books
1. "Making Embedded Systems" - Elecia White
2. "Patterns for Time-Triggered Embedded Systems" - Michael Pont
3. "Embedded Software Architecture" - Various

### Online Resources
1. [Embedded Artistry](https://embeddedartistry.com/)
2. [Jacob Beningo's Blog](https://www.beningo.com/)
3. [FreeRTOS Documentation](https://www.freertos.org/)

### Tools
1. Git - Version control
2. Doxygen - Documentation generation
3. Cppcheck - Static analysis

---

> **"Architecture is the decisions that you wish you could get right early in a project, but are more likely to get right later on."**

*Lanjutkan ke Day 05: Electronics Review*