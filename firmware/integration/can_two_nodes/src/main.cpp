#include <Arduino.h>
#include <SPI.h>

// ── CONFIGURATION ─────────────────────────────────────
#define NODE_ROLE TX  // Ubah ke RX untuk node penerima
#define MCP_CS 5
#define CAN_BAUDRATE_KBPS 500
#define CRYSTAL_FREQ_HZ 8000000UL // 8MHz

// ── MCP2515 Registers ────────────────────────────────
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
#define CANINTF  0x2C
#define EFLG     0x2D

// ── SPI Helpers ──────────────────────────────────────
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

bool mcp2515_init() {
  // Reset
  digitalWrite(MCP_CS, LOW); SPI.transfer(0xC0); digitalWrite(MCP_CS, HIGH);
  delay(50);
  
  // Config Mode
  writeReg(CANCTRL, 0x80);
  delay(10);
  
  // Baudrate 500kbps @ 8MHz
  writeReg(CNF1, 0x00);
  writeReg(CNF2, 0x90);
  writeReg(CNF3, 0x02);
  
  // Disable Filters (Receive All)
  writeReg(RXB0CTRL, 0x60);
  writeReg(TXB0CTRL, 0x00);
  writeReg(CANINTF, 0x00);
  
  // Normal Mode
  writeReg(CANCTRL, 0x00);
  delay(10);
  
  return (readReg(CANSTAT) & 0xE0) == 0x00;
}

void sendCanFrame(...) {
  // ... (write registers) ...
  writeReg(TXB0CTRL, 0x08); // TXREQ
  
  // ✅ Tambah: Tunggu sampai TXREQ clear atau timeout
  for(int i=0; i<100; i++) {
    if ((readReg(TXB0CTRL) & 0x08) == 0) break; // TXREQ clear = sent
    delayMicroseconds(100);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\n==========================================");
  Serial.printf(" CAN Two-Node Integration Test | ROLE: %s\n", 
                (NODE_ROLE == TX) ? "TRANSMITTER" : "RECEIVER");
  Serial.println("==========================================");

  pinMode(MCP_CS, OUTPUT);
  digitalWrite(MCP_CS, HIGH);

  SPI.begin(18, 19, 23, MCP_CS);
  SPI.setFrequency(250000); // 250kHz for stability on jumper wires
  SPI.setDataMode(SPI_MODE0);

  Serial.print("Initializing MCP2515... ");
  if (mcp2515_init()) Serial.println("✅ OK");
  else { Serial.println("❌ FAIL"); while(1); }

  if (NODE_ROLE == TX) {
    Serial.println("Mode: Periodic Transmitter (ID: 0x123)");
  } else {
    Serial.println("Mode: Continuous Receiver (Filter: OFF)");
  }
}

void loop() {
  if (NODE_ROLE == TX) {
    static uint32_t lastTx = 0;
    if (millis() - lastTx > 1000) {
      uint8_t payload[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11};
      sendCanFrame(0x123, 8, payload);
      delay(20);
      
      uint8_t txStat = readReg(TXB0CTRL);
      Serial.printf("[TX] Frame Sent | TXB0CTRL: 0x%02X | EFLG: 0x%02X\n", 
                    txStat, readReg(EFLG));
      lastTx = millis();
    }
  } 
  else if (NODE_ROLE == RX) {
    uint8_t intFlags = readReg(CANINTF);
    if (intFlags & 0x01) { // RX0IF
      uint8_t rxSidh = readReg(RXB0SIDH);
      uint8_t rxSidl = readReg(RXB0SIDL);
      uint16_t rxId = ((rxSidh << 3) | (rxSidl >> 5)) & 0x7FF;
      uint8_t dlc = readReg(RXB0DLC) & 0x0F;
      
      Serial.printf("[RX] ID: 0x%03X | DLC: %d | Data: ", rxId, dlc);
      for(uint8_t i=0; i<dlc; i++) {
        Serial.printf("%02X ", readReg(RXB0D0 + i));
      }
      Serial.printf("| EFLG: 0x%02X\n", readReg(EFLG));
      
      // Clear RX0IF
      writeReg(CANINTF, 0xFE);
    }
    
    // Monitor bus errors
    static uint32_t lastCheck = 0;
    if (millis() - lastCheck > 2000) {
      uint8_t eflg = readReg(EFLG);
      if (eflg & 0xC0) Serial.printf("⚠️ Bus Error Warning | EFLG: 0x%02X\n", eflg);
      lastCheck = millis();
    }
  }
}