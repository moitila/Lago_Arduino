#include "WaterPump.h"

WaterPump::WaterPump(const char* pumpName, int pumpPin, unsigned long delayTime, LogManager &logger)
: pumpName(pumpName), pumpPin(pumpPin), delayTime(delayTime), isRunning(false), lastChangeTime(0), logger(logger) {
    pinMode(pumpPin, OUTPUT);
    this->stop();
}

void WaterPump::start() {
    unsigned long currentTime = millis();
    if ((currentTime - lastChangeTime >= delayTime) && !isRunning) {
        digitalWrite(pumpPin, HIGH);
        isRunning = true;
        lastChangeTime = currentTime;
        logger.log(String(pumpName) + " ligou.");
    }
}

void WaterPump::stop() {
    unsigned long currentTime = millis();
    if ((currentTime - lastChangeTime >= delayTime) && isRunning) {
        digitalWrite(pumpPin, LOW);
        isRunning = false;
        lastChangeTime = currentTime;
        logger.log(String(pumpName) + " desligou.");
    }
}

bool WaterPump::getStatus() {
    return isRunning;
}

void WaterPump::setDelayTime(unsigned long delay) {
    delayTime = delay;
}
