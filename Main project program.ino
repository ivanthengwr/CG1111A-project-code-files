#include <MeMCore.h>

MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_2);
MeDCMotor motorRight(M1);
MeDCMotor motorLeft(M2);

uint8_t motorSpeedstr = 1500;
uint8_t motorSpeed1 = 150;  
uint8_t motorSpeed2 = 1000;

uint8_t motorSpeed3 = 1000;
uint8_t motorSpeed4 = 150;

int A = A2;
int B = A1;
float V2 = 0;
int readVal = 0;
int readPin = A0;
int delay_IR = 500;

void setup() {
  pinMode(readPin, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(38400); 
}

void loop() {

  int sensorState = lineFinder.readSensors();

  //Turn on the Y0 which IR sensor
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);

  //Read the IR at Y0 and generate V2
  readVal = analogRead(readPin);
  V2 = (5. / 1023.) * readVal;
  Serial.println(V2);
  

  if(sensorState == S1_IN_S2_IN) {
    motorRight.stop();
    motorLeft.stop();
    delay(20000);
  }

  motorRight.run(motorSpeedstr);
  motorLeft.run(-motorSpeedstr);

//Serial.println(ultraSensor.distanceCm());
  
 if (ultraSensor.distanceCm() <= 8){
    //m2 left motor, m1 right motor
    motorRight.run(motorSpeed2);
    motorLeft.run(-motorSpeed1);
    delay(100);
  } 

 if ( V2 < 4.45 && V2 >= 4.2){
    motorRight.run(motorSpeed4);
    motorLeft.run(-motorSpeed3);
    delay(10);
  }
}
