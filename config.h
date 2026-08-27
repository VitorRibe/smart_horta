/*
    Este arquivo reúne e define parâmetros utilizados pelo sistema
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Definições de Rede
#define SSID_WIFI "Sua_rede_wifi"
#define PASSWORD_WIFI "Sua_senha"

// Definição de localização
const long GMT = -3; // GMT (Brasil é "-3")

// Definições de irrigação
const int WATER_FLOW_RATE = 1; // Vazão em litros por minutos da bomba utilizada 
const int IRRIGATION_TIME = 10; // Tempo que a bomba ficará ligada em Segundos
inline bool PROGRAM_IRRIGATION = false; // guarda o estado da automatização da irrigação
inline String IRRIGATION_HOUR = "00:00"; // variável que guarda o horário de irrigação

#endif