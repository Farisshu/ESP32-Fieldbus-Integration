# Control Theory for Embedded Systems: From Transfer Functions to PID Implementation

> **Panduan Lengkap**: Memahami jembatan antara teori kontrol klasik (s-domain, z-domain) dengan implementasi praktis PID controller di mikrokontroler ESP32.  
> **Target Pembaca**: Embedded engineer yang ingin memahami *mengapa* dan *bagaimana* kontroler PID bekerja, bukan hanya *cara menggunakannya*.

---

## 📋 Daftar Isi

1. [Pendahuluan](#1-pendahuluan)
2. [Transfer Function Basics](#2-transfer-function-basics)
3. [Open-Loop vs Closed-Loop Control](#3-open-loop-vs-closed-loop-control)
4. [Dari Continuous ke Discrete: s-domain ke z-domain](#4-dari-continuous-ke-discrete-s-domain-ke-z-domain)
5. [Discretization of PID Controller](#5-discretization-of-pid-controller)
6. [Implementasi PID dalam Kode C](#6-implementasi-pid-dalam-kode-c)
7. [Digital Filtering untuk Sensor](#7-digital-filtering-untuk-sensor)
8. [Finite State Machine untuk Safety](#8-finite-state-machine-untuk-safety)
9. [MISRA C Compliance](#9-misra-c-compliance)
10. [Referensi & Further Reading](#10-referensi--further-reading)

---

## 1. Pendahuluan

### 1.1 Mengapa Kontrol Otomatis Penting?

Dalam sistem embedded otomotif dan robotika (seperti autonomous driving robot HORIBA ADS-EVO), kita sering perlu mengendalikan besaran fisik:
- **Steering angle** (sudut kemudi)
- **Motor speed** (kecepatan motor)
- **Temperature** (suhu mesin/baterai)
- **Position** (posisi linear/rotary)

**Masalahnya**: Sistem fisik memiliki **dinamika** (inersia, friksi, delay) dan **gangguan eksternal** (angin, perubahan beban, noise sensor).

**Solusinya**: **Closed-loop control** dengan feedback sensor untuk mengoreksi error secara otomatis.

### 1.2 Apa itu PID Controller?

**PID** = **Proportional + Integral + Derivative**

Ini adalah algoritma kontrol paling populer di industri karena:
- ✅ Sederhana untuk diimplementasikan
- ✅ Efektif untuk berbagai sistem (90%+ aplikasi industri)
- ✅ Mudah di-tune (meski butuh pemahaman)
- ✅ Robust terhadap variasi parameter sistem

### 1.3 Learning Objectives

Setelah membaca dokumen ini, Anda akan mampu:
1. Memahami konsep **transfer function** dan **feedback control**
2. Menjelaskan mengapa mikrokontroler perlu **discrete-time math** (bukan calculus kontinu)
3. Menurunkan **difference equation** PID dari transfer function kontinu
4. Mengimplementasikan PID **MISRA C compliant** dengan anti-windup
5. Menerapkan **digital filter** (EMA/IIR) untuk sensor noisy
6. Mendesain **FSM** untuk safety state management

---

## 2. Transfer Function Basics

### 2.1 Apa itu Transfer Function?

**Definisi**: Transfer function adalah representasi matematis dari hubungan input-output suatu sistem dinamis linier time-invariant (LTI).

Dalam **domain-s** (Laplace domain):

```
        Y(s)
G(s) = ──────
        U(s)
```

Dimana:
- `G(s)` = Transfer function sistem
- `Y(s)` = Output (Laplace transform dari y(t))
- `U(s)` = Input (Laplace transform dari u(t))
- `s` = Complex frequency variable (s = σ + jω)

### 2.2 Mengapa Menggunakan Laplace Transform?

**Problem**: Sistem fisik dijelaskan dengan **differential equations** (persamaan diferensial):

```
d²y      dy
─── + 3 ── + 2y = u(t)
dt²      dt
```

Menyelesaikan persamaan diferensial secara langsung **sulit**.

**Solusi**: Laplace transform mengubah **differential equation** → **algebraic equation**:

| Time Domain (t) | Laplace Domain (s) |
|-----------------|-------------------|
| y(t) | Y(s) |
| dy/dt | sY(s) - y(0) |
| d²y/dt² | s²Y(s) - sy(0) - y'(0) |
| ∫y(t)dt | Y(s)/s |

**Keuntungan**: 
- Differential equation → Polynomial equation (lebih mudah)
- Analisis stabilitas jadi sederhana (cek pole location)
- Mudah menggabungkan sistem seri/paralel

### 2.3 Contoh: DC Motor Transfer Function

Untuk DC motor sederhana (mengabaikan inductance):

```
    ω(s)          K
G(s) = ───── = ────────────
    V(s)   (Js + b)(Ls + R) + K²
```

Dimana:
- `ω(s)` = Angular velocity output
- `V(s)` = Voltage input
- `J` = Moment of inertia
- `b` = Viscous friction coefficient
- `L` = Armature inductance
- `R` = Armature resistance
- `K` = Motor constant

**Interpretasi Fisik**:
- **Pole** (akar denominator) menentukan respons alami sistem
- **Zero** (akar numerator) mempengaruhi overshoot
- **Gain** (K) menentukan seberapa besar output untuk input tertentu

### 2.4 Block Diagram: Plant + Controller

```
     ┌─────────────┐     ┌─────────────┐
r ──►│  Controller │────►│    Plant    │────► y
     │   C(s)      │  u  │   G(s)      │
     └─────────────┘     └─────────────┘
```

- `r` = Reference/Setpoint (nilai yang diinginkan)
- `u` = Control signal (output controller, input plant)
- `y` = Process variable (output plant, nilai aktual)
- `C(s)` = Controller transfer function (misal: PID)
- `G(s)` = Plant transfer function (misal: motor, steering mechanism)

---

## 3. Open-Loop vs Closed-Loop Control

### 3.1 Open-Loop Control

```
     ┌─────────────┐     ┌─────────────┐
r ──►│  Controller │────►│    Plant    │────► y
     │   C(s)      │  u  │   G(s)      │
     └─────────────┘     └─────────────┘
```

**Karakteristik**:
- ❌ **Tidak ada feedback** dari output
- ❌ Tidak bisa kompensasi disturbance
- ❌ Tidak bisa kompensasi variasi parameter plant
- ✅ Sederhana, murah
- ✅ Stabil jika plant stabil

**Contoh**: Memberi tegangan tetap ke motor tanpa encoder. Jika beban berubah, kecepatan berubah dan sistem tidak bisa koreksi.

### 3.2 Closed-Loop Control (Feedback Control)

```
                    ┌──────────────┐
         e          │  Controller  │       u       ┌─────────────┐
r ─────⊕───────────►│    C(s)      │──────────────►│    Plant    │──────► y
      (-)           │   (PID)      │               │   G(s)      │
        ▲           └──────────────┘               └─────────────┘
        │                                              │
        │              ┌──────────────┐                │
        └──────────────│   Sensor     │◄───────────────┘
                       │   H(s)       │
                       └──────────────┘
                          Feedback
```

**Karakteristik**:
- ✅ **Ada feedback** dari output ke input
- ✅ Bisa reject disturbance (gangguan eksternal)
- ✅ Bisa kompensasi variasi parameter plant
- ✅ Bisa tracking setpoint dengan akurat
- ⚠️ Lebih kompleks, risiko instability jika tidak didesain baik

**Error Signal**: `e(t) = r(t) - y(t)`

Controller berusaha **meminimalkan error** dengan menyesuaikan `u(t)`.

### 3.3 Mengapa Feedback Penting? (Disturbance Rejection)

**Skenario**: Robot mobil otonom sedang berbelok, tiba-tiba ada **angin samping** atau **jalan miring**.

**Open-Loop**:
- Steering angle tetap sesuai perintah awal
- Mobil menyimpang dari jalur
- ❌ Tidak ada koreksi

**Closed-Loop**:
- Sensor (encoder/IMU) mendeteksi penyimpangan
- Error meningkat → PID menghitung koreksi
- Actuator (motor steering) dikoreksi otomatis
- ✅ Mobil kembali ke jalur yang benar

**Kesimpulan**: Feedback memungkinkan sistem **beradaptasi** dengan kondisi yang berubah.

---

## 4. Dari Continuous ke Discrete: s-domain ke z-domain

### 4.1 Mengapa Mikrokontroler Tidak Bisa Pakai Calculus Kontinu?

**Problem Fundamental**: Mikrokontroler adalah sistem **discrete-time**, bukan continuous-time.

| Continuous-Time (Analog) | Discrete-Time (Digital) |
|--------------------------|-------------------------|
| Waktu berjalan kontinu | Waktu diskrit (sample setiap T detik) |
| t ∈ ℝ (real numbers) | k ∈ ℤ (integer samples) |
| Differential equations | Difference equations |
| Laplace transform (s) | Z-transform (z) |
| Op-amp circuits | Microcontroller algorithms |

**Alasan**:
1. **Sampling**: ADC hanya mengambil sample pada interval tertentu (sampling period `T`)
2. **Computation**: CPU butuh waktu untuk menghitung (tidak instant)
3. **Memory**: Nilai disimpan di register/memory (discrete values)

### 4.2 Sampling Period (T)

```
Continuous signal:  ~~~~~~~~~~~~~~~~ (analog, smooth)

Sampled signal:     •   •   •   •   •   (discrete points)
                    │   │   │   │   │
                    t₀  t₁  t₂  t₃  t₄
                    
Sampling period: T = t₁ - t₀ = t₂ - t₁ = ...
Sampling frequency: f_s = 1/T
```

**Pemilihan T**:
- Terlalu besar → Aliasing, respons lambat, instability
- Terlalu kecil → Overhead CPU, quantization noise
- **Rule of thumb**: `f_s ≥ 10× bandwidth sistem` atau `T ≤ 1/(10×ω_c)`

Untuk motor control: `T = 1-10 ms` umum digunakan.

### 4.3 Z-Transform: Discrete Counterpart of Laplace Transform

**Definisi Z-Transform**:

```
         ∞
X(z) =  Σ  x[k] · z^(-k)
       k=0
```

Dimana:
- `x[k]` = Sample ke-k dari sinyal discrete
- `z` = Complex variable (z = r·e^(jθ))
- `k` = Integer index (sample number)

**Mapping s → z**:

Hubungan antara s-domain dan z-domain:

```
z = e^(sT)
```

Atau inversnya:

```
s = (1/T) · ln(z)
```

**Interpretasi**:
- `s = jω` (imaginary axis di s-plane) → `z = e^(jωT)` (unit circle di z-plane)
- Pole di s-plane kiri (stabil) → Pole di dalam unit circle di z-plane (stabil)

### 4.4 Discrete-Time System Representation

**Difference Equation** (time domain):

```
y[k] = a₁·y[k-1] + a₂·y[k-2] + ... + b₀·u[k] + b₁·u[k-1] + ...
```

**Transfer Function** (z-domain):

```
        Y(z)   b₀ + b₁·z^(-1) + b₂·z^(-2) + ...
H(z) = ───── = ─────────────────────────────────
        U(z)   1 + a₁·z^(-1) + a₂·z^(-2) + ...
```

**Notasi**:
- `y[k]` = Output pada sample ke-k (saat ini)
- `y[k-1]` = Output pada sample sebelumnya
- `u[k]` = Input pada sample ke-k (saat ini)
- `u[k-1]` = Input pada sample sebelumnya

---

## 5. Discretization of PID Controller

### 5.1 Continuous-Time PID (Ideal Form)

**Time domain**:

```
                t
                ⌠
u(t) = Kp·e(t) + Ki·⎮ e(τ)dτ + Kd·de(t)/dt
                ⌡
                0
```

**Laplace domain**:

```
               Ki      Kd
C(s) = Kp + ───── + ─────
               s        s
```

Atau bentuk umum:

```
         Kd·s² + Kp·s + Ki
C(s) = ───────────────────
              s
```

Dimana:
- `Kp` = Proportional gain
- `Ki` = Integral gain (= Kp/Ti)
- `Kd` = Derivative gain (= Kp·Td)
- `e(t)` = Error signal = r(t) - y(t)

### 5.2 Metode Diskretisasi

Ada beberapa metode untuk mengkonversi `C(s)` → `C(z)`:

#### 5.2.1 Forward Euler (Forward Difference)

**Approximation**:

```
s ≈ (z - 1) / T
```

**Kelebihan**: Sederhana  
**Kekurangan**: Bisa unstable, kurang akurat

#### 5.2.2 Backward Euler (Backward Difference) ← **Yang Kita Pakai**

**Approximation**:

```
s ≈ (z - 1) / (T·z)
```

Atau dalam time domain:

```
dy/dt ≈ (y[k] - y[k-1]) / T
```

**Kelebihan**:
- Selalu stabil jika sistem kontinu stabil
- Simple implementation
- Good untuk integral term

**Kekurangan**:
- Phase lag (kurang akurat untuk derivative)

#### 5.2.3 Tustin (Bilinear Transform)

**Approximation**:

```
       2   (z - 1)
s ≈ ─── · ────────
      T   (z + 1)
```

**Kelebihan**: Paling akurat, preserve frequency response  
**Kekurangan**: Lebih kompleks komputasi

### 5.3 Diskretisasi dengan Backward Euler

Mari kita diskritisasi setiap term PID satu per satu.

#### Term Proportional:

```
u_p(t) = Kp · e(t)

Diskretisasi:
u_p[k] = Kp · e[k]
```

**Mudah!** Tidak ada perubahan bentuk.

#### Term Integral:

```
                t
                ⌠
u_i(t) = Ki ·   ⎮ e(τ)dτ
                ⌡
                0
```

Integral adalah **luas area** di bawah kurva error. Dengan Backward Euler:

```
Area per sample ≈ e[k] · T  (rectangle approximation)

Akumulasi:
u_i[k] = u_i[k-1] + Ki · T · e[k]
```

Atau definisikan `Ki' = Ki · T`:

```
u_i[k] = u_i[k-1] + Ki' · e[k]
```

**Interpretasi**: Integral term adalah **akumulasi error** dari waktu ke waktu.

#### Term Derivative:

```
u_d(t) = Kd · de(t)/dt
```

Dengan Backward Euler:

```
de/dt ≈ (e[k] - e[k-1]) / T

Jadi:
u_d[k] = Kd · (e[k] - e[k-1]) / T
```

Atau definisikan `Kd' = Kd / T`:

```
u_d[k] = Kd' · (e[k] - e[k-1])
```

**Interpretasi**: Derivative term memprediksi **trend error** (apakah error meningkat atau menurun).

### 5.4 Discrete PID Equation (Position Form)

Gabungkan ketiga term:

```
u[k] = u_p[k] + u_i[k] + u_d[k]

u[k] = Kp·e[k] + Ki'·Σ(e[j], j=0..k) + Kd'·(e[k] - e[k-1])
```

Atau tulis ulang dengan explicit summation:

```
u[k] = Kp·e[k] + Ki'·(e[0] + e[1] + ... + e[k]) + Kd'·(e[k] - e[k-1])
```

**Problem**: Summation eksplisit butuh loop atau memory tak terbatas!

**Solusi**: Gunakan **recursive form**:

```
Define: sum_e[k] = sum_e[k-1] + e[k]

Then:
u[k] = Kp·e[k] + Ki'·sum_e[k] + Kd'·(e[k] - e[k-1])
```

### 5.5 Incremental Form (Velocity Form)

Alternatif: Hitung **perubahan** output, bukan output absolut.

```
Δu[k] = u[k] - u[k-1]
```

Substitusi dan simplify (aljabar omitted untuk brevity):

```
Δu[k] = q₀·e[k] + q₁·e[k-1] + q₂·e[k-2]

Dimana:
q₀ = Kp + Ki' + Kd'
q₁ = -(Kp + 2·Kd')
q₂ = Kd'
```

**Keuntungan Incremental Form**:
- Tidak perlu track integral separately (built-in)
- Natural anti-windup (output bounded by Δu max)
- Smooth transition manual/auto mode

**Kekurangan**:
- Lebih kompleks tuning
- Perlu 3 previous error values

**Decision**: Untuk tutorial ini, kita pakai **Position Form** karena lebih intuitif untuk dipahami.

### 5.6 Anti-Windup Mechanism

**Problem**: **Integral Windup**

Ketika actuator **saturate** (misal: PWM max 100%, tapi PID minta 150%):
- Error masih ada → Integral term terus akumulasi
- Actuator stuck di max
- Saat error berbalik, integral "terlalu besar" → Overshoot besar

**Solusi**: **Clamp integral term**

```
sum_e[k] = sum_e[k-1] + e[k]

// Clamp integral contribution
integral_contrib = Ki' · sum_e[k]

if (integral_contrib > INTEGRAL_MAX) {
    integral_contrib = INTEGRAL_MAX
    sum_e[k] = INTEGRAL_MAX / Ki'  // Anti-windup reset
} else if (integral_contrib < -INTEGRAL_MAX) {
    integral_contrib = -INTEGRAL_MAX
    sum_e[k] = -INTEGRAL_MAX / Ki'
}
```

Atau **conditional integration** (hanya integrate saat actuator tidak saturate):

```
if (output_not_saturated) {
    sum_e[k] = sum_e[k-1] + e[k]
} else {
    sum_e[k] = sum_e[k-1]  // Freeze integration
}
```

### 5.7 Final Difference Equation

**Position Form dengan Anti-Windup**:

```
e[k] = setpoint - measurement

// Proportional
P = Kp · e[k]

// Integral (dengan anti-windup)
sum_e = sum_e + e[k]
I = clamp(Ki' · sum_e, -I_MAX, I_MAX)

// Derivative
D = Kd' · (e[k] - e[k-1])

// Total output
u[k] = P + I + D
u[k] = clamp(u[k], U_MIN, U_MAX)

// Save for next iteration
e[k-1] = e[k]
```

---

## 6. Implementasi PID dalam Kode C

### 6.1 Mapping Matematika → Kode

Berikut adalah mapping eksplisit dari variabel matematika ke variabel C code:

| Variabel Matematika | Variabel C Code | Tipe Data | Deskripsi |
|---------------------|-----------------|-----------|-----------|
| `e[k]` | `error_curr` | `float_t` | Error saat ini |
| `e[k-1]` | `error_prev` | `float_t` | Error sebelumnya |
| `sum_e[k]` | `integral_sum` | `float_t` | Akumulasi error |
| `u[k]` | `control_output` | `float_t` | Output kontroler |
| `Kp` | `kp` | `float_t` | Proportional gain |
| `Ki'` (= Ki·T) | `ki` | `float_t` | Integral gain (sudah diskalakan) |
| `Kd'` (= Kd/T) | `kd` | `float_t` | Derivative gain (sudah diskalakan) |
| `I_MAX` | `integral_limit` | `float_t` | Batas integral term |
| `U_MAX` | `output_max` | `float_t` | Batas output maksimum |
| `U_MIN` | `output_min` | `float_t` | Batas output minimum |

### 6.2 Struktur Data PID (MISRA C Compliant)

```c
/* File: pid_controller.h */
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

/* Type definitions for clarity and portability */
typedef float pid_float_t;  /* Could be double or fixed-point */

/* PID Controller State Structure */
typedef struct {
    /* Tuning parameters */
    pid_float_t kp;              /* Proportional gain */
    pid_float_t ki;              /* Integral gain (Ki * T) */
    pid_float_t kd;              /* Derivative gain (Kd / T) */
    
    /* Limits for anti-windup and saturation */
    pid_float_t integral_limit;  /* Maximum integral contribution */
    pid_float_t output_max;      /* Maximum output value */
    pid_float_t output_min;      /* Minimum output value */
    
    /* Internal state variables */
    pid_float_t integral_sum;    /* Accumulated error (sum_e) */
    pid_float_t error_prev;      /* Previous error (e[k-1]) */
    
    /* Status flags */
    bool initialized;            /* Initialization flag */
    bool saturated;              /* Output saturation flag */
} PID_Controller_t;

/* Function prototypes */
void PID_Init(PID_Controller_t *pid);
pid_float_t PID_Update(PID_Controller_t *pid, pid_float_t setpoint, 
                       pid_float_t measurement);
void PID_Reset(PID_Controller_t *pid);

#endif /* PID_CONTROLLER_H */
```

### 6.3 Implementasi Fungsi PID Update

```c
/* File: pid_controller.c */
#include "pid_controller.h"

/* Static helper function for clamping (file scope) */
static pid_float_t clamp_value(pid_float_t value, 
                                pid_float_t min_val, 
                                pid_float_t max_val)
{
    pid_float_t result;
    
    if (value > max_val) {
        result = max_val;
    } else if (value < min_val) {
        result = min_val;
    } else {
        result = value;
    }
    
    return result;
}

void PID_Init(PID_Controller_t *pid)
{
    if (pid == (PID_Controller_t *)0) {
        return;  /* Null pointer check */
    }
    
    /* Initialize all state variables to zero */
    pid->integral_sum = 0.0F;
    pid->error_prev = 0.0F;
    pid->initialized = true;
    pid->saturated = false;
}

pid_float_t PID_Update(PID_Controller_t *pid, 
                       pid_float_t setpoint, 
                       pid_float_t measurement)
{
    pid_float_t error_curr;
    pid_float_t proportional_term;
    pid_float_t integral_term;
    pid_float_t derivative_term;
    pid_float_t control_output;
    
    if (pid == (PID_Controller_t *)0) {
        return 0.0F;  /* Null pointer check */
    }
    
    if (pid->initialized != true) {
        return 0.0F;  /* Not initialized */
    }
    
    /* Step 1: Calculate current error */
    error_curr = setpoint - measurement;
    
    /* Step 2: Proportional term */
    proportional_term = pid->kp * error_curr;
    
    /* Step 3: Integral term with anti-windup */
    pid->integral_sum = pid->integral_sum + error_curr;
    
    /* Clamp integral contribution */
    integral_term = pid->ki * pid->integral_sum;
    integral_term = clamp_value(integral_term, 
                                 -(pid->integral_limit), 
                                 pid->integral_limit);
    
    /* Optional: Reset integral sum if clamped (anti-windup) */
    if (integral_term >= pid->integral_limit) {
        pid->integral_sum = pid->integral_limit / pid->ki;
    } else if (integral_term <= -(pid->integral_limit)) {
        pid->integral_sum = -(pid->integral_limit) / pid->ki;
    }
    
    /* Step 4: Derivative term */
    derivative_term = pid->kd * (error_curr - pid->error_prev);
    
    /* Step 5: Sum all terms */
    control_output = proportional_term + integral_term + derivative_term;
    
    /* Step 6: Clamp final output */
    control_output = clamp_value(control_output, 
                                  pid->output_min, 
                                  pid->output_max);
    
    /* Update saturation flag */
    if ((control_output >= pid->output_max) || 
        (control_output <= pid->output_min)) {
        pid->saturated = true;
    } else {
        pid->saturated = false;
    }
    
    /* Step 7: Save state for next iteration */
    pid->error_prev = error_curr;
    
    return control_output;
}

void PID_Reset(PID_Controller_t *pid)
{
    if (pid == (PID_Controller_t *)0) {
        return;
    }
    
    pid->integral_sum = 0.0F;
    pid->error_prev = 0.0F;
    pid->saturated = false;
}
```

### 6.4 Penjelasan Baris-per-Baris

Mari kita trace execution untuk satu sample:

```c
/* Asumsi initial conditions: */
/* setpoint = 100.0, measurement = 80.0 */
/* kp = 2.0, ki = 0.5, kd = 1.0 */
/* integral_sum = 0.0, error_prev = 0.0 */

/* Step 1: Calculate error */
error_curr = 100.0 - 80.0;  /* = 20.0 */

/* Step 2: Proportional */
proportional_term = 2.0 * 20.0;  /* = 40.0 */

/* Step 3: Integral */
pid->integral_sum = 0.0 + 20.0;  /* = 20.0 */
integral_term = 0.5 * 20.0;      /* = 10.0 */
/* Check clamp: 10.0 within limits, no clamping */

/* Step 4: Derivative */
derivative_term = 1.0 * (20.0 - 0.0);  /* = 20.0 */

/* Step 5: Sum */
control_output = 40.0 + 10.0 + 20.0;  /* = 70.0 */

/* Step 6: Clamp */
/* 70.0 within [0, 100], no clamping */

/* Step 7: Save state */
pid->error_prev = 20.0;

/* Return: 70.0 */
```

**Next sample** (measurement = 90.0):

```c
error_curr = 100.0 - 90.0;  /* = 10.0 */
proportional_term = 2.0 * 10.0;  /* = 20.0 */
pid->integral_sum = 20.0 + 10.0;  /* = 30.0 */
integral_term = 0.5 * 30.0;       /* = 15.0 */
derivative_term = 1.0 * (10.0 - 20.0);  /* = -10.0 */
control_output = 20.0 + 15.0 + (-10.0);  /* = 25.0 */
pid->error_prev = 10.0;
```

**Observasi**:
- Error mengecil → Proportional turun
- Integral naik (akumulasi)
- Derivative negatif (error decreasing) → anticipates dan mengurangi overshoot

---

## 7. Digital Filtering untuk Sensor

### 7.1 Mengapa Filter Diperlukan?

**Problem**: Sensor ADC selalu punya **noise**:
- Quantization noise (ADC resolution)
- Electrical noise (EMI, ground loops)
- Environmental noise (vibration, temperature)

**Dampak ke PID**:
- Noise di **derivative term** → Output berosilasi liar
- Noise di **proportional term** → Control jitter
- Sistem jadi **unstable** atau **wear pada actuator**

**Solusi**: **Low-pass filter** sebelum masuk PID.

### 7.2 Exponential Moving Average (EMA) / First-Order IIR

**Continuous-time low-pass filter**:

```
        1
H(s) = ─────────
       τs + 1
```

Dimana `τ` = time constant.

**Discrete-time (Backward Euler)**:

```
y[k] = α · x[k] + (1 - α) · y[k-1]
```

Dimana:
- `x[k]` = Raw sensor reading (noisy)
- `y[k]` = Filtered output
- `α` = Smoothing factor (0 < α ≤ 1)
- `τ` = Time constant
- `T` = Sampling period

**Relationship**:

```
α = T / (τ + T)
```

Atau jika ingin cutoff frequency `f_c`:

```
α = 1 - e^(-2πf_cT) ≈ 2πf_cT  (untuk f_cT << 1)
```

### 7.3 Interpretasi α

| Nilai α | Karakteristik | Aplikasi |
|---------|---------------|----------|
| α = 1.0 | No filtering (y[k] = x[k]) | Signal sudah clean |
| α = 0.5 | Moderate filtering | General purpose |
| α = 0.1 | Strong filtering | Very noisy sensor |
| α = 0.01 | Very strong filtering | Extreme noise, tapi lag besar |

**Trade-off**:
- α besar → Respons cepat, tapi noise lolos
- α kecil → Noise filtered baik, tapi **phase lag** (delay)

### 7.4 Implementasi EMA Filter (MISRA C)

```c
/* File: signal_filter.h */
#ifndef SIGNAL_FILTER_H
#define SIGNAL_FILTER_H

#include <stdint.h>
#include <stdbool.h>

typedef float filter_float_t;

typedef struct {
    filter_float_t alpha;        /* Smoothing factor */
    filter_float_t y_prev;       /* Previous output (y[k-1]) */
    bool initialized;            /* Initialization flag */
} EMA_Filter_t;

void EMA_Init(EMA_Filter_t *filter, filter_float_t alpha);
filter_float_t EMA_Update(EMA_Filter_t *filter, filter_float_t raw_value);

#endif /* SIGNAL_FILTER_H */
```

```c
/* File: signal_filter.c */
#include "signal_filter.h"

void EMA_Init(EMA_Filter_t *filter, filter_float_t alpha)
{
    if (filter == (EMA_Filter_t *)0) {
        return;
    }
    
    /* Validate alpha range: 0 < alpha <= 1 */
    if (alpha <= 0.0F) {
        alpha = 0.01F;  /* Minimum reasonable value */
    } else if (alpha > 1.0F) {
        alpha = 1.0F;   /* No filtering */
    }
    
    filter->alpha = alpha;
    filter->y_prev = 0.0F;
    filter->initialized = true;
}

filter_float_t EMA_Update(EMA_Filter_t *filter, filter_float_t raw_value)
{
    filter_float_t y_curr;
    
    if (filter == (EMA_Filter_t *)0) {
        return 0.0F;
    }
    
    if (filter->initialized != true) {
        return raw_value;  /* Pass through if not initialized */
    }
    
    /* EMA equation: y[k] = α·x[k] + (1-α)·y[k-1] */
    y_curr = (filter->alpha * raw_value) + 
             ((1.0F - filter->alpha) * filter->y_prev);
    
    /* Save state */
    filter->y_prev = y_curr;
    
    return y_curr;
}
```

### 7.5 Contoh Numerik

```
Asumsi: α = 0.3
Initial: y_prev = 0.0

Sample 1: x[1] = 100 (noisy: actual 95 + noise 5)
y[1] = 0.3 * 100 + 0.7 * 0 = 30.0

Sample 2: x[2] = 98 (actual 95 + noise 3)
y[2] = 0.3 * 98 + 0.7 * 30.0 = 29.4 + 21.0 = 50.4

Sample 3: x[3] = 96 (actual 95 + noise 1)
y[3] = 0.3 * 96 + 0.7 * 50.4 = 28.8 + 35.28 = 64.08

...

Sample 10+: y converges ke ~95 (actual value)
```

**Observasi**:
- Butuh beberapa sample untuk converge
- Noise teredam (fluktuasi x[k] tidak langsung ke y[k])
- Ada **lag** (butuh waktu untuk track perubahan step)

---

## 8. Finite State Machine untuk Safety

### 8.1 Mengapa FSM Diperlukan?

**Problem**: Sistem kontrol tidak boleh langsung jalan saat power-on!

**Skenario berbahaya**:
1. Power on → PID langsung aktif → Motor spin tanpa warning
2. Sensor fault → PID baca garbage → Actuator gerak liar
3. Emergency stop ditekan → Sistem harus freeze output

**Solusi**: **Finite State Machine (FSM)** untuk manage operational states.

### 8.2 State Definition

```
STATE_INIT     : Initialisasi hardware, sanity check sensor
STATE_IDLE     : System ready, waiting for start command
STATE_RUNNING  : Normal operation (PID active)
STATE_ESTOP    : Emergency stop (safe state, output = 0)
```

### 8.3 State Transition Diagram

```
                    ┌──────────────┐
                    │              │
         ┌──────────┤  STATE_INIT  │
         │ Reset    │              │
         │          └──────┬───────┘
         │                 │ Init complete
         │                 ▼
         │          ┌──────────────┐
         │ Start    │              │
         ├─────────►│  STATE_IDLE  │
         │          │              │
         │          └──────┬───────┘
         │                 │ Run command
         │                 ▼
         │          ┌──────────────┐
         │          │              │
         │          │ STATE_RUNNING│◄────┐
         │          │              │     │ Loop
         │          └──────┬───────┘     │
         │                 │ E-Stop      │
         │                 ▼             │
         │          ┌──────────────┐     │
         └─────────►│              │─────┘
           Reset    │ STATE_ESTOP  │
                    │              │
                    └──────────────┘
```

### 8.4 Implementasi FSM (MISRA C)

```c
/* File: control_system.h */
#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

/* State enumeration */
typedef enum {
    STATE_INIT = 0,
    STATE_IDLE = 1,
    STATE_RUNNING = 2,
    STATE_ESTOP = 3
} System_State_t;

/* System context structure */
typedef struct {
    System_State_t current_state;
    System_State_t previous_state;
    bool estop_triggered;
    bool sensor_fault;
    uint32_t init_counter;
} System_Context_t;

/* Function prototypes */
void SYS_Init(System_Context_t *ctx);
System_State_t SYS_Update(System_Context_t *ctx, 
                          bool start_cmd, 
                          bool estop_cmd);
const char* SYS_GetStateName(System_State_t state);

#endif /* CONTROL_SYSTEM_H */
```

```c
/* File: control_system.c */
#include "control_system.h"

/* Static state transition table (data-driven design) */
static const struct {
    System_State_t from_state;
    bool start_cmd;
    bool estop_cmd;
    System_State_t to_state;
} state_transitions[] = {
    /* from_state, start, estop, to_state */
    {STATE_INIT,    false, false, STATE_INIT},
    {STATE_INIT,    true,  false, STATE_INIT},  /* Ignore start during init */
    {STATE_INIT,    false, true,  STATE_ESTOP},
    {STATE_INIT,    true,  true,  STATE_ESTOP},
    
    {STATE_IDLE,    false, false, STATE_IDLE},
    {STATE_IDLE,    true,  false, STATE_RUNNING},
    {STATE_IDLE,    false, true,  STATE_ESTOP},
    {STATE_IDLE,    true,  true,  STATE_ESTOP},
    
    {STATE_RUNNING, false, false, STATE_RUNNING},
    {STATE_RUNNING, true,  false, STATE_RUNNING},
    {STATE_RUNNING, false, true,  STATE_ESTOP},
    {STATE_RUNNING, true,  true,  STATE_ESTOP},
    
    {STATE_ESTOP,   false, false, STATE_ESTOP},  /* Stay in ESTOP */
    {STATE_ESTOP,   true,  false, STATE_ESTOP},  /* Need explicit reset */
    {STATE_ESTOP,   false, true,  STATE_ESTOP},
    {STATE_ESTOP,   true,  true,  STATE_ESTOP}
};

void SYS_Init(System_Context_t *ctx)
{
    if (ctx == (System_Context_t *)0) {
        return;
    }
    
    ctx->current_state = STATE_INIT;
    ctx->previous_state = STATE_INIT;
    ctx->estop_triggered = false;
    ctx->sensor_fault = false;
    ctx->init_counter = 0U;
}

System_State_t SYS_Update(System_Context_t *ctx, 
                          bool start_cmd, 
                          bool estop_cmd)
{
    uint_fast8_t i;
    System_State_t next_state;
    
    if (ctx == (System_Context_t *)0) {
        return STATE_ESTOP;
    }
    
    ctx->previous_state = ctx->current_state;
    ctx->estop_triggered = estop_cmd;
    
    /* Special handling for INIT state (needs initialization delay) */
    if (ctx->current_state == STATE_INIT) {
        ctx->init_counter = ctx->init_counter + 1U;
        
        /* Simulate initialization delay (e.g., 100 iterations) */
        if (ctx->init_counter >= 100U) {
            ctx->current_state = STATE_IDLE;
            return ctx->current_state;
        }
        
        /* Check for estop during init */
        if (estop_cmd == true) {
            ctx->current_state = STATE_ESTOP;
            return ctx->current_state;
        }
        
        return ctx->current_state;
    }
    
    /* State transition lookup */
    next_state = ctx->current_state;  /* Default: stay in current state */
    
    for (i = 0U; i < 16U; i++) {
        if (state_transitions[i].from_state == ctx->current_state) {
            if ((state_transitions[i].start_cmd == start_cmd) &&
                (state_transitions[i].estop_cmd == estop_cmd)) {
                next_state = state_transitions[i].to_state;
                break;
            }
        }
    }
    
    /* Explicit reset from ESTOP requires special handling */
    if ((ctx->current_state == STATE_ESTOP) && 
        (estop_cmd == false) && 
        (start_cmd == false)) {
        /* External reset logic needed here */
        /* For now, stay in ESTOP */
        next_state = STATE_ESTOP;
    }
    
    ctx->current_state = next_state;
    return ctx->current_state;
}

const char* SYS_GetStateName(System_State_t state)
{
    switch (state) {
        case STATE_INIT:
            return "INIT";
        case STATE_IDLE:
            return "IDLE";
        case STATE_RUNNING:
            return "RUNNING";
        case STATE_ESTOP:
            return "ESTOP";
        default:
            return "UNKNOWN";
    }
}
```

### 8.5 Integrasi FSM dengan PID

```c
/* Di main.cpp atau task_pid.c */

System_Context_t g_system_ctx;
PID_Controller_t g_pid_ctrl;
EMA_Filter_t g_sensor_filter;

void Task_PID_Control(void *pvParameters)
{
    pid_float_t setpoint = 100.0F;
    pid_float_t raw_sensor;
    pid_float_t filtered_sensor;
    pid_float_t pwm_output;
    bool start_cmd = false;
    bool estop_cmd = false;
    
    /* Initialize subsystems */
    SYS_Init(&g_system_ctx);
    PID_Init(&g_pid_ctrl);
    EMA_Init(&g_sensor_filter, 0.3F);
    
    for (;;) {
        /* Read inputs */
        start_cmd = read_start_button();
        estop_cmd = read_estop_button();
        
        /* Update FSM */
        g_system_ctx.current_state = SYS_Update(&g_system_ctx, 
                                                 start_cmd, 
                                                 estop_cmd);
        
        /* State-dependent behavior */
        switch (g_system_ctx.current_state) {
            case STATE_INIT:
                /* Wait for initialization */
                pwm_output = 0.0F;
                break;
                
            case STATE_IDLE:
                /* System ready, but not running */
                PID_Reset(&g_pid_ctrl);  /* Reset integrator */
                pwm_output = 0.0F;
                break;
                
            case STATE_RUNNING:
                /* Normal PID operation */
                raw_sensor = read_adc();
                filtered_sensor = EMA_Update(&g_sensor_filter, raw_sensor);
                pwm_output = PID_Update(&g_pid_ctrl, setpoint, filtered_sensor);
                break;
                
            case STATE_ESTOP:
                /* Safe state: output = 0 */
                pwm_output = 0.0F;
                PID_Reset(&g_pid_ctrl);
                break;
                
            default:
                pwm_output = 0.0F;
                break;
        }
        
        /* Apply PWM output */
        set_pwm(pwm_output);
        
        /* Delay for 10ms sampling period */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

---

## 9. MISRA C Compliance

### 9.1 Apa itu MISRA C?

**MISRA** = **Motor Industry Software Reliability Association**

Standar coding untuk safety-critical systems (otomotif, aerospace, medical).

**Tujuan**:
- Mencegah common programming errors
- Improve code readability dan maintainability
- Enable static analysis verification

### 9.2 MISRA Rules yang Diterapkan

#### Rule 5.1: External identifiers shall be unique

✅ **Compliance**: Semua fungsi dan global variables punya nama unik dengan prefix modul.

```c
/* Good */
void PID_Init(...);
void PID_Update(...);
void EMA_Init(...);
void SYS_Update(...);

/* Bad - violates Rule 5.1 */
void init(...);  /* Too generic, might conflict */
void update(...);
```

#### Rule 7.2: A "volatile" qualified object shall be accessed appropriately

✅ **Compliance**: Hardware registers diakses via volatile pointers.

```c
/* Good */
#define ADC_REG (*(volatile uint32_t *)0x3FF44000)

/* Bad */
#define ADC_REG (*(uint32_t *)0x3FF44000)  /* Missing volatile */
```

#### Rule 10.1: Operands shall not be of inappropriate essential type

✅ **Compliance**: Explicit casting untuk narrowing conversions.

```c
/* Good */
uint8_t value_8bit;
int32_t value_32bit = 1000;

value_8bit = (uint8_t)(value_32bit & 0xFF);  /* Explicit cast */

/* Bad */
value_8bit = value_32bit;  /* Implicit narrowing conversion */
```

#### Rule 11.3: A cast shall not be performed between pointer types

✅ **Compliance**: Minimal pointer casting, hanya void* untuk generic APIs.

```c
/* Acceptable for FreeRTOS API */
void vTaskDelay(TickType_t xTicksToDelay);

/* Called as */
vTaskDelay(pdMS_TO_TICKS(10));

/* Bad */
int32_t *ptr_int;
float_t *ptr_float;
ptr_float = (float_t *)ptr_int;  /* Dangerous pointer cast */
```

#### Rule 13.4: The result of an assignment operator shall not be used

✅ **Compliance**: Tidak ada assignment dalam expression.

```c
/* Good */
x = 5;
if (x == 5) { ... }

/* Bad - violates Rule 13.4 */
if ((x = 5) == 5) { ... }  /* Assignment in condition */
```

#### Rule 14.1: A loop counter shall not have floating-point type

✅ **Compliance**: Loop counters selalu integer.

```c
/* Good */
for (uint_fast8_t i = 0U; i < 10U; i++) { ... }

/* Bad */
for (float_t f = 0.0F; f < 10.0F; f += 1.0F) { ... }
```

#### Rule 17.2: Functions shall not call themselves (No recursion)

✅ **Compliance**: Tidak ada recursive functions.

```c
/* Good - iterative */
uint32_t factorial(uint32_t n) {
    uint32_t result = 1U;
    while (n > 1U) {
        result = result * n;
        n = n - 1U;
    }
    return result;
}

/* Bad - recursive */
uint32_t factorial(uint32_t n) {
    if (n <= 1U) return 1U;
    return n * factorial(n - 1U);  /* Recursion forbidden */
}
```

#### Rule 21.1: #include shall only reference standard library headers

✅ **Compliance**: Hanya include headers yang valid dan terkontrol.

```c
/* Good */
#include <stdint.h>
#include <stdbool.h>
#include "pid_controller.h"

/* Bad */
#include <non_standard_lib.h>  /* Non-standard header */
```

### 9.3 Dynamic Memory Allocation

**Rule 22.6-22.9**: Dynamic memory allocation discouraged/forbidden.

✅ **Compliance**: Semua objects allocated statically atau di stack.

```c
/* Good - static allocation */
static PID_Controller_t g_pid_instance;

/* Good - stack allocation */
void some_function(void) {
    EMA_Filter_t local_filter;
    EMA_Init(&local_filter, 0.5F);
}

/* Bad - dynamic allocation */
PID_Controller_t *pid_ptr = malloc(sizeof(PID_Controller_t));  /* Forbidden */
```

---

## 10. Referensi & Further Reading

### 10.1 Buku Teks

1. **"Modern Control Engineering"** - Katsuhiko Ogata  
   *Classic textbook untuk control theory fundamentals*

2. **"Digital Control of Dynamic Systems"** - Gene F. Franklin et al.  
   *Excellent untuk discretization dan z-domain analysis*

3. **"Embedded Control Systems in Automotive Applications"** - Albert M. Styler  
   *Aplikasi praktis di industri otomotif*

### 10.2 Online Resources

- **MIT OpenCourseWare**: 6.302 Feedback System Design
- **Control Systems Lectures** - Brian Douglas (YouTube)
- **MISRA C:2012 Guidelines** - Official documentation

### 10.3 Implementasi Reference

- **Arduino PID Library** - Brett Beauregard (good untuk conceptual understanding)
- **ESP-IDF LEDC** - Official Espressif documentation
- **FreeRTOS Documentation** - Queue and task management

### 10.4 Catatan untuk Review Masa Depan

Saat membaca ulang dokumen ini dalam beberapa bulan/tahun:

1. **Review section 5.3-5.7** untuk refresh derivasi PID discrete
2. **Trace contoh numerik di 6.4** untuk understand data flow
3. **Compare EMA filter (section 7)** dengan alternative filters (moving average, Butterworth)
4. **Test anti-windup** dengan scenario: setpoint step change + actuator saturation
5. **Verify MISRA compliance** dengan static analyzer (PC-lint, Cppcheck)

---

**Document Version**: 1.0.0  
**Last Updated**: 2024  
**Author**: Embedded Control Systems Learning Module  
**License**: MIT (Educational Use)

---

*Dokumen ini adalah bagian dari repository ESP32-Fieldbus-Integration. Untuk kode implementasi lengkap, lihat file `src/pid_control_module.c`.*
