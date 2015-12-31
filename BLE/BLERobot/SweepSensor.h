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

#define DISTBUFFSIZE    5

class SweepSensor
{
  private:
    NewPing sonar;
    uint16_t maxDistance;
    uint8_t angle;

    uint16_t distbuff[DISTBUFFSIZE];   // Buffer containing measurements
    volatile uint8_t distix = 0;     // index into buffer
  public:
    SweepSensor(uint8_t pingPin1, uint8_t pingPin2, uint16_t maxDistance);
    void measure();
    uint16_t get_distance();


};

#endif // __SweepSensor_h__
