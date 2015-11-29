/***************************************************************
 * CODER DOJO OLNEY
 * Example 3: turn to follow a light
 * 
 * Author: Anthony Abbot
 * Date:   13 September 2015
 */

#include <Servo.h>

int potValue;
int servoAngle;
int light1Value;
int light2Value;
int diff;

Servo servo;

void setup() 
{
  // Write stuff out so we can debug
  Serial.begin(9600);
  servo.attach(9);
  servoAngle = 90;
  servo.write(servoAngle);
}

void loop() 
{
  potValue = analogRead(A0);
  light1Value = analogRead(A1);
  light2Value = analogRead(A2);
  diff = light1Value - light2Value;

  servoAngle += diff / 30;
  servoAngle = map(servoAngle, 0, 179, 0, 179);
  servo.write(servoAngle);

  Serial.print("diff=");
  Serial.print(diff);
  Serial.println("");

  delay(50);
}
