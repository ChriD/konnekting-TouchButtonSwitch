/*
  TouchSwitch_5X_V1.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchSwitch_5X_V1_h
#define TouchSwitch_5X_V1_h

  #include "Arduino.h"
  #include "TouchSwitch.h"
  #include "vendor/LedPattern_RGB.h"
  #include "LedPatternsRGB.h"


  class TouchSwitch_5X_V1 : public TouchSwitch
  {
    public:
      TouchSwitch_5X_V1();
      ~TouchSwitch_5X_V1();

      virtual void initButtons();
      virtual void setMode(SWITCH_MODE, uint16_t _modeLevel = 0);
      virtual void task();

    protected:
      LedPattern_RGB *rgbLed;
      uint64_t lastPatternRunTime;

      virtual void onButtonAction(uint16_t, uint16_t, uint16_t);

    private:


  };

#endif