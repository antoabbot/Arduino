#include <Servo.h>

Servo myServo;
int const potPin = A0;
int const leftEye = A1;
int const rightEye = A2;

void setup()
{
  myServo.attach(9);
  Serial.begin(9600);
}

void loop()
{
  int potVal = analogRead(potPin);
  int sensitivity = map(potVal, 0, 1023, 0, 100);
  int leftVal = analogRead(leftEye);
  int rightVal = analogRead(rightEye);
  
  int diff = (leftVal - rightVal);
  // diff = diff * (sensitivity / 100);
  int effect = map(diff, -800, 800, 0, 179);
  effect = constrain(effect, 0, 179);

  Serial.print("sens=");
  Serial.print(sensitivity);

  Serial.print(", l=");
  Serial.print(leftVal);
  Serial.print(", r=");
  Serial.print(rightVal);
  Serial.print(", diff=");
  Serial.print(diff);
  Serial.print(", eff=");
  Serial.print(effect);
  Serial.println("");
  
  myServo.write(effect);
  delay(50);
}
