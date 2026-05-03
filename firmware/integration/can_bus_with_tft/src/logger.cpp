#include "logger.h"

CANLogger::CANLogger(const char* filepath) : _path(filepath) {}

bool CANLogger::begin() {
    if (!LittleFS.begin(true)) {
        Serial.println("️ LittleFS Mount Failed");
        return false;
    }
    
    _file = LittleFS.open(_path, FILE_APPEND);
    if (!_file) {
        Serial.println("❌ Failed to open log file");
        return false;
    }
    
    if (_file.size() == 0) writeHeader();
    return true;
}

void CANLogger::writeHeader() {
    _file.println(F("timestamp_ms,id,dlc,d0,d1,d2,d3,d4,d5,d6,d7"));
    _file.flush();
}

void CANLogger::logFrame(const MCP2515Driver::CANFrame& frame, uint32_t timestamp) {
    if (!_file) return;
    
    _file.printf("%lu,0x%03X,%d", timestamp, frame.id, frame.dlc);
    for (uint8_t i = 0; i < frame.dlc; i++) {
        _file.printf(",%02X", frame.data[i]);
    }
    for (uint8_t i = frame.dlc; i < 8; i++) {
        _file.print(F(",--"));
    }
    _file.println();
    _file.flush();
}

void CANLogger::close() {
    if (_file) _file.close();
    LittleFS.end();
}