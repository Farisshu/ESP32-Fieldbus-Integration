#include <Arduino.h>

// --- Konfigurasi Pin XY-017 RS485 ---
#define RS485_TX_PIN      17    // GPIO 17 -> DI (Driver Input)
#define RS485_RX_PIN      16    // GPIO 16 -> RO (Receiver Output)
#define RS485_DE_RE_PIN   4     // GPIO 4  -> DE/RE (Direction Control)

// --- Konfigurasi Serial ---
#define BAUD_RATE         9600
#define RS485_SERIAL      Serial2

// --- Slave ID ---
#define SLAVE_ID          "01"

// --- Fungsi Helper untuk Mode Transmit/Receive ---
void setRS485Mode(bool transmit) {
  digitalWrite(RS485_DE_RE_PIN, transmit ? HIGH : LOW);
  delayMicroseconds(100); // Delay singkat untuk stabilitas transisi
}

// --- Simulasi Sensor Data ---
float getSimulatedTemperature() {
  // Simulasi sensor suhu dengan variasi kecil
  static float baseTemp = 25.0;
  baseTemp += (random(-5, 6) / 10.0); // Variasi ±0.5°C
  if (baseTemp < 20.0) baseTemp = 20.0;
  if (baseTemp > 30.0) baseTemp = 30.0;
  return baseTemp;
}

float getSimulatedHumidity() {
  // Simulasi sensor kelembaban dengan variasi kecil
  static float baseHum = 60.0;
  baseHum += (random(-10, 11) / 10.0); // Variasi ±1.0%
  if (baseHum < 40.0) baseHum = 40.0;
  if (baseHum > 80.0) baseHum = 80.0;
  return baseHum;
}

void setup() {
  // Inisialisasi random seed
  randomSeed(analogRead(0));
  
  // Inisialisasi Serial Monitor (USB)
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("\n========================================");
  Serial.println("👤 RS485 SLAVE Node Started (ID: " + String(SLAVE_ID) + ")");
  Serial.println("========================================");
  Serial.println("Role: Menerima command dari MASTER dan mengirim response");
  Serial.println("========================================\n");

  // Inisialisasi UART2 untuk RS485
  RS485_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  
  // Setup pin kontrol arah
  pinMode(RS485_DE_RE_PIN, OUTPUT);
  setRS485Mode(false); // Default mode Receive
  
  Serial.println("✅ RS485 Module Initialized. Listening for commands...");
}

void loop() {
  // Cek apakah ada data masuk dari Master
  if (RS485_SERIAL.available()) {
    String command = RS485_SERIAL.readStringUntil('\n');
    command.trim();
    
    if (!command.isEmpty()) {
      Serial.print("📥 [SLAVE RX] Received Command: ");
      Serial.println(command);
      
      // Parse command dan buat response
      String response = "";
      
      if (command.startsWith("CMD:READ_DATA")) {
        // Generate simulated sensor data
        float temp = getSimulatedTemperature();
        float hum = getSimulatedHumidity();
        uint32_t timestamp = millis();
        
        // Format response: ACK|TEMP:xx.x|HUM:xx.x|TS:xxxxx|ID:xx
        response = "ACK|TEMP:" + String(temp, 1) + "|HUM:" + String(hum, 1) + 
                   "|TS:" + String(timestamp) + "|ID:" + String(SLAVE_ID);
        
        Serial.println("   ✅ Processing READ_DATA command...");
      } 
      else if (command.startsWith("CMD:PING")) {
        response = "ACK|PONG|ID:" + String(SLAVE_ID) + "|STAT:OK";
        Serial.println("   ✅ Processing PING command...");
      }
      else if (command.startsWith("CMD:GET_ID")) {
        response = "ACK|SLAVE_ID:" + String(SLAVE_ID) + "|FW:1.0.0";
        Serial.println("   ✅ Processing GET_ID command...");
      }
      else {
        // Command tidak dikenali
        response = "NACK|ERROR:UNKNOWN_CMD|RCVD:" + command.substring(0, 20);
        Serial.println("   ⚠️ Unknown command received");
      }
      
      // Kirim response kembali ke Master
      if (!response.isEmpty()) {
        Serial.print("📤 [SLAVE TX] Sending Response: ");
        Serial.println(response);
        
        // Switch ke Transmit Mode
        setRS485Mode(true);
        
        // Kirim response
        RS485_SERIAL.println(response);
        RS485_SERIAL.flush();
        
        // Kembali ke Receive Mode
        setRS485Mode(false);
        
        Serial.println();
      }
    }
  }
  
  delay(10); // Small delay untuk stabilitas
}
