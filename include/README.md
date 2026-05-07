# Include Directory 📁

## Purpose

This directory is intended for project header files (.h) that contain C/C++ declarations and macro definitions shared between multiple source files.

## What Goes Here

- **Header Files**: `.h` files with function declarations, type definitions, and macros
- **Configuration Headers**: Pin definitions, compile-time options
- **Common Utilities**: Shared helper functions and constants

## Example Structure

```
include/
├── mcp2515.h       # MCP2515 register definitions
├── can_config.h    # CAN bus configuration
├── pin_defs.h      # GPIO pin assignments
└── README.md       # This file
```

## Usage in Source Files

```cpp
// In your src/main.cpp or other source files:
#include "pin_defs.h"
#include "mcp2515.h"

void setup() {
    pinMode(CAN_CS_PIN, OUTPUT);
    // ...
}
```

## Best Practices

1. **Include Guards**: Always use include guards to prevent multiple inclusion:
   ```cpp
   #ifndef MY_HEADER_H
   #define MY_HEADER_H
   
   // Your code here
   
   #endif // MY_HEADER_H
   ```

2. **Clear Naming**: Use descriptive filenames that reflect their purpose

3. **Documentation**: Comment your declarations for better maintainability

4. **Minimal Dependencies**: Keep headers self-contained when possible

---

*For more information about header files, see the [GCC documentation](https://gcc.gnu.org/onlinedocs/cpp/Header-Files.html)*

---

## 🇯🇵 日本語メモ (Japanese Notes)

> 📚 **Refer to main Japanese learning resources:**
> - [日本語技術学習ガイド](../materi/JAPANESE_LEARNING_GUIDE.md) - Japanese Technical Learning Guide
> - [Kamus Teknis Jepang](../materi/KAMUS_TEKNIS_JEPANG.md) - Japanese Technical Dictionary (Indonesian)
>
> **用語集 (Glossary):**
> | 日本語 | Romaji | English |
> |--------|--------|---------|
> | ヘッダーファイル | Hedda Fairu | Header File |
> | 宣言 | Sengen | Declaration |
> | マクロ | Makuro | Macro |
> | 資料 | Shiryō | Documentation |

