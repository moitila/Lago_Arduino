
#include "WaterControlSystem.h"
#include <ArduinoJson.h>
#include "LogManager.h"


WaterControlSystem::WaterControlSystem(int pinTrigLago, int pinEchoLago, int pinTrigFiltro, int pinEchoFiltro, 
                                        int pinBombaLago, int pinBombaFiltro, LogManager &logger) : 
                                        sensorLago(pinTrigLago, pinEchoLago,"Lago ", logger), 
                                        sensorFiltro(pinTrigFiltro, pinEchoFiltro,"Filtro ", logger), 
                                        bombaLago(pinBombaLago,5000), 
                                        bombaFiltro(pinBombaFiltro,5000), logger(logger) {
}
void WaterControlSystem::setDistanciaMaximaParaAguaLago(unsigned long distancia) { sensorLago.setDistanciaMaximaParaAgua(distancia);}
void WaterControlSystem::setDistanciaMinimaParaAguaLago(unsigned long distancia) { sensorLago.setDistanciaMinimaParaAgua(distancia);}
void WaterControlSystem::setDistanciaMaximaParaAguaFiltro(unsigned long distancia) { sensorFiltro.setDistanciaMaximaParaAgua(distancia);}
void WaterControlSystem::setDistanciaMinimaParaAguaFiltro(unsigned long distancia) { sensorFiltro.setDistanciaMinimaParaAgua(distancia);}

void WaterControlSystem::updateStatusBombas(int statusSistema)
  {
    switch (statusSistema)
    {
    case 1:
      bombaLago.stop();
      bombaFiltro.stop();
      break;
    case 2:
      bombaLago.stop();
      bombaFiltro.start();
      break;
    case 3:
      bombaLago.start();
      bombaFiltro.stop();
      break;
    case 4:
      bombaLago.start();
      bombaFiltro.start();
      break;

    default:
      bombaLago.stop();
      bombaFiltro.stop();
      break;
    }
  }
void WaterControlSystem::run() {

    float lagoonLevel = sensorLago.getWaterLevel();
    float filterLevel = sensorFiltro.getWaterLevel();

    logger.log("Nivel Lago: " + String(lagoonLevel, 2)); 
    logger.log("Nivel Filtro: " + String(filterLevel, 2));

    updateStatusBombas(getStatusSistema(sensorLago.getWaterLevelStatus(), sensorFiltro.getWaterLevelStatus()));
}
StatusSistema WaterControlSystem::getStatusSistema(WaterLevelStatus statusLago, WaterLevelStatus statusFiltro) {
    if (statusLago == OK && statusFiltro == OK) {
        return BOMBAS_DESLIGADAS; // Níveis de água estão em condições ideais
    } else if (statusLago == EMPTY && statusFiltro == FULL) {
        return APENAS_FILTRO; // Necessidade de transferir água do filtro para o lago
    } else if (statusLago == FULL && statusFiltro == EMPTY) {
        return APENAS_LAGO; // Necessidade de transferir água do lago para o filtro
    } else if (statusLago == EMPTY || statusFiltro == EMPTY) {
        return AMBAS_LIGADAS; // Necessidade de ativação de ambas as bombas para correção de nível
    } else {
        return ERRO; // Alguma condição não atendida ou erro de leitura
    }
}

String WaterControlSystem::getJsonStatus(){
    JsonDocument doc; //usar o Assistant do ArduinoJson para estimar o tamanho necessário.

    doc["lakeLevel"] = sensorLago.getWaterLevel();
    doc["distMinimaAguaLago"] = sensorLago.getDistanciaMinimaParaAgua();
    doc["distMaximaAguaLago"] = sensorLago.getDistanciaMaximaParaAgua();
    doc["filterLevel"] = sensorFiltro.getWaterLevel();
    doc["distMinimaAguaFiltro"] = sensorFiltro.getDistanciaMinimaParaAgua();
    doc["distMaximaAguaFiltro"] = sensorFiltro.getDistanciaMaximaParaAgua();
    doc["pumpLakeStatus"] = bombaLago.getStatus();
    doc["pumpFilterStatus"] = bombaFiltro.getStatus();
    String output;
    doc.shrinkToFit();
    serializeJson(doc, output);
    return output;
}
DeserializationError WaterControlSystem::setSystemConfig(const String &jsonConfig)
{
  JsonDocument doc; // Ajustar o tamanho
  DeserializationError error = deserializeJson(doc, jsonConfig);
  if (error)  { return error;  }
  unsigned long distMinimaAguaLago;
  unsigned long distMaximaAguaLago;
  unsigned long distMinimaAguaFiltro;
  unsigned long distMaximaAguaFiltro;
  unsigned long ultrasonicFailLimit;
  unsigned long pumpDelay;
  unsigned long ultrasonicReadInterval;

  if (doc.containsKey("distMinimaAguaLago")) { distMinimaAguaLago = doc["distMinimaAguaLago"];  }
  if (doc.containsKey("distMaximaAguaLago")) { distMaximaAguaLago = doc["distMaximaAguaLago"];  }
  if (doc.containsKey("distMinimaAguaFiltro")) { distMinimaAguaFiltro = doc["distMinimaAguaFiltro"];  }
  if (doc.containsKey("distMaximaAguaFiltro")) { distMaximaAguaFiltro = doc["distMaximaAguaFiltro"];  }
  if (doc.containsKey("ultrasonicFailLimit"))  { ultrasonicFailLimit = doc["ultrasonicFailLimit"];  }
  if (doc.containsKey("pumpDelay"))  { pumpDelay = doc["pumpDelay"]; }
  if (doc.containsKey("ultrasonicReadInterval"))  { ultrasonicReadInterval = doc["ultrasonicReadInterval"]; }

  sensorLago.setDistanciaMaximaParaAgua(distMaximaAguaLago);
  sensorLago.setDistanciaMinimaParaAgua(distMinimaAguaLago);
  sensorFiltro.setDistanciaMaximaParaAgua(distMaximaAguaFiltro);
  sensorFiltro.setDistanciaMinimaParaAgua(distMinimaAguaFiltro);
  sensorLago.setLimitFailures(ultrasonicFailLimit);
  sensorFiltro.setLimitFailures(ultrasonicFailLimit);
  bombaFiltro.setDelayTime(pumpDelay);
  bombaLago.setDelayTime(pumpDelay);
  this->setSensorReadInterval(ultrasonicReadInterval);

  return DeserializationError::Ok;
}
unsigned long WaterControlSystem::getSensorReadInterval(){
  return this->sensorReadInterval;
}
void WaterControlSystem::setSensorReadInterval(unsigned long interval){
  this->sensorReadInterval = interval;
}
