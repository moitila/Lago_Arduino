#include <NewPing.h>
#include "WaterLevelSensor.h";


WaterLevelSensor::WaterLevelSensor(int triggerPin, int echoPin,String nome,  LogManager &logger) : sonar(triggerPin, echoPin, 150),
    distanciaMaximaParaAgua(20), 
    distanciaMinimaParaAgua(4),  
    media(0),                    
    consecutivefailures(0),      
    limitfailures(10),
    logger(logger)     
{

}

void WaterLevelSensor::updateReadings(long novaLeitura)
  {
    leituras[0] = leituras[1];
    leituras[1] = leituras[2];
    leituras[2] = novaLeitura;
    updateMedia();
    updateStatus();
  }

  void WaterLevelSensor::updateMedia()
  {
    media = (leituras[0] + leituras[1] + leituras[2]) / 3.0f;
  }
  
  bool WaterLevelSensor::verifyReading(unsigned long value)
  {
    if (value > 0)
    {
      consecutivefailures = 0;
      updateReadings(value);
      return true;
    }
    else
    {
      consecutivefailures++;
      if (consecutivefailures >= limitfailures)
      {
        handleAlert();
      }
      return false;
    }
  }

  void WaterLevelSensor::handleAlert()
  {
    logger.warning("ALERTA: Leituras inválidas consecutivas excederam o limite. Sensor: " + getNome());
    // Adicionar mais lógicas de alerta conforme necessário
  }

  // Atualiza o status baseado na média das leituras
  void WaterLevelSensor::updateStatus()  {
    if (media > distanciaMaximaParaAgua) { status = EMPTY; }
    else if (media < distanciaMinimaParaAgua)  { status = FULL; }
    else {status = OK; }
  }

  WaterLevelStatus WaterLevelSensor::getWaterLevelStatus(){
    return status;
  }

  float WaterLevelSensor::getWaterLevel()
  {
    unsigned long reading = sonar.ping_cm();
    if (verifyReading(reading))
    {
      logger.log("Leitura válida sensor " + getNome() + ": " + String(reading,2));
    } else {
      logger.warning("Leitura descartada sensor " + getNome());
    }
    return getMediaLeitura();
  }

  float WaterLevelSensor::getMediaLeitura() { return media; }

  void WaterLevelSensor::setDistanciaMinimaParaAgua(unsigned long distancia) {
        distanciaMinimaParaAgua = distancia;
  }

  unsigned long WaterLevelSensor::getDistanciaMinimaParaAgua() const {
        return distanciaMinimaParaAgua;
  }

  void WaterLevelSensor::setDistanciaMaximaParaAgua(unsigned long distancia) {
        distanciaMaximaParaAgua = distancia;
  }

  unsigned long WaterLevelSensor::getDistanciaMaximaParaAgua() const {
        return distanciaMaximaParaAgua;
  }
  int WaterLevelSensor::getStatus() { return status; }
  
  void WaterLevelSensor::setLimitFailures(unsigned int limit){
    this->consecutivefailures = limit;
  }
  void WaterLevelSensor::setNome(String nome_){
    this->nome = nome_;
  }
  String WaterLevelSensor::getNome(){
    return nome;
  }