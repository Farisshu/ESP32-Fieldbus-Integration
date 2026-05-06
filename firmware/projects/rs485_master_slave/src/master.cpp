/**
 * @file master.cpp
 * @brief RS485 Master Node for Point-to-Point Communication
 * 
 * This node acts as the MASTER in a Master-Slave RS485 network.
 * It periodically sends commands to the Slave node and waits for responses.
 * 
 * Hardware Setup:
 * - Connect Module A terminal to Slave Module A terminal
 * - Connect Module B terminal to Slave Module B terminal
 * - Connect GND of both ESP32s together (CRITICAL)
 */

#include <Arduino.h>

// --- Pin Configuration ---
#define RS485_TX_PIN      17    // GPIO 17 -> DI (Driver Input)
#define RS485_RX_PIN      16    // GPIO 16 -> RO (Receiver Output)
#define RS485_DE_RE_PIN   4     // GPIO 4  -> DE/RE (Direction Control)

// --- Communication Settings ---
#define BAUD_RATE         9600
#define RS485_SERIAL      Serial2

// --- Helper Functions ---
void setRS485Mode(bool transmit) {
    digitalWrite(RS485_DE_RE_PIN, transmit ? HIGH : LOW);
    delayMicroseconds(100); // Allow time for driver/receiver switching
}

void setup() {
    // Initialize USB Serial for debugging
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    
    Serial.println("========================================");
    Serial.println("🤖 RS485 MASTER Node Started");
    Serial.println("========================================");

    // Initialize RS485 Serial
    RS485_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
    
    // Configure Direction Control Pin
    pinMode(RS485_DE_RE_PIN, OUTPUT);
    setRS485Mode(false); // Default to Receive Mode
    
    Serial.println("✅ Master Ready - Sending commands every 3 seconds");
    Serial.println("========================================\n");
}

void loop() {
    static unsigned long lastCommandTime = 0;
    const unsigned long COMMAND_INTERVAL_MS = 3000;

    // --- SEND COMMAND PHASE ---
    if (millis() - lastCommandTime >= COMMAND_INTERVAL_MS) {
        String command = "CMD:READ_DATA|TS:" + String(millis());
        
        Serial.print("📤 Master Sending: ");
        Serial.println(command);
        
        setRS485Mode(true); // Switch to Transmit Mode
        RS485_SERIAL.println(command);
        RS485_SERIAL.flush();
        setRS485Mode(false); // Switch back to Receive Mode
        
        lastCommandTime = millis();
    }

    // --- RECEIVE RESPONSE PHASE ---
    if (RS485_SERIAL.available()) {
        String response = RS485_SERIAL.readStringUntil('\n');
        response.trim();
        
        if (!response.isEmpty()) {
            Serial.print("📥 Master Received: ");
            Serial.println(response);
            
            // Parse and validate response
            if (response.startsWith("ACK|")) {
                Serial.println("✅ Valid Response from Slave\n");
            } else if (response.startsWith("NACK|")) {
                Serial.println("⚠️ Slave Reported Error\n");
            } else {
                Serial.println("❌ Unknown Response Format\n");
            }
        }
    }
}
