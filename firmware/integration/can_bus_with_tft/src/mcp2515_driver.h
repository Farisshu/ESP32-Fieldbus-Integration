#pragma once
#include <Arduino.h>
#include "config.h"

class MCP2515Driver {
public:
    MCP2515Driver(uint8_t csPin);
    bool begin();
    uint8_t readRegister(uint8_t addr);
    void writeRegister(uint8_t addr, uint8_t val);
    
    struct CANFrame {
        uint16_t id;
        uint8_t dlc;
        uint8_t data[8];
    };
    
    bool receiveFrame(CANFrame &frame);
    uint8_t getErrorFlags();

private:
    uint8_t _csPin;
    void select();
    void deselect();
};
