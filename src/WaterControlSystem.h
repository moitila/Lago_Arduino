
#ifndef WATERCONTROLSYSTEM_H
#define WATERCONTROLSYSTEM_H

#include "WaterLevelSensor.h"
#include "WaterPump.h"
#include "LogManager.h"
#include "ConfigData.h"
#include <ArduinoJson.h>

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
        int pinBombaFiltro, LogManager &logger, const ConfigData &config);
    void run();
    String getJsonStatus();
    DeserializationError setSystemConfig(const String &jsonConfig);
    void setSensorReadInterval(unsigned long interval);
    unsigned long getSensorReadInterval();
    void setConfig(int distMaximaAguaLago, int distMinimaAguaLago, int distMaximaAguaFiltro, int distMinimaAguaFiltro, 
        int ultrasonicFailLimit, unsigned long ultrasonicReadInterval, unsigned long pumpDelay, bool logLigado);
    void setDistanciaMaximaParaAguaLago(unsigned long distancia);
    void setDistanciaMinimaParaAguaLago(unsigned long distancia);
    void setDistanciaMaximaParaAguaFiltro(unsigned long distancia);
    void setDistanciaMinimaParaAguaFiltro(unsigned long distancia);

private:
    WaterLevelSensor sensorLago;
    WaterLevelSensor sensorFiltro;
    WaterPump bombaLago;
    WaterPump bombaFiltro;
    LogManager &logger;
    unsigned long sensorReadInterval;
    bool debug;
    void updateStatusBombas(int statusSistema);
    StatusSistema getStatusSistema(WaterLevelStatus statusLago, WaterLevelStatus statusFiltro);
};

#endif // WATERCONTROLSYSTEM_H
