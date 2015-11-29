
#include "MotorDrive.h"
#include "JoyStick.h"

int xVal;
int yVal;

// Initialize the two motor controllers
MotorDrive motor1(6,9);
MotorDrive motor2(10,11);

JoyStick joystick(A0, A1, 10);

void setup() 
{
  Serial.begin(9600);
  joystick.calibrate();
}

void loop() 
{
  int motX = joystick.getX();
  int motY = -joystick.getY();

  int ll = constrain(motX + motY, -1024, 1024);
  int rr = constrain(motX - motY, -1024, 1024);

  motor1.run(ll);
  motor2.run(rr);

  

  Serial.print("[x,y]=[\t");
  Serial.print(ll);
  Serial.print(",\t");
  Serial.print(rr);
  Serial.println("]");

  delay(10);
}



