#ifndef EXTERNALCOM_H
#define EXTERNALCOM_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WaterControlSystem.h"
#include "LogManager.h"

class ExternalCom {
    public:
        ExternalCom(SoftwareSerial &ecSerial, WaterControlSystem &waterControl, LogManager &logger);
        void run();
        void setWaterSystem(WaterControlSystem &waterSystem);
        
    private:
        SoftwareSerial &ecSerial;
        WaterControlSystem &waterControl;
        LogManager &logger;

        void interpretMessage(String mensagem);
        bool hasMessage();
        void sendMessage(String message);
};

#endif
/*
caso mensagem recebida não corresponda a algum comando abaixo, retornar "#CMD std error <mensagem_recebida>"
comandos a serem recebidos
1 - "#CMD get status" deve enviar "#CMD rsp status <JSON_DATA>" 
2 - "#CMD set config <JSON_config>" deve enviar "#CMD ack config" caso <JSON_config> com erro, retornar "#CMD std JSON_error <MENSAGEM_ERRO>"



<mensagem_recebida> = mensagem recebida pelo serial, vinda do ESP01
<MENSAGEM_ERRO> = erro proveniente da validação do JSON
*/