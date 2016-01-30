#include <CurieBle.h>
#include "CurieUart.h"

CurieUart uart("dojoRobo");

long previousMillis = 0;
int heartRate = 0;

void setup() 
{
  while(!Serial);
  Serial.begin(9600);  
  Serial.print("Initialising UART...");
  delay(1000);
  uart.begin();
  Serial.println("Done");
}

void loop() 
{  

  BLECentral central = uart.central();

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) 
    {
      if(Serial.available() > 0)
      {
          String str = Serial.readStringUntil('\n');
          uart.sendString(str);
          Serial.print("-> ");
          Serial.println(str);
      }
      
      delay(10);
      if (uart.hasString())
      {
        String strIn = uart.getString();
        Serial.println(strIn);
      } 
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
  
}
