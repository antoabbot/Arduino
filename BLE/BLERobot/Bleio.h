/*****************************************************************************/
// Robot remote control via Bluetooth LE
// Author: Anthony Abbot
// Date: 27 Dec 2015
// 
// This code has been developed for use by the Olney Coder Dojo club
//
// Class to better encapsulate the BLE UART protocol for receiving commands
// and sending back data
//
/*****************************************************************************/

#ifndef __Bleio_h__
#define __Bleio_h__

#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>

// text buffer for reading commands
#define BLEIO_BUFSIZE                    (20)

class Bleio
{
  private:
    uint8_t recvbuffer[BLEIO_BUFSIZE + 1];
    char    sendbuffer[BLEIO_BUFSIZE + 1];
    Adafruit_BluefruitLE_SPI ble;
  
  public:
    Bleio();
    void initBle();
    bool isConnected();
    uint8_t readPacket(); 
    void sendData(const char prefix, int value);
    void sendDistData(uint16_t dist, uint8_t angle);
    bool getButton(int& buttnum, bool& pressed);
  
};




#endif // __Bleio_h__
