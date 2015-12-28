/*****************************************************************************/
// Robot remote control via Bluetooth LE
// Author: Anthony Abbot
// Date: 27 Dec 2015
// 
// This code has been developed for use by the Olney Coder Dojo club
//
// This utilizes ther following components:
//  - An Arduino Uno R3
//  - An Adafruit bluefruit LE Shield
//  - An Adafruit Motor/Stepper/Servo Shield for Arduino v2
//  - An SR04 utrasonic proximity sensor
//  - A servo motor to rotate the sensor
//  - A two motor robot platform, driven from M1 an M2 of the motor shield
//
// Pins used:
//  - D2 and D3 for the proximity sensor
//  - D5 for the servo control
//  - M1 and M2 output to drive the left and right motors
//
// Control is best acheived from the Bluefruit LE phone App, connected in
// Controller mode and then running the control pad. 
// Streaming dta can be monitored by the same app in UART mode
// Raspberry Pi Python 2.7 can act as a controller
// So can Python on a Mac, but NO windoze 10 as it has terrible BLE support
// Commands are simple text strings, as are the returned data packets
/*****************************************************************************/

// Includes required for the Bluetooth LE IO. 
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include "Bleio.h"

// Includes for the Motor controller
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Includes for the scanning ultrasound proximity sensor 
#include <NewPing.h>
#include <Servo.h>
#include "SweepSensor.h"


/*****************************************************************************/
// Global variables and defines
#define SERVO_PIN          5
#define SENSOR_TX           2
#define SENSOR_RX           3
#define SENSOR_MAXDIST      100

// State variables used in the loop
int speed;        // Motor speed
int lDir, rDir;   // direction of left and right motors


Bleio ble;            // BlueFruit LE class to manage IO

// Create an instance of the motor shield and two motors
Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *lMotor = AFMS1.getMotor(1);
Adafruit_DCMotor *rMotor = AFMS1.getMotor(2);

// Create a sweep sensor instance
SweepSensor sensor(SERVO_PIN, SENSOR_TX, SENSOR_RX, SENSOR_MAXDIST);



/*****************************************************************************/
// Setup function - runs once at startup
void setup() 
{
  Serial.begin(115200);

  sensor.centre();      // centre the sensor
  ble.initBle();        // Initialize BLE. Will block until user connects
  AFMS1.begin();        // Start the motor control
}

bool isconnected = true;
int sendcount = 0;

/*****************************************************************************/
// Main loop - runs repeatedly
void loop() 
{
  delay(100);       // Don't spin too fast!
  sensor.measure();

  // Check if we are connected. if Not, don't bother doing any comms but stop the motors
  if (!ble.isConnected())
  {
    if (isconnected)
    {
      isconnected = false;
      Serial.println("BLE disconnected!");
      lMotor->run(RELEASE);
      rMotor->run(RELEASE);
    }
    
    delay(2000);  // Sleep for 2 seconds
    return;   
  }
  else
  {
    if(!isconnected)
    {
      isconnected = true;
      Serial.println("BLE reconnected!");
    }
  }

  ++sendcount;
  if (sendcount > 4)
  {
    sendcount = 0;
    // Read the sensor distance and angle, and broadcast back to the client
    uint16_t dist = sensor.get_distance();
    uint8_t angle = sensor.get_angle();
  
    ble.sendDistData(dist, angle);
  }

  //
  // Read which of the 8 buttons on the Bluefruit LE app is pressed
  volatile bool isMotor = false;   // flag up motor control commands
  bool pressed;           // Pressed or released
  int buttnum;        // The button number

  // Check if there is data. If false, then no command waiting for us
  if (!ble.getButton(buttnum, pressed))
    return;

  Serial.print(buttnum);
  switch(buttnum)
  {
    case 7:
      Serial.println(":Forward");
      speed = 150;
      isMotor = true;
      lDir = (pressed) ? FORWARD : RELEASE;
      rDir = (pressed) ? FORWARD : RELEASE;
      break;
    case 8:
      Serial.println(":Backward");
      speed = 150;
      isMotor = true;
      lDir = (pressed) ? BACKWARD : RELEASE;
      rDir = (pressed) ? BACKWARD : RELEASE;        
      break;
    case 6:
      Serial.println(":Left");
      speed = 100;
      isMotor = true;
      lDir = (pressed) ? BACKWARD : RELEASE;
      rDir = (pressed) ? FORWARD : RELEASE;
      break;
    case 5:
      Serial.println(":Right");
      speed = 100;
      isMotor = true;
      lDir = (pressed) ? FORWARD : RELEASE;
      rDir = (pressed) ? BACKWARD : RELEASE;
      break;
    case 1:
      Serial.println(":LookLeft");
      sensor.setangle(10);
//      lDir = RELEASE;
//      rDir = RELEASE;
      break;
    case 2:
      Serial.println(":LookAhead");
      sensor.centre();
//      lDir = RELEASE;
//      rDir = RELEASE;
      break;
    case 3:
      Serial.println(":LookRight");
      sensor.setangle(170);
//      lDir = RELEASE;
//      rDir = RELEASE;
      break;
    default:
      Serial.println(":Other");
//      lDir = RELEASE;
//      rDir = RELEASE;
      break;
  }

  if (isMotor)
  {
    lMotor->setSpeed(speed);
    lMotor->run(lDir);
    rMotor->setSpeed(speed);
    rMotor->run(rDir);
  }
}









