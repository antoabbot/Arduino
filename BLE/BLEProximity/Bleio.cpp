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

void Bleio::sendCommand(const char * cmd)
{
  ble.println(cmd);
}

bool Bleio::getButton(uint8_t& buttnum, bool& pressed)
{
  uint8_t len = readPacket();
  if (len == 0) return false;
  if (packetbuffer[1] != 'B') return false;
  buttnum = packetbuffer[2] - '0';
  pressed = packetbuffer[3] - '0';
  return true;
}


//
// Read a data packet starting with !. places data in the buffer and returns length read.
uint8_t Bleio::readPacket() 
{
  uint16_t replyidx = 0;

  // copy zeros into the buffer
  memset(packetbuffer, 0, BLEIO_BUFSIZE);

  // read data from the buffer while it is available
  while (ble.available()) 
  {
    char c =  ble.read();
    if (c == '!') replyidx = 0;
    if (replyidx >= 20) continue;
    packetbuffer[replyidx] = c;
    replyidx++;
  }
    
  packetbuffer[replyidx] = 0;  // null terminate the buffer

  if (!replyidx) return 0;
  if (packetbuffer[0] != '!') return 0;
    
  return replyidx;
}

