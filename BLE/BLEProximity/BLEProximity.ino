#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include "Bleio.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include <NewPing.h>
#include <Servo.h>
#include "SweepSensor.h"




// Create an instance of the BlueFruit LE class to manage IO
Bleio ble;

// Create an instance of the motor shield and two motors
Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *lMotor;
Adafruit_DCMotor *rMotor;
SweepSensor sensor(5,2,3,100);

//
// Setup function - runs once at startup
void setup() 
{
  Serial.begin(115200);

  // centre the sensor
  sensor.centre();
  
  ble.initBle();
  
  lMotor = AFMS1.getMotor(1);
  rMotor = AFMS1.getMotor(2);
  AFMS1.begin();

}

int speed;
int lDir, rDir;

//
// Main loop - runs repeatedly
void loop() 
{
  delay(100);
  
  uint16_t dist = sensor.get_distance();
  String s = "!S" + String(dist) + ";";
  ble.sendCommand(s.c_str());

  uint8_t buttnum;
  bool pressed;

  if (!ble.getButton(buttnum, pressed))
    return;

  if (!pressed)
  {
    Serial.println("Released");
    lMotor->run(RELEASE);
    rMotor->run(RELEASE);
  }
  else
  {
    Serial.print(buttnum);
    switch(buttnum)
    {
      case 7:
        Serial.println(":Forward");
        speed = 150;
        lDir = FORWARD;
        rDir = FORWARD;
        break;
      case 8:
        Serial.println(":Backward");
        speed = 150;
        lDir = BACKWARD;
        rDir = BACKWARD;        
        break;
      case 6:
        Serial.println(":Left");
        speed = 100;
        lDir = BACKWARD;
        rDir = FORWARD;
        break;
      case 5:
        Serial.println(":Right");
        speed = 100;
        lDir = FORWARD;
        rDir = BACKWARD;
        break;
      case 1:
        Serial.println(":LookLeft");
        sensor.left();
        lDir = RELEASE;
        rDir = RELEASE;
        break;
      case 2:
        Serial.println(":LookAhead");
        sensor.centre();
        lDir = RELEASE;
        rDir = RELEASE;
        break;
      case 3:
        Serial.println(":LookRight");
        sensor.right();
        lDir = RELEASE;
        rDir = RELEASE;
        break;
      default:
        Serial.println(":Other");
        lDir = RELEASE;
        rDir = RELEASE;
        break;
    }

    lMotor->setSpeed(speed);
    lMotor->run(lDir);
    rMotor->setSpeed(speed);
    rMotor->run(rDir);

    
  }
}









