#include <MeMCore.h>

MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_2);
MeDCMotor motorRight(M1);
MeDCMotor motorLeft(M2);
// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds

uint8_t motorSpeedstr = 1500;
uint8_t motorSpeed1 = 150;  
uint8_t motorSpeed2 = 1000;
uint8_t motorspeedturn = 492;

uint8_t motorSpeed3 = 1000;
uint8_t motorSpeed4 = 150;

float colourArray[] = {0,0,0};
float blackArray[] = {193,218,178};
float greyDiff[] = {513,487,404};

int A = A2;
int B = A1;
float V2 = 0;
int readVal = 0;
int readPin = A0;
int delay_IR = 500;
int LDR = A3; 

void setup() {
  pinMode(readPin, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(LDR, INPUT); 
  Serial.begin(38400); 
}

void loop() {

  int sensorState = lineFinder.readSensors();

  //Turn on the Y0 which is the IR sensor
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);

  //Read the IR at Y0 and generate V2
  readVal = analogRead(readPin);
  V2 = (5. / 1023.) * readVal;
  Serial.println(V2);
  

  if(sensorState == S1_IN_S2_IN) {
    motorRight.stop();
    motorLeft.stop();
    colour_sense_and_turn();
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

void colour_sense_and_turn(){

    for(int c = 0;c<=2;c++){    
     if(c==0){
      turn_on_red();
     }
     else if(c==1){
      turn_on_green();
     }
     else{
      turn_on_blue();
     }
    delay(RGBWait);
//get the average of 5 consecutive readings for the current colour and return an average 
    colourArray[c] = getAvgReading(5);
//the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c])/(greyDiff[c])*255;
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);  //turn off the current LED colour
    delay(RGBWait);
  }

  if(colourArray[0] > 200)
  {
   if(colourArray[2] > colourArray[1])//Red left turn
   {
     motorRight.run(motorspeedturn);
     motorLeft.run(motorspeedturn);
     delay(260);
   }
   if(colourArray[1] > colourArray[2]) //Orange Uturn
   {
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(520);
   }
  }else if(colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) //Green right turn
  {
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(260);
  }else if(colourArray[2] > colourArray[1] && colourArray[2] > colourArray[0])//Blue 2right turn
  {
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(260);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(550);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(260);
  }else{ //Purple 2left turn
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(280);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(550);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(280);
  }
  motorRight.stop();
  motorLeft.stop();
  delay(50);
}



/////HELPER FUNCTIONS///////
void turn_on_red(){
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
}

void turn_on_blue(){
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
}

void turn_on_green(){
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
}
  


int getAvgReading(int times){      
//find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
//take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
     reading = analogRead(LDR);
     total = reading + total;
     delay(LDRWait);
  }

//calculate the average and return it
  return total/times;
}
