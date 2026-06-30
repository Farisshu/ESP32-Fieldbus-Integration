# Exercises - Day 01: Engineer Mindset

## Latihan Pemahaman Konsep

---

## Exercise 1.1: Problem Solving Framework

### Studi Kasus: Sensor Tidak Membaca Data

**Skenario:**
Sebuah sistem embedded menggunakan sensor suhu I2C. Tiba-tiba, data yang dibaca selalu menunjukkan nilai 0.

**Tugas:**
Gunakan framework problem solving yang telah dipelajari untuk menganalisis masalah ini.

```
1. DEFINE the problem
   - Apa yang seharusnya terjadi?
   - Apa yang sebenarnya terjadi?
   - Kapan masalah mulai muncul?

2. BREAK DOWN the problem
   - Hardware atau software?
   - Komponen mana yang mungkin bermasalah?
   - Apa yang berubah sebelum masalah muncul?

3. PRIORITIZE
   - Masalah mana yang harus dicek dulu?
   - Mana yang paling mudah diverifikasi?

4. PLAN
   - Langkah-langkah troubleshooting apa yang akan dilakukan?
   - Tools apa yang dibutuhkan?

5. EXECUTE
   - Lakukan pengecekan sesuai prioritas
   - Dokumentasikan setiap langkah dan hasilnya
```

**Deliverable:** Buat flowchart troubleshooting dalam format markdown atau gambar.

---

## Exercise 1.2: Reading Datasheet

### Tugas: Ekstrak Informasi dari Datasheet

Pilih salah satu datasheet berikut (atau datasheet MCU yang kamu miliki):
- STM32F103x8/B
- ATmega328P
- ESP32

**Tugas:**
Temukan dan catat informasi berikut:

1. **Operating Voltage Range**
   - Minimum: _____ V
   - Maximum: _____ V
   - Recommended: _____ V

2. **Clock System**
   - Max CPU Frequency: _____ MHz
   - External crystal range: _____ - _____ MHz
   - Internal oscillator frequency: _____ MHz

3. **Memory**
   - Flash size: _____ KB
   - SRAM size: _____ KB
   - EEPROM size: _____ bytes (jika ada)

4. **GPIO**
   - Total GPIO pins: _____
   - Max current per pin: _____ mA
   - Total max current all pins: _____ mA

5. **Peripherals**
   - Number of UART: _____
   - Number of SPI: _____
   - Number of I2C: _____
   - ADC resolution: _____ bits
   - Number of ADC channels: _____

6. **Power Consumption**
   - Active mode: _____ mA/MHz
   - Sleep mode: _____ μA
   - Stop mode: _____ μA

**Refleksi:**
- Berapa lama waktu yang dibutuhkan untuk menemukan semua informasi?
- Bagian mana dari datasheet yang paling sulit dipahami?
- Informasi apa yang paling sering kamu butuhkan?

---

## Exercise 1.3: Flowchart Design

### Tugas: Buat Flowchart untuk Sistem

Pilih salah satu sistem berikut:

**Option A: Sistem Parkir Otomatis**
- Gate terbuka jika ada kartu RFID valid
- Gate tertutup setelah mobil masuk (deteksi sensor)
- LED hijau menyala jika ada slot tersedia
- LED merah menyala jika penuh
- Buzzer berbunyi jika mencoba masuk saat penuh

**Option B: Sistem Penyiram Tanaman Otomatis**
- Baca kelembaban tanah setiap 1 jam
- Jika kelembaban < 30%, nyalakan pompa selama 5 detik
- Jika kelembaban 30-60%, tidak perlu aksi
- Jika kelembaban > 60%, alarm kelebihan air
- Tombol manual untuk override

**Option C: Sistem Kontrol Suhu Ruangan**
- Baca suhu setiap 10 detik
- Jika suhu < 20°C, nyalakan heater
- Jika suhu 20-25°C, tidak perlu aksi
- Jika suhu > 25°C, nyalakan fan
- Jika suhu > 30°C, nyalakan alarm
- Mode eco: heater hanya nyala 50% duty cycle

**Requirements:**
- Gunakan simbol flowchart yang benar
- Minimal 10 boxes
- Include decision points (diamond shape)
- Show all possible paths
- Include error handling

**Tools yang bisa digunakan:**
- draw.io (gratis, online)
- Lucidchart
- Microsoft Visio
- Draw by hand and photograph

---

## Exercise 1.4: FSM Design

### Tugas: Design Finite State Machine

Pilih salah satu sistem dan buat FSM-nya:

**Option A: Mesin Vending Sederhana**
States: Idle, Coin Inserted, Selection Made, Dispensing, Out of Stock, Error

**Option B: Traffic Light Controller**
States: Red, Green, Yellow, Pedestrian Cross, Night Mode, Emergency

**Option C: Washing Machine**
States: Idle, Filling, Washing, Rinsing, Spinning, Draining, Error

**Requirements:**
1. List semua states dengan deskripsi
2. List semua transitions (event/kondisi yang menyebabkan perpindahan state)
3. List semua actions/output di setiap state
4. Gambarkan state transition diagram
5. Implementasi pseudo-code menggunakan switch-case

**Template Jawaban:**

```markdown
## States:
1. STATE_IDLE - Deskripsi: ...
2. STATE_... - Deskripsi: ...

## Transitions:
| From State | Event/Condition | To State |
|------------|-----------------|----------|
| IDLE | coin_inserted | COIN_INSERTED |
| ... | ... | ... |

## Actions per State:
- STATE_IDLE: turn_off_all_outputs(), check_sensors()
- STATE_...: ...

## State Transition Diagram:
[Gambar diagram]

## Pseudo-code Implementation:
void fsm_update(void) {
    switch(current_state) {
        case STATE_IDLE:
            // Actions
            if (condition) {
                current_state = STATE_NEXT;
            }
            break;
        // ... other states
    }
}
```

---

## Exercise 1.5: Debugging Scenario Analysis

### Analisis Kasus Debugging

Baca skenario debugging berikut dan jawab pertanyaan:

**Skenario 1:**
Seorang engineer melaporkan bahwa sistem reset sendiri setiap beberapa menit. Dia sudah mengganti power supply dan memeriksa semua koneksi, tetapi masalah tetap ada.

**Pertanyaan:**
1. Apa 3 hal pertama yang akan kamu cek?
2. Bagaimana cara kamu memverifikasi hipotesis kamu?
3. Apa kemungkinan root cause-nya?

**Skenario 2:**
UART communication bekerja dengan baik di lab, tetapi gagal ketika device dipasang di pabrik dengan banyak motor besar.

**Pertanyaan:**
1. Apa kemungkinan penyebabnya?
2. Test apa yang akan kamu lakukan?
3. Apa solusi jangka pendek dan jangka panjang?

**Skenario 3:**
Button press kadang-kadang terdeteksi multiple times (bounce), menyebabkan input ganda.

**Pertanyaan:**
1. Jelaskan mengapa ini terjadi dari sisi hardware
2. Berikan 2 solusi hardware debouncing
3. Berikan 2 solusi software debouncing
4. Implementasi salah satu solusi software dalam C code

---

## Exercise 1.6: Documentation Practice

### Tugas: Buat Technical Note

Pilih salah satu topik dan buat technical note (1-2 halaman):

**Topik Pilihan:**
- Cara memilih pull-up resistor value untuk I2C
- Best practices untuk GPIO configuration
- Clock tree configuration untuk low-power application
- Interrupt priority assignment strategy

**Format:**
```markdown
# Technical Note: [Judul]

## Objective
[Deskripsi tujuan]

## Background
[Informasi latar belakang]

## Analysis/Calculation
[Analisis atau perhitungan]

## Recommendation
[Rekomendasi/guideline]

## Example
[Contoh implementasi]

## References
[Daftar referensi]
```

---

## Self-Assessment Checklist

Setelah menyelesaikan exercises, cek pemahamanmu:

- [ ] Saya dapat menjelaskan 5-step problem solving framework
- [ ] Saya dapat menemukan informasi penting di datasheet dalam < 5 menit
- [ ] Saya dapat membuat flowchart untuk sistem sederhana
- [ ] Saya dapat mendesign FSM dengan minimal 5 states
- [ ] Saya dapat menganalisis debugging scenario dan memberikan solusi
- [ ] Saya dapat membuat dokumentasi teknis yang jelas

---

## Bonus Challenge

**Challenge: Reverse Engineering Mindset**

Ambil sebuah device elektronik sederhana di rumahmu (remote control, mouse, keyboard, dll).

**Tugas:**
1. Tebak apa komponen utama di dalamnya (tanpa membuka)
2. Buat perkiraan block diagram sistemnya
3. List kemungkinan MCU/peripheral yang digunakan
4. Buat perkiraan flowchart operasinya
5. Jika boleh membuka, dokumentasikan apa yang kamu temukan

**Deliverable:** Laporan 1 halaman dengan foto/gambar.

---

*Selamat belajar! Ingat: Engineer yang baik bukan yang tidak pernah membuat kesalahan, tapi yang belajar dari setiap kesalahan.*
