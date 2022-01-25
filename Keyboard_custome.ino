/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char wawa[7];
int i = 0;
float ref;
bool cambio = true;

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'.','0','-','D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  cambio = true;
  char customKey = customKeypad.getKey();

  if(customKey == 'C'){
    Serial.println("Introducir nuevo setpoint: ");
  
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
        }

        else{
          Serial.print("Nuevo setpoint: ");
          Serial.println(ref);
        }
    
        for(int j = 0; j<7; j++){
          wawa[j] = '\0';
        }
      }
    }
  }
}
