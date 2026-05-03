#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "config.h"
#include "mcp2515_driver.h"
#include "tft_ui.h"
#include "logger.h"
#include "app_tasks.h"

MCP2515Driver can(PIN_MCP_CS);
TFT_UI display(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
CANLogger logger("/can_log.csv");

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.println("\n🚀 CAN Bus + TFT (FreeRTOS + Logging)");

    SPI.begin(18, 19, 23, -1);
    
// ❌ Lama (Ukuran struct CANFrame):
// canFrameQueue = xQueueCreate(10, sizeof(MCP2515Driver::CANFrame));

// ✅ Baru (Ukuran struct QueuedMessage yang lebih besar):
// ✅ WAJIB SEPERTI INI:
canFrameQueue = xQueueCreate(10, sizeof(QueuedMessage));
    
    if (!display.begin() || !can.begin() || !logger.begin()) {
        Serial.println("❌ Init Failed"); while(1);
    }
    
    display.drawStaticLayout();
    Serial.println("✅ System Ready. Tasks Started.");

    xTaskCreatePinnedToCore(vTaskCAN,   "CAN_Task",   4096, &can,   2, NULL, 1);
    xTaskCreatePinnedToCore(vTaskUI,    "UI_Task",    4096, &display, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskLogger,"Logger_Task",4096, &logger,  1, NULL, 1);
}

void loop() {
    vTaskDelete(NULL); // Handover to FreeRTOS scheduler
}