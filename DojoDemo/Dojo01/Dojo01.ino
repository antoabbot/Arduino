/***************************************************************
 * CODER DOJO OLNEY
 * Example 1: Tilt alarm
 * 
 * Author: Anthony Abbot
 * Date:   13 September 2015
 * 
 * This simple sketch warns the user if a tilt switch is triggered 
 * by flashing lights
 */


// Constants to make the code easier to read
const int redLed = 4;
const int yellowLed = 5;
const int greenLed = 6;
const int blueLed = 7;
const int tiltSwitch = 8;



// A variable to hold the state of the switch
int switchState = 0;

//
// Setup function that runs once on initialisation
void setup() 
{
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(tiltSwitch, INPUT);
}

// Loop function is called repeatedly forever
void loop() 
{

  // Read the level switch
  switchState = digitalRead(tiltSwitch);

  //
  if (switchState == LOW)
  {
    // Its not set. Activate the green LED
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(blueLed, LOW);
  }
  else
  {
    // Alarm triggered, off with green and on with Red
    digitalWrite(redLed, HIGH); 
    digitalWrite(greenLed, LOW);
    
    delay(250);
    digitalWrite(yellowLed, LOW);
    digitalWrite(blueLed, HIGH);
    
    delay(250);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(blueLed, LOW);
  }
}
