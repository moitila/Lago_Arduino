
#ifndef WATERCONTROLSYSTEM_H
#define WATERCONTROLSYSTEM_H
#include <ArduinoJson.h>
#include "WaterLevelSensor.h"
#include "WaterPump.h"

enum StatusSistema {
    BOMBAS_DESLIGADAS = 0,
    APENAS_FILTRO = 1,
    APENAS_LAGO = 2,
    AMBAS_LIGADAS = 3,
    ERRO = 4
};

class WaterControlSystem {
public:
    WaterControlSystem(int pinTrigLago, int pinEchoLago, int pinTrigFiltro, int pinEchoFiltro, int pinBombaLago, 
        int pinBombaFiltro, LogManager &logger);
    void run();
    void setDistanciaMaximaParaAguaLago(unsigned long distancia);
    void setDistanciaMinimaParaAguaLago(unsigned long distancia);
    void setDistanciaMaximaParaAguaFiltro(unsigned long distancia);
    void setDistanciaMinimaParaAguaFiltro(unsigned long distancia);
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
    StatusSistema getStatusSistema(WaterLevelStatus statusLago, WaterLevelStatus statusFiltro);
    unsigned long sensorReadInterval;
    LogManager &logger;
};

#endif // WATERCONTROLSYSTEM_H
