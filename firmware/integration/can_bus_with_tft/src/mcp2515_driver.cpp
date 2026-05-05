#include "mcp2515_driver.h"
#include <SPI.h>

MCP2515Driver::MCP2515Driver(uint8_t csPin) : _csPin(csPin) {}

void MCP2515Driver::select()   { digitalWrite(_csPin, LOW); }
void MCP2515Driver::deselect() { digitalWrite(_csPin, HIGH); }

// ✅ Helper: Bulk read untuk register contiguous (lebih cepat & stabil)
void MCP2515Driver::readRegisters(uint8_t startAddr, uint8_t* buffer, uint8_t count) {
    SPI.beginTransaction(SPISettings(SPI_FREQ_HZ, MSBFIRST, SPI_MODE0));
    select();
    delayMicroseconds(SPI_STABILIZATION_DELAY_US);
    
    SPI.transfer(0x03); // READ command
    SPI.transfer(startAddr);
    
    for (uint8_t i = 0; i < count; i++) {
        buffer[i] = SPI.transfer(0x00);
    }
    
    deselect();
    SPI.endTransaction();
}

uint8_t MCP2515Driver::readRegister(uint8_t addr) {
    SPI.beginTransaction(SPISettings(SPI_FREQ_HZ, MSBFIRST, SPI_MODE0));
    select();
    
    // ✅ Stabilisasi SPI untuk clone modules
    delayMicroseconds(SPI_STABILIZATION_DELAY_US);
    
    SPI.transfer(0x03); // READ command
    SPI.transfer(addr);
    uint8_t val = SPI.transfer(0x00);
    
    deselect();
    SPI.endTransaction();
    return val;
}

void MCP2515Driver::writeRegister(uint8_t addr, uint8_t val) {
    SPI.beginTransaction(SPISettings(SPI_FREQ_HZ, MSBFIRST, SPI_MODE0));
    select();
    SPI.transfer(0x02); // WRITE command
    SPI.transfer(addr);
    SPI.transfer(val);
    deselect();
    SPI.endTransaction();
}

bool MCP2515Driver::begin() {
    pinMode(_csPin, OUTPUT);
    deselect();
    
    // Hardware Reset
    select(); SPI.transfer(0xC0); deselect();
    delay(MCP_RESET_DELAY_MS);
    
    // Enter Config Mode
    writeRegister(REG_CANCTRL, 0x80);
    delay(MCP_MODE_SWITCH_DELAY_MS);
    
    // Baudrate config (500kbps @ 8MHz)
    writeRegister(REG_CNF1, 0x00);
    writeRegister(REG_CNF2, 0x90);
    writeRegister(REG_CNF3, 0x02);
    
    // Disable Filters (Receive All)
    writeRegister(REG_RXB0CTRL, 0x60);
    writeRegister(REG_CANINTF, 0x00);
    
    // Enter Normal Mode
    writeRegister(REG_CANCTRL, 0x00);
    delay(MCP_MODE_SWITCH_DELAY_MS);
    
    return (readRegister(REG_CANSTAT) & 0xE0) == 0x00;
}

bool MCP2515Driver::receiveFrame(CANFrame &frame) {
    uint8_t intFlags = readRegister(REG_CANINTF);
    if (!(intFlags & 0x01)) return false; // RX0IF not set
    
    // Read ID registers
    uint8_t sidh = readRegister(REG_RXB0SIDH);
    uint8_t sidl_raw = readRegister(REG_RXB0SIDL);
    
    // ✅ Masking: Ambil hanya bit [7:5] untuk ID standard frame (buang SRR/IDE)
    uint8_t sidl = sidl_raw & 0xE0;
    
    // Decode ID 11-bit
    uint16_t candidateId = ((sidh << 3) | (sidl >> 5)) & 0x7FF;
    
    // ✅ VALIDASI: Buang ID yang tidak masuk akal (noise filter)
    if (candidateId == CAN_ID_INVALID_MIN || candidateId == CAN_ID_INVALID_MAX) {
        writeRegister(REG_CANINTF, 0xFE); // Clear RX0IF
        return false;
    }
    
    frame.id = candidateId;
    
    // Read DLC
    frame.dlc = readRegister(REG_RXB0DLC) & 0x0F;
    if (frame.dlc > CAN_DLC_MAX) frame.dlc = CAN_DLC_MAX;
    
    // ✅ Bulk read data bytes (RXB0D0 = 0x66, contiguous)
    if (frame.dlc > 0) {
        readRegisters(REG_RXB0D0, frame.data, frame.dlc);
    }
    
    // Clear RX0IF
    writeRegister(REG_CANINTF, 0xFE);
    return true;
}

uint8_t MCP2515Driver::getErrorFlags() {
    return readRegister(REG_EFLG);
}