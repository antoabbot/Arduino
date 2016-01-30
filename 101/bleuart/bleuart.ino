#include <CurieBle.h>

BLEPeripheral ble101;
BLEService ble101Service("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
BLECharacteristic ble101tx ("6E400003-B5A3-F393-E0A9-E50E24DCCA9E",  BLENotify, 20);
BLECharacteristic ble101rx  ("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse, 20);

long previousMillis = 0;
int heartRate = 0;

void setup() {
  while(!Serial);
  Serial.begin(9600);  
  ble101.setLocalName("dojorobo");
  ble101.setDeviceName("dojorobo");
  ble101.setAdvertisedServiceUuid(ble101Service.uuid());
  ble101.addAttribute(ble101Service);
  ble101.addAttribute(ble101rx);
  ble101.addAttribute(ble101tx);
  //ble101tx.setValue(0);
  //ble101rx.setValue(0);
  ble101.begin();
  Serial.println("Completed setup");
}

void loop() 
{  
  BLECentral central = ble101.central();

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) 
    {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= 2000) 
      {
        previousMillis = currentMillis;
        heartRate++;
        if (heartRate >= 100) heartRate = 0;

        char buff[10];
        String txStr = "HR=";
        dtostrf(heartRate, 4, 2, buff);
        txStr += buff;
        char b[20];
        txStr.toCharArray(b, 20);
        //char fu = heartRate;
        //Serial.println(b);

        ble101tx.setValue((unsigned char*)b, 20);  
      }

      if (ble101rx.written())
      {
        char buff[21];
        memset(buff, 0, 21);
        unsigned short len = ble101rx.valueLength();
        String strIn;
        for (unsigned short i = 0; i< len; ++i)
        {
          buff[i]= ble101rx[i];
        }
        buff[20] = '\0';
        
        Serial.println(buff);
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
