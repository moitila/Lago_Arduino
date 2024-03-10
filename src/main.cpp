#include <Arduino.h>
#include <NewPing.h>

#define PIN_TRIG_LAGO 6
#define pinEchoLago 5
#define pinTrigFiltro 4
#define pinEchoFiltro 3

#define pinBombaLago 9
#define pinBombaFilro 8

class WaterLevelSensor {
public:
    WaterLevelSensor(int triggerPin, int echoPin) : sonar(triggerPin, echoPin,150) {}

    float getWaterLevel() {
        unsigned long leituraCerta = sonar.ping_cm();
        Serial.print("Sensor "); Serial.println(leituraCerta);
        if (leituraCerta > 0){
          updateReadings(leituraCerta);
        } else {
          leituraCerta = -1;
        }
        return getMediaLeitura();
    }
    void setMin(long min){
      this->min = min;
    }

    void setMax(long max){
      this->max = max;
    }

    int getStatus(){
      return status;
    }

private:
    unsigned long min = 4;
    unsigned long max = 20;
    float media = 0;
    unsigned int status = -1;

    NewPing sonar;
    long leituras[3] = {0,0,0};
    void updateReadings(long novaLeitura) {
        // Desloca as leituras antigas e adiciona a nova leitura ao final
        leituras[0] = leituras[1];
        leituras[1] = leituras[2];
        leituras[2] = novaLeitura;
        updateMedia();
        updateStatus();
    }  
    void updateMedia(){
      media = (leituras[0] + leituras[1] + leituras[2]) / 3.0f;
    }
    float getMediaLeitura(){
      return media;
    }
    void updateStatus(){
      if (media > max){
        status = 0; //VAZIO
      } else if (media < min) {
        status = 2;//CHEIO
      } else {
        status = 1;//OK
      }
    };
};

class WaterPump {
public:
    WaterPump(int pumpPin) : pumpPin(pumpPin) {
        pinMode(pumpPin, OUTPUT);
        stop();
    }

    void start() {
        digitalWrite(pumpPin, HIGH);
    }

    void stop() {
        digitalWrite(pumpPin, LOW);
    }

private:
    int pumpPin;
};




class WaterControlSystem {
public:
    bool debug = true;
    WaterControlSystem() : sensorLago(6, 5), sensorFiltro(3, 4), bombaLago(9), bombaFiltro(8) {}

    void run() {
       
        float lagoonLevel = sensorLago.getWaterLevel();
        float filterLevel = sensorFiltro.getWaterLevel();
        
        if (debug){
          Serial.print("Nivel médio Lago ");
          Serial.print(lagoonLevel);
          Serial.println(" cm");

          Serial.print("Nivel medio Filtro ");
          Serial.print(filterLevel);
          Serial.println(" cm");
        }
        updateStatusBombas(getStatusSistema(sensorLago.getStatus(),sensorFiltro.getStatus()));
    }

private:
    
    WaterLevelSensor sensorLago;
    WaterLevelSensor sensorFiltro;
    WaterPump bombaLago;
    WaterPump bombaFiltro;
    void updateStatusBombas(int statusSistema){
      Serial.print("Status ");
      Serial.println(statusSistema);
      switch (statusSistema)
      {
      case 1 :
          bombaLago.stop();
          bombaFiltro.stop();
        break;
      case 2 :
          bombaLago.stop();
          bombaFiltro.start();
        break;
      case 3 :
          bombaLago.start();
          bombaFiltro.stop();
        break;
      case 4 :
          bombaLago.start();
          bombaFiltro.start();
        break;

      default:
          bombaLago.stop();
          bombaFiltro.stop();
        break;
      }
    }
    int getStatusSistema(int statusLago, int statusFiltro){
      int statusSistema = 0;
      if ( (statusLago == 0 && statusFiltro == 0) ||
           (statusLago == 2 && statusFiltro == 2)) 
        statusSistema = 1;
      else if ( (statusLago == 0 && statusFiltro == 1) ||
                (statusLago == 0 && statusFiltro == 2) ||
                (statusLago == 1 && statusFiltro == 2))
        statusSistema = 2;
      else if ( (statusLago == 1 && statusFiltro == 0) ||
                (statusLago == 2 && statusFiltro == 0) ||
                (statusLago == 2 && statusFiltro == 1))
        statusSistema = 3;
      else if (statusLago == 1 && statusFiltro == 1) 
        statusSistema = 4;

      return statusSistema;  
    }
};

WaterControlSystem waterSystem;

void setup() {
    Serial.begin(115200);
}

void loop() {
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
