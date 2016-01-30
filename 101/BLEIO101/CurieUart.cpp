#include "CurieUart.h"
CurieUart::CurieUart(const char* name)
  : ble101(),
    ble101Service("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"),
    ble101tx ("6E400003-B5A3-F393-E0A9-E50E24DCCA9E",  BLENotify, 21),
    ble101rx  ("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse, 21)
{
  ble101.setLocalName(name);
  ble101.setDeviceName(name);
  ble101.setAdvertisedServiceUuid(ble101Service.uuid());
  ble101.addAttribute(ble101Service);
  ble101.addAttribute(ble101rx);
  ble101.addAttribute(ble101tx);
}

void CurieUart::begin()
{
  ble101.begin(); 
}


BLECentral CurieUart::central()
{
  return ble101.central();
}

void CurieUart::sendString(const String& val)
{
  unsigned short len = val.length();  // Get the string length
  len = min(20, len);                 // truncate at 20
  char buff[21];                      // declare a buffer
  memset(buff, 0, 21);                // initialise all elements to 0
  strncpy(buff, val.c_str(), 20);     // copy string into the buffer
  
  ble101tx.setValue((unsigned char*)buff, len);  // Send the data
}

bool CurieUart::hasString()
{
  return ble101rx.written();
}

String CurieUart::getString()
{
    char buff[21];
    memset(buff, 0, 21);
    unsigned short len = ble101rx.valueLength();
    len = min(20, len);

    String strIn;
    for (unsigned short i = 0; i< len; ++i)
    {
      buff[i]= ble101rx[i];
    }

    return String(buff);
}

