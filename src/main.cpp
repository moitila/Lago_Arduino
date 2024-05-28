#include <EEPROM.h>
#include <Arduino.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include "WaterControlSystem.h"
#include "ExternalCom.h"
#include "LogManager.h"
#include "ConfigData.h"

#define pinTrigLago 6
#define pinEchoLago 5
#define pinTrigFiltro 4
#define pinEchoFiltro 3

#define pinBombaLago 9
#define pinBombaFiltro 8

unsigned long lastSensorReadTime = 0;

#define RX_PIN 10
#define TX_PIN 11

SoftwareSerial ecSerial(RX_PIN, TX_PIN);
LogManager logger;
void writeConfigToEEPROM(const ConfigData &config) {
    EEPROM.put(0, config);
}
ConfigData readConfigFromEEPROM() {
    ConfigData config;
    EEPROM.get(0, config);
    if (config.distMaximaAguaLago == 0xFFFF) {  // Supondo que 0xFFFF seja um valor inválido padrão
      // Inicializar com valores padrão
      config.distMaximaAguaLago = 100;
      config.distMinimaAguaLago = 50;
      config.distMaximaAguaFiltro = 100;
      config.distMinimaAguaFiltro = 50;
      config.ultrasonicFailLimit = 3;
      config.ultrasonicReadInterval = 1000;
      config.pumpDelay = 5000;
      config.logLigado = true;

      writeConfigToEEPROM(config);
    }
  return config;
}

void updateSetup(const ConfigData &config) {
    writeConfigToEEPROM(config);
    
    // Reinicializa o sistema após um breve atraso
    delay(2000);
    asm volatile ("  jmp 0");
}

bool setSystemConfig(String jsonConfig) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonConfig);
    
    if (error) {
        return false;
    }
    
    ConfigData config;
    config.distMaximaAguaLago = doc["distMaximaAguaLago"];
    config.distMinimaAguaLago = doc["distMinimaAguaLago"];
    config.distMaximaAguaFiltro = doc["distMaximaAguaFiltro"];
    config.distMinimaAguaFiltro = doc["distMinimaAguaFiltro"];
    config.ultrasonicFailLimit = doc["ultrasonicFailLimit"];
    config.ultrasonicReadInterval = doc["ultrasonicReadInterval"];
    config.pumpDelay = doc["pumpDelay"];
    config.logLigado = doc["logLigado"];
    
    updateSetup(config);
    return true;
}

ConfigData config = readConfigFromEEPROM();
WaterControlSystem waterSystem(pinTrigLago, pinEchoLago, pinTrigFiltro, pinEchoFiltro, pinBombaLago, pinBombaFiltro, logger, config);
ExternalCom externalCom(ecSerial, waterSystem, logger);

void setup()
{
  Serial.begin(115200);
  ecSerial.begin(115200);
  
  waterSystem.setConfig(config.distMaximaAguaLago, config.distMinimaAguaLago, config.distMaximaAguaFiltro, 
        config.distMinimaAguaFiltro, config.ultrasonicFailLimit, config.ultrasonicReadInterval, config.pumpDelay, config.logLigado);
  
   waterSystem.setSensorReadInterval(config.ultrasonicReadInterval);
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorReadTime >= waterSystem.getSensorReadInterval()) {
    lastSensorReadTime = currentMillis;
    waterSystem.run();
  } 
  
  externalCom.run();
}

/*

  Lago	Filtro	Status	BombaLago	BombaFiltro	Status
  0	    0	        1	      0	        0	        0
  0	    1	        2	      0	        1	        1	      1
  0	    2	        3	      0	        1	        1
  1	    0	        4	      1	        0	        10
  1	    1         5	      1	        1	        11	    2
  1	    2	        6	      0	        1	        1
  2	    0	        7	      1	        0	        10
  2	    1	        8	      1	        0	        10	    3
  2	    2	        9	      0	        0	        0	      4

*/
