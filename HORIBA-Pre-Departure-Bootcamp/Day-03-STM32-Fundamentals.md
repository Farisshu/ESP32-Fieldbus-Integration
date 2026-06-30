# Day 03 - STM32 Fundamentals

> **"Know your hardware, and your code will follow."**

---

## Tujuan Pembelajaran

Setelah mempelajari materi ini, Anda akan mampu:

- Memahami clock system dan hierarchy STM32
- Mengkonfigurasi GPIO untuk berbagai aplikasi
- Menggunakan komunikasi peripherals (UART, SPI, I2C)
- Memahami ADC, DAC, PWM, dan Timer
- Mengkonfigurasi interrupt dengan NVIC
- Memahami DMA dan kapan menggunakannya
- Memilih antara HAL dan LL library
- Menggunakan STM32CubeMX dan PlatformIO

---

## Konsep Dasar

### STM32 Family Overview

```
┌─────────────────────────────────────────────────────────┐
│              STM32 FAMILY TREE                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  STM32F0 → Entry level (Cortex-M0)                      │
│  STM32F1 → Mainstream (Cortex-M3)                       │
│  STM32F3 → Mixed signal (Cortex-M4)                     │
│  STM32F4 → Performance (Cortex-M4F)                     │
│  STM32F7 → High performance (Cortex-M7)                 │
│  STM32G0 → Mainstream M0+ (New)                         │
│  STM32G4 → Mixed signal M4F (New)                       │
│  STM32H7 → High performance M7                          │
│  STM32L0/L4 → Low power                                 │
│  STM32U5 → Ultra low power                              │
│                                                         │
│  Untuk bootcamp ini: STM32F1/F4 (paling umum)           │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## Penjelasan Detail

### 1. Clock System

#### Clock Hierarchy

```
┌─────────────────────────────────────────────────────────┐
│            STM32 CLOCK TREE                             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│         HSE (External Crystal)                          │
│              │                                          │
│              ↓                                          │
│         ┌────PLL────┐                                   │
│         │  × N / M  │                                   │
│         └─────┬─────┘                                   │
│               │                                         │
│       ┌───────┼───────┐                                │
│       ↓       ↓       ↓                                │
│   SYSCLK   AHB     APB1                               │
│    │        │        │                                 │
│    │        ↓        ↓                                 │
│    │     AHB Bus  APB1 Peripherals                    │
│    │              (UART, I2C, etc)                    │
│    │                                                  │
│    ↓                                                  │
│  APB2                                                 │
│    │                                                  │
│    ↓                                                  │
│  APB2 Peripherals                                     │
│  (TIM, ADC, SPI, etc)                                 │
│                                                         │
│  HSI (Internal RC) ──→ Backup clock                    │
│  LSE (32.768 kHz) ──→ RTC                              │
│  LSI (Internal) ──→ Watchdog                           │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Clock Configuration Code

```c
/**
 * @brief System Clock Configuration
 * @note Configure PLL to get 72MHz system clock (STM32F4 example)
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    /* Configure HSE Oscillator and activate PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;      /* HSE / 8 = 1MHz */
    RCC_OscInitStruct.PLL.PLLN = 72;     /* 1MHz × 72 = 72MHz */
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  /* 72MHz / 2 = 36MHz */
    RCC_OscInitStruct.PLL.PLLQ = 3;      /* For USB: 72/3 = 24MHz */
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    
    /* Configure system clock */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |
                                   RCC_CLOCKTYPE_HCLK |
                                   RCC_CLOCKTYPE_PCLK1 |
                                   RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   /* 72MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;    /* 36MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;    /* 72MHz */
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}
```

**Mengapa Clock Penting?**
- Setiap peripheral butuh clock untuk bekerja
- Clock speed mempengaruhi performa dan power consumption
- Timing-critical applications (UART baudrate, PWM frequency) bergantung pada clock accuracy

### 2. GPIO (General Purpose Input/Output)

#### GPIO Modes

```
┌─────────────────────────────────────────────────────────┐
│              GPIO MODES                                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  INPUT MODES:                                           │
│  • Input Floating    - No pull-up/pull-down             │
│  • Input Pull-up     - Internal resistor to VDD         │
│  • Input Pull-down   - Internal resistor to GND         │
│  • Analog            - For ADC input                    │
│                                                         │
│  OUTPUT MODES:                                          │
│  • Output Push-pull  - Drives high or low               │
│  • Output Open-drain - Drives low, high-Z otherwise     │
│                                                         │
│  ALTERNATE FUNCTION:                                    │
│  • AF Push-pull      - For UART, SPI, etc               │
│  • AF Open-drain     - For I2C, etc                     │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### GPIO Configuration

```c
/* LED Configuration (Output Push-pull) */
GPIO_InitTypeDef GPIO_InitStruct = {0};

__HAL_RCC_GPIOA_CLK_ENABLE();  /* Enable GPIOA clock FIRST! */

GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* Button Configuration (Input with Pull-up) */
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLUP;  /* Button connects to GND */
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```

#### GPIO Usage Patterns

```c
/* Toggle LED */
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

/* Read button (active low because of pull-up) */
if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
    // Button pressed
}

/* Bit-banging example (bitwise operations) */
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

// Set high
HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

// Set low
HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

// Toggle
HAL_GPIO_WritePin(LED_PORT, LED_PIN, 
                  (GPIO_PinState)!HAL_GPIO_ReadPin(LED_PORT, LED_PIN));
```

### 3. UART (Universal Asynchronous Receiver/Transmitter)

#### UART Frame Structure

```
┌─────────────────────────────────────────────────────────┐
│              UART FRAME                                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Idle ──→ [Start] [D0-D7] [Parity] [Stop] ──→ Idle     │
│           │________││______││_______││____│            │
│                1 bit   8 bits   0/1 bit   1/2 bits     │
│                                                         │
│  Total: 10 bits (no parity, 1 stop bit)                 │
│                                                         │
│  Baudrate = Bits per second                             │
│  Example: 9600 baud = 9600 bits/second                  │
│  Time per bit = 1/9600 ≈ 104μs                          │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### UART Configuration

```c
/* UART Configuration */
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

#### UART Communication Patterns

```c
/* Polling (blocking) */
uint8_t tx_data[] = "Hello World\r\n";
HAL_UART_Transmit(&huart1, tx_data, sizeof(tx_data), 100);

uint8_t rx_data;
HAL_UART_Receive(&huart1, &rx_data, 1, 100);

/* Interrupt (non-blocking) */
uint8_t rx_buffer[64];
HAL_UART_Receive_IT(&huart1, rx_buffer, 1);

/* Callback (called from ISR) */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // Process received byte
        ProcessByte(rx_buffer[0]);
        
        // Re-arm reception
        HAL_UART_Receive_IT(&huart1, rx_buffer, 1);
    }
}

/* DMA (most efficient for large data) */
uint8_t dma_buffer[256];
HAL_UART_Receive_DMA(&huart1, dma_buffer, sizeof(dma_buffer));
```

### 4. SPI (Serial Peripheral Interface)

#### SPI Architecture

```
┌─────────────────────────────────────────────────────────┐
│              SPI CONNECTION                             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│   MASTER                        SLAVE                   │
│   ┌──────┐                    ┌──────┐                 │
│   │      │──SCK──→            │      │                 │
│   │      │──MOSI─→            │      │                 │
│   │      │←─MISO─             │      │                 │
│   │      │──NSS──→            │      │                 │
│   │      │                    │      │                 │
│   └──────┘                    └──────┘                 │
│                                                         │
│   SCK  : Serial Clock (from master)                     │
│   MOSI : Master Out Slave In                            │
│   MISO : Master In Slave Out                            │
│   NSS  : Slave Select (Chip Select)                     │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### SPI Modes (Clock Polarity & Phase)

```
┌─────────────────────────────────────────────────────────┐
│              SPI MODES                                  │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Mode 0: CPOL=0, CPHA=0                                 │
│  ────    ────────                                       │
│  SCK: __/‾‾\__/‾‾\__                                    │
│       ▲          ▲                                      │
│       │          │                                      │
│  Data sampled on rising edge                            │
│                                                         │
│  Mode 1: CPOL=0, CPHA=1                                 │
│  SCK: __/‾‾\__/‾‾\__                                    │
│           ▲          ▲                                  │
│           │          │                                  │
│  Data sampled on falling edge                           │
│                                                         │
│  Mode 2: CPOL=1, CPHA=0                                 │
│  SCK: ‾‾\__/‾‾\__/‾‾                                    │
│       ▲          ▲                                      │
│       │          │                                      │
│  Data sampled on falling edge                           │
│                                                         │
│  Mode 3: CPOL=1, CPHA=1                                 │
│  SCK: ‾‾\__/‾‾\__/‾‾                                    │
│           ▲          ▲                                  │
│           │          │                                  │
│  Data sampled on rising edge                            │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### SPI Configuration

```c
/* SPI Configuration */
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

/* Chip Select control */
#define CS_PIN GPIO_PIN_4
#define CS_PORT GPIOB

/* Transaction example */
void SPI_Transaction(uint8_t *tx_data, uint8_t *rx_data, uint16_t size) {
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);  /* Select slave */
    
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, size, 100);
    
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);  /* Deselect slave */
}
```

### 5. I2C (Inter-Integrated Circuit)

#### I2C Bus Structure

```
┌─────────────────────────────────────────────────────────┐
│              I2C BUS                                    │
├─────────────────────────────────────────────────────────┤
│                                                         │
│   VDD                                                   │
│    │                                                    │
│   ┌┴┐  4.7kΩ    ┌┴┐  4.7kΩ                            │
│   │ │           │ │                                     │
│   └┬┘           └┬┘                                     │
│    │ SDA         │ SCL                                   │
│    ├─────────────┼──────────────────┐                   │
│    │             │                  │                   │
│ ┌──┴──┐       ┌──┴──┐            ┌──┴──┐              │
│ │MASTER│       │SLAVE│            │SLAVE│              │
│ │Addr 0│       │Addr 1│            │Addr 2│              │
│ └─────┘       └─────┘            └─────┘              │
│                                                         │
│  Open-drain with pull-up resistors                      │
│  Multi-master, multi-slave                              │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### I2C Protocol

```
┌─────────────────────────────────────────────────────────┐
│              I2C TRANSACTION                            │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  START → [ADDR+W] → ACK → [REG_ADDR] → ACK →           │
│  [DATA] → ACK → STOP                                    │
│                                                         │
│  READ:                                                  │
│  START → [ADDR+R] → ACK → [DATA] → NACK → STOP         │
│                                                         │
│  START Condition: SDA falls while SCL high              │
│  STOP Condition: SDA rises while SCL high               │
│  ACK: SDA pulled low during ACK clock pulse             │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### I2C Configuration

```c
/* I2C Configuration */
hi2c1.Instance = I2C1;
hi2c1.Init.Timing = 0x00702991;  /* Calculated for 100kHz */
hi2c1.Init.OwnAddress1 = 0x00;
hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

HAL_I2C_Init(&hi2c1);

/* Write to register */
uint8_t reg_addr = 0x00;
uint8_t data = 0x55;
uint8_t slave_addr = 0x48;  /* 7-bit address */

HAL_I2C_Mem_Write(&hi2c1, slave_addr << 1, 
                   reg_addr, I2C_MEMADD_SIZE_8BIT,
                   &data, 1, 100);

/* Read from register */
uint8_t read_data;
HAL_I2C_Mem_Read(&hi2c1, slave_addr << 1,
                  reg_addr, I2C_MEMADD_SIZE_8BIT,
                  &read_data, 1, 100);
```

### 6. ADC (Analog to Digital Converter)

#### ADC Concepts

```
┌─────────────────────────────────────────────────────────┐
│              ADC PARAMETERS                             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Resolution: Number of bits                             │
│  • 12-bit = 4096 levels (0-4095)                        │
│  • 10-bit = 1024 levels                                 │
│  • 8-bit = 256 levels                                   │
│                                                         │
│  Reference Voltage (Vref+):                             │
│  • Typically 3.3V                                       │
│  • LSB = Vref+ / 2^resolution                           │
│  • For 12-bit, 3.3V: LSB = 3.3V / 4096 ≈ 0.8mV         │
│                                                         │
│  Conversion Time:                                       │
│  • Sample time + conversion time                        │
│  • Faster = more noise                                  │
│                                                         │
│  Input Range: 0V to Vref+                               │
│  • Use voltage divider for higher voltages              │
│  • Use op-amp for signal conditioning                   │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### ADC Configuration

```c
/* ADC Configuration */
hadc1.Instance = ADC1;
hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
hadc1.Init.Resolution = ADC_RESOLUTION_12B;
hadc1.Init.ScanConvMode = DISABLE;
hadc1.Init.ContinuousConvMode = DISABLE;
hadc1.Init.DiscontinuousConvMode = DISABLE;
hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
hadc1.Init.NbrOfConversion = 1;
hadc1.Init.DMAContinuousRequests = DISABLE;

HAL_ADC_Init(&hadc1);

/* Configure channel */
ADC_ChannelConfTypeDef sConfig = {0};
sConfig.Channel = ADC_CHANNEL_0;
sConfig.Rank = 1;
sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;

HAL_ADC_ConfigChannel(&hadc1, &sConfig);

/* Single conversion */
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 100);
uint32_t adc_value = HAL_ADC_GetValue(&hadc1);

/* Convert to voltage */
float voltage = (adc_value * 3.3f) / 4095.0f;
```

### 7. PWM (Pulse Width Modulation) & Timer

#### PWM Concept

```
┌─────────────────────────────────────────────────────────┐
│              PWM WAVEFORM                               │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Period                                                 │
│  ←───────────────────→                                  │
│  ┌─────┐         ┌─────┐                               │
│  │     │         │     │                               │
│  │     │         │     │                               │
│  └     └─────────┘     └────────                       │
│  ↑               ↑                                      │
│  │               │                                      │
│  Duty Cycle      Period End                             │
│                                                         │
│  Duty Cycle = (On Time / Period) × 100%                 │
│                                                         │
│  Frequency = 1 / Period                                 │
│                                                         │
│  Applications:                                          │
│  • Motor speed control                                  │
│  • LED dimming                                          │
│  • Power supply regulation                              │
│  • Audio generation                                     │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Timer PWM Configuration

```c
/* Timer PWM Configuration */
htim2.Instance = TIM2;
htim2.Init.Prescaler = 71;  /* 72MHz / 72 = 1MHz timer clock */
htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
htim2.Init.Period = 999;    /* 1MHz / 1000 = 1kHz PWM frequency */
htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

HAL_TIM_PWM_Init(&htim2);

/* Configure PWM channel */
TIM_OC_InitTypeDef sConfigOC = {0};
sConfigOC.OCMode = TIM_OCMODE_PWM1;
sConfigOC.Pulse = 500;  /* 50% duty cycle (500/1000) */
sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

/* Start PWM output */
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

/* Change duty cycle dynamically */
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 250);  /* 25% duty */
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 750);  /* 75% duty */
```

### 8. Interrupt & NVIC

#### Interrupt Priority

```
┌─────────────────────────────────────────────────────────┐
│           NVIC PRIORITY STRUCTURE                       │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Priority = Preemption Priority + Sub Priority          │
│                                                         │
│  Lower number = Higher priority                         │
│                                                         │
│  Preemption Priority:                                   │
│  • Can interrupt lower preemption priority ISR          │
│  • Nested interrupts                                    │
│                                                         │
│  Sub Priority:                                          │
│  • Used when preemption priority is same                │
│  • Cannot nest, but determines pending order            │
│                                                         │
│  Example (4 bits for priority):                         │
│  • 2 bits preemption, 2 bits sub                        │
│  • Preemption: 0-3 (4 levels)                           │
│  • Sub: 0-3 (4 levels within same preemption)           │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### Interrupt Configuration

```c
/* Configure EXTI interrupt for button */
HAL_GPIO_EXTI_IRQHandler() is called automatically

/* In main.c, after HAL_Init() */
HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* GPIO configuration for interrupt */
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  /* Interrupt on falling edge */
GPIO_InitStruct.Pull = GPIO_PULLUP;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* Callback (override weak function) */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        // Button pressed - handle it
        // Keep ISR short!
        button_pressed_flag = true;
    }
}
```

### 9. DMA (Direct Memory Access)

#### DMA Concept

```
┌─────────────────────────────────────────────────────────┐
│              DMA OPERATION                              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  WITHOUT DMA:                                           │
│  CPU ──→ Read peripheral ──→ Write memory              │
│        (CPU busy during transfer)                       │
│                                                         │
│  WITH DMA:                                              │
│  DMA Controller                                         │
│  Peripheral ←───→ Memory                                │
│  (CPU free to do other tasks)                           │
│                                                         │
│  Best for:                                              │
│  • Large data transfers (ADC buffers, UART packets)     │
│  • High-speed streaming                                 │
│  • Power saving (CPU can sleep)                         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### DMA Configuration

```c
/* DMA Configuration for ADC */
hdma_adc1.Instance = DMA1_Stream0;
hdma_adc1.Init.Channel = DMA_CHANNEL_0;
hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
hdma_adc1.Init.Mode = DMA_CIRCULAR;  /* Continuous mode */
hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;

HAL_DMA_Init(&hdma_adc1);

/* Link DMA to ADC */
__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

/* Start ADC with DMA */
uint32_t adc_buffer[100];
HAL_ADC_Start_DMA(&hadc1, adc_buffer, 100);

/* DMA Complete Callback */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    // ADC buffer full, process data
    Process_ADC_Data(adc_buffer, 100);
    
    // Restart if not circular
    // HAL_ADC_Start_DMA(hadc, adc_buffer, 100);
}
```

### 10. HAL vs LL Library

#### Comparison

```
┌─────────────────────────────────────────────────────────┐
│           HAL vs LL COMPARISON                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  HAL (Hardware Abstraction Layer):                      │
│  • High-level API                                       │
│  • Easy to use, portable                                │
│  • More overhead (larger code, slower)                  │
│  • Good for prototyping                                 │
│  • CubeMX generates HAL code                            │
│                                                         │
│  LL (Low Layer):                                        │
│  • Register-level access                                │
│  • Minimal overhead                                     │
│  • Faster execution, smaller code                       │
│  • Steeper learning curve                               │
│  • MCU-specific (less portable)                         │
│                                                         │
│  Recommendation:                                        │
│  • Prototype with HAL                                   │
│  • Optimize critical sections with LL                   │
│  • Or mix: HAL for config, LL for time-critical         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

#### HAL vs LL Example

```c
/* HAL: Set GPIO pin */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

/* LL: Set GPIO pin (direct register access) */
LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);

/* HAL: UART transmit (blocking) */
HAL_UART_Transmit(&huart1, data, size, timeout);

/* LL: UART transmit (polling) */
while (--size) {
    while (!LL_USART_IsActiveFlag_TXE(USART1));
    LL_USART_TransmitData8(USART1, *data++);
}
while (!LL_USART_IsActiveFlag_TC(USART1));
```

---

## Kesalahan yang Sering Terjadi

### 1. Forgot to Enable Clock

```c
// ❌ Won't work!
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = GPIO_PIN_5;
// Missing: __HAL_RCC_GPIOA_CLK_ENABLE();
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// ✅ Correct
__HAL_RCC_GPIOA_CLK_ENABLE();  /* Enable clock FIRST */
GPIO_InitStruct.Pin = GPIO_PIN_5;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```

### 2. Wrong NVIC Priority Configuration

```c
// ❌ All interrupts at same priority - unpredictable order
HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);

// ✅ Different priorities for deterministic behavior
HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);  /* Higher priority */
HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);  /* Lower priority */
```

### 3. Blocking in ISR

```c
// ❌ NEVER do this in ISR!
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    HAL_Delay(100);  /* BLOCKING! */
    HAL_UART_Transmit(&huart1, data, size, 1000);  /* BLOCKING! */
}

// ✅ Set flag, handle in main loop
volatile bool event_flag = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    event_flag = true;  /* Quick! */
}

int main(void) {
    while (1) {
        if (event_flag) {
            event_flag = false;
            HAL_UART_Transmit(&huart1, data, size, 1000);
        }
    }
}
```

### 4. I2C Address Confusion

```c
// ❌ Using 8-bit address where 7-bit expected
uint8_t addr = 0x90;  /* This is 7-bit << 1 */
HAL_I2C_Master_Transmit(&hi2c1, addr, ...);  /* Wrong! */

// ✅ Use 7-bit address, HAL shifts it
uint8_t addr = 0x48;  /* 7-bit address */
HAL_I2C_Master_Transmit(&hi2c1, addr << 1, ...);  /* Correct */
```

### 5. ADC Sampling Too Fast

```c
// ❌ Not enough sampling time for accurate reading
sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;  /* Too fast! */

// ✅ Allow enough time for capacitor to charge
sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;  /* Better */

/* Also consider source impedance */
/* High impedance sources need longer sample times */
```

---

## Tips Engineer Profesional

### 💡 Tip 1: Use STM32CubeMX Wisely

CubeMX is great for initialization, but:
- Review generated code
- Understand what it configures
- Don't rely on it for everything
- Add your own comments

### 💡 Tip 2: Create Hardware Abstraction

```c
// Instead of scattering HAL calls everywhere:
void LED_On(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void LED_Off(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

// Now you can change hardware without touching application code
```

### 💡 Tip 3: Debug with SWO/SWT

STM32 has SWO (Serial Wire Output) for printf debugging without UART:

```c
#include <stdio.h>

// With ITM (Instrumentation Trace Macrocell)
ITM_SendChar('H');
ITM_SendChar('i');

// Or use semihosting for development
printf("Debug: value = %d\n", value);
```

### 💡 Tip 4: Measure Execution Time

```c
/* Use DWT cycle counter */
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

/* Measure function time */
uint32_t start = DWT->CYSCNT;
MyFunction();
uint32_t cycles = DWT->CYSCNT - start;
float time_us = (float)cycles / SystemCoreClock * 1000000.0f;
```

### 💡 Tip 5: Plan Your Pin Assignment

Before starting:
- List all required pins
- Check alternate functions
- Consider routing (for PCB)
- Reserve pins for debugging
- Document pin assignments

---

## Ringkasan

### Key Takeaways Day 03

1. **Clock system** adalah foundation - configure first
2. **GPIO** modes harus match application requirements
3. **UART/SPI/I2C**各有优缺点 - choose based on needs
4. **ADC** requires proper sampling time and reference
5. **PWM** controlled by timers - understand prescaler & period
6. **Interrupts** need careful priority planning
7. **DMA** frees CPU for other tasks
8. **HAL vs LL** - trade-off between ease and efficiency

### Checklist Pemahaman

- [ ] Saya bisa configure clock system
- [ ] Saya paham GPIO modes dan aplikasinya
- [ ] Saya bisa setup UART communication
- [ ] Saya mengerti perbedaan SPI dan I2C
- [ ] Saya bisa configure ADC dan baca nilai
- [ ] Saya bisa generate PWM dengan timer
- [ ] Saya paham interrupt priority
- [ ] Saya tahu kapan menggunakan DMA

---

## Referensi Belajar

### Official Documentation
1. [STM32 Datasheets](https://www.st.com/en/microcontrollers-microprocessors/stm32-mainstream-arm-cortex-mcus.html)
2. [STM32 Reference Manuals](https://www.st.com/en/microcontrollers-microprocessors/stm32-mainstream-arm-cortex-mcus.html#resources)
3. [STM32CubeMX User Manual](https://www.st.com/en/development-tools/stm32cubemx.html)

### Video Tutorials
1. Phil's Lab - STM32 Tutorial Series
2. Controllerstech - STM32 Tutorials
3. FastBit Embedded Brain Academy

### Books
1. "Mastering STM32" - Carmine Noviello
2. "Getting Started with STM32" - Various authors

### Tools
1. STM32CubeMX - Configuration tool
2. STM32CubeIDE - Development environment
3. STM32CubeProgrammer - Flashing utility
4. Saleae Logic Analyzer - Protocol debugging

---

> **"The devil is in the details. So is the elegance."**

*Lanjutkan ke Day 04: Embedded Architecture*
