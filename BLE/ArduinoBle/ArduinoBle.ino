/*****************************************************************************/
// IO via Bluetooth LE
// Author: Anthony Abbot
// Date: 3 Jan 2016
// 
// This code has been developed for use by the Olney Coder Dojo club
//
// This requires the following components:
//  - An Arduino Uno R3
//  - An Adafruit bluefruit LE Shield
//
//
/*****************************************************************************/

// Includes required for the Bluetooth LE IO. 
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include "Bleio.h"

Bleio ble;            // BlueFruit LE class to manage IO
String str;           // Data read from the local serial input
String data;          // Data received from the LE device

/*****************************************************************************/
// Setup function - runs once at startup
void setup() 
{
  Serial.begin(9600);
   ble.initBle("Dojo01");      // Initialize BLE. Will block until user connects
}
 



/*****************************************************************************/
// Main loop - runs repeatedly
void loop() 
{
  delay(100);       // Don't spin too fast!

    if(Serial.available() > 0)
    {
        str = Serial.readStringUntil('\n');
        ble.send(str.c_str());
        Serial.print("-> ");
        Serial.println(str);

    }
  // Check if there is data. If false, then no command waiting for us
  if (ble.available())
    {
      data = ble.readCommand('!');
      Serial.print("<- ");
      Serial.println(data);
    }
}









