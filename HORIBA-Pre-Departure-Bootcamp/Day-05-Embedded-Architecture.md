# Day 05: Embedded Software Architecture

## Tujuan Pembelajaran

Setelah menyelesaikan hari ini, Anda akan mampu:
- Memahami pentingnya software architecture dalam embedded systems
- Mengimplementasikan layered architecture yang modular
- Mendesain Finite State Machine (FSM) untuk aplikasi embedded
- Memahami konsep Driver Layer, Application Layer, dan BSP
- Mengenal RTOS dan kapan harus menggunakannya
- Menerapkan Git workflow untuk proyek embedded

---

## Konsep Dasar

### Mengapa Architecture Penting?

Dalam industri, kode embedded bukan sekadar "yang penting jalan". Kode harus:
1. **Maintainable** - Mudah diperbaiki dan dikembangkan
2. **Scalable** - Bisa bertambah fitur tanpa rewrite total
3. **Testable** - Bisa diuji per modul
4. **Reusable** - Bisa dipakai di proyek lain
5. **Readable** - Engineer lain bisa memahami

> **Engineer Mindset:** "Write code for humans first, machines second."

---

## Penjelasan Detail

### 1. Layered Architecture

Arsitektur berlapis memisahkan concern berdasarkan abstraksi:

```
+-------------------------------------+
|        Application Layer            |  <- Business logic, FSM
+-------------------------------------+
|         Middleware Layer            |  <- RTOS, Protocol stacks
+-------------------------------------+
|          Driver Layer               |  <- Hardware abstraction
+-------------------------------------+
|      Board Support Package          |  <- HAL/LL, Register access
+-------------------------------------+
|           Hardware                  |  <- MCU, Peripherals
+-------------------------------------+
```

#### Keuntungan Layered Architecture:
- **Isolation**: Perubahan di satu layer tidak mempengaruhi layer lain
- **Testing**: Bisa test application layer tanpa hardware
- **Portability**: Ganti MCU hanya ubah driver layer

---

### 2. Finite State Machine (FSM)

FSM adalah pattern fundamental dalam embedded systems untuk mengelola state sistem.

#### Kapan Menggunakan FSM?
- Sistem dengan mode operasi berbeda (Idle, Running, Error)
- UI dengan multiple screen
- Communication protocol handler
- Motor control dengan sequence

#### Implementasi FSM di C:

```c
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_ERROR
} SystemState_t;

typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_ERROR
} SystemEvent_t;

typedef void (*StateHandler_t)(SystemEvent_t event);

// State table implementation
static SystemState_t currentState = STATE_IDLE;

void FSM_ProcessEvent(SystemEvent_t event) {
    switch(currentState) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                currentState = STATE_RUNNING;
            }
            break;
        case STATE_RUNNING:
            if (event == EVENT_STOP) {
                currentState = STATE_IDLE;
            } else if (event == EVENT_PAUSE) {
                currentState = STATE_PAUSED;
            }
            break;
        // ... handle other states
    }
}
```

---

### 3. Driver Layer Design

**Buruk - Hardware Coupled:**
```c
void App_Task() {
    GPIOA->BSRR = (1 << 5);  // Direct register access
}
```

**Baik - Hardware Abstracted:**
```c
void App_Task() {
    Led_Toggle(LED_GREEN);  // Abstracted interface
}
```

---

### 4. Git Workflow untuk Embedded

#### Commit Message Convention:
```
feat(sensor): add I2C sensor driver

- Implement I2C communication
- Add CRC validation
- Support measurement rates: 1Hz, 2Hz, 4Hz
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

---

## Kesalahan yang Sering Terjadi

1. **Spaghetti Code** - Everything in main.c
2. **Hardware Coupling** - Application depends on specific MCU
3. **Global Variable Abuse** - Too many globals
4. **No Error Handling** - Assume everything works
5. **Blocking in ISR** - Long operations in interrupt

---

## Tips Engineer Profesional

1. **Design Before Coding** - Buat diagram arsitektur dulu
2. **Interface First** - Tulis header file dulu
3. **Document Why, Not What** - Jelaskan alasan, bukan kode
4. **Version Control Everything** - Code, docs, schematics
5. **Think About Testing** - Write testable code

---

## Ringkasan

| Konsep | Key Takeaway |
|--------|--------------|
| Layered Architecture | Separate concerns, enable testing |
| FSM | Manage state-based behavior systematically |
| Driver Layer | Abstract hardware from application |
| Git Workflow | Track changes, collaborate effectively |

---

## Referensi Belajar

1. "Embedded Systems Architecture" - Tammy Noergaard
2. "Making Embedded Systems" - Elecia White
3. FreeRTOS Documentation - https://www.freertos.org/
4. Embedded Artistry - https://embeddedartistry.com/

---

> "Good architecture makes the system easy to understand, modify, and extend."
