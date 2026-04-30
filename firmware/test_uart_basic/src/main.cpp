#include <Arduino.h>
#include <SPI.h>

#define CAN_CS_PIN 5
#define LED_PIN 2   // Built-in LED ESP32

// MCP2515 Registers
#define CANCTRL   0x0F
#define CANSTAT   0x0E
#define TXB0CTRL  0x30

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED ON = System running

  delay(1000);
  Serial.println("\n MCP2515 SPI DIAGNOSTIC V2");
  Serial.println("────────────────────────────");

  // Explicit pin setup
  pinMode(CAN_CS_PIN, OUTPUT);
  digitalWrite(CAN_CS_PIN, HIGH);
  pinMode(18, OUTPUT); // SCK
  pinMode(19, INPUT);  // MISO
  pinMode(23, OUTPUT); // MOSI

  // 1. Init SPI
  Serial.print("1. SPI Init (500kHz)... ");
  SPI.begin(18, 19, 23, CAN_CS_PIN);
  SPI.setFrequency(500000); // Lebih aman untuk debug wiring
  SPI.setDataMode(SPI_MODE0);
  Serial.println("OK");

  // 2. Soft Reset
  Serial.print("2. Send Reset (0xC0)... ");
  digitalWrite(CAN_CS_PIN, LOW);
  SPI.transfer(0xC0);
  digitalWrite(CAN_CS_PIN, HIGH);
  delay(20);
  Serial.println("Done");

  // 3. Read CANSTAT (Harusnya 0x80 atau 0xE0 setelah reset)
  Serial.print("3. Read CANSTAT... ");
  uint8_t stat = readReg(CANSTAT);
  Serial.printf("0x%02X ", stat);
  if (stat == 0x80 || stat == 0xE0) Serial.println("(Config Mode) ✅");
  else Serial.println("(Unexpected) ⚠️");

  // 4. Write/Read Loopback Test
  Serial.print("4. Write 0x55 -> Read Back... ");
  writeReg(TXB0CTRL, 0x55);
  delay(5);
  uint8_t back = readReg(TXB0CTRL);
  Serial.printf("Read: 0x%02X ", back);
  
  if (back == 0x55) {
    Serial.println("✅ SPI VERIFIED!");
    digitalWrite(LED_PIN, HIGH); // Steady ON = Success
  } else {
    Serial.println("❌ MISMATCH");
    // Fast blink = fail
    for(int i=0; i<8; i++) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(80);
    }
  }

  Serial.println("\n🏁 DIAGNOSTIC SELESAI");
  Serial.println("LED steady = OK | LED fast blink = FAIL");
}

void loop() {
  // Slow blink = idle after test
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(500);
}

// ── SPI Helpers ─────────────────────────────
void writeReg(uint8_t addr, uint8_t val) {
  digitalWrite(CAN_CS_PIN, LOW);
  SPI.transfer(0x02); // Write cmd
  SPI.transfer(addr);
  SPI.transfer(val);
  digitalWrite(CAN_CS_PIN, HIGH);
}

uint8_t readReg(uint8_t addr) {
  digitalWrite(CAN_CS_PIN, LOW);
  SPI.transfer(0x03); // Read cmd
  SPI.transfer(addr);
  uint8_t val = SPI.transfer(0x00); // Clock out MISO
  digitalWrite(CAN_CS_PIN, HIGH);
  return val;
}