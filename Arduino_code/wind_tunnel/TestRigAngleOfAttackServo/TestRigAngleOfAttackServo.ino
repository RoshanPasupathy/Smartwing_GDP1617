// zoomkat 12-13-11 serial servo (2) test
// for writeMicroseconds, use a value like 1500
// for IDE 1.0
// Powering a servo from the arduino usually DOES NOT WORK.
// two servo setup with two servo commands
// send eight character string like 15001500 or 14501550
// use serial monitor to test

#include <Servo.h> 
String readString, number;
Servo Left;  // create servo object to control a servo 
Servo Right;
Servo FlapLeft;
Servo FlapRight;
int WingOldPos=0;
int FlapOldPos=0;
int LeftCalib=4;
int RightCalib=8;
int FlapLeftCalib=0;
int FlapRightCalib=0;


void setup() {
  Serial.begin(9600);
  Serial.println('a');
  char a = 'b';
  //wait to receive data
  while (a != 'a'){
    a = Serial.read();
  }
  //Serial connection established
  Left.attach(9);  //the pin for the servo control 
  Right.attach(10);
  FlapLeft.attach(6);
  FlapRight.attach(5);
  WingServoMove(WingOldPos,WingOldPos);
  FlapServoMove(FlapOldPos,FlapOldPos);
  //Serial.println("two-servo-test-1.0"); // so I can keep track of what is loaded  
}

void loop() {

  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }

  if (readString.length() >0) {
      Serial.println(readString); //see what was received
      if(readString.substring(0,1)=="c"){
        if(readString.substring(1,2)=="l"){
          LeftCalib=readString.substring(2,4).toInt();
          //Serial.print("left changed to ");
          //Serial.println(LeftCalib);
        }
        else if(readString.substring(1,2)=="r"){
          RightCalib=readString.substring(2,4).toInt();
          //Serial.print("right changed to ");
          //Serial.println(RightCalib);
        }
        WingServoMove(WingOldPos,WingOldPos+10);
        WingServoMove(WingOldPos+10,WingOldPos);        
      }
      else if(readString.substring(0,1)=="w"){
          // expect a string like 07002100 containing the two servo positions      
        number = readString.substring(1, 4); //get the first four characters
        
        Serial.println(number);  //print to serial monitor to see parsed results
  
        int n1 = number.toInt();
  
        //Serial.print("the number is : ");
        //Serial.print(n1);  //print to serial monitor to see number results
        //Serial.print("  ");
        //Serial.println(WingOldPos);
        WingServoMove(WingOldPos,n1);
        WingOldPos=n1;
      }
      else if(readString.substring(0,1)=="f"){
          // expect a string like 07002100 containing the two servo positions      
        number = readString.substring(1, 4); //get the first four characters
        
        //Serial.println(number);  //print to serial monitor to see parsed results
  
        int n1 = number.toInt();
  
        //Serial.print("the number is : ");
        //Serial.print(n1);  //print to serial monitor to see number results
        //Serial.print("  ");
        //Serial.println(FlapOldPos);
        FlapServoMove(FlapOldPos,n1);
        FlapOldPos=n1;
      }
      else{}
      
      
    readString="";
  } 
}
void WingServoMove(int current, int newpos){
  current=current+90;
  newpos=newpos+90;
  if(newpos-current>0){
     for (int pos = current; pos <= newpos; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      Left.write(180-pos+LeftCalib);
      Right.write(pos+RightCalib);// tell servo to go to position in variable 'pos'
      delay(15);
     } 
  }
    else if (newpos-current<0) {
      for (int pos = current; pos >= newpos; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      Left.write(180-pos+LeftCalib);
      Right.write(pos+RightCalib);// tell servo to go to position in variable 'pos'
      delay(15);
     } 
      // waits 15ms for the servo to reach the position
  }
  else{
    Left.write(180-current+LeftCalib);
    Right.write(current+RightCalib);
    } 
}

void FlapServoMove(int current, int newpos){
  current=current+90;
  newpos=newpos+90;
  if(newpos-current>0){
     for (int pos = current; pos <= newpos; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      FlapLeft.write(180-pos+FlapLeftCalib);
      FlapRight.write(pos+FlapRightCalib);// tell servo to go to position in variable 'pos'
      delay(15);
     } 
  }
    else if (newpos-current<0) {
      for (int pos = current; pos >= newpos; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      FlapLeft.write(180-pos+FlapLeftCalib);
      FlapRight.write(pos+FlapRightCalib);// tell servo to go to position in variable 'pos'
      delay(15);
     } 
      // waits 15ms for the servo to reach the position
  }
  else{
    FlapLeft.write(180-current+FlapLeftCalib);
    FlapRight.write(current+FlapRightCalib);
    } 
}

