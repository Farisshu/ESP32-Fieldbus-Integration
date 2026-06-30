# Embedded C Cheat Sheet

## Quick Reference untuk Embedded Developer

---

## 1. Type Modifiers

| Modifier | Purpose | Example |
|----------|---------|---------|
| `const` | Read-only, stored in Flash | `const int MAX_VAL = 100;` |
| `volatile` | Prevent optimization, read from memory every time | `volatile uint32_t *REG = 0x40021000;` |
| `static` | Persistent across function calls / file scope | `static int counter = 0;` |
| `extern` | Declare variable defined elsewhere | `extern int global_var;` |
| `register` | Hint to store in register (compiler may ignore) | `register int i;` |

### Kapan Menggunakan volatile?
```c
// ✅ Hardware registers
volatile uint32_t *GPIO_ODR = (volatile uint32_t *)0x40020014;

// ✅ Variables shared with ISR
volatile uint8_t rx_flag = 0;

// ✅ Variables modified by DMA
volatile uint16_t adc_buffer[10];

// ❌ Jangan gunakan untuk variabel lokal biasa
void func() {
    volatile int x = 5; // Tidak perlu!
}
```

---

## 2. Pointer Essentials

### Pointer Declaration
```c
int var = 10;
int *ptr = &var;        // Pointer ke int
int **ptr2 = &ptr;      // Pointer ke pointer
const int *p1 = &var;   // Pointer ke const int (value tidak bisa diubah via pointer)
int *const p2 = &var;   // Const pointer ke int (pointer tidak bisa diubah)
const int *const p3 = &var; // Const pointer ke const int
```

### Function Pointer
```c
// Deklarasi
void (*func_ptr)(int);

// Assignment
func_ptr = &my_function;

// Call
func_ptr(5);

// Array of function pointers (untuk state machine)
void (*state_table[])(void) = {state_idle, state_run, state_error};
```

---

## 3. Struct & Union

### Struct
```c
typedef struct {
    uint16_t voltage;
    uint16_t current;
    uint8_t status;
} SensorData_t;

SensorData_t sensor;
sensor.voltage = 3300; // 3.3V dalam mV
```

### Union (Memory Overlay)
```c
typedef union {
    uint32_t raw;
    struct {
        uint8_t b0;
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
    } bytes;
} DataUnion_t;

DataUnion_t data;
data.raw = 0x12345678;
// data.bytes.b0 = 0x78 (little endian)
```

### Bit Field (Hati-hati dengan portability)
```c
typedef struct {
    uint8_t enable : 1;
    uint8_t mode   : 3;
    uint8_t speed  : 4;
} Config_t; // Total 1 byte
```

---

## 4. Memory Layout

```
┌─────────────────┐
│    Stack        │ ← Tumbuh ke bawah (local variables)
│    (High Addr)  │
├─────────────────┤
│       ↑         │
│       │         │
│    Free RAM     │
│       │         │
│       ↓         │
├─────────────────┤
│      Heap       │ ← Tumbuh ke atas (dynamic allocation)
├─────────────────┤
│      .bss       │ ← Uninitialized global/static (zero-filled)
├─────────────────┤
│      .data      │ ← Initialized global/static
├─────────────────┤
│      .text      │ ← Code (Flash)
│    (Low Addr)   │
└─────────────────┘
```

### Best Practice
```c
// ✅ GOOD: Gunakan static untuk buffer besar di embedded
static uint8_t dma_buffer[1024];

// ❌ BAD: Hindari malloc di embedded real-time
void func() {
    uint8_t *buf = malloc(1024); // Fragmentation risk!
    free(buf);
}
```

---

## 5. Common Macros

```c
// Bit manipulation
#define SET_BIT(REG, BIT)     ((REG) |= (1U << (BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1U << (BIT)))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1U << (BIT)))
#define READ_BIT(REG, BIT)    (((REG) >> (BIT)) & 1U)

// Min/Max
#define MIN(a, b)             (((a) < (b)) ? (a) : (b))
#define MAX(a, b)             (((a) > (b)) ? (a) : (b))

// Array size
#define ARRAY_SIZE(arr)       (sizeof(arr) / sizeof((arr)[0]))

// Unused parameter (hindari warning)
#define UNUSED(x)             ((void)(x))
```

---

## 6. MISRA C Key Rules (Overview)

| Rule | Description | Example |
|------|-------------|---------|
| 5.2 | Identifier length ≤ 31 chars | `very_long_variable_name_that_exceeds_limit` ❌ |
| 8.7 | External linkage should be minimized | Use `static` for file-scope functions |
| 10.1 | Operands shall not be of inappropriate essential type | Don't mix signed/unsigned without cast |
| 11.1 | Conversions shall not be performed between pointer and integer | Avoid `(int*)0x40021000` tanpa typedef |
| 14.1 | Loop counter shall not be modified inside loop body | Jangan ubah `i` di dalam `for(i=0; i<10; i++)` |
| 17.2 | Functions shall not call themselves (no recursion) | Hindari recursive function di embedded |

---

## 7. Interrupt Safe Code

```c
// ✅ Pattern: Critical section
#include <stdint.h>

void critical_section_enter(void) {
    __disable_irq();
}

void critical_section_leave(void) {
    __enable_irq();
}

// Usage
volatile uint32_t shared_counter = 0;

void update_counter(void) {
    critical_section_enter();
    shared_counter++;
    critical_section_leave();
}

// ✅ Pattern: Atomic flag
volatile uint8_t data_ready = 0;

void ISR_Handler(void) {
    // Process data
    data_ready = 1;
}

void main_loop(void) {
    if (data_ready) {
        data_ready = 0; // Clear flag
        // Process data safely
    }
}
```

---

## 8. Common Pitfalls

### ❌ Mistake 1: Forgetting volatile
```c
uint8_t flag = 0; // Missing volatile!

// ISR
void EXTI_IRQHandler(void) {
    flag = 1;
}

// Main
while (flag == 0) { // Compiler mungkin optimize jadi while(1)!
    // Wait
}
```

### ❌ Mistake 2: Buffer overflow
```c
char buffer[10];
strcpy(buffer, user_input); // Bahaya jika input > 9 chars!
```

### ❌ Mistake 3: Dangling pointer
```c
int* get_pointer(void) {
    int local = 10;
    return &local; // SALAH! local hilang setelah return
}
```

---

## 9. Quick Syntax Reference

```c
// Ternary operator
result = (condition) ? value_if_true : value_if_false;

// Switch-case (common in FSM)
switch(state) {
    case STATE_IDLE:
        // ...
        break;
    case STATE_RUNNING:
        // ...
        break;
    default:
        // Error handling
        break;
}

// Do-while (execute at least once)
do {
    // Process
} while (condition);

// Goto (acceptable in embedded for error handling)
if (error) {
    goto cleanup;
}
cleanup:
    // Resource cleanup
```

---

## 10. Debugging Tips

| Problem | Check |
|---------|-------|
| HardFault | Stack overflow? Invalid pointer? Unaligned access? |
| Unexpected behavior | Missing `volatile`? Race condition? |
| Memory leak | Using `malloc`? Consider static allocation |
| Intermittent bugs | Check uninitialized variables (.bss section) |

### Useful Debug Macros
```c
#ifdef DEBUG
    #define DBG_PRINT(fmt, ...) printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DBG_PRINT(fmt, ...) ((void)0)
#endif

// Breakpoint macro
#define BREAKPOINT() __asm__("bkpt 0")
```

---

## Ringkasan Cepat

| Konsep | Kapan Digunakan |
|--------|-----------------|
| `const` | Data yang tidak berubah (lookup tables, config) |
| `volatile` | Hardware registers, ISR-shared variables, DMA buffers |
| `static` | Persistent state, file-scope functions, large buffers |
| `extern` | Global variables shared across files |
| Function pointer | Callbacks, state machines, driver abstraction |
| Union | Memory-efficient data representation, protocol parsing |
| Bit field | Register mapping, memory-constrained structures |

---

**Pro Tip:** Selalu compile dengan warning flags maksimal:
```bash
-Wall -Wextra -Wpedantic -Werror
```

Dan gunakan static analysis tools seperti:
- PC-lint
- Cppcheck
- SonarQube

---

*Last Updated: [Tanggal]*  
*Untuk dokumentasi lengkap, lihat Day-02-Embedded-C.md*
