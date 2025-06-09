
#include "WaterControlSystem.h"
#include <ArduinoJson.h>
#include "LogManager.h"


WaterControlSystem::WaterControlSystem(int pinTrigLago, int pinEchoLago, int pinTrigFiltro, int pinEchoFiltro, int pinBombaLago, int pinBombaFiltro, LogManager &logger, const ConfigData &config)
: sensorLago(pinTrigLago, pinEchoLago, "Sensor Lago", config.distMaximaAguaLago, config.distMinimaAguaLago, config.ultrasonicFailLimit, logger), 
  sensorFiltro(pinTrigFiltro, pinEchoFiltro, "Sensor Filtro", config.distMaximaAguaFiltro, config.distMinimaAguaFiltro, config.ultrasonicFailLimit, logger), 
  bombaLago("Bomba Lago", pinBombaLago, config.pumpDelay, logger), 
  bombaFiltro("Bomba Filtro", pinBombaFiltro, config.pumpDelay, logger), 
  logger(logger), sensorReadInterval(config.ultrasonicReadInterval) {
}

void WaterControlSystem::run() {

    float lagoonLevel = sensorLago.getWaterLevel();
    float filterLevel = sensorFiltro.getWaterLevel();

    logger.log("Nivel Lago: " + String(lagoonLevel, 2)); 
    logger.log("Nivel Filtro: " + String(filterLevel, 2));

    updateStatusBombas(getStatusSistema(sensorLago.getWaterLevelStatus(), sensorFiltro.getWaterLevelStatus()));
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

void WaterControlSystem::setDistanciaMaximaParaAguaLago(unsigned long distancia) { sensorLago.setDistanciaMaximaParaAgua(distancia);}
void WaterControlSystem::setDistanciaMinimaParaAguaLago(unsigned long distancia) { sensorLago.setDistanciaMinimaParaAgua(distancia);}
void WaterControlSystem::setDistanciaMaximaParaAguaFiltro(unsigned long distancia) { sensorFiltro.setDistanciaMaximaParaAgua(distancia);}
void WaterControlSystem::setDistanciaMinimaParaAguaFiltro(unsigned long distancia) { sensorFiltro.setDistanciaMinimaParaAgua(distancia);}

void WaterControlSystem::updateStatusBombas(int statusSistema)
  {
    switch (statusSistema)
    {
    case BOMBAS_DESLIGADAS:
      bombaLago.stop();
      bombaFiltro.stop();
      break;
    case APENAS_FILTRO:
      bombaLago.stop();
      bombaFiltro.start();
      break;
    case APENAS_LAGO:
      bombaLago.start();
      bombaFiltro.stop();
      break;
    case AMBAS_LIGADAS:
      bombaLago.start();
      bombaFiltro.start();
      break;

    default:
      bombaLago.stop();
      bombaFiltro.stop();
      break;
    }
  }

StatusSistema WaterControlSystem::getStatusSistema(WaterLevelStatus statusLago, WaterLevelStatus statusFiltro) {
    if (statusLago == OK && statusFiltro == OK) {
        return AMBAS_LIGADAS; // Níveis de água estão em condições ideais
    } else if (statusLago == EMPTY && statusFiltro == FULL) {
        return APENAS_FILTRO; // Necessidade de transferir água do filtro para o lago
    } else if (statusLago == FULL && statusFiltro == EMPTY) {
        return APENAS_LAGO; // Necessidade de transferir água do lago para o filtro
    } else if (statusLago == EMPTY || statusFiltro == EMPTY) {
        return BOMBAS_DESLIGADAS; // Ambas as bombas devem permanecer desligadas ou o sistema está parando-as
    } else {
        return ERRO; // Alguma condição não atendida ou erro de leitura
    }
}


DeserializationError WaterControlSystem::setSystemConfig(const String &jsonConfig)
{
  JsonDocument doc; // Ajustar o tamanho
  DeserializationError error = deserializeJson(doc, jsonConfig);
  if (error)  { return error;  }

  setConfig(doc["distMaximaAguaLago"], 
            doc["distMinimaAguaLago"], 
            doc["distMaximaAguaFiltro"], 
            doc["distMinimaAguaFiltro"], 
            doc["ultrasonicFailLimit"], 
            doc["ultrasonicReadInterval"], 
            doc["pumpDelay"], 
            doc["logLigado"]);

  return DeserializationError::Ok;
}

void WaterControlSystem::setSensorReadInterval(unsigned long interval){
  this->sensorReadInterval = interval;
}

unsigned long WaterControlSystem::getSensorReadInterval(){
  return this->sensorReadInterval;
}

void WaterControlSystem::setConfig(int distMaximaAguaLago, int distMinimaAguaLago, int distMaximaAguaFiltro, int distMinimaAguaFiltro, int ultrasonicFailLimit, unsigned long ultrasonicReadInterval, unsigned long pumpDelay, bool logLigado) {
    // Aqui configuramos os parâmetros do sistema
    this->sensorLago.setDistanciaMaximaParaAgua(distMaximaAguaLago);
    this->sensorLago.setDistanciaMinimaParaAgua(distMinimaAguaLago);
    this->sensorFiltro.setDistanciaMaximaParaAgua(distMaximaAguaFiltro);
    this->sensorFiltro.setDistanciaMinimaParaAgua(distMinimaAguaFiltro);
    this->sensorLago.setLimitFailures(ultrasonicFailLimit);
    this->sensorFiltro.setLimitFailures(ultrasonicFailLimit);
    this->sensorReadInterval = ultrasonicReadInterval;
    this->bombaLago.setDelayTime(pumpDelay);
    this->bombaFiltro.setDelayTime(pumpDelay);
    logger.setLogStatus(logLigado);
}