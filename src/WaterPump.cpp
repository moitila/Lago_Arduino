#include "WaterPump.h"

WaterPump::WaterPump(int pumpPin, unsigned long delayTime)
: pumpPin(pumpPin), delayTime(delayTime), isRunning(false), lastChangeTime(0) {
    pinMode(pumpPin, OUTPUT);
    this->stop();
}

void WaterPump::start() {
    unsigned long currentTime = millis();
    if ((currentTime - lastChangeTime >= delayTime) && !isRunning) {
        digitalWrite(pumpPin, HIGH);
        isRunning = true;
        lastChangeTime = currentTime;
    }
}

void WaterPump::stop() {
    unsigned long currentTime = millis();
    if ((currentTime - lastChangeTime >= delayTime) && isRunning) {
        digitalWrite(pumpPin, LOW);
        isRunning = false;
        lastChangeTime = currentTime;
    }
}
