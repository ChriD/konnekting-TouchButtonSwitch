/*
  Button.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "TouchButton.h"


TouchButton::TouchButton(uint8_t _pin) : Button()
{
  this->pin   = _pin;
  this->mode  =  BUTTON_MODE_NORMAL;

  // standard calibration values will lead to a 250ms calibration with 25 samples
  this->calibrationSampleNumber   = 25;
  this->calibrationSamplePeriod   = 10;
  //  the offset to the noise
  this->baseNoiseOffsetValue      = 75;

  this->calcTriggerLevel();
}


TouchButton::~TouchButton()
{
}


boolean TouchButton::setup()
{
  // be sure we call the super method of setup, just in case the base method needs to setup any basic stuff
  Button::setup();
  // create the SAMD touch button insztance from adafruit touch library with some standard values
  this->qt = Adafruit_FreeTouch(this->pin, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
  if(!this->qt.begin())
    return false;
  return true;
}


void TouchButton::resetCalibrationInternals()
{
  this->calibrationSampleCount    = 0;
  this->calibrationSampleValueSum = 0;
  this->calibrationSampleTime     = 0;
}

void TouchButton::startCalibration()
{
  // we only need to set the button mode to calibration, the calibration stuff will
  // be done by the 'task ' method
  this->mode = BUTTON_MODE_CALIBRATION;
}


void TouchButton::calcTriggerLevel()
{
  this->triggerLevel = this->baseNoiseLevel + this->baseNoiseOffsetValue;
}


uint16_t TouchButton::getLastSampleValue()
{
  return this->lastSampleValue;
}


uint16_t TouchButton::getTriggerLevel()
{
  return this->triggerLevel;
}


int8_t TouchButton::calcButtonState()
{
    // if there was no calibration done, we can not identify the state
    if(this->baseNoiseLevel <= 0)
      return 0;

    this->lastSampleValue = qt.measure();
    if(this->lastSampleValue > this->triggerLevel)
      return 1;
    return 0;
}


void TouchButton::task()
{
  // if we are in calibration mode we do have to get the measurement over a certain amount of time
  // the average of those values is our base trigger value. It's a very simple type of calibration
  if(this->mode == BUTTON_MODE_CALIBRATION)
  {
    if(this->getPeriod(this->calibrationSampleTime) >= this->calibrationSamplePeriod)
    {
      // sum up noise values
      this->calibrationSampleCount++;
      this->calibrationSampleValueSum += qt.measure();
      this->calibrationSampleTime = millis();
      // if we have processed all calibration samples we calulate the base noise of the button and set back the mode to 'NORMAL'
      if(this->calibrationSampleCount >= this->calibrationSampleNumber)
      {
        this->baseNoiseLevel = this->calibrationSampleValueSum / this->calibrationSampleCount;
        this->calcTriggerLevel();
        this->resetCalibrationInternals();
        this->mode = BUTTON_MODE_NORMAL;
      }
    }
  }
  // let the base class do it's job, of dourse only if we are not in calibration mode
  else
  {
    Button::task();
  }
}