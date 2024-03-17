#include <Arduino.h>
#include <NewPing.h>

#define pinTrigLago 6
#define pinEchoLago 5
#define pinTrigFiltro 4
#define pinEchoFiltro 3

#define pinBombaLago 9
#define pinBombaFiltro 8

enum WaterLevelStatus
{
  EMPTY = 0, // VAZIO
  OK = 1,    // NÍVEL OK
  FULL = 2   // CHEIO
};

class WaterLevelSensor
{

private:
  WaterLevelStatus status = EMPTY;
  unsigned long distanciaMaximaParaAgua = 20; // Distância máxima esperada até a água (reservatório vazio)
  unsigned long distanciaMinimaParaAgua = 4;  // Distância mínima esperada até a água (reservatório cheio)
  float media = 0;
  unsigned int consecutivefailures = 0;  // Contador de falhas consecutivas
  const unsigned int limitfailures = 10; // Limite de falhas antes do alerta
  NewPing sonar;
  long leituras[3] = {0, 0, 0};

  void updateReadings(long novaLeitura)
  {
    leituras[0] = leituras[1];
    leituras[1] = leituras[2];
    leituras[2] = novaLeitura;
    updateMedia();
    updateStatus();
  }

  void updateMedia()
  {
    media = (leituras[0] + leituras[1] + leituras[2]) / 3.0f;
  }

  bool verifyReading(unsigned long value)
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
  void handleAlert()
  {
    Serial.println("ALERTA: Leituras inválidas consecutivas excederam o limite.");
    // Adicionar mais lógicas de alerta conforme necessário
  }

  // Atualiza o status baseado na média das leituras
  void updateStatus()  {
    if (media > distanciaMaximaParaAgua) { status = EMPTY; }
    else if (media < distanciaMinimaParaAgua)  { status = FULL; }
    else {status = OK; }
  };

public:
  WaterLevelSensor(int triggerPin, int echoPin) : sonar(triggerPin, echoPin, 150) {}

  float getWaterLevel()
  {
    unsigned long reading = sonar.ping_cm();
    if (verifyReading(reading))
    {
      Serial.print("Leitura válida: ");
      Serial.println(reading);
    }
    return getMediaLeitura();
  }

  float getMediaLeitura() { return media; }
  void setDistanciaMinimaParaAgua(unsigned long distancia) {
        distanciaMinimaParaAgua = distancia;
  }

  unsigned long getDistanciaMinimaParaAgua() const {
        return distanciaMinimaParaAgua;
  }

  void setDistanciaMaximaParaAgua(unsigned long distancia) {
        distanciaMaximaParaAgua = distancia;
  }

  unsigned long getDistanciaMaximaParaAgua() const {
        return distanciaMaximaParaAgua;
  }
  int getStatus() { return status; }
};

class WaterPump
{
public:
  WaterPump(int pumpPin, unsigned long delayTime) : pumpPin(pumpPin), delayTime(delayTime)
  {
    pinMode(pumpPin, OUTPUT);
    stop();
  }

  void start()
  {
    unsigned long currentTime = millis();
    if ((currentTime - lastChangeTime >= delayTime) && !isRunning)
    {
      digitalWrite(pumpPin, HIGH);
      isRunning = true;
      lastChangeTime = currentTime;
    }
  }

  void stop()
  {
    unsigned long currentTime = millis();
    if ((currentTime - lastChangeTime >= delayTime) && isRunning)
    {
      digitalWrite(pumpPin, LOW);
      isRunning = false;
      lastChangeTime = currentTime;
    }
  }

private:
  int pumpPin;
  bool isRunning = false;           // Mantém o estado atual da bomba
  unsigned long lastChangeTime = 0; // Armazena a última vez que o estado foi alterado
  unsigned long delayTime;          // Tempo de atraso mínimo para mudanças de estado
};

class WaterControlSystem
{
public:
  bool debug = true;
  WaterControlSystem() : sensorLago(pinTrigLago, pinEchoLago), sensorFiltro(pinEchoFiltro, pinTrigFiltro), bombaLago(pinBombaLago, 5000), bombaFiltro(pinBombaFiltro, 5000) {}

  void run()
  {

    float lagoonLevel = sensorLago.getWaterLevel();
    float filterLevel = sensorFiltro.getWaterLevel();

    if (debug)
      sendInfoSerialDebug(lagoonLevel, filterLevel);

    updateStatusBombas(getStatusSistema(sensorLago.getStatus(), sensorFiltro.getStatus()));
  }

private:
  WaterLevelSensor sensorLago;
  WaterLevelSensor sensorFiltro;
  WaterPump bombaLago;
  WaterPump bombaFiltro;
  void updateStatusBombas(int statusSistema)
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
  int getStatusSistema(int statusLago, int statusFiltro)
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
  void sendInfoSerialDebug(float lagoonLevel, float filterLevel)
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
};

WaterControlSystem waterSystem;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  waterSystem.run();
  delay(1000); // Ajuste conforme necessário
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
