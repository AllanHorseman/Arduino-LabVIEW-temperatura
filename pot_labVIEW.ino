#include <LiquidCrystal.h>

//Declaramos los pines del LCD//
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4, ref=50, periodo = 500;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int pot, var, relay = 13;
unsigned long ahora = 0;
float temp;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(relay, OUTPUT);
}

void loop() {
  pot = analogRead(A0);
  temp = pot*150.0/1023;

  if(millis() > ahora + periodo){
    ahora = millis();
    lcd.clear();
  }
  
  if(Serial.available()){
    Serial.println(temp);
    var = Serial.read();

    if(var=='a')
      {digitalWrite(relay,HIGH);}
    if(var=='b')
      {digitalWrite(relay,LOW);}

/////////El LCD muestra la temperatura sin considerar la temperatura de referencia/////////
    /*lcd.setCursor(0,0);       //Volver al inicio 
    lcd.print("Temp Ref: ");     //Muestra en LCD Temperatura//
    lcd.print (ref);
    lcd.setCursor(0,1);        // Cambia de renglon en el LCD//
    lcd.print("Temp:");     //Muestra en LCD Temperatura//
    lcd.print(temp);*/
///////////////////////////////////////////////////////////////////////////////////////////
    
    if (temp >= ref){                  // Para una temperatura alta//
      lcd.setCursor(0,0);
      lcd.print("Temp Ref: ");     //Muestra en LCD Temperatura//
      lcd.print (ref);
      lcd.setCursor (0,1);        // Cambia de renglon en el LCD//
      lcd.print("Temp alta:");     //Muestra en LCD Temperatura//
      lcd.print(temp);           //muestra la variable de la temperatura//
    }
    
    else{
      lcd.setCursor(0,0);
      lcd.print("Temp Ref:");     //Muestra en LCD Temperatura//
      lcd.print (ref);
      lcd.setCursor (0,1);        // Cambia de renglon en el LCD//
      lcd.print ("Temp baja:");         //Imprime Alta en el segundo renglon//
      lcd.print (temp);           //muestra la variable de la temperatura//
    }
  }
}
