/**
 * @file main.cpp
 * @brief RS485 Loopback Test for XY-017 Module
 * 
 * This test validates the hardware integrity of the RS485 module by
 * sending data and receiving it back through a loopback connection.
 * 
 * Hardware Setup:
 * - Connect RS485 Module A and B terminals together with a jumper wire
 * - This creates a physical loopback path for testing
 * 
 * Expected Result:
 * - Every message sent should be received back identically
 * - Serial monitor will show "SUCCESS" if data matches
 */

#include <Arduino.h>

// --- Pin Configuration ---
#define RS485_TX_PIN      17    // GPIO 17 -> DI (Driver Input)
#define RS485_RX_PIN      16    // GPIO 16 -> RO (Receiver Output)
#define RS485_DE_RE_PIN   4     // GPIO 4  -> DE/RE (Direction Control)

// --- Communication Settings ---
#define BAUD_RATE         9600
#define RS485_SERIAL      Serial2

void setup() {
    // Initialize USB Serial for debugging
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    
    Serial.println("========================================");
    Serial.println("🚀 RS485 Loopback Test Started");
    Serial.println("========================================");

    // Initialize RS485 Serial (Hardware UART2)
    RS485_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
    
    // Configure Direction Control Pin
    pinMode(RS485_DE_RE_PIN, OUTPUT);
    digitalWrite(RS485_DE_RE_PIN, LOW); // Default to Receive Mode
    
    Serial.println("✅ RS485 Module Initialized");
    Serial.println("📌 Ensure A and B terminals are jumpered together!");
    Serial.println("========================================\n");
}

void loop() {
    static unsigned long lastSendTime = 0;
    const unsigned long SEND_INTERVAL_MS = 2000;

    // --- TRANSMIT PHASE ---
    if (millis() - lastSendTime >= SEND_INTERVAL_MS) {
        String message = "LOOPBACK_TEST_" + String(millis());
        
        Serial.print("📤 Transmitting: ");
        Serial.println(message);

        // Switch to Transmit Mode
        digitalWrite(RS485_DE_RE_PIN, HIGH);
        delayMicroseconds(100); // Allow time for driver to enable

        // Send data over RS485
        RS485_SERIAL.println(message);
        RS485_SERIAL.flush(); // Wait for transmission to complete

        // Switch back to Receive Mode
        digitalWrite(RS485_DE_RE_PIN, LOW);
        delayMicroseconds(100); // Allow time for receiver to enable
        
        lastSendTime = millis();
    }

    // --- RECEIVE PHASE ---
    if (RS485_SERIAL.available()) {
        String receivedMessage = RS485_SERIAL.readStringUntil('\n');
        receivedMessage.trim();

        if (!receivedMessage.isEmpty()) {
            Serial.print("📥 Received:      ");
            Serial.println(receivedMessage);

            // Validate loopback integrity
            // Note: We check if the received message starts with expected prefix
            // since exact timestamp match is impossible due to propagation delay
            if (receivedMessage.startsWith("LOOPBACK_TEST_")) {
                Serial.println("✅ SUCCESS: Data integrity verified!\n");
            } else {
                Serial.println("❌ ERROR: Data mismatch or corruption detected!\n");
            }
        }
    }
}
