#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "mcp2515_driver.h"
#include "tft_ui.h"
#include "logger.h"

extern xQueueHandle canFrameQueue;

// ✅ Struktur data terpusat untuk antar-task
struct QueuedMessage {
    MCP2515Driver::CANFrame frame;
    uint32_t sequenceNumber;  // Counter global yang sinkron
    uint32_t timestamp;       // Untuk deteksi timeout akurat
    bool isValid;             // Flag validasi
};

void vTaskCAN(void* pv);
void vTaskUI(void* pv);
void vTaskLogger(void* pv);