
#ifndef WATERCONTROLSYSTEM_H
#define WATERCONTROLSYSTEM_H
#include <ArduinoJson.h>
#include "WaterLevelSensor.h"
#include "WaterPump.h"

class WaterControlSystem {
public:
    WaterControlSystem(int pinTrigLago, int pinEchoLago, int pinTrigFiltro, int pinEchoFiltro, int pinBombaLago, int pinBombaFiltro);
    void run();
    void setMinLakeLevel(unsigned long minLevel);
    void setMaxLakeLevel(unsigned long maxLevel);
    void setMinFilterLevel(unsigned long minLevel);
    void setMaxFilterLevel(unsigned long maxLevel);
    unsigned long getSensorReadInterval();
    void setSensorReadInterval(unsigned long interval);
    String getJsonStatus();
    DeserializationError setSystemConfig(const String &jsonConfig);

private:
    WaterLevelSensor sensorLago;
    WaterLevelSensor sensorFiltro;
    WaterPump bombaLago;
    WaterPump bombaFiltro;
    bool debug;
    void updateStatusBombas(int statusSistema);
    int getStatusSistema(int statusLago, int statusFiltro);
    void sendInfoSerialDebug(float lagoonLevel, float filterLevel);
    unsigned long sensorReadInterval;
};

#endif // WATERCONTROLSYSTEM_H
