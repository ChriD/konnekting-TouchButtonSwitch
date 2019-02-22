/*
  TouchSwitch_4X_V1.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchSwitch_1X_V1_h
#define TouchSwitch_1X_V1_h

  #include "Arduino.h"
  #include "TouchSwitch.h"
  #include "vendor/LedPattern_Mono.h"
  #include "LedPatterns.h"


  #define TS_1X_V1_BTN1_ID    1
  #define TS_1X_V1_PROX1_ID   10
  #define TS_1X_V1_PROX2_ID   11

  #define TS_1X_V1_BTN1_PIN   A1
  #define TS_1X_V1_PROX1_PIN  A2
  #define TS_1X_V1_PROX2_PIN  A4

  #define TS_1X_V1_LED_COUNT  1



  class TouchSwitch_1X_V1 : public TouchSwitch
  {
    public:
      TouchSwitch_1X_V1();
      ~TouchSwitch_1X_V1();

      virtual void initButtons();
      virtual void setMode(SWITCH_MODE, uint16_t _modeLevel = 0);
      virtual void task();

    protected:
      LedPattern_Mono *led1;
      uint64_t lastPatternRunTime;

      virtual void onButtonAction(uint16_t, uint16_t, uint16_t);

    private:


  };

#endif