# STM32 Cheat Sheet

## Quick Reference untuk STM32 Developer

---

## 1. Clock System Overview

```
┌──────────────┐
│   HSE/HSI    │  ← External/Internal High Speed (8-25 MHz / 16 MHz)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│     PLL      │  ← Phase Locked Loop (multiply clock)
└──────┬───────┘
       │
       ▼
┌──────────────┐
│   SYSCLK     │  ← System Clock (max 72/168/216 MHz tergantung seri)
└──────┬───────┘
       │
    ┌──┴──┬──────────┬──────────┐
    ▼     ▼          ▼          ▼
┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐
│ AHB  │ │ APB1 │ │ APB2 │ │ MCO  │
│      │ │(Slow)│ │(Fast)│ │      │
└──┬───┘ └──┬───┘ └──┬───┘ └──────┘
   │        │        │
   ▼        ▼        ▼
 Core     UART     GPIO
 DMA      I2C      ADC
          SPI      TIM
```

### Common Clock Configurations
```c
// STM32F103 (Blue Pill)
// HSE = 8 MHz, PLL x9 = 72 MHz SYSCLK

// STM32F407
// HSE = 8 MHz, PLL: VCO = 8 * 100 / 8 = 100 MHz
// SYSCLK = 100 MHz, AHB = 100 MHz, APB1 = 25 MHz, APB2 = 50 MHz
```

---

## 2. GPIO Quick Reference

### GPIO Modes
| Mode | Input | Output | Speed | Pull-up/down | Example |
|------|-------|--------|-------|--------------|---------|
| `GPIO_MODE_INPUT` | ✅ | ❌ | N/A | Yes | Button read |
| `GPIO_MODE_OUTPUT_PP` | ❌ | ✅ | Yes | No | LED control |
| `GPIO_MODE_OUTPUT_OD` | ❌ | ✅ | Yes | No | I2C SDA/SCL |
| `GPIO_MODE_AF_PP` | ❌ | ✅ | Yes | No | UART TX, SPI MOSI |
| `GPIO_MODE_AF_OD` | ❌ | ✅ | Yes | No | I2C pins |
| `GPIO_MODE_ANALOG` | ✅ | ❌ | N/A | No | ADC input |
| `GPIO_MODE_IT_RISING` | ✅ | ❌ | N/A | Yes | External interrupt |

### Pin Configuration Structure
```c
GPIO_InitTypeDef GPIO_InitStruct = {0};

GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```

### Common GPIO Operations
```c
// Set pin high
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

// Set pin low
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

// Toggle pin
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

// Read pin
uint8_t state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);

// Lock configuration
HAL_GPIO_LockPin(GPIOA, GPIO_PIN_5);
```

### Pin Mapping (STM32F103 Example)
```
PA0  - WKUP, USART2_CTS, ADC12_IN0
PA1  - USART2_RTS, ADC12_IN1
PA2  - USART2_TX, ADC12_IN2
PA3  - USART2_RX, ADC12_IN3
PA4  - SPI1_NSS, DAC_OUT1, ADC12_IN4
PA5  - SPI1_SCK, DAC_OUT2, ADC12_IN5
PA6  - SPI1_MISO, ADC12_IN6
PA7  - SPI1_MOSI, ADC12_IN7
PA8  - MCO, USART1_CK
PA9  - USART1_TX
PA10 - USART1_RX
PA11 - USBDM
PA12 - USBDP
PA13 - JTMS-SWDIO
PA14 - JTCK-SWCLK
PA15 - JTDI, SPI3_NSS

PB0  - ADC12_IN8
PB1  - ADC12_IN9
PB3  - JTDO, SPI3_SCK
PB4  - NJTRST, SPI3_MISO
PB5  - I2C1_SMBA, SPI3_MOSI
PB6  - I2C1_SCL, USART1_TX
PB7  - I2C1_SDA, USART1_RX
PB8  - I2C1_SCL, CAN_RX
PB9  - I2C1_SDA, CAN_TX, SPI2_NSS
PB10 - I2C2_SCL, USART3_TX
PB11 - I2C2_SDA, USART3_RX
PB12 - I2C2_SMBA, SPI2_NSS
PB13 - SPI2_SCK, USART3_CTS
PB14 - SPI2_MISO, USART3_RTS
PB15 - SPI2_MOSI
```

---

## 3. UART Configuration

### Basic UART Setup
```c
UART_HandleTypeDef huart1;

huart1.Instance = USART1;
huart1.Init.BaudRate = 115200;
huart1.Init.WordLength = UART_WORDLENGTH_8B;
huart1.Init.StopBits = UART_STOPBITS_1;
huart1.Init.Parity = UART_PARITY_NONE;
huart1.Init.Mode = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
huart1.Init.OverSampling = UART_OVERSAMPLING_16;
HAL_UART_Init(&huart1);
```

### UART Interrupt RX
```c
// Enable interrupt
HAL_UART_Receive_IT(&huart1, &rx_byte, 1);

// Callback (override weak function)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // Process received byte
        process_byte(rx_byte);
        
        // Re-enable interrupt
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}
```

### UART DMA RX (Circular Buffer)
```c
#define RX_BUFFER_SIZE 256
uint8_t rx_buffer[RX_BUFFER_SIZE];

// Start DMA receive
HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_BUFFER_SIZE);

// Check for new data
uint16_t get_available_bytes(void) {
    return RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
}
```

### Baud Rate Calculation
```
Baud Rate = f_CLK / (8 * (2 - OVER8) * (USARTDIV))

For OVER8 = 0 (oversampling by 16):
USARTDIV = f_CLK / (16 * BaudRate)

Example: f_CLK = 16 MHz, BaudRate = 115200
USARTDIV = 16000000 / (16 * 115200) = 8.68 ≈ 9
Actual BaudRate = 16000000 / (16 * 9) = 111111 (error: 3.5%)
```

---

## 4. SPI Configuration

### Basic SPI Setup (Master)
```c
SPI_HandleTypeDef hspi1;

hspi1.Instance = SPI1;
hspi1.Init.Mode = SPI_MODE_MASTER;
hspi1.Init.Direction = SPI_DIRECTION_2LINES;
hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi1.Init.NSS = SPI_NSS_SOFT;
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
HAL_SPI_Init(&hspi1);
```

### SPI Transfer
```c
// Polling
uint8_t tx_data = 0x55;
uint8_t rx_data;
HAL_SPI_TransmitReceive(&hspi1, &tx_data, &rx_data, 1, 100);

// DMA
HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buffer, rx_buffer, length);
```

### SPI Mode Table
| Mode | CPOL | CPHA | Data Captured On |
|------|------|------|------------------|
| 0 | 0 | 0 | Rising edge |
| 1 | 0 | 1 | Falling edge |
| 2 | 1 | 0 | Falling edge |
| 3 | 1 | 1 | Rising edge |

---

## 5. I2C Configuration

### Basic I2C Setup
```c
I2C_HandleTypeDef hi2c1;

hi2c1.Instance = I2C1;
hi2c1.Init.Timing = 0x00702681; // Use CubeMX to calculate
hi2c1.Init.OwnAddress1 = 0x00;
hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
hi2c1.Init.OwnAddress2 = 0x00;
hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
HAL_I2C_Init(&hi2c1);
```

### I2C Read/Write
```c
// Write to device
uint8_t reg_addr = 0x00;
uint8_t data = 0x55;
HAL_I2C_Master_Transmit(&hi2c1, (0x68 << 1), &reg_addr, 1, 100);
HAL_I2C_Master_Transmit(&hi2c1, (0x68 << 1), &data, 1, 100);

// Read from device
uint8_t rx_data;
HAL_I2C_Master_Transmit(&hi2c1, (0x68 << 1), &reg_addr, 1, 100);
HAL_I2C_Master_Receive(&hi2c1, (0x68 << 1), &rx_data, 1, 100);

// Combined write-read (most common)
HAL_I2C_Mem_Read(&hi2c1, (0x68 << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, 
                 &rx_data, 1, 100);
```

### I2C Timing Calculator
Gunakan tool online atau STM32CubeMX untuk menghitung timing register berdasarkan:
- I2C clock speed (100kHz, 400kHz, 1MHz)
- System clock frequency
- Rise/Fall time

---

## 6. ADC Configuration

### Single Channel Polling
```c
ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig = {0};

hadc1.Instance = ADC1;
hadc1.Init.ScanConvMode = DISABLE;
hadc1.Init.ContinuousConvMode = DISABLE;
hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
hadc1.Init.NbrOfConversion = 1;
HAL_ADC_Init(&hadc1);

sConfig.Channel = ADC_CHANNEL_0;
sConfig.Rank = 1;
sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
HAL_ADC_ConfigChannel(&hadc1, &sConfig);

// Read value
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 100);
uint16_t adc_value = HAL_ADC_GetValue(&hadc1);
HAL_ADC_Stop(&hadc1);

// Convert to voltage (3.3V reference, 12-bit)
float voltage = (adc_value / 4095.0f) * 3.3f;
```

### Multi-Channel DMA
```c
#define NUM_CHANNELS 4
uint32_t adc_values[NUM_CHANNELS];

// Configure scan mode
hadc1.Init.ScanConvMode = ENABLE;
hadc1.Init.ContinuousConvMode = ENABLE;
hadc1.Init.NbrOfConversion = NUM_CHANNELS;
HAL_ADC_Init(&hadc1);

// Configure channels
for (int i = 0; i < NUM_CHANNELS; i++) {
    sConfig.Channel = ADC_CHANNEL_0 + i;
    sConfig.Rank = i + 1;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

// Start DMA
HAL_ADC_Start_DMA(&hadc1, adc_values, NUM_CHANNELS);
```

### ADC Resolution Table
| Resolution | Bits | Max Value | LSB (3.3V ref) |
|------------|------|-----------|----------------|
| 6-bit | 6 | 63 | 51.6 mV |
| 8-bit | 8 | 255 | 12.9 mV |
| 10-bit | 10 | 1023 | 3.2 mV |
| 12-bit | 12 | 4095 | 0.8 mV |

---

## 7. Timer & PWM

### Basic Timer (Timebase)
```c
TIM_HandleTypeDef htim2;

htim2.Instance = TIM2;
htim2.Init.Period = 999;           // ARR
htim2.Init.Prescaler = 7199;       // PSC
htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
HAL_TIM_Base_Init(&htim2);

// Start timer with interrupt
HAL_TIM_Base_Start_IT(&htim2);

// Callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Timer interrupt handler (1ms jika 72MHz clock)
    }
}
```

### PWM Configuration
```c
TIM_OC_InitTypeDef sConfigOC = {0};

htim3.Instance = TIM3;
htim3.Init.Period = 999;           // Frequency = 72MHz / (999+1) = 72 kHz
htim3.Init.Prescaler = 0;
HAL_TIM_PWM_Init(&htim3);

sConfigOC.OCMode = TIM_OCMODE_PWM1;
sConfigOC.Pulse = 500;             // 50% duty cycle
sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

// Start PWM
HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

// Change duty cycle
__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 250); // 25%
```

### PWM Frequency Calculation
```
PWM Frequency = Timer Clock / ((ARR + 1) * (PSC + 1))

Example: Timer Clock = 72 MHz, ARR = 999, PSC = 0
PWM Frequency = 72,000,000 / (1000 * 1) = 72 kHz

Duty Cycle = CCRx / (ARR + 1)
50% duty: CCRx = 500
25% duty: CCRx = 250
```

---

## 8. Interrupt (NVIC)

### External Interrupt (EXTI)
```c
// Configure GPIO as interrupt
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// Enable and set EXTI interrupt
HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
HAL_NVIC_EnableIRQ(EXTI0_IRQn);

// Callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        // Handle interrupt
    }
}
```

### NVIC Priority (STM32F1)
```
Priority Group: 4 bits preemption priority

Priority Level | Preemption | Sub-Priority
---------------|------------|-------------
0 (Highest)    | 0          | 0
1              | 0          | 1
...            | ...        | ...
15 (Lowest)    | 15         | 15
```

### Critical Section
```c
// Disable all interrupts
__disable_irq();

// Critical code here

// Enable all interrupts
__enable_irq();
```

---

## 9. DMA Quick Reference

### DMA Configuration
```c
DMA_HandleTypeDef hdma_usart1_rx;

hdma_usart1_rx.Instance = DMA1_Channel5;
hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
HAL_DMA_Init(&hdma_usart1_rx);

// Link DMA to peripheral
__HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
```

### DMA Priority
| Priority | Value | Use Case |
|----------|-------|----------|
| Low | 0 | Background tasks |
| Medium | 1 | General peripherals |
| High | 2 | Time-critical |
| Very High | 3 | ADC, high-speed comms |

---

## 10. Reset & Clock Control (RCC)

### Common RCC Macros
```c
// Enable peripheral clocks
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_USART1_CLK_ENABLE();
__HAL_RCC_SPI1_CLK_ENABLE();
__HAL_RCC_TIM2_CLK_ENABLE();
__HAL_RCC_ADC1_CLK_ENABLE();

// Disable peripheral clocks
__HAL_RCC_GPIOA_CLK_DISABLE();

// Check if clock is enabled
if (__HAL_RCC_GPIOA_IS_CLK_ENABLED()) {
    // Clock is enabled
}

// Force/release peripheral reset
__HAL_RCC_GPIOA_FORCE_RESET();
__HAL_RCC_GPIOA_RELEASE_RESET();
```

---

## 11. Debugging Tips

### Common Issues & Solutions

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| HardFault on startup | Wrong clock config | Check PLL settings |
| GPIO not working | Clock not enabled | `__HAL_RCC_GPIOx_CLK_ENABLE()` |
| UART garbage data | Wrong baud rate | Check clock, recalculate baud |
| ADC reads wrong value | Wrong sampling time | Increase sampling time |
| PWM not outputting | Wrong AF mapping | Check alternate function pin |
| I2C timeout | Missing pull-up resistors | Add 4.7kΩ pull-ups |
| SPI wrong data | Wrong CPOL/CPHA | Match slave device mode |

### Useful Debug Tools
```c
// ITM printf (SWO output)
#define ITM_Port8(n)    (*((volatile uint8_t *)(0xE0000000 + 4*n)))
#define ITM_Port16(n)   (*((volatile uint16_t *)(0xE0000000 + 4*n)))
#define ITM_Port32(n)   (*((volatile uint32_t *)(0xE0000000 + 4*n)))

#define DBG_PRINTF(...) do { \
    if (CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) { \
        while (ITM_Port32(0) == 0); \
        ITM_Port32(0) = __VA_ARGS__; \
    } \
} while(0)

// Measure execution time
uint32_t start = DWT->CYCCNT;
// Code to measure
uint32_t cycles = DWT->CYCCNT - start;
float ms = (float)cycles / SystemCoreClock * 1000.0f;
```

---

## 12. Memory Map (STM32F103)

```
0x0000 0000 - 0x0003 FFFF : Flash (256 KB)
0x1FFF F800 - 0x1FFF FFFF : System Memory (Bootloader)
0x2000 0000 - 0x2000 4FFF : SRAM (20 KB)
0x4000 0000 - 0x4000 03FF : APB1 Peripherals
0x4001 0000 - 0x4001 03FF : APB2 Peripherals
0x4002 0000 - 0x4002 03FF : AHB Peripherals
0xE000 0000 - 0xE000 FFFF : Cortex-M3 Internal Peripherals
```

---

## Ringkasan Cepat

| Peripheral | Max Speed | Key Registers | Common Use |
|------------|-----------|---------------|------------|
| GPIO | 50 MHz | CRL, CRH, IDR, ODR | Digital I/O |
| UART | 4.5 Mbps | DR, SR, BRR | Serial comm |
| SPI | 18 Mbps | DR, SR, CR1, CR2 | High-speed comm |
| I2C | 400 kHz | DR, SR1, SR2, CCR | Sensor interface |
| ADC | 1 µs conv | DR, SR1, SR2, SMPR | Analog sensing |
| TIM | 72 MHz | CNT, ARR, CCRx | PWM, timing |
| DMA | AHB speed | CNDTR, CPAR, CMAR | Data transfer |

---

**Pro Tip:** Selalu gunakan STM32CubeMX untuk generate initialization code, lalu tambahkan logic aplikasi secara manual.

---

*Last Updated: [Tanggal]*  
*Untuk dokumentasi lengkap, lihat Day-03-STM32-Fundamentals.md dan Day-04-Peripherals.md*
