/*
  TouchSwitch_4X_V1.h
  Created by Christian Dürnberger, 2019
  Released into the public domain.
*/


#ifndef TouchSwitch_4X_V1_h
#define TouchSwitch_4X_V1_h

  #include "Arduino.h"
  #include "TouchSwitch.h"
  //#include "LEDWorker.h"
  #include "vendor/LedPattern_Mono.h"


  #define TS_4X_V1_BTN1_ID    1
  #define TS_4X_V1_BTN2_ID    2
  #define TS_4X_V1_BTN3_ID    3
  #define TS_4X_V1_BTN4_ID    4
  #define TS_4X_V1_PROX1_ID   10
  #define TS_4X_V1_PROX2_ID   11

  #define TS_4X_V1_BTN1_PIN   A1
  #define TS_4X_V1_BTN2_PIN   A0
  #define TS_4X_V1_BTN3_PIN   A5
  #define TS_4X_V1_BTN4_PIN   A3
  #define TS_4X_V1_PROX1_PIN  A2
  #define TS_4X_V1_PROX2_PIN  A4


// Define the pattern: A repeating sequence of fading the LED to on
// in 50 cycles, then fading back to off in 100 cycles.
const uint8_t myPattern[] = {
    LedPattern::CMD_SET, 10,
    LedPattern::CMD_REPEAT, LedPattern::repeatForever,
        LedPattern::CMD_FADETO,  25, 150,
        LedPattern::CMD_FADETO, 50, 10,
    LedPattern::CMD_ENDREPEAT
};


  class TouchSwitch_4X_V1 : public TouchSwitch
  {
    public:
      TouchSwitch_4X_V1();
      ~TouchSwitch_4X_V1();

      virtual void initButtons();
      virtual void setMode(SWITCH_MODE, uint16_t _modeLevel = 0);
      virtual void task();

    protected:
      //LEDWorker   *ledWorkers[4];
      //AlaLed  leds;
      //byte    ledPins[3];
      LedPattern_Mono *pattern;
      uint64_t lastPatternRunTime;

    private:


  };

#endif