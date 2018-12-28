/*
  TouchButton.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "TouchButton.h"


TouchButton::TouchButton(uint8_t _I2CAddress)
{
  this->pin              = _I2CAddress;
}


TouchButton::~TouchButton()
{
}


// uint8_t _sensorId, uint8_t _event, uint8_t _count
void TouchButton::setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)> _touchEventCallback)
{
  this->touchEventCallback = _touchEventCallback;
}

// uint8_t _sensorId, uint8_t _event
void TouchButton::setProximityEventCallback(std::function<void(uint8_t, uint8_t)> _proximityEventCallback)
{
  this->proximityEventCallback = _proximityEventCallback;
}

// uint8_t _gesture
void TouchButton::setGestureEventCallback(std::function<void(uint8_t)> _gestureEventCallback)
{
  this->gestureEventCallback = _gestureEventCallback;
}