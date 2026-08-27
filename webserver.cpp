#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "webserver.h"
#include "plants.h"

ESP8266WebServer server(80);

// Variaveis de estado e contadores
unsigned long total_activations = 0;
float water_used_total = 0; // Litros

const char html_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Horta</title>
<style>
body{font-family:Arial,sans-serif;background:#f3f4f6;margin:0;padding:20px;padding-bottom:110px}
h1{text-align:center;color:#1f2937}
.container{display:flex;justify-content:center;gap:20px;flex-wrap:wrap;max-width:800px;margin:0 auto}
.card{background:#fff;width:100%;max-width:320px;padding:24px;border-radius:16px;box-shadow:0 4px 6px rgba(0,0,0,.05);box-sizing:border-box}
.card1{border-top:6px solid #0f8179}
.card2{border-top:6px solid #a27a14}
.card h3{margin-top:0;color:#1f2937;font-size:20px}
.card p{color:#4b5563;font-size:14px;line-height:1.5;margin-bottom:20px}
input[type="time"]{width:100%;padding:10px;font-size:16px;border:1px solid #d1d5db;border-radius:8px;box-sizing:border-box;margin-bottom:12px;background:#f9fafb}
button{color:#fff;border:none;padding:12px 20px;font-size:16px;border-radius:8px;cursor:pointer;width:100%;font-weight:bold}
.card1 button{background:#0f8179}
.btn-salvar{background:#a27a14}
.btn-desativar{background:#dc2626}
.barra-status{position:fixed;bottom:0;left:0;width:100%;background:#fff;box-shadow:0 -4px 10px rgba(0,0,0,.08);padding:12px 15px;box-sizing:border-box;display:flex;justify-content:space-around;align-items:center;flex-wrap:wrap;gap:10px;z-index:1000}
.info-item{text-align:center;font-size:13px;color:#4b5563}
.info-item b{display:block;font-size:15px;color:#1f2937;margin-top:2px}
.status-badge{padding:6px 12px;border-radius:20px;font-size:14px;font-weight:bold;display:inline-block}
.status-ligada{background:#d1fae5;color:#065f46}
.status-desligada{background:#fee2e2;color:#991b1b}
</style>
</head>
<body>
<h1>Smart Horta 🌱</h1>
<div class="container">
<div class="card card1">
<h3>Rega Manual 👆</h3>
<p>Clique para ativar ou desativar a irrigação de forma imediata.</p>
<a href="/rega-manual"><button>Regar Agora</button></a>
</div>
<div class="card card2">
<h3>Rega Automática ⏱️</h3>
<p>Programe a irrigação para ser acionada em um horário específico.</p>
<form action="/rega-automatizada" method="GET">
<input type="time" name="time" value="{{horario_salvo}}" required>
<button type="submit" name="action" value="{{acaoBotao}}" class="{{classeBotao}}">{{textoBotao}}</button>
</form>
</div>
</div>
<div class="barra-status">
<div class="info-item">Status<b><span class="status-badge status-desligada">🔴 Desligada</span></b></div>
<div class="info-item">Acionamentos 🔄<b>{{totalAcionamentos}} vezes</b></div>
<div class="info-item">Água Utilizada 💧<b>{{TotalAguaLitros}} L</b></div>
</div>
</body>
</html>
)=====";

void handleRoot() {
  String html = FPSTR(html_page);

  html.replace("{{horario_salvo}}", String(IRRIGATION_HOUR));
  if (PROGRAM_IRRIGATION) {
    html.replace("{{acaoBotao}}", "desativar");
    html.replace("{{textoBotao}}", "Desativar");
    html.replace("{{classeBotao}}", "btn-desativar");
  } else {
    html.replace("{{acaoBotao}}", "salvar");
    html.replace("{{textoBotao}}", "Programar");
    html.replace("{{classeBotao}}", "btn-salvar");
  }
  html.replace("{{totalAcionamentos}}", String(total_activations));
  html.replace("{{TotalAguaLitros}}", String(water_used_total));
  server.send(200, "text/html", html);
}

void register_irrigation() {
  water_used_total += (float)(getIrrigationWaterAmount() / 1000.0); // converte ml para L
  total_activations++;
}

void execute_manual_irrigation() {
  activateIrrigation();
  register_irrigation();
  server.sendHeader("Location", "/", true);
  server.send(303, "text/plain", "");
}

void execute_automated_irrigation() {
  if (server.hasArg("action")) {
    String action = server.arg("action");
    if (action == "salvar" && server.hasArg("time")) {
      IRRIGATION_HOUR = server.arg("time");
      PROGRAM_IRRIGATION = true;
    } else if (action == "desativar") {
      PROGRAM_IRRIGATION = false;
    }
  }
  server.sendHeader("Location", "/", true);
  server.send(303, "text/plain", "");
}

void init_webserver() {
  delay(500);
  WiFi.begin(SSID_WIFI, PASSWORD_WIFI);
  Serial.print("Conectando a rede ");
  Serial.println(SSID_WIFI);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("\nConectado ao Wi-Fi!");
  Serial.println(SSID_WIFI);
  Serial.print("Endereço IP: http://");
  Serial.println(WiFi.localIP());

  // routes
  server.on("/", handleRoot);
  server.on("/rega-manual", execute_manual_irrigation);
  server.on("/rega-automatizada", execute_automated_irrigation);

  server.begin();
  Serial.println("Servidor Web iniciado!\n");
}

void process_webserver() {
  server.handleClient();
}