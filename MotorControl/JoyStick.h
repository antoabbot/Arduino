#ifndef JoyStick_h
#define JoyStick_h

#include "Arduino.h"

class JoyStick
{
  private:
    int _chanX;
    int _chanY;

    int _zeroX, _zeroY, _minX, _maxX, _minY, _maxY;
    int _deadZone;

    
  public:
  
    JoyStick(int chanX, int chanY, int deadZone);

    void calibrate();

    int getX();
    int getY();

    

};

#endif
