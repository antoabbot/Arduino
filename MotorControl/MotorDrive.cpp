#include "Arduino.h"
#include "MotorDrive.h"


  // Return the Pin A value for the motor
  int motorA(int val, int min, int max, int zero)
  {
    if (val < zero)
    {
      return 0;
    }
    
    return map(val, zero, max, 0, 255); 
  }
  
  // Return the Pin B value for the motor
  int motorB(int val, int min, int max, int zero)
  {
    if (val > zero)
    {
      return 0;
    }
    
    return map(val, min, zero, 255, 0); 
  }
  

  MotorDrive::MotorDrive(int pinA, int pinB)
  {
    _pinA = pinA;
    _pinB = pinB;

     pinMode(_pinA, OUTPUT);
     pinMode(_pinB, OUTPUT);
  }

  void MotorDrive::run(int value)
  {
    int xA, xB;
    if (value > 0)
    {
      xA = map(value, 0, 1024, 0, 255);
      xA = constrain(xA, 0, 255);
      xB = 0;
    }
    else
    {
      xA = 0;
      xB = map(value, 0, -1024, 0, 255);   
      xB = constrain(xB, 0, 255);   
    }
    
    analogWrite(_pinA, xA);
    analogWrite(_pinB, xB);
  }
