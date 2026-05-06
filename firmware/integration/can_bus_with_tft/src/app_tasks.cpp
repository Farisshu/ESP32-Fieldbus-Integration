#include "app_tasks.h"

xQueueHandle canFrameQueue;

// Global error tracking for Bus Health monitoring
static uint32_t g_totalErrorCount = 0;
static uint8_t g_lastEflg = 0;

// ── TASK 1: CAN READER (Producer) ──────────────────────────
void vTaskCAN(void* pv) {
    MCP2515Driver* can = (MCP2515Driver*)pv;
    MCP2515Driver::CANFrame frame;
    static uint32_t globalSequence = 0;

    for (;;) {
        if (can->receiveFrame(frame)) {
            // ✅ FILTER: Buang ID noise + DLC invalid
            if (frame.id != CAN_ID_INVALID_MAX && 
                frame.id != CAN_ID_INVALID_MIN && 
                frame.dlc <= CAN_DLC_MAX && 
                frame.dlc > 0) {
                
                QueuedMessage msg;
                msg.frame = frame;
                msg.sequenceNumber = ++globalSequence;
                msg.timestamp = millis();
                msg.isValid = true;
                
                if (xQueueSend(canFrameQueue, &msg, pdMS_TO_TICKS(QUEUE_SEND_TIMEOUT_MS)) == pdPASS) {
                    Serial.printf("[RX] ID:0x%03X DLC:%d | Count: %lu\n", 
                                  frame.id, frame.dlc, msg.sequenceNumber);
                }
            }
        }
        
        // Monitor EFLG for bus health tracking
        uint8_t eflg = can->getErrorFlags();
        if (eflg != 0) {
            g_totalErrorCount++;
            g_lastEflg = eflg;
            Serial.printf("[EFLG] 0x%02X (Total: %lu)\n", eflg, g_totalErrorCount);
        }
        
        vTaskDelay(pdMS_TO_TICKS(CAN_POLL_INTERVAL_MS));
    }
}

// ── TASK 2: UI DISPLAY (Consumer) ─────────────────────────
void vTaskUI(void* pv) {
    TFT_UI* ui = (TFT_UI*)pv;
    QueuedMessage msg;
    QueuedMessage latestMsg = {0};
    uint32_t lastActivityTime = millis();
    bool hasNewData = false;
    uint32_t lastHealthUpdate = 0;

    for (;;) {
        hasNewData = false;
        
        // ✅ DRAIN QUEUE dengan limiter (prevent starvation)
        int drained = 0;
        while (drained < UI_MAX_DRAIN_PER_CYCLE && 
               xQueueReceive(canFrameQueue, &msg, 0) == pdPASS) {
            if (msg.isValid) {
                latestMsg = msg;
                hasNewData = true;
                lastActivityTime = millis();
            }
            drained++;
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
            // Timeout: jika >BUS_IDLE_TIMEOUT_MS tidak ada data
            if (millis() - lastActivityTime > BUS_IDLE_TIMEOUT_MS) {
                ui->updateFrame(0, 0, nullptr, latestMsg.sequenceNumber, true);
            }
        }
        
        // Update Bus Health every 500ms (independent of frame reception)
        if (millis() - lastHealthUpdate >= 500) {
            ui->updateBusHealth(g_lastEflg, g_totalErrorCount);
            lastHealthUpdate = millis();
        }
        
        vTaskDelay(pdMS_TO_TICKS(UI_REFRESH_INTERVAL_MS));
    }
}

// ── TASK 3: LOGGER (Consumer) ───────────────────────────
void vTaskLogger(void* pv) {
    CANLogger* logger = (CANLogger*)pv;
    QueuedMessage msg;
    
    for (;;) {
        // ✅ Batch logging: ambil semua message tersedia dalam satu cycle
        int logged = 0;
        while (logged < LOGGER_MAX_BATCH_PER_CYCLE && 
               xQueueReceive(canFrameQueue, &msg, pdMS_TO_TICKS(QUEUE_RECV_TIMEOUT_MS)) == pdPASS) {
            if (msg.isValid) {
                logger->logFrame(msg.frame, msg.timestamp);
                logged++;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(LOGGER_CYCLE_INTERVAL_MS));
    }
}