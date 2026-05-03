#include "tft_ui.h"

TFT_UI::TFT_UI(uint8_t csPin, uint8_t dcPin, uint8_t rstPin) 
    : _tft(csPin, dcPin, rstPin) {}

bool TFT_UI::begin() {
    // initR() returns void di library ini, jadi langsung panggil
    _tft.initR(INITR_144GREENTAB);
    
    _tft.setRotation(1);
    _tft.setTextWrap(false);
    _tft.fillScreen(ST77XX_BLACK);
    
    // Asumsikan berhasil jika tidak crash/halt
    return true; 
}

void TFT_UI::drawStaticLayout() {
    _tft.setCursor(10, 5);
    _tft.setTextColor(ST77XX_CYAN); _tft.setTextSize(1);
    _tft.print("CAN BUS MONITOR");
    _tft.drawFastHLine(0, 14, 128, ST77XX_WHITE);
    
    _tft.setCursor(5, 22); _tft.setTextColor(ST77XX_YELLOW); _tft.print("ID:");
    _tft.setCursor(5, 36); _tft.setTextColor(ST77XX_GREEN); _tft.print("DATA:");
    _tft.setCursor(5, 50); _tft.setTextColor(ST77XX_ORANGE); _tft.print("COUNT:");
    _tft.setCursor(5, 64); _tft.setTextColor(ST77XX_MAGENTA); _tft.print("STATUS:");
    _tft.drawFastHLine(0, 78, 128, ST77XX_BLUE);
}

void TFT_UI::printHex(uint8_t val) {
    if (val < 0x10) _tft.print('0');
    _tft.print(val, HEX);
}

void TFT_UI::updateFrame(uint16_t id, uint8_t dlc, uint8_t* data, uint32_t count, bool error) {
    // ID
    _tft.fillRect(30, 22, 100, 12, ST77XX_BLACK);
    _tft.setCursor(30, 22); _tft.setTextColor(ST77XX_WHITE);
    _tft.print("0x");
    if (id < 0x10) _tft.print('0');
    if (id < 0x100) _tft.print('0');
    _tft.print(id, HEX);
    
    // DATA (Max 4 bytes + "...")
    _tft.fillRect(35, 36, 93, 12, ST77XX_BLACK);
    _tft.setCursor(35, 36); _tft.setTextColor(ST77XX_WHITE);
    uint8_t show = (dlc > 4) ? 4 : dlc;
    for (uint8_t i = 0; i < show; i++) {
        printHex(data[i]); _tft.print(' ');
    }
    if (dlc > 4) _tft.print("...");
    
    // COUNT
    _tft.fillRect(42, 50, 86, 12, ST77XX_BLACK);
    _tft.setCursor(42, 50); _tft.setTextColor(ST77XX_WHITE);
    _tft.println(count);
    
    // STATUS
    _tft.fillRect(42, 64, 86, 12, ST77XX_BLACK);
    _tft.setCursor(42, 64);
    _tft.setTextColor(error ? ST77XX_RED : ST77XX_GREEN);
    _tft.println(error ? "BUS ERROR!" : "ACTIVE OK");
}