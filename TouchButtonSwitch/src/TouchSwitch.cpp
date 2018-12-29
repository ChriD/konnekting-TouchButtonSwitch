/*
  TouchSwitch.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "TouchSwitch.h"


TouchSwitch::TouchSwitch() : BaseSwitch()
{
}


TouchSwitch::~TouchSwitch()
{
}

boolean TouchSwitch::setup()
{
  return BaseSwitch::setup();
}

void TouchSwitch::startCalibration()
{
  TouchButton *touchButton;
  // start calibration on all touch buttons which are defined
  for(uint8_t i=0; i<=this->maxButtonIdx; i++)
  {
    // be sure we do only calibrate touch buttons, only those have the 'startCalibration' function
    // we have to do this because we can not assume that all buttons on the switch are touch buttons (e.g reset or PRG button)
    if(static_cast<TouchButton*>(this->buttons[i]) != nullptr)
    {
      touchButton = static_cast<TouchButton*>(this->buttons[i]);
      touchButton->startCalibration();
    }
  }
}

void TouchSwitch::task()
{
  BaseSwitch::task();
}



