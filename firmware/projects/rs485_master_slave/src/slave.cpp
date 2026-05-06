/**
 * @file slave.cpp
 * @brief RS485 Slave Node for Point-to-Point Communication
 * 
 * This node acts as the SLAVE in a Master-Slave RS485 network.
 * It listens for commands from the Master and responds with simulated sensor data.
 * 
 * Hardware Setup:
 * - Connect Module A terminal to Master Module A terminal
 * - Connect Module B terminal to Master Module B terminal
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

// Simulate sensor data generation
String generateSensorData() {
    // Generate pseudo-random but realistic sensor values
    static int counter = 0;
    counter++;
    
    float temperature = 25.0 + (counter % 10) * 0.5;  // 25.0 - 29.5 °C
    float humidity = 60.0 + (counter % 5) * 2.0;      // 60.0 - 68.0 %
    
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "TEMP:%.1f|HUM:%.1f", temperature, humidity);
    return String(buffer);
}

void setup() {
    // Initialize USB Serial for debugging
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    
    Serial.println("========================================");
    Serial.println("👤 RS485 SLAVE Node Started");
    Serial.println("========================================");

    // Initialize RS485 Serial
    RS485_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
    
    // Configure Direction Control Pin
    pinMode(RS485_DE_RE_PIN, OUTPUT);
    setRS485Mode(false); // Default to Receive Mode
    
    Serial.println("✅ Slave Ready - Listening for commands...");
    Serial.println("========================================\n");
}

void loop() {
    // --- RECEIVE COMMAND PHASE ---
    if (RS485_SERIAL.available()) {
        String command = RS485_SERIAL.readStringUntil('\n');
        command.trim();
        
        if (!command.isEmpty()) {
            Serial.print("📥 Slave Received: ");
            Serial.println(command);
            
            // Process command and generate response
            String response;
            
            if (command.startsWith("CMD:READ_DATA")) {
                String sensorData = generateSensorData();
                response = "ACK|" + sensorData + "|STAT:OK";
                Serial.println("📤 Slave Responding: " + response);
            } 
            else if (command.startsWith("CMD:PING")) {
                response = "ACK|PONG|STAT:OK";
                Serial.println("📤 Slave Responding: " + response);
            }
            else {
                response = "NACK|ERR:UNKNOWN_CMD";
                Serial.println("⚠️ Unknown Command - Sending NACK");
            }
            
            // Send response back to Master
            setRS485Mode(true); // Switch to Transmit Mode
            RS485_SERIAL.println(response);
            RS485_SERIAL.flush();
            setRS485Mode(false); // Switch back to Receive Mode
            
            Serial.println();
        }
    }
}
