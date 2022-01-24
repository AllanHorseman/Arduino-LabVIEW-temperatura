#include <LiquidCrystal.h>

//Declaramos los pines del LCD//
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4, ref=50;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int pot, var;
int relay = 13; 
float temp;

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
}

void loop() {
  pot = analogRead(A0);
  temp = pot*150.0/1023;
  
  if(Serial.available()){
    Serial.println(temp);
    var = Serial.read();

    if(var=='a')
      {digitalWrite(relay,HIGH);}
    if(var=='b')
      {digitalWrite(relay,LOW);}
  }
}
