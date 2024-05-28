// WaterLevelSensor.h
#ifndef WATERLEVELSENSOR_H
#define WATERLEVELSENSOR_H

#include <NewPing.h>
#include "LogManager.h"


enum WaterLevelStatus {
    EMPTY = 0,
    OK = 1,
    FULL = 2
};

class WaterLevelSensor {
public:
    WaterLevelSensor(int triggerPin, int echoPin, String nome, unsigned long distMaximaParaAgua, unsigned long distMinimaParaAgua, unsigned int limitfailures, LogManager &logger);
    void updateReadings(long novaLeitura);
    void updateMedia();
    bool verifyReading(unsigned long value);
    void handleAlert();
    void updateStatus();
    WaterLevelStatus getWaterLevelStatus();
    float getWaterLevel();
    float getMediaLeitura();
    void setDistanciaMinimaParaAgua(unsigned long distancia);
    unsigned long getDistanciaMinimaParaAgua() const;
    void setDistanciaMaximaParaAgua(unsigned long distancia);
    unsigned long getDistanciaMaximaParaAgua() const;
    int getStatus();
    void setLimitFailures(unsigned int limit);
    void setNome(String nome);
    String getNome();

private:
    int triggerPin;
    int echoPin;
    String nome;
    unsigned long distanciaMaximaParaAgua;
    unsigned long distanciaMinimaParaAgua;
    float media;
    unsigned long leituras[3];
    unsigned int consecutivefailures;
    unsigned int limitfailures;
    LogManager &logger;
    NewPing sonar;
    WaterLevelStatus status;
    
};

#endif // WATERLEVELSENSOR_H
