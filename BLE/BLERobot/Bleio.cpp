#include "Bleio.h"

//
// An error helper. Prints and terminates
void error(const __FlashStringHelper*err) 
{
  Serial.println(err);
  while (1);
}

Bleio::Bleio()
  : ble(8, 7, 6)
{
  
}


//
// Initialise the BLE connection in Data mode
void Bleio::initBle()
{
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(true) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );
  Serial.println(F("Performing a factory reset: "));
  if ( ! ble.factoryReset() )
    error(F("Couldn't factory reset"));

  ble.echo(false);
  Serial.println("Requesting Bluefruit info:");
  ble.info();
  ble.verbose(false); 

  Serial.println(F("Waiting for connection to continue..."));
  while (! ble.isConnected()) 
  {
      delay(500);
  }

  ble.sendCommandCheckOK("AT+HWModeLED=MODE");
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
}


//
// Check if the client is connected
bool Bleio::isConnected()
{
  ble.print("+++\n");
  bool isConn = ble.isConnected();
  ble.print("+++\n");
  return isConn;
}

bool Bleio::getButton(int& buttnum, bool& pressed)
{
  uint8_t len = readPacket();
  if (len == 0) return false;
  if (recvbuffer[1] != 'B') return false;
  buttnum = recvbuffer[2] - '0';
  pressed = recvbuffer[3] - '0';

  Serial.println(buttnum);
  return true;
}

void Bleio::sendData(const char prefix, int value)
{
  memset(sendbuffer, 0, BLEIO_BUFSIZE);
  sprintf(sendbuffer, "!%c%03d;", prefix, value);
  ble.println(sendbuffer);  
}

void Bleio::sendDistData(uint16_t dist, uint8_t angle)
{
  memset(sendbuffer, 0, BLEIO_BUFSIZE);
  sprintf(sendbuffer, "!S%03d%03d;", dist, angle);
  ble.println(sendbuffer);  
  Serial.println(sendbuffer);
}


//
// Read a data packet starting with !. places data in the buffer and returns length read.
uint8_t Bleio::readPacket() 
{
  uint16_t replyidx = 0;

  // copy zeros into the buffer
  memset(recvbuffer, 0, BLEIO_BUFSIZE);

  // read data from the buffer while it is available
  while (ble.available()) 
  {
    char c =  ble.read();
    if (c == '!') replyidx = 0;
    if (replyidx >= 20) continue;
    recvbuffer[replyidx] = c;
    replyidx++;
  }
    
  recvbuffer[replyidx] = 0;  // null terminate the buffer

  if (!replyidx) return 0;
  if (recvbuffer[0] != '!') return 0;
    
  return replyidx;
}

