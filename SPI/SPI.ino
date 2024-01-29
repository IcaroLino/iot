#include <SPI.h>
#include <MFRC522.h>

#define PINO_SS 10
#define PINO_RST 9
#define ledVermelho 6 
#define ledVerde 5
#define buzzer 3

int contador = 0;
unsigned long temporizador;

MFRC522 mfrc522(PINO_SS, PINO_RST);

void piscarLed() {
  digitalWrite(ledVermelho, HIGH);
  delay(200);
  digitalWrite(ledVermelho, LOW);
  delay(100);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Sistema Inicializado: Aproxime o token");
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(ledVermelho, HIGH);

  if (!mfrc522.PICC_IsNewCardPresent()) return;  
  if (!mfrc522.PICC_ReadCardSerial()) return;
  
  Serial.print("UID da tag:"); 
  String conteudo= ""; 
  byte letra;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) Serial.print(" 0");
    else Serial.print(" ");
    
    Serial.print(mfrc522.uid.uidByte[i], HEX);
 
    if(mfrc522.uid.uidByte[i] < 0x10) conteudo.concat(String(" 0"));
    else conteudo.concat(String(" "));
    
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "73 2E 88 11") {
    contador = 0;

    Serial.println("Acesso liberado!");    
    digitalWrite(ledVerde,HIGH);
    digitalWrite(ledVermelho,LOW);
    delay(10000);
    digitalWrite(ledVerde,LOW);
    
  } else {
    contador++;
    
    Serial.println("Cartão inválido!");
    Serial.println("Tentativa: " + (String)contador);
    
    for (int i = 0; i <= 3; i++) piscarLed();
    
    if(contador >= 5) {
      temporizador = millis();
      while(millis() - temporizador < 30000) {
        digitalWrite(buzzer, HIGH);
        piscarLed();
      }
      digitalWrite(buzzer, LOW);
    }
  } 
  delay(1000);
}