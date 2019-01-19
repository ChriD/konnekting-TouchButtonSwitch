/*
  TouchButton.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


/*
TODO: * add a noiso to tap ratio?! calculated by the  base noise value?
      * Maybe check if ADCTouch library does perform well tool? (http://playground.arduino.cc/Code/ADCTouch)
*/


#ifndef TouchButton_h
#define TouchButton_h

  #include "Button.h"
  #include "Arduino.h"
  #include "Adafruit_FreeTouch.h"

  #define BTN_STD_TOUCH_NOISETOTAP_GAP  10

  enum class TOUCHBUTTON_MODE { NORMAL = 0, CALIBRATION = 10, DISABLED = 99};

  class TouchButton : public Button
  {
    public:
      TouchButton(uint8_t _pin, uint16_t _id = 0);
      ~TouchButton();

      virtual boolean setup();
      virtual void task();

      virtual void startCalibration();
      virtual uint16_t getLastSampleValue();
      virtual uint16_t getTriggerLevel();

      // baseNoiseOffsetValue parm methods
      uint16_t parmBaseNoiseOffsetValue();
      void parmBaseNoiseOffsetValue(uint16_t);

    protected:
      // the pin where the touch button is beeing set up
      uint8_t pin;

      // button mode
      TOUCHBUTTON_MODE mode;

      // calibration stuff
      uint16_t calibrationSampleCount;
      uint16_t calibrationSamplePeriod;
      uint16_t calibrationSampleNumber;
      uint16_t calibrationSampleValueSum;
      uint64_t calibrationSampleTime;

      // noise levels, offsets and trigger level for recognizing a touch
      // the baseNoiseOffsetValue will be used to calculate the trigger level for a touch
      uint16_t baseNoiseLevel;
      uint16_t baseNoiseOffsetValue;
      uint16_t baseNoiseMinLevel;
      uint16_t baseNoiseMaxLevel;
      uint16_t triggerLevel;

      uint16_t lastSampleValue;

      // the library for utilizing the qtouch chip on the samd21 device
      // unfortunatelly it's a very rudamentary library and does not use full capacity of the qtouch lib (with interrupts and calibrations...)
      // I hope in future someone will add those functionality or the qtouch source will become available!
      // Description:
      //http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42230-QTouch-Safety-Library-Peripheral-Touch-Controller_User-Guide.pdf
      Adafruit_FreeTouch qt;

      virtual int8_t calcButtonState();
      virtual void calcTriggerLevel();
      virtual void resetCalibrationInternals();
    };

#endif