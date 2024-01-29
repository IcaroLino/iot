#include <SoftwareSerial.h>
#define btPin 7

int estadoAnterior = 0;
int ledState = 0; 
int ValorPot = 0;
int potPin = A0;

String Dados;
String OldDados;

SoftwareSerial ArduinoSlave(10,11);

void setup() {
  Serial.begin(9600);
  Serial.println("Enter Commands: ");
  ArduinoSlave.begin(9600);
  pinMode(btPin, INPUT);
}

void loop() {  
  
  int estadoAtual = digitalRead(btPin);
  if (estadoAnterior == 1 && estadoAtual == 0) {
    Serial.println("\nBotão pressionado!");
    ledState = !ledState;
  }
  estadoAnterior = estadoAtual;
  
  delay(10);
  ValorPot = map(analogRead(potPin), 0, 1023, 0, 255);

  Dados = "/led" + (String)ledState + "/pot" + (String)ValorPot;

  if(OldDados != Dados){
    Serial.print("Master Sent: ");
    Serial.println(Dados);
    ArduinoSlave.print(Dados);
    OldDados = Dados;
  }

  delay(200);
}
