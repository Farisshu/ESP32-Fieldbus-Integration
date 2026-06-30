# PlatformIO Cheat Sheet

## Quick Reference untuk PlatformIO Development

---

## 1. Project Structure

```
project/
├── src/
│   └── main.cpp          # Application code
├── include/
│   └── my_header.h       # Public headers
├── lib/
│   └── MyLibrary/        # Private libraries
│       └── MyLibrary.cpp
├── test/
│   └── test_main.cpp     # Unit tests
├── data/                 # Filesystem images
├── boards/               # Custom board definitions
├── platformio.ini        # Configuration file
└── .vscode/              # VS Code settings (auto-generated)
```

---

## 2. platformio.ini Configuration

### Basic Setup (STM32)
```ini
[env:nucleo_f401re]
platform = ststm32
board = nucleo_f401re
framework = stm32cube

# Upload settings
upload_protocol = stlink
upload_speed = 921600

# Build flags
build_flags = 
    -D DEBUG
    -D USE_HAL_DRIVER
    -D STM32F401xE
    -O2
    
# Serial monitor
monitor_speed = 115200
monitor_port = /dev/ttyACM0
```

### Multiple Environments
```ini
[platformio]
default_envs = nucleo_f401re

[env:nucleo_f401re]
platform = ststm32
board = nucleo_f401re
framework = stm32cube
build_type = debug

[env:nucleo_f401re_release]
extends = env:nucleo_f401re
build_type = release
build_flags = 
    -D NDEBUG
    -Os

[env:blackpill_f103c8]
platform = ststm32
board = bluepill_f103c8
framework = stm32cube
upload_protocol = stlink
```

### Common Build Flags
```ini
build_flags = 
    # Optimization
    -O2                     # Optimize for speed
    -Os                     # Optimize for size
    
    # Warnings
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    
    # Defines
    -D DEBUG
    -D USE_FULL_ASSERT
    -D F_CPU=72000000L
    
    # Include paths
    -I include
    -I lib/MyLibrary
    
    # Linker flags
    -Wl,-u,vfprintf
    -lprintf_fp
    -lm
```

### Library Dependencies
```ini
lib_deps = 
    # From PlatformIO Registry
    arduino-libraries/Arduino_JSON@^0.2.0
    bblanchon/ArduinoJson@^6.21.0
    
    # GitHub repository
    https://github.com/user/repo.git
    
    # Specific branch/tag
    https://github.com/user/repo.git#v1.2.3
    
    # Local library
    lib/MyLocalLibrary
```

---

## 3. CLI Commands

### Project Management
```bash
# Create new project
pio init --board nucleo_f401re

# Create project with name
pio project init --board nucleo_f401re --name "MyProject"

# Clean build files
pio run --target clean

# Build project
pio run

# Build specific environment
pio run -e nucleo_f401re

# Build and upload
pio run --target upload

# Upload firmware only
pio run --target upload

# Monitor serial
pio device monitor

# Monitor with specific port
pio device monitor --port /dev/ttyACM0

# List connected devices
pio device list
```

### Debugging
```bash
# Start debugging session
pio debug

# Debug specific environment
pio debug -e nucleo_f401re

# Run unit tests
pio test

# Test with verbosity
pio test -v
```

### Library Management
```bash
# Search libraries
pio lib search "json"

# Install library
pio lib install "ArduinoJson"

# Install specific version
pio lib install "ArduinoJson@6.21.0"

# List installed libraries
pio lib list

# Update libraries
pio lib update

# Remove library
pio lib uninstall "LibraryName"

# Check outdated libraries
pio lib outdated
```

### Platform Management
```bash
# List installed platforms
pio platform list

# Update platforms
pio platform update

# Show platform info
pio platform show ststm32

# Install specific platform version
pio platform install ststm32@15.0.0
```

### Other Useful Commands
```bash
# Show project info
pio project info

# Run custom target
pio run --target program

# Build filesystem image
pio run --target buildfs

# Upload filesystem image
pio run --target uploadfs

# Show build flags
pio run --target envdump

# Remote development
pio remote agent start
pio remote run --target upload
```

---

## 4. Debug Configuration (.vscode/launch.json)

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug",
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "openocd",
            "cwd": "${workspaceRoot}",
            "executable": ".pio/build/nucleo_f401re/firmware.elf",
            "svdFile": "${env:PIO_HOME_DIR}/packages/tool-openocd/scripts/stm32f4.svd",
            "configFiles": [
                "interface/stlink.cfg",
                "target/stm32f4x.cfg"
            ],
            "preLaunchTask": "PlatformIO: Build",
            "postDebugTask": "PlatformIO: Clean"
        }
    ]
}
```

---

## 5. Tasks Configuration (.vscode/tasks.json)

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "PlatformIO: Build",
            "type": "shell",
            "command": "pio run",
            "group": "build",
            "problemMatcher": []
        },
        {
            "label": "PlatformIO: Upload",
            "type": "shell",
            "command": "pio run --target upload",
            "group": "build",
            "problemMatcher": []
        },
        {
            "label": "PlatformIO: Monitor",
            "type": "shell",
            "command": "pio device monitor",
            "group": "test",
            "problemMatcher": [],
            "isBackground": true
        },
        {
            "label": "PlatformIO: Clean",
            "type": "shell",
            "command": "pio run --target clean",
            "group": "build",
            "problemMatcher": []
        },
        {
            "label": "PlatformIO: Test",
            "type": "shell",
            "command": "pio test",
            "group": "test",
            "problemMatcher": []
        }
    ]
}
```

---

## 6. Common Issues & Solutions

### Issue: Upload fails
```bash
# Solution: Check permissions (Linux)
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
# Logout and login again

# Or add udev rules
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="0483", MODE="0666"' | \
  sudo tee /etc/udev/rules.d/50-stlink.rules
sudo udevadm control --reload-rules
```

### Issue: Missing dependencies
```bash
# Update all platforms and libraries
pio upgrade
pio platform update
pio lib update
```

### Issue: Build errors after changes
```bash
# Full clean rebuild
pio run --target clean
pio run
```

### Issue: Serial monitor not working
```bash
# Check port
pio device list

# Specify port explicitly
pio device monitor --port /dev/ttyACM0

# On Windows, use COM port
pio device monitor --port COM3
```

---

## 7. Advanced Configuration

### Custom Linker Script
```ini
board_build.ldscript = custom_linker_script.ld
```

### Custom Flash Size
```ini
board_upload.maximum_size = 131072  ; 128 KB
board_upload.maximum_ram_size = 20480  ; 20 KB
```

### Pre/Post Build Scripts
```ini
extra_scripts = 
    pre:scripts/pre_build.py
    post:scripts/post_build.py
```

### Custom Board Definition
```ini
; boards/custom_board.json
{
    "build": {
        "core": "stm32",
        "cpu": "cortex-m4",
        "extra_flags": "-DSTM32F401xE",
        "f_cpu": "84000000L",
        "mcu": "stm32f401ret6"
    },
    "debug": {
        "tools": {
            "stlink": {}
        }
    },
    "frameworks": ["stm32cube"],
    "name": "Custom Board",
    "upload": {
        "maximum_ram_size": 98304,
        "maximum_size": 524288,
        "protocol": "stlink"
    },
    "url": "https://example.com",
    "vendor": "Custom"
}
```

---

## 8. Unit Testing Example

### test/test_blink.cpp
```cpp
#include <unity.h>
#include "blink.h"

void setUp(void) {
    // Setup before each test
}

void tearDown(void) {
    // Teardown after each test
}

void test_led_toggle(void) {
    led_init();
    led_off();
    TEST_ASSERT_EQUAL(LOW, led_get_state());
    
    led_toggle();
    TEST_ASSERT_EQUAL(HIGH, led_get_state());
}

void test_led_blink_rate(void) {
    // Test timing logic
    uint32_t start = millis();
    led_blink(100);
    uint32_t elapsed = millis() - start;
    TEST_ASSERT_UINT_WITHIN(10, 100, elapsed);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_led_toggle);
    RUN_TEST(test_led_blink_rate);
    return UNITY_END();
}
```

### Run Tests
```bash
pio test
pio test -e native  # Run on PC
pio test -v         # Verbose output
```

---

## 9. CI/CD Integration

### GitHub Actions Example
```yaml
name: PlatformIO CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Set up Python
        uses: actions/setup-python@v4
        with:
          python-version: '3.9'
      
      - name: Install PlatformIO
        run: |
          pip install platformio
          pio upgrade
      
      - name: Build Firmware
        run: pio run
      
      - name: Run Tests
        run: pio test
      
      - name: Upload Artifact
        uses: actions/upload-artifact@v3
        with:
          name: firmware
          path: .pio/build/*/firmware.bin
```

---

## 10. Useful Extensions (VS Code)

- **PlatformIO IDE** - Official extension
- **C/C++** - IntelliSense and debugging
- **Cortex-Debug** - Enhanced ARM debugging
- **GitLens** - Git integration
- **Serial Monitor** - Alternative serial monitor
- **Todo Tree** - Highlight TODO comments

---

## Quick Command Reference

| Task | Command |
|------|---------|
| Build | `pio run` |
| Upload | `pio run --target upload` |
| Monitor | `pio device monitor` |
| Clean | `pio run --target clean` |
| Debug | `pio debug` |
| Test | `pio test` |
| Lib install | `pio lib install <name>` |
| Lib update | `pio lib update` |
| Device list | `pio device list` |

---

**Pro Tip:** Use `pio system prune` to clean up cached data and free disk space.

---

*Last Updated: [Tanggal]*  
*Official Docs: https://docs.platformio.org/*
