#include <Servo.h>
#define BUFSIZE    15  // Size of receive buffer (in bytes) (10-byte unique ID + null character)
#define RFID_START  0x02  // RFID Reader Start and Stop bytes
#define RFID_STOP   0x03


int QTILPin = 49;
int QTIMPin = 51;
int QTIRPin = 53;
int thres = 2000;
int del = 100;
int numOfTags = 0;
int count = 0;
int score = 0;
char answer;

Servo S_Left;
Servo S_Right;



void setup() {
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  Serial.flush();   // wait for all bytes to be transmitted to the Serial Monitor
  Serial2.begin(9600);//Initialize the Communication port
  Serial3.begin(9600);
  
  Serial1.begin(9600);

  while (!Serial);   // wait until ready
  Serial.println("RFID Card Reader");

  S_Left.attach(12);
  S_Right.attach(11);

  delay(1000);
}

void loop() {

  // LINE FOLLOWING CODE
  long QTILValue = rcTime(QTILPin);
  long QTIMValue = rcTime(QTIMPin);
  long QTIRValue = rcTime(QTIRPin);

  S_Right.writeMicroseconds(1300);
  S_Left.writeMicroseconds(1700);

  if (QTILValue > thres && QTIMValue > thres && QTIRValue > thres && count < 5) {
    Serial.println(count);
    stopMoving();
    go();
    delay(250);
    count += 1;
  }
  if (QTILValue > thres && QTIMValue > thres && QTIRValue > thres && count >= 5) {
    S_Right.writeMicroseconds(1500);
    S_Left.writeMicroseconds(1500);
    delay(1000000);
  }
  else if (QTILValue < thres && QTIMValue < thres && QTIRValue > thres) {
    right();
  }
  else if (QTILValue > thres && QTIMValue < thres && QTIRValue < thres) {
    left();
  }
  else if (QTILValue < thres && QTIMValue > thres && QTIRValue < thres) {
    go();
    delay(100);
  }
  else if (QTILValue < thres && QTIMValue < thres && QTIRValue < thres) {
    S_Right.writeMicroseconds(1600);
    S_Left.writeMicroseconds(1400);
  }
  else if (QTILValue < thres && QTIMValue > thres && QTIRValue > thres) {
    sright();
  }
  else if (QTILValue > thres && QTIMValue > thres && QTIRValue < thres) {
    sleft();
  }
  delay(100);
}

void stopMoving() {
  S_Right.writeMicroseconds(1500);
  S_Left.writeMicroseconds(1500);
  answer = checkRfid();
  waitForMessage(answer);
}

void left() {
  S_Left.writeMicroseconds(1500);
  S_Right.writeMicroseconds(1450);
}

void sleft() {
  S_Left.writeMicroseconds(1485);
  S_Right.writeMicroseconds(1470);
}

void right() {
  S_Left.writeMicroseconds(1525);
  S_Right.writeMicroseconds(1550);
}

void sright() {
  S_Left.writeMicroseconds(1515);
  S_Right.writeMicroseconds(1530);
}

void go() {
  S_Right.writeMicroseconds(1700);
  S_Left.writeMicroseconds(1300);
}

long rcTime(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delayMicroseconds(230);
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW); long
  time = micros();
  while (digitalRead(pin));
  time = micros() - time;
  return time;
}

char checkRfid() {
  boolean tagPresent = false;
  delay(1000);
  if(Serial1.available() > 0){
      tagPresent = true;
  }
  if(tagPresent==true) {
      Serial.println("Tag Detected");
      outgoing = 'A';
    }
  if(tagPresent==false) {
      Serial.println("No Tag");
      outgoing = 'B';
    }
  unsigned long now = millis ();
  while(millis () - now < 1000){
    Serial1.read();
    }
  Serial2.println(outgoing);
  return outgoing;
  }


void waitForMessage(char answer)
{
  while(1)
  {
    if(Serial2.available() > 0)
    {
    digitalWrite(3,HIGH);
    char re = Serial2.read();
    if(re == 'D' || re == 'C')
    { 
      if(re == 'C')
      {
        score+=10;
        Serial3.flush();
        Serial3.write("A: " + score);
        Serial.println(score);
      }
      Serial.println("break");
      break;
     }
    }
  }
}


