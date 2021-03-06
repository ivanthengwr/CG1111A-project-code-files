/* ColourSensor.txt
 * Designed by Dr Henry Tan
 * For CG1111A Photoelectric Sensor Studio
 */

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 

//#define LDR 0   //LDR sensor pin at A0 original code for ldr
//#define LED 13  //Check Indicator to signal Calibration Completed

// Define colour sensor LED pins
//int ledArray[] = {2,3,4};// original code
int red_num = 0;
int green_num = 1;
int blue_num = 2;
int A = A2; //new code
int B = A1; //new code
int LDR = A3; //new code

//placeholders for colour detected
//int red = 0;
//int green = 0;
//int blue = 0;

//floats to hold colour arrays
float colourArray[] = {0,0,0};
float blackArray[] = {715,491,455};
float greyDiff[] = {251,420,432};
float whiteArray[] = {0,0,0};

char colourStr[3][5] = {"R = ", "G = ", "B = "};

void setup(){
  //setup the outputs for the colour sensor
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(LDR, INPUT); 
  //pinMode(LED,OUTPUT);   //Check Indicator -- OFF during Calibration
  

  //begin serial communication
  Serial.begin(9600);
  
  //setBalance();  //calibration
  //digitalWrite(LED, HIGH); //Check Indicator -- ON after Calibration
}

void loop(){
//turn on one colour at a time and LDR reads 5 times
  for(int c = 0;c<=2;c++){    
    Serial.print(colourStr[c]);
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
    delay(RGBWait+1000);
    Serial.println(int(colourArray[c])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
    //Serial.println(int(greyDiff[c]));
  } 
}

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
  
  

void setBalance(){
//set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           //delay for five seconds for getting sample ready
  //digitalWrite(LED,LOW); //Check Indicator OFF during Calibration
//scan the white sample.
//go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for(int i = 0;i<=2;i++){
     if(i==0){
      turn_on_red();
     }
     else if(i==1){
      turn_on_green();
     }
     else{
      turn_on_blue();
     }
     delay(RGBWait);
     whiteArray[i] = getAvgReading(5);         //scan 5 times and return the average, 
     digitalWrite(A, LOW);
     digitalWrite(B, LOW);
     delay(RGBWait);
  }
//done scanning white, time for the black sample.
//set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready 
//go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for(int i = 0;i<=2;i++){
     if(i==0){
      turn_on_red();
     }
     else if(i==1){
      turn_on_green();
     }
     else{
      turn_on_blue();
     }
     delay(RGBWait);
     blackArray[i] = getAvgReading(5);
     digitalWrite(A, LOW);
     digitalWrite(B, LOW);
     delay(RGBWait);
//the differnce between the maximum and the minimum gives the range
     greyDiff[i] = whiteArray[i] - blackArray[i]; 
  }

//delay another 5 seconds for getting ready colour objects
//  Serial.println("Colour Sensor Is Ready.");//
  delay(5000);
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
