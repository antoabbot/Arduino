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
    uint8_t packetbuffer[BLEIO_BUFSIZE+1];
    Adafruit_BluefruitLE_SPI ble;
  
  public:
    Bleio();
    void initBle();
    bool isConnected();
    uint8_t readPacket(); 
    void sendCommand(const char * cmd);
    bool getButton(uint8_t& buttnum, bool& pressed);
  
};




#endif // __Bleio_h__
