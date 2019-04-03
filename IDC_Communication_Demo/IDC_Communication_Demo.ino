void setup() {
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(6,INPUT);
  Serial2.begin(9600);

}

void loop() {
  if(digitalRead(6)){
    char outgoing = '7';
    Serial2.print(outgoing);
    Serial2.println();
    digitalWrite(4,HIGH);
  }
  if(Serial2.available() && Serial2.read()!= '7'){
    char incoming = Serial2.read();
    digitalWrite(3,HIGH);
  }
  else {
    digitalWrite(3,LOW);
  }
  delay(50);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
}
