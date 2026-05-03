#include "mcp2515_driver.h"
#include <SPI.h>

MCP2515Driver::MCP2515Driver(uint8_t csPin) : _csPin(csPin) {}

void MCP2515Driver::select()   { digitalWrite(_csPin, LOW); }
void MCP2515Driver::deselect() { digitalWrite(_csPin, HIGH); }

uint8_t MCP2515Driver::readRegister(uint8_t addr) {
    SPI.beginTransaction(SPISettings(SPI_FREQ_HZ, MSBFIRST, SPI_MODE0));
    select();
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
    delay(50);
    
    // Enter Config Mode
    writeRegister(REG_CANCTRL, 0x80);
    delay(10);
    
    // Baudrate 500kbps @ 8MHz Crystal
    writeRegister(REG_CNF1, 0x00);
    writeRegister(REG_CNF2, 0x90);
    writeRegister(REG_CNF3, 0x02);
    
    // Disable Filters (Receive All)
    writeRegister(REG_RXB0CTRL, 0x60);
    writeRegister(REG_CANINTF, 0x00);
    
    // Enter Normal Mode
    writeRegister(REG_CANCTRL, 0x00);
    delay(10);
    
    return (readRegister(REG_CANSTAT) & 0xE0) == 0x00;
}

bool MCP2515Driver::receiveFrame(CANFrame &frame) {
    uint8_t intFlags = readRegister(REG_CANINTF);
    if (!(intFlags & 0x01)) return false; // RX0IF not set
    
    uint8_t sidh = readRegister(REG_RXB0SIDH);
    uint8_t sidl = readRegister(REG_RXB0SIDL);
    frame.id = ((sidh << 3) | (sidl >> 5)) & 0x7FF;
    
    frame.dlc = readRegister(REG_RXB0DLC) & 0x0F;
    if (frame.dlc > 8) frame.dlc = 8;
    
    for (uint8_t i = 0; i < frame.dlc; i++) {
        frame.data[i] = readRegister(REG_RXB0D0 + i);
    }
    
    // Clear RX0IF
    writeRegister(REG_CANINTF, 0xFE);
    return true;
}

uint8_t MCP2515Driver::getErrorFlags() {
    return readRegister(REG_EFLG);
}