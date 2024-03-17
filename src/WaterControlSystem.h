
#ifndef WATERCONTROLSYSTEM_H
#define WATERCONTROLSYSTEM_H

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

private:
    WaterLevelSensor sensorLago;
    WaterLevelSensor sensorFiltro;
    WaterPump bombaLago;
    WaterPump bombaFiltro;
    bool debug;
    void updateStatusBombas(int statusSistema);
    int getStatusSistema(int statusLago, int statusFiltro);
    void sendInfoSerialDebug(float lagoonLevel, float filterLevel);
};

#endif // WATERCONTROLSYSTEM_H
