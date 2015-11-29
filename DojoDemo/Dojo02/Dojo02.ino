/***************************************************************
 * CODER DOJO OLNEY
 * Example 2: Read some analog inputs, write them out
 * 
 * Author: Anthony Abbot
 * Date:   13 September 2015
 */

int potValue;
int light1Value;
int light2Value;

void setup() 
{
  // Write stuff out so we can debug
  Serial.begin(9600);
}

void loop() 
{
Lights  light1Value = analogRead(A1);
  light2Value = analogRead(A2);

  Serial.print("Pot=");
  Serial.print(potValue);
  Serial.print(",\tLeft=");
  Serial.print(light1Value);
  Serial.print(",\tRight=");
  Serial.print(light2Value);
  Serial.println("");
  delay(200);
}
