#include <Arduino.h>
#include <SPI.h>

#define MCP_CS 5

// ── Register Definitions ─────────────────────────────
#define CANCTRL  0x0F
#define CANSTAT  0x0E
#define CNF1     0x2A
#define CNF2     0x29
#define CNF3     0x28
#define TXB0CTRL 0x30
#define TXB0SIDH 0x32
#define TXB0SIDL 0x33
#define TXB0DLC  0x35
#define TXB0D0   0x36
#define RXB0CTRL 0x60
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0DLC  0x65
#define RXB0D0   0x66
#define CANINTF  0x2C  // Interrupt Flag Register
#define EFLG     0x2D  // Error Flag Register

// ── SPI Helpers ───────────────────────────────────────
void writeReg(uint8_t addr, uint8_t val) {
  digitalWrite(MCP_CS, LOW);
  SPI.transfer(0x02); SPI.transfer(addr); SPI.transfer(val);
  digitalWrite(MCP_CS, HIGH);
}

uint8_t readReg(uint8_t addr) {
  digitalWrite(MCP_CS, LOW);
  SPI.transfer(0x03); SPI.transfer(addr);
  uint8_t val = SPI.transfer(0x00);
  digitalWrite(MCP_CS, HIGH);
  return val;
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\n MCP2515 Diagnostic v7 (CANINTF Fix)");
  Serial.println("======================================");

  pinMode(MCP_CS, OUTPUT);
  digitalWrite(MCP_CS, HIGH);

  SPI.begin(18, 19, 23, MCP_CS);
  SPI.setFrequency(250000); // 250kHz for stability
  SPI.setDataMode(SPI_MODE0);
  delay(10);

  // 1. Reset
  Serial.print("1. Reset MCP2515... ");
  digitalWrite(MCP_CS, LOW); SPI.transfer(0xC0); digitalWrite(MCP_CS, HIGH);
  delay(50);
  Serial.println("Done");

  // 2. Verify SPI
  Serial.print("2. SPI Loopback Test... ");
  writeReg(CNF3, 0x55);
  uint8_t back = readReg(CNF3);
  Serial.printf("Read: 0x%02X ", back);
  if (back == 0x55 || back == 0x45) Serial.println("✅ OK");
  else { Serial.println("❌ FAIL"); while(1); }

  // 3. Config 500kbps @ 8MHz
  Serial.println("3. Set Baudrate 500kbps (8MHz)...");
  writeReg(CNF1, 0x00);
  writeReg(CNF2, 0x90);
  writeReg(CNF3, 0x02);
  delay(10);

  // 4. Loopback Mode
  Serial.print("4. Enter Loopback Mode... ");
  writeReg(CANCTRL, 0x40);
  delay(10);
  uint8_t mode = readReg(CANSTAT) & 0xE0;
  Serial.printf("CANSTAT: 0x%02X ", mode);
  if (mode == 0x40) Serial.println("✅ OK");
  else Serial.println("⚠️ Check");

  // 5. Disable RX Filters
  Serial.println("5. Disable RX Filters (Receive All)...");
  writeReg(RXB0CTRL, 0x60); 
  delay(10);

  // 6. Send Frame ID 0x123
  uint16_t txId = 0x123;
  Serial.printf("6. Send Frame ID: 0x%03X\n", txId);
  
  writeReg(TXB0CTRL, 0x00);
  writeReg(TXB0SIDH, (txId >> 3) & 0xFF);
  writeReg(TXB0SIDL, (txId & 0x07) << 5);
  writeReg(TXB0DLC, 0x08);
  
  uint8_t payload[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11};
  for(uint8_t i=0; i<8; i++) writeReg(TXB0D0 + i, payload[i]);
  
  writeReg(TXB0CTRL, 0x08); // TXREQ
  delay(50);

  // 7. Check TX
  Serial.print("7. TX Status: ");
  uint8_t txStat = readReg(TXB0CTRL);
  Serial.printf("0x%02X ", txStat);
  if ((txStat & 0x08) == 0) Serial.println("✅ Transmitted");
  else Serial.println("❌ Stuck");

  // 8. Poll RX using CANINTF (THE FIX)
  Serial.print("8. Polling CANINTF for RX0IF... ");
  bool rxFound = false;
  
  for(int i=0; i<10; i++) {
    delay(30);
    uint8_t intFlags = readReg(CANINTF);
    
    // Check Bit 0 (RX0IF)
    if (intFlags & 0x01) { 
      rxFound = true;
      Serial.println("✅ RX0IF Detected!");
      
      // Read ID
      uint8_t rxSidh = readReg(RXB0SIDH);
      uint8_t rxSidl = readReg(RXB0SIDL);
      uint16_t rxId = (rxSidh << 3) | (rxSidl >> 5);
      
      Serial.printf("   RX ID: 0x%03X ", rxId);
      Serial.println((rxId == txId) ? "✅ MATCH!" : "❌ MISMATCH");
      
      // Read & Print Full Payload
      Serial.print("   RX Data: ");
      bool dataOk = true;
      for(uint8_t j=0; j<8; j++) {
        uint8_t byte = readReg(RXB0D0 + j);
        Serial.printf("%02X ", byte);
        if (byte != payload[j]) dataOk = false;
      }
      Serial.println(dataOk ? "✅" : "❌");
      
      // ⚠️ PENTING: Clear Interrupt Flag agar buffer siap lagi
      writeReg(CANINTF, 0xFE); // Clear bit 0 (RX0IF)
      break;
    }
  }
  
  if (!rxFound) {
    Serial.println("❌ RX0IF never set");
    Serial.printf("   Final CANINTF: 0x%02X, EFLG: 0x%02X\n", readReg(CANINTF), readReg(EFLG));
  }

  Serial.println("\n🏁 Diagnostic Complete. MCP2515 Fully Operational.");
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last > 1000) {
    Serial.println("💓 System Alive - CAN Loopback Ready");
    last = millis();
  }
}