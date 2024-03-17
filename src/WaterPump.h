#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <Arduino.h>

class WaterPump {
public:
    WaterPump(int pumpPin, unsigned long delayTime);
    void start();
    void stop();
    bool getStatus();
    void setDelayTime(unsigned long delay);

private:
    int pumpPin;
    bool isRunning;
    unsigned long lastChangeTime;
    unsigned long delayTime;
};

#endif
