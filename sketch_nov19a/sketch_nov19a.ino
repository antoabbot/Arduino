#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMSbot(0x60);
Adafruit_MotorShield AFMStop(0x61);

Adafruit_DCMotor *myMotor = AFMStop.getMotor(4);
 

void setup()
{
  AFMSbot.begin();
  AFMStop.begin();
  myMotor->setSpeed(100);
  myMotor->run(FORWARD);
}

void loop() 
{
  delay(2000);
  myMotor->setSpeed(0);
  myMotor->run(FORWARD);

}
