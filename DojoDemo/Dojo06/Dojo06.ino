/***************************************************************
 * CODER DOJO OLNEY
 * Example 6: Lights with pot
 * 
 * Author: Anthony Abbot
 * Date:   13 September 2015
 * 
 */


// Constants to make the code easier to read
const int redLed = 4;
const int yellowLed = 5;
const int greenLed = 6;
const int blueLed = 7;
int potValue;
int outValue;

//
// Setup function that runs once on initialisation
void setup() 
{
  Serial.begin(9600);
    
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
}

// Loop function is called repeatedly forever
void loop() 
{
  potValue = analogRead(A0);
  outValue = map(potValue, 0, 1024, 0, 5);

  Serial.print("Pot=");
  Serial.print(potValue);
  Serial.print(",\tOut=");
  Serial.print(outValue);
  Serial.println("");


  switch(outValue)
  {
  case 0:
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(blueLed, LOW);
    break;
  case 1:  
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(blueLed, HIGH);
    break;
  case 2:
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(blueLed, HIGH);
    break;
  case 3:  
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(blueLed, HIGH);
    break;
  case 4:  
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(blueLed, HIGH);
    break;
  } 
}
