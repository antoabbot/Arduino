#include "Arduino.h"
#include "JoyStick.h"

  JoyStick::JoyStick(int chanX, int chanY, int deadZone)
    : _chanX(chanX), _chanY(chanY), _deadZone(deadZone)
  {
  }

  void JoyStick::calibrate()
  {
    delay(500);

    _zeroX = analogRead(_chanX);
    _zeroY = analogRead(_chanY);

    _minX = _zeroX;
    _maxX = _zeroX;
    _minY = _zeroY;
    _maxY = _zeroY;

    for(;;)
    {
      delay(10);
      
      int xVal = analogRead(_chanX);
      int yVal = analogRead(_chanY);


      Serial.print(xVal);
      Serial.print(",");
      Serial.print(yVal);
      Serial.print("==[");
      Serial.print(_minX);
      Serial.print(",");
      Serial.print(_zeroX);
      Serial.print(",");
      Serial.print(_maxX);
      Serial.print(",");
      Serial.print(_minY);
      Serial.print(",");
      Serial.print(_zeroY);
      Serial.print(",");
      Serial.print(_maxY);
      Serial.println("]");


      // Get out if the button is depressed
      if ((xVal == 1023) || (yVal == 1023))
      {
        break;
      }

      if (xVal > _maxX)
        _maxX = xVal;
      if (xVal < _minX)
        _minX = xVal;
      if (yVal > _maxY)
        _maxY = yVal;
      if (yVal < _minY)
        _minY = yVal;
    }

  }

  int getChan(int chan, int minval, int zeroval, int maxval, int deadZone)
  {
    int val = analogRead(chan);
    int out;

    if (val == 1023) return 0; // Depressed
    
    int diff = val - zeroval;
    
    if (diff > deadZone)
    {
      out = map(val, zeroval, maxval, 0, 1024);
      out = constrain(out, 0, 1024);  
    }
    else if (diff < -deadZone)
    {
      out = map(val, minval, zeroval, -1024, 0);
      out = constrain(out, -1024, 0);      
    }
    else
    {
      out = 0;
    }    

    return out;
     
  }

  int JoyStick::getX()
  {
    int out = getChan(_chanX, _minX, _zeroX, _maxX, _deadZone);
    return out;
  }

  int JoyStick::getY()
  {
    int out = getChan(_chanY, _minY, _zeroY, _maxY, _deadZone);
    return out;
  }


