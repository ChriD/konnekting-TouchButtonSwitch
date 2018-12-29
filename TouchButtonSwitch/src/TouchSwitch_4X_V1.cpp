/*
  TouchSwitch_4X_V1.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "TouchSwitch_4X_V1.h"


TouchSwitch_4X_V1::TouchSwitch_4X_V1() : TouchSwitch()
{
}


TouchSwitch_4X_V1::~TouchSwitch_4X_V1()
{
}

void TouchSwitch_4X_V1::initButtons()
{
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_1));
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_2));
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_3));
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_4));
}





