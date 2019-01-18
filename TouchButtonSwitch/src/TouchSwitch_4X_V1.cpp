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
  this->addButton(new TouchButton(TS_4X_V1_BTN1_PIN, TS_4X_V1_BTN1_ID));
  this->addButton(new TouchButton(TS_4X_V1_BTN2_PIN, TS_4X_V1_BTN2_ID));
  this->addButton(new TouchButton(TS_4X_V1_BTN3_PIN, TS_4X_V1_BTN3_ID));
  this->addButton(new TouchButton(TS_4X_V1_BTN4_PIN, TS_4X_V1_BTN4_ID));
  // we also have to set up the proximity sensors
  // in this case we do have 2 sensors. one at the top and one at the bottom, so  we may be
  // able to have simple gestures like 'swipe' up and 'swipe' down
  this->addButton(new ProximityTouchButton(TS_4X_V1_PROX1_PIN, TS_4X_V1_PROX1_ID));
  this->addButton(new ProximityTouchButton(TS_4X_V1_PROX2_PIN, TS_4X_V1_PROX2_ID));
}


void TouchSwitch_4X_V1::setMode(SWITCH_MODE _mode, uint16_t _modeLevel)
{
  TouchSwitch::setMode(_mode, _modeLevel);
  // TODO: update the LED controller mode (visible state of the mode)
}

/*
void TouchSwitch_4X_V1::initParameters()
{
    // setup settings for sensor 1
    bool    enableMultiTouch  = (bool) _Konnekting.getUINT8Param(PARAM_button1_multiTouchEnabled);
    uint8_t longTouchMode     = (uint8_t) _Konnekting.getUINT8Param(PARAM_button1_longTouchMode);
    uint8_t mode              = (uint8_t) _Konnekting.getUINT8Param(PARAM_button1_mode);
    this->getButtonById(TS_4X_V1_BTN1_ID)->parmMultipleTapsEnabled(enableMultiTouch);
    this->getButtonById(TS_4X_V1_BTN1_ID)->parmPositioningModeEnabled(longTouchMode == 2 ? true : false);
    Debug.println(F("Sensor %u: Mode=%u, LongTouchMode=%u, MultiTouch=%u, "), SENSORID_1, mode, longTouchMode, enableMultiTouch);

}
*/

// TODO: specify the ID's of the button so we can identify them
// TODO: when there is proximity on any proximity button/sensor, then light up all the leds
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!

// add gesture event on the touchSwitch class (gestureEnum) <-- will be provided by child class
// add proximity event on the touchSwitch class/or base class? even no touch may have proximity?! (id, proximityLevel)






