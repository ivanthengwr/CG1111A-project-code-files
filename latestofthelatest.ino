#include <MeMCore.h>

//5.90-6.01

MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_2);
MeDCMotor motorRight(M1);
MeDCMotor motorLeft(M2);
MeBuzzer buzzer;
// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds

uint8_t motorSpeedstr = 1500;
uint8_t motorSpeedstrLeft = 1500;
uint8_t motorSpeedstrRight = 1520;
uint8_t motorSpeed1 = 150;  
uint8_t motorSpeed2 = 1000;
uint8_t motorspeedturn = 492;

uint8_t motorSpeed3 = 1000;
uint8_t motorSpeed4 = 150;

float colourArray[] = {0,0,0};
float blackArray[] = {715,491,455};
float greyDiff[] = {251,420,432};

int A = A2;
int B = A1;
float V2Ambient = 0;
float V2diff = 0;
float V2Emitter = 0;
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
// readVal = analogRead(readPin);
 // V2 = (5. / 1023.) * readVal;
  //Serial.println(V2);
  

  if(sensorState == S1_IN_S2_IN) {
    motorRight.stop();
    motorLeft.stop();
    colour_sense_and_turn();
  }

  motorRight.run(motorSpeedstrRight);
  motorLeft.run(-motorSpeedstrLeft);

//Serial.println(ultraSensor.distanceCm());
  
 if (ultraSensor.distanceCm() <= 8.9){
    //m2 left motor, m1 right motor
    motorRight.run(motorSpeed2);
    motorLeft.run(-motorSpeed1);
    delay(25);
  } 
readValue();
 if ( V2diff > 0.53){ 
    motorRight.run(motorSpeed1);
    motorLeft.run(-motorSpeed2);
    delay(25);
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
  if(colourArray[0]>240 && colourArray[1]>240 && colourArray[2]>240){ //WHITE 
    motorRight.stop();
    motorLeft.stop();
    playmusic();
    exit(0);
  }

  if(colourArray[0] > 220)
  {
   if(colourArray[1] > colourArray[2] && (colourArray[1]-colourArray[2])>20) //Orange Uturn
   {
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(580);
   }
   else//Red left turn
   {
     motorRight.run(motorspeedturn);
     motorLeft.run(motorspeedturn);
     delay(320);
   }

  }else if(colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) //Green right turn
  {
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(280);
  }else if(colourArray[1]>200 && colourArray[2]>200)//Blue 2right turn
  {
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(310);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(600);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(310);
  }else{ //Purple 2left turn
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(310);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(715);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(295);
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


void readValue() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(A,HIGH);
  digitalWrite(B, LOW);
  delay(2);
readVal = analogRead(readPin);
V2Ambient = (5./1023.)*readVal;
//Serial.print("V2Ambient:");
//Serial.println(V2Ambient);

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  delay(2);
readVal = analogRead(readPin);
V2Emitter = (5./1023.)*readVal;
//Serial.print("V2Emitter:");
//Serial.println(V2Emitter);
V2diff = V2Ambient-V2Emitter;
Serial.print("V2diff:");
Serial.println(V2diff);


}

int melody[] = {
392,//5  
392,//5
440,//6
392,//5
523,//1.
494,//7
0,
};

int noteDurations[] = {
  8,8,4,4,4,4,
};

void playmusic()
{
    for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    buzzer.tone(8, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
  }
}
