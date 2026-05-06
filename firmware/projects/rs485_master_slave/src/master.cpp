#include <Arduino.h>

// --- Konfigurasi Pin XY-017 RS485 ---
#define RS485_TX_PIN      17    // GPIO 17 -> DI (Driver Input)
#define RS485_RX_PIN      16    // GPIO 16 -> RO (Receiver Output)
#define RS485_DE_RE_PIN   4     // GPIO 4  -> DE/RE (Direction Control)

// --- Konfigurasi Serial ---
#define BAUD_RATE         9600
#define RS485_SERIAL      Serial2

// --- Fungsi Helper untuk Mode Transmit/Receive ---
void setRS485Mode(bool transmit) {
  digitalWrite(RS485_DE_RE_PIN, transmit ? HIGH : LOW);
  delayMicroseconds(100); // Delay singkat untuk stabilitas transisi
}

void setup() {
  // Inisialisasi Serial Monitor (USB)
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("\n========================================");
  Serial.println("🤖 RS485 MASTER Node Started");
  Serial.println("========================================");
  Serial.println("Role: Mengirim command dan menerima response dari SLAVE");
  Serial.println("========================================\n");

  // Inisialisasi UART2 untuk RS485
  RS485_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  
  // Setup pin kontrol arah
  pinMode(RS485_DE_RE_PIN, OUTPUT);
  setRS485Mode(false); // Default mode Receive
  
  Serial.println("✅ RS485 Module Initialized. Waiting to send commands...");
}

void loop() {
  static unsigned long lastCmdTime = 0;
  const unsigned long CMD_INTERVAL = 3000; // Kirim command setiap 3 detik
  
  // 1. KIRIM COMMAND ke Slave
  if (millis() - lastCmdTime > CMD_INTERVAL) {
    String command = "CMD:READ_DATA|TS:" + String(millis());
    
    Serial.print("📤 [MASTER TX] Sending Command: ");
    Serial.println(command);
    
    // Switch ke Transmit Mode
    setRS485Mode(true);
    
    // Kirim command
    RS485_SERIAL.println(command);
    RS485_SERIAL.flush();
    
    // Kembali ke Receive Mode
    setRS485Mode(false);
    
    lastCmdTime = millis();
  }

  // 2. TERIMA RESPONSE dari Slave
  if (RS485_SERIAL.available()) {
    String response = RS485_SERIAL.readStringUntil('\n');
    response.trim();
    
    if (!response.isEmpty()) {
      Serial.print("📥 [MASTER RX] Received Response: ");
      Serial.println(response);
      
      // Parse response sederhana
      if (response.startsWith("ACK|")) {
        Serial.println("   ✅ SUCCESS: Valid response from SLAVE received!");
        
        // Extract data contoh
        int tempPos = response.indexOf("TEMP:");
        if (tempPos != -1) {
          String tempVal = response.substring(tempPos + 5);
          int commaPos = tempVal.indexOf('|');
          if (commaPos != -1) {
            tempVal = tempVal.substring(0, commaPos);
          }
          Serial.print("   🌡️ Temperature Data: ");
          Serial.println(tempVal);
        }
      } else if (response.startsWith("NACK|")) {
        Serial.println("   ⚠️ ERROR: Slave returned NACK (command error)");
      } else {
        Serial.println("   ⚠️ WARNING: Unknown response format");
      }
      Serial.println();
    }
  }
  
  delay(10); // Small delay untuk stabilitas
}
