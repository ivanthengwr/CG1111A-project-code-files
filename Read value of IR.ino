//Read IR 
int A = A2;
int B = A1;

int readVal;
int readPin = A0;
float V2 = 0;
void setup() {
  pinMode(readPin, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(A,LOW);
  digitalWrite(B, LOW);
readVal = analogRead(readPin);
V2 = (5./1023.)*readVal;
Serial.println(V2);
delay(100);
}

//read ambient and with emitter.
/*
int A = A2;
int B = A1;

int readVal;
int readPin = A0;
float V2Ambient = 0;
float V2diff = 0;
float V2Emitter = 0;
void setup() {
  pinMode(readPin, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(A,HIGH);
  digitalWrite(B, LOW);
  delay(100);
readVal = analogRead(readPin);
V2Ambient = (5./1023.)*readVal;
//Serial.print("V2Ambient:");
//Serial.println(V2Ambient);
delay(100);

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  delay(100);
readVal = analogRead(readPin);
V2Emitter = (5./1023.)*readVal;
//Serial.print("V2Emitter:");
//Serial.println(V2Emitter);
V2diff = V2Ambient-V2Emitter;
Serial.print("V2diff:");
Serial.println(V2diff);
delay(100);

}
*/
