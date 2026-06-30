# Day 01 - Engineer Mindset

> **"Seorang engineer tidak dinilai dari seberapa banyak yang dia tahu, tapi dari seberapa efektif dia menyelesaikan masalah."**

---

## Tujuan Pembelajaran

Setelah mempelajari materi ini, Anda akan mampu:

- Memahami cara berpikir seorang engineer dalam menghadapi masalah
- Mengaplikasikan problem-solving framework secara sistematis
- Membaca dan memahami datasheet dengan efektif
- Membuat dokumentasi teknis yang jelas
- Mengembangkan debugging mindset yang produktif
- Memahami konsep Finite State Machine (FSM) dan flowchart

---

## Konsep Dasar

### Apa Itu Engineer Mindset?

Engineer mindset adalah **cara berpikir sistematis** dalam menghadapi masalah. Bukan tentang menjadi pintar, tapi tentang menjadi **terstruktur**.

```
┌─────────────────────────────────────────────────────────┐
│           ENGINEER MINDSET FRAMEWORK                    │
├─────────────────────────────────────────────────────────┤
│                                                         │
│   PROBLEM → ANALYZE → PLAN → IMPLEMENT → VERIFY        │
│      ↑                                        │        │
│      └────────────── ITERATE ←────────────────┘        │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## Penjelasan Detail

### 1. Problem Solving Framework

#### Langkah 1: Define the Problem (Definisikan Masalah)

**Cara berpikir engineer:**
- Jangan langsung coding!
- Pahami apa yang **sebenarnya** terjadi vs apa yang **diharapkan** terjadi
- Tulis masalah dalam satu kalimat yang jelas

**Contoh:**
```
❌ Buruk: "Kode saya tidak jalan"
✅ Baik: "LED tidak menyala setelah fungsi GPIO_WriteHigh dipanggil"
```

#### Langkah 2: Gather Information (Kumpulkan Informasi)

Seorang engineer selalu bertanya:
- Kapan masalah muncul pertama kali?
- Apakah ada perubahan sebelum masalah muncul?
- Apakah masalah konsisten atau intermittent?
- Apa yang sudah dicoba?

#### Langkah 3: Break Down (Pecah Masalah)

```
Masalah Besar: "Sistem embedded tidak responsif"
       ↓
┌──────────────────────────────────────┐
│ Pecah menjadi:                       │
│ 1. Power supply OK?                  │
│ 2. Clock configuration benar?        │
│ 3. Interrupt aktif?                  │
│ 4. Code stuck di loop tertentu?      │
│ 5. Peripheral conflict?              │
└──────────────────────────────────────┘
```

#### Langkah 4: Hypothesize & Test (Hipotesis dan Uji)

```
Hipotesis: "Mungkin clock tidak dikonfigurasi dengan benar"
   ↓
Test: Cek register RCC, ukur frekuensi dengan oscilloscope
   ↓
Konfirmasi/Bantah hipotesis
   ↓
Ulangi dengan hipotesis baru jika perlu
```

### 2. Debugging Mindset

#### Prinsip Debugging

1. **Jangan menebak, verifikasi!**
   ```c
   // ❌ Jangan: "Kayanya sih ini masalahnya..."
   // ✅ Lakukan: "Mari kita cek dengan debugger"
   ```

2. **Reproduce dulu, fix kemudian**
   - Jika tidak bisa reproduce, tidak bisa verify fix
   - Buat test case minimal yang menunjukkan bug

3. **Satu perubahan pada satu waktu**
   ```
   ❌ Ubah 5 hal sekaligus → Tidak tahu mana yang fix
   ✅ Ubah 1 hal → Test → Dokumentasi → Lanjut
   ```

4. **Gunakan tools yang tepat**
   - Debugger (ST-Link, J-Link)
   - Logic Analyzer
   - Oscilloscope
   - Serial printf debugging
   - Unit tests

#### Debugging Workflow

```
┌─────────────────────────────────────────────────────────┐
│              DEBUGGING WORKFLOW                         │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  1. Reproduce bug                                       │
│         ↓                                               │
│  2. Isolate (minimal test case)                         │
│         ↓                                               │
│  3. Form hypothesis                                     │
│         ↓                                               │
│  4. Test hypothesis                                     │
│         ↓                                               │
│  5. Fix & Verify                                        │
│         ↓                                               │
│  6. Document & Prevent recurrence                       │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### 3. Reading Datasheet

#### Mengapa Datasheet Penting?

Datasheet adalah **kontrak** antara Anda dan manufacturer chip. Semua yang Anda butuhkan ada di sana.

#### Cara Efektif Membaca Datasheet

**Jangan baca dari halaman 1 sampai selesai!** Gunakan pendekatan targeted:

```
┌─────────────────────────────────────────────────────────┐
│         DATASHEET READING STRATEGY                      │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  START → Table of Contents                              │
│     ↓                                                   │
│  Identifikasi section yang dibutuhkan:                  │
│  • Pin definition → Cari pin yang digunakan             │
│  • Electrical characteristics → Cek voltage, current    │
│  • Register map → Untuk low-level programming           │
│  • Timing diagrams → Untuk komunikasi protocol          │
│     ↓                                                   │
│  Baca section tersebut secara detail                    │
│     ↓                                                   │
│  Cross-reference dengan reference manual                │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Section Penting di Datasheet STM32

| Section | Kegunaan | Kapan Dibaca |
|---------|----------|--------------|
| Pin Definition | Mengetahui fungsi setiap pin | Saat design hardware |
| Electrical Characteristics | Voltage, current limits | Saat power planning |
| Register Map | Low-level register access | Saat develop driver |
| Timing Diagrams | Setup/hold time, clock timing | Saat debug communication |
| Memory Map | Address mapping | Saat linkerscript/config |

### 4. Finite State Machine (FSM) Mindset

#### Apa Itu FSM?

FSM adalah cara berpikir bahwa sistem berada di **satu state pada satu waktu**, dan berpindah state berdasarkan **event/kondisi**.

#### Visualisasi FSM

```
┌─────────────────────────────────────────────────────────┐
│            LED BLINKING FSM                             │
│                                                         │
│         [IDLE]                                          │
│        /      \                                         │
│   btn_press   timeout                                   │
│      /            \                                     │
│     ↓              ↓                                    │
│ [TURN_ON] ────→ [TURN_OFF]                             │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Implementasi FSM dalam C

```c
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_ERROR
} SystemState_t;

SystemState_t currentState = STATE_IDLE;

void StateMachine_Update(void) {
    switch(currentState) {
        case STATE_IDLE:
            if (StartButton_Pressed()) {
                currentState = STATE_RUNNING;
                System_Init();
            }
            break;
            
        case STATE_RUNNING:
            if (StopButton_Pressed()) {
                currentState = STATE_PAUSED;
            } else if (Error_Detected()) {
                currentState = STATE_ERROR;
            }
            System_Process();
            break;
            
        case STATE_PAUSED:
            if (ResumeButton_Pressed()) {
                currentState = STATE_RUNNING;
            }
            break;
            
        case STATE_ERROR:
            if (ResetButton_Pressed()) {
                currentState = STATE_IDLE;
                System_Reset();
            }
            Error_Handler();
            break;
            
        default:
            // Should never happen
            Error_Handler();
            break;
    }
}
```

#### Mengapa FSM Penting di Embedded?

1. **Predictable behavior** - Selalu tahu sistem ada di state mana
2. **Easy to debug** - Bisa log state transitions
3. **Maintainable** - Mudah menambah state baru
4. **No blocking code** - Non-blocking, cocok untuk real-time systems

### 5. Flowchart Thinking

#### Kapan Menggunakan Flowchart?

- Sebelum menulis kode kompleks
- Saat menjelaskan algoritma ke team
- Saat debugging logic error
- Saat dokumentasi

#### Contoh Flowchart: ADC Reading dengan Validation

```
┌─────────────────────────────────────────────────────────┐
│                                                         │
│           [START]                                       │
│              │                                          │
│              ↓                                          │
│     [Init ADC]                                          │
│              │                                          │
│              ↓                                          │
│     [Start Conversion]                                  │
│              │                                          │
│              ↓                                          │
│     <Conversion Complete?> ---No--→ [Wait]             │
│              │ Yes                                      │
│              ↓                                          │
│     [Read ADC Value]                                    │
│              │                                          │
│              ↓                                          │
│     <Value Valid?> ---No--→ [Error Handler]           │
│              │ Yes                                      │
│              ↓                                          │
│     [Filter/Process]                                    │
│              │                                          │
│              ↓                                          │
│     [Store/Transmit]                                    │
│              │                                          │
│              ↓                                          │
│           [END]                                         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### 6. Reading Documentation

#### Hierarki Dokumentasi Embedded

```
┌─────────────────────────────────────────────────────────┐
│            DOCUMENTATION HIERARCHY                      │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Level 1: Datasheet                                     │
│  • Spesifikasi hardware                                 │
│  • Electrical characteristics                           │
│  • Pin definitions                                      │
│                                                         │
│  Level 2: Reference Manual                              │
│  • Register descriptions                                │
│  • Peripheral operation                                 │
│  • Programming guidelines                               │
│                                                         │
│  Level 3: Application Notes                             │
│  • Use cases spesifik                                   │
│  • Best practices                                       │
│  • Example implementations                              │
│                                                         │
│  Level 4: HAL/LL Library Documentation                  │
│  • API reference                                        │
│  • Usage examples                                       │
│  • Migration guides                                     │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### 7. Best Practices Engineer Profesional

#### Coding Best Practices

1. **Nama variabel yang descriptive**
   ```c
   // ❌ Buruk
   int d = 100;
   
   // ✅ Baik
   const int SENSOR_READ_INTERVAL_MS = 100;
   ```

2. **Fungsi kecil, satu tanggung jawab**
   ```c
   // ❌ Buruk: Fungsi melakukan terlalu banyak
   void Process_Data(void) {
       // Read sensor
       // Filter data
       // Send via UART
       // Log to SD card
       // Update display
   }
   
   // ✅ Baik: Terpisah per tanggung jawab
   void Sensor_Read(void);
   void Data_Filter(void);
   void UART_Transmit(void);
   void SDCard_Log(void);
   void Display_Update(void);
   ```

3. **Comment the WHY, not the WHAT**
   ```c
   // ❌ Tidak membantu
   i++;  // Increment i
   
   // ✅ Menjelaskan alasan
   i++;  // Skip header byte sesuai protokol v2.1
   ```

#### Documentation Best Practices

1. **Dokumentasi hidup bersama kode**
   - Update dokumentasi saat update kode
   - Versi dokumentasi match dengan versi kode

2. **Asumsi harus tertulis**
   ```
   ASUMSI:
   - Input voltage: 3.3V ±5%
   - Operating temperature: -40°C to 85°C
   - Max sampling rate: 1kHz
   ```

3. **Known issues harus didokumentasi**
   ```
   KNOWN ISSUES:
   - [BUG-001] ADC reading drift setelah 24 jam
     Workaround: Recalibrate setiap 12 jam
     Status: Under investigation
   ```

---

## Contoh Embedded: Implementasi Complete FSM

```c
/**
 * @file temperature_controller.c
 * @brief Temperature Control System menggunakan FSM
 * 
 * Sistem ini mengontrol heater berdasarkan temperature input
 * dengan hysteresis untuk mencegah oscillation.
 */

#include <stdint.h>
#include <stdbool.h>

/* State Definitions */
typedef enum {
    TEMP_STATE_INIT,
    TEMP_STATE_IDLE,
    TEMP_STATE_HEATING,
    TEMP_STATE_COOLING,
    TEMP_STATE_ERROR_OVERTEMP,
    TEMP_STATE_ERROR_SENSOR
} TemperatureState_t;

/* Configuration Constants */
#define TEMP_TARGET_CELSIUS     50.0f
#define TEMP_HYSTERESIS_CELSIUS 2.0f
#define TEMP_MAX_CELSIUS        80.0f
#define TEMP_MIN_CELSIUS        0.0f

/* State Variables */
static TemperatureState_t currentState = TEMP_STATE_INIT;
static float currentTemperature = 0.0f;
static uint32_t errorCounter = 0;

/* Function Prototypes */
static float Sensor_ReadTemperature(void);
static void Heater_Enable(void);
static void Heater_Disable(void);
static void Error_Handler(const char* message);

/**
 * @brief Main state machine update function
 * @note Dipanggil secara periodik (misal: setiap 100ms)
 */
void TemperatureController_Update(void) {
    currentTemperature = Sensor_ReadTemperature();
    
    switch(currentState) {
        case TEMP_STATE_INIT:
            // Initialisasi sistem
            Heater_Disable();
            errorCounter = 0;
            
            // Validasi sensor
            if (currentTemperature > TEMP_MIN_CELSIUS && 
                currentTemperature < TEMP_MAX_CELSIUS) {
                currentState = TEMP_STATE_IDLE;
            } else {
                currentState = TEMP_STATE_ERROR_SENSOR;
                Error_Handler("Sensor out of range during init");
            }
            break;
            
        case TEMP_STATE_IDLE:
            // Tunggu perintah atau kondisi start
            if (currentTemperature < (TEMP_TARGET_CELSIUS - TEMP_HYSTERESIS_CELSIUS)) {
                currentState = TEMP_STATE_HEATING;
                Heater_Enable();
            }
            break;
            
        case TEMP_STATE_HEATING:
            // Cek apakah sudah mencapai target
            if (currentTemperature >= TEMP_TARGET_CELSIUS) {
                currentState = TEMP_STATE_IDLE;
                Heater_Disable();
            }
            // Safety check: overtemperature
            else if (currentTemperature >= TEMP_MAX_CELSIUS) {
                currentState = TEMP_STATE_ERROR_OVERTEMP;
                Heater_Disable();
                Error_Handler("Overtemperature detected");
            }
            break;
            
        case TEMP_STATE_COOLING:
            // Similar logic untuk cooling mode
            if (currentTemperature <= TEMP_TARGET_CELSIUS) {
                currentState = TEMP_STATE_IDLE;
            }
            break;
            
        case TEMP_STATE_ERROR_OVERTEMP:
        case TEMP_STATE_ERROR_SENSOR:
            // Stay in error state until manual reset
            Heater_Disable();
            errorCounter++;
            
            // Auto-recover jika error counter belum threshold
            if (errorCounter > 100) {
                currentState = TEMP_STATE_INIT;
                errorCounter = 0;
            }
            break;
            
        default:
            // Invalid state - force init
            currentState = TEMP_STATE_INIT;
            Error_Handler("Invalid state detected");
            break;
    }
}

/**
 * @brief Get current system state (untuk monitoring/debugging)
 */
TemperatureState_t TemperatureController_GetState(void) {
    return currentState;
}

/**
 * @brief Manual reset function
 */
void TemperatureController_Reset(void) {
    currentState = TEMP_STATE_INIT;
    errorCounter = 0;
    Heater_Disable();
}

/* Dummy implementations - replace with actual hardware code */
static float Sensor_ReadTemperature(void) {
    // Implementasi actual: baca ADC, konversi ke Celsius
    return 25.0f;
}

static void Heater_Enable(void) {
    // Implementasi actual: set GPIO high, enable PWM, dll
}

static void Heater_Disable(void) {
    // Implementasi actual: set GPIO low, disable PWM, dll
}

static void Error_Handler(const char* message) {
    // Implementasi actual: log error, blink LED, send notification
    (void)message; // Suppress unused warning
}
```

---

## Kesalahan yang Sering Terjadi

### 1. Langsung Coding Tanpa Planning

**Salah:**
```
Problem → Langsung buka IDE → Coding → Debug berjam-jam
```

**Benar:**
```
Problem → Analisis → Flowchart/FSM → Coding → Test
```

### 2. Mengabaikan Datasheet

**Salah:**
- Copy-paste code dari internet tanpa paham hardware
- Asumsi pin configuration sama untuk semua MCU
- Tidak cek electrical limits

**Benar:**
- Selalu cross-reference dengan datasheet
- Verifikasi register settings
- Cek timing requirements

### 3. FSM dengan Blocking Code

**Salah:**
```c
case STATE_WAITING:
    while (!button_pressed) {
        // BLOCKING! Sistem tidak responsif
    }
    currentState = STATE_NEXT;
    break;
```

**Benar:**
```c
case STATE_WAITING:
    if (button_pressed) {
        currentState = STATE_NEXT;
    }
    // Non-blocking, bisa handle task lain
    break;
```

### 4. Tidak Ada Error Handling

**Salah:**
```c
value = ADC_Read();
// Langsung pakai value tanpa validasi
process(value);
```

**Benar:**
```c
value = ADC_Read();
if (ADC_IsValid(value)) {
    process(value);
} else {
    Error_Handler(ADC_ERROR_INVALID);
    currentState = STATE_ERROR;
}
```

### 5. Dokumentasi Setelah Selesai (Yang Tidak Pernah Selesai)

**Salah:**
- "Nanti saja dokumentasinya kalau sudah selesai"
- Kode berubah, dokumentasi tidak

**Benar:**
- Dokumentasi adalah bagian dari definition of done
- Update dokumentasi bersamaan dengan kode

---

## Tips Engineer Profesional

### 💡 Tip 1: Rubber Duck Debugging

Jelaskan masalah Anda kepada bebek karet (atau objek apapun). Proses verbalisasi sering kali membuat solusi muncul.

**Kenapa works?**
- Memaksa otak untuk struktur thinking
- Seringkali menyadari asumsi yang salah saat menjelaskan

### 💡 Tip 2: The 15-Minute Rule

Jika stuck lebih dari 15 menit:
1. Stop coding
2. Jalan-jalan sebentar
3. Tulis ulang masalah di kertas
4. Minta bantuan dengan prepared context

### 💡 Tip 3: Keep an Engineering Journal

Catat setiap hari:
- Masalah yang dihadapi
- Solusi yang dicoba
- Apa yang berhasil/gagal
- Lesson learned

**Template:**
```markdown
## Date: 2024-01-15

### Problem
UART communication gagal setelah 10 menit operasi

### Hypothesis Tested
1. Buffer overflow - ❌
2. Clock drift - ❌
3. Interrupt priority conflict - ✅

### Solution
Ubah interrupt priority UART lebih tinggi dari timer

### Lesson
Selalu cek NVIC priority configuration untuk multiple interrupts
```

### 💡 Tip 4: Learn to Read Schematics

Skill wajib engineer embedded:
- Trace signal path
- Identify power rails
- Understand pull-up/pull-down
- Recognize common circuits

### 💡 Tip 5: Version Control Everything

Bukan hanya code:
- Schematics
- PCB layout
- Documentation
- Configuration files

```bash
# Commit message format yang baik
git commit -m "fix: UART buffer overflow pada high baudrate

- Increase RX buffer size dari 64 ke 256 bytes
- Add overflow detection dan recovery
- Update documentation dengan new limits

Fixes issue #123"
```

---

## Ringkasan

### Key Takeaways Day 01

1. **Engineer Mindset** = Systematic problem solving, bukan sekadar coding
2. **Debugging** adalah proses ilmiah: hypothesis → test → verify
3. **Datasheet** adalah sumber kebenaran utama, bukan tutorial
4. **FSM** adalah pattern fundamental untuk embedded systems
5. **Flowchart** membantu visualisasi logic sebelum coding
6. **Documentation** adalah investasi, bukan beban
7. **Best practices** dibentuk dari pengalaman, pelajari sejak dini

### Checklist Pemahaman

- [ ] Saya bisa menjelaskan problem-solving framework
- [ ] Saya memahami debugging workflow yang sistematis
- [ ] Saya tahu cara membaca datasheet secara efektif
- [ ] Saya bisa membuat FSM sederhana
- [ ] Saya mengerti pentingnya flowchart
- [ ] Saya akan mulai dokumentasi sejak awal proyek

---

## Referensi Belajar

### Books
1. **"The Pragmatic Programmer"** - Andrew Hunt & David Thomas
2. **"Clean Code"** - Robert C. Martin
3. **"Making Embedded Systems"** - Elecia White
4. **"Test Driven Development for Embedded C"** - James W. Grenning

### Online Resources
1. [STM32 Datasheets](https://www.st.com/en/microcontrollers-microprocessors/stm32-mainstream-arm-cortex-mcus.html)
2. [ARM Infocenter](https://developer.arm.com/documentation)
3. [Embedded Artistry](https://embeddedartistry.com/)
4. [Jacob Beningo's Blog](https://www.beningo.com/)

### Videos
1. Phil's Lab - STM32 Tutorials
2. FastBit Embedded Brain Academy
3. Digi-Key's Introduction to Embedded Systems

### Tools
1. STM32CubeMX - Configuration tool
2. STM32CubeIDE - Development environment
3. Saleae Logic Analyzer - Debugging tool
4. Doxygen - Documentation generator

---

> **"First, solve the problem. Then, write the code."** — John Johnson

*Lanjutkan ke Day 02: Embedded C Programming*
