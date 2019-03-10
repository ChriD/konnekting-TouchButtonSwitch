/*
  TouchSwitch_5X_V1.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.

  This is a 5 button touch switch with one main button in the middle and 4 sub buttons each 2 left and right
  It does have one single RGB Led in the middle of the main button for siganlizing the button modes and user
  states. It also does have an optional speaker for click response and a optional BME280 for temperature and humidity
*/

#include "Arduino.h"
#include "TouchSwitch_5X_V1.h"


TouchSwitch_5X_V1::TouchSwitch_5X_V1() : TouchSwitch()
{
  this->lastPatternRunTime = 0;
  this->rgbLed = NULL;
}


TouchSwitch_5X_V1::~TouchSwitch_5X_V1()
{
}


void TouchSwitch_5X_V1::initButtons()
{
  // we have to set up ours switch by defining the touch buttons it does have
  this->addButton(new TouchButton(A2, 1));
  this->addButton(new TouchButton(A1, 2));
  this->addButton(new TouchButton(A3, 3));
  this->addButton(new TouchButton(A4, 4));
  this->addButton(new TouchButton(A5, 5));

  // TODO: Add PRG Button which is a normal "PinButton"
  //this->addButton(new PinButton(TS_4X_V1_BTN1_PIN, TS_4X_V1_BTN1_ID, FALLING));

  // TODO: reverse due common anode!
  this->rgbLed = new LedPattern_RGB(11,12,13);

  // due we have a common anode, setting 255 on each pwm pins does result in no light
  /*
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  analogWrite(11, 255);
  analogWrite(12, 255);
  analogWrite(13, 255);
  */
}


// TODO: Add general init()?
void TouchSwitch_5X_V1::setMode(SWITCH_MODE _mode, uint16_t _modeLevel)
{
  TouchSwitch::setMode(_mode, _modeLevel);

  // when chaangeing the mode we have to stop the current pattern which is running
  this->rgbLed->stop();

  if(_mode == SWITCH_MODE::NORMAL)
    this->rgbLed->start(ledPattern_Normal);
  if(_mode == SWITCH_MODE::PROG)
    this->rgbLed->start(ledPattern_Normal);
  if(_mode == SWITCH_MODE::CALIBRATION)
    this->rgbLed->start(ledPattern_Normal);
  if(_mode == SWITCH_MODE::SETUP)
    this->rgbLed->start(ledPattern_Normal);
}


void TouchSwitch_5X_V1::onButtonAction(uint16_t _buttonId, uint16_t _type, uint16_t _value)
{
  TouchSwitch::onButtonAction(_buttonId, _type, _value);

  // if we do have a touch on a button we do signalize it by a short pattern
  if(this->mode == SWITCH_MODE::NORMAL)
  {
    this->rgbLed->stop();
    this->rgbLed->start(ledPattern_Normal);
  }
}


void TouchSwitch_5X_V1::task()
{
  TouchSwitch::task();

  if(millis() - this->lastPatternRunTime > 10)
  {
    //SERIAL_PORT_USBVIRTUAL.println("UPD LED");
    this->rgbLed->update();

    // TODO:  if there is no animation running, then we do set the "current color" or "current pattern" which may
    //        represant a state

    //analogWrite(11, 255); // R
    //analogWrite(12, 255); // G
    //analogWrite(13, 0);   // B

    this->lastPatternRunTime = millis();
  }

  // TODO: periodically get the temperature and humidity if enabled
  // this may lead to problems if a button is clicked meanwhile?!

}



// TODO: specify the ID's of the button so we can identify them
// TODO: check if we can override callback functions?!
// TODO: add KNX stuff in here?!?! I think this is not a good idea?!






