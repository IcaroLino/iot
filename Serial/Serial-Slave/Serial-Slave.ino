#include <SoftwareSerial.h>
#define ledPin 5

int ledVal = 0;
int intVal = 0;
int oldIntVal = 0;

SoftwareSerial ArduinoMaster(10,11);

void setup() {
  Serial.begin(9600);
  ArduinoMaster.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() { 
  String msg = "";

  while(ArduinoMaster.available()>0){
    char c = ArduinoMaster.read();
    msg += c;
  }

  if(msg != "") {
    Serial.println(msg);
  }

  if(msg.startsWith("/led")){
    msg.remove(0, 4);
    if (msg.charAt(0) == '1') {
      digitalWrite(ledPin, HIGH);
      msg.remove(0, 1);
      if(msg.startsWith("/pot")){
        msg.remove(0, 4);
        analogWrite(ledPin, msg.toInt());
      }
    } else {
      digitalWrite(ledPin, LOW);
    }    
  }  
  delay(200);
}
