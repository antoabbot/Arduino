#include "SweepSensor.h"

SweepSensor::SweepSensor(uint8_t pingPin1, uint8_t pingPin2, uint16_t maxDistance)
  : sonar(pingPin1, pingPin2, maxDistance),
  maxDistance(maxDistance)
{
  memset(distbuff, 0, 5);
}


void SweepSensor::measure()
{
  uint16_t dist = sonar.ping_cm();
  distbuff[distix++] = dist;
  if (distix >= DISTBUFFSIZE) distix = 0;
}

uint16_t SweepSensor::get_distance()
{
  uint16_t sum = 0;
  uint8_t valCnt = 0;
  for (uint8_t i=0; i< DISTBUFFSIZE; ++i)
  {
    uint16_t val = distbuff[i];
    if (val != 0) 
    {
      valCnt++;
      sum += val;
    }
  }
  
  if (valCnt == 0) 
  {
    return maxDistance;
  }
  else
  {
    return sum / valCnt;
  }
}



