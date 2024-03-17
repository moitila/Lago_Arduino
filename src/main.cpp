#include <Arduino.h>
#include <NewPing.h>
#include "WaterControlSystem.h"

#define pinTrigLago 6
#define pinEchoLago 5
#define pinTrigFiltro 4
#define pinEchoFiltro 3

#define pinBombaLago 9
#define pinBombaFiltro 8


WaterControlSystem waterSystem(pinTrigLago, pinEchoLago, pinTrigFiltro, pinEchoFiltro, pinBombaLago, pinBombaFiltro);

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
