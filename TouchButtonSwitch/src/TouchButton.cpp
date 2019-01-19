/*
  TouchButton.cpp
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/

#include "Arduino.h"
#include "TouchButton.h"


TouchButton::TouchButton(uint8_t _pin, uint16_t _id) : Button()
{
  this->pin   = _pin;
  this->mode  =  TOUCHBUTTON_MODE::DISABLED;

  this->parmId(_id);
  this->parmBaseNoiseOffsetValue(0);

  // standard calibration values will lead to a 250ms calibration with 25 samples
  // that is good in many cases
  this->calibrationSampleNumber   = 25;
  this->calibrationSamplePeriod   = 5;


  this->calcTriggerLevel();
}


TouchButton::~TouchButton()
{
}


void TouchButton::parmBaseNoiseOffsetValue(uint16_t _baseNoiseOffsetValue){
  this->baseNoiseOffsetValue = _baseNoiseOffsetValue;
}
uint16_t TouchButton::parmBaseNoiseOffsetValue(){
  return this->baseNoiseOffsetValue;
}


boolean TouchButton::setup()
{
  // be sure we call the super method of setup, just in case the base method needs to setup any basic stuff
  Button::setup();

  /*
  Frequency mode
  allows users to configure the bursting waveform characteristics for better noise performance in the
  system. Following frequency modes are available:
  FREQ_MODE_NONE - Frequency mode is disabled
  FREQ_MODE_HOP - Frequency mode hopping
  FREQ_MODE_SPREAD - Frequency mode spread
  FREQ_MODE_SPREAD_MEDIAN - Frequency mode spread median

  Qtouch is described here:
  http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42230-QTouch-Safety-Library-Peripheral-Touch-Controller_User-Guide.pdf
  */

  // create the SAMD touch button instance from adafruit touch library with some standard values
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
  this->baseNoiseMinLevel         = 0;
  this->baseNoiseMaxLevel         = 0;
}

void TouchButton::startCalibration()
{
  // reset some internal values needed for calibration
  this->resetCalibrationInternals();
  // we only need to set the button mode to calibration, the calibration stuff will
  // be done by the 'task ' method
  this->mode = TOUCHBUTTON_MODE::CALIBRATION;
}


void TouchButton::calcTriggerLevel()
{
  // the trigger level has to be at least the maximum level of noise with a little gap and the desired offset set by the user
  this->triggerLevel = this->baseNoiseMaxLevel + BTN_STD_TOUCH_NOISETOTAP_GAP + this->baseNoiseOffsetValue;
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
  if(this->mode == TOUCHBUTTON_MODE::CALIBRATION)
  {
    if(this->getPeriod(this->calibrationSampleTime) >= this->calibrationSamplePeriod)
    {
      // calculate min and maximum nois values
      uint16_t value = qt.measure();
      if(this->baseNoiseMinLevel == 0 || ( value > 0 && value < this->baseNoiseMinLevel))
        this->baseNoiseMinLevel = value;
      if(this->baseNoiseMaxLevel == 0 || ( value > 0 && value > this->baseNoiseMaxLevel))
        this->baseNoiseMaxLevel = value;
      // sum up noise values
      this->calibrationSampleCount++;
      this->calibrationSampleValueSum += value;
      this->calibrationSampleTime = millis();
      // if we have processed all calibration samples we calulate the base noise of the button and set back the mode to 'NORMAL'
      if(this->calibrationSampleCount >= this->calibrationSampleNumber)
      {
        this->baseNoiseLevel = this->calibrationSampleValueSum / this->calibrationSampleCount;
        this->calcTriggerLevel();
        this->mode = TOUCHBUTTON_MODE::NORMAL;
      }
    }
  }
  //  the button may be disabled, do nothing when its disables!
  else if(this->mode == TOUCHBUTTON_MODE::DISABLED)
  {
  }
  // let the base class do it's job, of dourse only if we are not in calibration or disabled mode
  else
  {
    Button::task();
  }
}