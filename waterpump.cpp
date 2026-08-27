#include "waterpump.h"

const byte SINAL_RELE = D1;

void initWaterPump() {
  pinMode(SINAL_RELE, OUTPUT);
  digitalWrite(SINAL_RELE, LOW); // inicia com a bomba desligada
}

void activateWaterPump(bool status) {
  if (status) {
    Serial.println("Bomba ligada");
    digitalWrite(SINAL_RELE, HIGH); // Ligar
  } else {
    Serial.println("Bomba desligada");
    digitalWrite(SINAL_RELE, LOW); // Desligar
  }
}
