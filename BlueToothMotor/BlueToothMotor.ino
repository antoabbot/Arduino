
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x60); 
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61); 
Adafruit_DCMotor *motor1 = AFMS1.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS1.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS1.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS1.getMotor(4);
Adafruit_DCMotor *motor5 = AFMS2.getMotor(1);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];


/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
//  while (!Serial);  // required for Flora & Micro
//  delay(500);

  AFMS1.begin();  // create with the default frequency 1.6KHz
  AFMS2.begin();  // create with the default frequency 1.6KHz


  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));

}


void loop(void)
{
  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;

  /* Got a packet! */
  // printHex(packetbuffer, len);


  // Buttons
  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';

    if (pressed) 
    {
      switch(buttnum)
      {
        case 1:
          Serial.println("Motor 1 forward");
          motor1->setSpeed(150);
          motor1->run(FORWARD);
          break;
        case 2:
          Serial.println("Motor 1 reverse");
          motor1->setSpeed(150);
          motor1->run(BACKWARD);
          break;
        break;
        case 3:
          Serial.println("Motor 2 forward");
          motor2->setSpeed(150);
          motor2->run(FORWARD);
          break;
        case 4:
          Serial.println("Motor 2 reverse");
          motor2->setSpeed(150);
          motor2->run(BACKWARD);
          break;
        case 5:
          Serial.println("Motor 3 forward");
          motor3->setSpeed(150);
          motor3->run(FORWARD);
          break;
        case 6:
          Serial.println("Motor 3 reverse");
          motor3->setSpeed(150);
          motor3->run(BACKWARD);
          break;
        case 7:
          Serial.println("Motor 4 forward");
          motor5->setSpeed(150);
          motor5->run(FORWARD);
          break;
        case 8:
          Serial.println("Motor 4 reverse");
          motor5->setSpeed(150);
          motor5->run(BACKWARD);
          break;
      }

      
    }
    else 
    {
      Serial.println("Buttons released, stop");
      
      motor1->setSpeed(0);
      motor2->setSpeed(0);
      motor3->setSpeed(0);
      motor4->setSpeed(0);
      motor5->setSpeed(0);
      motor1->run(RELEASE);
      motor2->run(RELEASE);
      motor3->run(RELEASE);
      motor4->run(RELEASE);
      motor5->run(RELEASE);
    }
  }


}
