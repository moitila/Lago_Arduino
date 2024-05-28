#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <Arduino.h>
#include "LogManager.h"

class WaterPump {
public:
    WaterPump(const char* pumpName, int pumpPin, unsigned long delayTime, LogManager &logger);
    void start();
    void stop();
    bool getStatus();
    void setDelayTime(unsigned long delay);

private:
    const char* pumpName;
    int pumpPin;
    unsigned long delayTime;
    bool isRunning;
    unsigned long lastChangeTime;
    LogManager &logger;
};

#endif
