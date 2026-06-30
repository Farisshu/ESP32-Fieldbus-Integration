# Exercises - Day 02: Embedded C

## Latihan Pemrograman Embedded C

---

## Exercise 2.1: Pointer Mastery

### Part A: Pointer Basics

**Tugas:** Lengkapi kode berikut dan jelaskan outputnya

```c
#include <stdio.h>

int main(void) {
    int a = 10;
    int *ptr = &a;
    
    // Lengkapi bagian berikut dan prediksi output sebelum menjalankan
    printf("Value of a: %d\n", ___);
    printf("Address of a: %p\n", ___);
    printf("Value of ptr: %p\n", ___);
    printf("Value pointed by ptr: %d\n", ___);
    
    // Modifikasi value melalui pointer
    ___ = 20;
    printf("New value of a: %d\n", a);
    
    return 0;
}
```

**Pertanyaan:**
1. Apa perbedaan antara `ptr`, `&ptr`, dan `*ptr`?
2. Kapan kita harus menggunakan pointer vs direct variable?

---

### Part B: Pointer Arithmetic

**Tugas:** Analisis kode berikut

```c
uint8_t buffer[10] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
uint8_t *ptr = buffer;

// Prediksi nilai setiap ekspresi
printf("%02X\n", *ptr);        // = ?
printf("%02X\n", *(ptr + 2));  // = ?
printf("%02X\n", *ptr++);      // = ?, ptr sekarang menunjuk ke index ?
printf("%02X\n", *++ptr);      // = ?, ptr sekarang menunjuk ke index ?
```

**Challenge:** Implementasi fungsi memcpy menggunakan pointer arithmetic

```c
void my_memcpy(void *dest, const void *src, size_t n) {
    // Implementasi di sini
    // Hint: Cast ke uint8_t* dan increment pointer
}
```

---

## Exercise 2.2: Struct & Memory Layout

### Part A: Struct Size Analysis

**Tugas:** Prediksi ukuran struct berikut (asumsikan 32-bit system)

```c
struct A {
    char a;      // 1 byte
    int b;       // 4 bytes
    char c;      // 1 byte
};

struct B {
    char a;      // 1 byte
    char c;      // 1 byte
    int b;       // 4 bytes
};

struct C {
    uint8_t x;   // 1 byte
    uint32_t y;  // 4 bytes
    uint16_t z;  // 2 bytes
    uint8_t w;   // 1 byte
};
```

**Pertanyaan:**
1. Berapa sizeof(struct A), sizeof(struct B), sizeof(struct C)?
2. Mengapa ukurannya berbeda padahal field-nya sama?
3. Bagaimana cara mengoptimalkan memory layout?

**Verifikasi:** Buat program untuk mengecek jawabanmu

```c
#include <stdio.h>
#include <stdint.h>

// Define structs di sini

int main(void) {
    printf("Size of struct A: %zu bytes\n", sizeof(struct A));
    printf("Size of struct B: %zu bytes\n", sizeof(struct B));
    printf("Size of struct C: %zu bytes\n", sizeof(struct C));
    
    // Bonus: Tampilkan offset setiap member
    printf("Offset of a in A: %zu\n", offsetof(struct A, a));
    printf("Offset of b in A: %zu\n", offsetof(struct A, b));
    printf("Offset of c in A: %zu\n", offsetof(struct A, c));
    
    return 0;
}
```

---

### Part B: Practical Struct Usage

**Tugas:** Design struct untuk sensor data

```c
// Design struct untuk menyimpan data dari sensor IMU (gyro + accelerometer)
// Requirements:
// - Gyro: X, Y, Z (float, dalam deg/s)
// - Accelerometer: X, Y, Z (float, dalam m/s²)
// - Timestamp (uint32_t, dalam ms)
// - Status flags (uint8_t): bit 0 = gyro valid, bit 1 = accel valid, bit 2 = error

typedef struct {
    // Lengkapi di sini
    
} IMU_Data_t;

// Buat fungsi untuk:
// 1. Initialize struct dengan default values
// 2. Set data dari raw sensor readings
// 3. Check jika data valid
// 4. Print data dalam format readable
```

---

## Exercise 2.3: Volatile Deep Dive

### Part A: Identify Volatile Usage

**Tugas:** Tentukan variabel mana yang HARUS volatile dan jelaskan mengapa

```c
// Scenario 1: Counter dalam ISR
void timer_isr(void) {
    static uint32_t counter = 0;  // volatile?
    counter++;
}

void main_loop(void) {
    while (1) {
        if (counter > 1000) {  // counter diakses di main loop
            // do something
        }
    }
}

// Scenario 2: Configuration constant
const uint32_t MAX_VALUE = 1000;  // volatile?

// Scenario 3: Hardware register
#define GPIO_ODR (*(uint32_t *)0x40020014)  // volatile?

// Scenario 4: Local variable
void func(void) {
    int temp = 5;  // volatile?
    temp = temp * 2;
}

// Scenario 5: Shared buffer with DMA
uint8_t rx_buffer[256];  // volatile?
// DMA mengisi buffer ini secara otomatis
```

**Pertanyaan:**
Untuk setiap scenario, jawab:
1. Perlu volatile atau tidak?
2. Apa yang terjadi jika lupa menggunakan volatile?
3. Apa konsekuensi jika menggunakan volatile secara berlebihan?

---

### Part B: Volatile Bug Hunt

**Tugas:** Temukan dan perbaiki bug dalam kode berikut

```c
// BUGGY CODE: Temukan masalahnya!

uint8_t button_pressed = 0;

void EXTI_IRQHandler(void) {
    // Interrupt saat button ditekan
    button_pressed = 1;
}

void wait_for_button(void) {
    while (button_pressed == 0) {
        // Wait for button press
        // Compiler mungkin optimize jadi: while(1) karena button_pressed 
        // tidak berubah di dalam loop ini (dari perspective compiler)
    }
    button_pressed = 0;
}

// Perbaiki kode di atas!
```

---

## Exercise 2.4: Static & Extern

### Part A: Understanding Scope

**Tugas:** Prediksi output program berikut

```c
#include <stdio.h>

int global_var = 10;
static int static_global = 20;

void func1(void) {
    static int counter = 0;
    counter++;
    printf("func1 counter: %d\n", counter);
}

void func2(void) {
    static int counter = 0;
    counter++;
    printf("func2 counter: %d\n", counter);
}

void func3(void) {
    int counter = 0;
    counter++;
    printf("func3 counter: %d\n", counter);
}

int main(void) {
    func1();
    func1();
    func1();
    
    func2();
    func2();
    
    func3();
    func3();
    func3();
    
    return 0;
}
```

**Pertanyaan:**
1. Apa output dari program di atas?
2. Mengapa counter di func1 dan func2 behave berbeda dengan func3?
3. Apa bedanya `static int` di dalam function vs `static int` di file scope?
4. Kapan sebaiknya menggunakan static untuk function?

---

### Part B: Multi-file Project Simulation

**Tugas:** Organize code dalam multiple files

Buat struktur project berikut:

```
project/
├── main.c
├── sensor.c
├── sensor.h
├── config.c
└── config.h
```

**Requirements:**

sensor.h:
```c
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

// Public function declarations
void sensor_init(void);
uint16_t sensor_read(void);

// Public variable (extern)
extern uint8_t sensor_status;

#endif
```

sensor.c:
```c
#include "sensor.h"

// Private variable (static)
static uint16_t calibration_offset = 100;

// Public variable definition
uint8_t sensor_status = 0;

void sensor_init(void) {
    // Initialization code
    sensor_status = 1;
}

uint16_t sensor_read(void) {
    // Simulasi reading
    return calibration_offset;
}

// Private function (static)
static void calibrate(void) {
    // Hanya bisa dipanggil dari dalam sensor.c
}
```

config.h:
```c
#ifndef CONFIG_H
#define CONFIG_H

// Configuration constants
#define MAX_SENSOR_VALUE 1000
#define MIN_SENSOR_VALUE 0

#endif
```

main.c:
```c
#include <stdio.h>
#include "sensor.h"
#include "config.h"

int main(void) {
    sensor_init();
    
    if (sensor_status == 1) {
        uint16_t value = sensor_read();
        
        if (value >= MIN_SENSOR_VALUE && value <= MAX_SENSOR_VALUE) {
            printf("Sensor value: %d\n", value);
        }
    }
    
    return 0;
}
```

**Pertanyaan:**
1. Mengapa `calibration_offset` dibuat static?
2. Mengapa `calibrate()` function dibuat static?
3. Apa keuntungan memisahkan code ke multiple files?
4. Kapan sebaiknya menggunakan extern vs static?

---

## Exercise 2.5: Function Pointer & Callback

### Part A: Basic Function Pointer

**Tugas:** Lengkapi kode berikut

```c
#include <stdio.h>

void add(int a, int b) {
    printf("%d + %d = %d\n", a, b, a + b);
}

void subtract(int a, int b) {
    printf("%d - %d = %d\n", a, b, a - b);
}

void multiply(int a, int b) {
    printf("%d * %d = %d\n", a, b, a * b);
}

int main(void) {
    // Declare array of function pointers
    void (*operations[])(int, int) = {add, subtract, multiply};
    
    int choice;
    printf("Choose operation (0=add, 1=subtract, 2=multiply): ");
    scanf("%d", &choice);
    
    int a = 10, b = 5;
    
    // Call function through pointer
    if (choice >= 0 && choice < 3) {
        // Lengkapi: panggil function yang dipilih
    }
    
    return 0;
}
```

---

### Part B: Callback Pattern for Button Press

**Tugas:** Implementasi callback system untuk button

```c
#include <stdint.h>

// Maximum number of callbacks
#define MAX_BUTTON_CALLBACKS 4

// Callback function type
typedef void (*ButtonCallback_t)(void);

// Button handler structure
typedef struct {
    ButtonCallback_t callbacks[MAX_BUTTON_CALLBACKS];
    uint8_t callback_count;
} Button_Handler_t;

// Implement functions:
// 1. void button_register_callback(Button_Handler_t *handler, ButtonCallback_t callback)
// 2. void button_press_event(Button_Handler_t *handler)
//    - Dipanggil saat button ditekan, akan call semua registered callbacks

// Example usage:
void on_short_press(void) {
    // Toggle LED
}

void on_long_press(void) {
    // Enter config mode
}

void log_press(void) {
    // Log to serial
}

int main(void) {
    Button_Handler_t button_handler = {0};
    
    button_register_callback(&button_handler, on_short_press);
    button_register_callback(&button_handler, log_press);
    
    // Simulate button press
    button_press_event(&button_handler);
    
    return 0;
}
```

**Pertanyaan:**
1. Apa keuntungan menggunakan callback pattern?
2. Di mana callback pattern digunakan di embedded systems?
3. Apa risiko jika callback terlalu lama eksekusinya?

---

## Exercise 2.6: MISRA C Compliance

### Part A: Code Review

**Tugas:** Identifikasi pelanggaran MISRA C dalam kode berikut

```c
// Temukan minimal 5 pelanggaran MISRA C

#include <stdio.h>
#include <string.h>

char global_buffer[100];  // Issue 1

void process_data(char *input) {  // Issue 2
    int i = 0;
    int result;
    
    strcpy(global_buffer, input);  // Issue 3
    
    for (i = 0; i <= 10; i++) {  // Issue 4
        result = some_calculation(i);
        
        if (result = 5) {  // Issue 5
            break;
        }
    }
    
    goto end;  // Issue 6 (sometimes)
    
end:
    return;  // Issue 7
}

int main() {  // Issue 8
    char local[10];
    process_data("Hello World");
    return 0;
}
```

**Untuk setiap issue:**
1. Identifikasi baris dan masalahnya
2. Jelaskan aturan MISRA yang dilanggar
3. Berikan perbaikan kode

---

### Part B: Write MISRA-Compliant Code

**Tugas:** Rewrite fungsi berikut agar MISRA-compliant

```c
// Non-compliant version
void unsafe_function(int *ptr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        ptr[i] = i * 2;
    }
    
    if (size > 10) {
        return 1;  // Type mismatch!
    }
}
```

**Checklist MISRA compliance:**
- [ ] Semua parameter punya qualifier yang tepat (const, etc.)
- [ ] Return type konsisten
- [ ] No implicit type conversions
- [ ] Array bounds checked
- [ ] No unreachable code
- [ ] All paths return value (jika function bukan void)

---

## Exercise 2.7: Memory Layout Challenge

### Tugas: Analyze Memory Usage

Diberikan kode berikut:

```c
#include <stdint.h>

#define BUFFER_SIZE 256

// Global variables
uint32_t global_counter = 0;
const uint32_t lookup_table[256] = {0, 1, 4, 9, 16, ...};

// Static variables
static uint8_t rx_buffer[BUFFER_SIZE];

void process(void) {
    static uint32_t call_count = 0;
    uint8_t local_buffer[64];
    int i;
    
    call_count++;
    // ... processing
}

int main(void) {
    uint32_t stack_test[100];
    // ... main code
    return 0;
}
```

**Pertanyaan:**

1. **Memory Section Classification:**
   - Variabel mana yang ada di .text section?
   - Variabel mana yang ada di .data section?
   - Variabel mana yang ada di .bss section?
   - Variabel mana yang ada di stack?

2. **Memory Calculation:**
   - Berapa total ukuran .data section (perkiraan)?
   - Berapa total ukuran .bss section?
   - Berapa maximum stack usage?

3. **Optimization:**
   - Bagaimana cara mengurangi RAM usage?
   - Kapan sebaiknya menggunakan const vs #define?
   - Apakah ada variabel yang seharusnya static tapi tidak?

---

## Self-Assessment Checklist

- [ ] Saya memahami perbedaan pointer types (const int*, int* const, dll)
- [ ] Saya dapat menjelaskan struct padding dan alignment
- [ ] Saya tahu kapan harus menggunakan volatile
- [ ] Saya memahami perbedaan static di berbagai context
- [ ] Saya dapat menggunakan function pointer untuk callback
- [ ] Saya aware dengan MISRA C guidelines
- [ ] Saya memahami memory layout embedded system

---

## Bonus Challenge: Implement Ring Buffer

**Challenge:** Implementasi thread-safe ring buffer menggunakan konsep yang telah dipelajari

```c
typedef struct {
    uint8_t *buffer;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    // Tambahkan fields yang diperlukan untuk thread-safety
} RingBuffer_t;

// Implement:
// - RingBuffer_Init()
// - RingBuffer_Push() - return error jika full
// - RingBuffer_Pop() - return error jika empty
// - RingBuffer_IsEmpty()
// - RingBuffer_IsFull()
// - RingBuffer_GetCount()

// Requirements:
// - Harus aman untuk interrupt context (gunakan critical section)
// - Gunakan volatile untuk shared variables
// - Handle wrap-around correctly
```

---

*Keep coding! Practice makes perfect.*
