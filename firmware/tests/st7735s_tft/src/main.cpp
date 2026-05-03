#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// TFT Pins (separate CS dari MCP2515)
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  4
#define TFT_BLK  -1  // Connect to 3.3V/5V directly

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("🔍 ST7735S TFT Test Started");
  
  // Init TFT (128x128, rotated)
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  
  Serial.println("✅ Display Initialized");
  
  // Test 1: Clear screen dengan warna berbeda
  tft.fillScreen(ST77XX_RED);
  delay(500);
  tft.fillScreen(ST77XX_GREEN);
  delay(500);
  tft.fillScreen(ST77XX_BLUE);
  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  
  // Test 2: Draw text
  tft.setCursor(10, 10);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.println("ST7735S OK");
  
  tft.setCursor(10, 30);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.println("SPI Shared");
  
  Serial.println("✅ All tests passed!");
}

void loop() {
  // Blink pixel di tengah
  static bool state = false;
  tft.drawPixel(64, 64, state ? ST77XX_WHITE : ST77XX_BLACK);
  state = !state;
  delay(500);
}