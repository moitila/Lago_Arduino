#ifndef CONFIGDATA_H
#define CONFIGDATA_H

struct ConfigData {
    unsigned int distMaximaAguaLago;
    unsigned int distMinimaAguaLago;
    unsigned int distMaximaAguaFiltro;
    unsigned int distMinimaAguaFiltro;
    int ultrasonicFailLimit;
    unsigned long ultrasonicReadInterval;
    unsigned long pumpDelay;
    bool logLigado;
};

#endif

