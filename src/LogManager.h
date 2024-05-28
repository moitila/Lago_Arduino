#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <Arduino.h>

class LogManager {
public:
    LogManager();
    void log(String message);
    void warning(String message);
    void setLogStatus(bool status);
    bool getLogStatus();

private:
    bool logEnabled;
};

#endif
