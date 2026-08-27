#include "config.h"
#include "plants.h"
#include "waterpump.h"

const int MAX_IRRIGATION_TIME = 120; // altere para definir o tempo maximo em segundos (120 segundos = 2 minutos)

void printMsg() {
  Serial.print("Irrigando por ");
  Serial.print(IRRIGATION_TIME);
  Serial.println(" Segundos");
}

unsigned int getIrrigationWaterAmount() {
  return (IRRIGATION_TIME * (WATER_FLOW_RATE * 1000)) / 60; // retorna a qtd de água usada em cada rega (ml)
}

void activateIrrigation() { 
  // proteção contra inserção de tempo de rega exagerado  
  if (IRRIGATION_TIME > MAX_IRRIGATION_TIME) {
    return;
  }
 
  printMsg();
  activateWaterPump(true);
  delay(1000 * IRRIGATION_TIME); // irrigando...
  activateWaterPump(false);
}