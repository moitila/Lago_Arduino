// WaterLevelSensor.h
#ifndef WATERLEVELSENSOR_H
#define WATERLEVELSENSOR_H

#include <NewPing.h>

enum WaterLevelStatus {
    EMPTY = 0,
    OK = 1,
    FULL = 2
};

class WaterLevelSensor {
public:
    WaterLevelSensor(int triggerPin, int echoPin);
    float getWaterLevel();
    float getMediaLeitura();
    void setDistanciaMinimaParaAgua(unsigned long distancia);
    unsigned long getDistanciaMinimaParaAgua() const;
    void setDistanciaMaximaParaAgua(unsigned long distancia);
    unsigned long getDistanciaMaximaParaAgua() const;
    void setLimitFailures(unsigned int limit);
    int getStatus();

private:
    WaterLevelStatus status;
    unsigned long distanciaMaximaParaAgua;
    unsigned long distanciaMinimaParaAgua;
    float media;
    unsigned int consecutivefailures;
    const unsigned int limitfailures;
    NewPing sonar;
    long leituras[3];

    void updateReadings(long novaLeitura);
    void updateMedia();
    bool verifyReading(unsigned long value);
    void handleAlert();
    void updateStatus();
};

#endif // WATERLEVELSENSOR_H
