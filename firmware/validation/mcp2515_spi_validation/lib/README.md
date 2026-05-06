# Lib Directory

Private libraries for the CAN SPI Test project.

## Purpose

This directory is for project-specific libraries that are:
- Used only by this project
- Not shared with other projects
- Custom implementations for specific needs

## Structure

Place each library in its own subdirectory:

```
lib/
├── MyLibrary/
│   ├── MyLibrary.h
│   └── MyLibrary.cpp
└── README.md
```

## Usage

Libraries in this directory are automatically compiled and linked by PlatformIO.

---

*Part of the ESP32 Logic Analyzer Automation project*
