# Interrupt Cheat Sheet

## External Interrupts & NVIC Quick Reference

---

## Interrupt Vector Table (STM32F103)

| IRQn | Priority | Handler | Source |
|------|----------|---------|--------|
| -15 | - | NonMaskableInt_IRQn | NMI |
| -14 | - | HardFault_IRQn | Hard Fault |
| -13 | - | MemManage_IRQn | Memory Management |
| -12 | - | BusFault_IRQn | Bus Fault |
| -11 | - | UsageFault_IRQn | Usage Fault |
| -5 | 0-3 | SVCall_IRQn | SVCall |
| -4 | 0-3 | DebugMon_IRQn | Debug Monitor |
| -2 | 0-3 | PendSV_IRQn | PendSV |
| -1 | 0-3 | SysTick_IRQn | SysTick |
| 0 | 0-3 | WWDG_IRQHandler | Window Watchdog |
| 1 | 0-3 | PVD_IRQHandler | PVD through EXTI |
| 2 | 0-3 | TAMPER_IRQHandler | Tamper |
| 3 | 0-3 | RTC_IRQHandler | RTC |
| 4 | 0-3 | FLASH_IRQHandler | Flash |
| 5 | 0-3 | RCC_IRQHandler | RCC |
| 6 | 0-3 | EXTI0_IRQHandler | EXTI Line 0 |
| 7 | 0-3 | EXTI1_IRQHandler | EXTI Line 1 |
| ... | ... | ... | ... |

---

## Interrupt Priority

```
Priority Register (8 bits):
┌──────────────┬─────────────┐
│ Preemption   │ Sub         │
│ Priority     │ Priority    │
│ (4 bits)     │ (4 bits)    │
└──────────────┴─────────────┘
      ↓               ↓
  Determines      If preemption
  which ISR       priority equal,
  runs first      sub-priority decides
```

### Priority Grouping
```c
// Group 4: 4 bits preemption, 0 bits sub-priority
HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

// Set priority (0 = highest, 15 = lowest)
HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);

// Enable interrupt
HAL_NVIC_EnableIRQ(EXTI0_IRQn);
```

---

## External Interrupt Configuration

```c
// Configure GPIO as external interrupt
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;     // Rising edge
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// Alternative modes:
// GPIO_MODE_IT_FALLING   - Falling edge
// GPIO_MODE_IT_RISING_FALLING - Both edges
// GPIO_MODE_EVT_RISING  - Event (no interrupt)
```

---

## Interrupt Callback

```c
// Override this weak function in your code
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        // Handle EXTI0 interrupt
        button_pressed = 1;
    }
}

// Or use the handler directly
void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
    // Custom code here
}
```

---

## Critical Sections

```c
// Disable all interrupts
__disable_irq();

// Critical code here
shared_variable++;

// Enable all interrupts
__enable_irq();

// Or save/restore state
uint32_t primask = __get_PRIMASK();
__disable_irq();
// Critical code
__set_PRIMASK(primask);
```

---

## Common Interrupt Patterns

### Flag-Based Processing
```c
volatile uint8_t data_ready = 0;

void ISR_Handler(void) {
    // Minimal work in ISR
    data_ready = 1;
}

void main_loop(void) {
    if (data_ready) {
        data_ready = 0;
        // Process data in main loop
    }
}
```

### Ring Buffer with Interrupt
```c
#define RX_SIZE 64
volatile uint8_t rx_buffer[RX_SIZE];
volatile uint16_t rx_head = 0;
volatile uint16_t rx_tail = 0;

void UART_RX_ISR(void) {
    uint8_t byte = UART_DR;
    rx_buffer[rx_head] = byte;
    rx_head = (rx_head + 1) % RX_SIZE;
}

uint8_t uart_getchar(void) {
    while (rx_head == rx_tail);  // Wait for data
    uint8_t byte = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_SIZE;
    return byte;
}
```

---

## Interrupt Latency

```
Event occurs
    │
    ▼
┌─────────────┐
│ Hardware    │ ~12 cycles
│ Detection   │
└──────┬──────┘
       ▼
┌─────────────┐
│ Stack Push  │ ~6 cycles
│ (R0-R3,     │
│  R12, LR,   │
│  PC, xPSR)  │
└──────┬──────┘
       ▼
┌─────────────┐
│ Vector Fetch│ ~2 cycles
└──────┬──────┘
       ▼
┌─────────────┐
│ ISR Execute │ Your code
└──────┬──────┘
       ▼
┌─────────────┐
│ Stack Pop   │ ~6 cycles
└──────┬──────┘
       ▼
Resume Main Code

Total: ~26 cycles + ISR execution time
At 72 MHz: ~0.36 µs overhead
```

---

## Best Practices

### DO ✅
- Keep ISRs short and fast
- Use volatile for shared variables
- Clear interrupt flags
- Use priority grouping appropriately
- Document interrupt behavior

### DON'T ❌
- Don't use delay() in ISR
- Don't call blocking functions
- Don't use printf in ISR (slow!)
- Don't forget to clear flags
- Don't share non-volatile variables

---

## Troubleshooting

| Problem | Cause | Solution |
|---------|-------|----------|
| ISR not called | Interrupt not enabled | `HAL_NVIC_EnableIRQ()` |
| Wrong priority | Priority group mismatch | Set priority group first |
| Multiple triggers | Flag not cleared | Clear flag in ISR |
| Missed interrupts | ISR too slow | Move processing to main loop |
| HardFault | Stack overflow | Increase stack size |

---

*For complete documentation, see Cheat-Sheets/stm32-cheatsheet.md*
