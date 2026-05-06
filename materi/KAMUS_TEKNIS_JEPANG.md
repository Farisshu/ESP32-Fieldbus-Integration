# 🇯🇵 Kamus Teknis & Budaya Kerja Jepang untuk Engineer Otomotif/Elektronika

Dokumen ini dirancang khusus untuk persiapan magang di perusahaan Jepang (Otomotive/Electronics). Fokus pada istilah teknis yang sering muncul dalam datasheet, dokumentasi, dan komunikasi sehari-hari di lingkungan teknik.

---

## 1. Kosakata Dasar Teknik (Gijutsu Yougo)

| Indonesia | English | Japanese (Kanji) | Japanese (Hiragana/Katakana) | Romaji | Konteks Penggunaan |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Listrik & Elektronika** | | | | | |
| Listrik | Electricity | 電気 | でんき | Denki | Umum |
| Tegangan | Voltage | 電圧 | でんあつ | Den'atsu | Circuit design |
| Arus | Current | 電流 | でんりゅう | Denryū | Circuit analysis |
| Hambatan | Resistance | 抵抗 | ていこう | Teikō | Resistor component |
| Kapasitor | Capacitor | 容量 / コンデンサ | ようりょう / ... | Yōryō / Condensha | Komponen |
| Induktansi | Inductance | インダクタンス | ... | Indkutansu | Coil/Inductor |
| Sinyal | Signal | 信号 | しんごう | Shingō | Communication |
| Rangkaian | Circuit | 回路 | かいろ | Kairo | PCB/Schematic |
| Papan Sirkuit | Circuit Board | 基板 | きたん | Kitan | PCB |
| **Komunikasi Data** | | | | | |
| Komunikasi | Communication | 通信 | つうしん | Tsūshin | UART, CAN, SPI |
| Kecepatan | Speed/Rate | 速度 | そくど | Sokudo | Baudrate |
| Kesalahan | Error | エラー / 誤り | ... / あやまり | Erā / Ayamari | Error frame |
| Protokol | Protocol | プロトコル | ... | Purotokoru | Standard comms |
| Antarmuka | Interface | 界面 / インターフェース | ... / Intāfēsu | Menkai / Interface | Hardware connection |
| Pengiriman | Transmission | 送信 | そうしん | Sōshin | TX pin |
| Penerimaan | Reception | 受信 | じゅしん | Jushin | RX pin |
| Sinkronisasi | Synchronization | 同期 | どうき | Dōki | SPI/Clock |
| Asinkronus | Asynchronous | 非同期 | ひどうき | Hidōki | UART |
| **Komponen & Hardware** | | | | | |
| Mikrokontroler | Microcontroller | 制御装置 | せいぎょそち | Seigyosochi | MCU |
| Sensor | Sensor | センサー | ... | Sensā | Input device |
| Aktuator | Actuator | 作動機 | さどうき | Sadōki | Motor/Valve |
| Memori | Memory | 記憶装置 | きおくそち | Kiokusochi | Flash/RAM |
| Pin | Pin | 端子 | たんし | Tanshi | GPIO pin |
| Port | Port | ポート | ... | Pōto | I/O Port |
| **Pengembangan & Debugging** | | | | | |
| Pengembangan | Development | 開発 | かいはつ | Kaihatsu | R&D Dept |
| Pengujian | Testing | 試験 / テスト | しけん / ... | Shiken / Test | Validation |
| Verifikasi | Verification | 検証 | けんしょう | Kenshō | Code review |
| Debug | Debug | デバッグ | ... | Debaggugu | Fixing bugs |
| Logam | Metal | 金属 | きんぞく | Kinzoku | Shielding |
| Sambungan | Connection | 接続 | せつぞく | Setsuzoku | Wiring |

---

## 2. Istilah Khusus CAN Bus & Otomotif

Dalam industri otomotif Jepang (Toyota/Honda/Nissan), istilah ini sangat krusial.

| Istilah | Japanese | Romaji | Penjelasan |
| :--- | :--- | :--- | :--- |
| **Jaringan Kendaraan** | 車載ネットワーク | Shasai Nettowāku | In-vehicle network |
| **Unit Kontrol Elektronik** | 電子制御ユニット | Denshi Seigyo Yunitto | ECU (Sering disingkat ECU juga) |
| **Bus Data** | データバス | Dēta Basu | Jalur data bersama |
| **Frame** | フレーム | Furēmu | Paket data CAN |
| **ID Arbitrase** | 調停 ID | Chōtei ID | Arbitration ID (Prioritas) |
| **Node** | ノード | Nōdo | Titik koneksi (ECU) |
| **Dominan** | ドミナント | Dominanto | Logika 0 (Wired-AND) |
| **Resesif** | レセシブ | Reseshibu | Logika 1 (Default state) |
| **Checksum** | 検査サム | Kensa Samu | CRC verification |
| **Error Frame** | エラーフレーム | Erā Furēmu | Sinyal kesalahan |

> **💡 Fakta Menarik:** Di Jepang, sistem CAN sering disebut sebagai bagian dari *"Shasai LAN"* (Vehicle LAN).

---

## 3. Frasa Penting di Lingkungan Kerja (Genba)

Saat magang, Anda mungkin mendengar instruksi singkat ini di laboratorium atau lantai produksi.

### A. Instruksi Keselamatan & Persiapan
| Jepang | Romaji | Arti | Kapan Digunakan |
| :--- | :--- | :--- | :--- |
| 電源を切ってください。 | Dengen o kitte kudasai. | Matikan power. | Sebelum merakit hardware |
| 配線を確認してください。 | Haisen o kakunin shite kudasai. | Periksa kabelnya. | Sebelum menyalakan alat |
| 静電気に注意してください。 | Seidenki ni chūi shite kudasai. | Hati-hati listrik statis. | Saat memegang IC/MCU |
| 短絡しないでください。 | Tanraku shinaide kudasai. | Jangan sampai korsleting. | Saat wiring |
| 再起動しますか？ | Sakidō shimasu ka? | Apakah akan restart? | Konfirmasi sistem |

### B. Diskusi Teknis & Debugging
| Jepang | Romaji | Arti | Konteks |
| :--- | :--- | :--- | :--- |
| 動きません。 | Ugokimasen. | Tidak jalan/berfungsi. | Melaporkan bug |
| 原因は何ですか？ | Gen'in wa nan desu ka? | Apa penyebabnya? | Diskusi root cause |
| 仕様を確認しましょう。 | Shiyō o kakunin shimashō. | Mari cek spesifikasinya. | Referensi datasheet |
| 合っていますか？ | Atte imasu ka? | Apakah sudah benar? | Validasi hasil |
| 遅れています。 | Okurete imasu. | Terlambat/Lagging. | Masalah timing/delay |
| 通信できません。 | Tsūshin dekimasen. | Tidak bisa komunikasi. | Error UART/CAN |

---

## 4. Etika Kerja Jepang untuk Engineer (Shitsuke)

Memahami budaya sama pentingnya dengan bahasa.

1.  **Horenso (Ho-Ren-So):**
    *   **Ho**koku (Lapor): Lapor status progres secara berkala, jangan tunggu selesai.
    *   **Ren**raku (Hubungi): Informasikan perubahan jadwal atau masalah segera.
    *   **So**dan (Konsultasi): Minta saran sebelum mengambil keputusan besar jika ragu.
    *   *Penerapan:* Jika kode CAN Bus Anda error berhari-hari, segera lakukan "Sodan" kepada mentor.

2.  **Kaizen (Perbaikan Berkelanjutan):**
    *   Selalu ada cara untuk membuat kode lebih efisien atau rangkaian lebih rapi. Jangan puas dengan "yang penting jalan".

3.  **Nemawashi (Akar Putaran):**
    *   Proses konsensus informal sebelum rapat resmi. Pastikan rekan tim setuju dengan ide desain Anda sebelum dipresentasikan ke atasan.

4.  **Genchi Genbutsu (Lokasi Nyata, Barang Nyata):**
    *   Jangan cuma lihat log di komputer. Pergi ke lokasi alat (hardware), cek sinyal pakai Osiloskop, rasakan panasnya komponen. Masalah sebenarnya ada di sana.

---

## 5. Cara Membaca Datasheet Komponen Jepang

Banyak komponen (Renesas, Rohm, Toshiba, Kioxia) memiliki datasheet berbahasa Jepang atau campuran.

*   **目次 (Mokuji):** Daftar Isi
*   **概要 (Gaiyō):** Deskripsi Umum / Overview
*   **絶対最大定格 (Zettai Saidai Teikaku):** Absolute Maximum Ratings (JANGAN LEBIHI INI!)
*   **電気的特性 (Denkiteki Tokusei):** Electrical Characteristics (Voh, Vol, Icc, dll)
*   **ピン配置 (Pin Haichi):** Pin Configuration / Pinout
*   **使用方法 (Shiyō Hōhō):** Cara Penggunaan / Application Notes
*   **注意事項 (Chūi Jikō):** Peringatan / Cautions

---

## 6. Latihan Singkat: Studi Kasus CAN Bus

Coba terjemahkan kalimat teknis berikut dalam hati:

> **Kalimat:** "CAN bus no tsūshin sokudo wa 500 kbps desu ga, erā ga okite imasu."
>
> **Analisis:**
> *   CAN bus no = Milik CAN bus (Partikel 'no')
> *   tsūshin sokudo = Kecepatan komunikasi
> *   wa = Topik marker
> *   500 kbps desu ga = Adalah 500 kbps, tetapi... ('ga' = tapi)
> *   erā ga okite imasu = Sedang terjadi error.
>
> **Arti:** "Kecepatan komunikasi CAN bus adalah 500 kbps, tetapi sedang terjadi error."

---

## 7. Rekomendasi Belajar Lanjutan

1.  **JLPT N4/N3:** Targetkan level ini untuk kemampuan percakapan dasar.
2.  **Nihongo e-Go! (Jepang):** Aplikasi gratis dari pemerintah Jepang.
3.  **Teknical Terms:** Cari "Jisho.org" untuk kamus teknis.
4.  **Budaya:** Tonton video tentang "Monozukuri" (Seni membuat barang) untuk memahami filosofi manufaktur Jepang.

---

*Dibuat sebagai pendukung repository pembelajaran Embedded System & CAN Bus.*
*Semoga sukses persiapannya!* 🇮🇩 🤝 🇯🇵
