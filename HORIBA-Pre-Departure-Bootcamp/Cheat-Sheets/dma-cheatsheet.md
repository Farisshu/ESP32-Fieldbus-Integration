# DMA Cheat Sheet

## Direct Memory Access Quick Reference

---

## What is DMA?

DMA allows data transfer between memory and peripherals **without CPU intervention**, freeing the processor for other tasks.

```
Without DMA:              With DMA:
┌─────────┐              ┌─────────┐
│   CPU   │              │   CPU   │
│  ●●●●●  │              │  ─────  │ (free to do other work)
└────┬────┘              └─────────┘
     │                        │
     ▼                        ▼
┌─────────┐              ┌─────────┐
│Peripheral│             │   DMA   │◄──►┌─────────┐
└────┬────┘             └────┬────┘    │Memory   │
     │                        │         └─────────┘
     └────────────────────────┘
```

---

## DMA Channels (STM32F103 Example)

| Channel | Requests |
|---------|----------|
| DMA1 Ch1 | ADC1, TIM2_CH3 |
| DMA1 Ch2 | SPI1_RX, I2C1_TX |
| DMA1 Ch3 | SPI1_TX, I2C1_RX, TIM3_CH3 |
| DMA1 Ch4 | SPI2_RX, I2C2_TX, USART1_RX |
| DMA1 Ch5 | SPI2_TX, I2C2_RX, USART1_TX |
| DMA1 Ch6 | USART2_RX, TIM3_CH1 |
| DMA1 Ch7 | USART2_TX, TIM3_CH2 |

---

## Configuration Structure

```c
DMA_HandleTypeDef hdma;

hdma.Instance = DMA1_Channel5;
hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;      // or MEMORY_TO_PERIPH
hdma.Init.PeriphInc = DMA_PINC_DISABLE;          // Peripheral address fixed
hdma.Init.MemInc = DMA_MINC_ENABLE;              // Memory address increments
hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
hdma.Init.Mode = DMA_CIRCULAR;                   // or DMA_NORMAL
hdma.Init.Priority = DMA_PRIORITY_MEDIUM;
HAL_DMA_Init(&hdma);

// Link to peripheral
__HAL_LINKDMA(&huart, hdmarx, hdma);
```

---

## Transfer Directions

| Direction | Use Case |
|-----------|----------|
| PERIPH → MEMORY | UART RX, ADC, SPI RX |
| MEMORY → PERIPH | UART TX, DAC, SPI TX |
| MEMORY → MEMORY | Memory copy (if supported) |

---

## Transfer Modes

### Normal Mode
```
Transfer once, then stop
[Buffer] ──────► [Peripheral]
                  (stops when complete)
```

### Circular Mode
```
Continuous transfer (for streaming)
[Buffer] ◄────► [Peripheral]
  ▲               │
  └───────────────┘
(automatically restarts)
```

---

## Common Use Cases

### ADC Multi-Channel DMA
```c
#define NUM_CHANNELS 4
uint32_t adc_values[NUM_CHANNELS];

hadc1.Init.ScanConvMode = ENABLE;
hadc1.Init.DMAContinuousRequests = ENABLE;
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, NUM_CHANNELS);

// adc_values automatically updated by DMA
```

### UART Receive with DMA
```c
#define RX_SIZE 256
uint8_t rx_buffer[RX_SIZE];

HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_SIZE);

// Check how many bytes received
uint16_t received = RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
```

### UART Transmit with DMA
```c
char message[] = "Hello World";
HAL_UART_Transmit_DMA(&huart1, (uint8_t*)message, sizeof(message));
```

---

## Interrupt Handling

```c
// Enable DMA interrupts
hdma.XferCpltCallback = DMA_TransferComplete;
hdma.XferHalfCpltCallback = DMA_HalfTransfer;
hdma.XferErrorCallback = DMA_Error;

void DMA_TransferComplete(DMA_HandleTypeDef *hdma) {
    if (hdma->Instance == DMA1_Channel5) {
        // Full buffer received
    }
}

void DMA_HalfTransfer(DMA_HandleTypeDef *hdma) {
    // Process first half while second half fills
}
```

---

## Priority Levels

| Priority | Value | Use Case |
|----------|-------|----------|
| Low | 0 | Background transfers |
| Medium | 1 | General peripherals |
| High | 2 | Time-critical data |
| Very High | 3 | ADC, high-speed comms |

---

## Data Alignment

| Size | Peripheral | Memory |
|------|------------|--------|
| Byte | 8-bit | 8-bit |
| Half Word | 16-bit | 16-bit |
| Word | 32-bit | 32-bit |

**Important:** Mismatched alignment causes errors!

---

## Troubleshooting

| Problem | Cause | Solution |
|---------|-------|----------|
| No transfer | Clock not enabled | `__HAL_RCC_DMA1_CLK_ENABLE()` |
| Wrong data | Misaligned access | Check data alignment settings |
| Transfer stops | Normal mode used | Use circular for continuous |
| Data corruption | Buffer overflow | Increase buffer size |
| High priority needed | Multiple DMA channels | Adjust priority levels |

---

*For complete documentation, see Cheat-Sheets/stm32-cheatsheet.md*
