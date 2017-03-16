#include "HX711.h"
#define RSP 2.0
#define AVG_IT 12
#define DIFFMULT 0.007 

//function definition
void blinkled(int, int, int);
void update_and_send(HX711& a_pin, double& c_value, double resp , int iter_r);
double calc_rate(double new_r, double curr_val, double resp);
void arr_sort(double* arr, int len);

byte runflag = LOW;
int switchPin = 2;
int ledPin = 14;
int redledPin = 15;

HX711 l1;
//double l1val;
long l1val;

HX711 l2;
//double l2val;
long l2val;

HX711 l3;
//double l3val;
long l3val;

HX711 d1;
//double d1val;
long d1val;

HX711 d2;
//double d2val;
long d2val;

//Number of loading conditions
//int N = 5;

void setup() {
  // Initialise loadcells
  l1.begin(11,12);
  l2.begin(9,10);
  l3.begin(7,8);
  d1.begin(3,4);
  d2.begin(5,6);
  l1val = l1.read_average(1);
  l2val = l2.read_average(1);
  l3val = l3.read_average(1);
  d1val = d1.read_average(1);
  d2val = d2.read_average(1);
  
  //initialise pins
  pinMode(switchPin, INPUT);
  pinMode(redledPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  //Procedure
  digitalWrite(redledPin, LOW);
  
  //Green LED stays lit while waiting
  //for connection
  digitalWrite(redledPin, HIGH);
  Serial.begin(9600);
  Serial.println('a');
  char a = 'b';
  while (a != 'a'){
    a = Serial.read();
  }
  //Serial communication has been setup
  digitalWrite(redledPin, LOW);
  //light up green led to indicatestart of data transmission
}

void loop() {
  if (runflag == HIGH){
    l1val = l1.read_average(1);
    l2val = l2.read_average(1);
    l3val = l3.read_average(1);
    d1val = d1.read_average(1);
    d2val = d2.read_average(1);
  }
}


void serialEvent(){
  char inchar;
  while (Serial.available()){
    inchar = (char)Serial.read();
    if (inchar == 'r'){
      //data requested
      Serial.println(l1val);
      Serial.println(l2val);
      Serial.println(l3val);
      Serial.println(d1val);
      Serial.println(d2val);
    }
    else if (inchar == 'c'){
      //begin running
      runflag = HIGH;
      digitalWrite(ledPin, HIGH);
    }
    
    else if (inchar == 'd'){
      //delay
      digitalWrite(redledPin,HIGH);
      delay(4000);
      digitalWrite(redledPin, LOW);
    }
    
    else if (inchar == 'e'){
      //stop all processes
      runflag = LOW;
      digitalWrite(ledPin, LOW);
    }
  }
}


//*****************************************************************************************
/**
  * Flexible filtering >> updates current_value of laod cell
  * a_pin: load cell class
  * c_value: Current value
  * resp: responsiveness
  * iter_r: number of iterations
  * Uses median filtering for outlier rejection
      - takes 5 new readings > sort > median
  * *further work: creep recovery
*/
void update_and_send(HX711& a_pin, double& c_value, double resp , int iter_r){
    int i;
    double a, rate, temp;
    double newval = 0;
    double otl_rej_arr[] = {0, 0, 0, 0, 0};
    newval = a_pin.read_average(iter_r);
    rate = calc_rate(newval, c_value, resp);
    
    // check if rate > 1 
    if (rate > 1){
      /* reset history*/
      a = 1;
    }
    else {
      
      /* set routine for large changes
         to reject outliers */
      if (rate > 0.5){
        /* 
          assign next five values to outlier
          rejection array
        */
        otl_rej_arr[0] = newval;
        for (i = 1; i < 5; i++){
          otl_rej_arr[i] = a_pin.read_average(1); //could be iter_r also
        }
        
        /* Sort array */
        arr_sort((double *) otl_rej_arr, 5);
        // recalculate rate
        rate = calc_rate(otl_rej_arr[2], c_value, resp); 
      }
      a = rate;
    }
    temp = c_value; //not needed
    c_value = temp + ((newval - temp) * a);
    Serial.println(c_value);
}

/* determine rate of change */
double calc_rate(double new_r, double curr_val, double resp){
  // diff = abs(new value - current value)
  double diff = abs(new_r -  curr_val) * DIFFMULT;
  // y = ( 1 - 1/(1 + diff) ) * responsiveness 
  return (1 - (1/(diff + 1))) * resp;
}

void arr_sort(double* arr, int len){
  double temp;
  for (int i = 0; i < (int) (len/2) + 1; i++){
    for (int j = len - 1; j > i ; j--){
      if (arr[j - 1] > arr[j]){
        //swap the lowest val down to the i_th position
        temp = arr[j - 1];
        arr[j - 1] = arr[j];
        arr[j] =  temp;
      }
    }
  }
}
//*****************************************************************************************

void blinkled(int pinl, int times, int milsec){
  for (int i = 0; i < times; i++){
    delay(milsec);
    digitalWrite(pinl, LOW);
    delay(milsec);
    digitalWrite(pinl, HIGH);
  }
  digitalWrite(pinl, LOW);
}
  
