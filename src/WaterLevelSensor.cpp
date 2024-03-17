#include <NewPing.h>
#include "WaterLevelSensor.h";


WaterLevelSensor::WaterLevelSensor(int triggerPin, int echoPin) : sonar(triggerPin, echoPin, 150),
    distanciaMaximaParaAgua(20), 
    distanciaMinimaParaAgua(4),  
    media(0),                    
    consecutivefailures(0),      
    limitfailures(10)     
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
    Serial.println("ALERTA: Leituras inválidas consecutivas excederam o limite.");
    // Adicionar mais lógicas de alerta conforme necessário
  }

  // Atualiza o status baseado na média das leituras
  void WaterLevelSensor::updateStatus()  {
    if (media > distanciaMaximaParaAgua) { status = EMPTY; }
    else if (media < distanciaMinimaParaAgua)  { status = FULL; }
    else {status = OK; }
  }

  float WaterLevelSensor::getWaterLevel()
  {
    unsigned long reading = sonar.ping_cm();
    if (verifyReading(reading))
    {
      Serial.print("Leitura válida: ");
      Serial.println(reading);
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