# 🇯🇵 Japanese Learning for Embedded Engineers

> **Bilingual Technical Guide**: Belajar bahasa Jepang teknis sambil menguasai embedded systems  
> **二言語技術ガイド**: 組込システムを学びながら技術日本語をマスターする

---

## 📋 Daftar Isi / 目次 (もくじ)

1. [Introduction / はじめに](#1-introduction--はじめに)
2. [Technical Vocabulary / 技術用語 (ぎじゅつようご)](#2-technical-vocabulary--技術用語-ぎじゅつようご)
3. [Embedded Systems Terms / 組込システム用語](#3-embedded-systems-terms--組込システム用語)
4. [Communication Protocols / 通信プロトコル](#4-communication-protocols--通信プロトコル)
5. [Development Workflow / 開発ワークフロー](#5-development-workflow--開発ワークフロー)
6. [Testing & Documentation / テストと文書化](#6-testing--documentation--テストと文書化)
7. [Study Plan / 学習計画](#7-study-plan--学習計画)
8. [Practice Exercises / 練習問題](#8-practice-exercises--練習問題)

---

## 1. Introduction / はじめに

### Purpose / 目的 (もくてき)

This guide helps embedded engineers learn Japanese technical vocabulary while studying embedded systems concepts. All terms are presented in **trilingual format** (English - 日本語 - Indonesia).

このガイドは、組込システムを学びながら技術日本語の語彙を習得するためのものです。すべての用語は**三言語形式**（英語 - 日本語 - インドネシア語）で提示されます。

### Learning Approach / 学習アプローチ

- ✅ **Context-Based Learning**: Learn terms through real embedded projects
- ✅ **Progressive Difficulty**: From basic components to advanced protocols
- ✅ **Practical Usage**: Focus on terms used in industry documentation
- ✅ **Bilingual Notes**: Explanations in both English and Indonesian

---

## 2. Technical Vocabulary / 技術用語 (ぎじゅつようご)

### Basic Electronics / 電子工学基礎 (でんしがくきそ)

| English | 日本語 | Romaji | Indonesia | Context |
|---------|--------|--------|-----------|---------|
| Circuit | 回路 | kairo | Rangkaian | Electrical path |
| Resistor | 抵抗 | teikō | Resistor | Limits current |
| Capacitor | コンデンサ | kondensa | Kapasitor | Stores charge |
| Voltage | 電圧 | denatsu | Tegangan | Electric potential |
| Current | 電流 | denryū | Arus | Flow of electrons |
| Ground | グランド | gurando | Ground | Reference point |
| Signal | 信号 | shingō | Sinyal | Data transmission |
| Noise | ノイズ | noizu | Derau | Unwanted interference |
| Power Supply | 電源 | dengen | Catu Daya | Provides power |
| Oscilloscope | オシロスコープ | oshirosukōpu | Osiloskop | Waveform viewer |

### Microcontroller Components / マイコン部品 (まいこんぶひん)

| English | 日本語 | Romaji | Indonesia | Example |
|---------|--------|--------|-----------|---------|
| Microcontroller | マイクロコントローラ | maikoro kontorōra | Mikrokontroler | ESP32, STM32 |
| CPU | 中央処理装置 | chūō shori sōchi | Unit Pemroses | Core processor |
| Memory | メモリ | memori | Memori | RAM/Flash storage |
| GPIO | 汎用入出力 | han'yō nyūshutsuryoku | GPIO | General pins |
| Register | レジスタ | rejisuta | Register | Configuration bits |
| Interrupt | 割込み | warikomi | Interupsi | Event handling |
| Timer | タイマー | taimā | Timer | Time counting |
| Watchdog | ウォッチドッグ | u~otchidoggu | Watchdog | System reset |
| Clock | クロック | kurokku | Clock | Timing signal |
| Bus | バス | basu | Bus | Communication path |

---

## 3. Embedded Systems Terms / 組込システム用語 (こみこみシステムようご)

### System Architecture / システムアーキテクチャ

| English | 日本語 | Romaji | Indonesia | Description |
|---------|--------|--------|-----------|-------------|
| Embedded System | 組込システム | komikomi shisutemu | Sistem Tertanam | Dedicated computer system |
| Real-Time System | リアルタイムシステム | riarutaimu shisutemu | Sistem Waktu Nyata | Time-constrained processing |
| Firmware | ファームウェア | fāmuwea | Firmware | Low-level software |
| Bare-Metal | ベアメタル | beametaru | Bare-Metal | No OS, direct hardware |
| RTOS | リアルタイム OS | riarutaimu oesu | RTOS | Real-time operating system |
| Task | タスク | tasuku | Task | Independent thread |
| Queue | キュー | kyū | Queue | Message buffer |
| Semaphore | セマフォ | semafo | Semaphore | Sync primitive |
| Mutex | ミューテックス | myūtekkusu | Mutex | Mutual exclusion |
| Scheduler | スケジューラ | sukejūra | Scheduler | Task manager |

### Development Concepts / 開発概念 (かいはつがいねん)

| English | 日本語 | Romaji | Indonesia | Usage |
|---------|--------|--------|-----------|-------|
| Debugging | デバッグ | debaggu | Debugging | Finding bugs |
| Breakpoint | ブレークポイント | burēkupointo | Breakpoint | Pause execution |
| Step Through | ステップ実行 | suteppu jikkō | Step Through | Line-by-line |
| Variable | 変数 | hensū | Variabel | Data storage |
| Function | 関数 | kansū | Fungsi | Code block |
| Library | ライブラリ | raiburari | Library | Reusable code |
| Driver | ドライバ | doraiba | Driver | Hardware interface |
| API | API | API | API | Interface definition |
| Framework | フレームワーク | furēmuwāku | Framework | Development structure |
| Toolchain | ツールチェーン | tsūruchēn | Toolchain | Dev tools suite |

---

## 4. Communication Protocols / 通信プロトコル (つうしんプロトコル)

### Protocol Types / プロトコル種類

| English | 日本語 | Romaji | Indonesia | Speed | Distance |
|---------|--------|--------|-----------|-------|----------|
| UART | UART | UART | UART | ≤1 Mbps | <15m |
| SPI | SPI | SPI | SPI | ≤50 MHz | <1m |
| I2C | I2C | I2C | I2C | ≤3.4 Mbps | <1m |
| CAN Bus | CAN バス | CAN basu | CAN Bus | ≤1 Mbps | ≤1km |
| RS485 | RS485 | RS485 | RS485 | ≤10 Mbps | ≤1.2km |
| Ethernet | イーサネット | īsanetto | Ethernet | ≤1 Gbps | ≤100m |
| USB | USB | USB | USB | ≤10 Gbps | ≤5m |
| Bluetooth | ブルートゥース | burūtūsu | Bluetooth | ≤3 Mbps | ≤100m |
| WiFi | WiFi | WiFi | WiFi | ≤1 Gbps | ≤50m |
| LoRa | LoRa | LoRa | LoRa | ≤50 kbps | ≤10km |

### CAN Bus Specific Terms / CAN バス専門用語

| English | 日本語 | Romaji | Indonesia | Description |
|---------|--------|--------|-----------|-------------|
| CAN Controller | CAN コントローラ | CAN kontorōra | CAN Controller | Protocol handler |
| CAN Transceiver | CAN トランシーバ | CAN toranshība | CAN Transceiver | Physical layer |
| Frame | フレーム | furēmu | Frame | Data packet |
| ID | ID | ID | ID | Message identifier |
| DLC | DLC | DLC | DLC | Data length code |
| Payload | ペイロード | peirōdo | Payload | Actual data |
| Arbitration | 調停 | chōtei | Arbitrasi | Priority resolution |
| Termination | 終端 | shūtan | Terminasi | Bus ending resistor |
| Error Frame | エラーフレーム | erā furēmu | Error Frame | Error notification |
| Bus-Off | バスオフ | basu ofu | Bus-Off | Error state |

### SPI Specific Terms / SPI 専門用語

| English | 日本語 | Romaji | Indonesia | Function |
|---------|--------|--------|-----------|----------|
| Master | マスター | masutā | Master | Controller device |
| Slave | スレーブ | surēbu | Slave | Peripheral device |
| MOSI | MOSI | MOSI | MOSI | Master Out Slave In |
| MISO | MISO | MISO | MISO | Master In Slave Out |
| SCK | SCK | SCK | SCK | Serial Clock |
| Chip Select | チップセレクト | chippu serekuto | Chip Select | Device enable |
| Clock Polarity | クロック極性 | kurokku kyokusei | Polaritas Clock | CPOL setting |
| Clock Phase | クロック位相 | kurokku isō | Fasa Clock | CPHA setting |
| Full-Duplex | 全二重 | zen nijū | Full-Duplex | Simultaneous TX/RX |
| Transaction | トランザクション | toranzakushon | Transaksi | Data exchange |

---

## 5. Development Workflow / 開発ワークフロー (かいはつワークフロー)

### Development Stages / 開発段階

| English | 日本語 | Romaji | Indonesia | Output |
|---------|--------|--------|-----------|--------|
| Requirements | 要件定義 | yōken seigi | Spesifikasi Kebutuhan | Requirement doc |
| Design | 設計 | sekkei | Desain | Architecture diagram |
| Implementation | 実装 | jissō | Implementasi | Source code |
| Testing | テスト | tesuto | Pengujian | Test report |
| Debugging | デバッグ | debaggu | Debugging | Fixed issues |
| Documentation | 文書化 | bunshoka | Dokumentasi | User manual |
| Review | レビュー | rebyū | Review | Code review |
| Release | リリース | rirīsu | Rilis | Production version |
| Maintenance | 保守 | hoshu | Pemeliharaan | Updates |
| Version Control | バージョン管理 | bājon kanri | Kontrol Versi | Git history |

### Testing Terminology / テスト用語

| English | 日本語 | Romaji | Indonesia | Type |
|---------|--------|--------|-----------|------|
| Unit Test | 単体テスト | tantai tesuto | Unit Test | Individual module |
| Integration Test | 統合テスト | tōgō tesuto | Integration Test | Combined modules |
| System Test | システムテスト | shisutemu tesuto | System Test | Full system |
| Acceptance Test | 受入テスト | ukeire tesuto | Acceptance Test | Client validation |
| Regression Test | 回帰テスト | kaiki tesuto | Regression Test | After changes |
| Stress Test | ストレステスト | sutoresu tesuto | Stress Test |极限 conditions |
| Logic Analyzer | ロジックアナライザー | rojikku anaraizā | Logic Analyzer | Digital signals |
| Oscilloscope | オシロスコープ | oshirosukōpu | Osiloskop | Analog signals |
| Multimeter | マルチメータ | maruchimēta | Multimeter | Voltage/current |
| Debugger | デバッガ | debagga | Debugger | Code execution |

---

## 6. Testing & Documentation / テストと文書化 (てすと と ぶんしょか)

### Report Structure / 報告書構造 (ほうこくしょこうぞう)

| English | 日本語 | Romaji | Indonesia | Section |
|---------|--------|--------|-----------|---------|
| Test Report | テストレポート | tesuto repōto | Laporan Tes | Main document |
| Objective | 目的 | mokuteki | Tujuan | Test goal |
| Setup | 設定 | settei | Setup | Configuration |
| Procedure | 手順 | tejun | Prosedur | Steps |
| Results | 結果 | kekka | Hasil | Measurements |
| Analysis | 分析 | bunseki | Analisis | Interpretation |
| Conclusion | 結論 | ketsuron | Kesimpulan | Final verdict |
| Recommendation | 推奨事項 | suishō jikō | Rekomendasi | Next steps |
| Appendix | 付録 | furoku | Lampiran | Additional info |
| Revision History | 改訂履歴 | kaitei rireki | Riwayat Revisi | Version changes |

### Common Phrases / よくあるフレーズ

| English | 日本語 | Romaji | Indonesia | Usage |
|---------|--------|--------|-----------|-------|
| Test passed | テスト合格 | tesuto gōkaku | Tes lolos | Success |
| Test failed | テスト不合格 | tesuto fugōkaku | Tes gagal | Failure |
| Within spec | 仕様内 | shiyōnai | Dalam spesifikasi | Valid range |
| Out of spec | 仕様外 | shiyōgai | Luar spesifikasi | Invalid range |
| No issues found | 問題なし | mondai nashi | Tidak ada masalah | Clean test |
| Requires investigation | 要調査 | yō chōsa | Perlu investigasi | Needs review |
| Critical error | 重大エラー | jūdai erā | Error kritis | Severe issue |
| Minor warning | 軽微な警告 | keibi na keikoku | Peringatan kecil | Non-critical |
| Recommended action | 推奨アクション | suishō akushon | Tindakan disarankan | Suggestion |
| Immediate action required | 即対応必要 | soku taiō hitsuyō | Perlu tindakan segera | Urgent |

---

## 7. Study Plan / 学習計画 (がくしゅうけいかく)

### Beginner Level / 初級レベル (しょきゅうレベル) - Month 1-2

| Week | Topic | Vocabulary | Practice | Goal |
|------|-------|------------|----------|------|
| 1 | Basic Electronics | 20 terms | Flashcards | Recognize components |
| 2 | Microcontroller Basics | 25 terms | Label diagrams | Identify MCU parts |
| 3 | GPIO & Digital IO | 15 terms | Simple circuit | Read/write pins |
| 4 | UART Communication | 20 terms | Serial terminal | Send/receive data |
| 5 | Review & Practice | 80 terms total | Quiz | 80% accuracy |

### Intermediate Level / 中級レベル (ちゅうきゅうレベル) - Month 3-4

| Week | Topic | Vocabulary | Practice | Goal |
|------|-------|------------|----------|------|
| 1 | SPI Protocol | 25 terms | Logic analyzer | Capture waveforms |
| 2 | I2C Protocol | 20 terms | Sensor interfacing | Read sensors |
| 3 | Timers & Interrupts | 20 terms | ISR programming | Handle events |
| 4 | ADC/DAC | 15 terms | Analog reading | Measure voltage |
| 5 | Review & Practice | 80 terms total | Project demo | Complete mini-project |

### Advanced Level / 上級レベル (じょうきゅうレベル) - Month 5-6

| Week | Topic | Vocabulary | Practice | Goal |
|------|-------|------------|----------|------|
| 1 | CAN Bus Fundamentals | 30 terms | Two-node setup | Basic communication |
| 2 | CAN Error Handling | 20 terms | Error injection | Handle faults |
| 3 | FreeRTOS Basics | 25 terms | Multi-task app | Create tasks |
| 4 | RTOS Sync Primitives | 20 terms | Queue/semaphore | Safe data sharing |
| 5 | Final Project | 95 terms total | Full system | Portfolio-ready project |

---

## 8. Practice Exercises / 練習問題 (れんしゅうもんだい)

### Exercise 1: Translation / 翻訳練習 (ほんやくれんしゅう)

Translate these sentences to Japanese:

1. "Initialize the SPI bus at 1 MHz"
   - Answer: ________________________________
   - Correct: 「SPI バスを 1 MHz で初期化する」 (SPI basu o 1 MHz de shokika suru)

2. "The CAN frame was received successfully"
   - Answer: ________________________________
   - Correct: 「CAN フレームが正常に受信された」 (CAN furēmu ga seijō ni jushin sareta)

3. "Check the error flag register"
   - Answer: ________________________________
   - Correct: 「エラーフラグレジスタを確認する」 (Erā furagu rejisuta o kakunin suru)

### Exercise 2: Matching / マッチング練習

Match the English term with Japanese:

| English | Japanese |
|---------|----------|
| 1. Microcontroller | A. 通信プロトコル |
| 2. Communication Protocol | B. マイクロコントローラ |
| 3. Debugging | C. デバッグ |
| 4. Documentation | D. 文書化 |
| 5. Testing | E. テスト |

Answers: 1-B, 2-A, 3-C, 4-D, 5-E

### Exercise 3: Fill in the Blank / 穴埋め練習

Complete the sentences:

1. The ______ (マイクロコントローラ) processes all data.
2. We use ______ (ロジックアナライザー) to capture digital signals.
3. The ______ (CAN バス) connects multiple nodes.
4. Please write a ______ (テストレポート) after testing.
5. Initialize the ______ (SPI) peripheral.

---

## 📚 Additional Resources / 追加リソース (ついかリソース)

### Online Dictionaries / オンライン辞書

- [Jisho.org](https://jisho.org/) - English-Japanese dictionary
- [Takoboto](https://takoboto.jp/) - Technical terms dictionary
- [Weblio 技術用語](https://www.weblio.jp/) - Technical terminology

### Recommended Reading / おすすめ読書

- 「組込システム技術者教科書」 - Embedded Systems Engineer Textbook
- 「C 言語によるマイコン制御」 - Microcontroller Control with C
- 「CAN プロトコル入門」 - Introduction to CAN Protocol

### Practice Projects / 練習プロジェクト

1. **UART Loopback**: Send/receive Japanese characters via serial
2. **SPI Display**: Show Japanese text on TFT display
3. **CAN Logger**: Log CAN messages with Japanese comments
4. **Documentation**: Write test report in bilingual format

---

## 🎯 Certification Path / 認定パス (にんていパス)

### JLPT (Japanese-Language Proficiency Test)

| Level | Technical Ability | Target Vocabulary |
|-------|-------------------|-------------------|
| N5 | Basic greetings | 100 words |
| N4 | Simple instructions | 300 words |
| N3 | Technical documentation | 600 words |
| N2 | Professional communication | 1000+ words |
| N1 | Native-level fluency | 2000+ words |

### Embedded Systems Certifications

- **ESP32 Developer**: Focus on IoT applications
- **ARM Certified Engineer**: Cortex-M architecture
- **CAN Bus Specialist**: Automotive networking
- **FreeRTOS Developer**: Real-time systems

---

## 📝 Progress Tracker / 進捗トラッカー (しんちょくトラッカー)

### Vocabulary Mastery / 語彙習得 (ごいしゅうとく)

| Category | Total Terms | Learned | Mastery % | Target Date |
|----------|-------------|---------|-----------|-------------|
| Basic Electronics | 50 | ___ | ___% | YYYY-MM-DD |
| Microcontrollers | 75 | ___ | ___% | YYYY-MM-DD |
| Communication Protocols | 100 | ___ | ___% | YYYY-MM-DD |
| RTOS & Multitasking | 60 | ___ | ___% | YYYY-MM-DD |
| Testing & Debugging | 80 | ___ | ___% | YYYY-MM-DD |
| **Total** | **365** | **___** | **___%** | **YYYY-MM-DD** |

### Skill Assessment / スキル評価

Rate yourself (1-5) on each skill:

| Skill | Current Level | Target Level | Evidence |
|-------|---------------|--------------|----------|
| Reading Japanese datasheets | _ | 4 | _____________ |
| Writing technical reports | _ | 4 | _____________ |
| Understanding error messages | _ | 5 | _____________ |
| Verbal communication | _ | 3 | _____________ |
| Documentation translation | _ | 4 | _____________ |

---

## 🔗 Links to Other Materials / 他の資料へのリンク

- [Main Learning Path](README_MATERI_LENGKAP.md) - Complete technical roadmap
- [Technical Dictionary](KAMUS_TEKNIS_JEPANG.md) - Comprehensive term list (archived reference)
- [Project Documentation](../README.md) - Main project README

---

**Last Updated**: 2025-05-XX  
**Version**: 1.0.0  
**Maintained by**: Embedded Systems Learning Team
