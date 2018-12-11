/**
 *   L2 L1 R1 R2 / 8 9 A0 A1
 */
#include <Servo.h>
Servo servoRight;
Servo servoLeft;

#define L2 8
#define L1 9
#define R1 14 //A0
#define R2 15 //A1

#define L2_correction 0
#define L1_correction 25
#define R1_correction 10
#define R2_correction -300

#define threshold 150
int crossFlag;
int sensingResult;

void setup() {
  Serial.begin(9600);
  servoRight.attach(12);
  servoLeft.attach(13);
  crossFlag = 0;
}
void loop() {
  uint8_t result = getSensingResult();
  
  if(result & 0b10000) turnLeft();
  else {
    result &= 0b01111;
    switch(result)
    {
      case 0b0000 : moveStop(); break;
      case 0b1000 : moveLeft(); break;
      case 0b1100 : moveLeft(); break;
      case 0b1110 : moveLeft(); break;
      case 0b0100 : moveLeft(); break;
      case 0b0110 : moveFoward(); break;
      case 0b0010 : moveRight(); break;
      case 0b0111 : moveRight(); break;
      case 0b0011 : moveRight(); break;
      case 0b0001 : moveRight(); break;
      case 0b1111 : moveFoward(); break;
      default : break;
    }
  }
}
uint8_t getSensingResult() {
  int status = 0;
  long crossBuffer = 0;
  
  sensingResult = RCtime(8)+ L2_correction;
  crossBuffer += sensingResult;
  if( sensingResult > (threshold + L2_correction) ) status |= 1 << 3;
  else status &= 0b0111;
  Serial.print(sensingResult);
  Serial.print(" | ");
  
  sensingResult = RCtime(9)+ L1_correction;
  crossBuffer += sensingResult;
  if( sensingResult > (threshold + L1_correction) ) status |= 1 << 2;
  else status &= 0b1011;
  Serial.print(sensingResult);
  Serial.print(" | ");

  sensingResult = RCtime(A0)+ R1_correction;
  crossBuffer += sensingResult;
  if( sensingResult > (threshold + R1_correction) ) status |= 1 << 1;
  else status &= 0b1101;
  Serial.print(sensingResult);
  Serial.print(" | ");

  sensingResult = RCtime(A1)+ R2_correction;
  crossBuffer += sensingResult;
  if( sensingResult > threshold) status |= 1;
  else status &= 0b1110;
  Serial.println(sensingResult);

  //if(crossBuffer > 1200) status |= 0b10000;

  return status;
}
void moveFoward() {
  servoRight.writeMicroseconds(1700);
  servoLeft.writeMicroseconds(1300);
}
void moveRight() {
  servoRight.writeMicroseconds(1700);
  servoLeft.writeMicroseconds(1500);
}
void moveLeft() {
  servoRight.writeMicroseconds(1500);
  servoLeft.writeMicroseconds(1300);
}
void turnLeft() {
  moveLeft();
  delay(1200);
  crossFlag = 0;
}
void moveStop() {
  servoRight.writeMicroseconds(1495);
  servoLeft.writeMicroseconds(1500);
}
long RCtime(int sensPin)
{
  long result = 0; 
  pinMode(sensPin, OUTPUT); // make pin OUTPUT
  digitalWrite(sensPin, HIGH); // make pin HIGH to discharge capacitor - study the schematic
  delay(1); // wait a ms to make sure cap is discharged
  pinMode(sensPin, INPUT); // turn pin into an input and time till pin goes low
  digitalWrite(sensPin, LOW); // turn pullups off - or it won't work
  while(digitalRead(sensPin))
  { 
    result++; 
  }
  return result;
}
