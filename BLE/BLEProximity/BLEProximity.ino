#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// text buffer for reading commands
#define READ_BUFSIZE                    (20)
uint8_t packetbuffer[READ_BUFSIZE+1];

// Create an instance of the BlueFruit LE class to manage IO
Adafruit_BluefruitLE_SPI ble(8, 7, 6);

Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *lMotor;
Adafruit_DCMotor *rMotor;

//
// Setup function - runs once at startup
void setup() 
{
  Serial.begin(115200);
  initBle();
  
  lMotor = AFMS1.getMotor(1);
  rMotor = AFMS1.getMotor(2);
  AFMS1.begin();
}

//
// Main loop - runs repeatedly
void loop() 
{
  delay(100);
  int dist = analogRead(A1);

  // Write the sensor value out to the listener
  ble.print("!S"); ble.print(dist); ble.println(";");

  // read any incoming command packet
  uint8_t len = readPacket();
  if (len == 0) return;
   
  if (packetbuffer[1] == 'B') 
  {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';

    if (!pressed)
    {
      Serial.println("Released");
      lMotor->run(RELEASE);
      rMotor->run(RELEASE);
    }
    else
    {
      switch(buttnum)
      {
        case 8:
          Serial.println("Forward");
          lMotor->setSpeed(150);
          lMotor->run(FORWARD);
          rMotor->setSpeed(150);
          rMotor->run(FORWARD);
          break;
        case 7:
          Serial.println("Backward");
          lMotor->setSpeed(150);
          lMotor->run(BACKWARD);
          rMotor->setSpeed(150);
          rMotor->run(BACKWARD);
          break;
        case 6:
          Serial.println("Left");
          lMotor->setSpeed(120);
          lMotor->run(BACKWARD);
          rMotor->setSpeed(120);
          rMotor->run(FORWARD);
          break;
        case 5:
          Serial.println("Right");
          lMotor->setSpeed(120);
          lMotor->run(FORWARD);
          rMotor->setSpeed(120);
          rMotor->run(BACKWARD);          
          break;
      }
    }
  }
}



//********************************************************************************************

//
// An error helper. Prints and terminates
void error(const __FlashStringHelper*err) 
{
  Serial.println(err);
  while (1);
}

//
// Check if the client is connected
bool isConnected()
{
  ble.print("+++\n");
  bool isConn = ble.isConnected();
  ble.print("+++\n");
  return isConn;
}

//
// Initialise the BLE connection in Data mode
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

//
// Read a data packet starting with !. places data in the buffer and returns length read.
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


