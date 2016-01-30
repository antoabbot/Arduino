/*****************************************************************************/
// Simple UART driver for the Curie BLE on the Arduino 101
// Author: Anthony Abbot
// Date: 30 Jan 2016
// 
// This code has been developed for use by the Olney Coder Dojo club
//
// Class to better encapsulate the BLE UART protocol for receiving commands
// and sending back data
//
/*****************************************************************************/

#ifndef __CurieUart_h__
#define __CurieUart_h__

#include <CurieBle.h>

class CurieUart
{
  private:
    BLEPeripheral ble101;
    BLEService ble101Service;
    BLECharacteristic ble101tx;
    BLECharacteristic ble101rx;

  public:
    CurieUart(const char* name);
    void begin();
    BLECentral central();

    void sendString(const String& val);
    bool hasString();
    String getString();

};



#endif // __CurieUart_h__
