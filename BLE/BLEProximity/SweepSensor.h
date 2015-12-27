#ifndef __SweepSensor_h__
#define __SweepSensor_h__

#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>

class SweepSensor
{
  private:
    NewPing sonar;
    Servo servo;
    uint8_t servoPin;
    uint16_t maxDistance;
  public:
    SweepSensor(uint8_t servoPin, uint8_t pingPin1, uint8_t pingPin2, uint16_t maxDistance);
    void centre();

    uint16_t get_distance();
};

#endif // __SweepSensor_h__
