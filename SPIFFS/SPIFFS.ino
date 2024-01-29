#include <FS.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include "time.h"

#define ledPin 2
#define btn 21

int estadoAnterior = 0;
int estadoLed = 0;

String str;
String s;

const char* ssid = "***";
const char* password = "***";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3 * 3600;
const int   daylightOffset_sec = 3600;

String getDateLog() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter a hora local");
    return "Erro ao obter a hora local";
  }

  char formattedTime[30];
  strftime(formattedTime, sizeof(formattedTime), "[%d-%m-%Y] %H:%M:%S", &timeinfo);

  return String(formattedTime);
}

void writeFile(String state, String path, String writeType = "a") {
  File rFile = SPIFFS.open(path, writeType.c_str());
  if (!rFile) Serial.println("Erro ao abrir arquivo!");
  else {
    Serial.print("tamanho: ");
    Serial.println(rFile.size());
    rFile.println(state);
    Serial.print("Gravou: ");
    Serial.println(state);
  }
  rFile.close();
}

String readFile(String path) {
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");
  if (!rFile) Serial.println("Erro ao abrir arquivo!");
  else {
    Serial.print("---------- Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (rFile.position() < rFile.size()) {
      s = rFile.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    rFile.close();
    return s;
  }
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) Serial.println("\nErro ao abrir o sistema de arquivos");
  else Serial.println("\nSistema de arquivos aberto com sucesso!");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(btn, INPUT);

  // formatFile();
  Serial.println("abrir arquivo");
  openFS(); 
  estadoLed = readFile("/state.txt").toInt();
  digitalWrite(ledPin, estadoLed);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println(getDateLog());

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  readFile("/logs.txt");
}

void loop() {
  int estadoAtual = digitalRead(btn);
  
  if (estadoAnterior == 1 && estadoAtual == 0) {
    Serial.println("Botão pressionado!");
    estadoLed = !estadoLed;
    digitalWrite(ledPin, estadoLed);
    String log = getDateLog() + " - Led " + (estadoLed ? "Ligado" : "Desligado");
    writeFile(String(estadoLed), "/state.txt", "w");
    writeFile(log, "/logs.txt");
  }

  estadoAnterior = estadoAtual;
  delay(100);  
}
