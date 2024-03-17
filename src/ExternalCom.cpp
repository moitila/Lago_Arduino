#include "ExternalCom.h"


ExternalCom::ExternalCom(SoftwareSerial &ecSerial, WaterControlSystem &waterControl, LogManager &logger)
: ecSerial(ecSerial), waterControl(waterControl), logger(logger) {
    // Seu código de inicialização aqui, se necessário
}
bool ExternalCom::hasMessage() {
    return ecSerial.available() > 0;
}
void ExternalCom::run(){
    if (this->hasMessage()){
        String mensagem = ecSerial.readStringUntil('\n');
        interpretMessage(mensagem);
    }
};
void ExternalCom::interpretMessage(String mensagem) {
if (mensagem.startsWith("#CMD get status")) {
        String jsonStatus = waterControl.getJsonStatus();
        sendMessage("#CMD rsp status " + jsonStatus);
    } else if (mensagem.startsWith("#CMD set config ")) {
        String jsonConfig = mensagem.substring(17);
        DeserializationError erro = waterControl.setSystemConfig(jsonConfig);
        if (erro == DeserializationError::Ok) {
            sendMessage("#CMD ack config");
        } else {
            sendMessage("#CMD std JSON_error "+ String(erro.c_str()));
        }
    } else {
        // Se o comando recebido não corresponder a nenhum esperado
        ecSerial.println("#CMD std error " + mensagem);
    }
}

void ExternalCom::sendMessage(String message){
    ecSerial.print(message);
    logger.log(message);
}

