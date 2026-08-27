/*
  Este módulo controla exclusivamente o hardware do relé e o acionamento físico da bomba d'água
*/

#ifndef WATER_PUMP_CONTROL_H
#define WATER_PUMP_CONTROL_H

#include <Arduino.h>

void activateWaterPump(bool status);
void initWaterPump();

#endif
