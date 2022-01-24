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
