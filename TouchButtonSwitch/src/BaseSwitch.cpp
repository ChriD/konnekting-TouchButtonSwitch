/*
  BaseSwitch.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "BaseSwitch.h"


BaseSwitch::BaseSwitch()
{
  this->maxButtonIdx = -1;
  this->callback_onButtonAction = NULL;
  this->callback_onProximityAlert = NULL;
}


BaseSwitch::~BaseSwitch()
{
}


boolean BaseSwitch::setup()
{
  boolean ret;
  // let the child classes init the buttons (add the buttons)
  this->initButtons();
  // when init is done we need to setup all the buttons for duty
  ret = this->setupButtons();

  return ret;
}


void BaseSwitch::attachCallbackOnButtonAction(const CallbackFunction_ButtonAction &_callback)
{
  this->callback_onButtonAction = _callback;
}

//
void BaseSwitch::attachCallbackOnProximityAlert(const CallbackFunction_ProximityAlert &_callback)
{
  this->callback_onProximityAlert = _callback;
}


uint16_t BaseSwitch::addButton(Button* _button, uint16_t _buttonId)
{
  this->maxButtonIdx++;
  this->buttons[this->maxButtonIdx] = _button;
  // set a button id if not specified by the user
  if(!_buttonId)
    this->buttons[this->maxButtonIdx]->parmId(this->maxButtonIdx + 1);
  else
    this->buttons[this->maxButtonIdx]->parmId(_buttonId);
  return this->maxButtonIdx;
}


Button* BaseSwitch::getButtonByIndex(uint16_t _index)
{
  if(_index > this->maxButtonIdx)
    return NULL;
  return this->buttons[_index];
}

Button* BaseSwitch::getButtonById(uint16_t _id)
{
  for(uint8_t i=0; i<=this->maxButtonIdx; i++)
  {
    if(this->buttons[i]->parmId() == _id)
      return this->buttons[i];
  }
  return NULL;
}


void BaseSwitch::initButtons()
{
  // TODO: @@@ has to be overwritten
}


boolean BaseSwitch::setupButtons()
{
  boolean ret = true;
  for(uint8_t i=0; i<=this->maxButtonIdx; i++)
  {
    // setup the callbacks for the button. All buttons use the same callback method
    // we can distinguish between the buttons due to the button ID provided by the user or the 'addButton' method
    this->buttons[i]->attachCallbackOnButtonAction(makeFunctor((CallbackFunction_ButtonAction*)0, *this, &BaseSwitch::onButtonAction));
    this->buttons[i]->attachCallbackOnButtonStateChanged(makeFunctor((CallbackFunction_ButtonStateChanged*)0, *this, &BaseSwitch::onButtonStateChanged));
    this->buttons[i]->attachCallbackOnProximityAlert(makeFunctor((CallbackFunction_ProximityAlert*)0, *this, &BaseSwitch::onProximityAlert));
    // do the setup routine for the button
    ret = ret && this->buttons[i]->setup();
  }
  return ret;
}


void BaseSwitch::startCalibration()
{
  // start calibration on all buttons, no matter if they are calibrateable or not
  // the method is in the base class and if no calibration is needed it wont be overwritten
  for(uint8_t i=0; i<=this->maxButtonIdx; i++)
  {
    this->buttons[i]->startCalibration();
  }
}


void BaseSwitch::onButtonAction(uint16_t _id, uint16_t _type, uint16_t _data)
{
  if(this->callback_onButtonAction)
    this->callback_onButtonAction(_id, _type, _data);

}

void BaseSwitch::onButtonStateChanged(uint16_t _id, uint16_t _state)
{
  // we do not expose anything here...
  // we attached to the callback because we may use this in future
}

void BaseSwitch::onProximityAlert(uint16_t _id, boolean _isProximity, uint16_t _proximityValue, uint16_t _proximityLevel)
{
  if(this->callback_onProximityAlert)
    this->callback_onProximityAlert(_id, _isProximity, _proximityValue, _proximityLevel);
}


void BaseSwitch::task()
{
  for(uint8_t i=0; i<=this->maxButtonIdx; i++)
  {
    this->buttons[i]->task();
  }
}



