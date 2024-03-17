#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <Arduino.h>

class WaterPump {
public:
    WaterPump(int pumpPin, unsigned long delayTime);
    void start();
    void stop();

private:
    int pumpPin;
    bool isRunning;
    unsigned long lastChangeTime;
    unsigned long delayTime;
};

#endif
