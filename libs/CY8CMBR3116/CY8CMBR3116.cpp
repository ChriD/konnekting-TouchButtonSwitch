/*
  CY8MBR3116.cpp Library for controlling/handling the Cypress CY8MBR3116 touch controller
  Created by Christian Dürnberger, June 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include <Wire.h>
#include "CY8CMBR3116.h"


CY8CMBR3116::CY8CMBR3116(uint8_t _I2CAddress)
{
  this->I2CAddress              = _I2CAddress;
  this->longTouchThreshold      = 1250; // [milli]
  this->touchThreshold          = 250;  // [milli]
  this->loopTriggerGap          = 5000; // [micro]
  this->prevButtonStatus        = 0;
  this->prevProximityStatus     = 0;
}


CY8CMBR3116::~CY8CMBR3116()
{

}


// uint8_t _sensoryType, uint8_t _sensorId, bool _value
void CY8CMBR3116::setSensorStateCallback(std::function<void(uint8_t, uint8_t, bool)> _sensorStateChangedCallback)
{
  this->sensorStateChangedCallback = _sensorStateChangedCallback;
}

// uint8_t _sensorId, uint8_t _event, uint8_t _count
void CY8CMBR3116::setTouchEventCallback(std::function<void(uint8_t, uint8_t, uint8_t)> _touchEventCallback)
{
  this->touchEventCallback = _touchEventCallback;
}

// uint8_t _sensorId, uint8_t _event
void CY8CMBR3116::setProximityEventCallback(std::function<void(uint8_t, uint8_t)> _proximityEventCallback)
{
  this->proximityEventCallback = _proximityEventCallback;
}

// uint8_t _gesture
void CY8CMBR3116::setGestureEventCallback(std::function<void(uint8_t)> _gestureEventCallback)
{
  this->gestureEventCallback = _gestureEventCallback;
}


void CY8CMBR3116::setThresholds(uint16_t _touchThreshold, uint16_t _longTouchThreshold)
{
  this->longTouchThreshold = _longTouchThreshold;
  this->touchThreshold     = _touchThreshold;
}


void CY8CMBR3116::setup()
{
  // TODO: @@@
}


void CY8CMBR3116::interrupt()
{
  this->process();
}


void CY8CMBR3116::process()
{
  Serial.print("\nInterrupt!");

  // when we got an interrupt we do have new touch data available (something has changed)
  // so we can read it from the approriate register value over I2C
  uint8_t buttonStatus[2], proximityStatus[1];

  // first read the button states. This is a 2 bytes value
  this->readData(CY8_BUTTON_STATUS, 2, buttonStatus);
  uint16_t curButtonStatus = ((buttonStatus[1]<<8) | buttonStatus[0]);
  for(uint8_t i=0; i<16; i++)
  {
    bool prevState  = bitRead(this->prevButtonStatus, i);
    bool curState   = bitRead(curButtonStatus, i);
    if(prevState != curState)
        this->sensorStateChanged(0, i, curState);
  }
  this->prevButtonStatus = curButtonStatus;

  // read the proximity states
  this->readData(CY8_PROX_STAT, 1, proximityStatus);
  uint8_t curProximityStatus = proximityStatus[0];
  for(uint8_t i=0; i<8; i++)
  {
    bool prevState  = bitRead(this->prevProximityStatus, i);
    bool curState   = bitRead(curProximityStatus, i);
    if(prevState != curState)
        this->sensorStateChanged(1, i, curState);
  }
  this->prevProximityStatus = curProximityStatus;
}


uint16_t CY8CMBR3116::readData(uint8_t _register, uint8_t _length, uint8_t* _data)
{
  uint16_t bytes_received = 0;
  uint16_t remaining = _length;
  uint8_t* next = _data;

  Wire.beginTransmission(this->I2CAddress);
  Wire.write(_register);
  Wire.endTransmission();

  Wire.requestFrom(this->I2CAddress, _length);
  while (Wire.available() && remaining--)
  {
    *next++ = Wire.read();
    ++bytes_received;
  }

  return bytes_received;
}


void CY8CMBR3116::sensorStateChanged(uint8_t _sensoryType, uint8_t _sensorId, bool _value)
{
  // call callback method if registered
  if(this->sensorStateChangedCallback)
    this->sensorStateChangedCallback(_sensoryType, _sensorId, _value);

  // on non proximity sensors we want to handle the touch, long touch and double touch event
  // for this we have to store some timing data, when touch appears and disappears
  if(_sensoryType == 0)
  {
    if(_value == 1)
    {
      // if we do touch we check the last touch end time. If it is in a certain amount of time and we already touched one
      //touchEndTime
      uint_least64_t lastTouchEnd = this->touchEndTime[_sensorId];
      uint_least64_t touchStart   = millis();

      // when we did have a touch on the button before and we do touch again it may be a doublecklick or tripple....
      // to be aware of this we do add the touch counter, otherwise set the counter to 1
      if(lastTouchEnd > 0 && (touchStart - lastTouchEnd <= this->touchThreshold))
      {
        this->touchCounter[_sensorId] += 1;
        this->touchEndTime[_sensorId] = 0;
      }
      else
        this->touchCounter[_sensorId] = 1;

      // store the new touch start time
      this->touchStartTime[_sensorId] = touchStart;
      // if a button state has changed we have to process the button state in the loop
      this->loopProcess[_sensorId] = true;
    }
    else
    {
        // store the end time of the touch, but only if it was no long touch before
        if(this->touchCounter[_sensorId] > 0)
        {
          uint_least64_t touchEnd   = millis();
          this->touchEndTime[_sensorId] = touchEnd;
          // if a button state has changed we have to process the button state in the loop
          this->loopProcess[_sensorId] = true;
        }
    }
  }
  // for proximity sensors we can hand off the event as it is, only for gestures we have to store some data
  else
  {
    this->proximityEventCallback(_sensorId, _value);

    // TODO: gestures
    //  if Sensor A goes high, and then sensor c comes high and then sensor A is of and then sensor B, its a swipe down

  }
}


// loop entry for checking clicks
void CY8CMBR3116::loop()
{

  //no need to do this every ms. It should be adequate ~ 10ms
  if(this->loopTriggerGap = 0 || (micros() - this->loopLastRunTimeStop > this->loopTriggerGap ))
  {

    this->loopLastRunTimeStart = micros();
    // we have to check "released" buttons with a touch counter
    for(uint8_t i=0; i<TOUCHSENSORCOUNT; i++)
    {
      if(this->loopProcess[i])
      {
        // if the button is currently touched and it's touched beyond the long touch limit, then its a long touch
        // is this the case we have to remove the button from the process loop and do reset the touch counter
        if((millis() - this->touchStartTime[i]) >= this->longTouchThreshold)
        {
          // call callback method if registered
          if(this->touchEventCallback)
            this->touchEventCallback(i, 2, 1);

          this->touchStartTime[i] = 0;
          this->touchEndTime[i] = 0;
          this->touchCounter[i] = 0;
          this->loopProcess[i]  = false;
        }

        // if the treshhold for a click (time for waiting if there appears another click) is done
        // then we can assume the click, doubleclick aso... is done
        if(this->touchEndTime[i] > 0 && (millis() - this->touchEndTime[i]) >= this->touchThreshold)
        {
          // call callback method if registered
          if(this->touchEventCallback)
            this->touchEventCallback(i, 1, this->touchCounter[i]);

          this->touchStartTime[i] = 0;
          this->touchEndTime[i] = 0;
          this->touchCounter[i] = 0;
          this->loopProcess[i]  = false;
        }
      }
    }
    this->loopLastRunTimeStop = micros();
    //Serial.print("\nLoop duration: ");
    //Serial.print((this->loopLastRunTimeStop - this->loopLastRunTimeStart));
  }
}