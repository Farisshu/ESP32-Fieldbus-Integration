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
#define SPI_FREQ_HZ       1000000
#define UI_UPDATE_INTERVAL_MS 200