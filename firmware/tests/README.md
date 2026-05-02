# Firmware Tests 🧪

This directory contains unit tests for individual firmware modules using the [PlatformIO Test Runner](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html).

## Purpose

Unit testing verifies that individual units of source code (functions, modules, drivers) work correctly in isolation. This helps:

- Catch bugs early in development
- Ensure code changes don't break existing functionality
- Document expected behavior of modules
- Validate hardware abstraction layers

## Structure

```
tests/
├── src/           # Test source files
├── lib/           # Test-specific libraries/mocks
└── README.md      # This file
```

## Running Tests

```bash
# Run all tests
pio test

# Run specific test environment
pio test -e native

# Run with verbose output
pio test -v

# Run specific test file
pio test -f test_spi.cpp
```

## Writing Tests

Example test structure:

```cpp
#include <unity.h>
#include <your_module.h>

void setUp(void) {
    // Initialize before each test
}

void tearDown(void) {
    // Clean up after each test
}

void test_example_function(void) {
    TEST_ASSERT_EQUAL(42, your_function());
}

void main() {
    UNITY_BEGIN();
    RUN_TEST(test_example_function);
    UNITY_END();
}
```

## Test Categories

| Category | Description | Example |
|----------|-------------|---------|
| **Driver Tests** | Hardware driver validation | SPI, UART, I2C drivers |
| **Protocol Tests** | Communication protocol verification | CAN, Modbus |
| **Utility Tests** | Helper function validation | CRC, timing functions |

## Best Practices

1. **Isolation**: Each test should be independent
2. **Naming**: Use descriptive test names (`test_mcp2515_init_success`)
3. **Assertions**: Use appropriate Unity assertions
4. **Setup/Teardown**: Initialize and clean up resources properly
5. **Coverage**: Test both success and error cases

## Dependencies

- [Unity](https://github.com/ThrowTheSwitch/Unity) - C testing framework
- PlatformIO Unity integration

---

*For more information, see the [PlatformIO Unit Testing documentation](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)*
