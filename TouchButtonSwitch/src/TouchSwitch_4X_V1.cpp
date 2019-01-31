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
  this->lastPatternRunTime = 0;
  this->led1 = NULL;
  this->led2 = NULL;
  this->led3 = NULL;
  this->led4 = NULL;
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

  // TODO: outsourcing?! LedPatternAnimator. ????
  this->led1 = new LedPattern_Mono(12);
  this->led2 = new LedPattern_Mono(13);
  this->led3 = new LedPattern_Mono(11);
  this->led4 = new LedPattern_Mono(9);

  analogWrite(12, 0);
  analogWrite(13, 0);
  analogWrite(11, 0);
  analogWrite(9, 0);
}


// TODO: Add general init()?
void TouchSwitch_4X_V1::setMode(SWITCH_MODE _mode, uint16_t _modeLevel)
{
  TouchSwitch::setMode(_mode, _modeLevel);

  this->led1->stop();
  this->led2->stop();
  this->led3->stop();
  this->led4->stop();

  // TODO: update the LED controller mode (visible state of the mode)
  if(_mode == SWITCH_MODE::NORMAL)
  {
    this->led1->start(ledPattern_Normal);
    this->led2->start(ledPattern_Normal);
    this->led3->start(ledPattern_Normal);
    this->led4->start(ledPattern_Normal);
  }
  if(_mode == SWITCH_MODE::PROG)
  {
    this->led1->start(ledPattern_Prog);
    this->led2->start(ledPattern_Prog);
    this->led3->start(ledPattern_Prog);
    this->led4->start(ledPattern_Prog);
  }
  if(_mode == SWITCH_MODE::CALIBRATION)
  {
    this->led1->start(ledPattern_Calibration);
    this->led2->start(ledPattern_Calibration);
    this->led3->start(ledPattern_Calibration);
    this->led4->start(ledPattern_Calibration);
  }
  if(_mode == SWITCH_MODE::SETUP)
  {
    this->led1->start(ledPattern_Setup);
    this->led2->start(ledPattern_Setup);
    this->led3->start(ledPattern_Setup);
    this->led4->start(ledPattern_Setup);
  }
}


void TouchSwitch_4X_V1::onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  TouchSwitch::onButtonAction(_buttonId, _type, _value);
  if(this->mode == SWITCH_MODE::NORMAL)
  {
    // TODO: blink one touch

    if(_buttonId == 1)
    {
      this->led1->stop();
      this->led1->start(ledPattern_Touch);
    }
    if(_buttonId == 2)
    {
      this->led2->stop();
      this->led2->start(ledPattern_Touch);
    }
    if(_buttonId == 3)
    {
      this->led3->stop();
      this->led3->start(ledPattern_Touch);
    }
    if(_buttonId == 4)
    {
      this->led4->stop();
      this->led4->start(ledPattern_Touch);
    }
  }
}


void TouchSwitch_4X_V1::task()
{
  TouchSwitch::task();

  if(millis() - this->lastPatternRunTime > 10)
  {
    this->led1->update();
    this->led2->update();
    this->led3->update();
    this->led4->update();
    this->lastPatternRunTime = millis();
  }
}

/* TODO:  * make genric!!!
*/




// TODO: specify the ID's of the button so we can identify them
// TODO: when there is proximity on any proximity button/sensor, then light up all the leds
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!

// add gesture event on the touchSwitch class (gestureEnum) <-- will be provided by child class
// add proximity event on the touchSwitch class/or base class? even no touch may have proximity?! (id, proximityLevel)






