#include "webserver.h"
#include "waterpump.h"
#include "plants.h"
#include "timer.h"

void setup() {
  Serial.begin(9600);
  initWaterPump();
  init_webserver();
  initTimer();
}

void loop() {
  process_webserver();

  if (isIrrigationDue()) {
    Serial.println("Executando rega automatica");
    activateIrrigation();
    register_irrigation();
  }
}
