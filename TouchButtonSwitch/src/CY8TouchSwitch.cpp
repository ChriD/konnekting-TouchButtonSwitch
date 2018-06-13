/*
  CY8TouchSwitchh.h
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "CY8TouchSwitch.h"


CY8TouchSwitch::CY8TouchSwitch()
{
  this->nextButtonIdx = 0;
}


CY8TouchSwitch::~CY8TouchSwitch()
{

}


void CY8TouchSwitch::setup()
{
  // create and setup the instance of the touch controller class/object which will do a lot of work for us
  // the touvh controler itself is powerd with a standard setup on start
  this->touchController = new CY8CMBR3116(0x37);
  this->touchController->setThresholds(250, 1250, 750);
  this->touchController->setup();
  this->touchController->setSensorStateCallback(std::bind(&CY8TouchSwitch::sensorStateEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->touchController->setTouchEventCallback(std::bind(&CY8TouchSwitch::touchEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->touchController->setProximityEventCallback(std::bind(&CY8TouchSwitch::proximityEvent, this, std::placeholders::_1, std::placeholders::_2));
  this->touchController->setGestureEventCallback(std::bind(&CY8TouchSwitch::gestureEvent, this, std::placeholders::_1));
  this->touchController->reset();
}



// uint8_t _sensorId, uint8_t _event, uint8_t _count
void CY8TouchSwitch::setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)> _touchEventCallback)
{
  this->touchEventCallback = _touchEventCallback;
}

// uint8_t _sensorId, uint8_t _event
void CY8TouchSwitch::setProximityEventCallback(std::function<void(uint8_t, uint8_t)> _proximityEventCallback)
{
  this->proximityEventCallback = _proximityEventCallback;
}

// uint8_t _gesture
void CY8TouchSwitch::setGestureEventCallback(std::function<void(uint8_t)> _gestureEventCallback)
{
  this->gestureEventCallback = _gestureEventCallback;
}


void CY8TouchSwitch::addButton(uint8_t _sensorId, uint8_t _ledPin, bool _enableMultipleTouch,bool _enablePositioningTouch)
{
  if(_ledPin)
  {
    this->ledWorkers[this->nextButtonIdx] = new LEDWorker(_ledPin);
    this->ledWorkers[this->nextButtonIdx]->setup();
  }
  this->touchController->enableMultipleTouch(_sensorId, _enableMultipleTouch);
  this->touchController->enablePositioningTouch(_sensorId, _enablePositioningTouch);
  this->sendorIds[this->nextButtonIdx] = _sensorId;
  this->nextButtonIdx ++;
}


bool CY8TouchSwitch::isSensorIdActive(uint8_t _sensorId)
{
  bool exists = std::find(std::begin(this->sendorIds), std::end(this->sendorIds), _sensorId) != std::end(this->sendorIds);
  return exists;
}


void CY8TouchSwitch::sensorStateEvent(uint8_t sensorType, uint8_t _sensorId, bool _value)
{
  // we do nod need the state info for anything
}


void CY8TouchSwitch::touchEvent(uint8_t _sensorId, uint8_t _event, uint8_t _count)
{
  // if we are in any other mode than NORMAL, we should not handle any touches
  if(this->mode != TS_MODE_NORMAL)
    return;

  // check if this event is one we have to consider, for this we check if we do have a
  // registered button for the given sensor id
  Debug.println(F("Touch event on sensor: %u. Event: %u (%u)"), _sensorId, _event, _count);

  // only trigger event if we do have attached and if we do have the sensor id sepcified
  if(this->touchEventCallback && this->isSensorIdActive(_sensorId))
    this->touchEventCallback(_sensorId, _event, _count);
}


void CY8TouchSwitch::proximityEvent(uint8_t _sensorId, uint8_t _event)
{
  // if we are in any other mode than NORMAL, we should not handle any proximity alerts
  if(this->mode != TS_MODE_NORMAL)
    return;

  // if there is a proximity detected, we do light up all leds with a fade in
  if(_event == 1)
  {
    Debug.println(F("Proximity alert on %u"), _sensorId);
    for(uint8_t i=0; i < this->nextButtonIdx; i++)
    {
      this->ledWorkers[i]->fade(500, 255);
    }
  }
  // if the proximity is disappeared we wait a little bit and fade out all leds
  else
  {
    Debug.println(F("Proximity lost on %u"), _sensorId);
    for(uint8_t i=0; i < this->nextButtonIdx; i++)
    {
      this->ledWorkers[i]->fade(500, 10);
    }
  }

  if(this->proximityEventCallback)
    this->proximityEventCallback(_sensorId, _event);
}


void CY8TouchSwitch::gestureEvent(uint8_t _event)
{
    Debug.println(F("Gesture Event: %u"), _event);
    if(this->gestureEventCallback)
      this->gestureEventCallback(_event);
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
      case TS_MODE_STARTUP1:
        this->setMode_Startup(1);
        break;
      case TS_MODE_STARTUP2:
        this->setMode_Startup(2);
        break;
      case TS_MODE_STARTUP3:
        this->setMode_Startup(3);
        break;
      case TS_MODE_STARTUP4:
        this->setMode_Startup(4);
        break;
    }
  }
}


void CY8TouchSwitch::setMode_Normal()
{
  for(uint8_t i=0; i < this->nextButtonIdx; i++)
  {
    this->ledWorkers[i]->fade(500, 10);
  }
}


void CY8TouchSwitch::setMode_Prog()
{
  for(uint8_t i=0; i < this->nextButtonIdx; i++)
  {
    this->ledWorkers[i]->blink();
  }
}


void CY8TouchSwitch::setMode_Setup()
{
  for(uint8_t i=0; i < this->nextButtonIdx; i++)
  {
    this->ledWorkers[i]->blink(250,250);
  }
}


void CY8TouchSwitch::setMode_Startup(uint8_t _startupLevel)
{
  if(_startupLevel >= 1 && this->ledWorkers[0] != NULL)
    this->ledWorkers[0]->set(255);
  if(_startupLevel >= 2 && this->ledWorkers[1] != NULL)
    this->ledWorkers[1]->set(255);
  if(_startupLevel >= 3 && this->ledWorkers[2] != NULL)
    this->ledWorkers[2]->set(255);
  if(_startupLevel >= 4 && this->ledWorkers[3] != NULL)
    this->ledWorkers[3]->set(255);
}


void CY8TouchSwitch::task()
{
  // the touch controler needs a task trigger for working correctly
  // it will handle the touches and will raise the events we do need
  this->touchController->task();
  // now lets handle the leds output.
  // there are some options like fadeIn, fadeOut, StandbyLight, ProgrammingMode, ErrorMode...
  for(uint8_t i=0; i < this->nextButtonIdx; i++)
  {
    this->ledWorkers[i]->task();
  }
}


