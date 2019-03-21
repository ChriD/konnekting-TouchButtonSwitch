/*
  TouchSwitch_5X_V1.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchSwitch_5X_V1_h
#define TouchSwitch_5X_V1_h

  #include "Arduino.h"
  #include "TouchSwitch.h"
  #include "vendor/LedPattern_RGB.h"  // https://github.com/Yveaux/LedPattern
  #include "LedPatternsRGB.h"
  #include <BME280I2C.h>              // https://github.com/finitespace/BME280
  #include <Wire.h>


  class TouchSwitch_5X_V1 : public TouchSwitch
  {
    public:
      TouchSwitch_5X_V1();
      ~TouchSwitch_5X_V1();

      virtual void initButtons();
      virtual void setMode(SWITCH_MODE, uint16_t _modeLevel = 0);
      virtual boolean setup();
      virtual void task();

      virtual void parmLightningSettings(BaseSwitchLightning);

    protected:
      LedPattern_RGB *rgbLed;
      uint64_t lastPatternRunTime;

      BME280I2C bme;  // Default : forced mode, standby time = 1000 ms
                      // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
      uint64_t lastEnvSenorsRunTime;

      virtual void onButtonAction(uint16_t, uint16_t, uint16_t);

    private:
      uint8_t ledPattern_Default[9];


  };

#endif