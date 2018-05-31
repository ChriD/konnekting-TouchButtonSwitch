/*
  LEDWorker.h
  Created by Christian Dürnberger, Mai 2018
  Released into the public domain.
*/

/* TODO:  * add temperature and humidity sensor
          * */

#ifndef LEDWorker_h
#define LEDWorker_h

    #include "Arduino.h"
    #include "DebugUtil.h"

    #define LW_MODE_IDLE            0
    #define LW_MODE_FADE            10
    #define LW_MODE_BLINK           20

    class LEDWorker
    {
      public:
        LEDWorker(uint8_t _ledPin);
        ~LEDWorker();
        void loop();
        void setup();
        bool processMode();
        bool processMode_Fade();
        bool processMode_Blink();
        void fade(uint16_t _duration, uint8_t _toValue);
        void blink(uint16_t _lowPeriod = 500, uint16_t _highPeriod = 500, uint8_t _lowValue = 0, uint8_t _highValue = 255);
        // off
        // pulse
      protected:
        uint16_t getProcessPeriod();
      private:
        uint8_t   ledPin;
        uint8_t   ledValue;
        uint8_t   currentMode;
        uint64_t  modeStartTime;
        uint64_t  modeLastCallTime;

        uint16_t  mode_fade_callPeriod;
        uint16_t  mode_fade_toValue;

        uint16_t  mode_blink_lowPeriod;
        uint16_t  mode_blink_highPeriod;
        uint8_t   mode_blink_lowValue;
        uint8_t   mode_blink_highValue;
        bool      mode_blink_ledStatus;
    };

#endif