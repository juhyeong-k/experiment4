/**
 *   L2 L1 R1 R2 / 8 9 A0 A1
 */
#include <Servo.h>
Servo servoRight;
Servo servoLeft;

#define L2 8
#define L1 9
#define R1 A0
#define R2 A1

#define L2_correction 0
#define L1_correction 25
#define R1_correction 10
#define R2_correction -120

int threshold;
int crossFlag;

void setup() {
  Serial.begin(9600);
  servoRight.attach(12);
  servoLeft.attach(13);
  threshold = 110;
  crossFlag = 0;
}
void loop() {
  int status = 0;
  long crossBuffer = 0;
  moveStop();
  crossBuffer += RCtime(8);
  if( RCtime(8) > threshold) status |= 1 << 3;
  else status &= 0b0111;
  Serial.print(RCtime(8));
  Serial.print(" | ");

  crossBuffer += RCtime(9);
  if( RCtime(9) > threshold) status |= 1 << 2;
  else status &= 0b1011;
  Serial.print(RCtime(9));
  Serial.print(" | ");

  crossBuffer += RCtime(A0);
  if( RCtime(A0) > threshold) status |= 1 << 1;
  else status &= 0b1101;
  Serial.print(RCtime(A0));
  Serial.print(" | ");

  crossBuffer += RCtime(A1);
  if( RCtime(A1) > threshold) status |= 1;
  else status &= 0b1110;
  Serial.println(RCtime(A1));
  /*
  moveLeft();
  
  if(crossBuffer > 1200) turnLeft();
  else {
    switch(status)
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
  */
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
