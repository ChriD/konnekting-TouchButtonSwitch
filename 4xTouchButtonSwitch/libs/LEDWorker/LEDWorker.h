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

    #define LW_MODE_IDLE            0
    #define LW_MODE_FADE            10

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
        void blink(uint16_t _blinkPeriod = 500, uint8_t _lowValue = 0, uint8_t _highValue = 255);
        // off
        // pulse
      private:
        uint8_t ledPin;
        uint8_t ledValue;
        uint8_t currentMode;
        uint_least64_t modeStartTime;
        uint_least64_t modeLastCallTime;

        uint_least16_t mode_fade_callPeriod;
        uint_least16_t mode_fade_toValue;



    };

#endif