# Lib Directory 📚

## Purpose

This directory is intended for project-specific (private) libraries. PlatformIO will automatically compile them into static libraries and link them into the executable file.

## Structure

Each library should be placed in its own subdirectory:

```
lib/
├── MyLibrary/
│   ├── src/
│   │   ├── MyLibrary.cpp
│   │   └── MyLibrary.h
│   ├── examples/
│   │   └── basic_usage/
│   ├── docs/
│   │   └── README.md
│   ├── library.json
│   └── README.md
├── AnotherLibrary/
│   ├── AnotherLibrary.cpp
│   └── AnotherLibrary.h
└── README.md       # This file
```

## Creating a New Library

1. **Create directory structure**:
   ```bash
   mkdir -p lib/MyLibrary/src
   ```

2. **Add source files** in `src/`:
   ```cpp
   // MyLibrary.h
   #ifndef MYLIBRARY_H
   #define MYLIBRARY_H
   
   class MyLibrary {
   public:
       void begin();
       int read();
   };
   
   #endif
   ```

3. **(Optional) Add library.json** for metadata:
   ```json
   {
     "name": "MyLibrary",
     "version": "1.0.0",
     "description": "A custom library for my project",
     "authors": [
       {
         "name": "Your Name",
         "email": "your.email@example.com"
       }
     ],
     "frameworks": "arduino",
     "platforms": "espressif32"
   }
   ```

## Usage in Source Files

```cpp
#include <MyLibrary.h>

MyLibrary myLib;

void setup() {
    myLib.begin();
}

void loop() {
    int value = myLib.read();
}
```

## PlatformIO Features

- **Auto-detection**: Libraries are automatically discovered
- **Dependency resolution**: Uses [LDF](https://docs.platformio.org/en/latest/librarymanager/ldf.html)
- **Version control**: Can use Git repositories as dependencies

## Best Practices

1. **Modular Design**: Keep libraries focused on single responsibilities
2. **Documentation**: Include README and usage examples
3. **Testing**: Add unit tests in the parent `tests/` directory
4. **Versioning**: Use semantic versioning for library releases

## Resources

- [PlatformIO Library Manager](https://docs.platformio.org/en/latest/librarymanager/index.html)
- [Library Dependency Finder (LDF)](https://docs.platformio.org/en/latest/librarymanager/ldf.html)
- [library.json Format](https://docs.platformio.org/en/latest/librarymanager/config.html)

---

*For more information, see the [PlatformIO documentation](https://docs.platformio.org/en/latest/librarymanager/index.html)*

---

## 🇯🇵 日本語メモ (にほんご めも - Japanese Notes)

このドキュメントは技術学習とインターンシップ準備のために作成されました。
(This document was created for technical learning and internship preparation.)

**重要な用語 (Important Terms):**
- ファームウェア (Firmware) - 組み込みソフトウェア
- デバッグ (Debug) - 修正・検証作業
- テスト (Test) - 試験・検証
- 資料 (Shiryou) - Documentation

