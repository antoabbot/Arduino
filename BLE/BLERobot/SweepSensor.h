/*****************************************************************************/
// Robot remote control via Bluetooth LE
// Author: Anthony Abbot
// Date: 27 Dec 2015
// 
// This code has been developed for use by the Olney Coder Dojo club
//
// Class to support a sweeping proximity sensor controlled by a servo
//
/*****************************************************************************/

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
    uint8_t angle;

    uint16_t distbuff[5];   // Buffer containing measurements
    uint8_t distix = 0;     // index into buffer
  public:
    SweepSensor(uint8_t servoPin, uint8_t pingPin1, uint8_t pingPin2, uint16_t maxDistance);
    void centre();
    void setangle(uint8_t angle);

    void measure();
    uint16_t get_distance();
    uint16_t get_angle();

};

#endif // __SweepSensor_h__
