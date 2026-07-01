# Embedded Systems Engineering - Japanese Technical Guide

## Panduan Terminologi Teknis untuk Embedded Engineer

---

## 1. Komponen Hardware

### Mikrokontroler & Processor

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| マイクロコントローラ | maikoro kontorōra | Microcontroller | Mikrokontroler |
| MPU | MPU | Microprocessor Unit | Mikroprosesor |
| MCU | MCU | Microcontroller Unit | Unit Mikrokontroler |
| コア | koa | Core | Inti |
| クロック | kurokku | Clock | Clock |
| 発振器 | hasshinki | Oscillator | Osilator |
| リセット | risetto | Reset | Reset |
| 割込み | warikomi | Interrupt | Interupsi |
| ベクタ | bekuta | Vector | Vektor |
| プリフェッチ | purifecchi | Prefetch | Prefetch |
| パイプライン | paipurain | Pipeline | Pipeline |
| キャッシュ | kyasshu | Cache | Cache |
| DMA | DMA | Direct Memory Access | Akses Memori Langsung |
| バス | basu | Bus | Bus |
| アドレス | adoresu | Address | Alamat |
| データ | dēta | Data | Data |
| レジスタ | rejisuta | Register | Register |
| スタック | sutakku | Stack | Stack |
| ヒープ | hīpu | Heap | Heap |

### Memory

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| フラッシュメモリ | furasshu memori | Flash Memory | Memori Flash |
| EEPROM | EEPROM | EEPROM | EEPROM |
| SRAM | SRAM | Static RAM | SRAM |
| DRAM | DRAM | Dynamic RAM | DRAM |
| SDRAM | SDRAM | Synchronous DRAM | SDRAM |
| メモリマップ | memori mappu | Memory Map | Peta Memori |
| アロケーション | arokēshon | Allocation | Alokasi |
| ガベージコレクション | gabēji korekushon | Garbage Collection | GC |
| リーク | rīku | Leak | Kebocoran |
| オーバーフロー | ōbāfurō | Overflow | Meluap |
| アンダーフロー | andāfurō | Underflow | Kurang Alir |

### I/O & Peripherals

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| GPIO | GPIO | General Purpose I/O | I/O Serbaguna |
| ポート | pōto | Port | Port |
| ピン | pin | Pin | Pin |
| プルアップ | puru appu | Pull-up | Pull-up |
| プルダウン | puru daun | Pull-down | Pull-down |
| オープンドレイン | ōpun dorain | Open Drain | Open Drain |
| トーテムポール | tōtemu pōru | Totem Pole | Totem Pole |
| シュミットトリガ | shumitto toriga | Schmitt Trigger | Schmitt Trigger |
| ADC | ADC | Analog-to-Digital Converter | Konverter A/D |
| DAC | DAC | Digital-to-Analog Converter | Konverter D/A |
| 分解能 | bunkainō | Resolution | Resolusi |
| サンプリング | sanpuringu | Sampling | Sampling |
| 量子化 | ryōshika | Quantization | Kuantisasi |
| PWM | PWM | Pulse Width Modulation | Modulasi Lebar Pulsa |
| デューティ比 | dyūti hi | Duty Cycle | Duty Cycle |
| タイマ | taima | Timer | Timer |
| カウンタ | kaunta | Counter | Counter |
| ウォッチドッグ | wocchidoggu | Watchdog | Watchdog |
| RTC | RTC | Real-Time Clock | Jam Waktu Nyata |

---

## 2. Protokol Komunikasi

### Serial Communication

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| シリアル通信 | shiriaru tsūshin | Serial Communication | Komunikasi Serial |
| UART | UART | Universal Async Rx/Tx | UART |
| USART | USART | Universal Sync/Async Rx/Tx | USART |
| ボーレート | bōrēto | Baud Rate | Baud Rate |
| ビットレート | bitto rēto | Bit Rate | Laju Bit |
| スタートビット | sutāto bitto | Start Bit | Bit Mulai |
| ストップビット | sutoppu bitto | Stop Bit | Bit Berhenti |
| パリティ | pariti | Parity | Paritas |
| フロー制御 | furō seigyo | Flow Control | Kontrol Alur |
| RTS/CTS | RTS/CTS | Request to Send/Clear to Send | RTS/CTS |
| FIFO | FIFO | First In First Out | FIFO |
| バッファ | baffa | Buffer | Penyangga |
| オーバーラン | ōbāran | Overrun | Overrun |
| フレーミングエラー | furēmingu erā | Framing Error | Error Frame |

### SPI (Serial Peripheral Interface)

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| SPI | SPI | Serial Peripheral Interface | SPI |
| マスタ | masuta | Master | Master |
| スレーブ | surēbu | Slave | Slave |
| MOSI | MOSI | Master Out Slave In | MOSI |
| MISO | MISO | Master In Slave Out | MISO |
| SCLK | SCLK | Serial Clock | Clock Serial |
| SS/CS | SS/CS | Slave Select/Chip Select | Seleksi Chip |
| クロック極性 | kurokku kyokusei | Clock Polarity | Polaritas Clock |
| クロックフェーズ | kurokku fēzu | Clock Phase | Fase Clock |
| LSB First | LSB First | Least Significant Bit First | LSB Duluan |
| MSB First | MSB First | Most Significant Bit First | MSB Duluan |

### I2C (Inter-Integrated Circuit)

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| I2C | I2C | Inter-Integrated Circuit | I2C |
| TWI | TWI | Two-Wire Interface | Antarmuka Dua Kawat |
| SDA | SDA | Serial Data Line | Jalur Data Serial |
| SCL | SCL | Serial Clock Line | Jalur Clock Serial |
| オープンドレイン | ōpun dorain | Open Drain | Open Drain |
| プルアップ抵抗 | puru appu teikō | Pull-up Resistor | Resistor Pull-up |
| アドレス | adoresu | Address | Alamat |
| 7 ビットアドレス | 7 bitto adoresu | 7-bit Address | Alamat 7-bit |
| 10 ビットアドレス | 10 bitto adoresu | 10-bit Address | Alamat 10-bit |
| ACK | ACK | Acknowledge | Pengakuan |
| NACK | NACK | Not Acknowledge | Tidak Diakui |
| アービトレーション | ābitorēshon | Arbitration | Arbitrase |
| クロックストレッチ | kurokku sutoretchi | Clock Stretching | Peregangan Clock |

### CAN Bus

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| CAN | CAN | Controller Area Network | Jaringan Area Kontroler |
| CAN bus | CAN basu | CAN Bus | Bus CAN |
| フレーム | furēmu | Frame | Frame |
| メッセージ | messēji | Message | Pesan |
| ID | ID | Identifier | Pengenal |
| 標準フォーマット | hyōjun fōmatto | Standard Format | Format Standar |
| 拡張フォーマット | kakuchō fōmatto | Extended Format | Format Ekstensi |
| データフレーム | dēta furēmu | Data Frame | Frame Data |
| リモートフレーム | rimōto furēmu | Remote Frame | Frame Remote |
| エラーフレーム | erā furēmu | Error Frame | Frame Error |
| オーバーロードフレーム | ōbārōdo furēmu | Overload Frame | Frame Overload |
| DLC | DLC | Data Length Code | Kode Panjang Data |
| CRC | CRC | Cyclic Redundancy Check | Cek Redundansi Siklik |
| エラーフラグ | erā furagu | Error Flag | Flag Error |
| アクティブエラー | akutibu erā | Active Error | Error Aktif |
| パッシブエラー | passhibu erā | Passive Error | Error Pasif |
| バスオフ | basu ofu | Bus Off | Bus Off |
| ターミネーション | tāminēshon | Termination | Terminasi |
| 120Ω | 120 ohm | 120 Ohm | 120 Ohm |

### Automotive Protocols

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| LIN | LIN | Local Interconnect Network | LIN Bus |
| FlexRay | FlexRay | FlexRay | FlexRay |
| MOST | MOST | Media Oriented Systems Transport | MOST |
| OBD | OBD | On-Board Diagnostics | Diagnostik Kendaraan |
| UDS | UDS | Unified Diagnostic Services | Layanan Diagnostik Terpadu |
| ECU | ECU | Electronic Control Unit | Unit Kontrol Elektronik |
| BCM | BCM | Body Control Module | Modul Kontrol Bod |
| ABS | ABS | Anti-lock Braking System | Sistem Rem Anti-Selip |
| EPS | EPS | Electric Power Steering | Kemudi Tenaga Listrik |
| SRS | SRS | Supplemental Restraint System | Sistem Keamanan Tambahan |

---

## 3. Pemrograman Embedded

### C Programming untuk Embedded

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| ポインタ | pointa | Pointer | Pointer |
| 配列 | hairetsu | Array | Array |
| 構造体 | kōzōtai | Structure | Struktur |
| 共用体 | kyōyōtai | Union | Union |
| ビットフィールド | bitto fīrudo | Bit Field | Medan Bit |
| 型キャスト | kata kyasuto | Type Cast | Casting Tipe |
| volatile | volatile | volatile | volatile |
| const | const | const | const |
| static | static | static | static |
| extern | extern | extern | extern |
| inline | inline | inline | inline |
| マクロ | makuro | Macro | Makro |
| プリプロセッサ | puripurosessa | Preprocessor | Preprosesor |
| 条件コンパイル | jōken konpairu | Conditional Compilation | Kompilasi Bersyarat |
| インクルードガード | inkurūdo gādo | Include Guard | Include Guard |
| メモリマップド I/O | memori mappudo I/O | Memory-mapped I/O | I/O Terpeta Memori |
| レジスタ操作 | rejisuta sōsa | Register Operation | Operasi Register |
| ビット操作 | bitto sōsa | Bit Manipulation | Manipulasi Bit |
| シフト演算 | shifuto enzan | Shift Operation | Operasi Geser |
| 割り込みハンドラ | warikomi handora | Interrupt Handler | Handler Interupsi |
| コンテキストスイッチ | kontekisuto suicchi | Context Switch | Alih Konteks |
| クリティカルセクション | kuritikaru sekushon | Critical Section | Seksi Kritis |
| アトミック操作 | atomikku sōsa | Atomic Operation | Operasi Atomik |

### RTOS (Real-Time Operating System)

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| RTOS | RTOS | Real-Time OS | OS Waktu Nyata |
| タスク | tasuku | Task | Tugas |
| スレッド | sureddo | Thread | Thread |
| プロセス | purosesu | Process | Proses |
| スケジューリング | sukejūringu | Scheduling | Penjadwalan |
| プリエンプティブ | purienputibu | Preemptive | Preemptif |
| 協調性 | kyōchōsei | Cooperative | Kooperatif |
| 優先度 | yūsendo | Priority | Prioritas |
| コンテキスト | kontekisuto | Context | Konteks |
| スタックサイズ | sutakku saizu | Stack Size | Ukuran Stack |
| セマフォ | semafo | Semaphore | Semapor |
| ミューテックス | myūtekkusu | Mutex | Mutex |
| メッセージキュー | messēji kyū | Message Queue | Antrian Pesan |
| イベントフラグ | ibento furagu | Event Flag | Flag Event |
| デッドロック | deddrokku | Deadlock | Deadlock |
| ライスコンディション | raisu kondishon | Race Condition | Race Condition |
| タイムアウト | taimuauto | Timeout | Timeout |
| タイクスライス | taiku suraisu | Time Slice | Irisan Waktu |
| アイドルタスク | aidoru tasuku | Idle Task | Tugas Idle |

### Debugging Techniques

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| デバッグ | debaggu | Debug | Debug |
| ブレークポイント | burēkupointo | Breakpoint | Breakpoint |
| ウォッチポイント | uocchipointo | Watchpoint | Watchpoint |
| ステップ実行 | suteppu jikkō | Step Execution | Eksekusi Langkah |
| ステップオーバー | suteppu ōbā | Step Over | Step Over |
| ステップイントゥ | suteppu intou | Step Into | Step Into |
| ステップアウト | suteppu auto | Step Out | Step Out |
| 実行再開 | jikkō saikai | Resume Execution | Lanjut Eksekusi |
| 変数表示 | hensū hyōji | Variable Display | Tampilan Variabel |
| メモリダンプ | memori danpu | Memory Dump | Dump Memori |
| レジスタ表示 | rejisuta hyōji | Register Display | Tampilan Register |
| コールスタック | kōru sutakku | Call Stack | Stack Panggilan |
| バックトレース | bakku torēsu | Backtrace | Backtrace |
| ログ出力 | rogu shutsuryoku | Log Output | Output Log |
| シリアル出力 | shiriaru shutsuryoku | Serial Output | Output Serial |
| JTAG | JTAG | JTAG | JTAG |
| SWD | SWD | Serial Wire Debug | Debug Kawat Serial |
| ICE | ICE | In-Circuit Emulator | Emulator Dalam Rangkaian |
| オンチップデバッグ | onchippu debaggu | On-Chip Debug | Debug Pada Chip |

---

## 4. Testing & Measurement

### Test Equipment

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| オシロスコープ | oshirosukōpu | Oscilloscope | Osiloskop |
| デジタルオシロスコープ | dejitaru oshirosukōpu | Digital Oscilloscope | Osiloskop Digital |
| ロジックアナライザ | rojikku anaraiza | Logic Analyzer | Analizer Logika |
| マルチメータ | maruchimēta | Multimeter | Multimeter |
| テスター | tesutā | Tester | Tester |
| 電源装置 | dengen sōchi | Power Supply | Catu Daya |
| 関数発生器 | kansū hasseiki | Function Generator | Generator Fungsi |
| シグナルジェネレータ | shigunaru jenerēta | Signal Generator | Generator Sinyal |
| スペクトラムアナライザ | supekutoramu anaraiza | Spectrum Analyzer | Analizer Spektrum |
| ロガー | rogā | Logger | Logger |
| プローブ | purōbu | Probe | Probe |
| クリップ | kurippu | Clip | Jepit |
| グランド | gurando | Ground | Ground |

### Testing Types

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| 単体テスト | tantai tesuto | Unit Test | Uji Unit |
| 結合テスト | ketsugō tesuto | Integration Test | Uji Integrasi |
| システムテスト | shisutemu tesuto | System Test | Uji Sistem |
| 受け入れテスト | ukeire tesuto | Acceptance Test | Uji Penerimaan |
| 回帰テスト | kaikki tesuto | Regression Test | Uji Regresi |
| ストレステスト | sutoresu tesuto | Stress Test | Uji Stres |
| 負荷テスト | fuka tesuto | Load Test | Uji Beban |
| 耐久テスト | taikyū tesuto | Endurance Test | Uji Ketahanan |
| 環境テスト | kankyō tesuto | Environmental Test | Uji Lingkungan |
| 温度テスト | ondo tesuto | Temperature Test | Uji Suhu |
| 振動テスト | shindō tesuto | Vibration Test | Uji Getaran |
| 衝撃テスト | shōgeki tesuto | Shock Test | Uji Kejut |
| EMC テスト | EMC tesuto | EMC Test | Uji EMC |
| ノイズテスト | noizu tesuto | Noise Test | Uji Derau |
| 静電気テスト | seidanki tesuto | ESD Test | Uji ESD |

### Measurements

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| 電圧測定 | den'atsu sokutei | Voltage Measurement | Pengukuran Tegangan |
| 電流測定 | denryū sokutei | Current Measurement | Pengukuran Arus |
| 周波数測定 | shūhasū sokutei | Frequency Measurement | Pengukuran Frekuensi |
| 波形観測 | hakei kansoku | Waveform Observation | Observasi Gelombang |
| 立ち上がり時間 | tachiagari jikan | Rise Time | Waktu Naik |
| 立ち下がり時間 | tachisagari jikan | Fall Time | Waktu Turun |
| デューティサイクル | dyūti saikuru | Duty Cycle | Duty Cycle |
| パルス幅 | parusu haba | Pulse Width | Lebar Pulsa |
| 周期 | shūki | Period | Periode |
| 位相 | isō | Phase | Fase |
| ノイズマージン | noizu mājin | Noise Margin | Margin Derau |
| スルーレート | surūrēto | Slew Rate | Laju Perubahan |
| 消費電力 | shōhi denryoku | Power Consumption | Konsumsi Daya |
| 待機電力 | taiki denryoku | Standby Power | Daya Siaga |
| 効率 | kōritsu | Efficiency | Efisiensi |

---

## 5. Development Workflow

### Toolchain

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| コンパイラ | konpaira | Compiler | Kompilator |
| アセンブラ | asenbura | Assembler | Perakit |
| リンカ | rinka | Linker | Linker |
| ローダ | rōda | Loader | Loader |
| デバッガ | debagga | Debugger | Debugger |
| IDE | IDE | Integrated Dev Environment | Lingkungan Dev Terpadu |
| ビルドツール | birudo tsūru | Build Tool | Alat Build |
| Makefile | Makefile | Makefile | Makefile |
| バージョン管理 | bājon kanri | Version Control | Kontrol Versi |
| Git | Git | Git | Git |
| リポジトリ | ripojitori | Repository | Repositori |
| コミット | komitto | Commit | Commit |
| プルリクエスト | puru rikuesuto | Pull Request | Pull Request |
| マージ | māji | Merge | Gabung |
| ブランチ | buranchi | Branch | Cabang |
| CI/CD | CI/CD | CI/CD | CI/CD |
| 自動テスト | jidō tesuto | Automated Test | Tes Otomatis |
| デプロイ | depuroi | Deploy | Deploy |

### Documentation

| Jepang | Romaji | English | Indonesia |
|--------|--------|---------|-----------|
| 仕様書 | shiyōsho | Specification Document | Dokumen Spesifikasi |
| 設計書 | sekkeisho | Design Document | Dokumen Desain |
| 回路図 | kairozū | Circuit Diagram | Diagram Rangkaian |
| 部品表 | buhinhyō | Parts List | Daftar Komponen |
| BOM | BOM | Bill of Materials | Daftar Material |
| 接続図 | setsuzokuzu | Connection Diagram | Diagram Koneksi |
| 配線図 | haisenzu | Wiring Diagram | Diagram Pengkabelan |
| フローチャート | furō chāto | Flowchart | Diagram Alir |
| タイミングチャート | taimingu chāto | Timing Chart | Diagram Waktu |
| 状態遷移図 | jōtai sen'i zu | State Transition Diagram | Diagram Transisi Status |
| ブロック図 | burokku zu | Block Diagram | Diagram Blok |
| 使用説明書 | shiyō setsumeisho | User Manual | Manual Pengguna |
| 技術マニュアル | gijutsu manyuaru | Technical Manual | Manual Teknis |

---

## 6. Common Technical Phrases

### Problem Reporting

```
問題が発生しました。
(Mondai ga hassei shimashita)
= Terjadi masalah.

バグが見つかりました。
(Bagu ga mitsukarimashita)
= Bug ditemukan.

システムがクラッシュしました。
(Shisutemu ga kurasshu shimashita)
= Sistem crash.

再起動してください。
(Saikidō shite kudasai)
= Silakan restart.

ログを確認してください。
(Rogu o kakunin shite kudasai)
= Silakan periksa log.
```

### Status Updates

```
開発中です。
(Kaihatsu-chū desu)
= Sedang dalam pengembangan.

テスト中です。
(Tesuto-chū desu)
= Sedang dalam pengujian.

完了しました。
(Kanryō shimashita)
= Sudah selesai.

保留になっています。
(Horyū ni natte imasu)
= Sedang ditunda.

至急対応が必要です。
(Shikyū taiō ga hitsuyō desu)
= Perlu penanganan segera.
```

### Technical Discussions

```
この仕様は変更可能ですか？
(Kono shiyō wa henkō kanō desu ka?)
= Apakah spesifikasi ini bisa diubah?

実装方法を教えてください。
(Jissō hōhō o oshiete kudasai)
= Tolong ajarkan cara implementasinya.

原因を調査します。
(Genin o chōsa shimasu)
= Saya akan investigasi penyebabnya.

対策を提案します。
(Taisaku o teian shimasu)
= Saya usulkan tindakan penanggulangan.

確認させてください。
(Kakunin sasete kudasai)
= Izinkan saya memastikan.
```

---

## 7. Latihan (Exercises)

### Exercise 1: Terjemahkan Istilah

1. Microcontroller → ?
2. Interrupt Handler → ?
3. Baud Rate → ?
4. Flash Memory → ?
5. Oscilloscope → ?

### Exercise 2: Lengkapi Kalimat

1. CAN バスのターミネーション抵抗は______Ωです。
2. I2C の通信線は SDA と______です。
3. SPI のマスターがデータを送る線は______です。
4. 割り込み処理では______キーワードを使います。
5. RTOS でタスク間の排他制御には______を使います。

### Exercise 3: Situasi Kerja

Anda perlu melaporkan bug ke tim Jepang. Tulis dalam bahasa Jepang:
- "Bug ditemukan di fungsi ADC"
- "Sistem crash saat interrupt terjadi"
- "Saya sudah memperbaiki masalahnya"
- "Perlu testing lebih lanjut"

<details>
<summary>Jawaban (Click to reveal)</summary>

**Exercise 1:**
1. マイクロコントローラ (maikoro kontorōra)
2. 割り込みハンドラ (warikomi handora)
3. ボーレート (bōrēto)
4. フラッシュメモリ (furasshu memori)
5. オシロスコープ (oshirosukōpu)

**Exercise 2:**
1. 120
2. SCL
3. MOSI
4. volatile
5. ミューテックス (myūtekkusu)

**Exercise 3:**
- ADC 関数にバグが見つかりました。(ADC kansu ni bagu ga mitsukarimashita)
- 割り込み発生時にシステムがクラッシュしました。(Warikomi hassei-ji ni shisutemu ga kurasshu shimashita)
- 問題を修正しました。(Mondai o shūsei shimashita)
- さらにテストが必要です。(Sara ni tesuto ga hitsuyō desu)

</details>

---

## Tips untuk Engineer Non-Native

1. **Fokus pada Katakana** - Banyak istilah teknis adalah katakana dari English
2. **Pahami Datasheet** - Biasakan membaca datasheet versi Jepang
3. **Pelajari Singkatan** - CAN, SPI, I2C sama di semua bahasa
4. **Praktek Code Review** - Baca code comment dalam bahasa Jepang
5. **Gunakan Tools** - Google Translate untuk dokumentasi teknis

---

## Resources Tambahan

- [Renesas JP Docs](https://www.renesas.com/jp/)
- [Rohm Tech Web](https://techweb.rohm.com/)
- [EE Times Japan](https://eetimes.jp/)
- [Monoist](https://monoist.atmarkit.co.jp/)
