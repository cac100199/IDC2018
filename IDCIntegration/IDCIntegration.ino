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

Servo S_Left;
Servo S_Right;



void setup() {

  Serial.begin(9600);
  Serial.flush();   // wait for all bytes to be transmitted to the Serial Monitor
  Serial2.begin(9600);//Initialize the Communication port
  Serial1.begin(9600); // Inintialize the RFID serial port

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
    stopMoving();
    go();
    count += 1;
    Serial.println(count);
    delay(500);
  }
  if (QTILValue > thres && QTIMValue > thres && QTIRValue > thres && count >= 5) {
    stopMoving();
    delay(100000000);
  }
  else if (QTILValue < thres && QTIMValue < thres && QTIRValue > thres) {
    right();
  }
  else if (QTILValue > thres && QTIMValue < thres && QTIRValue < thres) {
    left();
  }
  else if (QTILValue < thres && QTIMValue > thres && QTIRValue < thres) {
    go();
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
  checkRfid();
  S_Right.writeMicroseconds(1500);
  S_Left.writeMicroseconds(1500);
  delay(1000);
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

void checkRfid() {
  char rfidData[BUFSIZE];  // Buffer for incoming data
  char offset = 0;         // Offset into buffer
  rfidData[0] = 0;         // Clear the buffer
  while (1) {

    if (Serial1.available() > 0) { // If there are any bytes available to read, then the RFID Reader has probably seen a valid tag
//
//      rfidData[offset] = Serial1.read();  // Get the byte and store it in our buffer
//      Serial1.flush();
//      if (rfidData[offset] == RFID_START)    // If we receive the start byte from the RFID Reader, then get ready to receive the tag's unique ID
//      {
//        offset = -1;     // Clear offset (will be incremented back to 0 at the end of the loop)
//      }
//      else if (rfidData[offset] == RFID_STOP)  // If we receive the stop byte from the RFID Reader, then the tag's entire unique ID has been sent
//      {
//        rfidData[offset] = 0; // Null terminate the string of bytes we just received              // Break out of the loop
//      }

      offset++;  // Increment offset into array
      if (offset >= BUFSIZE) offset = 0; // If the incoming data string is longer than our buffer, wrap around to avoid going out-of-bound
      Serial.println("detected");
      Serial.flush();
      delay(100);
      //    return("TAG DETECTED");
    }
    else {
      Serial.println("none");
      //    return ("No Tag Detected");
    }
    delay(200);
  }
}


