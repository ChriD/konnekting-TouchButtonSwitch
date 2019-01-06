/*
  ProximityTouchButton.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "ProximityTouchButton.h"

/*
  TODO: * check out proximity banging issue (3/0/3/0/3/0....)
        * redesign proximity level
*/


ProximityTouchButton::ProximityTouchButton(uint8_t _pin, uint16_t _id) : TouchButton(_pin, _id)
{
  this->isProximity = false;
  this->isProximityCur = false;
  this->isProximityLast = false;
  this->proximityLevel = 0;
  this->callback_onProximityAlert = NULL;
  this->isStableProximityCount = 0;
  this->isStableProximity = false;
}


ProximityTouchButton::~ProximityTouchButton()
{
}



void ProximityTouchButton::task()
{
  TouchButton::task();

  if(this->mode == TOUCHBUTTON_MODE::NORMAL)
  {
    uint16_t proximityValue = 0;
    uint16_t proximityLevel = this->proximityLevel;

    // check if we have some proximity detected, to detect proximity we should have at least a view ticks with proximity in a row
    if(this->lastSampleValue > (this->baseNoiseMaxLevel + 2)) // TODO: @@@ noiseOffset?!
      proximityValue = this->lastSampleValue - this->baseNoiseMaxLevel;
    this->isProximityCur = proximityValue > 0;

    // check if we had a stable proximity marker for a view ticks
    if(this->isStableProximityCount < 3)
    {
      this->isStableProximity = this->isProximityCur == this->isProximityLast;
      if(!this->isStableProximity)
        this->isStableProximityCount = 0;
      else
        this->isStableProximityCount++;
    }
    else
    {
      this->isStableProximityCount = 0;

      //////////////////////////////
      // TODO: do a better one :-)
          if(proximityValue > 50)
            proximityLevel = 10;
          else if(proximityValue > 40)
            proximityLevel = 8;
          else if(proximityValue > 20)
            proximityLevel = 6;
          else if(proximityValue > 10)
            proximityLevel = 4;
          else if(proximityValue > 5)
            proximityLevel = 2;
          else if(proximityValue > 0)
            proximityLevel = 1;
          else
            proximityLevel = 0;

          // if no levels are allowed, the level can only be 0 or 1
          if(!this->allowProximityLevels && proximityLevel > 0)
            proximityLevel  = 1;
      //////////////////////////////

    }

    this->isProximityLast = this->isProximityCur;

    if(this->proximityLevel != proximityLevel)
    {
      this->isProximity     = proximityLevel > 0 ? true : false;
      this->proximityLevel  = proximityLevel;
      if(this->callback_onProximityAlert)
        this->callback_onProximityAlert(this->id, this->isProximity, proximityValue, this->proximityLevel);
    }

  }


}

// TODO: add Task, call super and check for proximity after super (we can use 'lastSampleValue' for that)
// add some proximity levels (e.g 5% above highest is level 1, 7% is level 2, aso...)

// setup the callback onProximity(_id, _level)

  //--> so In fact all buttons can have the ability of proximity detection which is good?

// TODO: and / or create own class for proximity sensor?!

