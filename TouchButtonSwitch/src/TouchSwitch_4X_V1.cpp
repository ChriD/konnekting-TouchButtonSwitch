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


void TouchSwitch_4X_V1::initParameters()
{
  /*
  // thresholds
    uint16_t touch_threshold          = Konnekting.getUINT16Param(PARAM_touch_threshold);
    uint16_t mode_longtouch_threshold = Konnekting.getUINT16Param(PARAM_mode_longtouch_threshold);
    //uint16_t mode_position_threshold  = Konnekting.getUINT16Param(PARAM_mode_position_threshold);
    //touchSwitch->setThresholds(touch_threshold, mode_longtouch_threshold, mode_position_threshold);
    //Debug.println(F("Thresholds: Touch=%u, Longtouch=%u, Position=%u"), touch_threshold, mode_longtouch_threshold, mode_position_threshold);
    //this->parm

    // backlights
    uint8_t valueStandby      = Konnekting.getUINT8Param(PARAM_light_intensity_standy);
    uint8_t valueProximity    = Konnekting.getUINT8Param(PARAM_light_intensity_proximity);
    //touchSwitch->setBacklightParameters(valueStandby, valueProximity);
    //Debug.println(F("Backlight: Standby=%u, Proximity=%u"), valueStandby, valueProximity);

    // touch ic settings
    uint8_t tsic_sensitivity = Konnekting.getUINT8Param(PARAM_tsic_sensitivity);
    //touchSwitch->getTouchControllerObject()->setSensorSensitivity(tsic_sensitivity);
    uint8_t tsic_fingerThreshold = Konnekting.getUINT8Param(PARAM_tsic_fingerThreshold);
    //touchSwitch->getTouchControllerObject()->setSensorFingerThreshold(tsic_fingerThreshold);
    //Debug.println(F("TouchIC ButtonSensitivity: %u, ButtonFingerThreshold=%u"), tsic_sensitivity, tsic_fingerThreshold);

    // setup settings for sensor 1
    bool    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button1_enableMultiTouch);
    uint8_t mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button1_mode);
    //touchSwitch->setButtonParameters(SENSORID_1, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_1, enableMultiTouch, mode);

     // setup settings for sensor 2
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button2_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button2_mode);
    //touchSwitch->setButtonParameters(SENSORID_2, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_2, enableMultiTouch, mode);

     // setup settings for sensor 3
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button3_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button3_mode);
    //touchSwitch->setButtonParameters(SENSORID_3, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_3, enableMultiTouch, mode);

    // setup settings for sensor 4
    enableMultiTouch  = (bool) Konnekting.getUINT8Param(PARAM_button4_enableMultiTouch);
    mode              = (uint8_t) Konnekting.getUINT8Param(PARAM_button4_mode);
    //touchSwitch->setButtonParameters(SENSORID_4, enableMultiTouch, mode);
    //Debug.println(F("Sensor %u: MultiTouch=%u, Mode=%u"), SENSORID_4, enableMultiTouch, mode);
    */
}

// TODO: specify the ID's of the button so we can identify them
// TODO: when there is proximity on any proximity button/sensor, then light up all the leds
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!

// add gesture event on the touchSwitch class (gestureEnum) <-- will be provided by child class
// add proximity event on the touchSwitch class/or base class? even no touch may have proximity?! (id, proximityLevel)






