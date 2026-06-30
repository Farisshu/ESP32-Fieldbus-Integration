# UART Cheat Sheet

## Universal Asynchronous Receiver-Transmitter Quick Reference

---

## Baud Rate Calculation

```
Baud Rate = f_CLK / (8 × (2 - OVER8) × USARTDIV)

For oversampling by 16 (OVER8 = 0):
USARTDIV = f_CLK / (16 × BaudRate)
```

### Common Baud Rates (f_CLK = 16 MHz)

| Baud Rate | USARTDIV | Error % |
|-----------|----------|---------|
| 9600 | 104.17 | 0.16% |
| 19200 | 52.08 | 0.16% |
| 38400 | 26.04 | 0.16% |
| 57600 | 17.36 | 0.16% |
| 115200 | 8.68 | 0.16% |
| 230400 | 4.34 | 0.16% |
| 460800 | 2.17 | 0.16% |
| 921600 | 1.08 | 0.16% |

---

## Frame Format

```
Idle: ─────┐
           │ ┌───┬───┬───┬───┬───┬───┬───┬───┬───┐
Start:     └─┤ S │ D0│ D1│ D2│ D3│ D4│ D5│ D6│ D7│ ...
                └───┴───┴───┴───┴───┴───┴───┴───┘
                  │                               │
               LSB                             MSB
               
               ┌───┬──────┬──────┬──────────────┐
               │ S │ Data │ Parity│   Stop      │
               └───┴──────┴──────┴──────────────┘
                 1    8      0/1       1/2
```

---

## Configuration Structure

```c
UART_HandleTypeDef huart;

huart.Instance = USART1;
huart.Init.BaudRate = 115200;
huart.Init.WordLength = UART_WORDLENGTH_8B;      // 8 or 9 bits
huart.Init.StopBits = UART_STOPBITS_1;           // 1 or 2
huart.Init.Parity = UART_PARITY_NONE;            // None, Even, Odd
huart.Init.Mode = UART_MODE_TX_RX;               // TX, RX, or both
huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;      // None, RTS, CTS, Both
huart.Init.OverSampling = UART_OVERSAMPLING_16;  // 8 or 16
HAL_UART_Init(&huart);
```

---

## Transfer Methods

### Polling (Blocking)
```c
// Transmit
uint8_t tx_data[] = "Hello";
HAL_UART_Transmit(&huart, tx_data, sizeof(tx_data), 100);

// Receive
uint8_t rx_data;
HAL_UART_Receive(&huart, &rx_data, 1, 100);
```

### Interrupt (Non-blocking)
```c
// Start receive interrupt
HAL_UART_Receive_IT(&huart, &rx_byte, 1);

// Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    process_byte(rx_byte);
    HAL_UART_Receive_IT(&huart, &rx_byte, 1);  // Re-enable
}
```

### DMA (Most Efficient)
```c
// Circular buffer for continuous receive
#define RX_SIZE 256
uint8_t rx_buffer[RX_SIZE];

HAL_UART_Receive_DMA(&huart, rx_buffer, RX_SIZE);

// Check available data
uint16_t available = RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart_rx);
```

---

## Hardware Connection

```
MCU UART        USB-UART Adapter
┌─────────┐     ┌─────────────┐
│         │     │             │
│   TX  ──┼─────┼── RX        │
│   RX  ──┼─────┼── TX        │
│   GND ──┼─────┼── GND       │
│         │     │             │
└─────────┘     └─────────────┘

Note: Cross TX and RX!
```

---

## Common Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| Garbage data | Wrong baud rate | Recalculate with correct clock |
| No response | TX/RX not crossed | Swap TX and RX connections |
| Framing errors | Wrong stop bits | Match transmitter settings |
| Overrun errors | RX not read fast enough | Use interrupt or DMA |
| Missing bytes | Buffer too small | Increase buffer size |

---

*For complete documentation, see Cheat-Sheets/stm32-cheatsheet.md*
