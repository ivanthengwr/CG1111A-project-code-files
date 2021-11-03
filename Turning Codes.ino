#include <MeMCore.h>
//ultrasonic
MeUltrasonicSensor ultraSensor(PORT_1);
MeLineFollower lineFinder(PORT_2);
MeDCMotor motorRight(M1);
MeDCMotor motorLeft(M2);

uint8_t motorspeed1 = 492;
float colourArray[] = {165,140,150};

void setup(){}

void loop()
{
  if(colourArray[0] > 200)
  {
   if(colourArray[2] > colourArray[1])//Red left turn
   {
     motorRight.run(motorspeed1);
     motorLeft.run(motorspeed1);
     delay(260);
   }
   if(colourArray[1] > colourArray[2]) //Orange Uturn
   {
    motorRight.run(motorspeed1);
    motorLeft.run(motorspeed1);
    delay(520);
   }
  }else if(colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) //Green right turn
  {
    motorRight.run(-motorspeed1);
    motorLeft.run(-motorspeed1);
    delay(260);
  }else if(colourArray[2] > colourArray[1] && colourArray[2] > colourArray[0])//Blue 2right turn
  {
    motorRight.run(-motorspeed1);
    motorLeft.run(-motorspeed1);
    delay(260);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeed1);
    motorLeft.run(-motorspeed1);
    delay(550);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(-motorspeed1);
    motorLeft.run(-motorspeed1);
    delay(260);
  }else{ //Purple 2left turn
    motorRight.run(motorspeed1);
    motorLeft.run(motorspeed1);
    delay(280);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeed1);
    motorLeft.run(-motorspeed1);
    delay(550);
    motorRight.stop();
    motorLeft.stop();
    delay(700);
    motorRight.run(motorspeed1);
    motorLeft.run(motorspeed1);
    delay(280);
  }
  motorRight.stop();
  motorLeft.stop();
  delay(5000);
}
