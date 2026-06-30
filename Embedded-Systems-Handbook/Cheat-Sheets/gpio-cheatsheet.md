# GPIO Cheat Sheet

## General Purpose Input/Output Quick Reference

---

## Pin Configuration Modes

| Mode | Description | Use Case | Internal Resistor |
|------|-------------|----------|-------------------|
| Input Floating | No pull-up/down | External signal with defined level | None |
| Input Pull-up | Internal pull-up | Button to GND | ~40kΩ to VDD |
| Input Pull-down | Internal pull-down | Button to VDD | ~40kΩ to GND |
| Output Push-Pull | Active high/low | LED, digital output | N/A |
| Output Open-Drain | Pulls low only | I2C, wired-OR | External pull-up |
| Analog | ADC input | Analog sensor | None |
| Alternate Function | Peripheral control | UART, SPI, I2C | Configurable |

---

## Common Operations

```c
// Initialize
GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// Write
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);    // High
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);  // Low

// Toggle
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

// Read
uint8_t state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);

// Lock configuration
HAL_GPIO_LockPin(GPIOA, GPIO_PIN_5);
```

---

## Speed Settings

| Speed | Typical Frequency | Use Case |
|-------|------------------|----------|
| Low | ≤ 2 MHz | Low-speed signals, power saving |
| Medium | ≤ 10 MHz | General purpose |
| High | ≤ 30 MHz | Fast communication |
| Very High | ≤ 50 MHz | High-speed interfaces |

---

*For complete documentation, see Cheat-Sheets/stm32-cheatsheet.md*
