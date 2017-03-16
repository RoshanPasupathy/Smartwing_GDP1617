#include "HX711.h"

//function definition
void calibration_routine(void);

volatile byte state = LOW;
int switchPin = 2;
int ledPin = 14;
int redledPin = 15;

HX711 l1;
HX711 l2;
HX711 l3;
HX711 d1;
HX711 d2;

//Number of loading conditions
//int N = 5;

void setup() {
  // Initialise loadcells
  l1.begin(11,12);
  l2.begin(9,10);
  l3.begin(7,8);
  d1.begin(3,4);
  d2.begin(5,6);
  
  //initialise pins
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(redledPin, OUTPUT);
  
  //Procedure
  digitalWrite(redledPin, LOW);
  
  //Green LED stays lit while waiting
  //for connection
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);
  
  Serial.println('a');
  char a = 'b';
  while (a != 'a'){
    a = Serial.read();
  }
  digitalWrite(ledPin, LOW);
  
  calibration_routine();
}

void loop() {
  // put your main code here, to run repeatedly:

}

/**
  * Synchronises matlab and arduino serial 
    to prevent overflow
  * Synchronisation done by acknowledgement chars
  * Arduino prints data -> matlab reads when it detects
    presence of bytes in serial buffer
  * Acknowledgement procedure:
    - Arduino waits for matlab acknowledgement
    - When available -> read 
    - If unexpected char -> light Red LED
      else blink green LED twice
*/
void calibration_routine(){
  //Send offset values
  //Blink green led and red led 3 times each
  //indicates start of calibration routine
  blinkled(ledPin, 3, 200);
  blinkled(redledPin, 3, 200);
  volatile byte runflag = HIGH;
  
  char val = 'a'; 
  Serial.println(l1.read_average(10));
  Serial.println(l2.read_average(10));
  Serial.println(l3.read_average(10));
  Serial.println(d1.read_average(10));
  Serial.println(d2.read_average(10));
  
  //Waits for matlab to read all data 
  // and acknowledge 
  // acknowledgement char = 'c'
  while (Serial.available() == 0){
  }
  if (Serial.available() > 0){
    val = Serial.read();
    if (val == 'c'){
      //data received as expected
      //blink green twice
      blinkled(ledPin, 2, 300);
    }
    else {
      //Unexpected output -> Something went wrong 
      digitalWrite(redledPin, HIGH);
    }
  }
  //Green led lights up to indicate ready for next stage
  digitalWrite(ledPin, HIGH);
  //Send data for each loading condition
  //for (int i = 0; i < N; i++){
  while (runflag == HIGH){
    digitalWrite(ledPin, HIGH);
    state = digitalRead(switchPin);
    //Halt till load is placed
    while (digitalRead(switchPin) == state){
    }
    delay(3000);
    //Switch toggled -> load placed -> green led goes off
    digitalWrite(ledPin, LOW);
    
    //send data
    Serial.println(l1.read_average(10));
    Serial.println(l2.read_average(10));
    Serial.println(l3.read_average(10));
    Serial.println(d1.read_average(10));
    Serial.println(d2.read_average(10));
    
    //before proceeding check if data is received by matlab
    while (Serial.available() == 0){
    }
    if (Serial.available() > 0){
      val = Serial.read();
      if (val == 'g'){
        //blink green twice
        blinkled(ledPin, 2, 300);  
      }
      else if (val == 'e'){
        runflag = LOW;
      }
      else {
        //Received something but it's not expected
        digitalWrite(redledPin, HIGH);
        //abort
      }
    }
  }
  blinkled(redledPin, 3, 300);
  blinkled(ledPin, 3, 300);
}

void blinkled(int pinl, int times, int milsec){
  for (int i = 0; i < times; i++){
    delay(milsec);
    digitalWrite(pinl, LOW);
    delay(milsec);
    digitalWrite(pinl, HIGH);
  }
  digitalWrite(pinl, LOW);
}
  
  
  
  
