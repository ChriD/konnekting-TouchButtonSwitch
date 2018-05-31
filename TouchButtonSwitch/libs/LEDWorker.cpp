/*
  LEDWorker.cpp
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "LEDWorker.h"

LEDWorker::LEDWorker(uint8_t _ledPin)
{
  this->ledPin            = _ledPin;
  this->ledValue          = 0;
  this->currentMode       = LW_MODE_IDLE;
  this->modeStartTime     = 0;
  this->modeLastCallTime  = 0;
}


LEDWorker::~LEDWorker()
{

}

void LEDWorker::setup()
{
   pinMode(this->ledPin, OUTPUT);
}

void LEDWorker::fade(uint16_t _duration, uint8_t _toValue)
{
  // calculate the call period for the fade mode
  int16_t valueDiff = (_toValue - this->ledValue);
  this->mode_fade_callPeriod  = _duration / abs(valueDiff);
  this->mode_fade_toValue     = _toValue;

  this->modeStartTime = millis();
  this->currentMode = LW_MODE_FADE;
}


void LEDWorker::blink(uint16_t _lowPeriod, uint16_t _highPeriod, uint8_t _lowValue, uint8_t _highValue)
{
  this->mode_blink_lowPeriod  = _lowPeriod;
  this->mode_blink_highPeriod = _highPeriod;
  this->mode_blink_lowValue   = _lowValue;
  this->mode_blink_highValue  = _highValue;

  this->modeStartTime = millis();
  this->currentMode = LW_MODE_BLINK;
}


bool LEDWorker::processMode()
{
  switch(this->currentMode)
  {
    case LW_MODE_FADE:
      return this->processMode_Fade();
    case LW_MODE_IDLE:
      return false;
    default:
      return false;
  }
}



bool LEDWorker::processMode_Fade()
{
  uint16_t period = this->getProcessPeriod();
  if(period >= this->mode_fade_callPeriod)
  {
    this->modeLastCallTime = millis();
    if(this->ledValue == this->mode_fade_toValue)
    {
      this->modeStartTime = 0;
      this->currentMode   = LW_MODE_IDLE;
    }
    else if(this->ledValue > this->mode_fade_toValue)
      this->ledValue -= 1;
    else
      this->ledValue += 1;
    return true;
  }
  return false;
}


bool LEDWorker::processMode_Blink()
{
  uint16_t period = this->getProcessPeriod();
  if( this->mode_blink_ledStatus == HIGH && period >= this->mode_blink_highPeriod ||
      this->mode_blink_ledStatus == LOW && period >= this->mode_blink_lowPeriod )
  {
    this->modeLastCallTime = millis();
    if(this->mode_blink_ledStatus == HIGH)
      this->ledValue = this->mode_blink_lowValue;
    else
      this->ledValue = this->mode_blink_highValue;
    this->mode_blink_ledStatus = !this->mode_blink_ledStatus;
    return true;
  }
  return false;

}


uint16_t LEDWorker::getProcessPeriod()
{
  uint64_t cur = millis();
  // if we had an overflow we have to calculate the period in a special way
  // in fact we have to add "(max)uint16_t - LastCallTime"  to  "cur"
  // but ofr our purpose its okay to only return "cur".
  if(cur < this->modeLastCallTime)
  {
    return (uint16_t)(cur + (UINT64_MAX - this->modeLastCallTime));
  }
  else
  {
    return (uint16_t)(cur - this->modeLastCallTime);
  }
}



void LEDWorker::loop()
{
  // process the mode if there is one
  // the method will update the "ledValue" variable to the value it should have for the mode
  if(this->processMode())
  {
    analogWrite(this->ledPin, this->ledValue);
  }
}