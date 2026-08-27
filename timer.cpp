#include "config.h"
#include "timer.h"
#include <ESP8266WiFi.h>
#include <time.h>

// definições de localização
const long gmtOffset_sec = GMT * 3600;
const byte daylightOffset_sec = 0;

void initTimer() {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
  Serial.print("Sincronizando horario NTP");
  time_t now = 0;
  while (now < 24 * 3600) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\nHorario sincronizado com sucesso!");
}

bool isIrrigationDue() {
  if (!PROGRAM_IRRIGATION) return false;

  time_t now = time(nullptr);
  struct tm* weather_info = localtime(&now);

  int current_hour = weather_info->tm_hour;
  int current_minute = weather_info->tm_min;
  int current_day = weather_info->tm_mday;

  // Formata a hora/minuto atual para "HH:MM" (ex: "08:00")
  char currentTimeStr[6];
  sprintf(currentTimeStr, "%02d:%02d", current_hour, current_minute);

  static int last_irrigation_date = -1;

  // Compara a String formatada com o horário salvo
  if (String(currentTimeStr) == IRRIGATION_HOUR && last_irrigation_date != current_day) {
    last_irrigation_date = current_day;
    return true;
  }
  return false;
}