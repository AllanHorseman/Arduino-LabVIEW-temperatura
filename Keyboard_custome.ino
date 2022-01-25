#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>     // libreria de comunicacion por I2C
#include <LCD.h>      // libreria para funciones de LCD

//Declaramos los pines del LCD//
const int rs = 0, rw = 1, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal_I2C lcd (0x20, en, rw, rs, d4, d5, d6, d7);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char wawa[7];
int i = 0;
float ref = 50.00;
bool cambio = true;

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'.','0','-','D'}
};
byte rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  
  lcd.setBacklightPin(3,POSITIVE);  // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH);   // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);     // 16 columnas por 2 lineas para LCD 1602A
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp Ref: ");     //Muestra en LCD Temperatura//
  lcd.print (ref);
  lcd.setCursor(0,1);        // Cambia de renglon en el LCD//
  lcd.print("Temp: ");     //Muestra en LCD Temperatura//
}
  
void loop(){
  cambio = true;
  char customKey = customKeypad.getKey();

  if(customKey == 'C'){
    Serial.println("Introducir nuevo setpoint: ");
    lcd.clear();
    lcd.print("Cambiar setpoint");
  
    while(cambio){
      customKey = customKeypad.getKey();
      if (customKey){
        if(customKey != 'A' and customKey != 'C'){
          wawa[i] = customKey;
          Serial.println(wawa);
          i++;
        }
      }

      if(i == 6 or customKey == 'A'){
        i = 0;
        ref = atof(wawa);
        cambio = false;

        if(ref<-50.00 or ref>150.00){
          Serial.println("Fuera de escala!");
          lcd.clear();
          lcd.print("Fuera de escala!");
        }

        else{
          Serial.print("Nuevo setpoint: ");
          Serial.println(ref);
          lcd.clear();
          lcd.print("Nuevo setpoint: ");
          lcd.setCursor(0,1);
          lcd.print(ref);
        }
    
        for(int j = 0; j<7; j++){
          wawa[j] = '\0';
        }
      }
    }
  }
}
