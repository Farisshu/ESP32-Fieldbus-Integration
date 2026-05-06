#include <Arduino.h>

// --- Konfigurasi Pin XY-017 RS485 ---
#define RS485_TX_PIN      17    // GPIO 17 -> DI (Driver Input)
#define RS485_RX_PIN      16    // GPIO 16 -> RO (Receiver Output)
#define RS485_DE_RE_PIN   4     // GPIO 4  -> DE/RE (Direction Control)

// --- Konfigurasi Serial ---
#define BAUD_RATE         9600
#define RS485_SERIAL      Serial2

void setup() {
  // Inisialisasi Serial Monitor (USB)
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("\n========================================");
  Serial.println("🚀 RS485 Loopback Test Started");
  Serial.println("========================================");
  Serial.println("⚠️ PENTING: Pastikan pin A dan B pada modul RS485 di-jumper!");
  Serial.println("   (Hubungkan terminal A ke terminal B dengan kabel pendek)");
  Serial.println("========================================\n");

  // Inisialisasi UART2 untuk RS485
  RS485_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  
  // Setup pin kontrol arah
  pinMode(RS485_DE_RE_PIN, OUTPUT);
  digitalWrite(RS485_DE_RE_PIN, LOW); // Mode Receive (default)
  
  Serial.println("✅ Module Initialized. Starting loopback test...");
  Serial.println("   - Mengirim data setiap 2 detik");
  Serial.println("   - Menerima data dari port yang sama (loopback)\n");
}

void loop() {
  static unsigned long lastSendTime = 0;
  const unsigned long INTERVAL = 2000; // Kirim setiap 2 detik

  // 1. KIRIM DATA (Mode Transmit)
  if (millis() - lastSendTime > INTERVAL) {
    String msg = "LOOPBACK_TEST_ID:" + String(millis());
    
    Serial.print("📤 [TX] Sending: ");
    Serial.println(msg);

    // Aktifkan Driver (Transmit Mode)
    digitalWrite(RS485_DE_RE_PIN, HIGH);
    delayMicroseconds(100); // Delay singkat agar stabil

    // Kirim data
    RS485_SERIAL.println(msg);
    RS485_SERIAL.flush(); // Tunggu sampai selesai kirim

    // Nonaktifkan Driver (kembali ke Receive Mode)
    digitalWrite(RS485_DE_RE_PIN, LOW);
    delayMicroseconds(100);
    
    lastSendTime = millis();
  }

  // 2. TERIMA DATA (Mode Receive)
  if (RS485_SERIAL.available()) {
    String receivedMsg = RS485_SERIAL.readStringUntil('\n');
    receivedMsg.trim();
    
    if (!receivedMsg.isEmpty()) {
      Serial.print("📥 [RX] Received: ");
      Serial.println(receivedMsg);
      
      // Validasi: Cek apakah data yang diterima sama dengan yang dikirim
      if (receivedMsg.startsWith("LOOPBACK_TEST_ID:")) {
        Serial.println("   ✅ SUCCESS: Data valid diterima kembali!\n");
      } else {
        Serial.println("   ⚠️ WARNING: Data tidak sesuai format expected.\n");
      }
    }
  }
  
  delay(10); // Small delay untuk stabilitas
}
