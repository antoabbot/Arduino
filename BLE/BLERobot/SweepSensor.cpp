#include "SweepSensor.h"

SweepSensor::SweepSensor(uint8_t servoPin, uint8_t pingPin1, uint8_t pingPin2, uint16_t maxDistance)
  : sonar(pingPin1, pingPin2, maxDistance),
  servoPin(servoPin),
  maxDistance(maxDistance)
{
  servo.attach(servoPin);
}

void SweepSensor::centre()
{
  setangle(90);
}

void SweepSensor::setangle(uint8_t newangle)
{
  angle = constrain(newangle, 0, 180);
  servo.write(angle);
}


uint16_t SweepSensor::get_distance()
{
  uint16_t dist = sonar.ping_cm();
  if (dist == 0) dist = maxDistance;
  return dist;
}

uint16_t SweepSensor::get_angle()
{
  return angle;
}


