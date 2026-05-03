#include "app_tasks.h"

xQueueHandle canFrameQueue;

// ── TASK 1: CAN READER (Producer) ──────────────────────────
// ── TASK 1: CAN READER (Producer) ──────────────────────────
void vTaskCAN(void* pv) {
    MCP2515Driver* can = (MCP2515Driver*)pv;
    MCP2515Driver::CANFrame frame;
    static uint32_t globalSequence = 0;

    for (;;) {
        if (can->receiveFrame(frame)) {
            // ✅ FILTER GANDA: Buang ID noise + DLC invalid
            if (frame.id != 0x7FF && frame.id != 0x000 && frame.dlc <= 8 && frame.dlc > 0) {
                
                QueuedMessage msg;
                msg.frame = frame;
                msg.sequenceNumber = ++globalSequence;
                msg.timestamp = millis();
                msg.isValid = true;
                
                if (xQueueSend(canFrameQueue, &msg, pdMS_TO_TICKS(20)) == pdPASS) {
                    Serial.printf("[RX] ID:0x%03X DLC:%d | Count: %lu\n", 
                                  frame.id, frame.dlc, msg.sequenceNumber);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}   

// ── TASK 2 & 3: Tidak perlu diubah (sudah optimal) ──────────
// [vTaskUI dan vTaskLogger tetap sama seperti kode sebelumnya]

// ── TASK 2: UI DISPLAY (Consumer) ─────────────────────────
void vTaskUI(void* pv) {
    TFT_UI* ui = (TFT_UI*)pv;
    QueuedMessage msg;
    QueuedMessage latestMsg = {0}; // ✅ Zero-init aman
    uint32_t lastActivityTime = millis();
    bool hasNewData = false;

    for (;;) {
        hasNewData = false;
        
        // ✅ DRAIN QUEUE: Ambil semua frame antri, simpan yang TERAKHIR
        while (xQueueReceive(canFrameQueue, &msg, 0) == pdPASS) {
            if (msg.isValid) {
                latestMsg = msg;
                hasNewData = true;
                lastActivityTime = millis();
            }
        }
        
        if (hasNewData) {
            ui->updateFrame(
                latestMsg.frame.id,
                latestMsg.frame.dlc,
                latestMsg.frame.data,
                latestMsg.sequenceNumber,
                false
            );
        } else {
            // Timeout check: jika >2 detik tidak ada data, tampilkan BUS IDLE
            if (millis() - lastActivityTime > 2000) {
                ui->updateFrame(0, 0, nullptr, latestMsg.sequenceNumber, true);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100)); // Max 10Hz UI refresh
    }
}

// ── TASK 3: LOGGER (Consumer) ───────────────────────────
void vTaskLogger(void* pv) {
    CANLogger* logger = (CANLogger*)pv;
    QueuedMessage msg;
    
    for (;;) {
        if (xQueueReceive(canFrameQueue, &msg, pdMS_TO_TICKS(200)) == pdPASS && msg.isValid) {
            logger->logFrame(msg.frame, msg.timestamp);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}