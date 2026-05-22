# MISRA C:2012 Style Guide untuk Embedded Automotive

## 📋 Informasi Dokumen

| Atribut | Deskripsi |
|---------|-----------|
| **Standar** | MISRA C:2012 (Motor Industry Software Reliability Association) |
| **Level** | Intermediate → Advanced |
| **Prasyarat** | Dasar-dasar C, pemahaman mikrokontroler |
| **Relevansi** | Safety-critical systems, ISO 26262 compliance |
| **Estimasi Waktu** | 4-6 jam studi + latihan |
| **File Terkait** | [06_ISO_26262_Safety_Concepts.md](./06_ISO_26262_Safety_Concepts.md), [03_AUTOSAR_Classic_Arch.md](./03_AUTOSAR_Classic_Arch.md) |

---

## 🎯 Tujuan Pembelajaran

Setelah mempelajari materi ini, Anda akan mampu:

1. **Memahami filosofi MISRA C** - Mengapa standar ini penting untuk otomotif
2. **Mengidentifikasi pelanggaran MISRA** - Mendeteksi kode yang tidak compliant
3. **Menerapkan aturan wajib** - Mandatory rules yang harus dipatuhi
4. **Menulis kode production-ready** - Kode yang aman, maintainable, dan portable
5. **Melakukan code review berbasis MISRA** - Evaluasi kode dengan checklist sistematis
6. **Mengintegrasikan dengan tool static analysis** - PC-lint, SonarQube, dll

---

## 📚 Daftar Isi

1. [Pendahuluan MISRA C](#1-pendahuluan-misra-c)
2. [Kategori Aturan MISRA](#2-kategori-aturan-misra)
3. [Aturan Wajib (Mandatory Rules)](#3-aturan-wajib-mandatory-rules)
4. [Aturan Required (Required Rules)](#4-aturan-required-required-rules)
5. [Aturan Advisory (Advisory Rules)](#5-aturan-advisory-advisory-rules)
6. [Studi Kasus: Refactoring Kode](#6-studi-kasus-refactoring-kode)
7. [Integrasi dengan Development Workflow](#7-integrasi-dengan-development-workflow)
8. [Checklist Code Review](#8-checklist-code-review)
9. [Latihan dan Tantangan](#9-latihan-dan-tantangan)
10. [Glosarium Jepang-Inggris](#10-glosarium-jepang-inggris)

---

## 1. Pendahuluan MISRA C

### 1.1 Apa itu MISRA C?

**MISRA C** (Motor Industry Software Reliability Association C) adalah sekumpulan aturan dan pedoman untuk penggunaan bahasa pemrograman C dalam pengembangan sistem embedded, khususnya di industri otomotif.

```
┌─────────────────────────────────────────────────────────────┐
│                    MISRA C History                          │
├─────────────────────────────────────────────────────────────┤
│  MISRA C:1998  → First edition (127 rules)                  │
│  MISRA C:2004  → Second edition (141 rules)                 │
│  MISRA C:2012  → Third edition (143 rules, 3 categories)    │
│  MISRA C:2023  → Latest edition (under development)         │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Mengapa MISRA C Penting?

#### Alasan Utama:

| Alasan | Penjelasan | Contoh Dampak |
|--------|------------|---------------|
| **Safety** | Mencegah undefined behavior yang bisa menyebabkan kegagalan sistem | Crash pada brake control system |
| **Security** | Mengurangi vulnerability yang bisa dieksploitasi | Remote code execution melalui buffer overflow |
| **Portability** | Memastikan kode berjalan di berbagai compiler/platform | Migrasi dari AVR ke ARM tanpa rewrite |
| **Maintainability** | Kode lebih mudah dibaca dan dimodifikasi | Reduksi bug saat feature addition |
| **Compliance** | Requirement untuk ISO 26262 dan standar safety lainnya | Sertifikasi ASIL B/C/D |

#### Statistik Industri:

```
┌──────────────────────────────────────────────────────────────┐
│  Dampak Penerapan MISRA C pada Project Otomotif             │
├──────────────────────────────────────────────────────────────┤
│  • 40-60% pengurangan defect rate                            │
│  • 30% peningkatan kecepatan code review                     │
│  • 50% reduksi waktu debugging                               │
│  • 25% penurunan biaya maintenance                           │
│  • 100% compliance requirement untuk ASIL C/D                │
└──────────────────────────────────────────────────────────────┘
```

### 1.3 Hubungan MISRA C dengan Standar Lain

```
                    ┌─────────────────┐
                    │   ISO 26262     │
                    │ (Functional     │
                    │     Safety)     │
                    └────────┬────────┘
                             │ requires
                             ▼
┌─────────────────────────────────────────────────────────────┐
│                      MISRA C:2012                            │
│              (Coding Standard for Safety)                    │
└─────────────────────────────────────────────────────────────┘
                             │ supports
                             ▼
                    ┌─────────────────┐
                    │    AUTOSAR      │
                    │   (Architecture)│
                    └─────────────────┘
```

**Hubungan Kunci:**
- **ISO 26262** → Mensyaratkan coding standard untuk ASIL B/C/D
- **MISRA C** → Coding standard yang direkomendasikan/diwajibkan
- **AUTOSAR** → Menggunakan MISRA C sebagai baseline coding guideline

---

## 2. Kategori Aturan MISRA

### 2.1 Tiga Kategori Aturan

MISRA C:2012 mengklasifikasikan aturan menjadi 3 kategori berdasarkan tingkat kepatuhan:

#### 1. **Mandatory (Wajib)** - 21 aturan
- **Harus** dipatuhi tanpa kecuali
- Tidak boleh ada deviasi
- Pelanggaran = non-compliant
- Format: "Shall" dalam spesifikasi

```c
// ❌ VIOLATION - Mandatory Rule 2.1
int main() {  // Missing return type (C99 implicit int)
    return 0;
}

// ✅ COMPLIANT
int main(void) {
    return 0;
}
```

#### 2. **Required (Diperlukan)** - 92 aturan
- **Harus** dipatuhi, tapi boleh ada deviasi dengan justifikasi formal
- Perlu dokumentasi jika ada exception
- Format: "Shall" dalam spesifikasi

```c
// ❌ VIOLATION - Required Rule 10.1
uint8_t a = 5;
uint8_t b = 10;
uint8_t c = a + b;  // Operan integer promotion tidak eksplisit

// ✅ COMPLIANT
uint8_t a = 5U;
uint8_t b = 10U;
uint8_t c = (uint8_t)((uint8_t)a + (uint8_t)b);  // Cast eksplisit
```

#### 3. **Advisory (Anjuran)** - 30 aturan
- **Sebaiknya** dipatuhi
- Deviasi diperbolehkan dengan pertimbangan engineering
- Best practices untuk maintainability
- Format: "Should" dalam spesifikasi

```c
// ⚠️ ADVISORY VIOLATION - Rule 17.8
void processData(int data) {
    // Parameter 'data' dimodifikasi
    data = data * 2;  // Sebaiknya parameter tidak dimodifikasi
}

// ✅ BETTER PRACTICE
void processData(const int data) {
    int result = data * 2;  // Gunakan variabel lokal
}
```

### 2.2 Matriks Kepatuhan

| Kategori | Jumlah Aturan | Deviasi Diperbolehkan? | Justifikasi Formal | Tool Enforcement |
|----------|---------------|------------------------|--------------------|------------------|
| **Mandatory** | 21 | ❌ Tidak | N/A | Hard error |
| **Required** | 92 | ✅ Ya (terbatas) | Diperlukan | Warning/Error |
| **Advisory** | 30 | ✅ Ya | Disarankan | Warning/Info |

### 2.3 Proses Deviasi (Deviation Process)

Jika harus melakukan deviasi dari aturan Required:

```
┌──────────────────────────────────────────────────────────────┐
│              Deviation Approval Process                      │
├──────────────────────────────────────────────────────────────┤
│  1. Identifikasi aturan yang akan di-deviate                 │
│  2. Dokumentasikan alasan teknis                             │
│  3. Analisis dampak safety                                   │
│  4. Dapatkan approval dari safety manager                    │
│  5. Catat dalam deviation log                                │
│  6. Review berkala (setiap 6 bulan)                          │
└──────────────────────────────────────────────────────────────┘
```

**Template Deviation Document:**

```markdown
## Deviation Request Form

**Rule ID:** Rule 11.8  
**Rule Description:** A pointer shall not be cast to a different pointer type  
**Project:** Engine Control Unit (ECU) Firmware  
**Date:** 2024-01-15  

### Justification
Hardware register access requires casting void pointer to specific 
register structure type. This is unavoidable for memory-mapped I/O.

### Risk Assessment
- Severity: Low (isolated to HAL layer)
- Mitigation: Encapsulated in macro, reviewed by senior engineer
- Testing: Additional unit tests added for register access

### Approval
- Author: [Nama]
- Reviewer: [Nama Senior Engineer]
- Safety Manager: [Nama]
- Status: APPROVED (valid until 2025-01-15)
```

---

## 3. Aturan Wajib (Mandatory Rules)

### 3.1 Overview Mandatory Rules

Terdapat **21 aturan mandatory** yang HARUS dipatuhi tanpa kecuali:

| Rule ID | Deskripsi | Prioritas |
|---------|-----------|-----------|
| 2.1 | All code shall conform to ISO C standard | 🔴 Critical |
| 2.2 | All source files shall compile without errors | 🔴 Critical |
| 2.3 | Project shall not contain unused code | 🟡 High |
| 2.4 | Project shall not contain unused variables | 🟡 High |
| 2.5 | Function-like macros shall not be defined | 🟡 High |
| 2.6 | Functions shall not be declared with implicit return type | 🔴 Critical |
| 2.7 | Values shall not be discarded unnecessarily | 🟡 Medium |
| 3.1 | External identifiers shall be distinct | 🟡 Medium |
| 3.2 | Line-splicing shall not be used | 🟡 Low |
| 4.1 | Octal and hexadecimal escape sequences shall be valid | 🟡 Medium |
| 4.2 | Trigraphs shall not be used | 🟡 Low |
| 5.1 | External identifiers shall not conflict | 🔴 Critical |
| 5.2 | Identifiers shall not conflict in same scope | 🔴 Critical |
| 5.3 | Identifiers shall be unique in global namespace | 🔴 Critical |
| 5.4 | Macro identifiers shall be unique | 🟡 Medium |
| 5.5 | Identifiers shall not have more than one declaration | 🔴 Critical |
| 5.6 | No identifier shall have more than one external linkage | 🔴 Critical |
| 5.7 | No identifier shall be declared with more than one type | 🔴 Critical |
| 5.8 | Identifiers shall not be redefined in conflicting ways | 🔴 Critical |
| 5.9 | Objects shall not be defined with more than one storage class | 🔴 Critical |
| 6.1 | Bit-fields shall only be declared with unsigned int or signed int | 🟡 Medium |

### 3.2 Deep Dive: Rule 2.1 - Conformance to ISO C

**Statement:**
> All code shall conform to the ISO/IEC 9899:1999 standard (C99).

**Rationale:**
- Memastikan portabilitas antar compiler
- Menghindari compiler-specific extensions yang mungkin tidak supported
- Memfasilitasi static analysis

**Contoh Pelanggaran:**

```c
// ❌ VIOLATION - Non-standard extension (GCC specific)
void func(void) {
    int array[10];
    
    // Extension: named constant initialization
    int values[] = {[5] = 100, [8] = 200};  // GCC extension
    
    // Extension: statement expressions
    int x = ({ int y = 5; y * 2; });  // GCC extension
}

// ✅ COMPLIANT - Standard C99
void func(void) {
    int array[10];
    int values[10] = {0};
    
    values[5] = 100;
    values[8] = 200;
    
    int y = 5;
    int x = y * 2;
}
```

**Verifikasi Compliance:**

```bash
# Compile dengan strict C99 mode
gcc -std=c99 -pedantic-errors -Wall -Wextra source.c

# Dengan PC-lint
pc-lint +f99 -esym(9xx) source.c
```

### 3.3 Deep Dive: Rule 2.2 - Compilation Without Errors

**Statement:**
> All source files shall compile without any errors.

**Rationale:**
- Kode yang tidak compile = tidak bisa di-test
- Compiler warnings sering mengindikasikan potential bugs
- Early detection of issues

**Common Issues:**

```c
// ❌ VIOLATION - Implicit function declaration
void processSensor(void) {
    readADC();  // Function not declared before use
}

// ✅ COMPLIANT
#include "adc_driver.h"  // Declaration in header

void processSensor(void) {
    readADC();  // Function properly declared
}

// ❌ VIOLATION - Type mismatch
int getValue(void) {
    return 3.14;  // Returning float from int function
}

// ✅ COMPLIANT
float getValue(void) {
    return 3.14f;
}
```

### 3.4 Deep Dive: Rule 2.3 & 2.4 - Unused Code/Variables

**Statement:**
> Project shall not contain unused code or variables.

**Rationale:**
- Dead code meningkatkan maintenance burden
- Unused variables waste memory
- Indikasi incomplete refactoring atau bugs

**Deteksi dan Cleanup:**

```c
// ❌ VIOLATION - Unused variable and dead code
void calculateEngineLoad(uint16_t rpm, uint16_t throttle) {
    uint16_t temp_value = 0;  // Unused variable
    uint16_t base_load = 0;
    
    base_load = (rpm * throttle) / 1000;
    
    // Dead code - unreachable
    #if FEATURE_DISABLED
    temp_value = base_load * 2;
    sendToCAN(temp_value);
    #endif
    
    return base_load;
}

// ✅ COMPLIANT
void calculateEngineLoad(uint16_t rpm, uint16_t throttle) {
    uint16_t base_load = (rpm * throttle) / 1000;
    
    return base_load;
}
```

**Tool Support:**

```bash
# GCC warnings untuk unused code
gcc -Wunused-variable -Wunused-function -Wdead-code source.c

# Clang static analyzer
scan-build gcc -c source.c

# PC-lint configuration
+warn-unused
+warn-unreachable
```

### 3.5 Deep Dive: Rule 5.1-5.9 - Identifier Uniqueness

**Statement:**
> Identifiers shall be unique across various scopes to avoid conflicts.

**Rationale:**
- Mencegah ambiguity dan naming conflicts
- Memudahkan code navigation dan maintenance
- Critical untuk large codebases dengan multiple developers

**Contoh Konflik:**

```c
// ❌ VIOLATION - Multiple declarations with external linkage
// File: sensor.c
int sensor_count = 0;

// File: actuator.c  
int sensor_count = 0;  // Conflict! Same external identifier

// ✅ COMPLIANT - Static linkage atau unique names
// File: sensor.c
static int sensor_count = 0;  // File scope only

// File: actuator.c
static int actuator_count = 0;  // Different name
```

**Best Practices:**

```c
// Module prefix untuk global identifiers
typedef struct {
    uint16_t engine_rpm;
    uint8_t engine_temp;
} Engine_StatusType;

Engine_StatusType Engine_getStatus(void);
void Engine_setTargetRPM(uint16_t rpm);

// Static functions untuk internal module use
static void validateParameters(uint16_t rpm);
static uint16_t calculateLoad(uint16_t rpm, uint16_t throttle);
```

---

## 4. Aturan Required (Required Rules)

### 4.1 Overview Required Rules

Terdapat **92 aturan required** yang harus dipatuhi dengan kemungkinan deviasi terbatas. Berikut aturan-aturan paling kritis:

#### Category 6: Data Types

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 6.1 | Bit-fields shall only be declared with unsigned int or signed int | Memory layout |
| 6.2 | Single-bit bit-fields shall not be signed | Portability |
| 8.1 | Types shall be explicitly specified | Type safety |
| 8.2 | Function types shall be in prototype form | Type checking |
| 8.3 | Declarations shall be in same scope | Consistency |
| 8.4 | Compatible types shall be consistent | Type safety |
| 8.5 | Object definitions shall not have conflicting types | Type integrity |
| 8.6 | Objects shall be defined at block scope if possible | Scope minimization |
| 8.7 | Functions shall not be defined with external linkage if unused | Linkage control |
| 8.8 | External objects shall be const if not modified | Const correctness |
| 8.9 | Objects should be const if not modified | Const correctness |
| 8.10 | Inline functions shall not be extern | Linkage |
| 8.11 | External identifiers shall have internal linkage when possible | Encapsulation |
| 8.12 | Enumerations shall not have mixed signs | Type consistency |
| 8.13 | Pointer shall point to const if data not modified | Const correctness |
| 8.14 | Restrict qualifier shall be used appropriately | Optimization |

#### Category 7: Literals and Constants

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 7.1 | Octal constants shall not be used | Readability |
| 7.2 | Hexadecimal constants shall have uppercase letters | Consistency |
| 7.3 | Lowercase character 'l' shall not be used in literals | Readability |
| 7.4 | String literals shall not be concatenated | Maintainability |

#### Category 8: Declarations and Definitions

(Lihat detail di section 4.2-4.8)

#### Category 9: Initialization

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 9.1 | Objects shall be initialized | Predictability |
| 9.2 | Array elements shall be initialized | Safety |
| 9.3 | Objects shall be initialized before use | Undefined behavior prevention |

#### Category 10: Type Conversion

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 10.1 | Operands shall not be of inappropriate essential type | Type safety |
| 10.2 | Expressions shall not be of mixed essential types | Type consistency |
| 10.3 | Value of expression shall not be assigned to object with narrower essential type | Overflow prevention |
| 10.4 | Both operands of operator shall have same essential type category | Type safety |
| 10.5 | Result of operation shall not be discarded | Bug prevention |
| 10.6 | Value composed from composite literal shall be assigned to object of same type | Type matching |
| 10.7 | Composite literal shall have braces | Clarity |
| 10.8 | Impermissible conversion shall not be performed | Type safety |

#### Category 11: Pointers

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 11.1 | Conversions shall not be performed between pointers to different types | Type safety |
| 11.2 | Conversions shall not be performed between pointer to integer | Portability |
| 11.3 | Cast shall not be performed between pointer to void and pointer to function | Safety |
| 11.4 | Conversion between pointer to void and pointer to other type shall have explicit cast | Clarity |
| 11.5 | Object shall not be accessed via incompatible pointer | Type safety |
| 11.6 | Null pointer shall not be dereferenced | Crash prevention |
| 11.7 | Immediate operand of unary & operator shall not be of array type | Clarity |
| 11.8 | Pointer shall not be cast to different pointer type | Type safety |
| 11.9 | Macro NULL shall be used for null pointer constant | Consistency |

#### Category 12: Expressions

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 12.1 | Priority of operators shall be understood | Predictability |
| 12.2 | Right hand operand of shift operator shall be in range | Undefined behavior |
| 12.3 | Comma operator shall not be used | Clarity |
| 12.4 | Evaluation order shall not depend on side effects | Predictability |
| 12.5 | Logical operators shall not have side effects | Predictability |

#### Category 13: Statements

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 13.1 | Assignment statements shall not be used as sub-expressions | Clarity |
| 13.2 | Expressions shall not have persistent side effects | Predictability |
| 13.3 | Floating-point expressions shall not be tested for equality | Precision |
| 13.4 | Result of assignment shall not be unused | Bug detection |
| 13.5 | Logical operators shall not have side effects | Predictability |
| 13.6 | Operand of sizeof operator shall not have side effects | Clarity |

#### Category 14: Control Flow

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 14.1 | Loop counter shall not be modified inside loop body | Predictability |
| 14.2 | For loop shall have well-defined termination condition | Infinite loop prevention |
| 14.3 | Preprocessing directives shall not be used in control flow | Clarity |
| 14.4 | Switch statements shall have default label | Completeness |
| 14.5 | Continue statement shall not be used | Clarity |
| 14.6 | Goto statement shall not be used | Structured programming |

#### Category 15: Functions

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 15.1 | Function shall not return pointer to local automatic variable | Dangling pointer |
| 15.2 | Exit, abort, raise functions shall not be used | Controlled termination |
| 15.3 | va_start, va_arg, va_end shall be used correctly | Variadic safety |
| 15.4 | Function shall not call itself recursively | Stack overflow |
| 15.5 | Function shall have single exit point | Maintainability |
| 15.6 | Preprocessor directives shall not redefine standard library macros | Portability |
| 15.7 | All paths through function shall return value | Completeness |

#### Category 16: Control Flow (Extended)

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 16.1 | Switch labels shall be in switch statement | Syntax |
| 16.2 | Labels shall only be referenced from within same function | Scope |
| 16.3 | Default label shall be present in switch | Completeness |
| 16.4 | Every case label shall end with break/goto/return/throw | Fall-through prevention |
| 16.5 | Designated default label shall appear in every switch | Completeness |
| 16.6 | Switch expression shall not have Boolean type | Clarity |
| 16.7 | Deliberate fall-through shall be documented | Clarity |

#### Category 17: Function Parameters

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 17.1 | Variable-length arrays shall not be used | Stack safety |
| 17.2 | Functions shall not be declared with variable number of arguments | Type safety |
| 17.3 | Function shall not be declared implicitly | Type checking |
| 17.4 | Array parameters shall be declared with size | Clarity |
| 17.5 | Function arguments shall match parameter types | Type safety |
| 17.6 | Static array parameters shall have compatible sizes | Type matching |
| 17.7 | Function shall not return aggregate type | Efficiency |
| 17.8 | Function parameters shall not be modified | Const correctness |

#### Category 18: Arrays and Pointers

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 18.1 | Pointer arithmetic shall only be performed on arrays | Safety |
| 18.2 | Comparison of pointers to different objects shall not be performed | Safety |
| 18.3 | Relation operators shall not be performed on pointers to different objects | Safety |
| 18.4 | Pointer arithmetic shall not wrap around | Safety |
| 18.5 | Declarations shall contain no more than 2 levels of pointer indirection | Complexity |
| 18.6 | Address of object with automatic storage shall not be returned | Dangling pointer |
| 18.7 | Flexible array members shall not be declared | Portability |
| 18.8 | Variable-length arrays shall not be used | Stack safety |

#### Category 19: Preprocessing

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 19.1 | Preprocessor directives shall only be #include, #define, #undef, etc. | Safety |
| 19.2 | #pragma directive shall not be used | Portability |

#### Category 20: Preprocessing (Extended)

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 20.1 | #include directive shall only reference standard headers or project headers | Safety |
| 20.2 | Literal suffixes shall be uppercase | Consistency |
| 20.3 | Undef macro shall not be used for standard library macros | Safety |
| 20.4 | Macro shall not be redefined | Clarity |
| 20.5 | #undef shall not be used for standard library identifiers | Safety |
| 20.6 | Tokens that look like preprocessing directives shall not occur in string literals | Safety |
| 20.7 | Expressions resulting from macro expansion shall be enclosed in parentheses | Safety |
| 20.8 | Macro parameters shall be enclosed in parentheses | Safety |
| 20.9 | Macros shall not expand to compound literals | Clarity |
| 20.10 | Variadic macros shall not be used | Portability |
| 20.11 | Macro arguments shall not be tokenized within the macro definition | Safety |
| 20.12 | Macro replacements shall not cause overflow | Safety |
| 20.13 | Directive #line shall not be used | Debugging |
| 20.14 | All #else, #elif, #endif directives shall match appropriate #if/#ifdef/#ifndef | Syntax |

#### Category 21: Standard Libraries

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 21.1 | #defined identifiers and typedef names shall be unique | Clarity |
| 21.2 | Standard library functions shall not be redefined | Safety |
| 21.3 | Resources shall be freed after last use | Memory leak prevention |
| 21.4 | Functions from stdio.h shall not be used | Safety |
| 21.5 | Functions from time.h shall not be used | Safety |
| 21.6 | Functions from stdlib.h (except atof, atoi, atol, strtod, strtol, strtoul) shall not be used | Safety |
| 21.7 | atof, atoi, atol, atoll shall not be used | Error handling |
| 21.8 | Library functions shall not be called with invalid arguments | Safety |
| 21.9 | Functions strcmp, strcpy, strncpy, strcoll, strxfrm, strstr, strtok, memchr, memcmp, memcpy, memmove, memset, wcscmp, wcscpy, wcsncpy, wmemcmp, wmemcpy, wmemmove, wmemset shall not be used | Buffer overflow |
| 21.10 | Dynamic memory management functions shall not be used | Determinism |
| 21.11 | Maximum execution times of library functions shall be documented | Real-time |
| 21.12 | Signal handling functions shall not be used | Safety |
| 21.13 | Functions that perform input/output to designated streams shall not be used | Safety |

#### Category 22: Runtime Environment

| Rule ID | Deskripsi | Impact |
|---------|-----------|--------|
| 22.1 | All resources obtained dynamically shall be freed explicitly | Memory leak |
| 22.2 | Block of memory shall be explicitly deallocated after last use | Memory leak |
| 22.3 | Same function shall be used for allocation and deallocation | Compatibility |
| 22.4 | No attempt shall be made to free memory that was not allocated dynamically | Safety |
| 22.5 | Pointer to object shall not be used after deallocation | Use-after-free |
| 22.6 | Pointer to function shall not be used after deallocation | Safety |

### 4.2 Deep Dive: Rule 8.1 - Explicit Type Specification

**Statement:**
> Types shall be explicitly specified.

**Rationale:**
- Menghindari implicit type assumptions
- Meningkatkan readability dan maintainability
- Memfasilitasi static analysis

**Contoh:**

```c
// ❌ VIOLATION - Implicit type (old C style)
func(x, y)  // Types not specified
int x;
int y;
{
    return x + y;
}

// ✅ COMPLIANT - Explicit types
int func(int x, int y) {
    return x + y;
}

// ❌ VIOLATION - Implicit int return
calculate(value) {
    return value * 2;
}

// ✅ COMPLIANT
int calculate(int value) {
    return value * 2;
}
```

### 4.3 Deep Dive: Rule 10.1-10.8 - Type Conversion

**Statement:**
> Operations shall not be performed on mixed essential types without explicit conversion.

**Rationale:**
- Mencegah overflow/underflow
- Menghindari loss of precision
- Memastikan predictable behavior

**Essential Type Model:**

```
┌──────────────────────────────────────────────────────────────┐
│              MISRA Essential Type Categories                 │
├──────────────────────────────────────────────────────────────┤
│  Signed    : char, short, int, long, long long               │
│  Unsigned  : unsigned char, unsigned short, unsigned int...  │
│  Character : char, wchar_t                                   │
│  Boolean   : _Bool, bool                                     │
│  Floating  : float, double, long double                      │
│  Pointer   : All pointer types                               │
│  Enum      : Enumeration types                               │
│  Bitfield  : Bit-field types                                 │
└──────────────────────────────────────────────────────────────┘
```

**Contoh Pelanggaran dan Solusi:**

```c
// ❌ VIOLATION - Rule 10.1: Mixed essential types
uint8_t a = 100;
uint8_t b = 200;
uint8_t c = a + b;  // Integer promotion ke int, lalu truncation

// ✅ COMPLIANT
uint8_t a = 100U;
uint8_t b = 200U;
uint16_t temp = (uint16_t)a + (uint16_t)b;
uint8_t c = (temp > 255U) ? 255U : (uint8_t)temp;  // Saturation

// ❌ VIOLATION - Rule 10.3: Narrower type assignment
uint32_t large_value = 500000U;
uint16_t small_var = large_value;  // Potential overflow

// ✅ COMPLIANT
uint32_t large_value = 500000U;
if (large_value <= 65535U) {
    uint16_t small_var = (uint16_t)large_value;
} else {
    // Handle error
}

// ❌ VIOLATION - Rule 10.4: Mixed signed/unsigned
int16_t signed_val = -10;
uint16_t unsigned_val = 20;
int16_t result = signed_val + unsigned_val;  // Mixed types

// ✅ COMPLIANT
int16_t signed_val = -10;
uint16_t unsigned_val = 20;
int32_t result = (int32_t)signed_val + (int32_t)unsigned_val;
```

### 4.4 Deep Dive: Rule 11.1-11.9 - Pointers

**Statement:**
> Pointers shall be used safely with proper type conversions.

**Rationale:**
- Mencegah type confusion
- Menghindari undefined behavior
- Critical untuk memory safety

**Contoh:**

```c
// ❌ VIOLATION - Rule 11.1: Pointer conversion between different types
int value = 42;
int *int_ptr = &value;
char *char_ptr = (char *)int_ptr;  // Dangerous cast

// ✅ COMPLIANT - Use void* for generic pointer
int value = 42;
void *generic_ptr = &value;
// Later, cast back to original type
int *int_ptr = (int *)generic_ptr;

// ❌ VIOLATION - Rule 11.4: Missing explicit cast
void *ptr = malloc(sizeof(int));
int *int_ptr = ptr;  // Missing cast

// ✅ COMPLIANT
void *ptr = malloc(sizeof(int));
int *int_ptr = (int *)ptr;  // Explicit cast

// ❌ VIOLATION - Rule 11.6: Null pointer dereference
int *ptr = NULL;
*ptr = 42;  // Crash!

// ✅ COMPLIANT
int *ptr = NULL;
if (ptr != NULL) {
    *ptr = 42;
}

// ❌ VIOLATION - Rule 11.8: Casting between incompatible pointer types
typedef struct {
    uint16_t id;
    uint8_t data[8];
} CAN_MessageType;

typedef struct {
    uint32_t timestamp;
    uint16_t id;
    uint8_t data[8];
} Extended_CAN_MessageType;

CAN_MessageType msg1;
Extended_CAN_MessageType *msg2 = (Extended_CAN_MessageType *)&msg1;  // Dangerous!

// ✅ COMPLIANT - Copy data explicitly
CAN_MessageType msg1;
Extended_CAN_MessageType msg2;
msg2.id = msg1.id;
memcpy(msg2.data, msg1.data, 8);
```

### 4.5 Deep Dive: Rule 14.1-14.6 - Control Flow

**Statement:**
> Control flow constructs shall be used in a structured and predictable manner.

**Rationale:**
- Meningkatkan readability
- Mencegah infinite loops
- Memudahkan verification

**Contoh:**

```c
// ❌ VIOLATION - Rule 14.1: Loop counter modified inside loop
for (int i = 0; i < 10; i++) {
    if (condition) {
        i = 5;  // Modifying loop counter
    }
}

// ✅ COMPLIANT
int i = 0;
while (i < 10) {
    if (condition) {
        i = 5;  // Clear intent with while loop
    }
    i++;
}

// ❌ VIOLATION - Rule 14.2: For loop with unclear termination
for (int i = 0; ; i++) {  // No termination condition
    if (checkDone()) {
        break;
    }
}

// ✅ COMPLIANT
#define MAX_ITERATIONS 1000
for (int i = 0; i < MAX_ITERATIONS; i++) {
    if (checkDone()) {
        break;
    }
}

// ❌ VIOLATION - Rule 14.4: Switch without default
switch (mode) {
    case MODE_AUTO:
        handleAuto();
        break;
    case MODE_MANUAL:
        handleManual();
        break;
    // No default case
}

// ✅ COMPLIANT
switch (mode) {
    case MODE_AUTO:
        handleAuto();
        break;
    case MODE_MANUAL:
        handleManual();
        break;
    default:
        handleError();  // Handle unexpected values
        break;
}

// ❌ VIOLATION - Rule 14.5: Continue statement
for (int i = 0; i < 10; i++) {
    if (skipCondition(i)) {
        continue;  // Jump to next iteration
    }
    process(i);
}

// ✅ COMPLIANT
for (int i = 0; i < 10; i++) {
    if (!skipCondition(i)) {
        process(i);
    }
}

// ❌ VIOLATION - Rule 14.6: Goto statement
void process(void) {
    if (error1) {
        goto cleanup;
    }
    if (error2) {
        goto cleanup;
    }
    return;
cleanup:
    cleanupResources();
}

// ✅ COMPLIANT - Structured error handling
void process(void) {
    if (error1) {
        cleanupResources();
        return;
    }
    if (error2) {
        cleanupResources();
        return;
    }
}
```

### 4.6 Deep Dive: Rule 17.1-17.8 - Function Parameters

**Statement:**
> Function parameters shall be used safely and consistently.

**Rationale:**
- Mencegah stack overflow
- Memastikan type safety
- Meningkatkan maintainability

**Contoh:**

```c
// ❌ VIOLATION - Rule 17.1: Variable-length array
void processArray(int size) {
    int arr[size];  // VLA - stack size unpredictable
    // ...
}

// ✅ COMPLIANT - Fixed size or dynamic allocation
#define MAX_ARRAY_SIZE 100
void processArray(int size) {
    if (size > MAX_ARRAY_SIZE) {
        return;  // Error handling
    }
    int arr[MAX_ARRAY_SIZE];
    // ...
}

// ❌ VIOLATION - Rule 17.4: Array parameter without size
void processData(int data[]) {
    // How many elements?
}

// ✅ COMPLIANT
void processData(int data[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        // Process data[i]
    }
}

// ❌ VIOLATION - Rule 17.8: Modifying function parameter
void calculate(int value) {
    value = value * 2;  // Modifying parameter
    return value;
}

// ✅ COMPLIANT
int calculate(int value) {
    int result = value * 2;  // Use local variable
    return result;
}

// Better: Use const for input parameters
void processInput(const int value) {
    // Cannot modify 'value' - enforced by compiler
    int result = value * 2;
}
```

### 4.7 Deep Dive: Rule 18.1-18.8 - Arrays and Pointers

**Statement:**
> Array and pointer operations shall be performed safely within bounds.

**Rationale:**
- Mencegah buffer overflow
- Menghindari memory corruption
- Critical untuk security

**Contoh:**

```c
// ❌ VIOLATION - Rule 18.1: Pointer arithmetic not on array
int value = 42;
int *ptr = &value;
int *next = ptr + 1;  // Pointer arithmetic on non-array

// ✅ COMPLIANT
int arr[10] = {0};
int *ptr = arr;
int *next = ptr + 1;  // Valid array pointer arithmetic

// ❌ VIOLATION - Rule 18.3: Comparing pointers to different objects
int a = 10;
int b = 20;
int *ptr_a = &a;
int *ptr_b = &b;
if (ptr_a < ptr_b) {  // Comparing pointers to different objects
    // ...
}

// ✅ COMPLIANT
int arr[10] = {0};
int *ptr1 = &arr[2];
int *ptr2 = &arr[5];
if (ptr1 < ptr2) {  // Valid - same array
    // ...
}

// ❌ VIOLATION - Rule 18.4: Pointer arithmetic wrapping
uint8_t buffer[256];
uint8_t *ptr = &buffer[250];
uint8_t *wrapped = ptr + 10;  // Wraps around!

// ✅ COMPLIANT
uint8_t buffer[256];
uint8_t *ptr = &buffer[250];
if ((ptr - buffer) + 10 <= 256) {
    uint8_t *valid = ptr + 10;
    // Use valid pointer
} else {
    // Handle error
}

// ❌ VIOLATION - Rule 18.5: More than 2 levels of indirection
int ***triple_ptr;  // Too complex

// ✅ COMPLIANT
typedef struct {
    int **data;
    size_t rows;
    size_t cols;
} MatrixType;

MatrixType matrix;  // Clear structure

// ❌ VIOLATION - Rule 18.6: Returning address of local variable
int* getLocalPointer(void) {
    int local = 42;
    return &local;  // Dangling pointer!
}

// ✅ COMPLIANT
int global_value = 42;
int* getGlobalPointer(void) {
    return &global_value;  // Safe - static storage
}

// Or use caller-provided buffer
void getValue(int *output) {
    if (output != NULL) {
        *output = 42;
    }
}
```

### 4.8 Deep Dive: Rule 21.1-21.13 - Standard Libraries

**Statement:**
> Standard library functions shall be used safely or avoided in safety-critical contexts.

**Rationale:**
- Banyak fungsi standard library tidak deterministic
- Potensi buffer overflow
- Tidak suitable untuk real-time systems

**Restricted Functions:**

```c
// ❌ VIOLATION - Rule 21.4: Using stdio.h functions
printf("Value: %d\n", value);  // Not allowed in safety-critical code
sprintf(buffer, "%d", value);  // Buffer overflow risk

// ✅ COMPLIANT - Use custom logging
void logMessage(const char *msg) {
    UART_TransmitString(msg);  // Custom implementation
}

// ❌ VIOLATION - Rule 21.9: Using unsafe string functions
char dest[10];
strcpy(dest, source);  // No bounds checking!

// ✅ COMPLIANT - Use safe alternatives
char dest[10];
strncpy(dest, source, sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';  // Ensure null termination

// Or better: custom safe string function
Std_ReturnType safe_strcpy(char *dest, const char *source, size_t dest_size) {
    if (dest == NULL || source == NULL || dest_size == 0) {
        return E_NOT_OK;
    }
    
    size_t i;
    for (i = 0; i < dest_size - 1 && source[i] != '\0'; i++) {
        dest[i] = source[i];
    }
    dest[i] = '\0';
    
    return E_OK;
}

// ❌ VIOLATION - Rule 21.10: Dynamic memory allocation
int *arr = malloc(100 * sizeof(int));  // Not deterministic
free(arr);

// ✅ COMPLIANT - Static allocation
#define MAX_ARRAY_SIZE 100
static int arr[MAX_ARRAY_SIZE];  // Static memory

// Or memory pool for controlled allocation
typedef struct {
    uint8_t buffer[MEMORY_POOL_SIZE];
    uint8_t used[MEMORY_POOL_SIZE / BLOCK_SIZE];
} MemoryPoolType;

static MemoryPoolType memory_pool;

void* pool_alloc(size_t size) {
    // Custom allocator with bounded time
}

void pool_free(void *ptr) {
    // Custom deallocator
}
```

---

## 5. Aturan Advisory (Advisory Rules)

### 5.1 Overview Advisory Rules

Terdapat **30 aturan advisory** yang merupakan best practices:

| Rule ID | Deskripsi | Recommendation |
|---------|-----------|----------------|
| 1.1 | Program shall contain no unnecessary code | Remove dead code |
| 1.2 | Language extensions should not be used | Use standard C |
| 1.3 | Complex expressions should be avoided | Improve readability |
| 2.8 | Dependencies between functions should be minimized | Loose coupling |
| 2.9 | Functions should be small and focused | Single responsibility |
| 3.3 | Literal strings should not be used for control flags | Use enums |
| 4.3 | Character sets should be documented | Portability |
| 5.10 | Functions should not have too many parameters | Max 5-6 parameters |
| 6.3 | Bit-fields should not span storage unit boundaries | Portability |
| 7.5 | Literal suffixes should be used | Type clarity |
| 8.15 | External objects should be const if not modified | Const correctness |
| 9.4 | Elements of union should not be accessed simultaneously | Type safety |
| 10.9 | Floating-point types should not be used in loop counters | Precision |
| 11.10 | Pointer to void should not be used except for generic functions | Type safety |
| 12.2 | Shift operators should be used with care | Overflow prevention |
| 13.8 | Side effects in assertions should be avoided | Testing integrity |
| 15.8 | Functions should have single exit point | Maintainability |
| 16.8 | Switch expression should be of enumerated type | Type safety |
| 17.9 | Function parameters should be passed by value for small types | Efficiency |
| 18.9 | Array subscripts should be of signed type | Flexibility |
| 19.3 | #include should only reference standard or project headers | Safety |
| 20.15 | Preprocessing directives should be easy to understand | Readability |
| 21.14 | Library functions should be wrapped for error handling | Robustness |
| 22.7 | Memory should be initialized before use | Predictability |

### 5.2 Best Practices Implementation

```c
// ✅ Advisory Rule 2.9: Small, focused functions
// Bad: Large function doing multiple things
void processEngineData(void) {
    // Read sensors (20 lines)
    // Validate data (15 lines)
    // Calculate parameters (30 lines)
    // Log results (10 lines)
    // Send to CAN (15 lines)
    // Total: 90 lines - too complex!
}

// Good: Separated into focused functions
void processEngineData(void) {
    SensorDataType sensors = readSensors();
    
    if (!validateSensorData(sensors)) {
        handleSensorError();
        return;
    }
    
    EngineParamType params = calculateParameters(sensors);
    logResults(params);
    sendToCAN(params);
}

// ✅ Advisory Rule 5.10: Limited function parameters
// Bad: Too many parameters
void configureTimer(uint16_t prescaler, uint16_t period, 
                   uint8_t mode, uint8_t interrupt_enable,
                   uint8_t capture_mode, uint8_t output_mode,
                   uint16_t compare_value, uint32_t clock_source);

// Good: Use configuration structure
typedef struct {
    uint16_t prescaler;
    uint16_t period;
    uint8_t mode;
    uint8_t interrupt_enable;
    uint8_t capture_mode;
    uint8_t output_mode;
    uint16_t compare_value;
    uint32_t clock_source;
} TimerConfigType;

void configureTimer(const TimerConfigType *config);

// ✅ Advisory Rule 15.8: Single exit point
// Bad: Multiple exit points
int validateData(int value) {
    if (value < 0) {
        return -1;
    }
    if (value > 100) {
        return -2;
    }
    if (value == 50) {
        return -3;
    }
    return 0;
}

// Good: Single exit point
int validateData(int value) {
    int result = 0;
    
    if (value < 0) {
        result = -1;
    } else if (value > 100) {
        result = -2;
    } else if (value == 50) {
        result = -3;
    }
    
    return result;
}
```

---

## 6. Studi Kasus: Refactoring Kode

### 6.1 Case Study 1: Sensor Processing Module

**Before (Non-Compliant):**

```c
// sensor_processor.c - BEFORE refactoring
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sensor_count = 0;  // Global variable
char buffer[100];

process_sensor(id, value, flag) {  // Implicit types
    int temp;
    char *msg;
    
    if (flag = 1) {  // Assignment instead of comparison
        temp = value * 2;
    }
    
    sprintf(buffer, "Sensor %d: %d", id, temp);  // Unsafe sprintf
    printf("%s\n", buffer);  // Using stdio.h
    
    char *dyn_msg = malloc(50);
    strcpy(dyn_msg, buffer);  // No bounds check
    
    for (int i = 0; i <= 10; i++) {  // Off-by-one error
        sensor_array[i] = i * 2;
    }
    
    goto cleanup;  // Goto statement
    
cleanup:
    free(dyn_msg);
}
```

**After (MISRA Compliant):**

```c
// sensor_processor.c - AFTER refactoring
#include "sensor_processor.h"
#include "uart_driver.h"
#include "safe_string.h"

/* File-scope variables with static linkage */
static uint8_t sensor_count = 0U;
static char buffer[SENSOR_BUFFER_SIZE];

/**
 * @brief Process sensor data with MISRA compliance
 * @param[in] id Sensor identifier
 * @param[in] value Sensor reading
 * @param[in] flag Processing flag
 * @return Std_ReturnType Operation result
 */
Std_ReturnType process_sensor(const uint8_t id, 
                              const uint16_t value,
                              const uint8_t flag)
{
    Std_ReturnType result = E_OK;
    uint16_t temp = 0U;
    
    /* Validate inputs */
    if (id >= MAX_SENSORS) {
        result = E_NOT_OK;
    } else {
        /* Rule 13.1: No assignment in condition */
        if (flag == 1U) {
            temp = value * 2U;
        } else {
            temp = value;
        }
        
        /* Rule 21.4, 21.9: Use safe string functions */
        Std_ReturnType str_result = safe_snprintf(
            buffer, 
            sizeof(buffer), 
            "Sensor %u: %u", 
            (unsigned int)id, 
            (unsigned int)temp
        );
        
        if (str_result == E_OK) {
            /* Custom UART output instead of printf */
            UART_TransmitString(buffer);
            UART_TransmitString("\r\n");
        }
        
        /* Rule 18.1, 18.4: Safe array access */
        for (uint8_t i = 0U; i < SENSOR_ARRAY_SIZE; i++) {
            sensor_array[i] = (uint16_t)i * 2U;
        }
        
        /* No goto - structured error handling */
        if (some_error_condition) {
            result = E_NOT_OK;
        }
    }
    
    return result;
}
```

### 6.2 Case Study 2: Communication Module

**Before (Non-Compliant):**

```c
// can_comm.c - BEFORE
void *can_buffer;

send_can_message(id, data, len) {
    can_buffer = malloc(len);
    memcpy(can_buffer, data, len);  // No validation
    
    CAN_IDR = id;
    CAN_DLCR = len;
    
    for (int i = 0; i < len; i++) {
        CAN_DR[i] = ((char*)can_buffer)[i];
    }
    
    CAN_CR |= 0x01;  // Magic number
    
    free(can_buffer);
}
```

**After (MISRA Compliant):**

```c
// can_comm.c - AFTER
#include "can_driver.h"

#define CAN_TX_REQUEST_MASK  0x01U
#define MAX_CAN_DLC          8U

static uint8_t can_tx_buffer[MAX_CAN_DLC];

/**
 * @brief Transmit CAN message with MISRA compliance
 * @param[in] id CAN identifier (11-bit or 29-bit)
 * @param[in] data Pointer to data array
 * @param[in] len Data length (0-8)
 * @return Std_ReturnType Transmission result
 */
Std_ReturnType send_can_message(const uint32_t id,
                                const uint8_t *data,
                                const uint8_t len)
{
    Std_ReturnType result = E_NOT_OK;
    
    /* Rule 11.4, 11.5: Validate pointer */
    if (data == NULL) {
        result = E_NOT_OK;
    }
    /* Rule 10.3, 18.1: Validate length */
    else if (len > MAX_CAN_DLC) {
        result = E_NOT_OK;
    }
    else {
        /* Rule 18.5: Use static buffer instead of dynamic allocation */
        for (uint8_t i = 0U; i < len; i++) {
            can_tx_buffer[i] = data[i];
        }
        
        /* Configure CAN hardware */
        CAN_IDR = id;
        CAN_DLCR = len;
        
        for (uint8_t i = 0U; i < len; i++) {
            CAN_DR[i] = can_tx_buffer[i];
        }
        
        /* Rule 7.4: Use named constant instead of magic number */
        CAN_CR |= CAN_TX_REQUEST_MASK;
        
        /* Wait for transmission complete (with timeout) */
        uint32_t timeout = CAN_TIMEOUT_MS;
        while ((CAN_SR & CAN_TX_COMPLETE_MASK) == 0U) {
            if (timeout == 0U) {
                result = E_TIMEOUT;
                break;
            }
            timeout--;
        }
        
        if (timeout > 0U) {
            result = E_OK;
        }
    }
    
    return result;
}
```

---

## 7. Integrasi dengan Development Workflow

### 7.1 Static Analysis Tools

#### PC-lint Configuration

```
// misra_c2012.lnt - PC-lint configuration for MISRA C:2012
+flib(stdlib.lnt)
+f99              // C99 mode
+uchar
+ulong
+uint
+ushort

// MISRA C:2012 rules
+e(123)           // Rule 1.1
+e(124)           // Rule 1.2
+e(125)           // Rule 1.3
-e(537)           // Allow some flexibility
-w1 -w2 -w3       // Adjust warning levels

// Project-specific settings
-iinclude_path
-idriver_path
```

#### SonarQube Configuration

```xml
<!-- sonar-project.properties -->
sonar.projectKey=automotive_ecu_firmware
sonar.projectName=Automotive ECU Firmware
sonar.sources=src
sonar.c.family=c
sonar.c.misra.enabled=true
sonar.c.misra.version=2012
sonar.c.staticAnalysis.parser=PC-Lint
```

### 7.2 CI/CD Integration

```yaml
# .github/workflows/misra_check.yml
name: MISRA Compliance Check

on: [push, pull_request]

jobs:
  misra-analysis:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install PC-lint
      run: |
        wget https://www.gimpel.com/demo.zip
        unzip demo.zip
        
    - name: Run MISRA Check
      run: |
        flexelint src/*.c -misra_c2012.lnt
        
    - name: Upload Results
      uses: actions/upload-artifact@v2
      with:
        name: misra-report
        path: lint_report.txt
```

### 7.3 Code Review Checklist

```markdown
## MISRA C:2012 Code Review Checklist

### Mandatory Rules (Must Pass)
- [ ] Rule 2.1: Conforms to ISO C99
- [ ] Rule 2.2: Compiles without errors
- [ ] Rule 2.3: No unused code
- [ ] Rule 2.4: No unused variables
- [ ] Rule 5.1-5.9: Identifier uniqueness

### Required Rules (Must Pass or Documented Deviation)
- [ ] Rule 8.1: Explicit type specification
- [ ] Rule 10.1-10.8: Type conversion safety
- [ ] Rule 11.1-11.9: Pointer safety
- [ ] Rule 14.1-14.6: Control flow structure
- [ ] Rule 17.1-17.8: Function parameter safety
- [ ] Rule 18.1-18.8: Array/pointer bounds
- [ ] Rule 21.1-21.13: Standard library usage

### Advisory Rules (Should Follow)
- [ ] Rule 2.9: Functions are small and focused
- [ ] Rule 5.10: Limited function parameters
- [ ] Rule 15.8: Single exit point
- [ ] Rule 17.9: Efficient parameter passing

### Documentation
- [ ] Deviations documented and approved
- [ ] Rationale for exceptions provided
- [ ] Testing strategy for deviated code
```

---

## 8. Checklist Code Review

### 8.1 Quick Reference Card

```
┌──────────────────────────────────────────────────────────────┐
│              MISRA C:2012 Quick Reference                    │
├──────────────────────────────────────────────────────────────┤
│ ALWAYS DO:                                                   │
│ ✓ Use explicit types (Rule 8.1)                             │
│ ✓ Initialize all variables (Rule 9.1)                       │
│ ✓ Use const for read-only data (Rule 8.8)                   │
│ ✓ Add default case to switch (Rule 14.4)                    │
│ ✓ Check array bounds (Rule 18.1-18.4)                       │
│ ✓ Validate pointers before dereference (Rule 11.6)          │
│                                                              │
│ NEVER DO:                                                    │
│ ✗ Use goto statements (Rule 14.6)                           │
│ ✗ Modify loop counter inside loop (Rule 14.1)               │
│ ✗ Use dynamic memory allocation (Rule 21.10)                │
│ ✗ Use unsafe string functions (Rule 21.9)                   │
│ ✗ Compare floating-point for equality (Rule 13.3)           │
│ ✗ Return pointer to local variable (Rule 17.1)              │
│                                                              │
│ BE CAREFUL WITH:                                             │
│ ⚠ Type conversions (Rule 10.x)                              │
│ ⚠ Pointer arithmetic (Rule 11.x, 18.x)                      │
│ ⚠ Bit-field declarations (Rule 6.x)                         │
│ ⚠ Preprocessor macros (Rule 20.x)                           │
└──────────────────────────────────────────────────────────────┘
```

### 8.2 Common Violations and Fixes

| Violation | Rule | Fix Strategy |
|-----------|------|--------------|
| Implicit type conversion | 10.1-10.8 | Add explicit casts |
| Missing initialization | 9.1 | Initialize at declaration |
| Unsafe string copy | 21.9 | Use strncpy or custom safe function |
| Magic numbers | 7.4 | Define named constants |
| Unused variables | 2.4 | Remove or use intentionally |
| Goto statements | 14.6 | Refactor to structured control flow |
| Dynamic allocation | 21.10 | Use static or memory pool |
| No default in switch | 14.4 | Add default case |

---

## 9. Latihan dan Tantangan

### 9.1 Exercise 1: Identify Violations

Berikut kode dengan beberapa pelanggaran MISRA. Identifikasi dan perbaiki:

```c
// Exercise: Find and fix MISRA violations
#include <stdio.h>
#include <string.h>

int counter;

void update_data(ptr, size) {
    char buffer[50];
    
    for (i = 0; i <= size; i++) {
        buffer[i] = ptr[i];
    }
    
    sprintf(buffer, "Data: %s", buffer);
    printf("%s\n", buffer);
    
    if (size = 10) {
        counter++;
    }
}
```

**Solusi:**
```c
#include "safe_string.h"

static uint8_t counter = 0U;

Std_ReturnType update_data(const uint8_t *ptr, const size_t size) {
    Std_ReturnType result = E_OK;
    char buffer[50];
    
    if (ptr == NULL || size >= sizeof(buffer)) {
        result = E_NOT_OK;
    } else {
        for (size_t i = 0U; i < size; i++) {
            buffer[i] = (char)ptr[i];
        }
        buffer[size] = '\0';
        
        Std_ReturnType str_result = safe_snprintf(
            buffer, sizeof(buffer), "Data: %s", buffer
        );
        
        if (str_result == E_OK) {
            custom_printf("%s\r\n", buffer);
        }
        
        if (size == 10U) {
            counter++;
        }
    }
    
    return result;
}
```

### 9.2 Exercise 2: Refactor to MISRA Compliance

Refactor kode berikut agar compliant dengan MISRA C:2012:

```c
// Original code with violations
void *global_ptr;

process(int val) {
    int local = val * 2;
    global_ptr = &local;
    
    switch(val) {
        case 1:
            doSomething();
        case 2:
            doAnotherThing();
            break;
    }
    
    goto end;
    
end:
    return;
}
```

### 9.3 Challenge Project

Implementasikan modul berikut dengan full MISRA compliance:

**Requirements:**
- Circular buffer untuk UART communication
- Thread-safe dengan interrupt protection
- No dynamic memory allocation
- Full error handling
- Unit test coverage > 90%

**Deliverables:**
1. Source code compliant MISRA C:2012
2. Static analysis report (PC-lint/SonarQube)
3. Deviation log (jika ada)
4. Unit test suite
5. Code review checklist

---

## 10. Glosarium Jepang-Inggris

| Japanese | Romaji | English | Indonesian |
|----------|--------|---------|------------|
| 安全基準 | Anzen Kijun | Safety Standard | Standar Keselamatan |
| コーディング規約 | Coding Kiyaku | Coding Convention | Konvensi Pengkodean |
| 必須ルール | Hissu Rule | Mandatory Rule | Aturan Wajib |
| 推奨ルール | Suishou Rule | Recommended Rule | Aturan Anjuran |
| 逸脱 | Itsudatsu | Deviation | Penyimpangan |
| 静的解析 | Seiteki Kaiseki | Static Analysis | Analisis Statis |
| 型変換 | Kata Henkan | Type Conversion | Konversi Tipe |
| ポインタ安全性 | Pointa Anzensei | Pointer Safety | Keamanan Pointer |
| 配列境界 | Hairetsu Kyokai | Array Bounds | Batas Array |
| メモリリーク | Memori Leak | Memory Leak | Kebocoran Memori |
| バッファオーバーフロー | Buffer Overflow | Buffer Overflow | Limpasan Buffer |
| 未定義動作 | Miteigi Dousa | Undefined Behavior | Perilaku Tak Terdefinisi |
| コードレビュー | Code Review | Code Review | Tinjauan Kode |
| 準拠レベル | Junkyo Reberu | Compliance Level | Tingkat Kepatuhan |
| 安全批判的システム | Anzen Hihanteki System | Safety-Critical System | Sistem Kritis Keselamatan |

---

## 📝 Ringkasan

### Poin-Poin Kunci:

1. **MISRA C:2012** adalah standar coding wajib untuk automotive safety-critical systems
2. Terdapat **143 aturan** dalam 3 kategori: Mandatory (21), Required (92), Advisory (30)
3. **Mandatory rules** tidak boleh ada deviasi sama sekali
4. **Required rules** boleh dideviasi dengan justifikasi formal dan approval
5. **Advisory rules** adalah best practices yang sebaiknya diikuti
6. Integrasi dengan **static analysis tools** (PC-lint, SonarQube) sangat penting
7. **Code review** dengan checklist MISRA meningkatkan kualitas kode
8. MISRA C mendukung compliance dengan **ISO 26262** dan **AUTOSAR**

### Next Steps:

1. ✅ Pelajari teori MISRA C:2012 (file ini)
2. 🔄 Install static analysis tool (PC-lint/SonarQube)
3. 🔄 Practice dengan exercise di Section 9
4. 🔄 Apply MISRA ke project existing Anda
5. 🔄 Lakukan code review dengan checklist
6. ➡️ Lanjut ke [06_ISO_26262_Safety_Concepts.md](./06_ISO_26262_Safety_Concepts.md)

---

## 🔗 Referensi

- **MISRA C:2012 Guidelines** - Official document from MISRA Consortium
- **ISO 26262-6:2018** - Product development at the software level
- **AUTOSAR C++14 Guidelines** - Extension untuk C++
- **SEI CERT C Coding Standard** - Complementary security guidelines

---

**Dibuat untuk:** Embedded Automotive Learning Path  
**Versi:** 1.0  
**Last Updated:** 2024  
**Author:** Automotive Software Engineering Team
