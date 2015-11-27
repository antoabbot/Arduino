// **********************************************************************************
// Wall following algorithm
//
//
// **********************************************************************************

#include <NewPing.h>

#define MAX_DISTANCE 200
#define LOOP_WAIT 100

#define LEFT_SPD      10
#define LEFT_DIR      8
#define RIGHT_SPD      9
#define RIGHT_DIR      7


unsigned long currTime;
unsigned int distFront;
unsigned int distLeft;
unsigned int distRight;

unsigned int leftSpd;
unsigned int rightSpd;
bool leftDir;
bool rightDir;

unsigned long delta;
unsigned long remainder;

// Initialise the 3 sensors
NewPing sonarFront(2, 3, MAX_DISTANCE);
NewPing sonarLeft(4, 5, MAX_DISTANCE); 
NewPing sonarRight(11, 12, MAX_DISTANCE); 

void setup() 
{
  pinMode(LEFT_SPD, OUTPUT);
  pinMode(LEFT_DIR, OUTPUT);
  pinMode(RIGHT_SPD, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);
  
  Serial.begin (57600);
  currTime = millis();
}

void loop() 
{
  // Measure the 3 sensors
  distFront = sonarFront.ping_cm(); 
  if (distFront == 0) distFront = MAX_DISTANCE;
  distLeft = sonarLeft.ping_cm(); 
  if (distLeft == 0) distLeft = MAX_DISTANCE;
  distRight = sonarRight.ping_cm(); 
  if (distRight == 0) distRight = MAX_DISTANCE;

  if (distFront > 20)
  {
    rightSpd = 150;
    leftSpd = 150;
    leftDir = HIGH;
    rightDir = HIGH;
  }
  else
  {
    leftSpd = 100;
    rightSpd = 100;
    leftDir = HIGH;
    rightDir = LOW;
  }

  delay (50);
  
  analogWrite(LEFT_SPD, leftSpd);
  analogWrite(RIGHT_SPD, rightSpd);
  digitalWrite(LEFT_DIR, leftDir);
  digitalWrite(RIGHT_DIR, rightDir);


  

  diagnosticPrint();

  // Ensure we loop at a fixed frequency
  waitRemainder(LOOP_WAIT);
}

void waitRemainder(unsigned long wait)
{
  // Time the loop
  delta = millis() - currTime;
  // Delay to run at 10Hz
  remainder = constrain(wait - delta, 0, wait);
  delay(remainder);
  currTime = millis(); 
}

void diagnosticPrint()
{
  Serial.print("Ping: [F,L,R]=\t");
  Serial.print(leftSpd); 
  Serial.print(",\t");
  Serial.print(rightSpd); 
  Serial.print(",\t");
  Serial.print(distFront); 
  Serial.print(",\t");
  Serial.print(distLeft); 
  Serial.print(",\t ");
  Serial.print(distRight); 
  Serial.println("cm");
}

