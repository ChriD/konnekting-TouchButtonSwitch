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
  this->currentMode       = LW_MODE_IDLE
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


void LEDWorker::blink(uint16_t _blinkPeriod, uint8_t _lowValue, uint8_t _highValue)
{
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
  if(millis() - this->modeLastCallTime >= this->mode_fade_callPeriod)
  {
    Serial.print("\n");
    Serial.print(this->ledValue);
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