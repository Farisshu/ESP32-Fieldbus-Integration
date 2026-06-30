# Day 06: Electronics Review for Embedded Engineers

## Tujuan Pembelajaran

Setelah menyelesaikan hari ini, Anda akan mampu:
- Memahami konsep elektronika yang relevan dengan embedded systems
- Menganalisis rangkaian menggunakan hukum Kirchhoff, Thevenin, Norton
- Mendesain filter untuk signal conditioning
- Memahami karakteristik diode, BJT, MOSFET, Op-Amp
- Mendesain ADC front-end circuit
- Mengatasi masalah noise dan grounding

---

## Konsep Dasar

### Mengapa Electronics Penting?

Sebagai embedded engineer, Anda harus:
1. **Memahami hardware** yang dikendalikan kode
2. **Debug hardware issues** yang terlihat seperti software bugs
3. **Communicate** dengan electrical engineers
4. **Read schematics** untuk understand system architecture

> **Engineer Mindset:** "The best embedded engineers understand both electrons and bits."

---

## Penjelasan Detail

### 1. Hukum Kirchhoff

**KCL (Current Law):** ΣI_in = ΣI_out
**KVL (Voltage Law):** ΣV_loop = 0

**Aplikasi:** Power budgeting, voltage drop analysis

---

### 2. Voltage Divider

```
     Vin
      |
     [R1]
      +---- Vout
     [R2]
      |
     GND

Vout = Vin × R2/(R1+R2)
```

**Aplikasi:** Battery monitoring, level shifting

```c
// Battery monitoring example
#define DIVIDER_RATIO  0.262f

float ReadBatteryVoltage(void) {
    uint16_t adc = ADC_Read(ADC_CHANNEL_BATTERY);
    float vout = (adc / 4095.0f) * 3.3f;
    return vout / DIVIDER_RATIO;
}
```

---

### 3. RC Filter

```
     Vin
      |
     [R]
      +---- Vout
     --- C
      |
     GND

fc = 1 / (2πRC)
τ = RC
```

**Aplikasi:** Debouncing, anti-aliasing filter

**Example:** Button debounce dengan τ = 10ms
- R = 100kΩ, C = 100nF

---

### 4. Transistor as Switch

**NPN BJT:**
```
     Vcc
      |
     [Load]
      |
     Collector
      |
     [BJT]
      |
     Emitter -- GND
     
     Base --[R]-- GPIO
```

**MOSFET (Better for high current):**
- Logic level: V_GS(th) < 2.5V untuk 3.3V MCU
- Lower R_DS(on) = less power loss

---

### 5. Op-Amp Configurations

**Voltage Follower (Buffer):**
```
Vin ----(+) 
         |
        [OpAmp]---- Vout = Vin
         |
         +---------(-)
```

**Non-Inverting Amplifier:**
```
Gain = 1 + (Rf / Rg)
```

---

### 6. ADC Front-End Design

**Requirements:**
1. Input range matching (voltage divider)
2. Source impedance < 10kΩ
3. Anti-aliasing filter (LPF)
4. Overvoltage protection (clamping diodes)

```
Sensor --[Rdiv1]--+--[Rseries]-- ADC
                  |
                 [Rdiv2]
                  |
                 --- C_filter
                  |
                 GND
```

---

### 7. Noise and Grounding

**Decoupling Capacitors:**
- 10µF bulk (low frequency)
- 100nF ceramic (high frequency)
- Place close to IC power pins!

**Grounding:**
- Star ground untuk analog/digital separation
- Ground plane untuk PCB

---

## Kesalahan yang Sering Terjadi

| Error | Solution |
|-------|----------|
| High-Z sensor direct to ADC | Add buffer op-amp |
| No decoupling caps | Add 100nF per IC |
| Wrong MOSFET V_GS(th) | Use logic-level MOSFET |
| Missing flyback diode | Add across inductive loads |
| Ground loops | Use star grounding |

---

## Tips Engineer Profesional

1. **Always read datasheets** - Sensor, MCU, components
2. **Measure, don't assume** - Use multimeter, scope
3. **Design for worst case** - Min/max voltage, temperature
4. **Document calculations** - Future you will thank yourself
5. **Prototype first** - Breadboard before PCB

---

## Ringkasan

| Topik | Formula/Concept | Aplikasi |
|-------|----------------|----------|
| Voltage Divider | Vout = Vin×R2/(R1+R2) | Battery sensing |
| RC Filter | fc = 1/(2πRC) | Debouncing |
| BJT | I_C = β×I_B | Low-current switching |
| MOSFET | V_GS controls | High-current switching |
| Op-Amp | Virtual short | Signal conditioning |
| Decoupling | 100nF close to IC | Noise reduction |

---

## Referensi Belajar

1. "The Art of Electronics" - Horowitz & Hill
2. All About Circuits - https://www.allaboutcircuits.com/
3. EEVblog YouTube Channel
4. TI Precision Labs - ADC tutorials

---

> "A great embedded engineer sees the electrons flowing through the code."
