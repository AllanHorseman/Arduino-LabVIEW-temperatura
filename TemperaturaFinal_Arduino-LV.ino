/**************************************************************************
*Universidad Autónoma Metropolitana, Unidad Lerma
*25 de enero de 2022
*Allan Daniel Macedo Méndez
***************************************************************************
*Práctica final: Acondicionamiento de sensor de tempeatura.
*Acondicionamiento de señales eléctrias.
***************************************************************************
*DESCRIPCIÓN: Este código obtiene la señal de un sensor de temperatura LM35
*previamente acondicionada mediante un amplificador operacional no inversor.
*Si la temperatura supera un valor especificado (setpoint) se encenderá un 
*motor. El valor de la temperatura medida y el setpoint se muestran en una 
*pantalla LCD y en la interfaz gráfica del programa "Temperatura y motor"
*en LabVIEW, desde donde se puede encender el motor meidante un interruptor
*virtual. El Setpoint puede ser modificado en cualquier momento desde la
*interfaz de LabVIEW o desde un Keypad 4x4.
**************************************************************************/

#include <Keypad.h>   //Librería del Keypad

#include <LiquidCrystal_I2C.h>
#include <Wire.h>     // librería de comunicación por I2C
#include <LCD.h>      // librería para funciones de LCD

/***************************    LCD SETUP    ******************************
*Se utiliza una pantalla LCD 16x2 y un módulo I2C para mostrar el setpoint 
*y la temperatura medida.
*Se declara la dirección del módulo I2C y la conexión entre sus pines y los
*del LCD de la siguiente forma: 
LiquidCrystal_I2C lcd (DIR, E, RW, RS, D4, D5, D6, D7)
**************************************************************************/

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); 

/***************************   KEYPAD SETUP   *****************************
*Se utiliza un Keypad 4x4 para modificar el setpoint. Cuando se presiona la
*tecla 'C', se detiene el programa principal y se ejecuta una función para
*que se ingrese el nuevo valor del setpoint. Al ingresar 6 caracteres
*(número más grande: '1','5','0','.','0','0') o presionar la tecla 'A', se 
*finaliza la función ""
**************************************************************************/

const byte ROWS = 4;    //4 filas
const byte COLS = 4;    //4 columnas
char wawa[7];           //Array donde se almacenarán los caracteres que
                        //conformarán el valor del nuevo setpoint desde el keypad
                        
int i = 0;              //Índice para el Array       
float setpoint = 50.00; //Valor incial del setpoint

bool cambio = true;     //Variable que determina si se está modificando
                        //el setpoint o no

//definición de los símbolos de los botones del Keypad
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},  //Tecla A es igual a enter
  {'4','5','6','B'},
  {'7','8','9','C'},  //Tecla C es igual a cambiar valor del setpoint
  {'.','0','-','D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8}; //Pines a los que se conectan las filas:
                                      //{R1, R2, R3, R4}
byte colPins[COLS] = {7, 6, 5, 4};    //Pines a los que se conectan las columnas:
                                      //{C1, C2, C3, C4}

//Inicialización de una instancia (customKeypad) de clase NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


int sensor, lab;
const int relay = 13, periodo = 500;
unsigned long ahora = 0;
float temp;

void labview() {
  Serial.println(temp); //Imprimir la temperatura en interfaz de LabVIEW
  lab = Serial.read();  //Leer estado del interruptor virtual de LabVIEW

  if(lab=='a')
    {digitalWrite(relay,HIGH);}
  if(lab=='b')
    {digitalWrite(relay,LOW);}
    
  if (temp >= setpoint){         //Si la temperatura supera el setpoint...
    lcd.setCursor(0,0);
    lcd.print("Temp Ref: ");     //Muestra en LCD el setpoint
    lcd.print (setpoint);
    lcd.setCursor (0,1);         //Cambia de renglon en el LCD
    lcd.print("Temp alta:");     //muestra el valor de la temperatura medida
    lcd.print(temp);             
  }
    
  else{                        //Si la temperatura no supera el setpoint...
    lcd.setCursor(0,0);
    lcd.print("Temp Ref:");    //Muestra en LCD el setpoint
    lcd.print (setpoint);
    lcd.setCursor (0,1);       //Cambio de renglon en el LCD
    lcd.print ("Temp baja:");  //muestra el valor de la temperatura medida
    lcd.print (temp);          
  }
}

void newSetpoint(){
  Serial.println("Introducir nuevo setpoint: ");
  lcd.clear();
  lcd.print("Cambiar setpoint");
  
  while(cambio){  //Empieza la función de cambio de setpoint desde el keypad
    char customKey = customKeypad.getKey(); //Se lee la tecla presionada en el keypad que formará parte de los caracteres del setpoint nuevo
    
    if (customKey and customKey != 'A' and customKey != 'C'){ //Si se presionó una tecla cualquiera...
      wawa[i] = customKey;  //Se asigna el caracter presionado a la i-ésima posición del array wawa empezando desde 0
      Serial.println(wawa); //Se muestra el valor del string wawa hasta el momento
      lcd.setCursor(0,1);
      lcd.print(wawa);
      i++;  //Se incrementa el valor de i para asignar el siguiente caracter presionado a la siguiente posición del array
    }

    if(i == 6 or customKey == 'A'){ //Si ya se han introducido 6 caracteres (el setpoint máximo es 150.00 de seis caracteres) o se presiona A (enter)
                                    //se terminó de asignar el nuevo setpoint, entonces:
      i = 0;  //Se reinicia el índice del array
      setpoint = atof(wawa); //Se guarda en la variable setpoint el valor guardado en wawa hasta ahora y convertido a flotante (función atof())
      cambio = false;   //Para salir de la función (bucle) donde se cambia el valor del setpoint

      if(setpoint<-50.00 or setpoint>150.00){ //Si el setpoint introducido está fuera del rango del sensor LM35 [-50, 150], entonces se asigna el valor por default al setpoint
        Serial.println("Fuera de rango!");
        lcd.clear();
        lcd.print("Fuera de rango!");
        setpoint = 50.00;
        }

      else{ //Si el setpoint introducido está dentro del rango, entonces se imprime en el LCD y el monitor serial
        Serial.print("Nuevo setpoint: ");
        Serial.println(setpoint);
        lcd.clear();
        lcd.print("Nuevo setpoint: ");
        lcd.setCursor(0,1);
        lcd.print(setpoint);
      }

      delay(1000);
      lcd.clear();
      for(int j = 0; j<7; j++){ //Se limpia el array wawa asignando espacios vacíos a todas sus posiciones
        wawa[j] = '\0';
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  lcd.setBacklightPin(3,POSITIVE);  // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH);           // habilita iluminación posterior de LCD
  lcd.begin(16,2);                  // 16 columnas por 2 lineas para LCD 1602A
  lcd.clear();

  pinMode(relay, OUTPUT);
}

/************************    LOOP PRINCIPAL    **************************/

void loop() {
  cambio = true;
  char customKey = customKeypad.getKey(); //Leer la tecla presionada
                                          //desde el keypad 
  
  sensor = analogRead(A0);   //Leer la señal analógica del sensor (bits)
  temp = sensor*150.0/1023;  //Convertir el valor analógico a temperatura
                             //con la resolución del arduino (5V/1023bits)
                             //y del sensor (150°C/5V)

  if(Serial.available()){  //Si hay conexión en comunicación serial...
    labview();   //Llamar a la función "labview"
  }

  if(customKey == 'C'){    //Si se presionó la tecla 'C'...
    newSetpoint();
  }

  if(millis() > ahora + periodo){  //Limpiar el LCD cada 500 ms (periodo)
    ahora = millis();
    lcd.clear();
  }
}
