#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <Arduino.h>

class LogManager {
  public:
    LogManager() : loggingEnabled(true) {} 

    void enableLogging(bool enable) {
      loggingEnabled = enable;
    }

    void log(const String &message) {
      if (loggingEnabled) {
        Serial.println("Arduino message :" + message);
      }
    }
    void warning(const String &message){
      Serial.println("WARNING Arduino message :" + message);
    }

  private:
    bool loggingEnabled;
};

#endif
