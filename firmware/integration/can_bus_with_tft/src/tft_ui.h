#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"

class TFT_UI {
public:
    TFT_UI(uint8_t csPin, uint8_t dcPin, uint8_t rstPin);
    bool begin();
    void drawStaticLayout();
    void updateFrame(uint16_t id, uint8_t dlc, uint8_t* data, uint32_t count, bool error);
    void updateBusHealth(uint8_t eflg, uint32_t errorCount);
    
private:
    Adafruit_ST7735 _tft;
    void printHex(uint8_t val);
};
