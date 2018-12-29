/*
  ProximityTouchButton.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "ProximityTouchButton.h"


ProximityTouchButton::ProximityTouchButton(uint8_t _pin) : TouchButton(_pin)
{
}


ProximityTouchButton::~ProximityTouchButton()
{
}

// TODO: add Task, call super and check for proximity after super (we can use 'lastSampleValue' for that)
// add some proximity levels (e.g 5% above highest is level 1, 7% is level 2, aso...)

// setup the callback onProximity(_id, _level)

  //--> so In fact all buttons can have the ability of proximity detection which is good?

// TODO: and / or create own class for proximity sensor?!

