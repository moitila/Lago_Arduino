#include "LogManager.h"

LogManager::LogManager() : logEnabled(false) {}

void LogManager::log(String message) {
    if (logEnabled) {
        Serial.println("Arduino message :" + message);
    }
}

void LogManager::warning(String message) {
     Serial.println("WARNING Arduino message :" + message);
} 

void LogManager::setLogStatus(bool status) {
    logEnabled = status;
}

bool LogManager::getLogStatus() {
    return logEnabled;
}
