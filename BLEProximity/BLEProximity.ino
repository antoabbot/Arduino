
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>

#define BLUEFRUIT_SPI_CS              8
#define BLUEFRUIT_SPI_IRQ             7
#define BLUEFRUIT_SPI_RST             6    // Optional but recommended, set to -1 if unused

#define READ_BUFSIZE                    (20)
uint8_t packetbuffer[READ_BUFSIZE+1];


Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) 
{
  Serial.println(err);
  while (1);
}



void setup() 
{
  Serial.begin(115200);
  initBle();
  

}

void loop() 
{
  delay(100);
  int dist = analogRead(A1);

  // Write the sensor value out to the listener
  ble.print("!S"); ble.print(dist); ble.println(";");

  // read any incoming command packet
  uint8_t len = readPacket();
  if (len != 0) 
  {
    String s = "Got buffer, len=" + String(len) + ", val=" + String((char*)packetbuffer);
    Serial.println(s.c_str());
  }
}


bool isConnected()
{
  ble.print("+++\n");
  bool isConn = ble.isConnected();
  ble.print("+++\n");
  return isConn;
}

void initBle()
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

uint8_t readPacket() 
{
  uint16_t replyidx = 0;

  // copy zeros into the buffer
  memset(packetbuffer, 0, READ_BUFSIZE);

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


