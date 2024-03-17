
#include "WaterControlSystem.h"


WaterControlSystem::WaterControlSystem(int pinTrigLago, int pinEchoLago, int pinTrigFiltro, int pinEchoFiltro, 
                                        int pinBombaLago, int pinBombaFiltro) : sensorLago(pinTrigLago, pinEchoLago), 
                                        sensorFiltro(pinTrigFiltro, pinEchoFiltro), bombaLago(pinBombaLago,5000), 
                                        bombaFiltro(pinBombaFiltro,5000) {

}

void WaterControlSystem::setMinLakeLevel(unsigned long minLevel) {
    sensorLago.setDistanciaMinimaParaAgua(minLevel);
}

void WaterControlSystem::setMaxLakeLevel(unsigned long maxLevel) {
    sensorLago.setDistanciaMaximaParaAgua(maxLevel);
}

void WaterControlSystem::setMinFilterLevel(unsigned long minLevel) {
    sensorFiltro.setDistanciaMinimaParaAgua(minLevel);
}

void WaterControlSystem::setMaxFilterLevel(unsigned long maxLevel) {
    sensorFiltro.setDistanciaMaximaParaAgua(maxLevel);
}
void WaterControlSystem::updateStatusBombas(int statusSistema)
  {

    if (debug)
    {
      Serial.print("Status ");
      Serial.println(statusSistema);
    }

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

    if (debug)
      sendInfoSerialDebug(lagoonLevel, filterLevel);

    updateStatusBombas(getStatusSistema(sensorLago.getStatus(), sensorFiltro.getStatus()));
}
int WaterControlSystem::getStatusSistema(int statusLago, int statusFiltro)
{
    int statusSistema = 0;
    if ((statusLago == 0 && statusFiltro == 0) ||
        (statusLago == 2 && statusFiltro == 2))
        statusSistema = 1;
    else if ((statusLago == 0 && statusFiltro == 1) ||
             (statusLago == 0 && statusFiltro == 2) ||
             (statusLago == 1 && statusFiltro == 2))
        statusSistema = 2;
    else if ((statusLago == 1 && statusFiltro == 0) ||
             (statusLago == 2 && statusFiltro == 0) ||
             (statusLago == 2 && statusFiltro == 1))
        statusSistema = 3;
    else if (statusLago == 1 && statusFiltro == 1)
        statusSistema = 4;

    return statusSistema;
}
void WaterControlSystem::sendInfoSerialDebug(float lagoonLevel, float filterLevel)
  {
    {
      Serial.print("Nivel médio Lago ");
      Serial.print(lagoonLevel);
      Serial.println(" cm");

      Serial.print("Nivel medio Filtro ");
      Serial.print(filterLevel);
      Serial.println(" cm");
    }
  }
