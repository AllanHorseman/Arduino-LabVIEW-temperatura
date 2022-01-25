#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>     // libreria de comunicacion por I2C
#include <LCD.h>      // libreria para funciones de LCD

//Declaramos los pines del LCD y la dirección del módulo I2C//
const int rs = 0, rw = 1, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal_I2C lcd (0x20, en, rw, rs, d4, d5, d6, d7);

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char wawa[7];         //Variable donde se almacena el cambio del setpoint desde el keypad
int i = 0;            
float ref = 50.00;    //Setpoint incial
bool cambio = true;   //Variable que determina si se está modificando el setpoint o no

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},  //Tecla A es igual a enter
  {'4','5','6','B'},
  {'7','8','9','C'},  //Tecla C es igual a cambiar valor del setpoint
  {'.','0','-','D'}
};
byte rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5}; //connect to the column pinouts of the ke  ypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  
  lcd.setBacklightPin(3,POSITIVE);  // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH); // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);       // 16 columnas por 2 lineas para LCD 1602A
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp Ref: ");     //Muestra en LCD el setpoint//
  lcd.print (ref);
  lcd.setCursor(0,1);        // Cambia de renglon en el LCD//
  lcd.print("Temp: ");     //Muestra en LCD Temperatura actual//
}
  
void loop(){
  cambio = true;
  char customKey = customKeypad.getKey(); //Se lee lo ingresado desde el keypad y se guarda en customKey

  if(customKey == 'C'){ //Si se presiona la tecla C, comienza la función que cambia el valor del setpoint
    Serial.println("Introducir nuevo setpoint: ");
    lcd.clear();
    lcd.print("Cambiar setpoint");
  
    while(cambio){  //Empieza la función de cambio de setpoint desde el keypad
      customKey = customKeypad.getKey(); //Se lee la tecla presionada en el keypad que formará parte de los caracteres del setpoint nuevo
      if (customKey){ //Si se presionó una tecla cualquiera...
        if(customKey != 'A' and customKey != 'C'){  //Si la tecla presionada no es A ni C
          wawa[i] = customKey;  //Se asigna el caracter presionado a la i-ésima posición del array wawa empezando desde 0
          Serial.println(wawa); //Se muestra el valor del string wawa hasta el momento
          i++;  //Se incrementa el valor de i para asignar el siguiente caracter presionado a la siguiente posición del array
        }
      }

      if(i == 6 or customKey == 'A'){ //Si ya se han introducido 6 caracteres (el setpoint máximo es 150.00 de seis caracteres) o se presiona A (enter)
                                      //se terminó de asignar el nuevo setpoint, entonces:
        i = 0;  //Se reinicia el índice del array
        ref = atof(wawa); //Se guarda en la variable ref el valor guardado en wawa hasta ahora y convertido a flotante (función atof())
        cambio = false;   //Para salir de la función (bucle) donde se cambia el valor del setpoint

        if(ref<-50.00 or ref>150.00){ //Si el setpoint introducido está fuera del rango del sensor LM35 [-50, 150], entonces se asigna el valor por default al setpoint
          Serial.println("Fuera de escala!");
          lcd.clear();
          lcd.print("Fuera de escala!");
          ref = 50.00;
        }

        else{ //Si el setpoint introducido está dentro del rango, entonces se imprime en el LCD y el monitor serial
          Serial.print("Nuevo setpoint: ");
          Serial.println(ref);
          lcd.clear();
          lcd.print("Nuevo setpoint: ");
          lcd.setCursor(0,1);
          lcd.print(ref);
        }
    
        for(int j = 0; j<7; j++){ //Se limpia el array wawa asignando espacios vacíos a todas sus posiciones
          wawa[j] = '\0';
        }
      }
    }
  }
}
