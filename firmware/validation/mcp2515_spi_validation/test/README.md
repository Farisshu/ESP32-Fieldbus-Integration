# Test Directory

Unit tests for the CAN SPI Test project.

## Purpose

This directory contains unit tests using the [PlatformIO Test Runner](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html) and the Unity testing framework.

## Running Tests

```bash
# Run all tests
pio test

# Run with verbose output
pio test -v
```

## Writing Tests

Example test file structure:

```cpp
#include <unity.h>
#include "../src/main.cpp"

void setUp(void) {
    // Setup before each test
}

void tearDown(void) {
    // Cleanup after each test
}

void test_spi_pins_initialized(void) {
    TEST_ASSERT_TRUE(spi_pins_ready());
}

void main() {
    UNITY_BEGIN();
    RUN_TEST(test_spi_pins_initialized);
    UNITY_END();
}
```

## Best Practices

- Name test files with `test_` prefix
- Keep tests focused and independent
- Use descriptive test names
- Test both success and error cases

---

*Part of the ESP32 Logic Analyzer Automation project*
