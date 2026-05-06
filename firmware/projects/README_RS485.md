# 📡 RS485 Industrial Communication (XY-017 Module)

Proyek ini mengimplementasikan komunikasi serial half-duplex menggunakan standar industri **RS485** dengan modul **XY-017 RS485 to TTL**. Implementasi ini mensimulasikan sistem monitoring sensor terdistribusi yang umum digunakan di industri.

## 🎯 Fitur Utama

- ✅ **Loopback Test**: Validasi hardware dan wiring modul RS485
- ✅ **Master-Slave Communication**: Komunikasi point-to-point antara 2 ESP32
- ✅ **Simulasi Sensor Data**: Temperature & Humidity dengan variasi realistis
- ✅ **Protocol Parsing**: Command-response protocol dengan ACK/NACK
- ✅ **Auto Direction Control**: Kontrol otomatis pin DE/RE untuk half-duplex
- ✅ **Error Handling**: Deteksi command tidak dikenal dan format error

## 🛠️ Hardware Requirements

| Component | Quantity | Keterangan |
|-----------|----------|------------|
| ESP32 DevKit V1 | 2 unit | Untuk mode Master & Slave |
| XY-017 RS485 Module | 2 unit | Modul converter RS485 to TTL |
| Kabel Jumper | Secukupnya | Untuk koneksi GPIO & terminal |
| Resistor 120Ω | 1 buah | Terminasi bus (opsional, untuk kabel panjang) |

### 📌 Pinout Configuration

| XY-017 Pin | ESP32 GPIO | Fungsi |
|------------|------------|--------|
| VCC | 5V / 3.3V | Power (sesuaikan jumper modul) |
| GND | GND | **Wajib Common Ground** ⚠️ |
| RO (Receiver Out) | GPIO 16 (RX2) | Data dari modul ke ESP32 |
| DI (Driver In) | GPIO 17 (TX2) | Data dari ESP32 ke modul |
| DE/RE (Direction) | GPIO 4 | Kontrol arah transmit/receive |
| Terminal A | Kabel A (+) | Differential pair non-inverting |
| Terminal B | Kabel B (-) | Differential pair inverting |

> ⚠️ **PENTING**: Pastikan **GND** dari kedua ESP32 terhubung bersama. Tanpa common ground, komunikasi akan gagal atau menghasilkan noise.

## 📂 Struktur Folder

```
firmware/projects/
├── rs485_loopback_test/       # Test 1: Validasi Hardware Single ESP
│   ├── src/
│   │   └── main.cpp           # Kode loopback test
│   └── platformio.ini         # Konfigurasi PlatformIO
│
└── rs485_master_slave/        # Test 2: Komunikasi 2 ESP (Master-Slave)
    ├── src/
    │   ├── master.cpp         # Kode node MASTER
    │   └── slave.cpp          # Kode node SLAVE
    └── platformio.ini         # Konfigurasi PlatformIO (2 environments)
```

## 🧪 Testing Scenarios

### Test 1: Loopback Test (Validasi Hardware)

**Tujuan**: Memastikan modul XY-017 berfungsi dengan benar sebelum komunikasi multi-device.

#### 🔧 Wiring Setup
1. Pasang modul XY-017 ke ESP32 sesuai tabel pinout di atas.
2. **JUMPER pin A dan B** pada terminal blok modul RS485 dengan kabel pendek.
   ```
   [Terminal A] ----jumper---- [Terminal B]
   ```
3. Upload firmware `rs485_loopback_test` ke ESP32.

#### 📋 Cara Menjalankan
```bash
cd firmware/projects/rs485_loopback_test
pio run --target upload --environment rs485_loopback
pio device monitor --environment rs485_loopback
```

#### ✅ Expected Output
```
========================================
🚀 RS485 Loopback Test Started
========================================
⚠️ PENTING: Pastikan pin A dan B pada modul RS485 di-jumper!
   (Hubungkan terminal A ke terminal B dengan kabel pendek)
========================================

✅ Module Initialized. Starting loopback test...
   - Mengirim data setiap 2 detik
   - Menerima data dari port yang sama (loopback)

📤 [TX] Sending: LOOPBACK_TEST_ID:12345
📥 [RX] Received: LOOPBACK_TEST_ID:12345
   ✅ SUCCESS: Data valid diterima kembali!

📤 [TX] Sending: LOOPBACK_TEST_ID:14567
📥 [RX] Received: LOOPBACK_TEST_ID:14567
   ✅ SUCCESS: Data valid diterima kembali!
```

Jika muncul `SUCCESS`, hardware siap untuk test berikutnya!

---

### Test 2: Master-Slave Communication (Point-to-Point)

**Tujuan**: Simulasi komunikasi industri nyata antara controller (Master) dan sensor node (Slave).

#### 🔧 Wiring Setup
1. Siapkan **2 set ESP32 + XY-017**.
2. Hubungkan kedua modul RS485:
   - **A ↔ A** (Terminal A modul 1 ke Terminal A modul 2)
   - **B ↔ B** (Terminal B modul 1 ke Terminal B modul 2)
   - **GND ↔ GND** (Wajib!)
3. Lepaskan jumper A-B jika masih terpasang dari test sebelumnya.

```
[ESP32 Master] --- [XY-017 #1] ====== [XY-017 #2] --- [ESP32 Slave]
                      A  B              A  B
                      |  |==============|  |
                      |  |==============|  |
                     GND|==============|GND
```

#### 📋 Cara Menjalankan

**Upload ke ESP32 #1 (MASTER):**
```bash
cd firmware/projects/rs485_master_slave
pio run --target upload --environment rs485_master
pio device monitor --environment rs485_master
```

**Upload ke ESP32 #2 (SLAVE):**
```bash
cd firmware/projects/rs485_master_slave
pio run --target upload --environment rs485_slave
pio device monitor --environment rs485_slave
```

#### ✅ Expected Output

**Di Serial Monitor MASTER:**
```
========================================
🤖 RS485 MASTER Node Started
========================================
Role: Mengirim command dan menerima response dari SLAVE
========================================

✅ RS485 Module Initialized. Waiting to send commands...

📤 [MASTER TX] Sending Command: CMD:READ_DATA|TS:123456
📥 [MASTER RX] Received Response: ACK|TEMP:25.3|HUM:59.8|TS:123789|ID:01
   ✅ SUCCESS: Valid response from SLAVE received!
   🌡️ Temperature Data: 25.3

📤 [MASTER TX] Sending Command: CMD:READ_DATA|TS:126456
📥 [MASTER RX] Received Response: ACK|TEMP:25.1|HUM:60.2|TS:129012|ID:01
   ✅ SUCCESS: Valid response from SLAVE received!
   🌡️ Temperature Data: 25.1
```

**Di Serial Monitor SLAVE:**
```
========================================
👤 RS485 SLAVE Node Started (ID: 01)
========================================
Role: Menerima command dari MASTER dan mengirim response
========================================

✅ RS485 Module Initialized. Listening for commands...

📥 [SLAVE RX] Received Command: CMD:READ_DATA|TS:123456
   ✅ Processing READ_DATA command...
📤 [SLAVE TX] Sending Response: ACK|TEMP:25.3|HUM:59.8|TS:123789|ID:01

📥 [SLAVE RX] Received Command: CMD:READ_DATA|TS:126456
   ✅ Processing READ_DATA command...
📤 [SLAVE TX] Sending Response: ACK|TEMP:25.1|HUM:60.2|TS:129012|ID:01
```

#### 📝 Supported Commands

| Command | Deskripsi | Response Format |
|---------|-----------|-----------------|
| `CMD:READ_DATA` | Request data sensor simulasi | `ACK\|TEMP:xx.x\|HUM:xx.x\|TS:xxxxx\|ID:xx` |
| `CMD:PING` | Cek konektivitas slave | `ACK\|PONG\|ID:xx\|STAT:OK` |
| `CMD:GET_ID` | Request informasi ID slave | `ACK\|SLAVE_ID:xx\|FW:1.0.0` |
| Unknown | Command tidak dikenali | `NACK\|ERROR:UNKNOWN_CMD\|RCVD:...` |

## 🔍 Troubleshooting

| Masalah | Kemungkinan Penyebab | Solusi |
|---------|----------------------|--------|
| **Garbage Data** (karakter aneh) | Baudrate tidak cocok / Ground floating | 1. Cek `BAUD_RATE` sama di semua device<br>2. **Sambungkan GND** antar semua modul |
| **Tidak ada respon** | Pin DE/RE terbalik atau timing salah | 1. Pastikan wiring DE/RE ke GPIO 4<br>2. Cek delay transisi (100µs sudah OK) |
| **Hanya bisa kirim atau terima** | Jumper A-B masih terpasang (untuk test 2) | Lepas jumper A-B setelah selesai loopback test |
| **Noise tinggi / data corrupt** | Kabel terlalu panjang tanpa terminasi | Tambahkan resistor **120Ω** di ujung paling jauh dari bus |
| **Slave tidak merespon command tertentu** | Format command salah | Pastikan command dimulai dengan `CMD:` dan case-sensitive |

## 🏭 Relevansi Industri

Implementasi ini mensimulasikan skenario nyata di industri:

- **Modbus RTU**: Protocol ini menggunakan pola master-slave yang sama
- **Sensor Networks**: Multiple sensor nodes dalam satu bus RS485
- **PLC Communication**: PLC sebagai master, device field sebagai slave
- **Building Automation**: HVAC monitoring dengan distributed sensors

### 🚀 Next Steps (Pengembangan Lanjutan)

1. **Multi-Drop Network**: Tambahkan lebih dari 1 slave dengan ID berbeda
2. **Modbus RTU Library**: Implementasi protocol Modbus standar industri
3. **Error Detection**: Tambahkan CRC checksum untuk integritas data
4. **Non-blocking Communication**: Gunakan state machine untuk handling timeout
5. **Data Logging**: Simpan data sensor ke LittleFS atau kirim via WiFi

## 📚 Referensi

- [RS485 Communication Protocol](https://en.wikipedia.org/wiki/RS-485)
- [XY-017 Module Datasheet](https://docs.platformio.org/en/latest/boards/espressif32/esp32dev.html)
- [Arduino Serial2 Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/hardware_serial.html)

---

**Dibuat untuk Portfolio Embedded System Engineer**  
*Demonstrasi kemampuan industrial communication, protocol design, dan debugging hardware-software integration.*
