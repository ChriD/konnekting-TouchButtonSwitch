/*
  CY8TouchSwitchh.h
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "CY8TouchSwitch.h"


CY8TouchSwitch::CY8TouchSwitch()
{
}


CY8TouchSwitch::~CY8TouchSwitch()
{

}


void CY8TouchSwitch::setup()
{
  // create and setup the instance of the touch controller class/object which will do a lot of work for us
  this->touchController = new CY8CMBR3116(0x37);
  this->touchController->setThresholds(250, 1250);
  this->touchController->setup();
  this->touchController->setSensorStateCallback(std::bind(&CY8TouchSwitch::sensorStateEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->touchController->setTouchEventCallback(std::bind(&CY8TouchSwitch::touchEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->touchController->setProximityEventCallback(std::bind(&CY8TouchSwitch::proximityEvent, this, std::placeholders::_1, std::placeholders::_2));
  this->touchController->setGestureEventCallback(std::bind(&CY8TouchSwitch::gestureEvent, this, std::placeholders::_1));

  this->ledWorker1 = new LEDWorker(D3);
  this->ledWorker1->setup();
}

void CY8TouchSwitch::sensorStateEvent(uint8_t sensorType, uint8_t _sensorId, bool _value)
{
  Serial.print("\n");
  Serial.print("Sensor ");
  Serial.print(_sensorId);
  Serial.print(" val ");
  Serial.print(_value);
}

void CY8TouchSwitch::touchEvent(uint8_t _sensorId, uint8_t _event, uint8_t _count)
{
  // check if this event is one we have to consider, for this we check if we do have a
  // registered button for the given sensor id
  Serial.print("\n");
  Serial.print("Sensor ");
  Serial.print(_sensorId);
  Serial.print(" says ");
  Serial.print(_event);
  Serial.print(" > ");
  Serial.print(_count);
}

void CY8TouchSwitch::proximityEvent(uint8_t _sensorId, uint8_t _event)
{
  // check if this event is one we have to consider, for this we check if we do have a
  // registered proximity for the given proximity sensor id
  Serial.print("\n");
  Serial.print("Proximity ");
  Serial.print(_sensorId);
  Serial.print(" says ");
  Serial.print(_event);

  if(_event == 1)
  {
    this->ledWorker1->fade(500, 255);
    //digitalWrite(D3, HIGH);
    //ledController->fadeIn(1)
    //ledController->fadeIn(2)
    //ledController->fadeIn(3)
  }
  else
  {
    this->ledWorker1->fade(500, 10);
    //digitalWrite(D3, LOW);
    //ledController->fadeOut(1)
    //ledController->fadeOut(2)
    //ledController->fadeOut(3)
  }

  // if there is a proximity detected, we do light up all leds with a fade include

  // if the proximity is disappeared we wait a little bit and fade out all leds
}


void CY8TouchSwitch::gestureEvent(uint8_t _event)
{
}


void CY8TouchSwitch::interrupt()
{
  this->touchController->interrupt();
}


void CY8TouchSwitch::loop()
{
  // the touch controler needs a loop trigger for working correctly
  // it will handle the touches and will raise the events we do need
  this->touchController->loop();
  // now lets handle the leds output.
  // there are some options like fadeIn, fadeOut, StandbyLight, ProgrammingMode, ErrorMode...
  this->ledWorker1->loop();
}


