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
