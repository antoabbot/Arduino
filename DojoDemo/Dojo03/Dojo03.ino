/***************************************************************
 * CODER DOJO OLNEY
 * Example 3: Move a servo to Follow a potentiometer
 * 
 * Author: Anthony Abbot
 * Date:   13 September 2015
 */

#include <Servo.h>

int potValue;
int servoAngle;

Servo servo;

void setup() 
{
  // Write stuff out so we can debug
  Serial.begin(9600);
  servo.attach(9);
}

void loop() 
{
  potValue = analogRead(A0);

  servoAngle = map(potValue, 0, 1024, 0, 179);
  servo.write(servoAngle);

  Serial.print("Pot=");
  Serial.print(potValue);
  Serial.println("");

  delay(15);
}
