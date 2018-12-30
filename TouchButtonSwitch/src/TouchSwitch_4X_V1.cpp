/*
  TouchSwitch_4X_V1.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

/*
  TODO: giva a name for the switch?!
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
  // we have to set up ours switch by defining the touch buttons it does have
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_1));
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_2));
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_3));
  this->addButton(new TouchButton(TS_PIN_SENSOR_BUTTON_4));
  // we also have to set up the proximity sensors
  // in this case we do have 2 sensors. one at the top and one at the bottom, so  we may be
  // able to have simple gestures like 'swipe' up and 'swipe' down
  this->addButton(new ProximityTouchButton(TS_PIN_SENSOR_PROXIMITY_1));
  this->addButton(new ProximityTouchButton(TS_PIN_SENSOR_PROXIMITY_2));
}

// TODO: specify the ID's of the button so we can identify them
// TODO: when there is proximity on any proximity button/sensor, then light up all the leds
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!

// add gesture event on the touchSwitch class (gestureEnum) <-- will be provided by child class
// add proximity event on the touchSwitch class/or base class? even no touch may have proximity?! (id, proximityLevel)






