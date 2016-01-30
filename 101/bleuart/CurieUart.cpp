#include "CurieUart.h"
CurieUart::CurieUart(const char* name)
  : ble101(),
    ble101Service("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"),
    ble101tx ("6E400003-B5A3-F393-E0A9-E50E24DCCA9E",  BLENotify, 20),
    ble101rx  ("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse, 20)
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
  unsigned short len = val.length();
  len = min(len, 20);
  char b[20];
  val.toCharArray(b, len);
  ble101tx.setValue((unsigned char*)b, len);  
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
    String strIn;
    for (unsigned short i = 0; i< len; ++i)
    {
      buff[i]= ble101rx[i];
    }

    for(unsigned short i=len; i<20; ++i)
    {
      buff[i] = '\0';
    }

    buff[20] = '\0';

    return String(buff);
}

