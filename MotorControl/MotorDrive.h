#ifndef MotorDrive_h
#define MotorDrive_h

#include "Arduino.h"


class MotorDrive
{
  private:
    int _pinA;
    int _pinB;
  public:
  
    MotorDrive(int pinA, int pinB);
    void run(int value);
  
};

#endif
