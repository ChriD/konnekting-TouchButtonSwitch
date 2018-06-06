/*
  CY8TouchSwitchh.h
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "CY8TouchSwitch.h"


CY8TouchSwitch::CY8TouchSwitch()
{
  this->highestButtonIdx = 0;
}


CY8TouchSwitch::~CY8TouchSwitch()
{

}


void CY8TouchSwitch::setup()
{
  // create and setup the instance of the touch controller class/object which will do a lot of work for us
  // the touvh controler itself is powerd with a standard setup on start
  this->touchController = new CY8CMBR3116(0x37);
  this->touchController->setThresholds(250, 1250);
  this->touchController->setup();
  this->touchController->setSensorStateCallback(std::bind(&CY8TouchSwitch::sensorStateEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->touchController->setTouchEventCallback(std::bind(&CY8TouchSwitch::touchEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->touchController->setProximityEventCallback(std::bind(&CY8TouchSwitch::proximityEvent, this, std::placeholders::_1, std::placeholders::_2));
  this->touchController->setGestureEventCallback(std::bind(&CY8TouchSwitch::gestureEvent, this, std::placeholders::_1));
  this->touchController->reset();
}


void CY8TouchSwitch::addButton(uint8_t _sensorId, uint8_t _ledPin, bool _enableMultipleTouch)
{
  if(_ledPin)
  {
    this->ledWorkers[this->highestButtonIdx] = new LEDWorker(_ledPin);
    this->ledWorkers[this->highestButtonIdx]->setup();
  }
  this->touchController->enableMultipleTouch(_sensorId, _enableMultipleTouch);
  this->sendorIds[this->highestButtonIdx] = _sensorId;
  this->highestButtonIdx ++;
}

void CY8TouchSwitch::sensorStateEvent(uint8_t sensorType, uint8_t _sensorId, bool _value)
{

}

void CY8TouchSwitch::touchEvent(uint8_t _sensorId, uint8_t _event, uint8_t _count)
{
  // if we are in any other mode than NORMAL, we should not handle any touches
  if(this->mode != TS_MODE_NORMAL)
    return;

  // check if this event is one we have to consider, for this we check if we do have a
  // registered button for the given sensor id
  Debug.println(F("Touch event on sensor: %u. Event: %u (%u)"), _sensorId, _event, _count);
}

void CY8TouchSwitch::proximityEvent(uint8_t _sensorId, uint8_t _event)
{
  // if we are in any other mode than NORMAL, we should not handle any proximity alerts
  if(this->mode != TS_MODE_NORMAL)
    return;

  // check if this event is one we have to consider, for this we check if we do have a
  // registered proximity for the given proximity sensor id

  if(_event == 1)
  {
    Debug.println(F("Proximity alert on %u"), _sensorId);
    for(uint8_t i=0; i <=this->highestButtonIdx; i++)
    {
      this->ledWorkers[i]->fade(500, 255);
    }
  }
  else
  {
    Debug.println(F("Proximity lost on %u"), _sensorId);
    for(uint8_t i=0; i <=this->highestButtonIdx; i++)
    {
      this->ledWorkers[i]->fade(500, 10);
    }
  }

  // if there is a proximity detected, we do light up all leds with a fade include

  // if the proximity is disappeared we wait a little bit and fade out all leds
}


void CY8TouchSwitch::gestureEvent(uint8_t _event)
{
    // TODO: @@@
}


void CY8TouchSwitch::resetTouchController()
{
  this->touchController->reset();
}


void CY8TouchSwitch::interrupt()
{
  this->touchController->interrupt();
}


void CY8TouchSwitch::changeMode(uint8_t _mode, bool _force)
{
  if(this->mode != _mode || _force)
  {
    this->mode = _mode;
    switch(this->mode)
    {
      case TS_MODE_NORMAL:
        this->setMode_Normal();
        break;
      case TS_MODE_PROG:
        this->setMode_Prog();
        break;
      case TS_MODE_SETUP:
        this->setMode_Setup();
        break;
    }
  }
}


void CY8TouchSwitch::setMode_Normal()
{
  for(uint8_t i=0; i <=this->highestButtonIdx; i++)
  {
    this->ledWorkers[i]->fade(500, 10);
  }
}


void CY8TouchSwitch::setMode_Prog()
{
  for(uint8_t i=0; i <=this->highestButtonIdx; i++)
  {
    this->ledWorkers[i]->blink();
  }
}


void CY8TouchSwitch::setMode_Setup()
{
  for(uint8_t i=0; i <=this->highestButtonIdx; i++)
  {
    this->ledWorkers[i]->blink(250,250);
  }
}

void CY8TouchSwitch::task()
{
  // the touch controler needs a task trigger for working correctly
  // it will handle the touches and will raise the events we do need
  this->touchController->task();
  // now lets handle the leds output.
  // there are some options like fadeIn, fadeOut, StandbyLight, ProgrammingMode, ErrorMode...
  for(uint8_t i=0; i <=this->highestButtonIdx; i++)
  {
    this->ledWorkers[i]->task();
  }
}


