#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include "mcp2515_driver.h"

class CANLogger {
public:
    CANLogger(const char* filepath = "/can_log.csv");
    bool begin();
    void logFrame(const MCP2515Driver::CANFrame& frame, uint32_t timestamp);
    void close();

private:
    File _file;
    const char* _path;
    void writeHeader();
};

#endif // LOGGER_H