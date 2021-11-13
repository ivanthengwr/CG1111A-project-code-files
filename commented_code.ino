#include <MeMCore.h>

MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_2);
MeDCMotor motorRight(M1);
MeDCMotor motorLeft(M2);
MeBuzzer buzzer;
// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds

uint8_t motorSpeedstrLeft = 1500;//Speed of left motor when moving straight
uint8_t motorSpeedstrRight = 1520;//Speed of right motor when moving straight
uint8_t motorSpeed1 = 150; //for turning away from walls
uint8_t motorSpeed2 = 1000;//for turning away from walls
uint8_t motorspeedturn = 492;//for turning after sensing colours

float colourArray[] = {0,0,0};//Array to store the values of colour reflectivity of red, green and blue LED for colour sensing
float blackArray[] = {715,491,455};//pre-calibrated values for blackArray
float greyDiff[] = {251,420,432};//pre-calibrated values for greyDiff

int A = A2; //Select input pin A
int B = A1; ////Select input pin B
float V2Ambient = 0;//Voltage value of IR detector with IR emitter turned off
float V2diff = 0;// V2Ambient - V2Emitter
float V2Emitter = 0;//Voltage value of IR detector with IR emitter turned on
int readVal = 0;//Analog value of IR detector
int readPin = A0;//IR detector
int LDR = A3; //LDR

void setup() {
  pinMode(readPin, INPUT);//To read infrared detector values
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(LDR, INPUT); //To read LDR values
  Serial.begin(38400); 
}

void loop() {

  int sensorState = lineFinder.readSensors();

  //Stop when mbot detects black strip and calls function to detect colour and turn
  if(sensorState == S1_IN_S2_IN) {
    motorRight.stop();
    motorLeft.stop();
    colour_sense_and_turn();
  }

  //Mbot starts running
  motorRight.run(motorSpeedstrRight);
  motorLeft.run(-motorSpeedstrLeft);

 //Mbot turns left if it goes too close to the wall on the right side
 if (ultraSensor.distanceCm() <= 8.9){
    motorRight.run(motorSpeed2);
    motorLeft.run(-motorSpeed1);
    delay(25);
  } 
 readValue();//Calls function which calculates the voltage difference(V2diff) between ambient IR and when emitter is turned on
 
 //Mbot turns right if it gets too close to the wall on the left side
 if ( V2diff > 0.53){ 
    motorRight.run(motorSpeed1);
    motorLeft.run(-motorSpeed2);
    delay(25);
  }
}

//Sense the colour and execute the corresponding turn
void colour_sense_and_turn(){
    for(int c = 0;c<=2;c++){ //Turn on each LED one by one
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
  if(colourArray[0]>240 && colourArray[1]>240 && colourArray[2]>240){ //Mbot detects White and stops. Plays celebratory tune
    motorRight.stop();
    motorLeft.stop();
    playmusic();
    exit(0);
  }

  if(colourArray[0] > 220)
  {
   if(colourArray[1] > colourArray[2] && (colourArray[1]-colourArray[2])>20) //detects Orange, U-turn
   {
    motorRight.run(motorspeedturn);
    motorLeft.run(motorspeedturn);
    delay(580);
   }
   else//detects Red, left turn
   {
     motorRight.run(motorspeedturn);
     motorLeft.run(motorspeedturn);
     delay(320);
   }

  }else if(colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) //detects Green, right-turn
  {
    motorRight.run(-motorspeedturn);
    motorLeft.run(-motorspeedturn);
    delay(280);
  }else if(colourArray[1]>200 && colourArray[2]>200)//detects Blue 2 successive right-turns
  {
    motorRight.run(-motorspeedturn);//Right-turn
    motorLeft.run(-motorspeedturn);
    delay(310);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);//Mbot moves forward
    motorLeft.run(-motorspeedturn);
    delay(600);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(-motorspeedturn);//Right-turn again
    motorLeft.run(-motorspeedturn);
    delay(310);
  }else{ //detects Purple 2 successive left-turn
    motorRight.run(motorspeedturn);//Left-turn
    motorLeft.run(motorspeedturn);
    delay(310);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);//Mbot moves forward
    motorLeft.run(-motorspeedturn);
    delay(715);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeedturn);//Left-turn again
    motorLeft.run(motorspeedturn);
    delay(295);
  }
  motorRight.stop(); //Let the Mbot stop for a small period of time after turning for more accurate turning
  motorLeft.stop();
  delay(50);
}

//HELPER FUNCTIONS
void turn_on_red(){ //Turn on Red LED
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
}

void turn_on_blue(){ //Turn on Blue LED
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
}

void turn_on_green(){ //Turn on Green LED 
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

//To find the voltage difference between ambient IR and when the IR emitter is turned on
void readValue() {
  //Turn off IR 
  digitalWrite(A,HIGH);
  digitalWrite(B, LOW);
  delay(2);
//Measure voltage of ambient IR
readVal = analogRead(readPin);
V2Ambient = (5./1023.)*readVal;
  //Turn on IR
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  delay(2);
//Measure voltage with IR emitter turned on
readVal = analogRead(readPin);
V2Emitter = (5./1023.)*readVal;
V2diff = V2Ambient-V2Emitter;
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

//This function is used to generate the music, note by note
void playmusic()
{
    for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    buzzer.tone(8, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}
