/***************************************************************
 * CODER DOJO OLNEY
 * Example 4: Drive a motor
 * 
 * Author: Anthony Abbot
 * Date:   13 September 2015
 */

int potValue;
int power;
int absPwr;
bool isStopped;
bool isForward;

const int ctlPin1 = 2;
const int ctlPin2 = 3;
const int enablePin = 10;

void setup() 
{  
  pinMode(ctlPin1, OUTPUT);
  pinMode(ctlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);  

  digitalWrite(enablePin, LOW);
  Serial.begin(9600);

}

void loop() 
{
  potValue = analogRead(A0);

  // Map to the range -100, 100
  power = map(potValue, 0, 1024, -100, 100);
  absPwr = abs(power);
  isStopped = (absPwr < 30);
  isForward = (power > 0);
  
  Serial.print("[off,forward,power]=[");
  Serial.print(isStopped);
  Serial.print(",");
  Serial.print(isForward);
  Serial.print(",");
  Serial.print(absPwr);
  Serial.println("]");

  // If the power is low, stop the motor
  if (isStopped)
  {
    analogWrite(enablePin, 0);
  }
  else
  {
    analogWrite(enablePin, absPwr);
  }

  if (isForward)
  {
    digitalWrite(ctlPin1, HIGH);
    digitalWrite(ctlPin2, LOW);
  }
  else
  {
    digitalWrite(ctlPin1, LOW);
    digitalWrite(ctlPin2, HIGH);
  }
  
  delay(1);

}
