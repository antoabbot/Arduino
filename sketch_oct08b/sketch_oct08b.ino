const int sensorPin = A0;
const float baselineTemp = 20.0;
const int t1 = 9;
const int t2 = 10;
const int t3 = 11;
const int loopCount = 500;
const float subRange = 2.2;

void setup()
{
  Serial.begin(9600);
  pinMode(t1, OUTPUT);
  pinMode(t2, OUTPUT);
  pinMode(t3, OUTPUT);
  analogWrite(t1, 0);
  analogWrite(t2, 0);
  analogWrite(t3, 0);
}

void loop()
{
  float avgTemp = 0.0;
  for(int i=0; i<loopCount; i++)
  {
    int sensorVal = analogRead(sensorPin);
    float voltage = (sensorVal/1024.0) *5.0;
    float temperature = (voltage - .5) * 100;
    avgTemp += temperature;
    delay(1);
  }
  
  avgTemp = avgTemp / loopCount;
  
  Serial.print("Temp=: ");
  Serial.println(avgTemp);
  
  int v1 = constrain((avgTemp - baselineTemp)/subRange * 256, 0, 255);
  int v2 = constrain((avgTemp - baselineTemp - subRange)/subRange * 256, 0, 255);
  int v3 = constrain((avgTemp - baselineTemp - (2 * subRange))/subRange * 256, 0, 255);

  analogWrite(t1, v1);
  analogWrite(t2, v2);
  analogWrite(t3, v3);

   
}
