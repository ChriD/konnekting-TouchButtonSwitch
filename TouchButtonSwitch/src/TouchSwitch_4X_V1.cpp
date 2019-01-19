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
  // TODO: Add PRG Button which is a normal "PinButton"
  //this->addButton(new PinButton(TS_4X_V1_BTN1_PIN, TS_4X_V1_BTN1_ID, FALLING));

  // TODO: onlx for version 1
  pinMode(7, INPUT);

  // TODO: create AddLight() method and do it generic
  ledWorkers[0] = new LEDWorker(12);
  ledWorkers[1] = new LEDWorker(13);
  ledWorkers[2] = new LEDWorker(11);
  ledWorkers[3] = new LEDWorker(9);

  ledWorkers[0]->setup();
  ledWorkers[1]->setup();
  ledWorkers[2]->setup();
  ledWorkers[3]->setup();

}

// TODO: Add general init()?

void TouchSwitch_4X_V1::setMode(SWITCH_MODE _mode, uint16_t _modeLevel)
{
  TouchSwitch::setMode(_mode, _modeLevel);
  // TODO: update the LED controller mode (visible state of the mode)
  if(_mode == SWITCH_MODE::NORMAL)
  {
    ledWorkers[0]->fade(500, 25);
    ledWorkers[1]->fade(500, 25);
    ledWorkers[2]->fade(500, 25);
    ledWorkers[3]->fade(500, 25);
  }
  if(_mode == SWITCH_MODE::PROG)
  {
    ledWorkers[0]->blink();
    ledWorkers[1]->blink();
    ledWorkers[2]->blink();
    ledWorkers[3]->blink();
  }
  if(_mode == SWITCH_MODE::CALIBRATION)
  {
    ledWorkers[0]->fade(500, 255);
    ledWorkers[1]->fade(500, 255);
    ledWorkers[2]->fade(500, 255);
    ledWorkers[3]->fade(500, 255);
  }
   if(_mode == SWITCH_MODE::SETUP)
  {
    /*
    ledWorkers[0]->set(0);
    ledWorkers[1]->set(0);
    ledWorkers[2]->set(0);
    ledWorkers[3]->set(0);
    */

    ledWorkers[_modeLevel-1]->set(25);
  }

  ledWorkers[0]->task();
  ledWorkers[1]->task();
  ledWorkers[2]->task();
  ledWorkers[3]->task();

}

void TouchSwitch_4X_V1::task()
{
  TouchSwitch::task();
  ledWorkers[0]->task();
  ledWorkers[1]->task();
  ledWorkers[2]->task();
  ledWorkers[3]->task();
}



// TODO: specify the ID's of the button so we can identify them
// TODO: when there is proximity on any proximity button/sensor, then light up all the leds
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!

// add gesture event on the touchSwitch class (gestureEnum) <-- will be provided by child class
// add proximity event on the touchSwitch class/or base class? even no touch may have proximity?! (id, proximityLevel)






