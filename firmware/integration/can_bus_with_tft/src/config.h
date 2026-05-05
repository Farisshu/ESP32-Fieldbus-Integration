#pragma once

// ─── PIN DEFINITIONS ──────────────────────────────
#define PIN_MCP_CS   5
#define PIN_TFT_CS   17
#define PIN_TFT_DC   16
#define PIN_TFT_RST  4

// ─── MCP2515 REGISTERS ───────────────────────────
#define REG_CANCTRL  0x0F
#define REG_CANSTAT  0x0E
#define REG_CNF1     0x2A
#define REG_CNF2     0x29
#define REG_CNF3     0x28
#define REG_RXB0CTRL 0x60
#define REG_RXB0SIDH 0x61
#define REG_RXB0SIDL 0x62
#define REG_RXB0DLC  0x65
#define REG_RXB0D0   0x66
#define REG_CANINTF  0x2C
#define REG_EFLG     0x2D

// ─── SYSTEM CONSTANTS ────────────────────────────
#define CAN_BAUDRATE_KBPS 500
#define SPI_FREQ_HZ 250000 
#define UI_UPDATE_INTERVAL_MS 200

// ─── TIMING & INTERVAL CONSTANTS ──────────────────────────
#define CAN_POLL_INTERVAL_MS          20
#define QUEUE_SEND_TIMEOUT_MS         20
#define QUEUE_RECV_TIMEOUT_MS         10

#define UI_MAX_DRAIN_PER_CYCLE        20
#define BUS_IDLE_TIMEOUT_MS           2000
#define UI_REFRESH_INTERVAL_MS        100

#define LOGGER_MAX_BATCH_PER_CYCLE    10
#define LOGGER_CYCLE_INTERVAL_MS      100

#define SPI_STABILIZATION_DELAY_US    2
#define MCP_RESET_DELAY_MS            50
#define MCP_MODE_SWITCH_DELAY_MS      10

// ─── VALIDATION CONSTANTS ─────────────────────────────────
#define CAN_ID_INVALID_MIN            0x000
#define CAN_ID_INVALID_MAX            0x7FF
#define CAN_DLC_MAX                   8