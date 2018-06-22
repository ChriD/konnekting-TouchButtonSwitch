/*
  LEDWorker.cpp
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

#include "Arduino.h"
#include "LEDWorker.h"

LEDWorker::LEDWorker(uint8_t _ledPin, bool _useSoftPWM)
{
  this->ledPin              = _ledPin;
  this->ledValue            = 0;
  this->currentMode         = LW_MODE_IDLE;
  this->modeStartTime       = 0;
  this->modeLastCallTime    = 0;
  this->taskLastCallTime    = 0;

  this->useSoftPWM          = _useSoftPWM;
  this->softPWMPeriod       = 0;
  this->softPWMValue        = 0;

}


LEDWorker::~LEDWorker()
{

}

void LEDWorker::setup()
{
   pinMode(this->ledPin, OUTPUT);
   //pinMode(this->ledPin, PWM);
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
  this->mode_blink_ledStatus  = false;

  this->modeStartTime = millis();
  this->currentMode = LW_MODE_BLINK;
}


void LEDWorker::set(uint8_t _value)
{
   analogWrite(this->ledPin, _value);
   this->ledValue = _value;
}


bool LEDWorker::processMode()
{
  switch(this->currentMode)
  {
    case LW_MODE_FADE:
      return this->processMode_Fade();
    case LW_MODE_BLINK:
      return this->processMode_Blink();
    case LW_MODE_IDLE:
      return false;
    default:
      return false;
  }
}



bool LEDWorker::processMode_Fade()
{
  uint16_t period = this->getProcessPeriod(this->modeLastCallTime);
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
  uint16_t period = this->getProcessPeriod(this->modeLastCallTime);
  if( this->mode_blink_ledStatus == true && period >= this->mode_blink_highPeriod ||
      this->mode_blink_ledStatus == false && period >= this->mode_blink_lowPeriod )
  {
    this->modeLastCallTime = millis();
    if(this->mode_blink_ledStatus == true)
      this->ledValue = this->mode_blink_lowValue;
    else
      this->ledValue = this->mode_blink_highValue;
    this->mode_blink_ledStatus = !this->mode_blink_ledStatus;
    return true;
  }
  return false;
}


uint16_t LEDWorker::getProcessPeriod(uint64_t _lastCallTime, bool _useMicros)
{
  uint64_t cur;

  if(_useMicros)
    cur = micros();
  else
    cur = millis();
  // if we had an overflow we have to calculate the period in a special way
  // in fact we have to add "(max)uint16_t - LastCallTime"  to  "cur"
  // but ofr our purpose its okay to only return "cur".
  if(cur < _lastCallTime)
  {
    return (uint16_t)(cur + (UINT64_MAX - _lastCallTime));
  }
  else
  {
    return (uint16_t)(cur - _lastCallTime);
  }
}



void LEDWorker::task()
{
  // process the mode if there is one
  // the method will update the "ledValue" variable to the value it should have for the mode
  if(this->processMode())
  {
    // for analog write the pin has to be a PWM pin
    if(!this->useSoftPWM)
      analogWrite(this->ledPin, this->ledValue);
  }
  else
  {
    if(this->useSoftPWM)
    {
      uint16_t period = this->getProcessPeriod(this->taskLastCallTime, true);

      // TODO: @@ calc on value set!
      //this->softPWMPeriod =
      //10 127
      if( period >= this->softPWMPeriod )
      {
        // TODO: use SoftPWM Lib? (uses timers and interrupts, would be better?)
        // TODO: use native pin set (much faster) gpio_write_bit(GPIOB, 0, LOW);
        // TODO: generate PWM signal on pin for given value (SOFT PWM)
        // TODO: calculate pwm period
        this->softPWMValue != this->softPWMValue;
        digitalWrite(this->ledPin, this->softPWMValue);
      }
    }
  }
  this->taskLastCallTime = micros();
}