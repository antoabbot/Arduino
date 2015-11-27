
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

Adafruit_DCMotor *motor[5];
Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x60); 
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61); 



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
  motor[0] = AFMS1.getMotor(1);
  motor[1] = AFMS1.getMotor(2);
  motor[2] = AFMS1.getMotor(3);
  motor[3] = AFMS1.getMotor(4);
  motor[4] = AFMS1.getMotor(5);
  
  AFMS1.begin();  
  AFMS2.begin(); 


  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() )
    {
      error(F("Couldn't factory reset"));
    }
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  ble.verbose(false);  // debug info is a little annoying after this point!

  // Wait for connection 
  while (! ble.isConnected()) 
  {
      delay(500);
  }

  ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  ble.setMode(BLUEFRUIT_MODE_DATA);
}


void loop(void)
{
  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;


  // Buttons
  if (packetbuffer[1] == 'B') 
  {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';

    if (pressed) 
    {
      switch(buttnum)
      {
        case 1:
          Serial.println("Motor 1 forward");
          motor[0]->setSpeed(150);
          motor[0]->run(FORWARD);
          break;
        case 2:
          Serial.println("Motor 1 reverse");
          motor[0]->setSpeed(150);
          motor[0]->run(BACKWARD);
          break;
        break;
        case 3:
          Serial.println("Motor 2 forward");
          motor[1]->setSpeed(150);
          motor[1]->run(FORWARD);
          break;
        case 4:
          Serial.println("Motor 2 reverse");
          motor[1]->setSpeed(150);
          motor[1]->run(BACKWARD);
          break;
        case 5:
          Serial.println("Motor 3 forward");
          motor[2]->setSpeed(150);
          motor[2]->run(FORWARD);
          break;
        case 6:
          Serial.println("Motor 3 reverse");
          motor[2]->setSpeed(150);
          motor[2]->run(BACKWARD);
          break;
        case 7:
          Serial.println("Motor 4 forward");
          motor[3]->setSpeed(150);
          motor[3]->run(FORWARD);
          break;
        case 8:
          Serial.println("Motor 4 reverse");
          motor[3]->setSpeed(150);
          motor[3]->run(BACKWARD);
          break;
      }
    }
    else 
    {
      Serial.println("Buttons released, stop");
      
      motor[0]->run(RELEASE);
      motor[1]->run(RELEASE);
      motor[2]->run(RELEASE);
      motor[3]->run(RELEASE);
      motor[4]->run(RELEASE);
    }
  }


}
