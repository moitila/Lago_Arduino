#ifndef CONFIGDATA_H
#define CONFIGDATA_H

struct ConfigData {
    int distMaximaAguaLago;
    int distMinimaAguaLago;
    int distMaximaAguaFiltro;
    int distMinimaAguaFiltro;
    int ultrasonicFailLimit;
    unsigned long ultrasonicReadInterval;
    unsigned long pumpDelay;
    bool logLigado;
};

#endif
