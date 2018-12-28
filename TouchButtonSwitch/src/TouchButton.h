/*
  TouchButton.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchButton_h
#define TouchButton_h

  #include "Button.h"
  #include "Arduino.h"
  #include "Adafruit_FreeTouch.h"

  #define BUTTON_MODE_NORMAL        0
  #define BUTTON_MODE_CALIBRATION   10
  #define BUTTON_MODE_LOCKED        50

  class TouchButton : public Button
  {
    public:
      TouchButton(uint8_t _pin);
      ~TouchButton();

      virtual boolean setup();
      virtual void task();

      virtual void startCalibration();
      virtual uint16_t getLastSampleValue();
      virtual uint16_t getTriggerLevel();

    protected:
      // the pin where the touch button is beeing set up
      uint8_t pin;

      // button mode
      uint8_t mode;

      // calibration stuff
      uint16_t calibrationSampleCount;
      uint16_t calibrationSamplePeriod;
      uint16_t calibrationSampleNumber;
      uint16_t calibrationSampleValueSum;
      uint64_t calibrationSampleTime;

      // noise levels, offsets and trigger level for recognizing a touch
      uint16_t baseNoiseLevel;
      uint16_t baseNoiseOffsetValue;
      uint16_t triggerLevel;

      //uint8_t

      uint16_t lastSampleValue;
      //
      Adafruit_FreeTouch qt;

      virtual int8_t calcButtonState();
      virtual void calcTriggerLevel();
      virtual void resetCalibrationInternals();
    };

#endif