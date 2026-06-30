# Debugging Checklist

## Systematic Approach untuk Embedded Debugging

---

## Pre-Debug Preparation

- [ ] **Reproduce the issue consistently**
  - Can you make it happen every time?
  - What are the exact steps?
  
- [ ] **Document the symptoms**
  - What behavior is expected?
  - What behavior is actually observed?
  - When did it start happening?
  
- [ ] **Check recent changes**
  - What code was modified?
  - Any new hardware connections?
  - Configuration changes?

---

## Hardware Checklist

### Power Supply
- [ ] Measure VDD with multimeter (expected: 3.3V ±5%)
- [ ] Check for voltage drops under load
- [ ] Verify decoupling capacitors are present
- [ ] Check for short circuits (VDD to GND resistance)

### Clock System
- [ ] Verify external crystal is oscillating (oscilloscope)
- [ ] Check clock configuration in code
- [ ] Measure system clock frequency (MCO output)

### Reset Circuit
- [ ] Check NRST pin voltage (should be high during operation)
- [ ] Verify reset capacitor value
- [ ] Check for spurious resets

### Connections
- [ ] All required pins connected (VDD, VSS, NRST, BOOT0)
- [ ] SWD/JTAG connector properly wired
- [ ] No floating inputs on unused pins
- [ ] Pull-up/pull-down resistors where needed

### Peripherals
- [ ] I2C: Check pull-up resistors (4.7kΩ typical)
- [ ] SPI: Verify correct pin mapping (MOSI, MISO, SCK, NSS)
- [ ] UART: Check TX/RX cross-connection, baud rate
- [ ] ADC: Verify reference voltage, input range

---

## Software Checklist

### Build & Flash
- [ ] No compiler warnings (treat warnings as errors)
- [ ] Correct optimization level (-O0 for debug, -Os for release)
- [ ] Debug symbols enabled (-g)
- [ ] Firmware successfully flashed
- [ ] Verify flash content (readback comparison)

### Startup & Initialization
- [ ] Startup file matches MCU variant
- [ ] Linker script correct (memory sizes, sections)
- [ ] Stack size sufficient (minimum 1KB, check usage)
- [ ] Heap size configured (if using malloc)
- [ ] System clock initialized correctly
- [ ] Peripheral clocks enabled before use

### Code Review
- [ ] Check return values of HAL functions
- [ ] Verify buffer sizes (no overflow)
- [ ] Array bounds checking
- [ ] Pointer initialization (no dangling pointers)
- [ ] volatile keyword for shared variables
- [ ] Interrupt priority configuration
- [ ] Watchdog not resetting unexpectedly

### Memory Issues
- [ ] Stack overflow check (paint stack pattern)
- [ ] Heap fragmentation (avoid malloc in embedded)
- [ ] Uninitialized variables (check .bss section)
- [ ] Memory corruption (use MPU if available)

---

## Runtime Debugging

### LED Indicators
```c
// Use LEDs for status indication
#define DBG_LED_ON()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define DBG_LED_OFF()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define DBG_LED_TOGGLE() HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5)

// Pattern examples:
// 1 flash = running
// 2 flashes = error in init
// 3 flashes = error in main loop
// Continuous fast = HardFault
```

### Serial Debug Output
```c
// Minimal printf implementation via UART
#ifdef DEBUG
    #define DBG_PRINTF(fmt, ...) printf("[DBG] " fmt, ##__VA_ARGS__)
#else
    #define DBG_PRINTF(fmt, ...) ((void)0)
#endif

// Usage:
DBG_PRINTF("ADC value: %d\n", adc_value);
DBG_PRINTF("State: %s, Time: %lu ms\n", state_names[current_state], millis());
```

### ITM/SWO Output (if available)
```c
// ARM Cortex-M ITM printf
#define ITM_PORT32 (*(volatile uint32_t *)0xE0000000)

void itm_printf(const char *str) {
    while (*str) {
        while (!(ITM_PORT32 & 1));
        ITM_PORT32 = *str++;
    }
}
```

### Timing Measurement
```c
// Use DWT cycle counter
void timing_start(uint32_t *start) {
    *start = DWT->CYCCNT;
}

float timing_end_ms(uint32_t start) {
    return (float)(DWT->CYCCNT - start) / SystemCoreClock * 1000.0f;
}

// Usage:
uint32_t start;
timing_start(&start);
// ... code to measure ...
float elapsed = timing_end_ms(start);
DBG_PRINTF("Execution time: %.2f ms\n", elapsed);
```

---

## Common Issues & Solutions

### HardFault Debugging

**Symptoms:** Device resets unexpectedly, debugger shows HardFault

**Checklist:**
- [ ] Check stack pointer value (should be in RAM range)
- [ ] Check link register (LR) - which context?
- [ ] Examine fault status registers (CFSR, HFSR, DFSR)
- [ ] Check for unaligned memory access
- [ ] Verify function pointer validity
- [ ] Check array index bounds
- [ ] Look for recursive calls (not allowed in MISRA)

**Debug Code:**
```c
void HardFault_Handler(void) {
    __disable_irq();
    
    // Get stacked context
    uint32_t *sp = (uint32_t *)__get_PSP();
    if ((__get_CONTROL() & 2) == 0) {
        sp = (uint32_t *)__get_MSP();
    }
    
    // Save fault info to persistent memory or output via UART
    uint32_t r0 = sp[0];
    uint32_t r1 = sp[1];
    uint32_t r2 = sp[2];
    uint32_t r3 = sp[3];
    uint32_t pc = sp[6];
    uint32_t lr = sp[7];
    
    // Blink error code
    while (1) {
        DBG_LED_TOGGLE();
        for (volatile int i = 0; i < 1000000; i++);
    }
}
```

### Infinite Loop Detection

**Symptoms:** Device stops responding, watchdog resets

**Checklist:**
- [ ] Add timeout to all waiting loops
- [ ] Check interrupt flags are being cleared
- [ ] Verify peripheral clock is enabled
- [ ] Check for deadlock in ISR + main loop
- [ ] Enable and configure watchdog

**Pattern:**
```c
// ❌ BAD: No timeout
while (flag == 0) {
    // Wait forever if flag never set
}

// ✅ GOOD: With timeout
uint32_t timeout = HAL_GetTick() + 1000; // 1 second
while (flag == 0) {
    if (HAL_GetTick() >= timeout) {
        DBG_PRINTF("Timeout waiting for flag!\n");
        goto error_handler;
    }
}
```

### Communication Issues (UART/SPI/I2C)

**Symptoms:** Garbage data, no response, intermittent failures

**Checklist:**
- [ ] Verify baud rate / clock speed calculation
- [ ] Check pin assignments (alternate function)
- [ ] Confirm logic levels match (3.3V vs 5V)
- [ ] Check for noise/interference (scope waveform)
- [ ] Verify protocol timing (setup/hold times)
- [ ] Add delays between transactions if needed
- [ ] Check slave device address (I2C)
- [ ] Verify chip select handling (SPI)

### ADC Problems

**Symptoms:** Wrong readings, noisy data, crosstalk

**Checklist:**
- [ ] Check sampling time (increase if needed)
- [ ] Verify reference voltage stability
- [ ] Add delay between channel switching
- [ ] Check input impedance of source
- [ ] Add averaging/filtering in software
- [ ] Ensure analog ground is clean
- [ ] Check for digital noise coupling

---

## Debugger Usage (GDB/OpenOCD)

### Essential Commands
```bash
# Connect to target
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

# In GDB:
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue

# Breakpoints
(gdb) break main
(gdb) break filename.c:123
(gdb) break *0x08001234
(gdb) info breakpoints
(gdb) delete 1

# Inspection
(gdb) print variable_name
(gdb) print/x register_name
(gdb) x/10x 0x20000000  # Examine memory
(gdb) backtrace
(gdb) info registers

# Watchpoints
(gdb) watch variable
(gdb) rwatch variable  # Read watch
(gdb) awatch variable  # Access watch

# Step through code
(gdb) step      # Step into
(gdb) next      # Step over
(gdb) finish    # Run until function returns
(gdb) continue  # Continue execution
```

### Useful GDB Scripts
```bash
# Save to .gdbinit

# Print stack trace on HardFault
define hardfault-info
    echo Hard Fault Detected\n
    info registers
    backtrace
end

# Check stack usage
define stack-check
    print (unsigned int)0x20000000
    x/20x 0x20000000
end
```

---

## Logic Analyzer / Oscilloscope Checklist

### Signals to Probe
- [ ] Power supply rails (DC + ripple)
- [ ] Clock signals (frequency, duty cycle, jitter)
- [ ] Reset line (clean edges, proper timing)
- [ ] Communication lines (waveform quality)
- [ ] GPIO toggling (for timing measurement)

### Trigger Setup
- Set trigger on specific pattern or edge
- Use single-shot capture for intermittent issues
- Record long captures to find rare events

---

## Post-Debug Actions

- [ ] **Document the root cause**
  - What was the actual problem?
  - How was it discovered?
  
- [ ] **Implement permanent fix**
  - Not just a workaround
  
- [ ] **Add preventive measures**
  - Additional error checking?
  - Better logging?
  - Unit tests?
  
- [ ] **Update documentation**
  - Add to troubleshooting guide
  - Update design notes
  
- [ ] **Share knowledge**
  - Team discussion
  - Add to wiki/knowledge base

---

## Quick Reference: Error Codes

Create a standard error reporting system:

```c
typedef enum {
    ERR_OK          = 0x00,
    ERR_TIMEOUT     = 0x01,
    ERR_INVALID_PARAM = 0x02,
    ERR_NO_MEMORY   = 0x03,
    ERR_HARDWARE    = 0x04,
    ERR_COMM        = 0x05,
    ERR_CHECKSUM    = 0x06,
    ERR_NOT_READY   = 0x07,
    ERR_UNKNOWN     = 0xFF
} ErrorCode_t;

// Report error with location
#define REPORT_ERROR(err) report_error((err), __FILE__, __LINE__)

void report_error(ErrorCode_t err, const char *file, int line) {
    DBG_PRINTF("ERROR 0x%02X at %s:%d\n", err, file, line);
    // Log to non-volatile memory if critical
}
```

---

## Pro Tips

1. **Divide and Conquer**: Isolate the problem area by systematically eliminating working components

2. **Change One Thing at a Time**: Don't modify multiple variables simultaneously

3. **Use Version Control**: Commit before making changes, easy to revert

4. **Keep a Debug Journal**: Note what you tried and results

5. **Take Breaks**: Fresh eyes often spot obvious mistakes

6. **Rubber Duck Debugging**: Explain the problem out loud

7. **Sleep On It**: Complex bugs often become clear after rest

---

*Print this checklist and keep it at your desk!*

*Last Updated: [Tanggal]*  
*Related: Day-07-Debugging-and-Simulation.md*
