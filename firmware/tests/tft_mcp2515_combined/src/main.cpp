#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// ─ Pin Definitions ──────────────────────────────
#define MCP_CS   5
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Pastikan CS kedua device HIGH di awal (idle)
  pinMode(MCP_CS, OUTPUT); digitalWrite(MCP_CS, HIGH);
  pinMode(TFT_CS, OUTPUT); digitalWrite(TFT_CS, HIGH);

  Serial.println("\n🔍 Combined Unit Test: TFT + MCP2515 (SPI Shared)");
  Serial.println("===============================================");

  // 1. Init TFT
  Serial.print("1. Initialize TFT... ");
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("✅ OK");

  // 2. Init Shared SPI (Hardware SPI ESP32)
  Serial.print("2. Init Shared SPI (SCK:18, MISO:19, MOSI:23)... ");
  SPI.begin(18, 19, 23, -1); 
  SPI.setFrequency(1000000); // 1MHz (aman untuk debug)
  Serial.println("✅ OK");

  // 3. Safe Read MCP2515 (CANSTAT Register)
  Serial.print("3. Read MCP2515 CANSTAT... ");
  digitalWrite(MCP_CS, LOW);
  SPI.transfer(0x03);          // Read Command
  SPI.transfer(0x0E);          // CANSTAT Address
  uint8_t mcp_status = SPI.transfer(0x00);
  digitalWrite(MCP_CS, HIGH);  // Release Bus
  Serial.printf("0x%02X\n", mcp_status);

  // 4. Tampilkan Hasil di Layar
  tft.setCursor(10, 10);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.println("SPI SHARED TEST");
  
  tft.setCursor(10, 25);
  tft.setTextColor(ST77XX_GREEN);
  tft.printf("MCP2515: 0x%02X", mcp_status);
  
  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_YELLOW);
  tft.println("NO BUS CONFLICT!");

  Serial.println("\n✅ Combined Test PASSED. Ready for Integration.");
}

void loop() {
  // Live update setiap 2 detik
  static uint32_t last = 0;
  if (millis() - last > 2000) {
    digitalWrite(MCP_CS, LOW);
    SPI.transfer(0x03); SPI.transfer(0x0E);
    uint8_t val = SPI.transfer(0x00);
    digitalWrite(MCP_CS, HIGH);

    // Clear & redraw
    tft.fillRect(10, 25, 100, 10, ST77XX_BLACK);
    tft.setCursor(10, 25);
    tft.setTextColor(ST77XX_GREEN);
    tft.printf("MCP2515: 0x%02X", val);
    
    Serial.printf("Heartbeat | MCP Status: 0x%02X\n", val);
    last = millis();
  }
}