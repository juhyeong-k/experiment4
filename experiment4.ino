/**
 *   L2 L1 R1 R2 / 8 9 A0 A1
 */
#include <Servo.h>
#include <SoftwareSerial.h> //시리얼통신 라이브러리 호출
Servo servoRight;
Servo servoLeft;

int blueTx=2;   //Tx (보내는핀 설정)
int blueRx=3;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언

#define L2 8
#define L1 9
#define R1 14 //A0
#define R2 15 //A1

#define L2_correction 10
#define L1_correction 15
#define R1_correction 10
#define R2_correction -300

#define threshold 250
#define APEX_THRESHOLD 600
int crossFlag;
int sensingResult;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); //블루투스 시리얼
  servoRight.attach(13);
  servoLeft.attach(12);
  crossFlag = 0;
}
void loop() {
  uint8_t result = getSensingResult();
  if(result & 0b10000) turnLeft();
  else {
    result &= 0b01111;
    switch(result)
    {
      case 0b0000 : turnLeft(); break;
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
  
  sensingResult = RCtime(L2)+ L2_correction;
  crossBuffer += sensingResult;
  if( sensingResult > (threshold + L2_correction) ) status |= 1 << 3;
  else status &= 0b0111;
  Serial.print(sensingResult);
  Serial.print(" | ");
  
  sensingResult = RCtime(L1)+ L1_correction;
  crossBuffer += sensingResult;
  if( sensingResult > (threshold + L1_correction) ) status |= 1 << 2;
  else status &= 0b1011;
  Serial.print(sensingResult);
  Serial.print(" | ");

  sensingResult = RCtime(R1)+ R1_correction;
  crossBuffer += sensingResult;
  if( sensingResult > (threshold + R1_correction) ) status |= 1 << 1;
  else status &= 0b1101;
  Serial.print(sensingResult);
  Serial.print(" | ");

  sensingResult = RCtime(R2)+ R2_correction;
  crossBuffer += sensingResult;
  if( sensingResult > threshold) status |= 1;
  else status &= 0b1110;
  Serial.println(sensingResult);

  if( (crossBuffer < APEX_THRESHOLD) && (status & 0b1111) ) status |= 0b10000;

  return status;
}
void moveFoward() {
  servoLeft.writeMicroseconds(1700);//1700
  servoRight.writeMicroseconds(1300);//1300
  Serial.println("moveFoward");
}
void moveRight() {
  servoLeft.writeMicroseconds(1700);//1700
  servoRight.writeMicroseconds(1450);//1500
  Serial.println("moveRight");
}
void moveLeft() {
  servoLeft.writeMicroseconds(1550);//1500
  servoRight.writeMicroseconds(1300);//1300
  Serial.println("moveLeft");
}
void turnLeft() {
  servoLeft.writeMicroseconds(1450);//1600
  servoRight.writeMicroseconds(1300);//1400
  Serial.println("turnLeft");
}
void moveStop() {
  servoLeft.writeMicroseconds(1495);//
  servoRight.writeMicroseconds(1500);//
  Serial.println("moveStop");
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
